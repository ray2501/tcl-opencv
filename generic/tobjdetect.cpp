#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif


int QRCodeDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    QRCodeDetectorInfo *info;

    static const char *FUNC_strs[] = {
        "detect",
        "detectAndDecode",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_DETECTANDDECODE,
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
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "QRCodeDetector: invalid QRCodeDetector handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (QRCodeDetectorInfo *) Tcl_GetHashValue( hashEntryPtr );

    switch( (enum FUNC_enum)choice ){
        case FUNC_DETECT: {
            Tcl_HashEntry *hashImageEntryPtr;
            char *imagehandle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *srcinfo;
            MatrixInfo *dstinfo;
            cv::Mat points_matrix;
            bool result;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            imagehandle = Tcl_GetStringFromObj(objv[2], 0);
            hashImageEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, imagehandle );
            if( !hashImageEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "detect invalid MATRIX handle ",
                                            imagehandle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            srcinfo = (MatrixInfo *) Tcl_GetHashValue( hashImageEntryPtr );
            if ( !srcinfo ) {
                Tcl_SetResult(interp, (char *) "detect invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                result = info->qrcodedetector->detect(*(srcinfo->matrix), points_matrix);
                if (!result) {
                    Tcl_SetResult(interp, (char *) "detect result failed", TCL_STATIC);
                    return TCL_ERROR;
                }
            } catch (...){
                Tcl_SetResult(interp, (char *) "detect failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "detect malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(points_matrix);

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
        case FUNC_DETECTANDDECODE: {
            Tcl_HashEntry *hashImageEntryPtr;
            char *imagehandle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
            int newvalue;
            MatrixInfo *srcinfo;
            MatrixInfo *dstinfo, *code_info;
            cv::Mat points_matrix, code_matrix;
            std::string result;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            imagehandle = Tcl_GetStringFromObj(objv[2], 0);
            hashImageEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, imagehandle );
            if( !hashImageEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "detectAndDecode invalid MATRIX handle ",
                                            imagehandle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            srcinfo = (MatrixInfo *) Tcl_GetHashValue( hashImageEntryPtr );
            if ( !srcinfo ) {
                Tcl_SetResult(interp, (char *) "detectAndDecode invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                result = info->qrcodedetector->detectAndDecode(*(srcinfo->matrix),
                                                               points_matrix,
                                                               code_matrix);
            } catch (...){
                Tcl_SetResult(interp, (char *) "detectAndDecode failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "detectAndDecode malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo->matrix = new cv::Mat(points_matrix);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr1 = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, dstinfo);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            code_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!code_info) {
                Tcl_SetResult(interp, (char *) "detectAndDecode malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            code_info->matrix = new cv::Mat(code_matrix);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr2 = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, code_info);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr,
                                     Tcl_NewStringObj( result.c_str(), result.length() ));

            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1 );
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2 );

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            delete info->qrcodedetector;
            ckfree(info);

            Tcl_MutexLock(&myMutex);
            if( hashEntryPtr )  Tcl_DeleteHashEntry(hashEntryPtr);
            Tcl_MutexUnlock(&myMutex);

            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int QRCodeDetector(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::QRCodeDetector *qrcodedetector;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    QRCodeDetectorInfo *info;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        qrcodedetector = new cv::QRCodeDetector();
    } catch (...){
        Tcl_SetResult(interp, (char *) "QRCodeDetector failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info = (QRCodeDetectorInfo *) ckalloc(sizeof(QRCodeDetectorInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "QRCodeDetector: malloc QRCodeDetectorInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->qrcodedetector = qrcodedetector;

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-qdetect%d", detect_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) QRCodeDetector_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int CascadeClassifier_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    CascadeClassifierInfo *info;

    static const char *FUNC_strs[] = {
        "detectMultiScale",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_DETECTMULTISCALE,
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
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "CascadeClassifier: invalid CascadeClassifier handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (CascadeClassifierInfo *) Tcl_GetHashValue( hashEntryPtr );

    switch( (enum FUNC_enum)choice ){
        case FUNC_DETECTMULTISCALE: {
            Tcl_HashEntry *hashEntryPtr;
            char *handle;
            double scaleFactor = 1.1;
            int minNeighbors = 3, minWidth = 0, minHeight = 0, maxWidth = 0, maxHeight = 0;
            std::vector< cv::Rect > rect;
            Tcl_Obj *pResultStr = NULL;
            MatrixInfo *matrix_info;

            if( objc != 3 && objc != 9 ){
                Tcl_WrongNumArgs(interp, 2, objv,
                        "matrix ?scaleFactor minNeighbors minWidth minHeight maxWidth maxHeight?");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "CascadeClassifier: invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            matrix_info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !matrix_info ) {
                Tcl_SetResult(interp, (char *) "CascadeClassifier invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (objc == 9) {
                if(Tcl_GetDoubleFromObj(interp, objv[3], &scaleFactor) != TCL_OK) {
                    return TCL_ERROR;
                }

                if(Tcl_GetIntFromObj(interp, objv[4], &minNeighbors) != TCL_OK) {
                    return TCL_ERROR;
                }

                if(Tcl_GetIntFromObj(interp, objv[5], &minWidth) != TCL_OK) {
                    return TCL_ERROR;
                }

                if(Tcl_GetIntFromObj(interp, objv[6], &minHeight) != TCL_OK) {
                    return TCL_ERROR;
                }

                if(Tcl_GetIntFromObj(interp, objv[7], &maxWidth) != TCL_OK) {
                    return TCL_ERROR;
                }

                if(Tcl_GetIntFromObj(interp, objv[8], &maxHeight) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                info->classifier->detectMultiScale(*(matrix_info->matrix),
                                            rect, scaleFactor, minNeighbors, 0,
                                            cv::Size(minWidth, minHeight),
                                            cv::Size(maxWidth, maxHeight));
            } catch (...){
                Tcl_SetResult(interp, (char *) "detectMultiScale failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < rect.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( rect[i].x ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( rect[i].y ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( rect[i].width ));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( rect[i].height ));

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

            delete info->classifier;
            ckfree(info);

            Tcl_MutexLock(&myMutex);
            if( hashEntryPtr )  Tcl_DeleteHashEntry(hashEntryPtr);
            Tcl_MutexUnlock(&myMutex);

            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int CascadeClassifier(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    char *filename = NULL;
    int len = 0;
    cv::CascadeClassifier *classifier;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    CascadeClassifierInfo *info;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if( !filename || len < 1 ){
        Tcl_SetResult(interp, (char *) "CascadeClassifier: invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        classifier = new cv::CascadeClassifier(filename);

        if (classifier->empty() == true) {
            Tcl_SetResult(interp, (char *) "CascadeClassifier load file failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "CascadeClassifier failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info = (CascadeClassifierInfo *) ckalloc(sizeof(CascadeClassifierInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "CascadeClassifier: malloc CascadeClassifierInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->classifier = classifier;

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-odetect%d", detect_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) CascadeClassifier_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
