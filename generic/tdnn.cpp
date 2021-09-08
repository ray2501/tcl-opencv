#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int dnn_blobFromImage(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat dstimage;
    double scalefactor = 1.0;
    int width = 0, height = 0, swapRB = 0, crop = 0;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "matrix scalefactor width height mean_color_list swapRB crop");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "blobFromImage invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "blobFromImage invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &scalefactor) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "blobFromImage invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "blobFromImage invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[5], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_GetBooleanFromObj(interp, objv[6], &swapRB) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetBooleanFromObj(interp, objv[7], &crop) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::Scalar color(B, G, R, A);
        cv::Size size(width, height);

        dstimage = cv::dnn::blobFromImage(*(info->matrix), scalefactor, size, color, swapRB, crop);
    } catch (...){
        Tcl_SetResult(interp, (char *) "blobFromImage failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "blobFromImage malloc MatrixInfo failed", TCL_STATIC);
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


int READNET_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    DnnNetInfo *info;

    static const char *FUNC_strs[] = {
        "setPreferableBackend",
        "setPreferableTarget",
        "setInput",
        "forward",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_setPreferableBackend,
        FUNC_setPreferableTarget,
        FUNC_setInput,
        FUNC_forward,
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
            Tcl_AppendStringsToObj( resultObj, "invalid Net handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (DnnNetInfo *) Tcl_GetHashValue( hashEntryPtr );

    switch( (enum FUNC_enum)choice ){
        case FUNC_setPreferableBackend: {
            int backendId;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "backendId");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &backendId) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                info->net->setPreferableBackend(backendId);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setPreferableBackend failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setPreferableTarget: {
            int targetId;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "targetId");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &targetId) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                info->net->setPreferableTarget(targetId);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setPreferableTarget failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setInput: {
            cv::Mat blob;
            char *name = NULL;
            int len;
            double scalefactor = 1.0;
            int count = 0, B = 0, G = 0, R = 0, A = 0;
            Tcl_HashEntry *hashEntryPtr;
            char *handle;
            MatrixInfo *matrix_info;

            if( objc != 3 && objc != 6 ){
                Tcl_WrongNumArgs(interp, 2, objv, "blob ?name scalefactor mean_color_list?");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "setInput invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            matrix_info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !matrix_info ) {
                Tcl_SetResult(interp, (char *) "setInput invalid matrix info data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (objc == 6) {
                /* name could be an empty string */
                name = Tcl_GetStringFromObj(objv[3], &len);
                if( !name ){
                    Tcl_SetResult(interp, (char *) "setInput invalid name", TCL_STATIC);
                    return TCL_ERROR;
                }

                if(Tcl_GetDoubleFromObj(interp, objv[4], &scalefactor) != TCL_OK) {
                    return TCL_ERROR;
                }

                if(Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
                    Tcl_SetResult(interp, (char *) "setInput invalid list data", TCL_STATIC);
                    return TCL_ERROR;
                }

                if (count != 4) {
                    Tcl_SetResult(interp, (char *) "setInput invalid color data", TCL_STATIC);
                    return TCL_ERROR;
                } else {
                    Tcl_Obj *elemListPtr = NULL;

                    Tcl_ListObjIndex(interp, objv[5], 0, &elemListPtr);
                    if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
                    if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
                    if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
                    if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
                        return TCL_ERROR;
                    }
                }
            }

            try {
                blob = *(matrix_info->matrix);
                if (objc == 3) {
                    info->net->setInput(blob);
                } else {
                    cv::Scalar color(B, G, R, A);
                    info->net->setInput(blob, name, scalefactor, color);
                }
            } catch (...){
                Tcl_SetResult(interp, (char *) "setInput failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_forward: {
            cv::Mat result_mat;
            char *name = NULL;
            int len;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if( objc != 2 && objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "?name?");
                return TCL_ERROR;
            }

            if (objc == 3 ) {
                name = Tcl_GetStringFromObj(objv[2], &len);
                if( !name || len < 1 ){
                    Tcl_SetResult(interp, (char *) "forward invalid name", TCL_STATIC);
                    return TCL_ERROR;
                }
            }

            try {
                if (objc == 2) {
                    result_mat = info->net->forward();
                } else {
                    result_mat = info->net->forward(name);
                }
            } catch (...){
                Tcl_SetResult(interp, (char *) "forward failed", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!img_info) {
                Tcl_SetResult(interp, (char *) "forward malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info->matrix = new cv::Mat(result_mat);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, img_info);
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

            delete info->net;
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


int dnn_readNet(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    char *model = NULL, *config = NULL, *framework = NULL;
    int len = 0;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    cv::dnn::Net *net;
    DnnNetInfo *info;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "model ?config framework?");
        return TCL_ERROR;
    }

    model = Tcl_GetStringFromObj(objv[1], &len);
    if( !model || len < 1 ){
        Tcl_SetResult(interp, (char *) "readNet invalid model name", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        config = Tcl_GetStringFromObj(objv[2], &len);
        if( !config || len < 1 ){
            Tcl_SetResult(interp, (char *) "readNet invalid config name", TCL_STATIC);
            return TCL_ERROR;
        }

        framework = Tcl_GetStringFromObj(objv[3], &len);
        if( !framework || len < 1 ){
            Tcl_SetResult(interp, (char *) "readNet invalid framework name", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    try {
        net = new cv::dnn::Net();
        if (objc == 2) {
            *net = cv::dnn::readNet(model);
        } else {
            *net = cv::dnn::readNet(model, config, framework);
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "readNet failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info = (DnnNetInfo *) ckalloc(sizeof(DnnNetInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "readNet malloc DnnNetInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->net = net;

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-odetect%d", detect_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) READNET_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


#ifdef __cplusplus
}
#endif
