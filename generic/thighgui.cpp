#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int namedWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    Tcl_Size len = 0;
    int flags = cv::WINDOW_AUTOSIZE;
    Tcl_DString ds;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname ?flags?");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        cv::namedWindow(winname, flags);
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    return TCL_OK;
}


int imshow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    Tcl_Size len = 0;
    cv::Mat *mat;
    Tcl_DString ds;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname matrix");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mat) {
        return TCL_ERROR;
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        cv::imshow(winname, *mat);
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
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
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_SetObjResult(interp, Tcl_NewIntObj(result));
    return TCL_OK;
}


int moveWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    Tcl_Size len = 0;
    int x = 0, y = 0;
    Tcl_DString ds;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname x y");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
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
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);
    return TCL_OK;
}


int resizeWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL;
    Tcl_Size len = 0;
    int width = 0, height = 0;
    Tcl_DString ds;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname width height");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
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
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);
    return TCL_OK;
}


int setWindowTitle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *winname = NULL, *title = NULL;
    Tcl_Size len1 = 0, len2 = 0;
    Tcl_DString ds1, ds2;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname title");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len1);
    if (len1 < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
    }

    title = Tcl_GetStringFromObj(objv[2], &len2);
    if (len2 < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid title");
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len1, &ds1);
    title = Tcl_UtfToExternalDString(NULL, title, len2, &ds2);
    try {
        cv::setWindowTitle(winname, title);
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);

    return TCL_OK;
}


int destroyWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    char *winname = NULL;
    Tcl_Size len = 0;
    Tcl_DString ds;
    Tcl_HashEntry *hashEntryPtr;
    Tcl_HashSearch search;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);
    try {
        cv::destroyWindow(winname);
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }

    for (hashEntryPtr = Tcl_FirstHashEntry(&cvd->tbl[OPENCV_CALLBACK], &search);
         hashEntryPtr != NULL; hashEntryPtr = Tcl_NextHashEntry(&search)) {
        Opencv_Obj *cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);
        CvCallbackInfo *callbackinfo = (CvCallbackInfo *) cvo->obj;

        if (strcmp(Tcl_DStringValue(&callbackinfo->winname), winname) == 0) {
            cvo->top = NULL;
            cvo->key = NULL;
            cvo->cmd = NULL;
            Tcl_DStringFree(&callbackinfo->winname);
            if (callbackinfo->istb) {
                Tcl_DStringFree(&callbackinfo->tbname);
            }
            Tcl_DecrRefCount(callbackinfo->callback_code);
            ckfree(callbackinfo);
            ckfree(cvo);
            Tcl_DeleteHashEntry(hashEntryPtr);
        }
    }

    Tcl_DStringFree(&ds);

    return TCL_OK;
}


int destroyAllWindows(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_HashEntry *hashEntryPtr;
    Tcl_HashSearch search;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 0, objv, "destroyAllWindows");
        return TCL_ERROR;
    }

    try {
        cv::destroyAllWindows();
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    for (hashEntryPtr = Tcl_FirstHashEntry(&cvd->tbl[OPENCV_CALLBACK], &search);
         hashEntryPtr != NULL; hashEntryPtr = Tcl_NextHashEntry(&search)) {
        Opencv_Obj *cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);
        CvCallbackInfo *callbackinfo = (CvCallbackInfo *) cvo->obj;

        cvo->top = NULL;
        cvo->key = NULL;
        cvo->cmd = NULL;
        Tcl_DStringFree(&callbackinfo->winname);
        if (callbackinfo->istb) {
            Tcl_DStringFree(&callbackinfo->tbname);
        }
        Tcl_DecrRefCount(callbackinfo->callback_code);
        ckfree(callbackinfo);
        ckfree(cvo);
        Tcl_DeleteHashEntry(hashEntryPtr);
    }

    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4

int selectROI(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int showCrosshair = 1, fromCenter = 0;
    cv::Mat *mat;
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
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_Obj *list[4];

    list[0] = Tcl_NewIntObj((int) rect.x);
    list[1] = Tcl_NewIntObj((int) rect.y);
    list[2] = Tcl_NewIntObj((int) rect.width);
    list[3] = Tcl_NewIntObj((int) rect.height);

    Tcl_SetObjResult(interp, Tcl_NewListObj(4, list));
    return TCL_OK;
}

#endif /* TCL_USE_OPENCV4 */


