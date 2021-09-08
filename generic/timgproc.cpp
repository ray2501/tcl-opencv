#include "tclopencv.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

int applyColorMap(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int colormap = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc !=3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix colormap");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "applyColorMap invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "applyColorMap invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &colormap) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::applyColorMap(*(info->matrix), image, colormap);
    } catch (...){
        Tcl_SetResult(interp, (char *) "applyColorMap failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "applyColorMap malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int cvtColor(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int code = 0, dstCn = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix code ?dstCn?");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "cvtColor invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "cvtColor invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &code) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetIntFromObj(interp, objv[3], &dstCn) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::cvtColor(*(info->matrix), image, code, dstCn);
    } catch (...){
        Tcl_SetResult(interp, (char *) "cvtColor failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "cvtColor malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int calcBackProject(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int count = 0, rangecount = 0;
    int *channels = NULL;
    float **ranges = NULL;
    double scale = 1;
    int uniform = 1;
    cv::Mat dst;
    Tcl_HashEntry *hashEntryPtr;
    char *handle, *histhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *histinfo, *dstinfo;

    if (objc != 5 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "matrix channels hist_matrix ranges_list ?scale uniform?");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcBackProject invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "calcBackProject invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "calcBackProject invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0) {
        Tcl_SetResult(interp, (char *) "calcBackProject channels is empty list", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int size;

        channels = (int *) ckalloc (sizeof(int) * count);
        if (!channels) {
            Tcl_SetResult(interp, (char *) "calcBackProject channels malloc memory failed", TCL_STATIC);
            return TCL_ERROR;
        }

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &size) != TCL_OK) {
                ckfree(channels);
                return TCL_ERROR;
            }

            if (size < 0) {
                ckfree(channels);
                Tcl_SetResult(interp, (char *) "calcBackProject channels out of range", TCL_STATIC);
                return TCL_ERROR;
            }

            channels[i] = size;
        }
    }

    histhandle = Tcl_GetStringFromObj(objv[3], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, histhandle );
    if( !hashEntryPtr ) {
        ckfree(channels);
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcBackProject invalid MATRIX handle ",
                                    histhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    histinfo = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !histinfo ) {
        ckfree(channels);
        Tcl_SetResult(interp, (char *) "calcBackProject invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        ckfree(channels);
        Tcl_SetResult(interp, (char *) "calcBackProject invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0 || count%2 != 0) {
        ckfree(channels);
        Tcl_SetResult(interp, (char *) "calcBackProject ranges_list invalid data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int range = count / 2;
        double min, max;
        rangecount = range;

        ranges = (float **) ckalloc (sizeof(float *) * count);
        if (!ranges) {
            ckfree(channels);
            Tcl_SetResult(interp, (char *) "calcBackProject ranges malloc memory failed", TCL_STATIC);
            return TCL_ERROR;
        }

        for (int i = 0, j = 0; i < range; i++, j = j + 2) {
            Tcl_ListObjIndex(interp, objv[4], j, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &min) != TCL_OK) {
                ckfree(channels);
                ckfree(ranges);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], j + 1, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &max) != TCL_OK) {
                ckfree(channels);
                ckfree(ranges);
                return TCL_ERROR;
            }

            ranges[i] = (float *) ckalloc (sizeof(float) * 2);
            if (!ranges[i]) {
                ckfree(channels);
                ckfree(ranges);
                Tcl_SetResult(interp, (char *) "calcBackProject ranges malloc memory failed", TCL_STATIC);
                return TCL_ERROR;
            }

            ranges[i][0] = (float) min;
            ranges[i][1] = (float) max;
        }
    }

    if (objc == 7) {
        if(Tcl_GetDoubleFromObj(interp, objv[5], &scale) != TCL_OK) {
            ckfree(channels);
            ckfree(ranges);
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[6], &uniform) != TCL_OK) {
            ckfree(channels);
            ckfree(ranges);
            return TCL_ERROR;
        }
    }

    try {
        cv::calcBackProject(info->matrix, 1, channels, *(histinfo->matrix),
                            dst, (const float**) ranges, scale, (bool) uniform);
    } catch (...){
        ckfree(channels);
        ckfree(ranges);
        Tcl_SetResult(interp, (char *) "calcBackProject failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        ckfree(ranges);

        Tcl_SetResult(interp, (char *) "calcBackProject malloc MatrixInfo failed", TCL_STATIC);
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

    ckfree(channels);

    for (int i = 0; i < rangecount; i++) {
        ckfree(ranges[i]);
    }
    ckfree(ranges);

    return TCL_OK;
}


int calcHist(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int dims = 0, count = 0, rangecount = 0;
    int *histSize = NULL, *channels = NULL;
    float **ranges = NULL;
    int uniform = 1, accumulate = 0;
    cv::Mat mask, dst;
    Tcl_HashEntry *hashEntryPtr;
    char *handle, *maskhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *maskinfo, *dstinfo;

    if (objc != 7 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix channels mask dims histSize_list ranges_list ?uniform accumulate?");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcHist invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "calcHist invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "calcHist invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0) {
        Tcl_SetResult(interp, (char *) "calcHist channels is empty list", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int size;

        channels = (int *) ckalloc (sizeof(int) * count);
        if (!channels) {
            Tcl_SetResult(interp, (char *) "calcHist channels malloc memory failed", TCL_STATIC);
            return TCL_ERROR;
        }

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &size) != TCL_OK) {
                ckfree(channels);
                return TCL_ERROR;
            }

            if (size < 0) {
                ckfree(channels);
                Tcl_SetResult(interp, (char *) "calcHist channels out of range", TCL_STATIC);
                return TCL_ERROR;
            }

            channels[i] = size;
        }
    }

    maskhandle = Tcl_GetStringFromObj(objv[3], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, maskhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            ckfree(channels);
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "calcHist invalid MATRIX handle ",
                                    maskhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    maskinfo = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !maskinfo ) {
        ckfree(channels);
        Tcl_SetResult(interp, (char *) "calcHist invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &dims) != TCL_OK) {
        ckfree(channels);
        return TCL_ERROR;
    }

    if (dims < 0 || dims > 32) {
        ckfree(channels);
        Tcl_SetResult(interp, (char *) "calcHist dims out of range", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        ckfree(channels);
        Tcl_SetResult(interp, (char *) "calcHist invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0) {
        ckfree(channels);
        Tcl_SetResult(interp, (char *) "calcHist histSize_list is empty list", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int size;

        histSize = (int *) ckalloc (sizeof(int) * count);
        if (!histSize) {
            ckfree(channels);
            Tcl_SetResult(interp, (char *) "calcHist histSize malloc memory failed", TCL_STATIC);
            return TCL_ERROR;
        }

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[5], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &size) != TCL_OK) {
                ckfree(channels);
                ckfree(histSize);
                return TCL_ERROR;
            }

            histSize[i] = size;
        }
    }

    if(Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
        ckfree(channels);
        ckfree(histSize);
        Tcl_SetResult(interp, (char *) "calcHist invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0 || count%2 != 0) {
        ckfree(channels);
        ckfree(histSize);

        Tcl_SetResult(interp, (char *) "calcHist ranges_list invalid data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int range = count / 2;
        double min, max;
        rangecount = range;

        ranges = (float **) ckalloc (sizeof(float *) * count);
        if (!ranges) {
            ckfree(channels);
            ckfree(histSize);
            Tcl_SetResult(interp, (char *) "calcHist ranges malloc memory failed", TCL_STATIC);
            return TCL_ERROR;
        }

        for (int i = 0, j = 0; i < range; i++, j = j + 2) {
            Tcl_ListObjIndex(interp, objv[6], j, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &min) != TCL_OK) {
                ckfree(channels);
                ckfree(histSize);
                ckfree(ranges);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[6], j + 1, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &max) != TCL_OK) {
                ckfree(channels);
                ckfree(histSize);
                ckfree(ranges);
                return TCL_ERROR;
            }

            ranges[i] = (float *) ckalloc (sizeof(float) * 2);
            if (!ranges[i]) {
                ckfree(channels);
                ckfree(histSize);
                ckfree(ranges);
                Tcl_SetResult(interp, (char *) "calcHist ranges malloc memory failed", TCL_STATIC);
                return TCL_ERROR;
            }

            ranges[i][0] = (float) min;
            ranges[i][1] = (float) max;
        }
    }

    if (objc == 9) {
        if(Tcl_GetBooleanFromObj(interp, objv[7], &uniform) != TCL_OK) {
            ckfree(channels);
            ckfree(histSize);
            ckfree(ranges);
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[8], &accumulate) != TCL_OK) {
            ckfree(channels);
            ckfree(histSize);
            ckfree(ranges);
            return TCL_ERROR;
        }
    }

    try {
        mask = *(maskinfo->matrix);
        cv::calcHist(info->matrix, 1, channels, mask, dst, dims, histSize,
                         (const float**) ranges, (bool) uniform, (bool) accumulate);
    } catch (...){
        ckfree(channels);
        ckfree(histSize);
        ckfree(ranges);
        Tcl_SetResult(interp, (char *) "calcHist failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        ckfree(channels);
        ckfree(histSize);
        ckfree(ranges);

        Tcl_SetResult(interp, (char *) "calcHist malloc MatrixInfo failed", TCL_STATIC);
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

    ckfree(channels);
    ckfree(histSize);

    for (int i = 0; i < rangecount; i++) {
        ckfree(ranges[i]);
    }
    ckfree(ranges);

    return TCL_OK;
}


int equalizeHist(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat dst;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *dstinfo;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "equalizeHist invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "equalizeHist invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::equalizeHist(*(info->matrix), dst);
    } catch (...){
        Tcl_SetResult(interp, (char *) "equalizeHist failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "equalizeHist malloc MatrixInfo failed", TCL_STATIC);
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
    return TCL_OK;
}


int floodFill(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int seed_x = 0, seed_y = 0;
    int new_B = 0, new_G = 0, new_R = 0, new_A = 0;
    int r_x = 0, r_y = 0, r_w = 0, r_h = 0;
    int lo_B = 0, lo_G= 0, lo_R = 0, lo_A = 0;
    int up_B = 0, up_G = 0, up_R = 0, up_A = 0;
    int flags = 4, count = 0;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    MatrixInfo *info;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
                "matrix seed_x seed_y color_list ?rect_list loDiff_color upDiff_color flags?");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "floodFill invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "floodFill invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_GetIntFromObj(interp, objv[2], &seed_x) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &seed_y) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "floodFill invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "floodFill invalid color data", TCL_STATIC);
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

    if (objc == 9) {
        if(Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
            Tcl_SetResult(interp, (char *) "floodFill invalid list data", TCL_STATIC);
            return TCL_ERROR;
        }

        if (count != 4) {
            Tcl_SetResult(interp, (char *) "floodFill invalid rect data", TCL_STATIC);
            return TCL_ERROR;
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[5], 0, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &r_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &r_y) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &r_w) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &r_h) != TCL_OK) {
                return TCL_ERROR;
            }
        }

        if(Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
            Tcl_SetResult(interp, (char *) "floodFill invalid list data", TCL_STATIC);
            return TCL_ERROR;
        }

        if (count != 4) {
            Tcl_SetResult(interp, (char *) "floodFill invalid color data", TCL_STATIC);
            return TCL_ERROR;
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[6], 0, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &lo_B) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[6], 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &lo_G) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[6], 2, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &lo_R) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[6], 3, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &lo_A) != TCL_OK) {
                return TCL_ERROR;
            }
        }

        if(Tcl_ListObjLength(interp, objv[7], &count) != TCL_OK) {
            Tcl_SetResult(interp, (char *) "floodFill invalid list data", TCL_STATIC);
            return TCL_ERROR;
        }

        if (count != 4) {
            Tcl_SetResult(interp, (char *) "floodFill invalid color data", TCL_STATIC);
            return TCL_ERROR;
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[7], 0, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &up_B) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &up_G) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 2, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &up_R) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 3, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &up_A) != TCL_OK) {
                return TCL_ERROR;
            }
        }

        if(Tcl_GetIntFromObj(interp, objv[8], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar newColor(new_B, new_G, new_R, new_A);
        cv::Rect rect(r_x, r_y, r_w, r_h);
        cv::Scalar loColor(lo_B, lo_G, lo_R, lo_A);
        cv::Scalar upColor(up_B, up_G, up_R, up_A);

        if (objc == 9) {
            cv::floodFill(image, cv::Point(seed_x, seed_y), newColor,
                      &rect, loColor, upColor, flags);
        } else {
            cv::floodFill(image, cv::Point(seed_x, seed_y), newColor);
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "floodFill failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int grabCut(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int x = 0, y = 0, width = 0, height = 0;
    int iterCount = 0, mode = cv::GC_INIT_WITH_RECT;
    cv::Mat image, maskimage, bgdModel, fgdModel;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix x y width height iterCount");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "grabCut invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "grabCut invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

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
    if(Tcl_GetIntFromObj(interp, objv[6], &iterCount) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::Rect rect(x, y, width, height);

        cv::grabCut(image, maskimage, rect, bgdModel, fgdModel, iterCount, mode);
    } catch (...){
        Tcl_SetResult(interp, (char *) "grabCut failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "grabCut malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(maskimage);

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


int matchTemplate(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat result_image;
    int method = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    char *rhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *templ_info;
    MatrixInfo *dstinfo;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix templ_matrix method");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "matchTemplate invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "matchTemplate invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    rhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, rhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "matchTemplate invalid MATRIX handle ",
                                    rhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    templ_info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !templ_info ) {
        Tcl_SetResult(interp, (char *) "matchTemplate invalid templ_info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        if (info->matrix->cols < templ_info->matrix->cols ||
            info->matrix->rows < templ_info->matrix->rows) {
            Tcl_SetResult(interp, (char *) "matchTemplate src image is too small", TCL_STATIC);
            return TCL_ERROR;
        }

        cv::matchTemplate(*(info->matrix), *(templ_info->matrix),
                          result_image, method);
    } catch (...){
        Tcl_SetResult(interp, (char *) "matchTemplate failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "matchTemplate malloc MatrixInfo failed", TCL_STATIC);
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

    return TCL_OK;
}


int getRotationMatrix2D(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int x = 0, y = 0;
    double angle = 0.0, scale = 1.0;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *dstinfo;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "x y angle scale");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &x) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &y) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &angle) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &scale) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        image = cv::getRotationMatrix2D(cv::Point2f(x, y), angle, scale);
    } catch (...){
        Tcl_SetResult(interp, (char *) "getRotationMatrix2D failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "getRotationMatrix2D malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int getRectSubPix(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat sub_image;
    int width = 0, height = 0, center_x = 0, center_y = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix width height center_x center_y");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "getRectSubPix invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "getRectSubPix invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (width <= 0 || height <= 0) {
        Tcl_SetResult(interp, (char *) "getRectSubPix invalid range", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &center_x) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &center_y) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::getRectSubPix(*(info->matrix), cv::Size(width, height),
                          cv::Point2f(center_x, center_y), sub_image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "getRectSubPix failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "getRectSubPix malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(sub_image);

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


int remap(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int interpolation = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *handle, *map1handle, *map2handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *mapinfo1, *mapinfo2;
    MatrixInfo *dstinfo;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix map1 map2 interpolation");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "remap invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "remap invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    map1handle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, map1handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "remap invalid MATRIX handle ",
                                    map1handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    mapinfo1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !mapinfo1 ) {
        Tcl_SetResult(interp, (char *) "remap invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    map2handle = Tcl_GetStringFromObj(objv[3], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, map2handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "remap invalid MATRIX handle ",
                                    map2handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    mapinfo2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !mapinfo2 ) {
        Tcl_SetResult(interp, (char *) "remap invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &interpolation) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::remap(*(info->matrix), image, *(mapinfo1->matrix), *(mapinfo2->matrix), interpolation);
    } catch (...){
        Tcl_SetResult(interp, (char *) "remap failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "remap malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int resize(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int width = 0, height = 0;
  	int flags = cv::INTER_LINEAR;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix width height ?flags?");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "resize invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "resize invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (width <= 0 || height <= 0) {
        Tcl_SetResult(interp, (char *) "resize invalid range", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetIntFromObj(interp, objv[4], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::resize(*(info->matrix), image, cv::Size(width, height), 0, 0, flags);
    } catch (...){
        Tcl_SetResult(interp, (char *) "resize failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "resize malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int threshold(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    double thresh = 0, maxval = 0;
    int type;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix thresh maxval type");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "threshold invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "threshold invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }


    if(Tcl_GetDoubleFromObj(interp, objv[2], &thresh) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &maxval) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::threshold(*(info->matrix), image, thresh, maxval, type);
    } catch (...){
        Tcl_SetResult(interp, (char *) "threshold failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "threshold malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int adaptiveThreshold(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    double maxValue = 0, C = 0;
    int adaptiveMethod = 0, thresholdType = 0, blockSize = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix maxValue adaptiveMethod thresholdType blockSize C");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "adaptiveThreshold invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "adaptiveThreshold invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &maxValue) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &adaptiveMethod) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &thresholdType) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &blockSize) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[6], &C) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::adaptiveThreshold(*(info->matrix), image, maxValue,
                              adaptiveMethod, thresholdType, blockSize, C);
    } catch (...){
        Tcl_SetResult(interp, (char *) "adaptiveThreshold failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "adaptiveThreshold malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int getAffineTransform(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int count = 0, npts = 0;
    cv::Mat mat_image;
    std::vector<cv::Point2f> src_points, dst_points;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *dstinfo;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_list dst_list");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "getAffineTransform invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "getAffineTransform invalid point data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        double number_from_list_x;
        double number_from_list_y;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            cv::Point2f point;
            point.x = (float) number_from_list_x;
            point.y = (float) number_from_list_y;
            src_points.push_back(point);
        }
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "getAffineTransform invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "getAffineTransform invalid point data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        double number_from_list_x;
        double number_from_list_y;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            cv::Point2f point;
            point.x = (float) number_from_list_x;
            point.y = (float) number_from_list_y;
            dst_points.push_back(point);
        }
    }

    try {
        mat_image = cv::getAffineTransform(src_points, dst_points);
    } catch (...){
        Tcl_SetResult(interp, (char *) "getAffineTransform failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "getAffineTransform malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(mat_image);

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


int warpAffine(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat rotated_image;
    int width = 0, height = 0;
    int flags = cv::INTER_LINEAR;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    char *rhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *rotate_info;
    MatrixInfo *dstinfo;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix transformation_matrix width height ?flags?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "warpAffine invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "warpAffine invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    rhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, rhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "warpAffine invalid MATRIX handle ",
                                    rhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    rotate_info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !rotate_info ) {
        Tcl_SetResult(interp, (char *) "warpAffine invalid rotate_info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (width <= 0 || height <= 0) {
        Tcl_SetResult(interp, (char *) "warpAffine invalid range", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[5], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::warpAffine(*(info->matrix), rotated_image, *(rotate_info->matrix),
                        cv::Size(width, height), flags);
    } catch (...){
        Tcl_SetResult(interp, (char *) "warpAffine failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "warpAffine malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(rotated_image);

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


int getPerspectiveTransform(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int count = 0, npts = 0;
    cv::Mat mat_image;
    std::vector<cv::Point2f> src_points, dst_points;
    int solveMethod = cv::DECOMP_LU;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *dstinfo;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_list dst_list ?solveMethod?");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "getPerspectiveTransform invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "getPerspectiveTransform invalid point data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        double number_from_list_x;
        double number_from_list_y;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            cv::Point2f point;
            point.x = (float) number_from_list_x;
            point.y = (float) number_from_list_y;
            src_points.push_back(point);
        }
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "getPerspectiveTransform invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "getPerspectiveTransform invalid point data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        double number_from_list_x;
        double number_from_list_y;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if(Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            cv::Point2f point;
            point.x = (float) number_from_list_x;
            point.y = (float) number_from_list_y;
            dst_points.push_back(point);
        }
    }

    if (objc == 4) {
        if(Tcl_GetIntFromObj(interp, objv[3], &solveMethod) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        mat_image = cv::getPerspectiveTransform(src_points, dst_points, solveMethod);
    } catch (...){
        Tcl_SetResult(interp, (char *) "getPerspectiveTransform failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "getPerspectiveTransform malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(mat_image);

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


int warpPerspective(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat mat_image;
    int width = 0, height = 0;
    int flags = cv::INTER_LINEAR;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    char *rhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *matrix_info;
    MatrixInfo *dstinfo;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix transformation_matrix width height ?flags?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "warpPerspective invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "warpPerspective invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    rhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, rhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "warpPerspective invalid MATRIX handle ",
                                    rhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    matrix_info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !matrix_info ) {
        Tcl_SetResult(interp, (char *) "warpPerspective invalid matrix_info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (width <= 0 || height <= 0) {
        Tcl_SetResult(interp, (char *) "warpPerspective invalid range", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[5], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::warpPerspective(*(info->matrix), mat_image, *(matrix_info->matrix),
                        cv::Size(width, height), flags);
    } catch (...){
        Tcl_SetResult(interp, (char *) "warpPerspective failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "warpPerspective malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(mat_image);

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


int filter2D(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat filter_image;
    int anchor_x = -1, anchor_y = -1, borderType = cv::BORDER_DEFAULT;
    double delta = 0.0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle, *khandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *kinfo;
    MatrixInfo *dstinfo;

    if (objc != 3 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix kernel_matrix ?anchor_x anchor_y delta borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "filter2D invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "filter2D invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    khandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, khandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "filter2D invalid MATRIX handle ",
                                    khandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    kinfo = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !kinfo ) {
        Tcl_SetResult(interp, (char *) "filter2D invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 7) {
        if(Tcl_GetIntFromObj(interp, objv[3], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[4], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[5], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::filter2D(*(info->matrix), filter_image, -1, *(kinfo->matrix),
                     cv::Point(anchor_x, anchor_y), delta, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "filter2D failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "filter2D malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(filter_image);

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


int getGaborKernel(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat result_mat;
    int ksize_width = 0, ksize_height = 0, type = CV_64F;
    double sigma = 0, theta = 0, lambd = 0, gamma = 0, psi = CV_PI * 0.5;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *dstinfo;

    if (objc != 7 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "ksize_width ksize_height sigma theta lambd gamma ?psi type?");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &sigma) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &theta) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[5], &lambd) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[6], &gamma) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if(Tcl_GetDoubleFromObj(interp, objv[7], &psi) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[8], &type) != TCL_OK) {
            return TCL_ERROR;
        }

        if (type != CV_32F && type != CV_64F) {
            Tcl_SetResult(interp, (char *) "getGaborKernel type is wrong", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    try {
        result_mat = cv::getGaborKernel(cv::Size(ksize_width, ksize_height),
                           sigma, theta, lambd, gamma, psi, type);
    } catch (...){
        Tcl_SetResult(interp, (char *) "getGaborKernel failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "getGaborKernel malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(result_mat);

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


int blur(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat blur_image;
    int ksize_width = 0, ksize_height = 0, anchor_x = -1, anchor_y = -1;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix ksize_width ksize_height ?anchor_x anchor_y borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "blur invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "blur invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if(Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::blur(*(info->matrix), blur_image,
                 cv::Size(ksize_width, ksize_height),
                 cv::Point(anchor_x, anchor_y), borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "blur failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "blur malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(blur_image);

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


int GaussianBlur(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat blur_image;
    int ksize_width = 0, ksize_height = 0;
    double sigmaX = 0, sigmaY = 0;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 5 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix ksize_width ksize_height sigmaX ?sigmaY borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "GaussianBlur invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "GaussianBlur invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &sigmaX) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if(Tcl_GetDoubleFromObj(interp, objv[5], &sigmaY) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::GaussianBlur(*(info->matrix), blur_image,
                 cv::Size(ksize_width, ksize_height),
                 sigmaX, sigmaY, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "GaussianBlur failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "GaussianBlur malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(blur_image);

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


int medianBlur(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat blur_image;
    int ksize = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix ksize");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "medianBlur invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "medianBlur invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &ksize) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::medianBlur(*(info->matrix), blur_image, ksize);
    } catch (...){
        Tcl_SetResult(interp, (char *) "medianBlur failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "medianBlur malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(blur_image);

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


int bilateralFilter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat blur_image;
    int d = 0;
    double sigmaColor = 0, sigmaSpace = 0;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix d sigmaColor sigmaSpace ?borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "bilateralFilter invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "bilateralFilter invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &d) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &sigmaColor) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &sigmaSpace) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[5], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::bilateralFilter(*(info->matrix), blur_image,
                            d, sigmaColor, sigmaSpace, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "bilateralFilter failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "bilateralFilter malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(blur_image);

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


int boxFilter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat filter_image;
    int ksize_width = 0, ksize_height = 0, anchor_x = -1, anchor_y = -1;
    int normalize = 1;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix ksize_width ksize_height ?anchor_x anchor_y normalize borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "boxFilter invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "boxFilter invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if(Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[6], &normalize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[7], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::boxFilter(*(info->matrix), filter_image, -1,
                 cv::Size(ksize_width, ksize_height),
                 cv::Point(anchor_x, anchor_y), (bool) normalize, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "boxFilter failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "boxFilter malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(filter_image);

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


int sqrBoxFilter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat filter_image;
    int ksize_width = 0, ksize_height = 0, anchor_x = -1, anchor_y = -1;
    int normalize = 1;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix ksize_width ksize_height ?anchor_x anchor_y normalize borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "sqrBoxFilter invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "sqrBoxFilter invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if(Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[6], &normalize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[7], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::sqrBoxFilter(*(info->matrix), filter_image, -1,
                 cv::Size(ksize_width, ksize_height),
                 cv::Point(anchor_x, anchor_y), (bool) normalize, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "sqrBoxFilter failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "sqrBoxFilter malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(filter_image);

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


int getStructuringElement(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat kernel;
    int shape = 0, ksize_width = 0, ksize_height = 0, anchor_x = -1, anchor_y = -1;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "shape ksize_width ksize_height ?anchor_x anchor_y?");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &shape) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        kernel = cv::getStructuringElement(shape, cv::Size(ksize_width, ksize_height),
                                  cv::Point(anchor_x, anchor_y));
    } catch (...){
        Tcl_SetResult(interp, (char *) "getStructuringElement failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "getStructuringElement malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(kernel);

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


int dilate(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int anchor_x = -1, anchor_y = -1, iterations = 1;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle, *khandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *kinfo;
    MatrixInfo *dstinfo;

    if (objc != 3 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix kernel_matrix ?anchor_x anchor_y iterations?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "dilate invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "dilate invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    khandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, khandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "dilate invalid MATRIX handle ",
                                    khandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    kinfo = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !kinfo ) {
        Tcl_SetResult(interp, (char *) "dilate invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[3], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[4], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[5], &iterations) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::dilate(*(info->matrix), image, *(kinfo->matrix),
                   cv::Point(anchor_x, anchor_y), iterations);
    } catch (...){
        Tcl_SetResult(interp, (char *) "dilate failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "dilate malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int erode(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int anchor_x = -1, anchor_y = -1, iterations = 1;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle, *khandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *kinfo;
    MatrixInfo *dstinfo;

    if (objc != 3 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix kernel_matrix ?anchor_x anchor_y iterations?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "erode invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "erode invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    khandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, khandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "erode invalid MATRIX handle ",
                                    khandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    kinfo = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !kinfo ) {
        Tcl_SetResult(interp, (char *) "erode invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[3], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[4], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[5], &iterations) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::erode(*(info->matrix), image, *(kinfo->matrix),
                   cv::Point(anchor_x, anchor_y), iterations);
    } catch (...){
        Tcl_SetResult(interp, (char *) "erode failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "erode malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int morphologyEx(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int op = 0, anchor_x = -1, anchor_y = -1, iterations = 1;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle, *khandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *kinfo;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix op kernel_matrix ?anchor_x anchor_y iterations?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "morphologyEx invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "morphologyEx invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &op) != TCL_OK) {
        return TCL_ERROR;
    }

    khandle = Tcl_GetStringFromObj(objv[3], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, khandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "morphologyEx invalid MATRIX handle ",
                                    khandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    kinfo = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !kinfo ) {
        Tcl_SetResult(interp, (char *) "morphologyEx invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 7) {
        if(Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[6], &iterations) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::morphologyEx(*(info->matrix), image, op, *(kinfo->matrix),
                   cv::Point(anchor_x, anchor_y), iterations);
    } catch (...){
        Tcl_SetResult(interp, (char *) "morphologyEx failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "morphologyEx malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int pyrUp(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int dstsize_width = 0, dstsize_height = 0, borderType = cv::BORDER_DEFAULT;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix ?dstsize_width dstsize_height borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "pyrUp invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "pyrUp invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetIntFromObj(interp, objv[2], &dstsize_width) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[3], &dstsize_height) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[4], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 5) {
            cv::Size dstsize(dstsize_width, dstsize_height);
            cv::pyrUp(*(info->matrix), image, dstsize, borderType);
        } else {
            cv::pyrUp(*(info->matrix), image);
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "pyrUp failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "pyrUp malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int pyrDown(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int dstsize_width = 0, dstsize_height = 0, borderType = cv::BORDER_DEFAULT;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix ?dstsize_width dstsize_height borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "pyrDown invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "pyrDown invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetIntFromObj(interp, objv[2], &dstsize_width) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[3], &dstsize_height) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[4], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 5) {
            cv::Size dstsize(dstsize_width, dstsize_height);
            cv::pyrDown(*(info->matrix), image, dstsize, borderType);
        } else {
            cv::pyrDown(*(info->matrix), image);
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "pyrDown failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "pyrDown malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int pyrMeanShiftFiltering(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    double sp = 0, sr = 0;
    int maxLevel = 1;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix sp sr ?maxLevel?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "pyrMeanShiftFiltering invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "pyrMeanShiftFiltering invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &sp) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &sr) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetIntFromObj(interp, objv[4], &maxLevel) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::pyrMeanShiftFiltering(*(info->matrix), image, sp, sr, maxLevel);
    } catch (...){
        Tcl_SetResult(interp, (char *) "pyrMeanShiftFiltering failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "pyrMeanShiftFiltering malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int Canny(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int apertureSize = 3, L2gradient = 0;
    double threshold1 = 0.0, threshold2 = 0.0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix threshold1 threshold2 ?apertureSize L2gradient?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "Canny invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "Canny invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &threshold1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &threshold2) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[4], &apertureSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[5], &L2gradient) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Canny(*(info->matrix), image, threshold1, threshold2,
                    apertureSize, (bool) L2gradient);
    } catch (...){
        Tcl_SetResult(interp, (char *) "Canny failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "Canny malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int Sobel(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int ddepth = CV_64F;
    int dx = 0, dy = 0, ksize= 3, borderType = cv::BORDER_DEFAULT;
    double scale = 1.0, delta = 0.0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix dx dy ?ksize scale delta borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "Sobel invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "Sobel invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &dx) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &dy) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if(Tcl_GetIntFromObj(interp, objv[4], &ksize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[5], &scale) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[6], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[7], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Sobel(*(info->matrix), image, ddepth, dx, dy,
                    ksize, scale, delta, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "Sobel failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "Sobel malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int Scharr(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int ddepth = CV_64F;
    int dx = 0, dy = 0, borderType = cv::BORDER_DEFAULT;
    double scale = 1.0, delta = 0.0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix dx dy ?scale delta borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "Scharr invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "Scharr invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &dx) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &dy) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if(Tcl_GetDoubleFromObj(interp, objv[4], &scale) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[5], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scharr(*(info->matrix), image, ddepth, dx, dy,
                   scale, delta, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "Scharr failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "Scharr malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int Laplacian(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int ddepth = CV_8U;
    int ksize = 1, borderType = cv::BORDER_DEFAULT;
    double scale = 1.0, delta = 0.0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 2 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?ksize scale delta borderType?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "Laplacian invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "Laplacian invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[2], &ksize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[3], &scale) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[4], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Laplacian(*(info->matrix), image, ddepth, ksize,
                      scale, delta, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "Laplacian failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "Laplacian malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int distanceTransform(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int dstType = CV_32F;
    int distanceType = 1, maskSize;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix distanceType maskSize");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "distanceTransform invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "distanceTransform invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &distanceType) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &maskSize) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::distanceTransform(*(info->matrix), image, distanceType, maskSize, dstType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "distanceTransform failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "distanceTransform malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int connectedComponents(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int ltype = CV_32S;
    int connectivity = 8;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?connectivity?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "connectedComponents invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "connectedComponents invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 3) {
        if(Tcl_GetIntFromObj(interp, objv[2], &connectivity) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::connectedComponents(*(info->matrix), image, connectivity, ltype);
    } catch (...){
        Tcl_SetResult(interp, (char *) "connectedComponents failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "connectedComponents malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(image);

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


int watershed(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle, *mhandle;
    MatrixInfo *sinfo, *minfo;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix markers");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "watershed invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    sinfo = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !sinfo ) {
        Tcl_SetResult(interp, (char *) "watershed invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    mhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "watershed invalid MATRIX handle ",
                                    mhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    minfo = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !minfo ) {
        Tcl_SetResult(interp, (char *) "watershed invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::watershed(*(sinfo->matrix), *(minfo->matrix));
    } catch (...){
        Tcl_SetResult(interp, (char *) "watershed failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int goodFeaturesToTrack(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int maxCorners = 0, blockSize = 3, useHarrisDetector = 0;
    double qualityLevel = 0, minDistance = 0, k = 0.04;
    cv::Mat image, maskimage, result_image;
    Tcl_HashEntry *hashEntryPtr;
    char *handle, *maskhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info, *mask_info, *dstinfo;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix maxCorners qualityLevel minDistance ?mask blockSize useHarrisDetector k?");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "goodFeaturesToTrack invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "goodFeaturesToTrack invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_GetIntFromObj(interp, objv[2], &maxCorners) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &qualityLevel) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &minDistance) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        maskhandle = Tcl_GetStringFromObj(objv[5], 0);
        hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, maskhandle );
        if( !hashEntryPtr ) {
            if( interp ) {
                Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                Tcl_AppendStringsToObj( resultObj, "goodFeaturesToTrack invalid MATRIX handle ",
                                        maskhandle, (char *)NULL );
            }

            return TCL_ERROR;
        }

        mask_info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
        if ( !mask_info ) {
            Tcl_SetResult(interp, (char *) "goodFeaturesToTrack invalid info data", TCL_STATIC);
            return TCL_ERROR;
        }

        maskimage = *(mask_info->matrix);

        if(Tcl_GetIntFromObj(interp, objv[6], &blockSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[7], &useHarrisDetector) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[8], &k) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::goodFeaturesToTrack(image, result_image, maxCorners, qualityLevel, minDistance,
                                maskimage, blockSize, (bool) useHarrisDetector, k);
    } catch (...){
        Tcl_SetResult(interp, (char *) "goodFeaturesToTrack failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "matchTemplate malloc MatrixInfo failed", TCL_STATIC);
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
    return TCL_OK;
}


int cornerHarris(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat dst;
    int blockSize = 0, ksize = 0, borderType = cv::BORDER_DEFAULT;
    double k = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix blockSize ksize k ?borderType?");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "cornerHarris invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "cornerHarris invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }


    if(Tcl_GetIntFromObj(interp, objv[2], &blockSize) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &ksize) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &k) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[5], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::cornerHarris(*(info->matrix), dst, blockSize, ksize, k, borderType);
    } catch (...){
        Tcl_SetResult(interp, (char *) "cornerHarris failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "cornerHarris malloc MatrixInfo failed", TCL_STATIC);
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

    return TCL_OK;
}


int cornerSubPix(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int winSize_width = 0, winSize_height = 0, zeroZone_widht = 0, zeroZone_height;
    Tcl_HashEntry *hashEntryPtr;
    char *handle, *corners_handle, *thandle;
    MatrixInfo *info, *info2;
    TermCriteriaInfo *term_info;

    if (objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix corners winSize_width winSize_height zeroZone_widht zeroZone_height termCriteria");
        return TCL_ERROR;
    }

    handle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "cornerSubPix invalid MATRIX handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "cornerSubPix invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    corners_handle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, corners_handle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "cornerSubPix invalid MATRIX handle ",
                                    corners_handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "cornerSubPix invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &winSize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &winSize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &zeroZone_widht) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[6], &zeroZone_height) != TCL_OK) {
        return TCL_ERROR;
    }

    thandle = Tcl_GetStringFromObj(objv[7], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, thandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "cornerSubPix invalid TermCriteria handle ",
                                    thandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    term_info = (TermCriteriaInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !term_info ) {
        Tcl_SetResult(interp, (char *) "cornerSubPix invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::cornerSubPix(*(info->matrix), *(info2->matrix),
                         cv::Size(winSize_width, winSize_height),
                         cv::Size(zeroZone_widht, zeroZone_height),
                         *(term_info->termCriteria));
    } catch (...){
        Tcl_SetResult(interp, (char *) "cornerSubPix failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int HoughCircles(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int method = 0, minRadius = 0, maxRadius = 0;
    double dp = 0, minDist = 0, param1 = 100, param2 = 100;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    Tcl_Obj *pResultStr = NULL;
    std::vector<cv::Vec3f> circles;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix method dp minDist ?param1 param2 minRadius maxRadius?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "HoughCircles invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "HoughCircles invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &dp) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &minDist) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if(Tcl_GetDoubleFromObj(interp, objv[5], &param1) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[6], &param2) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[7], &minRadius) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[8], &maxRadius) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::HoughCircles(*(info->matrix), circles, method, dp, minDist,
                       param1, param2, minRadius, maxRadius);
    } catch (...){
        Tcl_SetResult(interp, (char *) "HoughCircles failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < circles.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( (int) circles[i][0] ));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( (int) circles[i][1] ));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( (int) circles[i][2] ));

        Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int HoughLines(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int threshold = 0;
    double rho = 0.0, theta = 0.0, srn = 0.0, stn = 0.0, min_theta = 0.0, max_theta = CV_PI;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    Tcl_Obj *pResultStr = NULL;
    std::vector<cv::Vec2f> lines;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix rho theta threshold ?srn stn min_theta max_theta?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "HoughLines invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "HoughLines invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &rho) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &theta) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if(Tcl_GetDoubleFromObj(interp, objv[5], &srn) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[6], &stn) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[7], &min_theta) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[8], &max_theta) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::HoughLines(*(info->matrix), lines, rho, theta, threshold,
                       srn, stn, min_theta, max_theta);
    } catch (...){
        Tcl_SetResult(interp, (char *) "HoughLines failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < lines.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( lines[i][0] ));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj( lines[i][1] ));

        Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int HoughLinesP(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int threshold = 0;
    double rho = 0.0, theta = 0.0, minLineLength = 0.0, maxLineGap = 0.0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    Tcl_Obj *pResultStr = NULL;
    std::vector<cv::Vec4i> lines;

    if (objc != 5 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix rho theta threshold ?minLineLength maxLineGap?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "HoughLinesP invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "HoughLinesP invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &rho) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &theta) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if(Tcl_GetDoubleFromObj(interp, objv[5], &minLineLength) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[6], &maxLineGap) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::HoughLinesP(*(info->matrix), lines, rho, theta, threshold,
                        minLineLength, maxLineGap);
    } catch (...){
        Tcl_SetResult(interp, (char *) "HoughLinesP failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < lines.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( lines[i][0] ));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( lines[i][1] ));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( lines[i][2] ));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( lines[i][3] ));

        Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int findContours(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int mode = 0, method = 0, offset_point_x = 0, offset_point_y = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    Tcl_Obj *pResultStr = NULL;
    std::vector<std::vector<cv::Point> > contours;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix mode method ?offset_point_x offset_point_y?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "findContours invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "findContours invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &mode) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[4], &offset_point_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &offset_point_y) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 6) {
            cv::findContours(*(info->matrix), contours, mode, method,
                             cv::Point(offset_point_x, offset_point_y));
        } else {
            cv::findContours(*(info->matrix), contours, mode, method);
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "findContours failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < contours.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);

        for(size_t j = 0; j < contours.at(i).size(); j++) {
            Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( contours.at(i).at(j).x ));
            Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( contours.at(i).at(j).y ));
        }

        Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int drawContours(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int contourIdx = 0, thickness = 1, lineType = cv::LINE_8, maxLevel = INT_MAX;
    int B = 0, G = 0, R = 0, A = 0, count = 0;
    int offset_point_x = 0, offset_point_y = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    std::vector<std::vector<cv::Point> > contours;

    if (objc != 5 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
                "matrix contours_list contourIdx color_list ?thickness lineType maxLevel offset_point_x offset_point_y?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "drawContours invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "drawContours invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawContours invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count <= 0) {
        Tcl_SetResult(interp, (char *) "drawContours list data is empty", TCL_STATIC);
        return TCL_ERROR;
    } else {
        for (int contours_count = 0; contours_count < count; contours_count++) {
            Tcl_Obj *elemListPtr = NULL;
            int index_count = 0;
            std::vector<cv::Point> points;

            Tcl_ListObjIndex(interp, objv[2], contours_count, &elemListPtr);

            if(Tcl_ListObjLength(interp, elemListPtr, &index_count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "drawContours invalid contours data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (index_count%2 != 0) {
                Tcl_SetResult(interp, (char *) "drawContours invalid points data", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                int number_from_list_x;
                int number_from_list_y;
                int npts;

                npts = index_count / 2;
                for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
                    cv::Point point;
                    Tcl_ListObjIndex(interp, elemListPtr, i, &elemListSubPtr);
                    if(Tcl_GetIntFromObj(interp, elemListSubPtr, &number_from_list_x) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, i + 1, &elemListSubPtr);
                    if(Tcl_GetIntFromObj(interp, elemListSubPtr, &number_from_list_y) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    point.x = number_from_list_x;
                    point.y = number_from_list_y;
                    points.push_back (point);
                }
            }

            contours.push_back(points);
        }
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &contourIdx) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawContours invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "drawContours invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[4], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (objc == 10) {
        if(Tcl_GetIntFromObj(interp, objv[5], &thickness) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[6], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }
        if(Tcl_GetIntFromObj(interp, objv[7], &maxLevel) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[8], &offset_point_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[9], &offset_point_y) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);
        cv::drawContours(*(info->matrix), contours, contourIdx, color,
                        thickness, lineType, cv::noArray(), maxLevel,
                        cv::Point(offset_point_x, offset_point_y));
    } catch (...){
        Tcl_SetResult(interp, (char *) "drawContours failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int contourArea(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    std::vector<cv::Point> points;
    int oriented = 0, count = 0;
    double result = 0;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour oriented");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "contourArea invalid contour data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "contourArea invalid points data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if(Tcl_GetBooleanFromObj(interp, objv[2], &oriented) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        result = cv::contourArea(points, (bool) oriented);
    } catch (...){
        Tcl_SetResult(interp, (char *) "contourArea failed", TCL_STATIC);
        return TCL_ERROR;
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj( result ));
    return TCL_OK;
}


int boundingRect(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    std::vector<cv::Point> points;
    cv::Rect rect;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "boundingRect invalid contour data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "boundingRect invalid points data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    try {
        rect = cv::boundingRect(points);
    } catch (...){
        Tcl_SetResult(interp, (char *) "boundingRect failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( rect.x ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( rect.y ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( rect.width ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( rect.height ));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int minAreaRect(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    std::vector<cv::Point> points;
    cv::RotatedRect rect;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "minAreaRect invalid contour data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "minAreaRect invalid points data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    try {
        rect = cv::minAreaRect(points);
    } catch (...){
        Tcl_SetResult(interp, (char *) "minAreaRect failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( rect.center.x ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( rect.center.y ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( rect.size.width ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( rect.size.height ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( rect.angle ));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int boxPoints(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat points;
    double x, y, width, height, angle;
    cv::RotatedRect rect;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "box");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "boxPoints invalid box data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 5) {
        Tcl_SetResult(interp, (char *) "boxPoints invalid rotated rect data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[1], 0, &elemListPtr);
        if(Tcl_GetDoubleFromObj(interp, elemListPtr, &x) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 1, &elemListPtr);
        if(Tcl_GetDoubleFromObj(interp, elemListPtr, &y) != TCL_OK) {
            return TCL_ERROR;
        }
        Tcl_ListObjIndex(interp, objv[1], 2, &elemListPtr);
        if(Tcl_GetDoubleFromObj(interp, elemListPtr, &width) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 3, &elemListPtr);
        if(Tcl_GetDoubleFromObj(interp, elemListPtr, &height) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 4, &elemListPtr);
        if(Tcl_GetDoubleFromObj(interp, elemListPtr, &angle) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Point2f center((float) x, (float) y);
        cv::Size2f size((float) width, (float) height);
        cv::boxPoints(cv::RotatedRect(center, size, (float) angle),
                      points);
    } catch (...){
        Tcl_SetResult(interp, (char *) "boxPoints failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    cv::MatIterator_<float> _it = points.begin<float>();
    for(;_it != points.end<float>(); _it++){
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( (int) *_it ));
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int minEnclosingCircle(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    std::vector<cv::Point> points;
    cv::Point2f center;
    float radius;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "minEnclosingCircle invalid contour data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "minEnclosingCircle invalid points data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    try {
        cv::minEnclosingCircle(points, center, radius);
    } catch (...){
        Tcl_SetResult(interp, (char *) "minEnclosingCircle failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( (int ) center.x ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( (int ) center.y ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( (int ) radius ));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int convexHull(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    std::vector<cv::Point> points, hull;
    Tcl_Obj *pResultStr = NULL;
    int count = 0, clockwise = 0;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour ?clockwise?");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "convexHull invalid contour data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "convexHull invalid points data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if (objc == 3) {
        if(Tcl_GetBooleanFromObj(interp, objv[2], &clockwise) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::convexHull(points, hull, clockwise);
    } catch (...){
        Tcl_SetResult(interp, (char *) "convexHull failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for(size_t i = 0; i < hull.size(); i++) {
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( hull.at(i).x ));
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( hull.at(i).y ));
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int arrowedLine(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    int count = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int B = 0, G = 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;
    double tipLength = 0.1;

    if (objc != 8 && objc != 11) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_x1 point_y point2_x2 point2_y2 color_list thickness ?lineType shift tipLength?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "arrowedLine invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "arrowedLine invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "arrowedLine invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "arrowedLine invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[6], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_GetIntFromObj(interp, objv[7], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 11) {
        if(Tcl_GetIntFromObj(interp, objv[8], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[9], &shift) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetDoubleFromObj(interp, objv[10], &tipLength) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::arrowedLine(image, cv::Point(x1, y1), cv::Point(x2, y2),
                color, thickness, lineType, shift, tipLength);
    } catch (...){
        Tcl_SetResult(interp, (char *) "arrowedLine failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int circle(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    int count = 0;
    int x1 = 0, y1 = 0, radius = 0;
    int B = 0, G = 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;

    if (objc != 7 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix center_x center_y radius color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "circle invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "circle invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &radius) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "circle invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "circle invalid color data", TCL_STATIC);
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

    if(Tcl_GetIntFromObj(interp, objv[6], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if(Tcl_GetIntFromObj(interp, objv[7], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[8], &shift) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::circle(image, cv::Point(x1, y1), radius,
                color, thickness, lineType, shift);
    } catch (...){
        Tcl_SetResult(interp, (char *) "circle failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int clipLine(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int count = 0;
    int width, height;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    bool result;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "size_list point1_list point2_list");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "clipLine invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 2) {
        Tcl_SetResult(interp, (char *) "clipLine invalid size data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[1], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &width) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &height) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "clipLine invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 2) {
        Tcl_SetResult(interp, (char *) "clipLine invalid point data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &x1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &y1) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "clipLine invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 2) {
        Tcl_SetResult(interp, (char *) "clipLine invalid point data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &x2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &y2) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Point point1(x1, y1);
        cv::Point point2(x2, y2);
        result = cv::clipLine(cv::Size(width, height), point1, point2);
    } catch (...){
        Tcl_SetResult(interp, (char *) "clipLine failed", TCL_STATIC);
        return TCL_ERROR;
    }

    if (result) {
        Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 1 ));
    } else {
        Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 0 ));
    }

    return TCL_OK;
}


int drawMarker(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    int count = 0;
    int x1 = 0, y1 = 0;
    int B = 0, G = 0, R = 0, A = 0;
    int markerType = cv::MARKER_CROSS, markerSize = 20, thickness = 1, line_type = cv::LINE_8;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_x point_y color_list ?markerType markerSize thickness line_type?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "drawMarker invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "drawMarker invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "drawMarker invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "drawMarker invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[4], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (objc == 9) {
        if(Tcl_GetIntFromObj(interp, objv[5], &markerType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[6], &markerSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[7], &thickness) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[8], &line_type) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::drawMarker(image, cv::Point(x1, y1), color,
                    markerType, markerSize, thickness, line_type);
    } catch (...){
        Tcl_SetResult(interp, (char *) "drawMarker failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int ellipse(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    int count = 0;
    int x1 = 0, y1 = 0, width = 0, height;
    int B = 0, G = 0, R = 0, A = 0;
    double angle = 0.0, startAngle = 0.0, endAngle = 0.0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;

    if (objc != 11 && objc != 13) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix center_x center_y width hgieht angle startAngle endAngle color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "ellipse invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "ellipse invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[6], &angle) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[7], &startAngle) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[8], &endAngle) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[9], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "ellipse invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "ellipse invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[9], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[9], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[9], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[9], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_GetIntFromObj(interp, objv[10], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 13) {
        if(Tcl_GetIntFromObj(interp, objv[11], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[12], &shift) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::ellipse(image, cv::Point(x1, y1), cv::Size(width, height),
                angle, startAngle, endAngle, color,
                thickness, lineType, shift);
    } catch (...){
        Tcl_SetResult(interp, (char *) "ellipse failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int fillConvexPoly(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    int lineType = cv::LINE_8, shift = 0;
    cv::Point *pts;
    int npts = 0;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "matrix point_list color_list ?lineType shift?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "fillConvexPoly invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "fillConvexPoly invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "fillConvexPoly invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "fillConvexPoly invalid point data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;

        npts = count / 2;
        pts = (cv::Point *) ckalloc (sizeof(cv::Point) * npts);

        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                ckfree(pts);
                return TCL_ERROR;
            }

            pts[j].x = number_from_list_x;
            pts[j].y = number_from_list_y;
        }
    }

    if(Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        ckfree(pts);
        Tcl_SetResult(interp, (char *) "fillConvexPoly invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        ckfree(pts);
        Tcl_SetResult(interp, (char *) "fillConvexPoly invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (objc == 6) {
        if(Tcl_GetIntFromObj(interp, objv[4], &lineType) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[5], &shift) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::fillConvexPoly(image,  (const cv::Point *) pts, npts,
                           color, lineType, shift);
    } catch (...){
        ckfree(pts);
        Tcl_SetResult(interp, (char *) "fillConvexPoly failed", TCL_STATIC);
        return TCL_ERROR;
    }

    ckfree(pts);
    return TCL_OK;
}


int line(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    int count = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int B = 0, G = 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;

    if (objc != 8 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_x1 point_y1 point_x2 point_y2 color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "line invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "line invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "line invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "line invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[6], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_GetIntFromObj(interp, objv[7], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 10) {
        if(Tcl_GetIntFromObj(interp, objv[8], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[9], &shift) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2),
                 color, thickness, lineType, shift);
    } catch (...){
        Tcl_SetResult(interp, (char *) "line failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int polylines(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    int ncontours = 0, isClosed = 0;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;
    cv::Point *pts;
    int npts = 0;

    if (objc != 7 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_list ncontours isClosed color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "polylines invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "polylines invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "polylines invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "polylines invalid point data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;

        npts = count / 2;
        pts = (cv::Point *) ckalloc (sizeof(cv::Point) * npts);

        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                ckfree(pts);
                return TCL_ERROR;
            }

            pts[j].x = number_from_list_x;
            pts[j].y = number_from_list_y;
        }
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &ncontours) != TCL_OK) {
        ckfree(pts);
        return TCL_ERROR;
    }

    if(Tcl_GetBooleanFromObj(interp, objv[4], &isClosed) != TCL_OK) {
        ckfree(pts);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        ckfree(pts);
        Tcl_SetResult(interp, (char *) "polylines invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        ckfree(pts);
        Tcl_SetResult(interp, (char *) "polylines invalid color data", TCL_STATIC);
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

    if(Tcl_GetIntFromObj(interp, objv[6], &thickness) != TCL_OK) {
        ckfree(pts);
        return TCL_ERROR;
    }

    if (objc == 9) {
        if(Tcl_GetIntFromObj(interp, objv[7], &lineType) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[8], &shift) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::polylines(image,  (const cv::Point* const*) &pts, (const int*) &npts,
                      ncontours, (bool) isClosed, color,
                      thickness, lineType, shift);
    } catch (...){
        ckfree(pts);
        Tcl_SetResult(interp, (char *) "polylines failed", TCL_STATIC);
        return TCL_ERROR;
    }

    ckfree(pts);
    return TCL_OK;
}


int putText(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    char *text = NULL;
    int len = 0;
    int count = 0;
    int x1 = 0, y1 = 0, fontFace = 0, B = 0, G = 0, R = 0, A = 0;
    double fontScale = 1.0;
    int thickness = 1, lineType = cv::LINE_8, bottomLeftOrigin = 0;

    if (objc != 9 && objc != 11) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix text point_x point_y fontFace fontScale color_list thickness ?lineType bottomLeftOrigin?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "putText invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "putText invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    text = Tcl_GetStringFromObj(objv[2], &len);
    if( !text || len < 1 ){
        Tcl_SetResult(interp, (char *) "putText invalid text", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &fontFace) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[6], &fontScale) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[7], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "putText invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "putText invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[7], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[7], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_GetIntFromObj(interp, objv[8], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 11) {
        if(Tcl_GetIntFromObj(interp, objv[9], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetBooleanFromObj(interp, objv[10], &bottomLeftOrigin) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::putText(image, text, cv::Point(x1, y1), fontFace, fontScale,
                 color, thickness, lineType, (bool) bottomLeftOrigin);
    } catch (...){
        Tcl_SetResult(interp, (char *) "putText failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int rectangle(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    int count = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int B = 0, G= 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;

    if (objc != 8 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_x1 point_y1 point_x2 point_y2 color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "rectangle invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "rectangle invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    if(Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "rectangle invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "rectangle invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[6], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_GetIntFromObj(interp, objv[7], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 10) {
        if(Tcl_GetIntFromObj(interp, objv[8], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[9], &shift) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::rectangle(image, cv::Point(x1, y1), cv::Point(x2, y2),
                 color, thickness, lineType, shift);
    } catch (...){
        Tcl_SetResult(interp, (char *) "rectangle failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
