#include "tclopencv.h"

/*
 * OpenCV Stitcher uses smart pointer to handle its memory.
 * Here I create a static object to use it.
 */
static cv::Ptr< cv::Stitcher > stitcher;

#ifdef __cplusplus
extern "C" {
#endif

int Stitcher_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    char *handle;

    static const char *FUNC_strs[] = {
        "stitch",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_STITCH,
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
        case FUNC_STITCH: {
            int count = 0;
            std::vector<cv::Mat> imgs;
            cv::Mat result_image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *dstinfo;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "image_list");
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "stitch invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0) {
                Tcl_SetResult(interp, (char *) "stitch empty list", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListPtr = NULL;
                cv::Mat image;
                Tcl_HashEntry *hashEntryPtr;
                char *handle;
                MatrixInfo *info;

                for (int i = 0; i <  count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                    handle = Tcl_GetStringFromObj(elemListPtr, 0);
                    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, handle );
                    if( !hashEntryPtr ) {
                        if( interp ) {
                            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                            Tcl_AppendStringsToObj( resultObj, "stitch invalid MATRIX handle ",
                                                    handle, (char *)NULL );
                        }

                        return TCL_ERROR;
                    }

                    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
                    if ( !info ) {
                        Tcl_SetResult(interp, (char *) "stitch invalid info data", TCL_STATIC);
                        return TCL_ERROR;
                    }

                    image = *(info->matrix);
                    imgs.push_back(image);
                }
            }

            try {
                cv::Stitcher::Status status = stitcher->stitch(imgs, result_image);

                if (status != cv::Stitcher::OK)
                {
                    Tcl_SetResult(interp, (char *) "stitch can't stitch images", TCL_STATIC);
                    return TCL_ERROR;
                }
            } catch (...){
                Tcl_SetResult(interp, (char *) "stitch failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!dstinfo) {
                Tcl_SetResult(interp, (char *) "stitch malloc MatrixInfo failed", TCL_STATIC);
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

            stitcher.reset();
            Tcl_DeleteCommand(interp, handle);

            break;
        }
    }

    return TCL_OK;
}


int Stitcher(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int mode = 0;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "mode");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &mode) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        stitcher = cv::Stitcher::create( (cv::Stitcher::Mode) mode);

        if (stitcher == nullptr) {
            Tcl_SetResult(interp, (char *) "Stitcher create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "Stitcher failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj( "cv-stitcher", -1 );

    Tcl_CreateObjCommand(interp, "cv-stitcher", (Tcl_ObjCmdProc *) Stitcher_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}

#ifdef __cplusplus
}
#endif
