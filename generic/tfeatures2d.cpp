#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int drawKeypoints(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dst_image;
    std::vector< cv::KeyPoint > keypoints;
    int count = 0, flags = 0, new_B = 0, new_G = 0, new_R = 0, new_A = 0;
    int nodstimage = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2 = NULL, *dstmat;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix keypoints dst_image color_list ?flags?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "no keypoints data");
    } else {
        for (int i = 0; i < count; i++) {
            Tcl_Obj *elemListPtr = NULL;
            int sub_count = 0;
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid keypoints data");
            }

            if (sub_count != 7) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong  keypoints number");
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                double x, y, size, angle, response;
                int octave, class_id;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                    return TCL_ERROR;
                }

                cv::KeyPoint keypoint(cv::Point2f((float) x, (float)y), (float) size,
                                      (float) angle, (float) response,
                                      octave, class_id);

                keypoints.push_back(keypoint);
            }
        }
    }

    /*
     * If users give the handle to "None", then setup a nodstimage flag to
     * indicate this thing. This flag is used to decide the argument.
     */
    if (strcmp(Tcl_GetString(objv[3]), "None") == 0) {
        nodstimage = 1;
    } else {
        nodstimage = 0;

        mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
        if (!mat2) {
            return TCL_ERROR;
        }
    }

    if (Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[4], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &new_B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &new_G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &new_R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &new_A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[5], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(new_B, new_G, new_R, new_A);

        if (nodstimage == 0 && (flags & (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG) == 1) {
                dst_image = *mat2;
                cv::drawKeypoints(*mat1, keypoints, dst_image, color,
#ifdef TCL_USE_OPENCV4
                                  (cv::DrawMatchesFlags) flags
#else
                                  flags
#endif
                                 );
        } else if (nodstimage == 1 && (flags & (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG) == 0) {
                cv::drawKeypoints(*mat1, keypoints, dst_image, color,
#ifdef TCL_USE_OPENCV4
                                  (cv::DrawMatchesFlags) flags
#else
                                  flags
#endif
                                 );
        } else {
            CV_Error(cv::Error::StsError, "Invalid arguments");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dst_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int drawMatches(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat mask, dst_image;
    std::vector< cv::KeyPoint > keypoints1, keypoints2;
    std::vector< cv::DMatch > matches1to2;
    int count = 0, flags = 0, B1 = 0, G1 = 0, R1 = 0, A1 = 0;
    int B2 = 0, G2 = 0, R2 = 0, A2 = 0;
    int nodstimage = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *mat3 = NULL, *dstmat;

    if (objc != 9 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix1 keypoints1 matrix2 keypoints2 matches1to2 dst_image matchColor_list singlePointColor_list ?flags?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "no keypoints data");
    } else {
        for (int i = 0; i < count; i++) {
            Tcl_Obj *elemListPtr = NULL;
            int sub_count = 0;
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid keypoints data");
            }

            if (sub_count != 7) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong  keypoints number");
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                double x, y, size, angle, response;
                int octave, class_id;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                    return TCL_ERROR;
                }

                cv::KeyPoint keypoint(cv::Point2f((float) x, (float)y), (float) size,
                                      (float) angle, (float) response,
                                      octave, class_id);

                keypoints1.push_back(keypoint);
            }
        }
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "no keypoints data");
    } else {
        for (int i = 0; i < count; i++) {
            Tcl_Obj *elemListPtr = NULL;
            int sub_count = 0;
            Tcl_ListObjIndex(interp, objv[4], i, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid keypoints data");
            }

            if (sub_count != 7) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong  keypoints number");
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                double x, y, size, angle, response;
                int octave, class_id;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                    return TCL_ERROR;
                }

                cv::KeyPoint keypoint(cv::Point2f((float) x, (float)y), (float) size,
                                      (float) angle, (float) response,
                                      octave, class_id);

                keypoints2.push_back(keypoint);
            }
        }
    }

    /*
     * Matches from the first image to the second one
     */
    if (Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "matches1to2 list is empty");
    } else {
        for (int i = 0; i < count; i++) {
            Tcl_Obj *elemListPtr = NULL;
            int sub_count = 0;
            Tcl_ListObjIndex(interp, objv[5], i, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid matches1to2 data");
            }

            if (sub_count != 4) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong matches1to2 number");
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                int queryIdx, trainIdx, imgIdx;
                double distance;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &queryIdx) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &trainIdx) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                if (Tcl_GetIntFromObj(interp, elemListSubPtr, &imgIdx) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &distance) != TCL_OK) {
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
    if (strcmp(Tcl_GetString(objv[6]), "None") == 0) {
        nodstimage = 1;
    } else {
        nodstimage = 0;

        mat3 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[6]);
        if (!mat3) {
            return TCL_ERROR;
        }
    }

    if (Tcl_ListObjLength(interp, objv[7], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[7], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A1) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_ListObjLength(interp, objv[8], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[8], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[8], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[8], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[8], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A2) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (objc == 10) {
        if (Tcl_GetIntFromObj(interp, objv[9], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color1(B1, G1, R1, A1);
        cv::Scalar color2(B2, G2, R2, A2);

        if (nodstimage == 0 && (flags & (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG) == 1) {
                dst_image = *mat3;
                cv::drawMatches(*mat1, keypoints1, *mat2, keypoints2,
                                matches1to2, dst_image, color1, color2,
                                mask,
#ifdef TCL_USE_OPENCV4
                                (cv::DrawMatchesFlags) flags
#else

                                flags
#endif
                               );
        } else if (nodstimage == 1 && (flags & (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG) == 0) {
                cv::drawMatches(*mat1, keypoints1, *mat2, keypoints2,
                                matches1to2, dst_image, color1, color2,
                                mask,
#ifdef TCL_USE_OPENCV4
                                (cv::DrawMatchesFlags) flags
#else

                                flags
#endif
                               );
        } else {
            CV_Error(cv::Error::StsError, "Invalid arguments");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dst_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


static void FastFeatureDetector_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->fastdetector) {
        cvd->fastdetector.release();
    }
    cvd->cmd_fastdetector = NULL;
}


static int FastFeatureDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detect",
        "getNonmaxSuppression",
        "getThreshold",
        "getType",
        "setNonmaxSuppression",
        "setThreshold",
        "setType",
        "close",
        "_command",
        "_name",
        "_type",
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
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->fastdetector == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->fastdetector->detect(*mat, keypoints);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_GETNONMAXSUPPRESSION: {
            int nonmaxSuppression = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                nonmaxSuppression = cvd->fastdetector->getNonmaxSuppression();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj(nonmaxSuppression));
            break;
        }
        case FUNC_GETTHRESHOLD: {
            int threshold = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                threshold = cvd->fastdetector->getThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(threshold));
            break;
        }
        case FUNC_GETTYPE: {
            int type = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                type = (int) cvd->fastdetector->getType();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(type));
            break;
        }
        case FUNC_SETNONMAXSUPPRESSION: {
            int nonmaxSuppression = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &nonmaxSuppression) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->fastdetector->setNonmaxSuppression(nonmaxSuppression);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_SETTHRESHOLD: {
            int threshold = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &threshold) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->fastdetector->setThreshold(threshold);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_SETTYPE: {
            int type = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &type) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
#ifdef TCL_USE_OPENCV4
                cvd->fastdetector->setType((cv::FastFeatureDetector::DetectorType) type);
#else
                cvd->fastdetector->setType(type);
#endif
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_fastdetector) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_fastdetector);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_fastdetector) {
                Tcl_GetCommandFullName(interp, cvd->cmd_fastdetector, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::FastFeatureDetector", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int FastFeatureDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int threshold = 10, nonmaxSuppression = 1, type = ::cv::FastFeatureDetector::TYPE_9_16;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::FastFeatureDetector> fastdetector;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?threshold nonmaxSuppression type?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[1], &threshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[2], &nonmaxSuppression) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
#ifdef TCL_USE_OPENCV4
        fastdetector = cv::FastFeatureDetector::create(threshold, (bool) nonmaxSuppression,
                                                (cv::FastFeatureDetector::DetectorType) type);
#else
        fastdetector = cv::FastFeatureDetector::create(threshold, (bool) nonmaxSuppression,
                                                type);
#endif
        if (fastdetector == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "FastFeatureDetector nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-fastdetector", -1);

    if (cvd->cmd_fastdetector) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_fastdetector);
    }
    cvd->cmd_fastdetector =
        Tcl_CreateObjCommand(interp, "::cv-fastdetector",
            (Tcl_ObjCmdProc *) FastFeatureDetector_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) FastFeatureDetector_DESTRUCTOR);

    cvd->fastdetector = fastdetector;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void AgastFeatureDetector_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->agastdetector) {
        cvd->agastdetector.release();
    }
    cvd->cmd_agastdetector = NULL;
}