void CallBackFunc(int event, int x, int y, int flags, void *userdata)
{
    if (userdata) {
        CvCallbackInfo *callbackinfo = (CvCallbackInfo *) userdata;
        Tcl_Obj *script_obj[5];
        int i = 0;

        script_obj[0] = callbackinfo->callback_code;
        script_obj[1] = Tcl_NewIntObj(event);
        script_obj[2] = Tcl_NewIntObj(x);
        script_obj[3] = Tcl_NewIntObj(y);
        script_obj[4] = Tcl_NewIntObj(flags);

        for (i = 0; i < 5; i++) {
            Tcl_IncrRefCount(script_obj[i]);
        }

        Tcl_EvalObjv(callbackinfo->interp, 5, script_obj, TCL_EVAL_GLOBAL);

        for (i = 0; i < 5; i++) {
            Tcl_DecrRefCount(script_obj[i]);
        }
    }
}


int setMouseCallback(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    char *winname = NULL;
    Tcl_Size len = 0, len2 = 0;
    int isNew = 1;
    CvCallbackInfo *callbackinfo;
    Tcl_DString ds;
    Tcl_HashEntry *hashEntryPtr;
    Tcl_HashSearch search;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "winname callback_code");
        return TCL_ERROR;
    }

    winname = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
    }

    Tcl_GetStringFromObj(objv[2], &len2);
    if (len2 < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid callback");
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, len, &ds);

    for (hashEntryPtr = Tcl_FirstHashEntry(&cvd->tbl[OPENCV_CALLBACK], &search);
         hashEntryPtr != NULL; hashEntryPtr = Tcl_NextHashEntry(&search)) {
        Opencv_Obj *cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);

        callbackinfo = (CvCallbackInfo *) cvo->obj;
        if (callbackinfo->istb) {
            continue;
        }
        if (strcmp(Tcl_DStringValue(&callbackinfo->winname), winname) == 0) {
            isNew = 0;
            break;
        }
    }

    if (isNew) {
        callbackinfo = NULL;
        try {
            callbackinfo = (CvCallbackInfo *) ckalloc(sizeof (CvCallbackInfo));
            callbackinfo->istb = 0;
            callbackinfo->interp = interp;
            Tcl_DStringInit(&callbackinfo->winname);
            Tcl_DStringAppend(&callbackinfo->winname, winname, -1);
            callbackinfo->callback_code = objv[2];
            callbackinfo->value = 0;
            cv::setMouseCallback(winname, CallBackFunc, callbackinfo);
        } catch (const cv::Exception &ex) {
            if (callbackinfo != NULL) {
                Tcl_DStringFree(&callbackinfo->winname);
                ckfree(callbackinfo);
            }
            Tcl_DStringFree(&ds);
            return Opencv_Exc2Tcl(interp, &ex);
        } catch (...) {
            if (callbackinfo != NULL) {
                Tcl_DStringFree(&callbackinfo->winname);
                ckfree(callbackinfo);
            }
            Tcl_DStringFree(&ds);
            return Opencv_Exc2Tcl(interp, NULL);
        }
    }
    Tcl_DStringFree(&ds);

    if (isNew) {
        Opencv_NewHandle(cd, interp, OPENCV_CALLBACK, callbackinfo);
    } else {
        Tcl_DecrRefCount(callbackinfo->callback_code);
        callbackinfo->callback_code = objv[2];
        callbackinfo->value = 0;
    }
    Tcl_IncrRefCount(callbackinfo->callback_code);

    return TCL_OK;
}


void TrackbarCallback(int value, void *userdata)
{
    if (userdata) {
        CvCallbackInfo *callbackinfo = (CvCallbackInfo *) userdata;

        if (callbackinfo->callback_code) {
            Tcl_Obj *script_obj[2];
            int i = 0;

            script_obj[0] = callbackinfo->callback_code;
            script_obj[1] = Tcl_NewIntObj(value);

            for (i = 0; i < 2; i++) {
                Tcl_IncrRefCount(script_obj[i]);
            }

            Tcl_EvalObjv(callbackinfo->interp, 2, script_obj, TCL_EVAL_GLOBAL);

            for (i = 0; i < 2; i++) {
                Tcl_DecrRefCount(script_obj[i]);
            }
        }
    }
}


