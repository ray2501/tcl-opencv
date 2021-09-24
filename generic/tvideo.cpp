#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif


static void SubtractorMOG2_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->bgsmog2) {
        cvd->bgsmog2.release();
    }
    cvd->cmd_bgsmog2 = NULL;
}


static int SubtractorMOG2_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "apply",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_APPLY,
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

    if (cvd->bgsmog2 == nullptr) {
        Tcl_SetResult(interp, (char *) "singleton not instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_APPLY: {
            cv::Mat result_image;
            Tcl_Obj *pResultStr = NULL;
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
                cvd->bgsmog2->apply(*mat, result_image);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "apply failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(result_image);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_bgsmog2) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_bgsmog2);
            }

            break;
        }
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_bgsmog2) {
                Tcl_GetCommandFullName(interp, cvd->cmd_bgsmog2, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::BackgroundSubtractorMOG2", TCL_STATIC);
            break;
        }

    }

    return TCL_OK;
}


int BackgroundSubtractorMOG2(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int history = 0, detectShadows = 1;
    double varThreshold = 16;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::BackgroundSubtractorMOG2> bgsmog2;

    if (objc > 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?history varThreshold detectShadows?");
        return TCL_ERROR;
    }

    if (objc > 1 && Tcl_GetIntFromObj(interp, objv[1], &history) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc > 2 && Tcl_GetDoubleFromObj(interp, objv[2], &varThreshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc > 3 && Tcl_GetBooleanFromObj(interp, objv[3], &detectShadows) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        bgsmog2 = cv::createBackgroundSubtractorMOG2(history, varThreshold, (bool) detectShadows);

        if (bgsmog2 == nullptr) {
            Tcl_SetResult(interp, (char *) "BackgroundSubtractorMOG2 create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "BackgroundSubtractorMOG2 failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-subtractorMOG2", -1);

    if (cvd->cmd_bgsmog2) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_bgsmog2);
    }
    cvd->cmd_bgsmog2 =
        Tcl_CreateObjCommand(interp, "::cv-subtractorMOG2",
            (Tcl_ObjCmdProc *) SubtractorMOG2_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) SubtractorMOG2_DESTRUCTOR);

    cvd->bgsmog2 = bgsmog2;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int meanShift(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int retval = 0;
    int x = 0, y = 0, width = 0, height = 0;
    cv::Rect window;
    cv::Mat *mat1;
    cv::TermCriteria *termCriteria;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix x y width height termCriteria");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
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

    termCriteria = (cv::TermCriteria *) Opencv_FindHandle(cd, interp, OPENCV_TERMCRITERIA, objv[6]);
    if (!termCriteria) {
        return TCL_ERROR;
    }

    try {
        window = cv::Rect(x, y, width, height);
        retval = cv::meanShift(*mat1, window, *termCriteria);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "meanShift failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(retval));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(window.x));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(window.y));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(window.width));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(window.height));

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int CamShift(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int x = 0, y = 0, width = 0, height = 0;
    cv::Rect window;
    cv::RotatedRect rect_result;
    cv::Point2f points[4];
    cv::Mat *mat1;
    cv::TermCriteria *termCriteria;
    Tcl_Obj *pResultStr = NULL;
    Tcl_Obj *pSubListStr1 = NULL;
    Tcl_Obj *pSubListStr2 = NULL;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix x y width height termCriteria");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
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

    termCriteria = (cv::TermCriteria *) Opencv_FindHandle(cd, interp, OPENCV_TERMCRITERIA, objv[6]);
    if (!termCriteria) {
        return TCL_ERROR;
    }

    try {
        window = cv::Rect(x, y, width, height);
        rect_result = cv::CamShift(*mat1, window, *termCriteria);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "CamShift failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    pSubListStr1  = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubListStr1, Tcl_NewIntObj(window.x));
    Tcl_ListObjAppendElement(NULL, pSubListStr1, Tcl_NewIntObj(window.y));
    Tcl_ListObjAppendElement(NULL, pSubListStr1, Tcl_NewIntObj(window.width));
    Tcl_ListObjAppendElement(NULL, pSubListStr1, Tcl_NewIntObj(window.height));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubListStr1);
    rect_result.points(points);

    pSubListStr2  = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj((int) points[0].x));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj((int) points[0].y));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj((int) points[1].x));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj((int) points[1].y));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj((int) points[2].x));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj((int) points[2].y));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj((int) points[3].x));
    Tcl_ListObjAppendElement(NULL, pSubListStr2, Tcl_NewIntObj((int) points[3].y));

    Tcl_ListObjAppendElement(NULL, pResultStr, pSubListStr2);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int calcOpticalFlowPyrLK(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int width = 0, height = 0, maxLevel = 3;
    cv::Mat nextPts, status, err;
    cv::Mat *mat1, *mat2, *mat3;
    cv::Mat *dstmat, *statusmat, *errmat;
    cv::TermCriteria *termCriteria;
    Tcl_Obj *pResultStr = NULL;
    Tcl_Obj *pResultStr1 = NULL, *pResultStr2 = NULL, *pResultStr3 = NULL;

    if (objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "prevImg nextImg prevPts winSize_width winSize_height maxLevel termCriteria");
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

    if (Tcl_GetIntFromObj(interp, objv[4], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &maxLevel) != TCL_OK) {
        return TCL_ERROR;
    }

    termCriteria = (cv::TermCriteria *) Opencv_FindHandle(cd, interp, OPENCV_TERMCRITERIA, objv[7]);
    if (!termCriteria) {
        return TCL_ERROR;
    }

    try {
        cv::calcOpticalFlowPyrLK(*mat1, *mat2, *mat3,
                                 nextPts, status, err, cv::Size(width, height), maxLevel,
                                 *termCriteria);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowPyrLK failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat = new cv::Mat(nextPts);

    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    statusmat = new cv::Mat(status);

    pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, statusmat);

    errmat = new cv::Mat(err);

    pResultStr3 = Opencv_NewHandle(cd, interp, OPENCV_MAT, errmat);

    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr3);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int calcOpticalFlowFarneback(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double pyr_scale = 0, poly_sigma = 0;
    int levels = 0, winsize = 0, iterations = 0, poly_n = 0, flags = 0;
    cv::Mat flow;
    cv::Mat *mat1, *mat2, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "prevImg nextImg pyr_scale levels winsize iterations poly_n poly_sigma flags");
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

    if (Tcl_GetDoubleFromObj(interp, objv[3], &pyr_scale) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &levels) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &winsize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &iterations) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[7], &poly_n) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[8], &poly_sigma) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[9], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::calcOpticalFlowFarneback(*mat1, *mat2,
                                     flow, pyr_scale, levels, winsize,
                                     iterations, poly_n, poly_sigma, flags);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "calcOpticalFlowFarneback failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstmat = new cv::Mat(flow);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