static int AgastFeatureDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detect",
        "getNonmaxSuppression",
        "getThreshold",
        "getType",
        "setNonmaxSuppression",
        "setThreshold",
        "setType",
        "close",
        "_command",
        "_name",
        "_type",
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
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->agastdetector == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->agastdetector->detect(*mat, keypoints);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_GETNONMAXSUPPRESSION: {
            int nonmaxSuppression = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                nonmaxSuppression = cvd->agastdetector->getNonmaxSuppression();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj(nonmaxSuppression));
            break;
        }
        case FUNC_GETTHRESHOLD: {
            int threshold = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                threshold = cvd->agastdetector->getThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(threshold));
            break;
        }
        case FUNC_GETTYPE: {
            int type = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                type = (int) cvd->agastdetector->getType();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(type));
            break;
        }
        case FUNC_SETNONMAXSUPPRESSION: {
            int nonmaxSuppression = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &nonmaxSuppression) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->agastdetector->setNonmaxSuppression(nonmaxSuppression);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_SETTHRESHOLD: {
            int threshold = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &threshold) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->agastdetector->setThreshold(threshold);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_SETTYPE: {
            int type = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &type) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
#ifdef TCL_USE_OPENCV4
                cvd->agastdetector->setType((cv::AgastFeatureDetector::DetectorType) type);
#else
                cvd->agastdetector->setType(type);
#endif
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_agastdetector) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_agastdetector);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_agastdetector) {
                Tcl_GetCommandFullName(interp, cvd->cmd_agastdetector, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::AgastFeatureDetector", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int AgastFeatureDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int threshold = 10, nonmaxSuppression = 1, type = ::cv::AgastFeatureDetector::OAST_9_16;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::AgastFeatureDetector> agastdetector;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?threshold nonmaxSuppression type?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[1], &threshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[2], &nonmaxSuppression) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
#ifdef TCL_USE_OPENCV4
        agastdetector = cv::AgastFeatureDetector::create(threshold, (bool) nonmaxSuppression,
                           (cv::AgastFeatureDetector::DetectorType) type);
#else
        agastdetector = cv::AgastFeatureDetector::create(threshold, (bool) nonmaxSuppression,
                           type);
#endif
        if (agastdetector == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "AgastFeatureDetector nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-agastdetector", -1);

    if (cvd->cmd_agastdetector) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_agastdetector);
    }
    cvd->cmd_agastdetector =
        Tcl_CreateObjCommand(interp, "::cv-agastdetector",
            (Tcl_ObjCmdProc *) AgastFeatureDetector_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) AgastFeatureDetector_DESTRUCTOR);

    cvd->agastdetector = agastdetector;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void MSER_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->mserextractor) {
        cvd->mserextractor.release();
    }
    cvd->cmd_mserextractor = NULL;
}


