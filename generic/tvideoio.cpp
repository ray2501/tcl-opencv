#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int VideoWriter_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::VideoWriter *writer;

    static const char *FUNC_strs[] = {
        "isOpened",
        "write",
        "get",
        "set",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_ISOPENED,
        FUNC_WRITE,
        FUNC_GET,
        FUNC_SET,
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
    writer = (cv::VideoWriter *) cvo->obj;
    if (!writer) {
        Tcl_Panic("null VideoWrite object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_ISOPENED: {
            bool result;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                result = writer->isOpened();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
            }
            break;
        }
        case FUNC_WRITE: {
            cv::Mat *mat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                writer->write(*mat);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_GET: {
            int propId;
            double result;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "propId");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &propId) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                result = writer->get(propId);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
            break;
        }
        case FUNC_SET: {
            int propId;
            double value;
            bool result;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "propId value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &propId) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[3], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                result = writer->set(propId, value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
            }
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Opencv_CloseHandle(interp, cvo);

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

            Tcl_SetResult(interp, (char *) "cv::VideoWriter", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int VideoWriter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *fourcc = NULL;
    char *filename = NULL;
    int len, len4;
    double fps = 0, width = 0, height = 0;
    int fourccvalue = 0, isColor = 1;
    cv::VideoWriter *writer;
    Tcl_DString ds;

    if (objc != 6 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename fourcc fps width height ?isColor?");
        return TCL_ERROR;
    }


    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    fourcc = Tcl_GetStringFromObj(objv[2], &len4);
    if (len4 != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid fourcc");
        return TCL_ERROR;
    }

    fourccvalue = cv::VideoWriter::fourcc(fourcc[0], fourcc[1], fourcc[2], fourcc[3]);

    if (Tcl_GetDoubleFromObj(interp, objv[3], &fps) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if (Tcl_GetBooleanFromObj(interp, objv[6], &isColor) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        writer = new cv::VideoWriter(filename, fourccvalue, fps,
                                     cv::Size (width,  height), (bool) isColor);
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    return Opencv_NewHandleResult(cd, interp, OPENCV_VIDEOWRITER, writer);
}


int VideoCapture_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::VideoCapture *capture;

    static const char *FUNC_strs[] = {
        "isOpened",
        "read",
        "get",
        "set",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_ISOPENED,
        FUNC_READ,
        FUNC_GET,
        FUNC_SET,
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
    capture = (cv::VideoCapture *) cvo->obj;
    if (!capture) {
        Tcl_Panic("null VideoCapture object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_ISOPENED: {
            bool result;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                result = capture->isOpened();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
            }
            break;
        }
        case FUNC_READ: {
            cv::Mat frame;
            cv::Mat *mat;
            bool isSuccess;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                isSuccess = capture->read(frame);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (!isSuccess) {
                return Opencv_SetResult(interp, cv::Error::StsError, "read failed");
            }

            mat = new cv::Mat(frame);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, mat);
            break;
        }
        case FUNC_GET: {
            int propId;
            double result;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "propId");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &propId) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                result = capture->get(propId);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
            break;
        }
        case FUNC_SET: {
            int propId;
            double value;
            bool result;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "propId value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &propId) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[3], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                result = capture->set(propId, value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            if (result) {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(1));
            } else {
                Tcl_SetObjResult(interp, Tcl_NewBooleanObj(0));
            }
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Opencv_CloseHandle(interp, cvo);

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

            Tcl_SetResult(interp, (char *) "cv::VideoCapture", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int VideoCapture(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *opentype = NULL;
    char *filename = NULL;
    int index = 0;
    int len = 0;
    int type = 1;
    int flags = cv::CAP_ANY;
    cv::VideoCapture *capture;
    Tcl_DString ds;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "file/index filename/number ?flags?");
        return TCL_ERROR;
    }

    opentype = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid open type");
    }

    if (strcmp(opentype, "file") == 0) {
        filename = Tcl_GetStringFromObj(objv[2], &len);
        if (len < 1) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
        }

        type = 1;
    } else if (strcmp(opentype, "index") == 0) {
        if (Tcl_GetIntFromObj(interp, objv[2], &index) != TCL_OK) {
            return TCL_ERROR;
        }

        type = 2;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (type == 1) {
        filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    } else {
        Tcl_DStringInit(&ds);
    }
    try {
        if (type == 1) {
            capture = new cv::VideoCapture(filename, flags);
        } else {
#ifdef TCL_USE_OPENCV4
            capture = new cv::VideoCapture(index, flags);
#else
            capture = new cv::VideoCapture(index);
#endif
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    return Opencv_NewHandleResult(cd, interp, OPENCV_VIDEOCAPTURE, capture);
}
#ifdef __cplusplus
}
#endif
