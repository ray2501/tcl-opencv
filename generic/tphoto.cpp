#include "tclopencv.h"

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
#ifdef __cplusplus
}
#endif