static int MSER_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detectRegions",
        "getDelta",
        "getMaxArea",
        "getMinArea",
        "getPass2Only",
        "setDelta",
        "setMaxArea",
        "setMinArea",
        "setPass2Only",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_detectRegions,
        FUNC_getDelta,
        FUNC_getMaxArea,
        FUNC_getMinArea,
        FUNC_getPass2Only,
        FUNC_setDelta,
        FUNC_setMaxArea,
        FUNC_setMinArea,
        FUNC_setPass2Only,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->mserextractor == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_detectRegions: {
            std::vector< std::vector< cv::Point > > msers;
            std::vector< cv::Rect > bboxes;
            cv::Mat *mat;
            Tcl_Obj *pResultStr = NULL, *pListResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->mserextractor->detectRegions(*mat, msers, bboxes);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);

            pListResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < msers.size(); i++) {
                Tcl_Obj *pListStr = NULL;

                pListStr = Tcl_NewListObj(0, NULL);
                for (size_t j = 0; j < msers[i].size(); j++) {
                    Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(msers[i][j].x));
                    Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(msers[i][j].y));
                }

                Tcl_ListObjAppendElement(NULL, pListResultStr, pListStr);
            }
            Tcl_ListObjAppendElement(NULL, pResultStr, pListResultStr);

            pListResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < bboxes.size(); i++) {
                Tcl_Obj *pListStr = NULL;

                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(bboxes[i].x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(bboxes[i].y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(bboxes[i].width));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(bboxes[i].height));

                Tcl_ListObjAppendElement(NULL, pListResultStr, pListStr);
            }
            Tcl_ListObjAppendElement(NULL, pResultStr, pListResultStr);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_getDelta: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->mserextractor->getDelta();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMaxArea: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->mserextractor->getMaxArea();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMinArea: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->mserextractor->getMinArea();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getPass2Only: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = (int) cvd->mserextractor->getPass2Only();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj(value));
            break;
        }
        case FUNC_setDelta: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->mserextractor->setDelta(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxArea: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->mserextractor->setMaxArea(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMinArea: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->mserextractor->setMinArea(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setPass2Only: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->mserextractor->setPass2Only((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_mserextractor) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_mserextractor);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_mserextractor) {
                Tcl_GetCommandFullName(interp, cvd->cmd_mserextractor, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::MSER", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int MSER(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int delta = 5, min_area = 60, max_area = 14400, max_evolution = 200, edge_blur_size = 5;
    double max_variation = 0.25, min_diversity = 0.2, area_threshold = 1.01, min_margin = 0.003;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::MSER> mserextractor;

    if (objc != 1 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "?delta min_area max_area max_variation min_diversity max_evolution area_threshold min_margin edge_blur_size?");
        return TCL_ERROR;
    }

    if (objc == 10) {
        if (Tcl_GetIntFromObj(interp, objv[1], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &min_area) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &max_area) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[4], &max_variation) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[5], &min_diversity) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &max_evolution) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[7], &area_threshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[8], &min_margin) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[9], &edge_blur_size) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        mserextractor = cv::MSER::create(delta, min_area, max_area,
                                         max_variation, min_diversity,
                                         max_evolution, area_threshold,
                                         min_margin, edge_blur_size);

        if (mserextractor == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "MSER nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-mserextractor", -1);

    if (cvd->cmd_mserextractor) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_mserextractor);
    }
    cvd->cmd_mserextractor =
        Tcl_CreateObjCommand(interp, "::cv-mserextractor",
            (Tcl_ObjCmdProc *) MSER_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) MSER_DESTRUCTOR);

    cvd->mserextractor = mserextractor;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void ORB_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->orbdetector) {
        cvd->orbdetector.release();
    }
    cvd->cmd_orbdetector = NULL;
}


