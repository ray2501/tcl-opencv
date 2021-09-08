#include "tclopencv.h"

/*
 * OpenCV BackgroundSubtractorMOG2 uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::BackgroundSubtractorMOG2 > subtractorMOG2;

#ifdef __cplusplus
extern "C" {
#endif

int SubtractorMOG2_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "apply",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_APPLY,
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
        case FUNC_APPLY: {
            cv::Mat image;
            cv::Mat result_image;
            Tcl_HashEntry *hashEntryPtr;
            char *handle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *info;
            MatrixInfo *dstinfo;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "apply invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info ) {
                Tcl_SetResult(interp, (char *) "apply invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                subtractorMOG2->apply(image, result_image);
            } catch (...){
                Tcl_SetResult(interp, (char *) "apply failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "apply malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(result_image);

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

            subtractorMOG2.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int BackgroundSubtractorMOG2(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int history = 0, detectShadows = 1;
    double varThreshold = 16;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "history varThreshold detectShadows");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &history) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &varThreshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetBooleanFromObj(interp, objv[3], &detectShadows) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        subtractorMOG2 = cv::createBackgroundSubtractorMOG2( history, varThreshold, (bool) detectShadows);

        if (subtractorMOG2 == nullptr) {
            Tcl_SetResult(interp, (char *) "BackgroundSubtractorMOG2 create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "BackgroundSubtractorMOG2 failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-subtractorMOG2", -1 );

    Tcl_CreateObjCommand(interp, "cv-subtractorMOG2", (Tcl_ObjCmdProc *) SubtractorMOG2_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int meanShift(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int retval = 0;
    int x = 0, y = 0, width = 0, height = 0;
    cv::Rect window;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *thandle;
    MatrixInfo *info1;
    TermCriteriaInfo *term_info;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix x y width height termCriteria");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "meanShift invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "meanShift invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    thandle = Tcl_GetStringFromObj(objv[6], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, thandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "meanShift invalid TermCriteria handle ",
                                    thandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    term_info = (TermCriteriaInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !term_info ) {
        Tcl_SetResult(interp, (char *) "meanShift invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        window = cv::Rect(x, y, width, height);
        retval = cv::meanShift(*(info1->matrix), window, *(term_info->termCriteria));
    } catch (...){
        Tcl_SetResult(interp, (char *) "meanShift failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( retval ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( window.x ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( window.y ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( window.width ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( window.height ));

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int CamShift(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int x = 0, y = 0, width = 0, height = 0;
    cv::Rect window;
    cv::RotatedRect rect_result;
    cv::Point2f points[4];
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *thandle;
    MatrixInfo *info1;
    TermCriteriaInfo *term_info;
    Tcl_Obj *pResultStr = NULL;
    Tcl_Obj *pSubListStr1 = NULL;
    Tcl_Obj *pSubListStr2 = NULL;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix x y width height termCriteria");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "CamShift invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "CamShift invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    thandle = Tcl_GetStringFromObj(objv[6], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, thandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "CamShift invalid TermCriteria handle ",
                                    thandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    term_info = (TermCriteriaInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !term_info ) {
        Tcl_SetResult(interp, (char *) "CamShift invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        window = cv::Rect(x, y, width, height);
        rect_result = cv::CamShift(*(info1->matrix), window, *(term_info->termCriteria));
    } catch (...){
        Tcl_SetResult(interp, (char *) "CamShift failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    pSubListStr1  = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubListStr1, Tcl_NewIntObj( window.x ));
    Tcl_ListObjAppendElement(NULL, pSubListStr1, Tcl_NewIntObj( window.y ));
    Tcl_ListObjAppendElement(NULL, pSubListStr1, Tcl_NewIntObj( window.width ));
    Tcl_ListObjAppendElement(NULL, pSubListStr1, Tcl_NewIntObj( window.height ));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubListStr1 );
    rect_result.points(points);

    pSubListStr2  = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj( (int ) points[0].x ));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj( (int ) points[0].y ));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj( (int ) points[1].x ));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj( (int ) points[1].y ));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj( (int ) points[2].x ));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj( (int ) points[2].y ));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj( (int ) points[3].x ));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj( (int ) points[3].y ));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubListStr2 );

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int calcOpticalFlowPyrLK(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int width = 0, height = 0, maxLevel = 3;
    cv::Mat nextPts, status, err;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle, *nhandle, *ptshandle, *thandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1, *info2, *info3;
    MatrixInfo *dstinfo, *statusinfo, *errinfo;
    TermCriteriaInfo *term_info;
    Tcl_Obj *pResultStr = NULL;
    Tcl_Obj *pResultStr1 = NULL, *pResultStr2 = NULL, *pResultStr3 = NULL;

    if (objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "prevImg nextImg prevPts winSize_width winSize_height maxLevel termCriteria");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcOpticalFlowPyrLK invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    nhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, nhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcOpticalFlowPyrLK invalid MATRIX handle ",
                                    nhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    ptshandle = Tcl_GetStringFromObj(objv[3], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ptshandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcOpticalFlowPyrLK invalid MATRIX handle ",
                                    ptshandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info3 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[6], &maxLevel) != TCL_OK) {
        return TCL_ERROR;
    }

    thandle = Tcl_GetStringFromObj(objv[7], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, thandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcOpticalFlowPyrLK invalid TermCriteria handle ",
                                    thandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    term_info = (TermCriteriaInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !term_info ) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::calcOpticalFlowPyrLK(*(info1->matrix), *(info2->matrix),*(info3->matrix),
                                 nextPts, status, err, cv::Size(width, height), maxLevel,
                                 *(term_info->termCriteria));
    } catch (...){
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(nextPts);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr1 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, dstinfo);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    statusinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!statusinfo) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    statusinfo->matrix = new cv::Mat(status);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr2 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, statusinfo);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    errinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!errinfo) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    errinfo->matrix = new cv::Mat(err);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr3 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, errinfo);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1 );
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2 );
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr3 );

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int calcOpticalFlowFarneback(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double pyr_scale = 0, poly_sigma = 0;
    int levels = 0, winsize = 0, iterations = 0, poly_n = 0, flags = 0;
    cv::Mat flow;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle, *nhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "prevImg nextImg pyr_scale levels winsize iterations poly_n poly_sigma flags");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcOpticalFlowFarneback invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowFarneback invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    nhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, nhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcOpticalFlowFarneback invalid MATRIX handle ",
                                    nhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowFarneback invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &pyr_scale) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &levels) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &winsize) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[6], &iterations) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[7], &poly_n) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[8], &poly_sigma) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[9], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::calcOpticalFlowFarneback(*(info1->matrix), *(info2->matrix),
                                 flow, pyr_scale, levels, winsize,
                                 iterations, poly_n, poly_sigma, flags);
    } catch (...){
        Tcl_SetResult(interp, (char *) "calcOpticalFlowFarneback failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowFarneback malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(flow);

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
