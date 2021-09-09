#include "tclopencv.h"

/*
 * OpenCV StereoBM uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::StereoBM > stereoBM;

#ifdef __cplusplus
extern "C" {
#endif

int findHomography(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double ransacReprojThreshold = 3;
    int method = 0;
    cv::Mat result;
    Tcl_HashEntry *hashEntryPtr;
    char *phandle, *nhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *dstinfo;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 3 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "srcPoints dstPoints ?method ransacReprojThreshold?");
        return TCL_ERROR;
    }

    phandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, phandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "findHomography invalid MATRIX handle ",
                                    phandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "findHomography invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    nhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, nhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "findHomography invalid MATRIX handle ",
                                    nhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "findHomography invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[4], &ransacReprojThreshold) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        result = cv::findHomography(*(info1->matrix), *(info2->matrix),
                                    method, ransacReprojThreshold);
    } catch (...){
        Tcl_SetResult(interp, (char *) "findHomography failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "findHomography malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(result);

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


int StereoBM_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "compute",
        "getPreFilterCap",
        "getPreFilterSize",
        "getPreFilterType",
        "getSmallerBlockSize",
        "getTextureThreshold",
        "getUniquenessRatio",
        "setPreFilterCap",
        "setPreFilterSize",
        "setPreFilterType",
        "setSmallerBlockSize",
        "setTextureThreshold",
        "setUniquenessRatio",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_COMPUTE,
        FUNC_getPreFilterCap,
        FUNC_getPreFilterSize,
        FUNC_getPreFilterType,
        FUNC_getTextureThreshold,
        FUNC_getUniquenessRatio,
        FUNC_setPreFilterCap,
        FUNC_setPreFilterSize,
        FUNC_setPreFilterType,
        FUNC_setTextureThreshold,
        FUNC_setUniquenessRatio,
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
        case FUNC_COMPUTE: {
            cv::Mat dst;
            Tcl_HashEntry *hashEntryPtr;
            char *ahandle, *bhandle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *info1, *info2;
            MatrixInfo *mat_info;

            if( objc != 4 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix1 matrix2");
                return TCL_ERROR;
            }

            ahandle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "compute: invalid MATRIX handle ",
                                            ahandle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info1 ) {
                Tcl_SetResult(interp, (char *) "compute: invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            bhandle = Tcl_GetStringFromObj(objv[3], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "compute: invalid MATRIX handle ",
                                            bhandle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !info2 ) {
                Tcl_SetResult(interp, (char *) "compute: invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                stereoBM->compute(*(info1->matrix), *(info2->matrix), dst);
            } catch (...){
                Tcl_SetResult(interp, (char *) "compute failed", TCL_STATIC);
                return TCL_ERROR;
            }

            mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!mat_info) {
                Tcl_SetResult(interp, (char *) "compute: malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            mat_info->matrix = new cv::Mat(dst);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, mat_info);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getPreFilterCap: {
            int value;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = stereoBM->getPreFilterCap();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getPreFilterCap failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );
            break;
        }
        case FUNC_getPreFilterSize: {
            int value;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = stereoBM->getPreFilterSize();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getPreFilterSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );
            break;
        }
        case FUNC_getPreFilterType: {
            int value;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = stereoBM->getPreFilterType();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getPreFilterType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );
            break;
        }
        case FUNC_getTextureThreshold: {
            int value;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = stereoBM->getTextureThreshold();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getTextureThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );
            break;
        }
        case FUNC_getUniquenessRatio: {
            int value;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = stereoBM->getUniquenessRatio();
            } catch (...){
                Tcl_SetResult(interp, (char *) "getUniquenessRatio failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj (value) );
            break;
        }
        case FUNC_setPreFilterCap: {
            int value;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                stereoBM->setPreFilterCap(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setPreFilterCap failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setPreFilterSize: {
            int value;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                stereoBM->setPreFilterSize(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setPreFilterSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setPreFilterType: {
            int value;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                stereoBM->setPreFilterType(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setPreFilterType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setTextureThreshold: {
            int value;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                stereoBM->setTextureThreshold(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setTextureThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setUniquenessRatio: {
            int value;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                stereoBM->setUniquenessRatio(value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "setUniquenessRatio failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            stereoBM.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int StereoBM(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int numDisparities = 0, blockSize = 21;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "?numDisparities blockSize?");
        return TCL_ERROR;
    }

    if (objc == 3) {
        if(Tcl_GetIntFromObj(interp, objv[1], &numDisparities) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[2], &blockSize) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        stereoBM = cv::StereoBM::create( numDisparities, blockSize);

        if (stereoBM == nullptr) {
            Tcl_SetResult(interp, (char *) "stereoBM create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "StereoBM failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-stereoBM", -1 );

    Tcl_CreateObjCommand(interp, "cv-stereoBM", (Tcl_ObjCmdProc *) StereoBM_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
