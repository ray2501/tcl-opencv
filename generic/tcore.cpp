#include "tclopencv.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

int MATRIX_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    MatrixInfo *info;

    static const char *FUNC_strs[] = {
        "dims",
        "size",
        "rows",
        "cols",
        "channels",
        "depth",
        "type",
        "empty",
        "at",
        "inv",
        "dot",
        "cross",
        "*",
        "add",
        "subtract",
        "multiply",
        "divide",
        "transpose",
        "diag",
        "crop",
        "rect",
        "copyTo",
        "convertTo",
        "colRange",
        "rowRange",
        "reshape",
        "setData",
        "setTo",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_DIMS,
        FUNC_SIZE,
        FUNC_ROWS,
        FUNC_COLS,
        FUNC_CHANNELS,
        FUNC_DEPTH,
        FUNC_TYPE,
        FUNC_EMPTY,
        FUNC_AT,
        FUNC_INV,
        FUNC_DOT,
        FUNC_CROSS,
        FUNC_MULTIPLICATION,
        FUNC_ADD,
        FUNC_SUBTRACT,
        FUNC_MULTIPLY,
        FUNC_DIVIDE,
        FUNC_TRANSPOSE,
        FUNC_DIAG,
        FUNC_CROP,
        FUNC_RECT,
        FUNC_COPYTO,
        FUNC_CONVERTTO,
        FUNC_COLRANGE,
        FUNC_ROWRANGE,
        FUNC_RESHAPE,
        FUNC_SETDATA,
        FUNC_SETTO,
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
            Tcl_AppendStringsToObj( resultObj, "invalid matrix handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );

    switch( (enum FUNC_enum)choice ){
        case FUNC_DIMS: {
            int dims = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            dims = info->matrix->dims;
            Tcl_SetObjResult(interp, Tcl_NewIntObj(dims));
            break;
        }
        case FUNC_SIZE: {
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (int i = 0; i < info->matrix->dims; i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj( info->matrix->size[i] ));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_ROWS: {
            int rows = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            rows = info->matrix->rows;
            Tcl_SetObjResult(interp, Tcl_NewIntObj(rows));
            break;
        }
        case FUNC_COLS: {
            int cols = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            cols = info->matrix->cols;
            Tcl_SetObjResult(interp, Tcl_NewIntObj(cols));
            break;
        }
        case FUNC_CHANNELS: {
            int channels = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            channels = info->matrix->channels();
            Tcl_SetObjResult(interp, Tcl_NewIntObj(channels));
            break;
        }
        case FUNC_DEPTH: {
            int depth = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            depth = info->matrix->depth();
            Tcl_SetObjResult(interp, Tcl_NewIntObj(depth));
            break;
        }
        case FUNC_TYPE: {
            int type = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            type = info->matrix->type();
            Tcl_SetObjResult(interp, Tcl_NewIntObj(type));
            break;
        }
        case FUNC_EMPTY: {
            bool result = false;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            result = info->matrix->empty();
            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
            }
            break;
        }
        case FUNC_AT: {
            int count = 0;
            int channel = 0;
            int *index = NULL;;
            double value = 0.0;
            uchar *return_uchar_val = NULL;
            schar *return_schar_val = NULL;
            ushort *return_ushort_val = NULL;
            short *return_short_val = NULL;
            int *return_int_val = NULL;
            float *return_float_val = NULL;
            double *return_double_val = NULL;
            long long_val = 0;
            double double_val = 0;

            if (objc != 4 && objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "index_list channel ?value?");
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "at invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count != info->matrix->dims) {
                Tcl_SetResult(interp, (char *) "at wrong list number", TCL_STATIC);
                return TCL_ERROR;
            }

            index = (int *) ckalloc(sizeof(int) * info->matrix->dims);
            if (!index) {
                Tcl_SetResult(interp, (char *) "at malloc index failed", TCL_STATIC);
                return TCL_ERROR;
            }

            for (int i = 0; i < count; i++) {
                Tcl_Obj *elemListPtr = NULL;
                int index_number;

                Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                if(Tcl_GetIntFromObj(interp, elemListPtr, &index_number) != TCL_OK) {
                    ckfree(index);
                    return TCL_ERROR;
                }

                index[i] = index_number;
            }

            for (int i = 0; i < count; i++) {
                if (index[i] >= info->matrix->size[i]) {
                    ckfree(index);
                    Tcl_SetResult(interp, (char *) "at index out of range", TCL_STATIC);
                    return TCL_ERROR;
                }
            }

            if(Tcl_GetIntFromObj(interp, objv[3], &channel) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }

            if (channel >= info->matrix->channels()) {
                ckfree(index);
                Tcl_SetResult(interp, (char *) "at wrong channels range", TCL_STATIC);
                return TCL_ERROR;
            }

            switch(info->matrix->type()) {
                case CV_8UC1: return_uchar_val = &info->matrix->at<uchar>(index); break;
                case CV_8UC2: return_uchar_val = &info->matrix->at<cv::Vec2b>(index)[channel]; break;
                case CV_8UC3: return_uchar_val = &info->matrix->at<cv::Vec3b>(index)[channel]; break;
                case CV_8UC4: return_uchar_val = &info->matrix->at<cv::Vec4b>(index)[channel]; break;

                case CV_8SC1: return_schar_val = &info->matrix->at<schar>(index); break;
                case CV_8SC2: return_schar_val = &info->matrix->at<cv::Vec<schar, 2>>(index)[channel]; break;
                case CV_8SC3: return_schar_val = &info->matrix->at<cv::Vec<schar, 3>>(index)[channel]; break;
                case CV_8SC4: return_schar_val = &info->matrix->at<cv::Vec<schar, 4>>(index)[channel]; break;

                case CV_16UC1: return_ushort_val = &info->matrix->at<ushort>(index); break;
                case CV_16UC2: return_ushort_val = &info->matrix->at<cv::Vec2w>(index)[channel]; break;
                case CV_16UC3: return_ushort_val = &info->matrix->at<cv::Vec3w>(index)[channel]; break;
                case CV_16UC4: return_ushort_val = &info->matrix->at<cv::Vec4w>(index)[channel]; break;

                case CV_16SC1: return_short_val = &info->matrix->at<short>(index); break;
                case CV_16SC2: return_short_val = &info->matrix->at<cv::Vec2s>(index)[channel]; break;
                case CV_16SC3: return_short_val = &info->matrix->at<cv::Vec3s>(index)[channel]; break;
                case CV_16SC4: return_short_val = &info->matrix->at<cv::Vec4s>(index)[channel]; break;

                case CV_32SC1: return_int_val = &info->matrix->at<int>(index); break;
                case CV_32SC2: return_int_val = &info->matrix->at<cv::Vec2i>(index)[channel]; break;
                case CV_32SC3: return_int_val = &info->matrix->at<cv::Vec3i>(index)[channel]; break;
                case CV_32SC4: return_int_val = &info->matrix->at<cv::Vec4i>(index)[channel]; break;

                case CV_32FC1: return_float_val = &info->matrix->at<float>(index); break;
                case CV_32FC2: return_float_val = &info->matrix->at<cv::Vec2f>(index)[channel]; break;
                case CV_32FC3: return_float_val = &info->matrix->at<cv::Vec3f>(index)[channel]; break;
                case CV_32FC4: return_float_val = &info->matrix->at<cv::Vec4f>(index)[channel]; break;

                case CV_64FC1: return_double_val = &info->matrix->at<double>(index); break;
                case CV_64FC2: return_double_val = &info->matrix->at<cv::Vec2d>(index)[channel]; break;
                case CV_64FC3: return_double_val = &info->matrix->at<cv::Vec3d>(index)[channel]; break;
                case CV_64FC4: return_double_val = &info->matrix->at<cv::Vec4d>(index)[channel]; break;

                default: {
                    ckfree(index);
                    Tcl_SetResult(interp, (char *) "at invalid type", TCL_STATIC);
                    return TCL_ERROR;
                }
            }

            if (objc == 5) {
                if(Tcl_GetDoubleFromObj(interp, objv[4], &value) != TCL_OK) {
                    ckfree(index);
                    return TCL_ERROR;
                }

                switch(info->matrix->depth()){
                    case CV_8U: *return_uchar_val = cv::saturate_cast<uchar>(value); break;
                    case CV_8S: *return_schar_val = cv::saturate_cast<schar>(value); break;
                    case CV_16U: *return_ushort_val = cv::saturate_cast<ushort>(value); break;
                    case CV_16S: *return_short_val = cv::saturate_cast<short>(value); break;
                    case CV_32S: *return_int_val = cv::saturate_cast<int>(value); break;
                    case CV_32F: *return_float_val = cv::saturate_cast<float>(value); break;
                    case CV_64F: *return_double_val = cv::saturate_cast<double>(value); break;

                    default: {
                        ckfree(index);
                        Tcl_SetResult(interp, (char *) "at set: wrong matrix type", TCL_STATIC);
                        return TCL_ERROR;
                    }
                }
            } else {
                switch(info->matrix->depth()) {
                    case CV_8U: long_val = *return_uchar_val; break;
                    case CV_8S: long_val = *return_schar_val; break;
                    case CV_16U: long_val = *return_ushort_val; break;
                    case CV_16S: long_val = *return_short_val; break;
                    case CV_32S: long_val = *return_int_val; break;
                    case CV_32F: double_val = *return_float_val; break;
                    case CV_64F: double_val = *return_double_val; break;

                    default: {
                        ckfree(index);
                        Tcl_SetResult(interp, (char *) "at get: wrong matrix type", TCL_STATIC);
                        return TCL_ERROR;
                    }
                }

                switch(info->matrix->depth()){
                    case CV_8U:
                    case CV_8S:
                    case CV_16U:
                    case CV_16S:
                    case CV_32S:
                        Tcl_SetObjResult(interp, Tcl_NewLongObj( long_val ));
                        break;

                    case CV_32F:
                    case CV_64F:
                        Tcl_SetObjResult(interp, Tcl_NewDoubleObj( double_val ));
                        break;

                    default: {
                        ckfree(index);
                        Tcl_SetResult(interp, (char *) "at wrong matrix type", TCL_STATIC);
                        return TCL_ERROR;
                    }
                }
            }

            ckfree(index);
            break;
        }
        case FUNC_INV: {
            int method = cv::DECOMP_LU;
            cv::Mat image, result_mat;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 2 && objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?method?");
                return TCL_ERROR;
            }

            if (objc == 3) {
                if(Tcl_GetIntFromObj(interp, objv[2], &method) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            image = *(info->matrix);
            try {
                 result_mat = image.inv(method);
            } catch (...){
                Tcl_SetResult(interp, (char *) "inv failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if ( result_mat.empty() ||  !result_mat.data ) {
                Tcl_SetResult(interp, (char *) "inv no data", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!img_info) {
                Tcl_SetResult(interp, (char *) "inv malloc MatrixInfo failed", TCL_STATIC);
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
        case FUNC_DOT: {
            Tcl_HashEntry *subhashEntryPtr;
            char *dot_handle;
            MatrixInfo *img_info;
            cv::Mat image;
            double result = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix");
                return TCL_ERROR;
            }

            dot_handle = Tcl_GetStringFromObj(objv[2], 0);
            subhashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, dot_handle );
            if( !subhashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "dot invalid MATRIX handle ",
                                            dot_handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) Tcl_GetHashValue( subhashEntryPtr );
            if ( !img_info ) {
                Tcl_SetResult(interp, (char *) "dot invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                result = image.dot(*(img_info->matrix));
            } catch (...){
                Tcl_SetResult(interp, (char *) "dot failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
            break;
        }
        case FUNC_CROSS: {
            Tcl_HashEntry *subhashEntryPtr;
            char *src_handle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            cv::Mat image, result_mat;
            MatrixInfo *img_info, *result_info;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix");
                return TCL_ERROR;
            }

            src_handle = Tcl_GetStringFromObj(objv[2], 0);
            subhashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, src_handle );
            if( !subhashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "corss invalid MATRIX handle ",
                                            src_handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) Tcl_GetHashValue( subhashEntryPtr );
            if ( !img_info ) {
                Tcl_SetResult(interp, (char *) "corss invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                /*
                 * Computes a cross-product of two 3-element vectors.
                 */
                result_mat = image.cross( *(img_info->matrix) );
            } catch (...){
                Tcl_SetResult(interp, (char *) "corss failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if ( result_mat.empty() ||  !result_mat.data ) {
                Tcl_SetResult(interp, (char *) "corss no data", TCL_STATIC);
                return TCL_ERROR;
            }

            result_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!result_info) {
                Tcl_SetResult(interp, (char *) "corss malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            result_info->matrix = new cv::Mat(result_mat);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, result_info);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_MULTIPLICATION: {
            Tcl_HashEntry *subhashEntryPtr;
            char *src_handle;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            cv::Mat image, result_mat;
            MatrixInfo *img_info, *result_info;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix");
                return TCL_ERROR;
            }

            src_handle = Tcl_GetStringFromObj(objv[2], 0);
            subhashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, src_handle );
            if( !subhashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "* invalid MATRIX handle ",
                                            src_handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) Tcl_GetHashValue( subhashEntryPtr );
            if ( !img_info ) {
                Tcl_SetResult(interp, (char *) "* invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                result_mat = image * (*(img_info->matrix));
            } catch (...){
                Tcl_SetResult(interp, (char *) "* failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if ( result_mat.empty() ||  !result_mat.data ) {
                Tcl_SetResult(interp, (char *) "* no data", TCL_STATIC);
                return TCL_ERROR;
            }

            result_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!result_info) {
                Tcl_SetResult(interp, (char *) "* malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            result_info->matrix = new cv::Mat(result_mat);

            Tcl_MutexLock(&myMutex);
            sprintf( handleName, "cv-mat%zd", matrix_count++ );

            pResultStr = Tcl_NewStringObj( handleName, -1 );

            newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
            Tcl_SetHashValue(newHashEntryPtr, result_info);
            Tcl_MutexUnlock(&myMutex);

            Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_ADD: {
            double value = 0;
            cv::Mat image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                cv::Mat result_mat = image + value;

                if ( result_mat.empty() ||  !result_mat.data ) {
                    Tcl_SetResult(interp, (char *) "add no data", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
                if (!img_info) {
                    Tcl_SetResult(interp, (char *) "add malloc MatrixInfo failed", TCL_STATIC);
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
            } catch (...){
                Tcl_SetResult(interp, (char *) "add failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_SUBTRACT: {
            double value = 0;
            cv::Mat image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                cv::Mat result_mat = image - value;

                if ( result_mat.empty() ||  !result_mat.data ) {
                    Tcl_SetResult(interp, (char *) "subtract no data", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
                if (!img_info) {
                    Tcl_SetResult(interp, (char *) "subtract malloc MatrixInfo failed", TCL_STATIC);
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
            } catch (...){
                Tcl_SetResult(interp, (char *) "subtract failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_MULTIPLY: {
            double value = 0;
            cv::Mat image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                cv::Mat result_mat = image * value;

                if ( result_mat.empty() ||  !result_mat.data ) {
                    Tcl_SetResult(interp, (char *) "multiply no data", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
                if (!img_info) {
                    Tcl_SetResult(interp, (char *) "multiply malloc MatrixInfo failed", TCL_STATIC);
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
            } catch (...){
                Tcl_SetResult(interp, (char *) "multiply failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_DIVIDE: {
            double value = 0;
            cv::Mat image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if(Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                cv::Mat result_mat = image / value;

                if ( result_mat.empty() ||  !result_mat.data ) {
                    Tcl_SetResult(interp, (char *) "divide no data", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
                if (!img_info) {
                    Tcl_SetResult(interp, (char *) "divide malloc MatrixInfo failed", TCL_STATIC);
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
            } catch (...){
                Tcl_SetResult(interp, (char *) "divide failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_TRANSPOSE: {
            cv::Mat image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            image = *(info->matrix);
            try {
                cv::Mat result_mat = image.t();

                if ( result_mat.empty() ||  !result_mat.data ) {
                    Tcl_SetResult(interp, (char *) "transpose no data", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
                if (!img_info) {
                    Tcl_SetResult(interp, (char *) "transpose malloc MatrixInfo failed", TCL_STATIC);
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
            } catch (...){
                Tcl_SetResult(interp, (char *) "transpose failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_DIAG: {
            int d = 0;
            cv::Mat image, diag_image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 2 && objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?d?");
                return TCL_ERROR;
            }

            if (objc == 3) {
                if(Tcl_GetIntFromObj(interp, objv[2], &d) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            image = *(info->matrix);
            try {
                diag_image = image.diag(d);
            } catch (...){
                Tcl_SetResult(interp, (char *) "diag failed", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!img_info) {
                Tcl_SetResult(interp, (char *) "diag malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info->matrix = new cv::Mat(diag_image);

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
        case FUNC_CROP: {
            int x = 0, y= 0, width = 0, height = 0;
            cv::Mat image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 6) {
                Tcl_WrongNumArgs(interp, 2, objv, "x y width height");
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

            image = *(info->matrix);
            try {
                cv::Mat cropped_image = image(cv::Rect(x, y, width, height));

                if ( cropped_image.empty() ||  !cropped_image.data ) {
                    Tcl_SetResult(interp, (char *) "crop no data", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
                if (!img_info) {
                    Tcl_SetResult(interp, (char *) "crop malloc MatrixInfo failed", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info->matrix = new cv::Mat(cropped_image);

                Tcl_MutexLock(&myMutex);
                sprintf( handleName, "cv-mat%zd", matrix_count++ );

                pResultStr = Tcl_NewStringObj( handleName, -1 );

                newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
                Tcl_SetHashValue(newHashEntryPtr, img_info);
                Tcl_MutexUnlock(&myMutex);

                Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (...){
                Tcl_SetResult(interp, (char *) "crop failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_RECT: {
            int x = 0, y= 0, width = 0, height = 0;
            cv::Mat image;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 6) {
                Tcl_WrongNumArgs(interp, 2, objv, "x y width height");
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

            image = *(info->matrix);
            try {
                cv::Mat rect_image(image, cv::Rect(x, y, width, height));

                if ( rect_image.empty() ||  !rect_image.data ) {
                    Tcl_SetResult(interp, (char *) "rect image data", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
                if (!img_info) {
                    Tcl_SetResult(interp, (char *) "rect malloc MatrixInfo failed", TCL_STATIC);
                    return TCL_ERROR;
                }

                img_info->matrix = new cv::Mat(rect_image);

                Tcl_MutexLock(&myMutex);
                sprintf( handleName, "cv-mat%zd", matrix_count++ );

                pResultStr = Tcl_NewStringObj( handleName, -1 );

                newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
                Tcl_SetHashValue(newHashEntryPtr, img_info);
                Tcl_MutexUnlock(&myMutex);

                Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
                    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (...){
                Tcl_SetResult(interp, (char *) "rect failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_COPYTO: {
            Tcl_HashEntry *subhashEntryPtr;
            char *copy_handle, *mask_handle;
            MatrixInfo *img_info1, *img_info2;
            cv::Mat image;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix ?mask?");
                return TCL_ERROR;
            }

            copy_handle = Tcl_GetStringFromObj(objv[2], 0);
            subhashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, copy_handle );
            if( !subhashEntryPtr ) {
                if( interp ) {
                    Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                    Tcl_AppendStringsToObj( resultObj, "copyTo invalid MATRIX handle ",
                                            copy_handle, (char *)NULL );
                }

                return TCL_ERROR;
            }

            img_info1 = (MatrixInfo *) Tcl_GetHashValue( subhashEntryPtr );
            if ( !img_info1 ) {
                Tcl_SetResult(interp, (char *) "copyTo invalid info data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (objc == 4) {
                mask_handle = Tcl_GetStringFromObj(objv[3], 0);
                subhashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mask_handle );
                if( !subhashEntryPtr ) {
                    if( interp ) {
                        Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                        Tcl_AppendStringsToObj( resultObj, "copyTo invalid MATRIX handle ",
                                                mask_handle, (char *)NULL );
                    }

                    return TCL_ERROR;
                }

                img_info2 = (MatrixInfo *) Tcl_GetHashValue( subhashEntryPtr );
                if ( !img_info2 ) {
                    Tcl_SetResult(interp, (char *) "copyTo invalid info data", TCL_STATIC);
                    return TCL_ERROR;
                }
            }

            image = *(info->matrix);
            try {
                if (objc == 3) {
                    image.copyTo(*(img_info1->matrix));
                } else {
                    image.copyTo(*(img_info1->matrix), *(img_info2->matrix));
                }
            } catch (...){
                Tcl_SetResult(interp, (char *) "copyTo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_CONVERTTO: {
            int type = 0;
            double scale = 1, shift = 0;
            cv::Mat image, imageChanged;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 3 && objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "type ?scale shift?");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &type) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 5) {
                if(Tcl_GetDoubleFromObj(interp, objv[3], &scale) != TCL_OK) {
                    return TCL_ERROR;
                }

                if(Tcl_GetDoubleFromObj(interp, objv[4], &shift) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            image = *(info->matrix);

            try {
                image.convertTo(imageChanged, type, scale, shift);
            } catch (...){
                Tcl_SetResult(interp, (char *) "convertTo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if ( imageChanged.empty() ||  !imageChanged.data ) {
                Tcl_SetResult(interp, (char *) "convertTo no data", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!img_info) {
                Tcl_SetResult(interp, (char *) "convertTo malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info->matrix = new cv::Mat(imageChanged);

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
        case FUNC_COLRANGE: {
            int startcol = 0, endcol = 0;
            cv::Mat image, imageChanged;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "startcol endcol");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &startcol) != TCL_OK) {
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[3], &endcol) != TCL_OK) {
                return TCL_ERROR;
            }

            image = *(info->matrix);

            try {
                /*
                 * startcol: An inclusive 0-based start index of the column span.
                 * endcol: An exclusive 0-based ending index of the column span.
                 */
                imageChanged = image.colRange(startcol, endcol);
            } catch (...){
                Tcl_SetResult(interp, (char *) "colRange failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if ( imageChanged.empty() ||  !imageChanged.data ) {
                Tcl_SetResult(interp, (char *) "colRange no data", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!img_info) {
                Tcl_SetResult(interp, (char *) "colRange malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info->matrix = new cv::Mat(imageChanged);

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
        case FUNC_ROWRANGE: {
            int startcol = 0, endcol = 0;
            cv::Mat image, imageChanged;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "startcol endcol");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &startcol) != TCL_OK) {
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[3], &endcol) != TCL_OK) {
                return TCL_ERROR;
            }

            image = *(info->matrix);

            try {
                /*
                 * startcol: An inclusive 0-based start index of the column span.
                 * endcol: An exclusive 0-based ending index of the column span.
                 */
                imageChanged = image.rowRange(startcol, endcol);
            } catch (...){
                Tcl_SetResult(interp, (char *) "rowRange failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if ( imageChanged.empty() ||  !imageChanged.data ) {
                Tcl_SetResult(interp, (char *) "rowRange no data", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!img_info) {
                Tcl_SetResult(interp, (char *) "rowRange malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info->matrix = new cv::Mat(imageChanged);

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
        case FUNC_RESHAPE: {
            int cn = 0, rows = 0;
            cv::Mat image, dstimage;
            Tcl_HashEntry *newHashEntryPtr;
            char handleName[16 + TCL_INTEGER_SPACE];
            Tcl_Obj *pResultStr = NULL;
            int newvalue;
            MatrixInfo *img_info;

            if( objc != 3 && objc != 4){
                Tcl_WrongNumArgs(interp, 2, objv, "cn ?rows?");
                return TCL_ERROR;
            }

            if(Tcl_GetIntFromObj(interp, objv[2], &cn) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                if(Tcl_GetIntFromObj(interp, objv[3], &rows) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            image = *(info->matrix);
            try {
                dstimage = image.reshape(cn, rows);
            } catch (...){
                Tcl_SetResult(interp, (char *) "reshape failed", TCL_STATIC);
                return TCL_ERROR;
            }

            if ( dstimage.empty() ||  !dstimage.data ) {
                Tcl_SetResult(interp, (char *) "reshape no data", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
            if (!img_info) {
                Tcl_SetResult(interp, (char *) "reshape malloc MatrixInfo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            img_info->matrix = new cv::Mat(dstimage);

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
        case FUNC_SETDATA: {
            int count = 0;
            int depth = info->matrix->depth();
            long max_data_len = info->matrix->total() * info->matrix->channels();
            uchar *orig_data = info->matrix->data;
            double val;

            if( objc != 3 ){
                Tcl_WrongNumArgs(interp, 2, objv, "list_data");
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "setData invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count > max_data_len) {
                Tcl_SetResult(interp, (char *) "setData out of max range", TCL_STATIC);
                return TCL_ERROR;
            }

            for (int i = 0; i < count; i++) {
                Tcl_Obj *elemListPtr = NULL;

                Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                if(Tcl_GetDoubleFromObj(interp, elemListPtr, &val) != TCL_OK) {
                    return TCL_ERROR;
                }

                switch(depth) {
                    case CV_8U:   ((uchar *) orig_data)[i] = (uchar) val; break;
                    case CV_8S:   ((schar *) orig_data)[i] = (schar) val; break;
                    case CV_16U:  ((ushort *) orig_data)[i] = (ushort) val; break;
                    case CV_16S:  ((short *) orig_data)[i] = (short) val; break;
                    case CV_32S:  ((int *) orig_data)[i] = (int) val; break;
                    case CV_32F:  ((float *) orig_data)[i] = (float) val; break;
                    case CV_64F:  ((double* ) orig_data)[i] = (double) val; break;
                }
            }
            break;
        }
        case FUNC_SETTO: {
            int B = 0, G = 0, R = 0, A = 0, count = 0;
            cv::Mat image;
            Tcl_HashEntry *subhashEntryPtr;
            char *mask_handle;
            MatrixInfo *img_info;

            if( objc != 3 && objc != 4){
                Tcl_WrongNumArgs(interp, 2, objv, "color_list ?mask?");
                return TCL_ERROR;
            }

            if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "setTo invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count != 4) {
                Tcl_SetResult(interp, (char *) "setTo invalid color data", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elemListPtr = NULL;

                Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
                if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
                if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[2], 2, &elemListPtr);
                if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[2], 3, &elemListPtr);
                if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            if (objc == 4) {
                mask_handle = Tcl_GetStringFromObj(objv[3], 0);
                subhashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mask_handle );
                if( !subhashEntryPtr ) {
                    if( interp ) {
                        Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                        Tcl_AppendStringsToObj( resultObj, "setTo invalid MATRIX handle ",
                                                mask_handle, (char *)NULL );
                    }

                    return TCL_ERROR;
                }

                img_info = (MatrixInfo *) Tcl_GetHashValue( subhashEntryPtr );
                if ( !img_info ) {
                    Tcl_SetResult(interp, (char *) "setTo invalid info data", TCL_STATIC);
                    return TCL_ERROR;
                }
            }

            image = *(info->matrix);
            try {
                cv::Scalar color(B, G, R, A);

                if (objc == 3) {
                    image.setTo(color);
                } else {
                    image.setTo(color, *(img_info->matrix));
                }
            } catch (...){
                Tcl_SetResult(interp, (char *) "setTo failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            delete info->matrix;
            ckfree(info);

            Tcl_MutexLock(&myMutex);
            if( hashEntryPtr )  Tcl_DeleteHashEntry(hashEntryPtr);
            Tcl_MutexUnlock(&myMutex);

            Tcl_DeleteCommand(interp, handle);
            Tcl_SetObjResult(interp, Tcl_NewIntObj( 0 ));

            break;
        }
    }

    return TCL_OK;
}


int tcl_CV_8UC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int num = 0, result = 0;

    if (objc !=2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_8UC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
    return TCL_OK;
}


int tcl_CV_8SC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int num = 0, result = 0;

    if (objc !=2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_8SC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
    return TCL_OK;
}


int tcl_CV_16UC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int num = 0, result = 0;

    if (objc !=2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_16UC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
    return TCL_OK;
}


int tcl_CV_16SC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int num = 0, result = 0;

    if (objc !=2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_16SC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
    return TCL_OK;
}


int tcl_CV_32SC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int num = 0, result = 0;

    if (objc !=2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_32SC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
    return TCL_OK;
}


int tcl_CV_32FC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int num = 0, result = 0;

    if (objc !=2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_32FC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
    return TCL_OK;
}


int tcl_CV_64FC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int num = 0, result = 0;

    if (objc !=2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_64FC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj( result ));
    return TCL_OK;
}


int mat_mat(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int rows, cols, type;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    cv::Mat mat_mat;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "rows cols type ?color_list?");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &rows) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &cols) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
            Tcl_SetResult(interp, (char *) "mat invalid list data", TCL_STATIC);
            return TCL_ERROR;
        }

        if (count != 4) {
            Tcl_SetResult(interp, (char *) "mat invalid color data", TCL_STATIC);
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
    }

    if (objc == 5) {
        cv::Scalar color(B, G, R, A);
        mat_mat = cv::Mat(rows, cols, type, color);
    } else {
        mat_mat = cv::Mat(rows, cols, type);
    }

    info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "mat malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->matrix = new cv::Mat(mat_mat);

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


int mat_matwithdims(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int dims, *index, type;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    cv::Mat mat_mat;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "dims size_list type ?color_list?");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &dims) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "matwithdims invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != dims) {
        Tcl_SetResult(interp, (char *) "matwithdims wrong list number", TCL_STATIC);
        return TCL_ERROR;
    }

    index = (int *) ckalloc (sizeof(int) * dims);
    if (!index) {
        Tcl_SetResult(interp, (char *) "matwithdims malloc failed", TCL_STATIC);
        return TCL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        Tcl_Obj *elemListPtr = NULL;
        int index_number;

        Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &index_number) != TCL_OK) {
            ckfree(index);
            return TCL_ERROR;
        }

        index[i] = index_number;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        ckfree(index);
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
            ckfree(index);
            Tcl_SetResult(interp, (char *) "matwithdims invalid list data", TCL_STATIC);
            return TCL_ERROR;
        }

        if (count != 4) {
            ckfree(index);
            Tcl_SetResult(interp, (char *) "matwithdims invalid color data", TCL_STATIC);
            return TCL_ERROR;
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[4], 0, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 1, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 2, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 3, &elemListPtr);
            if(Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }
        }
    }

    if (objc == 5) {
        cv::Scalar color(B, G, R, A);
        mat_mat = cv::Mat(dims, index, type, color);
    } else {
        mat_mat = cv::Mat(dims, index, type);
    }

    info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!info) {
        ckfree(index);
        Tcl_SetResult(interp, (char *) "matwithdims malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->matrix = new cv::Mat(mat_mat);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    ckfree(index);
    return TCL_OK;
}


int mat_diag(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image, diag_image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *dstinfo;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "diag invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "diag invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    /*
     * One-dimensional matrix that represents the main diagonal.
     */
    image = *(info->matrix);
    diag_image = cv::Mat::diag(image);

    dstinfo = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "diag malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->matrix = new cv::Mat(diag_image);

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


int mat_eye(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int rows, cols, type;
    cv::Mat eye_mat;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;

    if (objc !=4) {
        Tcl_WrongNumArgs(interp, 1, objv, "rows cols type");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &rows) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &cols) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    eye_mat = cv::Mat::eye(rows, cols, type);
    info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "eye malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->matrix = new cv::Mat(eye_mat);

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


int mat_ones(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int rows, cols, type;
    cv::Mat ones_mat;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;

    if (objc !=4) {
        Tcl_WrongNumArgs(interp, 1, objv, "rows cols type");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &rows) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &cols) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    ones_mat = cv::Mat::ones(rows, cols, type);
    info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "ones malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->matrix = new cv::Mat(ones_mat);

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


int mat_zeros(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int rows, cols, type;
    cv::Mat zeros_mat;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;

    if (objc !=4) {
        Tcl_WrongNumArgs(interp, 1, objv, "rows cols type");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &rows) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &cols) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    zeros_mat = cv::Mat::zeros(rows, cols, type);
    info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!info) {
        Tcl_SetResult(interp, (char *) "zeros malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    info->matrix = new cv::Mat(zeros_mat);

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


int mat_abs(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *mat_info;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "abs invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "abs invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        image = cv::abs(*(info1->matrix));
    } catch (...){
        Tcl_SetResult(interp, (char *) "abs failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "abs malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_absdiff(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *mat_info;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "absdiff invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "absdiff invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "absdiff invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "absdiff invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::absdiff(*(info1->matrix), *(info2->matrix), image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "absdiff failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "absdiff malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_add(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *add_info;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "add invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "add invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "add invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "add invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::add(*(info1->matrix), *(info2->matrix), image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "add failed", TCL_STATIC);
        return TCL_ERROR;
    }

    add_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!add_info) {
        Tcl_SetResult(interp, (char *) "add malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    add_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, add_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_addWeighted(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    double alpha = 0.0, beta = 0.0, gamma = 0.0;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *add_info;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 alpha matrix_2 beta gamma");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "addWeighted invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "addWeighted invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[3], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "addWeighted invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "addWeighted invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[4], &beta) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[5], &gamma) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::addWeighted(*(info1->matrix), alpha, *(info2->matrix), beta, gamma, image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "addWeighted failed", TCL_STATIC);
        return TCL_ERROR;
    }

    add_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!add_info) {
        Tcl_SetResult(interp, (char *) "addWeighted malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    add_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, add_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_bitwise_and(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle, *mask_handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2, *info3;
    MatrixInfo *matrix_info;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?mask?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "bitwise_and invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "bitwise_and invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "bitwise_and invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "bitwise_and invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        mask_handle = Tcl_GetStringFromObj(objv[3], 0);
        hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mask_handle );
        if( !hashEntryPtr ) {
            if( interp ) {
                Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                Tcl_AppendStringsToObj( resultObj, "bitwise_and invalid MATRIX handle ",
                                        mask_handle, (char *)NULL );
            }

            return TCL_ERROR;
        }

        info3 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
        if ( !info3 ) {
            Tcl_SetResult(interp, (char *) "bitwise_and invalid info data", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 3) {
            cv::bitwise_and(*(info1->matrix), *(info2->matrix), image);
        } else {
            cv::bitwise_and(*(info1->matrix), *(info2->matrix), image, *(info3->matrix));
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "bitwise_and failed", TCL_STATIC);
        return TCL_ERROR;
    }

    matrix_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!matrix_info) {
        Tcl_SetResult(interp, (char *) "bitwise_and malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    matrix_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, matrix_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_bitwise_or(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle, *mask_handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2, *info3;
    MatrixInfo *matrix_info;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?mask?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "bitwise_or invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "bitwise_or invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "bitwise_or invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "bitwise_or invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        mask_handle = Tcl_GetStringFromObj(objv[3], 0);
        hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mask_handle );
        if( !hashEntryPtr ) {
            if( interp ) {
                Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                Tcl_AppendStringsToObj( resultObj, "bitwise_or invalid MATRIX handle ",
                                        mask_handle, (char *)NULL );
            }

            return TCL_ERROR;
        }

        info3 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
        if ( !info3 ) {
            Tcl_SetResult(interp, (char *) "bitwise_or invalid info data", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 3) {
            cv::bitwise_or(*(info1->matrix), *(info2->matrix), image);
        } else {
            cv::bitwise_or(*(info1->matrix), *(info2->matrix), image, *(info3->matrix));
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "bitwise_or failed", TCL_STATIC);
        return TCL_ERROR;
    }

    matrix_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!matrix_info) {
        Tcl_SetResult(interp, (char *) "bitwise_or malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    matrix_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, matrix_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_bitwise_xor(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle, *mask_handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2, *info3;
    MatrixInfo *matrix_info;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?mask?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "bitwise_xor invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "bitwise_xor invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "bitwise_xor invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "bitwise_xor invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        mask_handle = Tcl_GetStringFromObj(objv[3], 0);
        hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mask_handle );
        if( !hashEntryPtr ) {
            if( interp ) {
                Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                Tcl_AppendStringsToObj( resultObj, "bitwise_xor invalid MATRIX handle ",
                                        mask_handle, (char *)NULL );
            }

            return TCL_ERROR;
        }

        info3 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
        if ( !info3 ) {
            Tcl_SetResult(interp, (char *) "bitwise_xor invalid info data", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 3) {
            cv::bitwise_xor(*(info1->matrix), *(info2->matrix), image);
        } else {
            cv::bitwise_xor(*(info1->matrix), *(info2->matrix), image, *(info3->matrix));
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "bitwise_xor failed", TCL_STATIC);
        return TCL_ERROR;
    }

    matrix_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!matrix_info) {
        Tcl_SetResult(interp, (char *) "bitwise_xor malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    matrix_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, matrix_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_bitwise_not(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *mask_handle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *img_info;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?mask?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "bitwise_not invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "bitwise_not invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 3) {
        mask_handle = Tcl_GetStringFromObj(objv[2], 0);
        hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, mask_handle );
        if( !hashEntryPtr ) {
            if( interp ) {
                Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                Tcl_AppendStringsToObj( resultObj, "bitwise_not invalid MATRIX handle ",
                                        mask_handle, (char *)NULL );
            }

            return TCL_ERROR;
        }

        info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
        if ( !info2 ) {
            Tcl_SetResult(interp, (char *) "bitwise_not invalid info data", TCL_STATIC);
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 2) {
            cv::bitwise_not(*(info1->matrix), dstimage);
        } else {
            cv::bitwise_not(*(info1->matrix), dstimage, *(info2->matrix));
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "bitwise_not failed", TCL_STATIC);
        return TCL_ERROR;
    }

    img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!img_info) {
        Tcl_SetResult(interp, (char *) "bitwise_not malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    img_info->matrix = new cv::Mat(dstimage);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, img_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_cartToPolar(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int angleInDegrees = 0;
    cv::Mat magnitude, angle;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *magnitude_info, *angle_info;

    if  (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?angleInDegrees?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "cartToPolar invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "cartToPolar invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "cartToPolar invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "cartToPolar invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetBooleanFromObj(interp, objv[3], &angleInDegrees) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::cartToPolar(*(info1->matrix), *(info2->matrix), magnitude, angle, angleInDegrees);
    } catch (...){
        Tcl_SetResult(interp, (char *) "cartToPolar failed", TCL_STATIC);
        return TCL_ERROR;
    }

    magnitude_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!magnitude_info) {
        Tcl_SetResult(interp, (char *) "cartToPolar malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    magnitude_info->matrix = new cv::Mat(magnitude);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr1 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, magnitude_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    angle_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!angle_info) {
        Tcl_SetResult(interp, (char *) "cartToPolar malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    angle_info->matrix = new cv::Mat(angle);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr2 = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, angle_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_convertScaleAbs(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double alpha = 1, beta = 0;
    cv::Mat dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *img_info;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix alpha beta");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "convertScaleAbs invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "convertScaleAbs invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &beta) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::convertScaleAbs(*(info->matrix), dstimage, alpha, beta);
    } catch (...){
        Tcl_SetResult(interp, (char *) "convertScaleAbs failed", TCL_STATIC);
        return TCL_ERROR;
    }

    img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!img_info) {
        Tcl_SetResult(interp, (char *) "convertScaleAbs malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    img_info->matrix = new cv::Mat(dstimage);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, img_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_copyMakeBorder(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int top = 0, bottom = 0, left = 0, right = 0, borderType = cv::BORDER_DEFAULT;
    int B = 0, G = 0, R = 0, A = 0;
    int count = 0;
    cv::Mat image, dstimage;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info;
    MatrixInfo *img_info;

    if (objc != 7 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix top bottom left right borderType ?color_list?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "copyMakeBorder invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "copyMakeBorder invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &top) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &bottom) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &left) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[5], &right) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if(Tcl_ListObjLength(interp, objv[7], &count) != TCL_OK) {
            Tcl_SetResult(interp, (char *) "copyMakeBorder invalid list data", TCL_STATIC);
            return TCL_ERROR;
        }

        if (count != 4) {
            Tcl_SetResult(interp, (char *) "copyMakeBorder invalid color data", TCL_STATIC);
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
    }

    image = *(info->matrix);

    try {
        cv::Scalar color(B, G, R, A);

        if (borderType == cv::BORDER_CONSTANT) {
            cv::copyMakeBorder(image, dstimage, top, bottom, left, right, borderType, color);
        } else {
            cv::copyMakeBorder(image, dstimage, top, bottom, left, right, borderType);
        }
    } catch (...){
        Tcl_SetResult(interp, (char *) "copyMakeBorder failed", TCL_STATIC);
        return TCL_ERROR;
    }

    img_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!img_info) {
        Tcl_SetResult(interp, (char *) "copyMakeBorder malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    img_info->matrix = new cv::Mat(dstimage);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, img_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_divide(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double scale = 1.0;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *mat_info;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?scale?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "divide invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "divide invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "divide invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "divide invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetDoubleFromObj(interp, objv[3], &scale) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::divide(*(info1->matrix), *(info2->matrix), image, scale);
    } catch (...){
        Tcl_SetResult(interp, (char *) "divide failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "divide malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_flip(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int flipCode = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *flip_info;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix flipCode");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "flip invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "flip invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    /*
     * 0 means flipping around the x-axis and positive value (for example, 1)
     * means flipping around y-axis. Negative value (for example, -1) means
     * flipping around both axes.
     */
    if(Tcl_GetIntFromObj(interp, objv[2], &flipCode) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::flip(*(info1->matrix), image, flipCode);
    } catch (...){
        Tcl_SetResult(interp, (char *) "flip failed", TCL_STATIC);
        return TCL_ERROR;
    }

    flip_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!flip_info) {
        Tcl_SetResult(interp, (char *) "flip malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    flip_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, flip_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_getOptimalDFTSize(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int vecsize = 0;
    int result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "vecsize");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &vecsize) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        result = cv::getOptimalDFTSize(vecsize);
    } catch (...){
        Tcl_SetResult(interp, (char *) "getOptimalDFTSize failed", TCL_STATIC);
        return TCL_ERROR;
    }

    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int mat_dft(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int flags = 0, nonzeroRows = 0;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *mat_info;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?falgs nonzeroRows?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "dft invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "dft invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }

        if(Tcl_GetIntFromObj(interp, objv[3], &nonzeroRows) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::dft(*(info1->matrix), image, flags, nonzeroRows);
    } catch (...){
        Tcl_SetResult(interp, (char *) "dft failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "dft malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_inRange(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    int count = 0;
    int B1 = 0, G1 = 0, R1 = 0, A1 = 0;
    int B2 = 0, G2 = 0, R2 = 0, A2 = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *range_info;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix color_list1 color_list2");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "inRange invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "inRange invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "inRange invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "inRange invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A1) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if(Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "inRange invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count != 4) {
        Tcl_SetResult(interp, (char *) "inRange invalid color data", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &B2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &G2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 2, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &R2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 3, &elemListPtr);
        if(Tcl_GetIntFromObj(interp, elemListPtr, &A2) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::inRange(*(info1->matrix), cv::Scalar(B1, G1, R1, A1),
                    cv::Scalar(B2, G2, R2, A2), image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "inRange failed", TCL_STATIC);
        return TCL_ERROR;
    }

    range_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!range_info) {
        Tcl_SetResult(interp, (char *) "inRange malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    range_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, range_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_log(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *log_info;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "log invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "log invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::log(*(info1->matrix), image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "log failed", TCL_STATIC);
        return TCL_ERROR;
    }

    log_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!log_info) {
        Tcl_SetResult(interp, (char *) "log malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    log_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, log_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_lut(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *mat_info;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix1 matrix2");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "lut invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "lut invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "lut invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "lut invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::LUT(*(info1->matrix), *(info2->matrix), image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "lut failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "lut malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_magnitude(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *mat_info;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix1 matrix2");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "magnitude invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "magnitude invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "magnitude invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "magnitude invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::magnitude(*(info1->matrix), *(info2->matrix), image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "magnitude failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "magnitude malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_meanStdDev(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat mean, stddev;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName1[16 + TCL_INTEGER_SPACE], handleName2[16 + TCL_INTEGER_SPACE];;
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *mat_info1, *mat_info2;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "meanStdDev invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "meanStdDev invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::meanStdDev(*(info1->matrix), mean, stddev);
    } catch (...){
        Tcl_SetResult(interp, (char *) "meanStdDev failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info1 = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info1) {
        Tcl_SetResult(interp, (char *) "meanStdDev malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info1->matrix = new cv::Mat(mean);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName1, "cv-mat%zd", matrix_count++ );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName1, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info1);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName1, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    mat_info2 = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info2) {
        Tcl_SetResult(interp, (char *) "meanStdDev malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info2->matrix = new cv::Mat(stddev);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName2, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName2, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName2, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info2);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName2, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewStringObj( handleName1, -1 ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewStringObj( handleName2, -1 ));
    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_minMaxLoc(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    MatrixInfo *info1;
    Tcl_Obj *pResultStr = NULL, *pSubResultStr = NULL;
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "minMaxLoc invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "minMaxLoc invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::minMaxLoc(*(info1->matrix), &minVal, &maxVal, &minLoc, &maxLoc);
    } catch (...){
        Tcl_SetResult(interp, (char *) "minMaxLoc failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    pSubResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewDoubleObj( minVal ));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj( minLoc.x ));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj( minLoc.y ));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubResultStr);

    pSubResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewDoubleObj( maxVal ));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj( maxLoc.x ));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj( maxLoc.y ));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubResultStr);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_split(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *shandle;
    MatrixInfo *info;
    Tcl_Obj *listPtr;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    shandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "split invalid MATRIX handle ",
                                    shandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "split invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    image = *(info->matrix);

    std::vector<cv::Mat> channels;
    try {
        cv::split(image, channels);
    } catch (...){
        Tcl_SetResult(interp, (char *) "split failed", TCL_STATIC);
        return TCL_ERROR;
    }

    if(channels.size() == 0) {
        Tcl_SetResult(interp, (char *) "split channels is 0", TCL_STATIC);
        return TCL_ERROR;
    }

    listPtr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < channels.size(); i++) {
        Tcl_HashEntry *newHashEntryPtr = NULL;
        char handleName[16 + TCL_INTEGER_SPACE];
        Tcl_Obj *pStr = NULL;
        int newvalue;
        MatrixInfo *split_info = NULL;

        split_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
        if (!split_info) {
            Tcl_SetResult(interp, (char *) "split malloc MatrixInfo failed", TCL_STATIC);
            return TCL_ERROR;
        }

        split_info->matrix = new cv::Mat(channels[i]);

        Tcl_MutexLock(&myMutex);
        sprintf( handleName, "cv-mat%zd", matrix_count++ );

        pStr = Tcl_NewStringObj( handleName, -1 );

        newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
        Tcl_SetHashValue(newHashEntryPtr, split_info);
        Tcl_MutexUnlock(&myMutex);

        Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
            (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

        Tcl_ListObjAppendElement(NULL, listPtr, pStr);
    }

    Tcl_SetObjResult(interp, listPtr);
    return TCL_OK;
}


int mat_multiply(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double scale = 1.0;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *mat_info;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?scale?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "multiply invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "multiply invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "multiply invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "multiply invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 4) {
        if(Tcl_GetDoubleFromObj(interp, objv[3], &scale) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::multiply(*(info1->matrix), *(info2->matrix), image, scale);
    } catch (...){
        Tcl_SetResult(interp, (char *) "multiply failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "multiply malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_mulSpectrums(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int flags = 0, conjB = 0;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *mat_info;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 flags ?conjB?");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "mulSpectrums invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "mulSpectrums invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "mulSpectrums invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "mulSpectrums invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if(Tcl_GetBooleanFromObj(interp, objv[4], &conjB) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::mulSpectrums(*(info1->matrix), *(info2->matrix), image, flags, conjB);
    } catch (...){
        Tcl_SetResult(interp, (char *) "mulSpectrums failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "mulSpectrums malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_merge(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    std::vector<cv::Mat> channels;
    cv::Mat image;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *merge_info;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_list");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "merge: invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count == 0) {
        Tcl_SetResult(interp, (char *) "merge: empty list", TCL_STATIC);
        return TCL_ERROR;
    }

    for(int i = 0; i < count; i++) {
        Tcl_HashEntry *hashEntryPtr;
        char *shandle;
        MatrixInfo *info;
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
        shandle = Tcl_GetStringFromObj(elemListPtr, 0);

        hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, shandle );
        if( !hashEntryPtr ) {
            if( interp ) {
                Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
                Tcl_AppendStringsToObj( resultObj, "merge: invalid MATRIX handle ",
                                        shandle, (char *)NULL );
            }

            return TCL_ERROR;
        }

        info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
        if ( !info ) {
            Tcl_SetResult(interp, (char *) "merge: invalid info data", TCL_STATIC);
            return TCL_ERROR;
        }

        channels.push_back(*(info->matrix));
    }

    try {
        cv::merge(channels, image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "merge failed", TCL_STATIC);
        return TCL_ERROR;
    }

    merge_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!merge_info) {
        Tcl_SetResult(interp, (char *) "merge: malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    merge_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, merge_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_norm(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int norm_type = cv::NORM_L2;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    double result;
    MatrixInfo *info1;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix norm_type");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "norm: invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "norm: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &norm_type) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        result = cv::norm(*(info1->matrix), norm_type);
    } catch (...){
        Tcl_SetResult(interp, (char *) "norm failed", TCL_STATIC);
        return TCL_ERROR;
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int mat_normalize(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double alpha = 0, beta = 0;
    int norm_type = cv::NORM_L2;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *result_info;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix alpha beta norm_type");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "normalize: invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "normalize: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &beta) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[4], &norm_type) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::normalize(*(info1->matrix), image, alpha, beta, norm_type);
    } catch (...){
        Tcl_SetResult(interp, (char *) "normalize failed", TCL_STATIC);
        return TCL_ERROR;
    }

    result_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!result_info) {
        Tcl_SetResult(interp, (char *) "normalize: malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    result_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, result_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_pow(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double power = 0;
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *result_info;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix power");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "pow: invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "pow: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[2], &power) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::pow(*(info1->matrix), power, image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "pow failed", TCL_STATIC);
        return TCL_ERROR;
    }

    result_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!result_info) {
        Tcl_SetResult(interp, (char *) "pow: malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    result_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, result_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_randu(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int min = 0, max = 0;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    MatrixInfo *info1;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix min max");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "randu: invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "randu: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &min) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[3], &max) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        /*
         * Generates a single uniformly-distributed random number or an array of random numbers.
         */
        cv::randu(*(info1->matrix), cv::Scalar(min), cv::Scalar(max));
    } catch (...){
        Tcl_SetResult(interp, (char *) "randu failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int mat_sqrt(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1;
    MatrixInfo *mat_info;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "sqrt: invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "sqrt: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::sqrt(*(info1->matrix), image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "sqrt failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "sqrt: malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_subtract(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Mat image;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle, *bhandle;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    MatrixInfo *info1, *info2;
    MatrixInfo *mat_info;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "subtract: invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "subtract: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "subtract: invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info2 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info2 ) {
        Tcl_SetResult(interp, (char *) "subtract: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::subtract(*(info1->matrix), *(info2->matrix), image);
    } catch (...){
        Tcl_SetResult(interp, (char *) "subtract failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info = (MatrixInfo *) ckalloc(sizeof(MatrixInfo));
    if (!mat_info) {
        Tcl_SetResult(interp, (char *) "subtract: malloc MatrixInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    mat_info->matrix = new cv::Mat(image);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-mat%zd", matrix_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, mat_info);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) MATRIX_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_sum(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    cv::Scalar result;
    Tcl_HashEntry *hashEntryPtr;
    char *ahandle;
    Tcl_Obj *pResultStr = NULL;
    MatrixInfo *info1;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    ahandle = Tcl_GetStringFromObj(objv[1], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, ahandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "sum: invalid MATRIX handle ",
                                    ahandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info1 = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info1 ) {
        Tcl_SetResult(interp, (char *) "sum: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        result = cv::sum(*(info1->matrix));
    } catch (...){
        Tcl_SetResult(interp, (char *) "sum failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( result[0] ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( result[1] ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( result[2] ));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj( result[3] ));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int perspectiveTransform(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int count = 0, npts = 0;
    cv::Mat mat_image;
    std::vector<cv::Point2f> src_points, dst_points;
    Tcl_HashEntry *hashEntryPtr;
    char *bhandle;
    Tcl_Obj *pResultStr = NULL;
    MatrixInfo *info;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_list transformation_matrix");
        return TCL_ERROR;
    }

    if(Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "perspectiveTransform invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count%2 != 0) {
        Tcl_SetResult(interp, (char *) "perspectiveTransform invalid point data", TCL_STATIC);
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

    bhandle = Tcl_GetStringFromObj(objv[2], 0);
    hashEntryPtr = Tcl_FindHashEntry( cv_hashtblPtr, bhandle );
    if( !hashEntryPtr ) {
        if( interp ) {
            Tcl_Obj *resultObj = Tcl_GetObjResult( interp );
            Tcl_AppendStringsToObj( resultObj, "perspectiveTransform: invalid MATRIX handle ",
                                    bhandle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (MatrixInfo *) Tcl_GetHashValue( hashEntryPtr );
    if ( !info ) {
        Tcl_SetResult(interp, (char *) "perspectiveTransform: invalid info data", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cv::perspectiveTransform(src_points, dst_points, *(info->matrix));
    } catch (...){
        Tcl_SetResult(interp, (char *) "perspectiveTransform failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < dst_points.size(); i++) {
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( (int) dst_points[i].x ));
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj( (int) dst_points[i].y ));
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int getTickCount(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double result;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        result = (double) cv::getTickCount();
    } catch (...){
        Tcl_SetResult(interp, (char *) "getTickCount failed", TCL_STATIC);
        return TCL_ERROR;
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj (result));
    return TCL_OK;
}


int getTickFrequency(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    double result;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        result = cv::getTickFrequency();
    } catch (...){
        Tcl_SetResult(interp, (char *) "getTickFrequency failed", TCL_STATIC);
        return TCL_ERROR;
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj (result));
    return TCL_OK;
}


int TERM_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int choice;
    Tcl_HashEntry *hashEntryPtr;
    char *handle;
    TermCriteriaInfo *info;

    static const char *FUNC_strs[] = {
        "close",
        0
    };

    enum FUNC_enum {
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
            Tcl_AppendStringsToObj( resultObj, "TermCriteria: invalid video capture handle ",
                                    handle, (char *)NULL );
        }

        return TCL_ERROR;
    }

    info = (TermCriteriaInfo *) Tcl_GetHashValue( hashEntryPtr );

    switch( (enum FUNC_enum)choice ){
        case FUNC_CLOSE: {
            if( objc != 2 ){
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            delete info->termCriteria;
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


int TermCriteria(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv){
    int type = 1, maxCount = 1;
    double epsilon = 1;
    cv::TermCriteria term_crit;
    Tcl_HashEntry *newHashEntryPtr;
    char handleName[16 + TCL_INTEGER_SPACE];
    Tcl_Obj *pResultStr = NULL;
    int newvalue;
    TermCriteriaInfo *dstinfo;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 2, objv, "type maxCount epsilon");
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[1], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetIntFromObj(interp, objv[2], &maxCount) != TCL_OK) {
        return TCL_ERROR;
    }

    if(Tcl_GetDoubleFromObj(interp, objv[3], &epsilon) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        term_crit = cv::TermCriteria(type, maxCount, epsilon);
    } catch (...){
        Tcl_SetResult(interp, (char *) "TermCriteria failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo = (TermCriteriaInfo *) ckalloc(sizeof(TermCriteriaInfo));
    if (!dstinfo) {
        Tcl_SetResult(interp, (char *) "TermCriteria malloc TermCriteriaInfo failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstinfo->termCriteria = new cv::TermCriteria(term_crit);

    Tcl_MutexLock(&myMutex);
    sprintf( handleName, "cv-term%d", term_count++ );

    pResultStr = Tcl_NewStringObj( handleName, -1 );

    newHashEntryPtr = Tcl_CreateHashEntry(cv_hashtblPtr, handleName, &newvalue);
    Tcl_SetHashValue(newHashEntryPtr, dstinfo);
    Tcl_MutexUnlock(&myMutex);

    Tcl_CreateObjCommand(interp, handleName, (Tcl_ObjCmdProc *) TERM_FUNCTION,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