int createTrackbar(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    char *trackbarname = NULL, *winname = NULL;
    int init_value = 0, max_value = 0;
    Tcl_Size len = 0, tlen = 0, wlen = 0;
    int isNew = 1;
    CvCallbackInfo *callbackinfo = NULL;
    Tcl_DString ds1, ds2;
    Tcl_HashEntry *hashEntryPtr;
    Tcl_HashSearch search;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "trackbarname winname init_value max_value callback_code");
        return TCL_ERROR;
    }
    trackbarname = Tcl_GetStringFromObj(objv[1], &tlen);
    if (tlen < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid trackbar name");
    }

    winname = Tcl_GetStringFromObj(objv[2], &wlen);
    if (wlen < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &init_value) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &max_value) != TCL_OK) {
        return TCL_ERROR;
    }

    Tcl_GetStringFromObj(objv[5], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid callback");
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, wlen, &ds1);
    trackbarname = Tcl_UtfToExternalDString(NULL, trackbarname, tlen, &ds2);

    for (hashEntryPtr = Tcl_FirstHashEntry(&cvd->tbl[OPENCV_CALLBACK], &search);
         hashEntryPtr != NULL; hashEntryPtr = Tcl_NextHashEntry(&search)) {
        Opencv_Obj *cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);

        callbackinfo = (CvCallbackInfo *) cvo->obj;
        if (!callbackinfo->istb) {
            continue;
        }
        if (strcmp(Tcl_DStringValue(&callbackinfo->winname), winname) == 0 &&
            strcmp(Tcl_DStringValue(&callbackinfo->tbname), trackbarname) == 0) {
            isNew = 0;
            break;
        }
    }

    if (isNew) {
        try {
            callbackinfo = (CvCallbackInfo *) ckalloc(sizeof (CvCallbackInfo));
            callbackinfo->istb = 1;
            callbackinfo->interp = interp;
            Tcl_DStringInit(&callbackinfo->winname);
            Tcl_DStringAppend(&callbackinfo->winname, winname, -1);
            Tcl_DStringInit(&callbackinfo->tbname);
            Tcl_DStringAppend(&callbackinfo->tbname, trackbarname, -1);
            callbackinfo->callback_code = objv[5];
            callbackinfo->value = init_value;
            cv::createTrackbar(trackbarname, winname, &(callbackinfo->value),
                               max_value, TrackbarCallback, callbackinfo);
        } catch (const cv::Exception &ex) {
            if (callbackinfo != NULL) {
                Tcl_DStringFree(&callbackinfo->winname);
                Tcl_DStringFree(&callbackinfo->tbname);
                ckfree(callbackinfo);
            }
            Tcl_DStringFree(&ds1);
            Tcl_DStringFree(&ds2);
            return Opencv_Exc2Tcl(interp, &ex);
        } catch (...) {
            if (callbackinfo != NULL) {
                Tcl_DStringFree(&callbackinfo->winname);
                Tcl_DStringFree(&callbackinfo->tbname);
                ckfree(callbackinfo);
            }
            Tcl_DStringFree(&ds1);
            Tcl_DStringFree(&ds2);
            return Opencv_Exc2Tcl(interp, NULL);
        }
    }

    if (isNew) {
        Opencv_NewHandle(cd, interp, OPENCV_CALLBACK, callbackinfo);
    } else {
        Tcl_DecrRefCount(callbackinfo->callback_code);
        callbackinfo->callback_code = NULL;
        try {
            cv::setTrackbarMax(trackbarname, winname, max_value);
            cv::setTrackbarPos(trackbarname, winname, init_value);
        } catch (...) {
        }
        callbackinfo->callback_code = objv[5];
        callbackinfo->value = init_value;
    }
    Tcl_IncrRefCount(callbackinfo->callback_code);

    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);

    return TCL_OK;
}


int getTrackbarPos(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *trackbarname = NULL, *winname = NULL;
    Tcl_Size tlen = 0, wlen = 0;
    int value;
    Tcl_DString ds1, ds2;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "trackbarname winname");
        return TCL_ERROR;
    }
    trackbarname = Tcl_GetStringFromObj(objv[1], &tlen);
    if (tlen < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid trackbar name");
    }

    winname = Tcl_GetStringFromObj(objv[2], &wlen);
    if (wlen < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid window name");
    }

    winname = Tcl_UtfToExternalDString(NULL, winname, wlen, &ds1);
    trackbarname = Tcl_UtfToExternalDString(NULL, trackbarname, tlen, &ds2);
    try {
        value = cv::getTrackbarPos(trackbarname, winname);
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);

    Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