static int ORB_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

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
        "_command",
        "_name",
        "_type",
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
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->orbdetector == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->detect(*mat, keypoints);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_COMPUTE: {
            cv::Mat descriptors;
            int count = 0;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix keypoints");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "no keypoints data");
            } else {
                for (int i = 0; i < count; i++) {
                    Tcl_Obj *elemListPtr = NULL;
                    int sub_count = 0;
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);

                    if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid keypoints data");
                    }

                    if (sub_count != 7) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong  keypoints number");
                    } else {
                        Tcl_Obj *elemListSubPtr = NULL;
                        double x, y, size, angle, response;
                        int octave, class_id;

                        Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        cv::KeyPoint keypoint(cv::Point2f((float) x, (float)y), (float) size,
                                            (float) angle, (float) response,
                                            octave, class_id);

                        keypoints.push_back(keypoint);
                    }
                }
            }

            try {
                cvd->orbdetector->compute(*mat, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_DETECTANDCOMPUTE: {
            cv::Mat mask, descriptors;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat1, *mat2, *dstmat;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix ?mask?");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
                if (!mat2) {
                    return TCL_ERROR;
                }
                mask = *mat2;
            }

            try {
                cvd->orbdetector->detectAndCompute(*mat1, mask, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_getEdgeThreshold: {
            int value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->orbdetector->getEdgeThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));

            break;
        }
        case FUNC_getFastThreshold: {
            int value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->orbdetector->getFastThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));

            break;
        }
        case FUNC_getFirstLevel: {
            int value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->orbdetector->getFirstLevel();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));

            break;
        }
        case FUNC_getMaxFeatures: {
            int value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->orbdetector->getMaxFeatures();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));

            break;
        }
        case FUNC_getNLevels: {
            int value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->orbdetector->getNLevels();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));

            break;
        }
        case FUNC_getPatchSize: {
            int value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->orbdetector->getPatchSize();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));

            break;
        }
        case FUNC_getScaleFactor: {
            double value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->orbdetector->getScaleFactor();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));

            break;
        }
        case FUNC_getScoreType: {
            int value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = (int) cvd->orbdetector->getScoreType();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));

            break;
        }
        case FUNC_getWTA_K: {
            int value = 0;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->orbdetector->getWTA_K();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));

            break;
        }
        case FUNC_setEdgeThreshold: {
            int value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->setEdgeThreshold(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setFastThreshold: {
            int value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->setFastThreshold(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setFirstLevel: {
            int value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->setFirstLevel(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxFeatures: {
            int value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->setMaxFeatures(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setNLevels: {
            int value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->setNLevels(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setPatchSize: {
            int value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->setPatchSize(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setScaleFactor: {
            double value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->setScaleFactor(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setScoreType: {
            int value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
#ifdef TCL_USE_OPENCV4
                cvd->orbdetector->setScoreType((::cv::ORB::ScoreType) value);
#else
                cvd->orbdetector->setScoreType(value);
#endif
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setWTA_K: {
            int value = 0;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->orbdetector->setWTA_K(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_orbdetector) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_orbdetector);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_orbdetector) {
                Tcl_GetCommandFullName(interp, cvd->cmd_orbdetector, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ORB", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int ORB(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int nfeatures = 500, nlevels = 8, edgeThreshold = 31, firstLevel = 0, WTA_K = 2;
    int scoreType = ::cv::ORB::HARRIS_SCORE, patchSize = 31, fastThreshold = 20;
    double scaleFactor = 1.2f;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ORB> orbdetector;

    if (objc != 1 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "?nfeatures scaleFactor nlevels edgeThreshold firstLevel WTA_K scoreType patchSize fastThreshold?");
        return TCL_ERROR;
    }

    if (objc == 10) {
        if (Tcl_GetIntFromObj(interp, objv[1], &nfeatures) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[2], &scaleFactor) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &nlevels) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[4], &edgeThreshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &firstLevel) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &WTA_K) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &scoreType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &patchSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[9], &fastThreshold) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
#ifdef TCL_USE_OPENCV4
        orbdetector = cv::ORB::create(nfeatures, (float) scaleFactor, nlevels,
                                      edgeThreshold, firstLevel, WTA_K,
                                      (cv::ORB::ScoreType) scoreType, patchSize,
                                      fastThreshold);
#else
        orbdetector = cv::ORB::create(nfeatures, (float) scaleFactor, nlevels,
                                      edgeThreshold, firstLevel, WTA_K,
                                      scoreType, patchSize, fastThreshold);
#endif
        if (orbdetector == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "ORB nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-orbdetector", -1);

    if (cvd->cmd_orbdetector) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_orbdetector);
    }
    cvd->cmd_orbdetector =
        Tcl_CreateObjCommand(interp, "::cv-orbdetector",
            (Tcl_ObjCmdProc *) ORB_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) ORB_DESTRUCTOR);

    cvd->orbdetector = orbdetector;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void AKAZE_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->akazedetector) {
        cvd->akazedetector.release();
    }
    cvd->cmd_akazedetector = NULL;
}


static int AKAZE_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detect",
        "compute",
        "detectAndCompute",
        "getDescriptorChannels",
        "getDescriptorSize",
        "getDescriptorType",
        "getDiffusivity",
        "getNOctaveLayers",
        "getNOctaves",
        "getThreshold",
        "setDescriptorChannels",
        "setDescriptorSize",
        "setDescriptorType",
        "setDiffusivity",
        "setNOctaveLayers",
        "setNOctaves",
        "setThreshold",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_COMPUTE,
        FUNC_DETECTANDCOMPUTE,
        FUNC_getDescriptorChannels,
        FUNC_getDescriptorSize,
        FUNC_getDescriptorType,
        FUNC_getDiffusivity,
        FUNC_getNOctaveLayers,
        FUNC_getNOctaves,
        FUNC_getThreshold,
        FUNC_setDescriptorChannels,
        FUNC_setDescriptorSize,
        FUNC_setDescriptorType,
        FUNC_setDiffusivity,
        FUNC_setNOctaveLayers,
        FUNC_setNOctaves,
        FUNC_setThreshold,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->akazedetector == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->akazedetector->detect(*mat, keypoints);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_COMPUTE: {
            cv::Mat descriptors;
            int count = 0;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix keypoints");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "no keypoints data");
            } else {
                for (int i = 0; i < count; i++) {
                    Tcl_Obj *elemListPtr = NULL;
                    int sub_count = 0;
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);

                    if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid keypoints data");
                    }

                    if (sub_count != 7) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong  keypoints number");
                    } else {
                        Tcl_Obj *elemListSubPtr = NULL;
                        double x, y, size, angle, response;
                        int octave, class_id;

                        Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        cv::KeyPoint keypoint(cv::Point2f((float) x, (float)y), (float) size,
                                            (float) angle, (float) response,
                                            octave, class_id);

                        keypoints.push_back(keypoint);
                    }
                }
            }

            try {
                cvd->akazedetector->compute(*mat, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_DETECTANDCOMPUTE: {
            cv::Mat mask, descriptors;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat1, *mat2, *dstmat;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix ?mask?");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
                if (!mat2) {
                    return TCL_ERROR;
                }
                mask = *mat2;
            }

            try {
                cvd->akazedetector->detectAndCompute(*mat1, mask, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_getDescriptorChannels: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->akazedetector->getDescriptorChannels();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDescriptorSize: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->akazedetector->getDescriptorSize();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDescriptorType: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = (int) cvd->akazedetector->getDescriptorType();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDiffusivity: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = (int) cvd->akazedetector->getDiffusivity();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getNOctaveLayers: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->akazedetector->getNOctaveLayers();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getNOctaves: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->akazedetector->getNOctaves();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getThreshold: {
            double value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->akazedetector->getThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_setDescriptorChannels: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->akazedetector->setDescriptorChannels(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setDescriptorSize: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->akazedetector->setDescriptorSize(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setDescriptorType: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
#ifdef TCL_USE_OPENCV4
                cvd->akazedetector->setDescriptorType((cv::AKAZE::DescriptorType) value);
#else
                cvd->akazedetector->setDescriptorType(value);
#endif
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setDiffusivity: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
#ifdef TCL_USE_OPENCV4
                cvd->akazedetector->setDiffusivity((cv::KAZE::DiffusivityType) value);
#else
                cvd->akazedetector->setDiffusivity(value);
#endif
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setNOctaveLayers: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->akazedetector->setNOctaveLayers(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setNOctaves: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->akazedetector->setNOctaves(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setThreshold: {
            double value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->akazedetector->setThreshold(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_akazedetector) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_akazedetector);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_akazedetector) {
                Tcl_GetCommandFullName(interp, cvd->cmd_akazedetector, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::AKAZE", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int AKAZE(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int descriptor_type = (int) cv::AKAZE::DESCRIPTOR_MLDB, descriptor_size = 0;
    int descriptor_channels = 3, nOctaves = 4, nOctaveLayers = 4;
    int diffusivity = (int) cv::KAZE::DIFF_PM_G2;
    double threshold = 0.001f;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::AKAZE> akazedetector;

    if (objc != 1 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "?descriptor_type descriptor_size descriptor_channels threshold nOctaves nOctaveLayers diffusivity?");
        return TCL_ERROR;
    }

    if (objc == 8) {
        if (Tcl_GetIntFromObj(interp, objv[1], &descriptor_type) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &descriptor_size) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &descriptor_channels) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[4], &threshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &nOctaves) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &nOctaveLayers) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &diffusivity) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
#ifdef TCL_USE_OPENCV4
        akazedetector = cv::AKAZE::create((cv::AKAZE::DescriptorType) descriptor_type,
                                          descriptor_size, descriptor_channels,
                                          (float) threshold, nOctaves, nOctaveLayers,
                                          (cv::KAZE::DiffusivityType) diffusivity);
#else
        akazedetector = cv::AKAZE::create(descriptor_type,
                                          descriptor_size, descriptor_channels,
                                          (float) threshold, nOctaves, nOctaveLayers,
                                          diffusivity);
#endif

        if (akazedetector == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "AKAZE nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-akazedetector", -1);

    if (cvd->cmd_akazedetector) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_akazedetector);
    }
    cvd->cmd_akazedetector =
        Tcl_CreateObjCommand(interp, "::cv-akazedetector",
            (Tcl_ObjCmdProc *) AKAZE_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) AKAZE_DESTRUCTOR);

    cvd->akazedetector = akazedetector;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void BRISK_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->briskdetector) {
        cvd->briskdetector.release();
    }
    cvd->cmd_briskdetector = NULL;
}


static int BRISK_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detect",
        "compute",
        "detectAndCompute",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_COMPUTE,
        FUNC_DETECTANDCOMPUTE,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->briskdetector == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->briskdetector->detect(*mat, keypoints);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_COMPUTE: {
            cv::Mat descriptors;
            int count = 0;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix keypoints");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "no keypoints data");
            } else {
                for (int i = 0; i < count; i++) {
                    Tcl_Obj *elemListPtr = NULL;
                    int sub_count = 0;
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);

                    if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid keypoints data");
                    }

                    if (sub_count != 7) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong  keypoints number");
                    } else {
                        Tcl_Obj *elemListSubPtr = NULL;
                        double x, y, size, angle, response;
                        int octave, class_id;

                        Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        cv::KeyPoint keypoint(cv::Point2f((float) x, (float)y), (float) size,
                                            (float) angle, (float) response,
                                            octave, class_id);

                        keypoints.push_back(keypoint);
                    }
                }
            }

            try {
                cvd->briskdetector->compute(*mat, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_DETECTANDCOMPUTE: {
            cv::Mat mask, descriptors;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat1, *mat2, *dstmat;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix ?mask?");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
                if (!mat2) {
                    return TCL_ERROR;
                }
                mask = *mat2;
            }

            try {
                cvd->briskdetector->detectAndCompute(*mat1, mask, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_briskdetector) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_briskdetector);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_briskdetector) {
                Tcl_GetCommandFullName(interp, cvd->cmd_briskdetector, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::BRISK", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int BRISK(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int thresh = 30, octaves = 3;
    double patternScale = 1.0f;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::BRISK> briskdetector;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?thresh octaves patternScale?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[1], &thresh) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &octaves) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &patternScale) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        briskdetector = cv::BRISK::create(thresh, octaves, (float) patternScale);

        if (briskdetector == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "BRISK nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-briskdetector", -1);

    if (cvd->cmd_briskdetector) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_briskdetector);
    }
    cvd->cmd_briskdetector =
        Tcl_CreateObjCommand(interp, "::cv-briskdetector",
            (Tcl_ObjCmdProc *) BRISK_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) BRISK_DESTRUCTOR);

    cvd->briskdetector = briskdetector;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_SIFT
