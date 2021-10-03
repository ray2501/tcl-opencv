#include "tclopencv.h"

#ifdef TCL_USE_TKPHOTO
#include <tk.h>
#endif

#ifdef TCL_USE_VECTCL
#ifdef __cplusplus
extern "C" {
#endif
#include <vectcl.h>
#ifdef __cplusplus
}
#endif
#endif

#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

int MATRIX_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *) cd;
    int choice;
    cv::Mat *mat;

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
        "pop_back",
        "push_back",
        "reshape",
        "setData",
        "setTo",
#ifdef TCL_USE_TKPHOTO
        "toPhoto",
        "fromPhoto",
#endif
#ifdef TCL_USE_VECTCL
        "toNumArray",
        "fromNumArray",
#endif
        "close",
        "_command",
        "_name",
        "_type",
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
        FUNC_POP_BACK,
        FUNC_PUSH_BACK,
        FUNC_RESHAPE,
        FUNC_SETDATA,
        FUNC_SETTO,
#ifdef TCL_USE_TKPHOTO
        FUNC_TOPHOTO,
        FUNC_FROMPHOTO,
#endif
#ifdef TCL_USE_VECTCL
        FUNC_TONUMARRAY,
        FUNC_FROMNUMARRAY,
#endif
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    cd = (void *) cvo->top;
    mat = (cv::Mat *) cvo->obj;
    if (!mat) {
        Tcl_Panic("null Matrix object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DIMS: {
            int dims = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            dims = mat->dims;
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
            for (int i = 0; i < mat->dims; i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(mat->size[i]));

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

            rows = mat->rows;
            Tcl_SetObjResult(interp, Tcl_NewIntObj(rows));
            break;
        }
        case FUNC_COLS: {
            int cols = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            cols = mat->cols;
            Tcl_SetObjResult(interp, Tcl_NewIntObj(cols));
            break;
        }
        case FUNC_CHANNELS: {
            int channels = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            channels = mat->channels();
            Tcl_SetObjResult(interp, Tcl_NewIntObj(channels));
            break;
        }
        case FUNC_DEPTH: {
            int depth = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            depth = mat->depth();
            Tcl_SetObjResult(interp, Tcl_NewIntObj(depth));
            break;
        }
        case FUNC_TYPE: {
            int type = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            type = mat->type();
            Tcl_SetObjResult(interp, Tcl_NewIntObj(type));
            break;
        }
        case FUNC_EMPTY: {
            bool result = false;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            result = mat->empty();
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

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count != mat->dims) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong list number");
            }

            index = (int *) attemptckalloc(sizeof(int) * mat->dims);
            if (!index) {
                return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc index failed");
            }

            for (int i = 0; i < count; i++) {
                Tcl_Obj *elemListPtr = NULL;
                int index_number;

                Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &index_number) != TCL_OK) {
                    ckfree(index);
                    return TCL_ERROR;
                }

                index[i] = index_number;
            }

            for (int i = 0; i < count; i++) {
                if (index[i] >= mat->size[i]) {
                    ckfree(index);
                    return Opencv_SetResult(interp, cv::Error::StsBadArg, "index out of range");
                }
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &channel) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }

            if (channel >= mat->channels()) {
                ckfree(index);
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong channel range");
            }

            switch (mat->type()) {
                case CV_8UC1: return_uchar_val = &mat->at<uchar>(index); break;
                case CV_8UC2: return_uchar_val = &mat->at<cv::Vec2b>(index)[channel]; break;
                case CV_8UC3: return_uchar_val = &mat->at<cv::Vec3b>(index)[channel]; break;
                case CV_8UC4: return_uchar_val = &mat->at<cv::Vec4b>(index)[channel]; break;

                case CV_8SC1: return_schar_val = &mat->at<schar>(index); break;
                case CV_8SC2: return_schar_val = &mat->at<cv::Vec<schar, 2>>(index)[channel]; break;
                case CV_8SC3: return_schar_val = &mat->at<cv::Vec<schar, 3>>(index)[channel]; break;
                case CV_8SC4: return_schar_val = &mat->at<cv::Vec<schar, 4>>(index)[channel]; break;

                case CV_16UC1: return_ushort_val = &mat->at<ushort>(index); break;
                case CV_16UC2: return_ushort_val = &mat->at<cv::Vec2w>(index)[channel]; break;
                case CV_16UC3: return_ushort_val = &mat->at<cv::Vec3w>(index)[channel]; break;
                case CV_16UC4: return_ushort_val = &mat->at<cv::Vec4w>(index)[channel]; break;

                case CV_16SC1: return_short_val = &mat->at<short>(index); break;
                case CV_16SC2: return_short_val = &mat->at<cv::Vec2s>(index)[channel]; break;
                case CV_16SC3: return_short_val = &mat->at<cv::Vec3s>(index)[channel]; break;
                case CV_16SC4: return_short_val = &mat->at<cv::Vec4s>(index)[channel]; break;

                case CV_32SC1: return_int_val = &mat->at<int>(index); break;
                case CV_32SC2: return_int_val = &mat->at<cv::Vec2i>(index)[channel]; break;
                case CV_32SC3: return_int_val = &mat->at<cv::Vec3i>(index)[channel]; break;
                case CV_32SC4: return_int_val = &mat->at<cv::Vec4i>(index)[channel]; break;
                case CV_32SC(6): return_int_val = &mat->at<cv::Vec6i>(index)[channel]; break;
                case CV_32SC(8): return_int_val = &mat->at<cv::Vec8i>(index)[channel]; break;

                case CV_32FC1: return_float_val = &mat->at<float>(index); break;
                case CV_32FC2: return_float_val = &mat->at<cv::Vec2f>(index)[channel]; break;
                case CV_32FC3: return_float_val = &mat->at<cv::Vec3f>(index)[channel]; break;
                case CV_32FC4: return_float_val = &mat->at<cv::Vec4f>(index)[channel]; break;
                case CV_32FC(6): return_float_val = &mat->at<cv::Vec6f>(index)[channel]; break;

                case CV_64FC1: return_double_val = &mat->at<double>(index); break;
                case CV_64FC2: return_double_val = &mat->at<cv::Vec2d>(index)[channel]; break;
                case CV_64FC3: return_double_val = &mat->at<cv::Vec3d>(index)[channel]; break;
                case CV_64FC4: return_double_val = &mat->at<cv::Vec4d>(index)[channel]; break;
                case CV_64FC(6): return_double_val = &mat->at<cv::Vec6d>(index)[channel]; break;

                default: {
                    ckfree(index);
                    return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid type");
                }
            }

            if (objc == 5) {
                if (Tcl_GetDoubleFromObj(interp, objv[4], &value) != TCL_OK) {
                    ckfree(index);
                    return TCL_ERROR;
                }

                switch (mat->depth()) {
                    case CV_8U: *return_uchar_val = cv::saturate_cast<uchar>(value); break;
                    case CV_8S: *return_schar_val = cv::saturate_cast<schar>(value); break;
                    case CV_16U: *return_ushort_val = cv::saturate_cast<ushort>(value); break;
                    case CV_16S: *return_short_val = cv::saturate_cast<short>(value); break;
                    case CV_32S: *return_int_val = cv::saturate_cast<int>(value); break;
                    case CV_32F: *return_float_val = cv::saturate_cast<float>(value); break;
                    case CV_64F: *return_double_val = cv::saturate_cast<double>(value); break;

                    default: {
                        ckfree(index);
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong matrix type");
                    }
                }
            } else {
                switch (mat->depth()) {
                    case CV_8U: long_val = *return_uchar_val; break;
                    case CV_8S: long_val = *return_schar_val; break;
                    case CV_16U: long_val = *return_ushort_val; break;
                    case CV_16S: long_val = *return_short_val; break;
                    case CV_32S: long_val = *return_int_val; break;
                    case CV_32F: double_val = *return_float_val; break;
                    case CV_64F: double_val = *return_double_val; break;

                    default: {
                        ckfree(index);
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong matrix type");
                    }
                }

                switch (mat->depth()) {
                    case CV_8U:
                    case CV_8S:
                    case CV_16U:
                    case CV_16S:
                    case CV_32S:
                        Tcl_SetObjResult(interp, Tcl_NewLongObj(long_val));
                        break;

                    case CV_32F:
                    case CV_64F:
                        Tcl_SetObjResult(interp, Tcl_NewDoubleObj(double_val));
                        break;

                    default: {
                        ckfree(index);
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong matrix type");
                    }
                }
            }

            ckfree(index);
            break;
        }
        case FUNC_INV: {
            int method = cv::DECOMP_LU;
            cv::Mat result_mat;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 2 && objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?method?");
                return TCL_ERROR;
            }

            if (objc == 3) {
                if (Tcl_GetIntFromObj(interp, objv[2], &method) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                 result_mat = mat->inv(method);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (result_mat.empty() || !result_mat.data) {
                return Opencv_SetResult(interp, cv::Error::StsError, "no data");
            }

            dstmat = new cv::Mat(result_mat);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_DOT: {
            cv::Mat *mat2;
            double result = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix");
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat2) {
                return TCL_ERROR;
            }

            try {
                result = mat->dot(*mat2);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
            break;
        }
        case FUNC_CROSS: {
            Tcl_Obj *pResultStr = NULL;
            cv::Mat result_mat;
            cv::Mat *mat2, *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix");
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);

            if (!mat2) {
                return TCL_ERROR;
            }

            try {
                /*
                 * Computes a cross-product of two 3-element vectors.
                 */
                result_mat = mat->cross(*mat2);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (result_mat.empty() || !result_mat.data) {
                return Opencv_SetResult(interp, cv::Error::StsError, "no cross product data");
            }

            dstmat = new cv::Mat(result_mat);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_MULTIPLICATION: {
            Tcl_Obj *pResultStr = NULL;
            cv::Mat result_mat;
            cv::Mat *mat2, *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix");
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat2) {
                return TCL_ERROR;
            }

            try {
                result_mat = (*mat) * (*mat2);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (result_mat.empty() || !result_mat.data) {
                return Opencv_SetResult(interp, cv::Error::StsError, "no data");
            }

            dstmat = new cv::Mat(result_mat);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_ADD: {
            double value = 0;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cv::Mat result_mat = (*mat) + value;

                if (result_mat.empty() ||  !result_mat.data) {
                    CV_Error(cv::Error::StsError, "no data");
                }

                dstmat = new cv::Mat(result_mat);

                pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_SUBTRACT: {
            double value = 0;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cv::Mat result_mat = (*mat) - value;

                if (result_mat.empty() || !result_mat.data) {
                    CV_Error(cv::Error::StsError, "no data");
                }

                dstmat = new cv::Mat(result_mat);

                pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_MULTIPLY: {
            double value = 0;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cv::Mat result_mat = (*mat) * value;

                if (result_mat.empty() || !result_mat.data) {
                    CV_Error(cv::Error::StsError, "no data");
                }

                dstmat = new cv::Mat(result_mat);

                pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_DIVIDE: {
            double value = 0;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cv::Mat result_mat = (*mat) / value;

                if (result_mat.empty() || !result_mat.data) {
                    CV_Error(cv::Error::StsError, "no data");
                }

                dstmat = new cv::Mat(result_mat);

                pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_TRANSPOSE: {
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                cv::Mat result_mat = mat->t();

                if (result_mat.empty() || !result_mat.data) {
                    CV_Error(cv::Error::StsError, "no data");
                    return TCL_ERROR;
                }

                dstmat = new cv::Mat(result_mat);

                pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_DIAG: {
            int d = 0;
            cv::Mat diag_image;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 2 && objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?d?");
                return TCL_ERROR;
            }

            if (objc == 3) {
                if (Tcl_GetIntFromObj(interp, objv[2], &d) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                diag_image = mat->diag(d);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(diag_image);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_CROP: {
            int x = 0, y= 0, width = 0, height = 0;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 6) {
                Tcl_WrongNumArgs(interp, 2, objv, "x y width height");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[4], &width) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[5], &height) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cv::Mat cropped_image = (*mat)(cv::Rect(x, y, width, height));

                if (cropped_image.empty() || !cropped_image.data) {
                    CV_Error(cv::Error::StsError, "no data");
                }

                dstmat = new cv::Mat(cropped_image);

                pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_RECT: {
            int x = 0, y= 0, width = 0, height = 0;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 6) {
                Tcl_WrongNumArgs(interp, 2, objv, "x y width height");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[4], &width) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[5], &height) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cv::Mat rect_image(*mat, cv::Rect(x, y, width, height));

                if (rect_image.empty() || !rect_image.data) {
                    CV_Error(cv::Error::StsError, "no data");
                }

                dstmat = new cv::Mat(rect_image);

                pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_SetObjResult(interp, pResultStr);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_COPYTO: {
            cv::Mat *mat1, *mat2 = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix ?mask?");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
                if (!mat2) {
                    return TCL_ERROR;
                }
            }

            try {
                if (objc == 3) {
                    mat->copyTo(*mat1);
                } else {
                    mat->copyTo(*mat1, *mat2);
                }
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CONVERTTO: {
            int type = 0;
            double scale = 1, shift = 0;
            cv::Mat imageChanged;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 3 && objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "type ?scale shift?");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &type) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 5) {
                if (Tcl_GetDoubleFromObj(interp, objv[3], &scale) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetDoubleFromObj(interp, objv[4], &shift) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                mat->convertTo(imageChanged, type, scale, shift);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (imageChanged.empty() || !imageChanged.data) {
                CV_Error(cv::Error::StsError, "no data");
            }

            dstmat = new cv::Mat(imageChanged);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_COLRANGE: {
            int startcol = 0, endcol = 0;
            cv::Mat imageChanged;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "startcol endcol");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &startcol) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &endcol) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                /*
                 * startcol: An inclusive 0-based start index of the column span.
                 * endcol: An exclusive 0-based ending index of the column span.
                 */
                imageChanged = mat->colRange(startcol, endcol);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (imageChanged.empty() || !imageChanged.data) {
                CV_Error(cv::Error::StsError, "no data");
            }

            dstmat = new cv::Mat(imageChanged);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_ROWRANGE: {
            int startcol = 0, endcol = 0;
            cv::Mat imageChanged;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "startcol endcol");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &startcol) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &endcol) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                /*
                 * startcol: An inclusive 0-based start index of the column span.
                 * endcol: An exclusive 0-based ending index of the column span.
                 */
                imageChanged = mat->rowRange(startcol, endcol);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (imageChanged.empty() || !imageChanged.data) {
                CV_Error(cv::Error::StsError, "no data");
            }

            dstmat = new cv::Mat(imageChanged);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_POP_BACK: {
            size_t nelems = 1;

            if (objc != 2 && objc != 3) {
                Tcl_WrongNumArgs(interp, 1, objv, "?nelems?");
                return TCL_ERROR;
            }

            if (objc == 3) {
                if (Tcl_GetLongFromObj(interp, objv[2], (long *) &nelems) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                mat->pop_back(nelems);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_PUSH_BACK: {
            cv::Mat *mat1;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 1, objv, "matrix");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            try {
                mat->push_back(*mat1);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_RESHAPE: {
            int cn = 0, rows = 0;
            cv::Mat dstimage;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "cn ?rows?");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &cn) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                if (Tcl_GetIntFromObj(interp, objv[3], &rows) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                dstimage = mat->reshape(cn, rows);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (dstimage.empty() || !dstimage.data) {
                return Opencv_SetResult(interp, cv::Error::StsError, "no data");
            }

            dstmat = new cv::Mat(dstimage);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_SETDATA: {
            int count = 0;
            int depth = mat->depth();
            long max_data_len = mat->total() * mat->channels();
            uchar *orig_data = mat->data;
            double val;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "list_data");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count > max_data_len) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "out of max range");
            }

            for (int i = 0; i < count; i++) {
                Tcl_Obj *elemListPtr = NULL;

                Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &val) != TCL_OK) {
                    return TCL_ERROR;
                }

                switch (depth) {
                    case CV_8U:   ((uchar *) orig_data)[i] = (uchar) val; break;
                    case CV_8S:   ((schar *) orig_data)[i] = (schar) val; break;
                    case CV_16U:  ((ushort *) orig_data)[i] = (ushort) val; break;
                    case CV_16S:  ((short *) orig_data)[i] = (short) val; break;
                    case CV_32S:  ((int *) orig_data)[i] = (int) val; break;
                    case CV_32F:  ((float *) orig_data)[i] = (float) val; break;
                    case CV_64F:  ((double *) orig_data)[i] = (double) val; break;
                }
            }
            break;
        }
        case FUNC_SETTO: {
            int B = 0, G = 0, R = 0, A = 0, count = 0;
            cv::Mat *mmat = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "color_list ?mask?");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count != 4) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
            } else {
                Tcl_Obj *elemListPtr = NULL;

                Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[2], 2, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[2], 3, &elemListPtr);
                if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            if (objc == 4) {
                mmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
                if (!mmat) {
                    return TCL_ERROR;
                }
            }

            try {
                cv::Scalar color(B, G, R, A);

                if (objc == 3) {
                    mat->setTo(color);
                } else {
                    mat->setTo(color, *mmat);
                }
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
#ifdef TCL_USE_TKPHOTO
        case FUNC_TOPHOTO: {
            char *name;
            Tk_PhotoHandle photo;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "photo");
                return TCL_ERROR;
            }

            if (Opencv_CheckForTk(cd, interp) != TCL_OK) {
                return TCL_ERROR;
            }

            name = Tcl_GetString(objv[2]);
            photo = Tk_FindPhoto(interp, name);
            if (photo == NULL) {
                Tcl_SetObjResult(interp, Tcl_ObjPrintf("can't use \"%s\": not a photo image", name));
                return TCL_ERROR;
            }
            if ((mat->type() == CV_8UC1 && mat->channels() == 1) ||
                (mat->type() == CV_8UC2 && mat->channels() == 2) ||
                (mat->type() == CV_8UC3 && mat->channels() == 3) ||
                (mat->type() == CV_8UC4 && mat->channels() == 4)) {
                Tk_PhotoImageBlock blk;
                unsigned char *data;
                cv::Mat tmat;

                if (!mat->isContinuous()) {
                    tmat = mat->clone();
                    mat = &tmat;
                }
                data = mat->data;
                blk.pixelPtr = data;
                blk.width = mat->cols;
                blk.height = mat->rows;
                blk.pitch = blk.width * mat->channels();
                blk.pixelSize = mat->channels();
                switch ((int) mat->type()) {
                    case CV_8UC1:     /* grey */
                    case CV_8UC2:     /* grey + alpha */
                        blk.offset[0] = 0;
                        blk.offset[1] = 0;
                        blk.offset[2] = 0;
                        blk.offset[3] = 1;
                        break;
                    case CV_8UC3:     /* BGR */
                    case CV_8UC4:     /* BGRA */
                        blk.offset[0] = 2;
                        blk.offset[1] = 1;
                        blk.offset[2] = 0;
                        blk.offset[3] = 3;
                        break;
                }
                if (Tk_PhotoSetSize(interp, photo, blk.width, blk.height) != TCL_OK) {
                    return TCL_ERROR;
                }
                if (Tk_PhotoPutBlock(interp, photo, &blk, 0, 0, blk.width, blk.height, TK_PHOTO_COMPOSITE_SET) != TCL_OK) {
                    return TCL_ERROR;
                }
            } else {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "incompatible matrix");
            }
            break;
        }
        case FUNC_FROMPHOTO: {
            char *name;
            Tk_PhotoHandle photo;
            Tk_PhotoImageBlock blk;
            unsigned char *data;
            int newType;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "photo");
                return TCL_ERROR;
            }

            if (Opencv_CheckForTk(cd, interp) != TCL_OK) {
                return TCL_ERROR;
            }

            name = Tcl_GetString(objv[2]);
            photo = Tk_FindPhoto(interp, name);
            if (photo == NULL) {
                Tcl_SetObjResult(interp, Tcl_ObjPrintf("can't use \"%s\": not a photo image", name));
                return TCL_ERROR;
            }
            if ((mat->type() == CV_8UC1 && mat->channels() == 1) ||
                (mat->type() == CV_8UC2 && mat->channels() == 2) ||
                (mat->type() == CV_8UC3 && mat->channels() == 3) ||
                (mat->type() == CV_8UC4 && mat->channels() == 4)) {
                newType = mat->type();
            } else {
                newType = CV_8UC4;
            }
            Tk_PhotoGetImage(photo, &blk);
            if (blk.width != mat->cols || blk.height != mat->rows || newType != mat->type()) {
                try {
                    mat->create(blk.height, blk.width, newType);
                } catch (const cv::Exception &ex) {
                    return Opencv_Exc2Tcl(interp, &ex);
                } catch (...) {
                    return Opencv_Exc2Tcl(interp, NULL);
                }
            }
            if (!mat->isContinuous()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "matrix not continuous");
            }
            data = mat->data;
            switch (mat->type()) {
                case CV_8UC1:     /* grey */
                    for (int y = 0; y < blk.height; y++) {
                        int off = y * blk.width;
                        unsigned char *pix = blk.pixelPtr + y * blk.pitch;
                        for (int x = 0; x < blk.width; x++) {
                            data[off++] =
                                (114 * pix[blk.offset[0]] +
                                 587 * pix[blk.offset[1]] +
                                 299 * pix[blk.offset[2]]) / 1000;
                            pix += blk.pixelSize;
                        }
                    }
                    break;
                case CV_8UC2:     /* grey + alpha */
                    for (int y = 0; y < blk.height; y++) {
                        int off = y * blk.width * 2;
                        unsigned char *pix = blk.pixelPtr + y * blk.pitch;
                        for (int x = 0; x < blk.width; x++) {
                            data[off++] =
                                (114 * pix[blk.offset[0]] +
                                 587 * pix[blk.offset[1]] +
                                 299 * pix[blk.offset[2]]) / 1000;
                            data[off++] = pix[blk.offset[3]];
                            pix += blk.pixelSize;
                        }
                    }
                    break;
                case CV_8UC3:     /* BGR */
                    for (int y = 0; y < blk.height; y++) {
                        int off = y * blk.width * 3;
                        unsigned char *pix = blk.pixelPtr + y * blk.pitch;
                        for (int x = 0; x < blk.width; x++) {
                            data[off++] = pix[blk.offset[2]];
                            data[off++] = pix[blk.offset[1]];
                            data[off++] = pix[blk.offset[0]];
                            pix += blk.pixelSize;
                        }
                    }
                    break;
                case CV_8UC4:     /* BGRA */
                    for (int y = 0; y < blk.height; y++) {
                        int off = y * blk.width * 4;
                        unsigned char *pix = blk.pixelPtr + y * blk.pitch;
                        for (int x = 0; x < blk.width; x++) {
                            data[off++] = pix[blk.offset[2]];
                            data[off++] = pix[blk.offset[1]];
                            data[off++] = pix[blk.offset[0]];
                            data[off++] = pix[blk.offset[3]];
                            pix += blk.pixelSize;
                        }
                    }
                    break;
            }
            break;
        }
