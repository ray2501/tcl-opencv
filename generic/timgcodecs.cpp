#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int imread(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    char *filename = NULL;
    int len = 0;
  	int flags = cv::IMREAD_COLOR;
    cv::Mat image;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    Tcl_DString ds;

    if (objc !=2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename ?flags?");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if( !filename || len < 1 ){
        Tcl_SetResult(interp, (char *) "imread: invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 3) {
        if(Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
        image = cv::imread(filename, flags);
        Tcl_DStringFree(&ds);
    } catch (...){
        Tcl_SetResult(interp, (char *) "imread failed", TCL_STATIC);
        return TCL_ERROR;
    }

    if ( image.empty() ||  !image.data ) {
        Tcl_SetResult(interp, (char *) "imread: no image data", TCL_STATIC);
        return TCL_ERROR;
    }

    info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "imread malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int imwrite(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    char *filename = NULL;
    int len = 0;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    MatrixInfo *info;
    bool result = false;
    Tcl_DString ds;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename matrix");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if( !filename || len < 1 ){
        Tcl_SetResult(interp, (char *) "imwrite invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "imwrite invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "imwrite invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
        result = cv::imwrite(filename, *(info->matrix));
        Tcl_DStringFree(&ds);
    } catch (...){
        Tcl_SetResult(interp, (char *) "imwrite failed", TCL_STATIC);
        return TCL_ERROR;
    }

    if (!result) {
        Tcl_SetResult(interp, (char *) "imwrite ERROR: Can't save file.", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
