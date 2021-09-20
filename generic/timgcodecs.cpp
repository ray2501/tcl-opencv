#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int imread(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *filename = NULL;
    int len = 0;
    int flags = cv::IMREAD_COLOR;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat;
    Tcl_DString ds;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename ?flags?");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "imread invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        image = cv::imread(filename, flags);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "imread failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    if (image.empty() || !image.data) {
        Tcl_SetResult(interp, (char *) "imread no image data", TCL_STATIC);
        return TCL_ERROR;
    }

    mat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, mat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int imdecode(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    unsigned char *data;
    int len = 0;
    int flags = cv::IMREAD_COLOR;
    cv::Mat image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "bytes ?flags?");
        return TCL_ERROR;
    }

    data = Tcl_GetByteArrayFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "imdecode invalid data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        std::vector<unsigned char> v;
        v.insert(v.end(), data, data + len);
        image = cv::imdecode(v, flags);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "imdecode failed", TCL_STATIC);
        return TCL_ERROR;
    }

    if (image.empty() || !image.data) {
        Tcl_SetResult(interp, (char *) "imdecode no image data", TCL_STATIC);
        return TCL_ERROR;
    }

    mat = new cv::Mat(image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, mat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int imwrite(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *filename = NULL;
    int len = 0;
    cv::Mat *mat;
    bool result = false;
    Tcl_DString ds;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename matrix");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "imwrite invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat) {
        return TCL_ERROR;
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        result = cv::imwrite(filename, *mat);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "imwrite failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    if (!result) {
        Tcl_SetResult(interp, (char *) "imwrite can't save file", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}

int imencode(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *fileext = NULL;
    int len = 0;
    cv::Mat *mat;
    bool result = false;
    Tcl_DString ds;
    std::vector<unsigned char> buf;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "fileext matrix");
        return TCL_ERROR;
    }

    fileext = Tcl_GetStringFromObj(objv[1], &len);

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat) {
        return TCL_ERROR;
    }

    fileext = Tcl_UtfToExternalDString(NULL, fileext, len, &ds);
    try {
        result = cv::imencode(fileext, *mat, buf);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "imencode failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    if (!result) {
        Tcl_SetResult(interp, (char *) "imencode can't process data", TCL_STATIC);
        return TCL_ERROR;
    }
    if (buf.size()) {
        Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(buf.data(), buf.size()));
    }

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
