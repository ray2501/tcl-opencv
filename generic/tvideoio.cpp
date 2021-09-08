#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int VideoWriter_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    VideoWriterInfo *info;

    static const char *FUNC_strs[] = {
        "isOpened",
        "write",
        "get",
        "set",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_ISOPENED,
        FUNC_WRITE,
        FUNC_GET,
        FUNC_SET,
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
            Tcl_AppendStringsToObj( resultObj, "VideoWriter: invalid video capture handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (VideoWriterInfo *) Tcl_GetHashValue( hashEntryPtr );

    switch( (enum FUNC_enum)choice ){
        case FUNC_ISOPENED: {
            bool result;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                result = info->writer->isOpened();
            } catch (...){
                Tcl_SetResult(interp, (char *) "isOpened failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 1 ));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 0 ));
            }
            break;
        }
        case FUNC_WRITE: {
            Tcl_HashEntry *hashEntryPtr;
            char *handle;
            MatrixInfo *matrix_info;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            handle = Tcl_GetStringFromObj(objv[2], 0);
            hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
            if( !hashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "write: invalid MATRIX handle ",
                                            handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            matrix_info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
            if ( !matrix_info ) {
                Tcl_SetResult(interp, (char *) "write: invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            try {
                info->writer->write(*(matrix_info->matrix));
            } catch (...){
                Tcl_SetResult(interp, (char *) "write failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_GET: {
            int propId;
            double result;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "propId");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &propId) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                result = info->writer->get(propId);
            } catch (...){
                Tcl_SetResult(interp, (char *) "get failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj( result ));
            break;
        }
        case FUNC_SET: {
            int propId;
            double value;
            bool result;

            if( objc != 4 ){
                Tcl_WrongNumArgs(interp, 2, objv, "propId value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &propId) != TCL_OK) {
                return TCL_ERROR;
            }

            if(Tcl_GetDoubleFromObj(interp, objv[3], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                result = info->writer->set(propId, value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "set failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 1 ));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 0 ));
            }
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            info->writer->release();
            delete info->writer;
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


int VideoWriter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    char *fourcc = NULL;
    char *filename = NULL;
    int len = 0;
    double fps = 0, width = 0, height = 0;
    int fourccvalue = 0, isColor = 1;
    cv::VideoWriter *writer;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    VideoWriterInfo *info;

    if (objc !=6 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename fourcc fps width height ?isColor?");
        return TCL_ERROR;
    }


    filename = Tcl_GetStringFromObj(objv[1], &len);
    if( !filename || len < 1 ){
        Tcl_SetResult(interp, (char *) "VideoWriter: invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    fourcc = Tcl_GetStringFromObj(objv[2], &len);
    if( !fourcc || len != 4 ){
        Tcl_SetResult(interp, (char *) "VideoWriter: invalid fourcc", TCL_STATIC);
        return TCL_ERROR;
    }

    fourccvalue = cv::VideoWriter::fourcc(fourcc[0], fourcc[1], fourcc[2], fourcc[3]);

    if(Tcl_GetDoubleFromObj(interp, objv[3], &fps) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[5], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if(Tcl_GetBooleanFromObj(interp, objv[6], &isColor) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        writer = new cv::VideoWriter(filename, fourccvalue, fps,
                                     cv::Size (width,  height), (bool) isColor);
    } catch (...){
        Tcl_SetResult(interp, (char *) "VideoWriter failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info = (VideoWriterInfo *) ckalloc(sizeof(VideoWriterInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "VideoWriter: malloc VideoWriterInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->writer = writer;

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-videow%d", video_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) VideoWriter_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int VideoCapture_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    VideoCaptureInfo *info;

    static const char *FUNC_strs[] = {
        "isOpened",
        "read",
        "get",
        "set",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_ISOPENED,
        FUNC_READ,
        FUNC_GET,
        FUNC_SET,
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
            Tcl_AppendStringsToObj( resultObj, "invalid video capture handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (VideoCaptureInfo *) Tcl_GetHashValue( hashEntryPtr );

    switch( (enum FUNC_enum)choice ){
        case FUNC_ISOPENED: {
            bool result;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                result = info->capture->isOpened();
            } catch (...){
                Tcl_SetResult(interp, (char *) "isOpened failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 1 ));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 0 ));
            }
            break;
        }
        case FUNC_READ: {
            cv::Mat frame;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *matrix_info;
            bool isSuccess;

            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                isSuccess = info->capture->read(frame);
            } catch (...){
                Tcl_SetResult(interp, (char *) "read failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if (!isSuccess) {
                Tcl_SetResult(interp, (char *) "read failed", TCL_STATIC);
                return TCL_ERROR;
            }

            matrix_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!info) {
                Tcl_SetResult(interp, (char *) "read: malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }
            matrix_info->matrix = new cv::Mat(frame);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, matrix_info);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_GET: {
            int propId;
            double result;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "propId");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &propId) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                result = info->capture->get(propId);
            } catch (...){
                Tcl_SetResult(interp, (char *) "get failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj( result ));
            break;
        }
        case FUNC_SET: {
            int propId;
            double value;
            bool result;

            if( objc != 4 ){
                Tcl_WrongNumArgs(interp, 2, objv, "propId value");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &propId) != TCL_OK) {
                return TCL_ERROR;
            }

            if(Tcl_GetDoubleFromObj(interp, objv[3], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                result = info->capture->set(propId, value);
            } catch (...){
                Tcl_SetResult(interp, (char *) "set failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 1 ));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj( 0 ));
            }
            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            info->capture->release();
            delete info->capture;
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


int VideoCapture(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    char *opentype = NULL;
    char *filename = NULL;
    int index = 0;
    int len = 0;
    int type = 1;
    int flags = cv::CAP_ANY;
    cv::VideoCapture *capture;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    VideoCaptureInfo *info;
    Tcl_DString ds;

    if (objc !=3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "file/index filename/number ?flags?");
        return TCL_ERROR;
    }

    opentype = Tcl_GetStringFromObj(objv[1], &len);
    if( !opentype || len < 1 ){
        Tcl_SetResult(interp, (char *) "VideoCapture: invalid open type", TCL_STATIC);
        return TCL_ERROR;
    }

    if (strcmp(opentype, "file")==0) {
        filename = Tcl_GetStringFromObj(objv[2], &len);
        if( !filename || len < 1 ){
            Tcl_SetResult(interp, (char *) "VideoCapture: invalid file name", TCL_STATIC);
            return TCL_ERROR;
        }

        type = 1;
    } else if(strcmp(opentype, "index")==0) {
        if(Tcl_GetIntFromObj(interp, objv[2], &index) != TCL_OK) {
            Tcl_SetResult(interp, (char *) "VideoCapture: invalid index value", TCL_STATIC);
            return TCL_ERROR;
        }

        type = 2;
    }

    if (objc == 4) {
        if(Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (type == 1) {
            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            capture = new cv::VideoCapture(filename, flags);
            Tcl_DStringFree(&ds);
        } else {
            capture = new cv::VideoCapture(index, flags);
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "VideoCapture failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info = (VideoCaptureInfo *) ckalloc(sizeof(VideoCaptureInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "VideoCapture: malloc VideoCaptureInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->capture = capture;

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-videoc%d", video_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) VideoCapture_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
