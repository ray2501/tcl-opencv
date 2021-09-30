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
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        image = cv::imread(filename, flags);
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    if (image.empty() || !image.data) {
        return Opencv_SetResult(interp, cv::Error::StsError, "no image data");
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
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid data");
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
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    if (image.empty() || !image.data) {
        return Opencv_SetResult(interp, cv::Error::StsError, "no image data");
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
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat) {
        return TCL_ERROR;
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        result = cv::imwrite(filename, *mat);
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    if (!result) {
        return Opencv_SetResult(interp, cv::Error::StsError, "image data not saved");
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
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    if (!result) {
        return Opencv_SetResult(interp, cv::Error::StsError, "image data not processed");
    }
    if (buf.size()) {
        Tcl_SetObjResult(interp, Tcl_NewByteArrayObj(buf.data(), buf.size()));
    }

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
