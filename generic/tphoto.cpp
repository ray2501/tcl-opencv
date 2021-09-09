#include "tclopencv.h"

/*
 * OpenCV AlignMTB uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::AlignMTB > alignMTB;

/*
 * OpenCV CalibrateDebevec uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::CalibrateDebevec > calibrateDebevec;

/*
 * OpenCV MergeDebevec uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::MergeDebevec > mergeDebevec;

/*
 * OpenCV MergeMertens uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::MergeMertens > mergeMertens;

/*
 * OpenCV TonemapDrago uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::TonemapDrago > tonemapDrago;

/*
 * OpenCV TonemapMantiuk uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::TonemapMantiuk > tonemapMantiuk;

/*
 * OpenCV TonemapReinhard uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::TonemapReinhard > tonemapReinhard;

#ifdef __cplusplus
extern "C" {
#endif


int inpaint(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double inpaintRadius = 0;
    int flags = 0;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle, *mhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix inpaintMask inpaintRadius flags");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "inpaint invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "inpaint invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    mhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "inpaint invalid MATRIX handle ",
                                    mhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "inpaint invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &inpaintRadius) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::inpaint(*(info1->matrix), *(info2->matrix), dstimage,
                    inpaintRadius, flags);
    } catch (...){
        Tcl_SetResult(interp, (char *) "inpaint failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "inpaint malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int decolor(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat dstimage1, dstimage2;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *dstinfo1, *dstinfo2;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "decolor invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "decolor invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::decolor(*(info1->matrix), dstimage1, dstimage2);
    } catch (...){
        Tcl_SetResult(interp, (char *) "decolor failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo1 = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo1) {
        Tcl_SetResult(interp, (char *) "decolor malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo1->matrix = new cv::Mat(dstimage1);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr1 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, dstinfo1);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    dstinfo2 = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo2) {
        Tcl_SetResult(interp, (char *) "decolor malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo2->matrix = new cv::Mat(dstimage2);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr2 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, dstinfo2);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1 );
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2 );

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int fastNlMeansDenoisingColored(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double h = 3, hColor = 3;
    int templateWindowSize = 7, searchWindowSize = 21;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix ?h hColor templateWindowSize searchWindowSize?");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "fastNlMeansDenoisingColored invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "fastNlMeansDenoisingColored invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetDoubleFromObj(interp, objv[2], &h) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &hColor) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[4], &templateWindowSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &searchWindowSize) != TCL_OK) {
            return TCL_ERROR;
        }
    }


    try {
        cv::fastNlMeansDenoisingColored(*(info1->matrix), dstimage,
                                        (float) h, (float) hColor,
                                        templateWindowSize, searchWindowSize);
    } catch (...){
        Tcl_SetResult(interp, (char *) "fastNlMeansDenoisingColored failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "fastNlMeansDenoisingColored malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int colorChange(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double red_mul = 1.0, green_mul = 1.0, blue_mul = 1.0;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle, *mhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix mask red_mul green_mul blue_mul");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "colorChange invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "colorChange invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    mhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "colorChange invalid MATRIX handle ",
                                    mhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "colorChange invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &red_mul) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &green_mul) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[5], &blue_mul) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::colorChange(*(info1->matrix), *(info2->matrix), dstimage,
                        (float) red_mul, (float) green_mul, (float) blue_mul);
    } catch (...){
        Tcl_SetResult(interp, (char *) "colorChange failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "colorChange malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int illuminationChange(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double alpha = 0.2, beta = 0.4;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle, *mhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix mask alpha beta");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "illuminationChange invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "illuminationChange invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    mhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "illuminationChange invalid MATRIX handle ",
                                    mhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "illuminationChange invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    if (alpha < 0 || alpha > 2) {
        Tcl_SetResult(interp, (char *) "illuminationChange alpha out of range", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &beta) != TCL_OK) {
        return TCL_ERROR;
    }

    if (beta < 0 || beta > 2) {
        Tcl_SetResult(interp, (char *) "illuminationChange beta out of range", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::illuminationChange(*(info1->matrix), *(info2->matrix), dstimage,
                        (float) alpha, (float) beta);
    } catch (...){
        Tcl_SetResult(interp, (char *) "illuminationChange failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "illuminationChange malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int textureFlattening(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double low_threshold = 30, high_threshold = 145;
    int kernel_size = 3;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle, *mhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix mask low_threshold high_threshold ?kernel_size?");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "textureFlattening invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "textureFlattening invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    mhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "textureFlattening invalid MATRIX handle ",
                                    mhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "textureFlattening invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &low_threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (low_threshold < 0 || low_threshold > 100) {
        Tcl_SetResult(interp, (char *) "textureFlattening low_threshold out of range", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &high_threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (high_threshold < 100) {
        Tcl_SetResult(interp, (char *) "textureFlattening high_threshold out of range", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[5], &kernel_size) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::textureFlattening(*(info1->matrix), *(info2->matrix), dstimage,
                (float) low_threshold, (float) high_threshold, kernel_size);
    } catch (...){
        Tcl_SetResult(interp, (char *) "textureFlattening failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "textureFlattening malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int seamlessClone(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int x = 0, y = 0, flags = cv::NORMAL_CLONE;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle, *dhandle, *mhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1, *info2, *info3;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix dst_matrix mask x y flags");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "seamlessClone invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "seamlessClone invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    dhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, dhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "seamlessClone invalid MATRIX handle ",
                                    dhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "seamlessClone invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    mhandle = Tcl_GetStringFromObj(objv[3], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "seamlessClone invalid MATRIX handle ",
                                    mhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info3 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info3 ) {
        Tcl_SetResult(interp, (char *) "seamlessClone invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    /*
     * Location of the center of the source image in the destination image.
     * Get x location.
     */
    if(Tcl_GetIntFromObj(interp, objv[4], &x) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Location of the center of the source image in the destination image.
     * Get y location.
     */
    if(Tcl_GetIntFromObj(interp, objv[5], &y) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[6], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::seamlessClone(*(info1->matrix), *(info2->matrix), *(info3->matrix),
                          cv::Point(x, y), dstimage, flags);
    } catch (...){
        Tcl_SetResult(interp, (char *) "seamlessClone failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "seamlessClone malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int detailEnhance(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double sigma_s = 10, sigma_r = 0.15;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?sigma_s sigma_r?");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "detailEnhance invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "detailEnhance invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetDoubleFromObj(interp, objv[2], &sigma_s) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &sigma_r) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::detailEnhance(*(info1->matrix), dstimage, sigma_s, sigma_r);
    } catch (...){
        Tcl_SetResult(interp, (char *) "detailEnhance failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "detailEnhance malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int edgePreservingFilter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int flags = 1;
    double sigma_s = 60, sigma_r = 0.4;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?flags sigma_s sigma_r?");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "edgePreservingFilter invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "edgePreservingFilter invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &sigma_s) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[4], &sigma_r) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::edgePreservingFilter(*(info1->matrix), dstimage, flags, (float) sigma_s, (float) sigma_r);
    } catch (...){
        Tcl_SetResult(interp, (char *) "edgePreservingFilter failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "edgePreservingFilter malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int pencilSketch(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double sigma_s = 60, sigma_r = 0.07, shade_factor = 0.02;
    cv::Mat dstimage1, dstimage2;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *dstinfo1, *dstinfo2;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?sigma_s sigma_r shade_factor?");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "pencilSketch invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "pencilSketch invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetDoubleFromObj(interp, objv[2], &sigma_s) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &sigma_r) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[4], &shade_factor) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::pencilSketch(*(info1->matrix), dstimage1, dstimage2,
                         (float) sigma_s, (float) sigma_r, (float) shade_factor);
    } catch (...){
        Tcl_SetResult(interp, (char *) "pencilSketch failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo1 = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo1) {
        Tcl_SetResult(interp, (char *) "pencilSketch malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo1->matrix = new cv::Mat(dstimage1);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr1 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, dstinfo1);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    dstinfo2 = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo2) {
        Tcl_SetResult(interp, (char *) "pencilSketch malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo2->matrix = new cv::Mat(dstimage2);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr2 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, dstinfo2);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1 );
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2 );

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int stylization(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double sigma_s = 60, sigma_r = 0.45;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?sigma_s sigma_r?");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "stylization invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "stylization invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetDoubleFromObj(interp, objv[2], &sigma_s) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &sigma_r) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::stylization(*(info1->matrix), dstimage, sigma_s, sigma_r);
    } catch (...){
        Tcl_SetResult(interp, (char *) "stylization failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "stylization malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(dstimage);

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


int AlignMTB_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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
        case FUNC_PROCESS: {
            std::vector< cv::Mat > src;
            std::vector< cv::Mat > dst;
            cv::Mat image;
            int count;
            Tcl_Obj *pResultStr = NULL;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix_list");
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "process invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0 || count%2 != 0) {
                Tcl_SetResult(interp, (char *) "process matrix_list invalid data", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListPtr = NULL;
                Tcl_HashEntry *hashEntryPtr;
                char *matrix_handle;
                MatrixInfo *info;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                    matrix_handle = Tcl_GetStringFromObj(elemListPtr, 0);
                    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, matrix_handle );
                    if( !hashEntryPtr ) {
                        if( interp ) {
                            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                            Tcl_AppendStringsToObj( resultObj, "process invalid MATRIX handle ",
                                                    matrix_handle, (char *)NULL );
                        }

                        return TCL_ERROR;
                    }

                    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
                    if ( !info ) {
                        Tcl_SetResult(interp, (char *) "process invalid info data", TCL_STATIC);
                        return TCL_ERROR;
                    }

                    image = *(info->matrix);
                    src.push_back(image);
                }
            }

            try {
                alignMTB->process(src, dst);
            } catch (...){
                Tcl_SetResult(interp, (char *) "process failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < dst.size(); i++) {
                Tcl_HashEntry *newHashEntryPtr;
                char handleName[16 + TCL_INTEGER_SPACE];
                Tcl_Obj *pSubResultStr = NULL;
                int newvalue;
                MatrixInfo *dstinfo;
                cv::Mat dstimage;

                dstimage = dst[i];

                dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
                if (!dstinfo) {
                    Tcl_SetResult(interp, (char *) "process malloc MatrixInfo failed", TCL_STATIC);
                    return TCL_ERROR;
                }

                dstinfo->matrix = new cv::Mat(dstimage);

                Tcl_MutexLock(&myMutex);
                sprintf( handleName, "cv-mat%zd", matrix_count++ );

                pSubResultStr = Tcl_NewStringObj( handleName, -1 );

                newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
                Tcl_SetHashValue(newHashEntryPtr, dstinfo);
                Tcl_MutexUnlock(&myMutex);

                Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

                Tcl_ListObjAppendElement(NULL, pResultStr, pSubResultStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            alignMTB.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int AlignMTB(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int max_bits = 6, exclude_range = 4, cut = 1;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?max_bits exclude_range cut?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetIntFromObj(interp, objv[1], &max_bits) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[2], &exclude_range) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[3], &cut) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        alignMTB = cv::createAlignMTB( max_bits, exclude_range, (bool) cut);

        if (alignMTB == nullptr) {
            Tcl_SetResult(interp, (char *) "alignMTB create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "AlignMTB failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-alignMTB", -1 );

    Tcl_CreateObjCommand(interp, "cv-alignMTB", (Tcl_ObjCmdProc *) AlignMTB_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int CalibrateDebevec_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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
        case FUNC_PROCESS: {
            int count;
            cv::Mat image, responseDebevec;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            int newvalue;
            MatrixInfo *dstinfo;
            Tcl_Obj *pResultStr = NULL;
            std::vector< cv::Mat > src;
            std::vector< float > timesArray;

            if( objc != 4 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix_list times_list");
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "process invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0) {
                Tcl_SetResult(interp, (char *) "process matrix_list invalid data", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListPtr = NULL;
                Tcl_HashEntry *hashEntryPtr;
                char *matrix_handle;
                MatrixInfo *info;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                    matrix_handle = Tcl_GetStringFromObj(elemListPtr, 0);
                    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, matrix_handle );
                    if( !hashEntryPtr ) {
                        if( interp ) {
                            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                            Tcl_AppendStringsToObj( resultObj, "process invalid MATRIX handle ",
                                                    matrix_handle, (char *)NULL );
                        }

                        return TCL_ERROR;
                    }

                    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
                    if ( !info ) {
                        Tcl_SetResult(interp, (char *) "process invalid info data", TCL_STATIC);
                        return TCL_ERROR;
                    }

                    image = *(info->matrix);
                    src.push_back(image);
                }
            }

            if(Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "process invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0) {
                Tcl_SetResult(interp, (char *) "process times_list invalid data", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListPtr = NULL;
                double value = 0;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);
                    if(Tcl_GetDoubleFromObj(interp, elemListPtr, &value) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    timesArray.push_back( (float) value);
                }
            }

            try {
                calibrateDebevec->process(src, responseDebevec, timesArray);
            } catch (...){
                Tcl_SetResult(interp, (char *) "process failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "process malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(responseDebevec);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            calibrateDebevec.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int CalibrateDebevec(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int samples = 70, random = 0;
    double lambda = 10.0;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?samples lambda random?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetIntFromObj(interp, objv[1], &samples) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[2], &lambda) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[3], &random) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        calibrateDebevec = cv::createCalibrateDebevec( samples, (float) lambda, (bool) random);

        if (calibrateDebevec == nullptr) {
            Tcl_SetResult(interp, (char *) "calibrateDebevec create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "CalibrateDebevec failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-calibrateDebevec", -1 );

    Tcl_CreateObjCommand(interp, "cv-calibrateDebevec", (Tcl_ObjCmdProc *) CalibrateDebevec_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int MergeDebevec_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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
        case FUNC_PROCESS: {
            int count;
            cv::Mat image, dst;
            Tcl_HashEntry *hashEntryPtr;
            char *rhandle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            int newvalue;
            MatrixInfo *info;
            MatrixInfo *dstinfo;
            Tcl_Obj *pResultStr = NULL;
            std::vector< cv::Mat > src;
            std::vector< float > timesArray;

            if( objc != 5 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix_list times_list response");
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "process invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0) {
                Tcl_SetResult(interp, (char *) "process matrix_list invalid data", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListPtr = NULL;
                Tcl_HashEntry *hashEntryPtr;
                char *matrix_handle;
                MatrixInfo *info;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                    matrix_handle = Tcl_GetStringFromObj(elemListPtr, 0);
                    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, matrix_handle );
                    if( !hashEntryPtr ) {
                        if( interp ) {
                            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                            Tcl_AppendStringsToObj( resultObj, "process invalid MATRIX handle ",
                                                    matrix_handle, (char *)NULL );
                        }

                        return TCL_ERROR;
                    }

                    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
                    if ( !info ) {
                        Tcl_SetResult(interp, (char *) "process invalid info data", TCL_STATIC);
                        return TCL_ERROR;
                    }

                    image = *(info->matrix);
                    src.push_back(image);
                }
            }

            if(Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "process invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0) {
                Tcl_SetResult(interp, (char *) "process times_list invalid data", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListPtr = NULL;
                double value = 0;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);
                    if(Tcl_GetDoubleFromObj(interp, elemListPtr, &value) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    timesArray.push_back( (float) value);
                }
            }

            rhandle = Tcl_GetStringFromObj(objv[4], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, rhandle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "process invalid MATRIX handle ",
                                            rhandle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "process invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                mergeDebevec->process(src, dst, timesArray, *(info->matrix));
            } catch (...){
                Tcl_SetResult(interp, (char *) "process failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "process malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(dst);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            mergeDebevec.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int MergeDebevec(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 ) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        mergeDebevec = cv::createMergeDebevec( );

        if (mergeDebevec == nullptr) {
            Tcl_SetResult(interp, (char *) "mergeDebevec create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "MergeDebevec failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-mergeDebevec", -1 );

    Tcl_CreateObjCommand(interp, "cv-mergeDebevec", (Tcl_ObjCmdProc *) MergeDebevec_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int MergeMertens_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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
        case FUNC_PROCESS: {
            int count;
            cv::Mat image, dst;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            int newvalue;
            MatrixInfo *dstinfo;
            Tcl_Obj *pResultStr = NULL;
            std::vector< cv::Mat > src;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix_list");
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "process invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0) {
                Tcl_SetResult(interp, (char *) "process matrix_list invalid data", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListPtr = NULL;
                Tcl_HashEntry *hashEntryPtr;
                char *matrix_handle;
                MatrixInfo *info;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                    matrix_handle = Tcl_GetStringFromObj(elemListPtr, 0);
                    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, matrix_handle );
                    if( !hashEntryPtr ) {
                        if( interp ) {
                            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                            Tcl_AppendStringsToObj( resultObj, "process invalid MATRIX handle ",
                                                    matrix_handle, (char *)NULL );
                        }

                        return TCL_ERROR;
                    }

                    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
                    if ( !info ) {
                        Tcl_SetResult(interp, (char *) "process invalid info data", TCL_STATIC);
                        return TCL_ERROR;
                    }

                    image = *(info->matrix);
                    src.push_back(image);
                }
            }

            try {
                mergeMertens->process(src, dst);
            } catch (...){
                Tcl_SetResult(interp, (char *) "process failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "process malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(dst);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            mergeMertens.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int MergeMertens(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double contrast_weight = 1.0, saturation_weight = 1.0, exposure_weight = 0.0;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?contrast_weight saturation_weight exposure_weight?");
        return TCL_ERROR;
    }

    if (objc == 4 ) {
        if(Tcl_GetDoubleFromObj(interp, objv[1], &contrast_weight) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[2], &saturation_weight) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &exposure_weight) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        mergeMertens = cv::createMergeMertens( (float) contrast_weight,
                                               (float) saturation_weight,
                                               (float) exposure_weight );

        if (mergeMertens == nullptr) {
            Tcl_SetResult(interp, (char *) "mergeMertens create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "mergeMertens failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-mergeMertens", -1 );

    Tcl_CreateObjCommand(interp, "cv-mergeMertens", (Tcl_ObjCmdProc *) MergeMertens_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int TonemapDrago_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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
        case FUNC_PROCESS: {
            cv::Mat image, dst;
            Tcl_HashEntry *hashEntryPtr;
            char *rhandle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            int newvalue;
            MatrixInfo *info;
            MatrixInfo *dstinfo;
            Tcl_Obj *pResultStr = NULL;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "hdrDebevec");
                return TCL_ERROR;
            }

            rhandle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, rhandle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "process invalid MATRIX handle ",
                                            rhandle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "process invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                tonemapDrago->process(*(info->matrix), dst);
            } catch (...){
                Tcl_SetResult(interp, (char *) "process failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "process malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(dst);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            tonemapDrago.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int TonemapDrago(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    Tcl_Obj *pResultStr = NULL;
    double gamma = 1.0, saturation = 1.0, bias = 0.85;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?gamma saturation bias?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetDoubleFromObj(interp, objv[1], &gamma) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[2], &saturation) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &bias) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        tonemapDrago = cv::createTonemapDrago( gamma, saturation, bias);

        if (tonemapDrago == nullptr) {
            Tcl_SetResult(interp, (char *) "tonemapDrago create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "TonemapDrago failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-tonemapDrago", -1 );

    Tcl_CreateObjCommand(interp, "cv-tonemapDrago", (Tcl_ObjCmdProc *) TonemapDrago_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int TonemapMantiuk_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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
        case FUNC_PROCESS: {
            cv::Mat image, dst;
            Tcl_HashEntry *hashEntryPtr;
            char *rhandle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            int newvalue;
            MatrixInfo *info;
            MatrixInfo *dstinfo;
            Tcl_Obj *pResultStr = NULL;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "hdrDebevec");
                return TCL_ERROR;
            }

            rhandle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, rhandle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "process invalid MATRIX handle ",
                                            rhandle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "process invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                tonemapMantiuk->process(*(info->matrix), dst);
            } catch (...){
                Tcl_SetResult(interp, (char *) "process failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "process malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(dst);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            tonemapMantiuk.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int TonemapMantiuk(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    Tcl_Obj *pResultStr = NULL;
    double gamma = 1.0, scale = 0.7, saturation = 1.0;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?gamma scale saturation?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetDoubleFromObj(interp, objv[1], &gamma) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[2], &scale) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &saturation) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        tonemapMantiuk = cv::createTonemapMantiuk( (float) gamma, (float) scale, (float) saturation);

        if (tonemapMantiuk == nullptr) {
            Tcl_SetResult(interp, (char *) "tonemapMantiuk create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "TonemapMantiuk failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-tonemapMantiuk", -1 );

    Tcl_CreateObjCommand(interp, "cv-tonemapMantiuk", (Tcl_ObjCmdProc *) TonemapMantiuk_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int TonemapReinhard_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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
        case FUNC_PROCESS: {
            cv::Mat image, dst;
            Tcl_HashEntry *hashEntryPtr;
            char *rhandle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            int newvalue;
            MatrixInfo *info;
            MatrixInfo *dstinfo;
            Tcl_Obj *pResultStr = NULL;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "hdrDebevec");
                return TCL_ERROR;
            }

            rhandle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, rhandle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "process invalid MATRIX handle ",
                                            rhandle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "process invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                tonemapReinhard->process(*(info->matrix), dst);
            } catch (...){
                Tcl_SetResult(interp, (char *) "process failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "process malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(dst);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            tonemapReinhard.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int TonemapReinhard(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    Tcl_Obj *pResultStr = NULL;
    double gamma = 1.0, intensity = 0.0, light_adapt = 1.0, color_adapt = 0.0;

    if (objc != 1 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "?gamma intensity light_adapt color_adapt?");
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetDoubleFromObj(interp, objv[1], &gamma) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[2], &intensity) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &light_adapt) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[4], &color_adapt) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        tonemapReinhard = cv::createTonemapReinhard( (float) gamma, (float) intensity,
                                                     (float) light_adapt, (float) color_adapt);

        if (tonemapReinhard == nullptr) {
            Tcl_SetResult(interp, (char *) "tonemapReinhard create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "TonemapReinhard failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-tonemapReinhard", -1 );

    Tcl_CreateObjCommand(interp, "cv-tonemapReinhard", (Tcl_ObjCmdProc *) TonemapReinhard_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