static void SIFT_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->siftdetector) {
        cvd->siftdetector.release();
    }
    cvd->cmd_siftdetector = NULL;
}


static int SIFT_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detect",
        "compute",
        "detectAndCompute",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_COMPUTE,
        FUNC_DETECTANDCOMPUTE,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->siftdetector == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->siftdetector->detect(*mat, keypoints);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_COMPUTE: {
            cv::Mat descriptors;
            int count = 0;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix keypoints");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "no keypoints data");
            } else {
                for (int i = 0; i < count; i++) {
                    Tcl_Obj *elemListPtr = NULL;
                    int sub_count = 0;
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);

                    if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid keypoints data");
                    }

                    if (sub_count != 7) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong  keypoints number");
                    } else {
                        Tcl_Obj *elemListSubPtr = NULL;
                        double x, y, size, angle, response;
                        int octave, class_id;

                        Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        cv::KeyPoint keypoint(cv::Point2f((float) x, (float)y), (float) size,
                                            (float) angle, (float) response,
                                            octave, class_id);

                        keypoints.push_back(keypoint);
                    }
                }
            }

            try {
                cvd->siftdetector->compute(*mat, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_DETECTANDCOMPUTE: {
            cv::Mat mask, descriptors;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat1, *mat2, *dstmat;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix ?mask?");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
                if (!mat2) {
                    return TCL_ERROR;
                }
                mask = *mat2;
            }

            try {
                cvd->siftdetector->detectAndCompute(*mat1, mask, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr1 = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_siftdetector) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_siftdetector);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_siftdetector) {
                Tcl_GetCommandFullName(interp, cvd->cmd_siftdetector, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::SIFT", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int SIFT(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int nfeatures = 0, nOctaveLayers = 3;
    double contrastThreshold = 0.04, edgeThreshold = 10, sigma = 1.6;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::SIFT> siftdetector;

    if (objc != 1 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "?nfeatures nOctaveLayers contrastThreshold edgeThreshold sigma?");
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[1], &nfeatures) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &nOctaveLayers) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &contrastThreshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[4], &edgeThreshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[5], &sigma) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        siftdetector = cv::SIFT::create(nfeatures, nOctaveLayers,
                                        contrastThreshold, edgeThreshold,
                                        sigma);

        if (siftdetector == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "SIFT nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-siftdetector", -1);

    if (cvd->cmd_siftdetector) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_siftdetector);
    }
    cvd->cmd_siftdetector =
        Tcl_CreateObjCommand(interp, "::cv-siftdetector",
            (Tcl_ObjCmdProc *) SIFT_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) SIFT_DESTRUCTOR);

    cvd->siftdetector = siftdetector;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif /* TCL_USE_SIFT */

