#include "tclopencv.h"


/*
 * OpenCV FastFeatureDetector uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::FastFeatureDetector > fastdetector;

/*
 * OpenCV ORB uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::ORB > orbdetector;

/*
 * OpenCV BFMatcher uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::BFMatcher > bfmatcher;

/*
 * OpenCV SimpleBlobDetector uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::SimpleBlobDetector > simpleBlobDetector;

#ifdef __cplusplus
extern "C" {
#endif

int drawKeypoints(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat dst_image;
    std::vector< cv::KeyPoint > keypoints;
    int count = 0, flags = 0, new_B = 0, new_G = 0, new_R = 0, new_A = 0;
    int nodstimage = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *dstinfo;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix keypoints dst_image color_list ?flags?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "drawKeypoints invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "drawKeypoints invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawKeypoints invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0) {
        Tcl_SetResult(interp, (char *) "drawKeypoints keypoints data is empty", TCL_STATIC);
        return TCL_ERROR;
    } else {
        for (int i = 0; i < count; i++) {
            Tcl_Obj *elemListPtr = NULL;
            int sub_count = 0;
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);

            if(Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "drawKeypoints invalid keypoints data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (sub_count != 7) {
                Tcl_SetResult(interp, (char *) "drawKeypoints wrong keypoints number", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                double x, y, size, angle, response;
                int octave, class_id;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                    return TCL_ERROR;
                }

                cv::KeyPoint keypoint(cv::Point2f( (float) x, (float)y), (float) size,
                                      (float) angle, (float) response,
                                      octave, class_id);

                keypoints.push_back(keypoint);
            }
        }
    }

    /*
     * If users give the handle to "None", then setup a nodstimage to indicate
     * this thing. This flag is used to decide the argument.
     */
    bhandle = Tcl_GetStringFromObj(objv[3], 0);
    if (strcmp(bhandle, "None")==0) {
        nodstimage = 1;
    } else {
        nodstimage = 0;

        hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
        if( !hashEntryPtr ) {
            if( interp ) {
                Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                Tcl_AppendStringsToObj( resultObj, "drawKeypoints invalid MATRIX handle ",
                                        bhandle, (char *)NULL );
            }

            return TCL_ERROR;
        }

        info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
        if ( !info2 ) {
            Tcl_SetResult(interp, (char *) "drawKeypoints invalid info data", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    if(Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawKeypoints invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "drawKeypoints invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[4], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &new_B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &new_G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &new_R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &new_A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[5], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(new_B, new_G, new_R, new_A);

        if (nodstimage == 0 && (flags & (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG)==1) {
                dst_image = *(info2->matrix);
                cv::drawKeypoints(*(info1->matrix), keypoints, dst_image, color,
                                  (cv::DrawMatchesFlags) flags);
        } else if (nodstimage == 1 && (flags & (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG)==0) {
                cv::drawKeypoints(*(info1->matrix), keypoints, dst_image, color,
                                  (cv::DrawMatchesFlags) flags);
        } else {
            Tcl_SetResult(interp, (char *) "drawKeypoints arguments is invalid", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "drawKeypoints failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "drawKeypoints malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dst_image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, dstinfo);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int drawMatches(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat mask, dst_image;
    std::vector< cv::KeyPoint > keypoints1, keypoints2;
    std::vector< cv::DMatch > matches1to2;
    int count = 0, flags = 0, B1 = 0, G1 = 0, R1 = 0, A1 = 0;
    int B2 = 0, G2 = 0, R2 = 0, A2 = 0;
    int nodstimage = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle, *dhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2, *info3;
    MatrixInfo *dstinfo;

    if (objc != 9 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix1 keypoints1 matrix2 keypoints2 matches1to2 dst_image matchColor_list singlePointColor_list ?flags?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "drawMatches invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0) {
        Tcl_SetResult(interp, (char *) "drawMatches keypoints data is empty", TCL_STATIC);
        return TCL_ERROR;
    } else {
        for (int i = 0; i < count; i++) {
            Tcl_Obj *elemListPtr = NULL;
            int sub_count = 0;
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);

            if(Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "drawMatches invalid keypoints data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (sub_count != 7) {
                Tcl_SetResult(interp, (char *) "drawMatches wrong keypoints number", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                double x, y, size, angle, response;
                int octave, class_id;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                    return TCL_ERROR;
                }

                cv::KeyPoint keypoint(cv::Point2f( (float) x, (float)y), (float) size,
                                      (float) angle, (float) response,
                                      octave, class_id);

                keypoints1.push_back(keypoint);
            }
        }
    }

    bhandle = Tcl_GetStringFromObj(objv[3], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "drawMatches invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0) {
        Tcl_SetResult(interp, (char *) "drawMatches keypoints data is empty", TCL_STATIC);
        return TCL_ERROR;
    } else {
        for (int i = 0; i < count; i++) {
            Tcl_Obj *elemListPtr = NULL;
            int sub_count = 0;
            Tcl_ListObjIndex(interp, objv[4], i, &elemListPtr);

            if(Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "drawMatches invalid keypoints data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (sub_count != 7) {
                Tcl_SetResult(interp, (char *) "drawMatches wrong keypoints number", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                double x, y, size, angle, response;
                int octave, class_id;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                    return TCL_ERROR;
                }

                cv::KeyPoint keypoint(cv::Point2f( (float) x, (float)y), (float) size,
                                      (float) angle, (float) response,
                                      octave, class_id);

                keypoints2.push_back(keypoint);
            }
        }
    }

    /*
     * Matches from the first image to the second one
     */
    if(Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0) {
        Tcl_SetResult(interp, (char *) "drawMatches matches1to2 data is empty", TCL_STATIC);
        return TCL_ERROR;
    } else {
        for (int i = 0; i < count; i++) {
            Tcl_Obj *elemListPtr = NULL;
            int sub_count = 0;
            Tcl_ListObjIndex(interp, objv[5], i, &elemListPtr);

            if(Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "drawMatches invalid matches1to2 data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (sub_count != 4) {
                Tcl_SetResult(interp, (char *) "drawMatches wrong matches1to2 number", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                int queryIdx, trainIdx, imgIdx;
                double distance;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &queryIdx) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &trainIdx) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                if(Tcl_GetIntFromObj(interp, elemListSubPtr, &imgIdx) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &distance) != TCL_OK) {
                    return TCL_ERROR;
                }

                cv::DMatch dmatch(queryIdx, trainIdx, imgIdx, (float) distance);
                matches1to2.push_back(dmatch);
            }
        }
    }

    /*
     * If users give the handle to "None", then setup a nodstimage to indicate
     * this thing. This flag is used to decide the argument.
     */
    dhandle = Tcl_GetStringFromObj(objv[6], 0);
    if (strcmp(dhandle, "None")==0) {
        nodstimage = 1;
    } else {
        nodstimage = 0;

        hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, dhandle );
        if( !hashEntryPtr ) {
            if( interp ) {
                Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                Tcl_AppendStringsToObj( resultObj, "drawMatches invalid MATRIX handle ",
                                        dhandle, (char *)NULL );
            }

            return TCL_ERROR;
        }

        info3 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
        if ( !info3 ) {
            Tcl_SetResult(interp, (char *) "drawMatches invalid info data", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    if(Tcl_ListObjLength(interp, objv[7], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[7], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A1) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_ListObjLength(interp, objv[8], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "drawMatches invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[8], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[8], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[8], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[8], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A2) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (objc == 10) {
        if(Tcl_GetIntFromObj(interp, objv[9], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color1(B1, G1, R1, A1);
        cv::Scalar color2(B2, G2, R2, A2);

        if (nodstimage == 0 && (flags & (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG)==1) {
                dst_image = *(info3->matrix);
                cv::drawMatches(*(info1->matrix), keypoints1, *(info2->matrix), keypoints2,
                                matches1to2, dst_image, color1, color2,
                                mask, (cv::DrawMatchesFlags) flags);
        } else if (nodstimage == 1 && (flags & (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG)==0) {
                cv::drawMatches(*(info1->matrix), keypoints1, *(info2->matrix), keypoints2,
                                matches1to2, dst_image, color1, color2,
                                mask, (cv::DrawMatchesFlags) flags);
        } else {
            Tcl_SetResult(interp, (char *) "drawMatches arguments is invalid", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "drawMatches failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "drawMatches malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dst_image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, dstinfo);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int FastFeatureDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "detect",
        "getNonmaxSuppression",
        "getThreshold",
        "getType",
        "setNonmaxSuppression",
        "setThreshold",
        "setType",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_GETNONMAXSUPPRESSION,
        FUNC_GETTHRESHOLD,
        FUNC_GETTYPE,
        FUNC_SETNONMAXSUPPRESSION,
        FUNC_SETTHRESHOLD,
        FUNC_SETTYPE,
        FUNC_CLOSE,
    };

    if( objc < 2 ){
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if( Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice) ){
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[0], 0);

    switch( (enum FUNC_enum)choice ){
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            Tcl_HashEntry *hashEntryPtr;
            char *handle;
            MatrixInfo *info;
            Tcl_Obj *pResultStr = NULL;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "detect invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "detect invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                fastdetector->detect(*(info->matrix), keypoints);
            } catch (...){
                Tcl_SetResult(interp, (char *) "detect failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.x ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.y ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].size ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].angle ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].response ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].octave ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].class_id ));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_GETNONMAXSUPPRESSION: {
            int nonmaxSuppression = 0;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                nonmaxSuppression = fastdetector->getNonmaxSuppression();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getNonmaxSuppression failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj (nonmaxSuppression) );
            break;
        }
        case FUNC_GETTHRESHOLD: {
            int threshold = 0;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                threshold = fastdetector->getThreshold();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (threshold) );
            break;
        }
        case FUNC_GETTYPE: {
            int type = 0;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                type = (int) fastdetector->getType();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (type) );
            break;
        }
        case FUNC_SETNONMAXSUPPRESSION: {
            int nonmaxSuppression = 0;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetBooleanFromObj(interp, objv[2], &nonmaxSuppression) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                fastdetector->setNonmaxSuppression(nonmaxSuppression);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setNonmaxSuppression failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_SETTHRESHOLD: {
            int threshold = 0;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &threshold) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                fastdetector->setThreshold(threshold);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_SETTYPE: {
            int type = 0;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &type) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                fastdetector->setType((cv::FastFeatureDetector::DetectorType) type);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            fastdetector.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int FastFeatureDetector(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int threshold = 10, nonmaxSuppression = 1, type = ::cv::FastFeatureDetector::TYPE_9_16;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?threshold nonmaxSuppression type?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetIntFromObj(interp, objv[1], &threshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[2], &nonmaxSuppression) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        fastdetector = cv::FastFeatureDetector::create( threshold, (bool) nonmaxSuppression,
                                                (cv::FastFeatureDetector::DetectorType) type);

        if (fastdetector == nullptr) {
            Tcl_SetResult(interp, (char *) "FastFeatureDetector create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "FastFeatureDetector failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-fastdetector", -1 );

    Tcl_CreateObjCommand(interp, "cv-fastdetector", (Tcl_ObjCmdProc *) FastFeatureDetector_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int ORB_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "detect",
        "compute",
        "detectAndCompute",
        "getEdgeThreshold",
        "getFastThreshold",
        "getFirstLevel",
        "getMaxFeatures",
        "getNLevels",
        "getPatchSize",
        "getScaleFactor",
        "getScoreType",
        "getWTA_K",
        "setEdgeThreshold",
        "setFastThreshold",
        "setFirstLevel",
        "setMaxFeatures",
        "setNLevels",
        "setPatchSize",
        "setScaleFactor",
        "setScoreType",
        "setWTA_K",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_COMPUTE,
        FUNC_DETECTANDCOMPUTE,
        FUNC_getEdgeThreshold,
        FUNC_getFastThreshold,
        FUNC_getFirstLevel,
        FUNC_getMaxFeatures,
        FUNC_getNLevels,
        FUNC_getPatchSize,
        FUNC_getScaleFactor,
        FUNC_getScoreType,
        FUNC_getWTA_K,
        FUNC_setEdgeThreshold,
        FUNC_setFastThreshold,
        FUNC_setFirstLevel,
        FUNC_setMaxFeatures,
        FUNC_setNLevels,
        FUNC_setPatchSize,
        FUNC_setScaleFactor,
        FUNC_setScoreType,
        FUNC_setWTA_K,
        FUNC_CLOSE,
    };

    if( objc < 2 ){
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if( Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice) ){
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[0], 0);

    switch( (enum FUNC_enum)choice ){
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            Tcl_HashEntry *hashEntryPtr;
            char *handle;
            MatrixInfo *info;
            Tcl_Obj *pResultStr = NULL;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "detect invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "detect invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                orbdetector->detect(*(info->matrix), keypoints);
            } catch (...){
                Tcl_SetResult(interp, (char *) "detect failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.x ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.y ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].size ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].angle ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].response ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].octave ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].class_id ));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_COMPUTE: {
            cv::Mat descriptors;
            int count = 0;
            std::vector< cv::KeyPoint > keypoints;
            Tcl_HashEntry *hashEntryPtr;
            char *handle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            int newvalue;
            MatrixInfo *info;
            MatrixInfo *dstinfo;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if( objc != 4 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix keypoints");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "compute invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "compute invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "compute invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0) {
                Tcl_SetResult(interp, (char *) "compute keypoints data is empty", TCL_STATIC);
                return TCL_ERROR;
            } else {
                for (int i = 0; i < count; i++) {
                    Tcl_Obj *elemListPtr = NULL;
                    int sub_count = 0;
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);

                    if(Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                        Tcl_SetResult(interp, (char *) "compute invalid keypoints data", TCL_STATIC);
                        return TCL_ERROR;
                    }

                    if (sub_count != 7) {
                        Tcl_SetResult(interp, (char *) "compute wrong keypoints number", TCL_STATIC);
                        return TCL_ERROR;
                    } else {
                        Tcl_Obj *elemListSubPtr = NULL;
                        double x, y, size, angle, response;
                        int octave, class_id;

                        Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                        if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                        if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                        if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                        if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                        if(Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                        if(Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                        if(Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        cv::KeyPoint keypoint(cv::Point2f( (float) x, (float)y), (float) size,
                                            (float) angle, (float) response,
                                            octave, class_id);

                        keypoints.push_back(keypoint);
                    }
                }
            }

            try {
                orbdetector->compute(*(info->matrix), keypoints, descriptors);
            } catch (...){
                Tcl_SetResult(interp, (char *) "compute failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.x ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.y ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].size ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].angle ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].response ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].octave ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].class_id ));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "compute malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(descriptors);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr2 = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_DETECTANDCOMPUTE: {
            cv::Mat mask, descriptors;
            std::vector< cv::KeyPoint > keypoints;
            Tcl_HashEntry *hashEntryPtr;
            char *handle, *mask_handle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            int newvalue;
            MatrixInfo *info1, *info2;
            MatrixInfo *dstinfo;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if( objc != 3 && objc != 4){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix ?mask?");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "detectAndCompute invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info1 ) {
                Tcl_SetResult(interp, (char *) "detectAndCompute invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (objc == 4) {
                mask_handle = Tcl_GetStringFromObj(objv[3], 0);
                hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mask_handle );
                if( !hashEntryPtr ) {
                    if( interp ) {
                        Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                        Tcl_AppendStringsToObj( resultObj, "detectAndCompute invalid MATRIX handle ",
                                                mask_handle, (char *)NULL );
                    }

                    return TCL_ERROR;
                }

                info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
                if ( !info2 ) {
                    Tcl_SetResult(interp, (char *) "detectAndCompute invalid info data", TCL_STATIC);
                    return TCL_ERROR;
                }

                mask = *(info2->matrix);
            }

            try {
                orbdetector->detectAndCompute(*(info1->matrix), mask, keypoints, descriptors);
            } catch (...){
                Tcl_SetResult(interp, (char *) "detectAndCompute failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.x ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.y ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].size ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].angle ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].response ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].octave ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].class_id ));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "detectAndCompute malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(descriptors);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr2 = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_getEdgeThreshold: {
            int value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = orbdetector->getEdgeThreshold();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getEdgeThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );

            break;
        }
        case FUNC_getFastThreshold: {
            int value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = orbdetector->getFastThreshold();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getFastThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );

            break;
        }
        case FUNC_getFirstLevel: {
            int value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = orbdetector->getFirstLevel();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getFirstLevel failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );

            break;
        }
        case FUNC_getMaxFeatures: {
            int value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = orbdetector->getMaxFeatures();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getMaxFeatures failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );

            break;
        }
        case FUNC_getNLevels: {
            int value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = orbdetector->getNLevels();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getNLevels failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );

            break;
        }
        case FUNC_getPatchSize: {
            int value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = orbdetector->getPatchSize();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getPatchSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );

            break;
        }
        case FUNC_getScaleFactor: {
            double value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = orbdetector->getScaleFactor();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getScaleFactor failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj (value) );

            break;
        }
        case FUNC_getScoreType: {
            int value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = (int) orbdetector->getScoreType();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getScoreType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );

            break;
        }
        case FUNC_getWTA_K: {
            int value = 0;
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = orbdetector->getWTA_K();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getWTA_K failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );

            break;
        }
        case FUNC_setEdgeThreshold: {
            int value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setEdgeThreshold(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setEdgeThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setFastThreshold: {
            int value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setFastThreshold(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setFastThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setFirstLevel: {
            int value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setFirstLevel(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setFirstLevel failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setMaxFeatures: {
            int value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setMaxFeatures(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setMaxFeatures failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setNLevels: {
            int value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setNLevels(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setNLevels failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setPatchSize: {
            int value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setPatchSize(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setPatchSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setScaleFactor: {
            double value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setScaleFactor(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setScaleFactor failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setScoreType: {
            int value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setScoreType((::cv::ORB::ScoreType) value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setScoreType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setWTA_K: {
            int value = 0;
            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                orbdetector->setWTA_K(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setWTA_K failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            orbdetector.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int ORB(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int nfeatures = 500, nlevels = 8, edgeThreshold = 31, firstLevel = 0, WTA_K = 2;
    int scoreType = ::cv::ORB::HARRIS_SCORE, patchSize = 31, fastThreshold = 20;
    double scaleFactor = 1.2f;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "?nfeatures scaleFactor nlevels edgeThreshold firstLevel WTA_K scoreType patchSize fastThreshold?");
        return TCL_ERROR;
    }

    if (objc == 10) {
        if(Tcl_GetIntFromObj(interp, objv[1], &nfeatures) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[2], &scaleFactor) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[3], &nlevels) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[4], &edgeThreshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &firstLevel) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[6], &WTA_K) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[7], &scoreType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[8], &patchSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[9], &fastThreshold) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        orbdetector = cv::ORB::create( nfeatures, (float) scaleFactor, nlevels,
                                       edgeThreshold, firstLevel, WTA_K,
                                       (cv::ORB::ScoreType) scoreType, patchSize,
                                       fastThreshold);

        if (orbdetector == nullptr) {
            Tcl_SetResult(interp, (char *) "ORB create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "ORB failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-orbdetector", -1 );

    Tcl_CreateObjCommand(interp, "cv-orbdetector", (Tcl_ObjCmdProc *) ORB_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int BFMatcher_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "match",
        "knnMatch",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_MATCH,
        FUNC_KNNMATCH,
        FUNC_CLOSE,
    };

    if( objc < 2 ){
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if( Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice) ){
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[0], 0);

    switch( (enum FUNC_enum)choice ){
        case FUNC_MATCH: {
            Tcl_HashEntry *hashEntryPtr;
            char *handle1, *handle2;
            MatrixInfo *info1, *info2;
            std::vector< cv::DMatch > matches;
            Tcl_Obj *pResultStr = NULL;

            if( objc != 4 ){
                Tcl_WrongNumArgs(interp, 2, objv, "queryDescriptors trainDescriptors");
                return TCL_ERROR;
            }

            handle1 = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle1 );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "match invalid MATRIX handle ",
                                            handle1, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info1 ) {
                Tcl_SetResult(interp, (char *) "match invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            handle2 = Tcl_GetStringFromObj(objv[3], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle2 );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "match invalid MATRIX handle ",
                                            handle2, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info2 ) {
                Tcl_SetResult(interp, (char *) "match invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                bfmatcher->match(*(info1->matrix), *(info2->matrix), matches);
            } catch (...){
                Tcl_SetResult(interp, (char *) "match failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < matches.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( matches[i].queryIdx ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( matches[i].trainIdx ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( matches[i].imgIdx ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( matches[i].distance ));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_KNNMATCH: {
            Tcl_HashEntry *hashEntryPtr;
            char *handle1, *handle2;
            MatrixInfo *info1, *info2;
            std::vector<std::vector<cv::DMatch> > vmatches;
            Tcl_Obj *pResultStr = NULL;
            int k;

            if( objc != 5 ){
                Tcl_WrongNumArgs(interp, 2, objv, "queryDescriptors trainDescriptors k");
                return TCL_ERROR;
            }

            handle1 = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle1 );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "knnMatch invalid MATRIX handle ",
                                            handle1, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info1 ) {
                Tcl_SetResult(interp, (char *) "knnMatch invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            handle2 = Tcl_GetStringFromObj(objv[3], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle2 );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "knnMatch invalid MATRIX handle ",
                                            handle2, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info2 ) {
                Tcl_SetResult(interp, (char *) "knnMatch invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[4], &k) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                bfmatcher->knnMatch(*(info1->matrix), *(info2->matrix), vmatches, k);
            } catch (...){
                Tcl_SetResult(interp, (char *) "knnMatch failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < vmatches.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);

                std::vector<cv::DMatch> matches = vmatches[i];
                for (size_t j = 0; j < matches.size(); j++) {
                    Tcl_Obj *pSubListStr = NULL;
                    pSubListStr = Tcl_NewListObj(0, NULL);
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj( matches[j].queryIdx ));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj( matches[j].trainIdx ));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj( matches[j].imgIdx ));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewDoubleObj( matches[j].distance ));

                    Tcl_ListObjAppendElement(NULL, pListStr, pSubListStr);
                }

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            bfmatcher.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int BFMatcher(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int normType = cv::NORM_L2, crossCheck = 0;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "?normType crossCheck?");
        return TCL_ERROR;
    }

    if (objc == 3) {
        if(Tcl_GetIntFromObj(interp, objv[1], &normType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[2], &crossCheck) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        bfmatcher = cv::BFMatcher::create( normType, (bool) crossCheck);

        if (bfmatcher == nullptr) {
            Tcl_SetResult(interp, (char *) "BFMatcher create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "BFMatcher failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-bfmatcher", -1 );

    Tcl_CreateObjCommand(interp, "cv-bfmatcher", (Tcl_ObjCmdProc *) BFMatcher_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int SimpleBlobDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "detect",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_CLOSE,
    };

    if( objc < 2 ){
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if( Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice) ){
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[0], 0);

    switch( (enum FUNC_enum)choice ){
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            Tcl_HashEntry *hashEntryPtr;
            char *handle;
            MatrixInfo *info;
            Tcl_Obj *pResultStr = NULL;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "detect invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "detect invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                simpleBlobDetector->detect(*(info->matrix), keypoints);
            } catch (...){
                Tcl_SetResult(interp, (char *) "detect failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.x ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].pt.y ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].size ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].angle ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( keypoints[i].response ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].octave ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( keypoints[i].class_id ));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            simpleBlobDetector.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int SimpleBlobDetector(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    Tcl_Obj *pResultStr = NULL;
    char *zArg = NULL;
    double minThreshold = 0, maxThreshold = 0;
    double minArea = 0, minCircularity = 0, minConvexity = 0, minInertiaRatio = 0;
    double maxArea = 0, maxCircularity = 0, maxConvexity = 0, maxInertiaRatio = 0;
    int filterByArea = 1, filterByCircularity = 1, filterByConvexity = 1, filterByInertia = 1;
    cv::SimpleBlobDetector::Params params;

    if ((objc&1)!=1) {
        Tcl_WrongNumArgs(interp, 1, objv, "?-minThreshold value? ?-maxThreshold value? ?-filterByArea boolean? ?-minArea value? ?-maxArea value? ?-filterByCircularity boolean? ?-minCircularity value? ?-maxCircularity value? ?-filterByConvexity boolean? ?-minConvexity value? ?-maxConvexity value? ?-filterByInertia boolean? ?-minInertiaRatio value? ?-maxInertiaRatio value?");
        return TCL_ERROR;
    }

    for(int i=1; i+1<objc; i+=2){
        zArg = Tcl_GetStringFromObj(objv[i], 0);

        if( strcmp(zArg, "-minThreshold")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &minThreshold) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minThreshold = (float) minThreshold;
        } else if( strcmp(zArg, "-maxThreshold")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &maxThreshold) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxThreshold = (float) maxThreshold;
        } else if( strcmp(zArg, "-filterByArea")==0 ){
            if(Tcl_GetBooleanFromObj(interp, objv[i+1], &filterByArea) != TCL_OK) {
                return TCL_ERROR;
            }

            params.filterByArea = (bool) filterByArea;
        } else if( strcmp(zArg, "-minArea")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &minArea) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minArea = (float) minArea;
        } else if( strcmp(zArg, "-maxArea")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &maxArea) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxArea = (float) maxArea;
        } else if( strcmp(zArg, "-filterByCircularity")==0 ){
            if(Tcl_GetBooleanFromObj(interp, objv[i+1], &filterByCircularity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.filterByCircularity = (bool) filterByCircularity;
        } else if( strcmp(zArg, "-minCircularity")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &minCircularity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minCircularity = (float) minCircularity;
        } else if( strcmp(zArg, "-maxCircularity")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &maxCircularity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxCircularity = (float) maxCircularity;
        } else if( strcmp(zArg, "-filterByConvexity")==0 ){
            if(Tcl_GetBooleanFromObj(interp, objv[i+1], &filterByConvexity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.filterByConvexity = (bool) filterByConvexity;
        } else if( strcmp(zArg, "-minConvexity")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &minConvexity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minConvexity = (float) minConvexity;
        } else if( strcmp(zArg, "-maxConvexity")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &maxConvexity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxConvexity = (float) maxConvexity;
        } else if( strcmp(zArg, "-filterByInertia")==0 ){
            if(Tcl_GetBooleanFromObj(interp, objv[i+1], &filterByInertia) != TCL_OK) {
                return TCL_ERROR;
            }

            params.filterByInertia = (bool) filterByInertia;
        } else if( strcmp(zArg, "-minInertiaRatio")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &minInertiaRatio) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minInertiaRatio = (float) minInertiaRatio;
        } else if( strcmp(zArg, "-maxInertiaRatio")==0 ){
            if(Tcl_GetDoubleFromObj(interp, objv[i+1], &maxInertiaRatio) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxInertiaRatio = (float) maxInertiaRatio;
        } else {
            Tcl_SetResult(interp, (char *) "SimpleBlobDetector invalid parameter", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    try {
        simpleBlobDetector = cv::SimpleBlobDetector::create(params);

        if (simpleBlobDetector == nullptr) {
            Tcl_SetResult(interp, (char *) "simpleBlobDetector create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "SimpleBlobDetector failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-simpleBlobDetector", -1 );

    Tcl_CreateObjCommand(interp, "cv-simpleBlobDetector", (Tcl_ObjCmdProc *) SimpleBlobDetector_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