#endif
#ifdef TCL_USE_VECTCL
        case FUNC_TONUMARRAY: {
            Tcl_Obj *naObj;
            NumArrayType natype = NumArray_NoType;
            int ndims = -1, elsize;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, NULL);
                return TCL_ERROR;
            }

            if (Opencv_CheckForVectcl(cd, interp) != TCL_OK) {
                return TCL_ERROR;
            }

            if (!mat->isContinuous()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "matrix not continuous");
            }
            ndims = mat->channels();
            switch (mat->type() & CV_MAT_DEPTH_MASK) {
                case CV_8U:
                    natype = NumArray_Uint8;
                    elsize = sizeof(unsigned char);
                    break;
                case CV_8S:
                    natype = NumArray_Int8;
                    elsize = sizeof(char);
                    break;
                case CV_16U:
                    natype = NumArray_Uint16;
                    elsize = sizeof(unsigned short);
                    break;
                case CV_16S:
                    natype = NumArray_Int16;
                    elsize = sizeof(short);
                    break;
                case CV_32S:
                    natype = NumArray_Int32;
                    elsize = sizeof(int);
                    break;
                case CV_32F:
                    natype = NumArray_Float32;
                    elsize = sizeof(float);
                    break;
                case CV_64F:
                    natype = NumArray_Float64;
                    elsize = sizeof(double);
                    break;
                default:
                    return Opencv_SetResult(interp, cv::Error::StsNoConv, "unsupported data type");
            }
            if (ndims == 1) {
                naObj = NumArrayNewMatrix(natype, mat->rows, mat->cols);
                void *p = NumArrayGetPtrFromObj(interp, naObj);
                memcpy(p, mat->data, mat->rows * mat->cols * elsize);
            } else {
                naObj = Tcl_NewObj();
                index_t dims[3];
                dims[0] = mat->rows;
                dims[1] = mat->cols;
                dims[2] = ndims;
                NumArrayInfo *info = CreateNumArrayInfo(3, dims, natype);
                NumArraySharedBuffer *shbuf = NumArrayNewSharedBuffer(info->bufsize);
                NumArraySetInternalRep(naObj, shbuf, info);
                void *p = NumArrayGetPtrFromObj(interp, naObj);
                memcpy(p, mat->data, mat->rows * mat->cols * ndims * elsize);
            }
            Tcl_SetObjResult(interp, naObj);
            break;
        }
        case FUNC_FROMNUMARRAY: {
            NumArrayInfo *info;
            int cvtype, dims[2];

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "numarray");
                return TCL_ERROR;
            }

            if (Opencv_CheckForVectcl(cd, interp) != TCL_OK) {
                return TCL_ERROR;
            }

            info = NumArrayGetInfoFromObj(interp, objv[2]);
            if (info == NULL) {
                return TCL_ERROR;
            }
            switch (info->type) {
                case NumArray_Int8: cvtype = CV_8S; break;
                case NumArray_Uint8: cvtype = CV_8U; break;
                case NumArray_Int16: cvtype = CV_16S; break;
                case NumArray_Uint16: cvtype = CV_16U; break;
                case NumArray_Int32: cvtype = CV_32S; break;
                case NumArray_Uint32: cvtype = CV_32S; break;
                case NumArray_Float32: cvtype = CV_32F; break;
                case NumArray_Float64: cvtype = CV_64F; break;
                case NumArray_Int:
                    if (sizeof(NaWideInt) == sizeof(int)) {
                        cvtype = CV_32S;
                        break;
                    }
                default:
                    return Opencv_SetResult(interp, cv::Error::StsNoConv, "unsupported data type");
            }
            if (info->nDim < 1 || info->nDim > 3) {
                return Opencv_SetResult(interp, cv::Error::StsNoConv, "invalid number dimensions");
            }
            dims[0] = info->dims[0];
            if (info->nDim > 1) {
                dims[1] = info->dims[1];
            } else {
                dims[1] = 1;
            }
            if (info->nDim > 2) {
                cvtype = CV_MAKE_TYPE(cvtype, info->dims[2]);
            } else {
                cvtype = CV_MAKE_TYPE(cvtype, 1);
            }
            try {
                *mat = cv::Mat(2, dims, cvtype, NumArrayGetPtrFromObj(interp, objv[2]));
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }
            break;
        }