static void BFMatcher_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->bfmatcher) {
        cvd->bfmatcher.release();
    }
    cvd->cmd_bfmatcher = NULL;
}


static int BFMatcher_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "match",
        "knnMatch",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_MATCH,
        FUNC_KNNMATCH,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->bfmatcher == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_MATCH: {
            cv::Mat *mat1, *mat2;
            std::vector< cv::DMatch > matches;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "queryDescriptors trainDescriptors");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
            if (!mat2) {
                return TCL_ERROR;
            }

            try {
                cvd->bfmatcher->match(*mat1, *mat2, matches);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < matches.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(matches[i].queryIdx));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(matches[i].trainIdx));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(matches[i].imgIdx));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(matches[i].distance));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_KNNMATCH: {
            cv::Mat *mat1, *mat2;
            std::vector<std::vector<cv::DMatch> > vmatches;
            Tcl_Obj *pResultStr = NULL;
            int k;

            if (objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "queryDescriptors trainDescriptors k");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
            if (!mat2) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[4], &k) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->bfmatcher->knnMatch(*mat1, *mat2, vmatches, k);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < vmatches.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);

                std::vector<cv::DMatch> matches = vmatches[i];
                for (size_t j = 0; j < matches.size(); j++) {
                    Tcl_Obj *pSubListStr = NULL;
                    pSubListStr = Tcl_NewListObj(0, NULL);
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj(matches[j].queryIdx));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj(matches[j].trainIdx));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj(matches[j].imgIdx));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewDoubleObj(matches[j].distance));

                    Tcl_ListObjAppendElement(NULL, pListStr, pSubListStr);
                }

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_bfmatcher) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_bfmatcher);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_bfmatcher) {
                Tcl_GetCommandFullName(interp, cvd->cmd_bfmatcher, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::BFMatcher", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int BFMatcher(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int normType = cv::NORM_L2, crossCheck = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::BFMatcher> bfmatcher;

    if (objc != 1 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "?normType crossCheck?");
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[1], &normType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[2], &crossCheck) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        bfmatcher = cv::BFMatcher::create(normType, (bool) crossCheck);

        if (bfmatcher == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "BFMatcher nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-bfmatcher", -1);

    if (cvd->cmd_bfmatcher) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_bfmatcher);
    }
    cvd->cmd_bfmatcher =
        Tcl_CreateObjCommand(interp, "::cv-bfmatcher",
            (Tcl_ObjCmdProc *) BFMatcher_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) BFMatcher_DESTRUCTOR);

    cvd->bfmatcher = bfmatcher;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void FlannBasedMatcher_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->flannbasedmatcher) {
        cvd->flannbasedmatcher.release();
    }
    cvd->cmd_flannbasedmatcher = NULL;
}


static int FlannBasedMatcher_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "match",
        "knnMatch",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_MATCH,
        FUNC_KNNMATCH,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->flannbasedmatcher == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_MATCH: {
            cv::Mat *mat1, *mat2;
            std::vector< cv::DMatch > matches;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "queryDescriptors trainDescriptors");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
            if (!mat2) {
                return TCL_ERROR;
            }

            try {
                cvd->flannbasedmatcher->match(*mat1, *mat2, matches);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < matches.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(matches[i].queryIdx));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(matches[i].trainIdx));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(matches[i].imgIdx));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(matches[i].distance));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_KNNMATCH: {
            cv::Mat *mat1, *mat2;
            std::vector<std::vector<cv::DMatch> > vmatches;
            Tcl_Obj *pResultStr = NULL;
            int k;

            if (objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "queryDescriptors trainDescriptors k");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
            if (!mat2) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[4], &k) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->flannbasedmatcher->knnMatch(*mat1, *mat2, vmatches, k);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < vmatches.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);

                std::vector<cv::DMatch> matches = vmatches[i];
                for (size_t j = 0; j < matches.size(); j++) {
                    Tcl_Obj *pSubListStr = NULL;
                    pSubListStr = Tcl_NewListObj(0, NULL);
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj(matches[j].queryIdx));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj(matches[j].trainIdx));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewIntObj(matches[j].imgIdx));
                    Tcl_ListObjAppendElement(NULL, pSubListStr, Tcl_NewDoubleObj(matches[j].distance));

                    Tcl_ListObjAppendElement(NULL, pListStr, pSubListStr);
                }

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_flannbasedmatcher) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_flannbasedmatcher);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_flannbasedmatcher) {
                Tcl_GetCommandFullName(interp, cvd->cmd_flannbasedmatcher, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::FlannBasedMatcher", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int FlannBasedMatcher(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    int count = 0, table_number = 6, key_size = 12, multi_probe_level = 1;
    int trees = 5;
    char *algorithm = NULL;
    int len = 0;
    int algo = 0;
    cv::Ptr<cv::FlannBasedMatcher> flannbasedmatcher;

    if (objc != 1 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "?algorithm indexParams?");
        return TCL_ERROR;
    }

    if (objc == 3) {
        algorithm = Tcl_GetStringFromObj(objv[1], &len);
        if (len < 1) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid algorithm name");
        }

        if (strcmp(algorithm, "FLANN_INDEX_LSH") == 0) {
            algo = 0;
        } else if (strcmp(algorithm, "FLANN_INDEX_KDTREE") == 0) {
            algo = 1;
        } else {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid algorithm name");
        }

        if (algo == 0) {
            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count != 3) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid index paramter data");
            } else {
                Tcl_Obj *elemListPtr = NULL;

                Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &table_number) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &key_size) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[2], 2, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &multi_probe_level) != TCL_OK) {
                    return TCL_ERROR;
                }
            }
        } else if (algo == 1) {
            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count != 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid index parameter data");
            } else {
                Tcl_Obj *elemListPtr = NULL;

                Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &trees) != TCL_OK) {
                    return TCL_ERROR;
                }
            }
        }
    }

    try {
        if (algo == 0) {
            flannbasedmatcher = cv::makePtr<cv::FlannBasedMatcher>(
                                                cv::makePtr<cv::flann::LshIndexParams>(
                                                    table_number,
                                                    key_size,
                                                    multi_probe_level
                                                )
                                            );
        } else if (algo == 1) {
            flannbasedmatcher = cv::makePtr<cv::FlannBasedMatcher>(
                                                cv::makePtr<cv::flann::KDTreeIndexParams>(
                                                    trees
                                                )
                                            );
        }

        if (flannbasedmatcher == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "FlannBasedMatcher nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-flannbasedmatcher", -1);

    if (cvd->cmd_flannbasedmatcher) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_flannbasedmatcher);
    }
    cvd->cmd_flannbasedmatcher =
        Tcl_CreateObjCommand(interp, "::cv-flannbasedmatcher",
            (Tcl_ObjCmdProc *) FlannBasedMatcher_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) FlannBasedMatcher_DESTRUCTOR);

    cvd->flannbasedmatcher = flannbasedmatcher;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void SimpleBlobDetector_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->sbdetector) {
        cvd->sbdetector.release();
    }
    cvd->cmd_sbdetector = NULL;
}


