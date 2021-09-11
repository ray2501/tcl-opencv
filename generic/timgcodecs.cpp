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

    if (objc !=2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename ?flags?");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (!filename || len < 1) {
        Tcl_SetResult(interp, (char *) "imread: invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
        image = cv::imread(filename, flags);
        Tcl_DStringFree(&ds);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "imread failed", TCL_STATIC);
        return TCL_ERROR;
    }

    if (image.empty() ||  !image.data) {
        Tcl_SetResult(interp, (char *) "imread: no image data", TCL_STATIC);
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
    if (!filename || len < 1) {
        Tcl_SetResult(interp, (char *) "imwrite invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat) {
        return TCL_ERROR;
    }

    try {
        filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
        result = cv::imwrite(filename, *mat);
        Tcl_DStringFree(&ds);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "imwrite failed", TCL_STATIC);
        return TCL_ERROR;
    }

    if (!result) {
        Tcl_SetResult(interp, (char *) "imwrite ERROR: Can't save file.", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
