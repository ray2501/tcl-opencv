#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TCL_USE_OPENCV4

int QRCodeDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::QRCodeDetector *qrdet;

    static const char *FUNC_strs[] = {
        "detect",
        "detectAndDecode",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_DETECTANDDECODE,
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

    cd = cvo->top;
    qrdet = (cv::QRCodeDetector *) cvo->obj;
    if (!qrdet) {
        Tcl_Panic("null QRCodeDetector object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *srcmat, *dstmat;
            cv::Mat points_matrix;
            bool result;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            srcmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!srcmat) {
                return TCL_ERROR;
            }

            try {
                result = qrdet->detect(*srcmat, points_matrix);
                if (!result) {
                    Tcl_SetResult(interp, (char *) "detect result failed", TCL_STATIC);
                    return TCL_ERROR;
                }
            } catch (...) {
                Tcl_SetResult(interp, (char *) "detect failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(points_matrix);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_DETECTANDDECODE: {
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
            cv::Mat *srcmat, *dstmat, *codemat;
            cv::Mat points_matrix, code_matrix;
            std::string result;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            srcmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!srcmat) {
                return TCL_ERROR;
            }

            try {
                result = qrdet->detectAndDecode(*srcmat,
                                                points_matrix,
                                                code_matrix);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "detectAndDecode failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(points_matrix);

            pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            codemat = new cv::Mat(code_matrix);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, codemat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr,
                                     Tcl_NewStringObj(result.c_str(), result.length()));

            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

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

            Tcl_SetResult(interp, (char *) "cv::QRCodeDetector", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int QRCodeDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::QRCodeDetector *qrdet;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        qrdet = new cv::QRCodeDetector();
    } catch (...) {
        Tcl_SetResult(interp, (char *) "QRCodeDetector failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_QDETECT, qrdet);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}

#endif /* TCL_USE_OPENCV4 */


int CascadeClassifier_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::CascadeClassifier *cas;

    static const char *FUNC_strs[] = {
        "detectMultiScale",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_DETECTMULTISCALE,
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

    cd = cvo->top;
    cas = (cv::CascadeClassifier *) cvo->obj;
    if (!cas) {
        Tcl_Panic("null CascadeClassifier object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECTMULTISCALE: {
            double scaleFactor = 1.1;
            int minNeighbors = 3, minWidth = 0, minHeight = 0, maxWidth = 0, maxHeight = 0;
            std::vector< cv::Rect > rect;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *mat;

            if (objc != 3 && objc != 9) {
                Tcl_WrongNumArgs(interp, 2, objv,
                        "matrix ?scaleFactor minNeighbors minWidth minHeight maxWidth maxHeight?");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (objc == 9) {
                if (Tcl_GetDoubleFromObj(interp, objv[3], &scaleFactor) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[4], &minNeighbors) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[5], &minWidth) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[6], &minHeight) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[7], &maxWidth) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[8], &maxHeight) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cas->detectMultiScale(*mat,
                                      rect, scaleFactor, minNeighbors, 0,
                                      cv::Size(minWidth, minHeight),
                                      cv::Size(maxWidth, maxHeight));
            } catch (...) {
                Tcl_SetResult(interp, (char *) "detectMultiScale failed", TCL_STATIC);
                return TCL_ERROR;
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < rect.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].width));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].height));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

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

            Tcl_SetResult(interp, (char *) "cv::CascadeClassifier", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int CascadeClassifier(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *filename = NULL;
    int len = 0;
    cv::CascadeClassifier *cas;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (!filename || len < 1) {
        Tcl_SetResult(interp, (char *) "CascadeClassifier: invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    try {
        cas = new cv::CascadeClassifier(filename);

        if (cas->empty() == true) {
            Tcl_SetResult(interp, (char *) "CascadeClassifier load file failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "CascadeClassifier failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_ODETECT, cas);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