static int SimpleBlobDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detect",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->sbdetector == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->sbdetector->detect(*mat, keypoints);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < keypoints.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].pt.y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].size));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].angle));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(keypoints[i].response));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].octave));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(keypoints[i].class_id));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_sbdetector) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_sbdetector);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_sbdetector) {
                Tcl_GetCommandFullName(interp, cvd->cmd_sbdetector, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::SimpleBlobDetector", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int SimpleBlobDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    char *zArg = NULL;
    double minThreshold = 0, maxThreshold = 0;
    double minArea = 0, minCircularity = 0, minConvexity = 0, minInertiaRatio = 0;
    double maxArea = 0, maxCircularity = 0, maxConvexity = 0, maxInertiaRatio = 0;
    int filterByArea = 1, filterByCircularity = 1, filterByConvexity = 1, filterByInertia = 1;
    cv::SimpleBlobDetector::Params params;
    cv::Ptr<cv::SimpleBlobDetector> sbdetector;

    if ((objc&1) != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, "?-minThreshold value? ?-maxThreshold value? ?-filterByArea boolean? ?-minArea value? ?-maxArea value? ?-filterByCircularity boolean? ?-minCircularity value? ?-maxCircularity value? ?-filterByConvexity boolean? ?-minConvexity value? ?-maxConvexity value? ?-filterByInertia boolean? ?-minInertiaRatio value? ?-maxInertiaRatio value?");
        return TCL_ERROR;
    }

    for (int i = 1; i + 1 < objc; i += 2) {
        zArg = Tcl_GetString(objv[i]);

        if (strcmp(zArg, "-minThreshold") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &minThreshold) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minThreshold = (float) minThreshold;
        } else if (strcmp(zArg, "-maxThreshold") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &maxThreshold) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxThreshold = (float) maxThreshold;
        } else if (strcmp(zArg, "-filterByArea") == 0) {
            if (Tcl_GetBooleanFromObj(interp, objv[i+1], &filterByArea) != TCL_OK) {
                return TCL_ERROR;
            }

            params.filterByArea = (bool) filterByArea;
        } else if (strcmp(zArg, "-minArea") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &minArea) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minArea = (float) minArea;
        } else if (strcmp(zArg, "-maxArea") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &maxArea) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxArea = (float) maxArea;
        } else if (strcmp(zArg, "-filterByCircularity") == 0) {
            if (Tcl_GetBooleanFromObj(interp, objv[i+1], &filterByCircularity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.filterByCircularity = (bool) filterByCircularity;
        } else if (strcmp(zArg, "-minCircularity") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &minCircularity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minCircularity = (float) minCircularity;
        } else if (strcmp(zArg, "-maxCircularity") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &maxCircularity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxCircularity = (float) maxCircularity;
        } else if (strcmp(zArg, "-filterByConvexity") == 0) {
            if (Tcl_GetBooleanFromObj(interp, objv[i+1], &filterByConvexity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.filterByConvexity = (bool) filterByConvexity;
        } else if (strcmp(zArg, "-minConvexity") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &minConvexity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minConvexity = (float) minConvexity;
        } else if (strcmp(zArg, "-maxConvexity") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &maxConvexity) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxConvexity = (float) maxConvexity;
        } else if (strcmp(zArg, "-filterByInertia") == 0) {
            if (Tcl_GetBooleanFromObj(interp, objv[i+1], &filterByInertia) != TCL_OK) {
                return TCL_ERROR;
            }

            params.filterByInertia = (bool) filterByInertia;
        } else if (strcmp(zArg, "-minInertiaRatio") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &minInertiaRatio) != TCL_OK) {
                return TCL_ERROR;
            }

            params.minInertiaRatio = (float) minInertiaRatio;
        } else if (strcmp(zArg, "-maxInertiaRatio") == 0) {
            if (Tcl_GetDoubleFromObj(interp, objv[i+1], &maxInertiaRatio) != TCL_OK) {
                return TCL_ERROR;
            }

            params.maxInertiaRatio = (float) maxInertiaRatio;
        } else {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid parameter");
        }
    }

    try {
        sbdetector = cv::SimpleBlobDetector::create(params);

        if (sbdetector == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "SimpleBlobDetector nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-sbdetector", -1);

    if (cvd->cmd_sbdetector) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_sbdetector);
    }
    cvd->cmd_sbdetector =
        Tcl_CreateObjCommand(interp, "::cv-sbdetector",
            (Tcl_ObjCmdProc *) SimpleBlobDetector_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) SimpleBlobDetector_DESTRUCTOR);

    cvd->sbdetector = sbdetector;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int BOWKMeansTrainer_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::BOWKMeansTrainer *bowkmeanstrainer;

    static const char *FUNC_strs[] = {
        "add",
        "clear",
        "cluster",
        "descriptorsCount",
        "getDescriptors",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_ADD,
        FUNC_CLEAR,
        FUNC_CLUSTER,
        FUNC_descriptorsCount,
        FUNC_getDescriptors,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    cd = cvo->top;
    bowkmeanstrainer = (cv::BOWKMeansTrainer *) cvo->obj;
    if (!bowkmeanstrainer) {
        Tcl_Panic("null BOWKMeansTrainer object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_ADD: {
            cv::Mat *mat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "descriptors");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                bowkmeanstrainer->add(*mat);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLEAR: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                bowkmeanstrainer->clear();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLUSTER: {
            cv::Mat result;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2 && objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?descriptors?");
                return TCL_ERROR;
            }

            if (objc == 3) {
                mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
                if (!mat) {
                    return TCL_ERROR;
                }
            }

            try {
                if (objc == 2) {
                    result = bowkmeanstrainer->cluster();
                } else {
                    result = bowkmeanstrainer->cluster(*mat);
                }
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(result);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_descriptorsCount: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = bowkmeanstrainer->descriptorsCount();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDescriptors: {
            std::vector<cv::Mat> value;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = bowkmeanstrainer->getDescriptors();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);

            for (size_t i = 0; i < value.size(); i++) {
                Tcl_Obj *pMatResultStr = NULL;
                cv::Mat *dstmat;

                dstmat = new cv::Mat(value[i]);
                pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_DeleteCommandFromToken(interp, cvo->cmd);

            break;
        }
        case FUNC__COMMAND: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__NAME: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(cvo->key, -1));
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::BOWKMeansTrainer", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int BOWKMeansTrainer(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Tcl_Obj *pResultStr = NULL;
    int clusterCount = 0, attempts = 3, flags = cv::KMEANS_PP_CENTERS;
    cv::TermCriteria *termCriteria;
    cv::BOWKMeansTrainer *bowkmeanstrainer;

    if (objc != 3 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "clusterCount termCriteria ?attempts flags?");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &clusterCount) != TCL_OK) {
        return TCL_ERROR;
    }

    termCriteria = (cv::TermCriteria *) Opencv_FindHandle(cd, interp, OPENCV_TERMCRITERIA, objv[2]);
    if (!termCriteria) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[3], &attempts) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[4], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        bowkmeanstrainer = new cv::BOWKMeansTrainer(clusterCount, *termCriteria,
                                                    attempts, flags);

        if (bowkmeanstrainer == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "BOWKMeansTrainer nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_BOWTRAINER, bowkmeanstrainer);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int BOWImgDescriptorExtractor_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::BOWImgDescriptorExtractor *bowimgextractor;

    static const char *FUNC_strs[] = {
        "compute",
        "descriptorSize",
        "descriptorType",
        "getVocabulary",
        "setVocabulary",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_compute,
        FUNC_descriptorSize,
        FUNC_descriptorType,
        FUNC_getVocabulary,
        FUNC_setVocabulary,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    cd = cvo->top;
    bowimgextractor = (cv::BOWImgDescriptorExtractor *) cvo->obj;
    if (!bowimgextractor) {
        Tcl_Panic("null BOWImgDescriptorExtractor object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_compute: {
            cv::Mat descriptors;
            int count = 0;
            std::vector< cv::KeyPoint > keypoints;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix keypoints");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "no keypoints data");
            } else {
                for (int i = 0; i < count; i++) {
                    Tcl_Obj *elemListPtr = NULL;
                    int sub_count = 0;
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);

                    if (Tcl_ListObjLength(interp, elemListPtr, &sub_count) != TCL_OK) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid keypoints data");
                    }

                    if (sub_count != 7) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong  keypoints number");
                    } else {
                        Tcl_Obj *elemListSubPtr = NULL;
                        double x, y, size, angle, response;
                        int octave, class_id;

                        Tcl_ListObjIndex(interp, elemListPtr, 0, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &x) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 1, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &y) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 2, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &size) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 3, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &angle) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 4, &elemListSubPtr);
                        if (Tcl_GetDoubleFromObj(interp, elemListSubPtr, &response) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 5, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &octave) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        Tcl_ListObjIndex(interp, elemListPtr, 6, &elemListSubPtr);
                        if (Tcl_GetIntFromObj(interp, elemListSubPtr, &class_id) != TCL_OK) {
                            return TCL_ERROR;
                        }

                        cv::KeyPoint keypoint(cv::Point2f((float) x, (float)y), (float) size,
                                            (float) angle, (float) response,
                                            octave, class_id);

                        keypoints.push_back(keypoint);
                    }
                }
            }

            try {
                bowimgextractor->compute(*mat, keypoints, descriptors);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(descriptors);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_descriptorSize: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = bowimgextractor->descriptorSize();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_descriptorType: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = bowimgextractor->descriptorType();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getVocabulary: {
            cv::Mat result;
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                result = bowimgextractor->getVocabulary();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(result);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_setVocabulary: {
            cv::Mat *mat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "vocabulary");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                bowimgextractor->setVocabulary(*mat);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_DeleteCommandFromToken(interp, cvo->cmd);

            break;
        }
        case FUNC__COMMAND: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__NAME: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(cvo->key, -1));
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::BOWImgDescriptorExtractor", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int BOWImgDescriptorExtractor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr< cv::DescriptorExtractor > dextractor;
    cv::Ptr< cv::DescriptorMatcher > dmatcher;
    cv::BOWImgDescriptorExtractor *bowimgextractor;
    char *dextractor_name = NULL, *dmatcher_name = NULL;
    int len = 0;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "dextractor dmatcher");
        return TCL_ERROR;
    }

    dextractor_name = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid dextractor name");
    }

    if (strcmp(dextractor_name, "::cv-siftdetector") == 0) {
        if (cvd->siftdetector) {
            dextractor = cvd->siftdetector;
        } else {
            CV_Error(cv::Error::StsNullPtr, "siftdetector nullptr");
        }
    } else {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid dextractor name");
    }

    dmatcher_name = Tcl_GetStringFromObj(objv[2], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid dmatcher name");
    }

    if (strcmp(dmatcher_name, "::cv-flannbasedmatcher") == 0) {
        if (cvd->flannbasedmatcher) {
            dmatcher = cvd->flannbasedmatcher;
        } else {
            CV_Error(cv::Error::StsNullPtr, "flannbasedmatcher nullptr");
        }
    } else {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid dmatcher name");
    }

    try {
        bowimgextractor = new cv::BOWImgDescriptorExtractor(dextractor, dmatcher);

        if (bowimgextractor == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "BOWImgDescriptorExtractor nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_BOWEXTRACTOR, bowimgextractor);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