#endif
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_DeleteCommandFromToken(interp, cvo->cmd);
            Tcl_SetObjResult(interp, Tcl_NewIntObj(0));

            break;
        }
        case FUNC__COMMAND: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__NAME: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(cvo->key, -1));
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::Mat", TCL_STATIC);
            break;
        }
    }
    return TCL_OK;
}


int tcl_CV_8UC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int num = 0, result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_8UC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int tcl_CV_8SC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int num = 0, result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_8SC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int tcl_CV_16UC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int num = 0, result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_16UC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int tcl_CV_16SC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int num = 0, result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_16SC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int tcl_CV_32SC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int num = 0, result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_32SC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int tcl_CV_32FC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int num = 0, result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_32FC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int tcl_CV_64FC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int num = 0, result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "num");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &num) != TCL_OK) {
        return TCL_ERROR;
    }

    result = CV_64FC(num);
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int mat_mat(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int rows, cols, type;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    cv::Mat mat_mat;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "rows cols type ?color_list?");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &rows) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &cols) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
        }

        if (count != 4) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[4], 0, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 2, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 3, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
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

    dstmat = new cv::Mat(mat_mat);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_matwithdims(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int dims, *index, type;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    cv::Mat mat_mat;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "dims size_list type ?color_list?");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &dims) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != dims) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong list number");
    }

    index = (int *) attemptckalloc(sizeof(int) * dims);
    if (!index) {
        return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc index failed");
    }

    for (int i = 0; i < count; i++) {
        Tcl_Obj *elemListPtr = NULL;
        int index_number;

        Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &index_number) != TCL_OK) {
            ckfree(index);
            return TCL_ERROR;
        }

        index[i] = index_number;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        ckfree(index);
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
            ckfree(index);
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
        }

        if (count != 4) {
            ckfree(index);
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[4], 0, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 2, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
                ckfree(index);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], 3, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
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

    dstmat = new cv::Mat(mat_mat);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    ckfree(index);
    return TCL_OK;
}


