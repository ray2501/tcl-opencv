#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int namedWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    int len = 0;
    int flags = cv::WINDOW_AUTOSIZE;
    Tcl_DString ds;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname ?flags?");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "namedWindow invalid winname", TCL_STATIC);
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        cv::namedWindow(winname, flags);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "namedWindow failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    return TCL_OK;
}


int imshow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    int len = 0;
    cv::Mat *mat;
    Tcl_DString ds;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname matrix");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "imshow invalid winname", TCL_STATIC);
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat) {
        return TCL_ERROR;
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        cv::imshow(winname, *mat);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "imshow failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);
    return TCL_OK;
}


int waitKey(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int delay = 0;
    int result = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "delay");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &delay) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        result = cv::waitKey(delay);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "waitKey failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int moveWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    int len = 0, x = 0, y = 0;
    Tcl_DString ds;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname x y");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "moveWindow Iinvalid winname", TCL_STATIC);
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        cv::moveWindow(winname, x, y);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "moveWindow failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);
    return TCL_OK;
}


int resizeWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    int len = 0, width = 0, height = 0;
    Tcl_DString ds;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname width height");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "resizeWindow Iinvalid winname", TCL_STATIC);
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Only windows created without cv::WINDOW_AUTOSIZE flag can be resized.
     */
    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        cv::resizeWindow(winname, width, height);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "resizeWindow failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);
    return TCL_OK;
}


int destroyWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    int len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "destroyWindow invalid winname", TCL_STATIC);
        return TCL_ERROR;
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        cv::destroyWindow(winname);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "destroyWindow failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);
    return TCL_OK;
}


int destroyAllWindows(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 0, objv, "destroyAllWindows");
        return TCL_ERROR;
    }

    try {
        cv::destroyAllWindows();
    } catch (...) {
        Tcl_SetResult(interp, (char *) "destroyAllWindows failed", TCL_STATIC);
        return TCL_ERROR;
    }
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4

int selectROI(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int showCrosshair = 1, fromCenter = 0;
    cv::Mat *mat;
    Tcl_Obj *pResultStr = NULL;
    cv::Rect rect;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?showCrosshair fromCenter?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetBooleanFromObj(interp, objv[2], &showCrosshair) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[3], &fromCenter) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        rect = cv::selectROI(*mat, showCrosshair, fromCenter);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "selectROI failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.x));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.y));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.width));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.height));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}

#endif /* TCL_USE_OPENCV4 */


void CallBackFunc(int event, int x, int y, int flags, void *userdata)
{
    if (userdata) {
        CvCallbackInfo *callbackinfo = (CvCallbackInfo *) userdata;
        Tcl_Obj *script_obj[5];
        int i = 0;

        script_obj[0] = Tcl_NewStringObj(callbackinfo->callback_code, -1);
        script_obj[1] = Tcl_NewIntObj(event);
        script_obj[2] = Tcl_NewIntObj(x);
        script_obj[3] = Tcl_NewIntObj(y);
        script_obj[4] = Tcl_NewIntObj(flags);

        for (i = 0; i < 5; i++) {
            Tcl_IncrRefCount(script_obj[i]);
        }

        Tcl_EvalObjv(callbackinfo->interp, 5, script_obj, 0);

        for (i = 0; i < 5; i++) {
            Tcl_DecrRefCount(script_obj[i]);
        }
    }
}


int setMouseCallback(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL, *callback_code = NULL;
    int len = 0, len2 = 0;
    CvCallbackInfo *callbackinfo;
    Tcl_DString ds;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname callback_code");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "setMouseCallback invalid winname", TCL_STATIC);
        return TCL_ERROR;
    }

    callback_code = Tcl_GetStringFromObj(objv[2], &len2);
    if (len2 < 1) {
        Tcl_SetResult(interp, (char *) "setMouseCallback invalid callback_code", TCL_STATIC);
        return TCL_ERROR;
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        callbackinfo = (CvCallbackInfo *) ckalloc(sizeof (CvCallbackInfo));
        callbackinfo->interp = interp;
        callbackinfo->callback_code = callback_code;
        callbackinfo->value = 0;
        cv::setMouseCallback(winname, CallBackFunc, callbackinfo);
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "setMouseCallback failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    Opencv_NewHandle(cd, interp, OPENCV_CALLBACK, callbackinfo);

    return TCL_OK;
}


