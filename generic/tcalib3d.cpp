#include "tclopencv.h"

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
#ifdef __cplusplus
}
#endif