int mat_diag(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat diag_image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    /*
     * One-dimensional matrix that represents the main diagonal.
     */
    diag_image = cv::Mat::diag(*mat);

    dstmat = new cv::Mat(diag_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_eye(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int rows, cols, type;
    cv::Mat eye_mat;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "rows cols type");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &rows) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &cols) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    eye_mat = cv::Mat::eye(rows, cols, type);

    dstmat = new cv::Mat(eye_mat);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_ones(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int rows, cols, type;
    cv::Mat ones_mat;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "rows cols type");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &rows) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &cols) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    ones_mat = cv::Mat::ones(rows, cols, type);

    dstmat = new cv::Mat(ones_mat);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_zeros(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int rows, cols, type;
    cv::Mat zeros_mat;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "rows cols type");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &rows) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &cols) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    zeros_mat = cv::Mat::zeros(rows, cols, type);

    dstmat = new cv::Mat(zeros_mat);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_abs(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *dstmat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    try {
        image = cv::abs(*mat1);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_absdiff(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    try {
        cv::absdiff(*mat1, *mat2, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_add(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    try {
        cv::add(*mat1, *mat2, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_addWeighted(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    double alpha = 0.0, beta = 0.0, gamma = 0.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 alpha matrix_2 beta gamma");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &beta) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &gamma) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::addWeighted(*mat1, alpha, *mat2, beta, gamma, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_bitwise_and(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *mat3, *dstmat;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?mask?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        mat3 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
        if (!mat3) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 3) {
            cv::bitwise_and(*mat1, *mat2, image);
        } else {
            cv::bitwise_and(*mat1, *mat2, image, *mat3);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_bitwise_or(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *mat3, *dstmat;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?mask?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        mat3 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
        if (!mat3) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 3) {
            cv::bitwise_or(*mat1, *mat2, image);
        } else {
            cv::bitwise_or(*mat1, *mat2, image, *mat3);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_bitwise_xor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *mat3, *dstmat;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?mask?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        mat3 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
        if (!mat3) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 3) {
            cv::bitwise_xor(*mat1, *mat2, image);
        } else {
            cv::bitwise_xor(*mat1, *mat2, image, *mat3);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_bitwise_not(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dstimage;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?mask?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (objc == 3) {
        mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
        if (!mat2) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 2) {
            cv::bitwise_not(*mat1, dstimage);
        } else {
            cv::bitwise_not(*mat1, dstimage, *mat2);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_calcCovarMatrix(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int flags = 0, ctype = CV_64F;
    cv::Mat covar, mean;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
    cv::Mat *mat1, *matrix_covar, *matrix_mean;

    if  (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix flags ?ctype?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[3], &ctype) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::calcCovarMatrix(*mat1, covar, mean, flags, ctype);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    matrix_covar = new cv::Mat(covar);
    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, matrix_covar);

    matrix_mean = new cv::Mat(mean);
    pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, matrix_mean);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_cartToPolar(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int angleInDegrees = 0;
    cv::Mat magnitude, angle;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
    cv::Mat *mat1, *mat2, *magnitudemat, *anglemat;

    if  (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?angleInDegrees?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetBooleanFromObj(interp, objv[3], &angleInDegrees) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::cartToPolar(*mat1, *mat2, magnitude, angle, angleInDegrees);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    magnitudemat = new cv::Mat(magnitude);

    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, magnitudemat);

    anglemat = new cv::Mat(angle);

    pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, anglemat);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_compare(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int cmpop = 0;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 cmpop");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &cmpop) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::compare(*mat1, *mat2, image, cmpop);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_convertScaleAbs(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double alpha = 1, beta = 0;
    cv::Mat dstimage;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix alpha beta");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &beta) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::convertScaleAbs(*mat, dstimage, alpha, beta);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_copyMakeBorder(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int top = 0, bottom = 0, left = 0, right = 0, borderType = cv::BORDER_DEFAULT;
    int B = 0, G = 0, R = 0, A = 0;
    int count = 0;
    cv::Mat dstimage;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 7 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix top bottom left right borderType ?color_list?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &top) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &bottom) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &left) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &right) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if (Tcl_ListObjLength(interp, objv[7], &count) != TCL_OK) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
        }

        if (count != 4) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[7], 0, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 2, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 3, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
                return TCL_ERROR;
            }
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        if (borderType == cv::BORDER_CONSTANT) {
            cv::copyMakeBorder(*mat, dstimage, top, bottom, left, right, borderType, color);
        } else {
            cv::copyMakeBorder(*mat, dstimage, top, bottom, left, right, borderType);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_countNonZero(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (mat->channels() != 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "single channel array required");
    }

    try {
        result = cv::countNonZero(*mat);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int mat_determinant(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    double result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        result = cv::determinant(*mat);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int mat_divide(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double scale = 1.0;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?scale?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetDoubleFromObj(interp, objv[3], &scale) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::divide(*mat1, *mat2, image, scale);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_eigen(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    bool retval;
    cv::Mat image1, image2;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
    cv::Mat *mat, *dstmat1, *dstmat2;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        /*
         * Calculates eigenvalues and eigenvectors of a symmetric matrix.
         */
        retval = cv::eigen(*mat, image1, image2);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat1 = new cv::Mat(image1);
    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

    dstmat2 = new cv::Mat(image2);
    pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewBooleanObj((int) retval));
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int mat_eigenNonSymmetric(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image1, image2;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
    cv::Mat *mat, *dstmat1, *dstmat2;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        cv::eigenNonSymmetric(*mat, image1, image2);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat1 = new cv::Mat(image1);
    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

    dstmat2 = new cv::Mat(image2);
    pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif


int mat_exp(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        cv::exp(*mat, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_extractChannel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int coi = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix coi");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &coi) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        /*
         * coi is 0-based index.
         */
        cv::extractChannel(*mat, image, coi);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_flip(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int flipCode = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix flipCode");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    /*
     * 0 means flipping around the x-axis and positive value (for example, 1)
     * means flipping around y-axis. Negative value (for example, -1) means
     * flipping around both axes.
     */
    if (Tcl_GetIntFromObj(interp, objv[2], &flipCode) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::flip(*mat, image, flipCode);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_getOptimalDFTSize(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int vecsize = 0;
    int result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "vecsize");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &vecsize) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        result = cv::getOptimalDFTSize(vecsize);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int mat_dft(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int flags = 0, nonzeroRows = 0;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?falgs nonzeroRows?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &nonzeroRows) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::dft(*mat, image, flags, nonzeroRows);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_inRange(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int count = 0;
    int B1 = 0, G1 = 0, R1 = 0, A1 = 0;
    int B2 = 0, G2 = 0, R2 = 0, A2 = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix color_list1 color_list2");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A1) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A2) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::inRange(*mat, cv::Scalar(B1, G1, R1, A1),
                    cv::Scalar(B2, G2, R2, A2), image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_insertChannel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int coi = 0;
    cv::Mat *mat, *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix dst_matrix coi");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    dstmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!dstmat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &coi) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        /*
         * Inserts a single channel to dst (coi is 0-based index).
         * coi is index of channel for insertion.
         */
        cv::insertChannel(*mat, *dstmat, coi);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int mat_log(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        cv::log(*mat, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_lut(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix1 matrix2");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    try {
        cv::LUT(*mat1, *mat2, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_Mahalanobis(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat1, *mat2, *icovar;
    double result = 0;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix1 matrix2 icovar_matrix");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    icovar = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!icovar) {
        return TCL_ERROR;
    }

    try {
        result = cv::Mahalanobis(*mat1, *mat2, *icovar);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int mat_magnitude(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix1 matrix2");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    try {
        cv::magnitude(*mat1, *mat2, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_meanStdDev(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat mean, stddev;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
    cv::Mat *mat, *dstmat1, *dstmat2;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        cv::meanStdDev(*mat, mean, stddev);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat1 = new cv::Mat(mean);

    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

    dstmat2 = new cv::Mat(stddev);

    pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_minMaxIdx(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Tcl_Obj *pResultStr = NULL, *pSubResultStr = NULL;
    double minVal, maxVal;
    int minIdx, maxIdx;
    cv::Mat *mat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        cv::minMaxIdx(*mat, &minVal, &maxVal, &minIdx, &maxIdx);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    pSubResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewDoubleObj(minVal));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj(minIdx));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubResultStr);

    pSubResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewDoubleObj(maxVal));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj(maxIdx));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubResultStr);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_minMaxLoc(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Tcl_Obj *pResultStr = NULL, *pSubResultStr = NULL;
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::Mat *mat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        cv::minMaxLoc(*mat, &minVal, &maxVal, &minLoc, &maxLoc);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    pSubResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewDoubleObj(minVal));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj(minLoc.x));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj(minLoc.y));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubResultStr);

    pSubResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewDoubleObj(maxVal));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj(maxLoc.x));
    Tcl_ListObjAppendElement(NULL, pSubResultStr, Tcl_NewIntObj(maxLoc.y));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubResultStr);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_split(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Tcl_Obj *listPtr;
    cv::Mat *mat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    std::vector<cv::Mat> channels;
    try {
        cv::split(*mat, channels);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    if (channels.size() == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid channel size");
    }

    listPtr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < channels.size(); i++) {
        Tcl_Obj *pStr = NULL;
        cv::Mat *dstmat = NULL;

        dstmat = new cv::Mat(channels[i]);

        pStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

        Tcl_ListObjAppendElement(NULL, listPtr, pStr);
    }

    Tcl_SetObjResult(interp, listPtr);
    return TCL_OK;
}


int mat_multiply(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double scale = 1.0;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?scale?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetDoubleFromObj(interp, objv[3], &scale) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::multiply(*mat1, *mat2, image, scale);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_mulSpectrums(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int flags = 0, conjB = 0;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 flags ?conjB?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetBooleanFromObj(interp, objv[4], &conjB) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::mulSpectrums(*mat1, *mat2, image, flags, conjB);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_merge(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Mat> channels;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_list");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty list");
    }

    for (int i = 0; i < count; i++) {
        cv::Mat *mat;
        Tcl_Obj *elem = NULL;

        Tcl_ListObjIndex(interp, objv[1], i, &elem);
        mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, elem);
        if (!mat) {
            return TCL_ERROR;
        }

        channels.push_back(*mat);
    }

    try {
        cv::merge(channels, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_norm(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int norm_type = cv::NORM_L2;
    double result;
    cv::Mat *mat, *mat2 = NULL;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix norm_type");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &norm_type) != TCL_OK) {
            return TCL_ERROR;
        }
    } else {
        mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
        if (!mat2) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &norm_type) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 3) {
            result = cv::norm(*mat, norm_type);
        } else {
            result = cv::norm(*mat, *mat2, norm_type);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int mat_normalize(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double alpha = 0, beta = 0;
    int norm_type = cv::NORM_L2;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix alpha beta norm_type");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &beta) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &norm_type) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::normalize(*mat, image, alpha, beta, norm_type);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_pow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double power = 0;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix power");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &power) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::pow(*mat, power, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_randn(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double mean_B = 0, mean_G = 0, mean_R = 0, mean_A = 0;
    double stddev_B = 0, stddev_G = 0, stddev_R = 0, stddev_A = 0;
    int count = 0;
    cv::Mat *mat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix mean_list stddev_list");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid mean data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &mean_B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &mean_G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 2, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &mean_R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 3, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &mean_A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid stddev data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &stddev_B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &stddev_G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 2, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &stddev_R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 3, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &stddev_A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        /*
         * Fills the array with normally distributed random numbers.
         */
        cv::randn(*mat, cv::Scalar(mean_B, mean_G, mean_R, mean_A),
                  cv::Scalar(stddev_B, stddev_G, stddev_R, stddev_A));
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int mat_randu(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double min_B = 0, min_G = 0, min_R = 0, min_A = 0;
    double max_B = 0, max_G = 0, max_R = 0, max_A = 0;
    int count = 0;
    cv::Mat *mat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix min_list max_list");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid min data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &min_B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &min_G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 2, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &min_R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 3, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &min_A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid max data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &max_B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &max_G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 2, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &max_R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 3, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &max_A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        /*
         * Generates a single uniformly-distributed random number or an array of random numbers.
         */
        cv::randu(*mat, cv::Scalar(min_B, min_G, min_R, min_A),
                  cv::Scalar(max_B, max_G, max_R, max_A));
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int mat_randShuffle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double iterFactor = 1;
    cv::Mat *mat;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?iterFactor?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetDoubleFromObj(interp, objv[2], &iterFactor) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        /*
         * The function cv::randShuffle shuffles the specified 1D array
         * by randomly choosing pairs of elements and swapping them.
         */
        cv::randShuffle(*mat, iterFactor);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int mat_reduce(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int dim = 0, rtype = 0, dtype = -1;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix dim rtype ?dtype?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &dim) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &rtype) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * When negative, the output vector will have the same type as the input
     * matrix. In case of REDUCE_MAX and REDUCE_MIN, the output image should
     * have the same type as the source one. In case of REDUCE_SUM and
     * REDUCE_AVG, the output may have a larger element bit-depth to preserve
     * accuracy.
     */
    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[4], &dtype) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::reduce(*mat, image, dim, rtype, dtype);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_rotate(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int rotateCode = 0;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "rotateCode");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &rotateCode) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::rotate(*mat, image, rotateCode);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_setRNGSeed(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int seed = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "seed");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &seed) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::setRNGSeed(seed);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int mat_solve(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL;
    cv::Mat *mat1, *mat2, *dstmat;
    bool result;
    int flags = cv::DECOMP_LU;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?flags?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        result = cv::solve(*mat1, *mat2, image, flags);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat = new cv::Mat(image);
    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewBooleanObj((int) result));
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_solveCubic(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL;
    cv::Mat *mat1, *dstmat;
    int result;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    try {
        result = cv::solveCubic(*mat1, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat = new cv::Mat(image);
    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(result));
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_sqrt(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        cv::sqrt(*mat, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_subtract(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    try {
        cv::subtract(*mat1, *mat2, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_sum(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Scalar result;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        result = cv::sum(*mat);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result[0]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result[1]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result[2]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result[3]));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_SVBackSubst(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat result;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *mat3, *mat4, *dstmat;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_w matrix_u matrix_vt matrix");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    mat3 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!mat3) {
        return TCL_ERROR;
    }

    mat4 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[4]);
    if (!mat4) {
        return TCL_ERROR;
    }

    try {
        cv::SVBackSubst(*mat1, *mat2, *mat3, *mat4, result);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(result);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_SVDecomp(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat w, v, ut;
    Tcl_Obj *pResultStr = NULL, *pMatResultStr = NULL;
    cv::Mat *mat1, *dstmat1, *dstmat2, *dstmat3;
    int flags = 0;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?flags?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::SVDecomp(*mat1, w, v, ut, flags);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat1 = new cv::Mat(w);
    pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);

    dstmat2 = new cv::Mat(v);
    pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);
    Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);

    dstmat3 = new cv::Mat(ut);
    pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat3);
    Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_trace(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Scalar result;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    /*
     * Returns the sum of the diagonal elements of the matrix.
     */
    try {
        result = cv::trace(*mat);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result[0]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result[1]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result[2]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result[3]));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_transform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    try {
        cv::transform(*mat1, image, *mat2);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_hconcat(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    try {
        cv::hconcat(*mat1, *mat2, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int mat_vconcat(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat2) {
        return TCL_ERROR;
    }

    try {
        cv::vconcat(*mat1, *mat2, image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int kmeans(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image, bestLabels;
    Tcl_Obj *pResultStr = NULL, *pDstResultStr = NULL, *pLabelResultStr = NULL;
    cv::Mat *matrix = NULL, *labelsmst = NULL, *dstmat = NULL;
    cv::TermCriteria *termCriteria;
    int k = 0, attempts = 0, flags = 0;
    double value = 0;
    int nolabelmatrix = 0;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix k bestLabels termCriteria attempts flags");
        return TCL_ERROR;
    }

    matrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!matrix) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &k) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * If users give the handle to "None", then setup a nolabelmatrix flag to
     * indicate this thing. This flag is used to decide the argument.
     */
    if (strcmp(Tcl_GetString(objv[3]), "None") == 0) {
        nolabelmatrix = 1;
    } else {
        nolabelmatrix = 0;

        labelsmst = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
        if (!labelsmst) {
            return TCL_ERROR;
        }
    }

    termCriteria = (cv::TermCriteria *) Opencv_FindHandle(cd, interp, OPENCV_TERMCRITERIA, objv[4]);
    if (!termCriteria) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &attempts) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        if (nolabelmatrix == 0 && (flags & cv::KMEANS_USE_INITIAL_LABELS) == 1) {
            bestLabels = *labelsmst;
            value = cv::kmeans(*matrix, k, bestLabels, *termCriteria, attempts, flags, image);
        } else if (nolabelmatrix == 1 && (flags & cv::KMEANS_USE_INITIAL_LABELS) == 0) {
            value = cv::kmeans(*matrix, k, bestLabels, *termCriteria, attempts, flags, image);
        } else {
            CV_Error(cv::Error::StsBadArg, "invalid argument");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat = new cv::Mat(image);
    pDstResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    if (nolabelmatrix == 1) {
        labelsmst = new cv::Mat(bestLabels);
        pLabelResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, labelsmst);
    } else {
        pLabelResultStr = objv[3];
    }

    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(value));
    Tcl_ListObjAppendElement(NULL, pResultStr, pLabelResultStr);
    Tcl_ListObjAppendElement(NULL, pResultStr, pDstResultStr);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int perspectiveTransform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int count = 0, npts = 0;
    cv::Mat mat_image;
    std::vector<cv::Point2f> src_points, dst_points;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_list transformation_matrix");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        double number_from_list_x;
        double number_from_list_y;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            cv::Point2f point;
            point.x = (float) number_from_list_x;
            point.y = (float) number_from_list_y;
            src_points.push_back(point);
        }
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        cv::perspectiveTransform(src_points, dst_points, *mat);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < dst_points.size(); i++) {
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) dst_points[i].x));
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) dst_points[i].y));
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int getBuildInformation(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::String result;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        result = cv::getBuildInformation();
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewStringObj(result.c_str(), -1));
    return TCL_OK;
}


int getTickCount(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double result;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        result = (double) cv::getTickCount();
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int getTickFrequency(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double result;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        result = cv::getTickFrequency();
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int PCA_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::PCA *pca;

    static const char *FUNC_strs[] = {
        "mean",
        "eigenvalues",
        "eigenvectors",
        "backProject",
        "project",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_mean,
        FUNC_eigenvalues,
        FUNC_eigenvectors,
        FUNC_backProject,
        FUNC_project,
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    cd = (void *) cvo->top;
    pca = (cv::PCA *) cvo->obj;
    if (!pca) {
        Tcl_Panic("null PCA object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_mean: {
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(pca->mean);
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_eigenvalues: {
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(pca->eigenvalues);
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_eigenvectors: {
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(pca->eigenvectors);
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_backProject: {
            Tcl_Obj *pResultStr = NULL;
            cv::Mat image;
            cv::Mat *mat, *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                pca->backProject(*mat, image);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(image);
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_project: {
            Tcl_Obj *pResultStr = NULL;
            cv::Mat image;
            cv::Mat *mat, *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                pca->project(*mat, image);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(image);
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_DeleteCommandFromToken(interp, cvo->cmd);

            break;
        }
        case FUNC__COMMAND: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__NAME: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(cvo->key, -1));
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::PCA", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int PCA(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int flags = 0, maxComponents = 0;
    cv::PCA local_pca;
    cv::PCA *pca;
    cv::Mat *mat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 2, objv, "matrix flags ?maxComponents?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[3], &maxComponents) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        local_pca = cv::PCA(*mat, cv::Mat(), flags, maxComponents);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pca = new cv::PCA(local_pca);
    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_PCA, pca);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int TERM_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::TermCriteria *termCriteria;

    static const char *FUNC_strs[] = {
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_CLOSE,
        FUNC__COMMAND,
        FUNC__NAME,
        FUNC__TYPE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    cd = (void *) cvo->top;
    termCriteria = (cv::TermCriteria *) cvo->obj;
    if (!termCriteria) {
        Tcl_Panic("null TermCriteria object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_DeleteCommandFromToken(interp, cvo->cmd);

            break;
        }
        case FUNC__COMMAND: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__NAME: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(cvo->key, -1));
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::TermCriteria", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int TermCriteria(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int type = 1, maxCount = 1;
    double epsilon = 1;
    cv::TermCriteria term_crit, *termCriteria;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?type maxCount epsilon?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[1], &type) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &maxCount) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &epsilon) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 1) {
            term_crit = cv::TermCriteria();
        } else {
            term_crit = cv::TermCriteria(type, maxCount, epsilon);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    termCriteria = new cv::TermCriteria(term_crit);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_TERMCRITERIA, termCriteria);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