void TrackbarCallback(int value, void *userdata)
{
    if (userdata) {
        CvCallbackInfo *callbackinfo = (CvCallbackInfo *) userdata;
        Tcl_Obj *script_obj[2];
        int i = 0;

        script_obj[0] = Tcl_NewStringObj(callbackinfo->callback_code, -1);
        script_obj[1] = Tcl_NewIntObj(value);

        for (i = 0; i < 2; i++) {
            Tcl_IncrRefCount(script_obj[i]);
        }

        Tcl_EvalObjv(callbackinfo->interp, 2, script_obj, 0);

        for (i = 0; i < 2; i++) {
            Tcl_DecrRefCount(script_obj[i]);
        }
    }
}


int createTrackbar(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *trackbarname = NULL, *winname = NULL, *callback_code = NULL;
    int init_value = 0, max_value = 0;
    int len = 0, tlen = 0, wlen = 0;
    CvCallbackInfo *callbackinfo;
    Tcl_DString ds1, ds2;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "trackbarname winname init_value max_value callback_code");
        return TCL_ERROR;
    }
    trackbarname = Tcl_GetStringFromObj(objv[1], &tlen);
    if (tlen < 1) {
        Tcl_SetResult(interp, (char *) "createTrackbar invalid trackbarname", TCL_STATIC);
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[2], &wlen);
    if (wlen < 1) {
        Tcl_SetResult(interp, (char *) "createTrackbar invalid winname", TCL_STATIC);
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &init_value) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &max_value) != TCL_OK) {
        return TCL_ERROR;
    }

    callback_code = Tcl_GetStringFromObj(objv[5], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "createTrackbar invalid callback_code", TCL_STATIC);
        return TCL_ERROR;
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, wlen, &ds1);
    trackbarname = Tcl_UtfToExternalDString(NULL, trackbarname, tlen, &ds2);
    try {
        callbackinfo = (CvCallbackInfo *) ckalloc(sizeof (CvCallbackInfo));
        callbackinfo->interp = interp;
        callbackinfo->callback_code = callback_code;
        callbackinfo->value = init_value;

        cv::createTrackbar(trackbarname, winname, &(callbackinfo->value),
                           max_value, TrackbarCallback, callbackinfo);
    } catch (...) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        Tcl_SetResult(interp, (char *) "createTrackbar failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);

    Opencv_NewHandle(cd, interp, OPENCV_CALLBACK, callbackinfo);

    return TCL_OK;
}


int getTrackbarPos(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *trackbarname = NULL, *winname = NULL;
    int tlen = 0, wlen = 0;
    int value;
    Tcl_DString ds1, ds2;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "trackbarname winname");
        return TCL_ERROR;
    }
    trackbarname = Tcl_GetStringFromObj(objv[1], &tlen);
    if (tlen < 1) {
        Tcl_SetResult(interp, (char *) "getTrackbarPos invalid trackbarname", TCL_STATIC);
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[2], &wlen);
    if (wlen < 1) {
        Tcl_SetResult(interp, (char *) "getTrackbarPos invalid winname", TCL_STATIC);
        return TCL_ERROR;
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, wlen, &ds1);
    trackbarname = Tcl_UtfToExternalDString(NULL, trackbarname, tlen, &ds2);
    try {
        value = cv::getTrackbarPos(trackbarname, winname);
    } catch (...) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        Tcl_SetResult(interp, (char *) "getTrackbarPos failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);

    Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
