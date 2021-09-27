#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

int findChessboardCorners(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int patternSize_width = 0, patternSize_height = 0;
    int flags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;
    bool retval;
    cv::Mat result;
    cv::Mat *mat1, *dstmat;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "image patternSize_width patternSize_height ?flags?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &patternSize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &patternSize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[4], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        retval = cv::findChessboardCorners(*mat1,
                                           cv::Size(patternSize_width, patternSize_height),
                                           result, flags);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "findChessboardCorners failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat = new cv::Mat(result);
    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
    Tcl_SetObjResult(interp, pResultStr1);

    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewBooleanObj((int) retval));
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int drawChessboardCorners(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int patternSize_width = 0, patternSize_height = 0, patternWasFound = 0;
    cv::Mat result;
    cv::Mat *mat1, *corners;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "image patternSize_width patternSize_height corners patternWasFound");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &patternSize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &patternSize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    corners = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[4]);
    if (!corners) {
        return TCL_ERROR;
    }

    if (Tcl_GetBooleanFromObj(interp, objv[5], &patternWasFound) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::drawChessboardCorners(*mat1,
                                  cv::Size(patternSize_width, patternSize_height),
                                  *corners, (bool) patternWasFound);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "drawChessboardCorners failed", TCL_STATIC);
        return TCL_ERROR;
    }

    return TCL_OK;
}


int calibrateCamera(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int width = 0, height = 0, count = 0;
    std::vector<cv::Mat> objectPoints, imagePoints, rvecs, tvecs;
    cv::Mat *cameraMatrix, *distCoeffs;
    Tcl_Obj *pResultStr = NULL, *pMatResultStr = NULL, *pListResultStr = NULL;
    double ret = 0;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "objectPoints_list imagePoints_list width height cameraMatrix distCoeffs");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "calibrateCamera invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count <= 0) {
        Tcl_SetResult(interp, (char *) "calibrateCamera list data is empty", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        cv::Mat *mat;

        for (int index = 0; index < count; index++) {
            Tcl_ListObjIndex(interp, objv[1], index, &elemListPtr);
            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, elemListPtr);
            if (!mat) {
                return TCL_ERROR;
            }

            objectPoints.push_back(*mat);
        }
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        Tcl_SetResult(interp, (char *) "calibrateCamera invalid list data", TCL_STATIC);
        return TCL_ERROR;
    }

    if (count <= 0) {
        Tcl_SetResult(interp, (char *) "calibrateCamera list data is empty", TCL_STATIC);
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        cv::Mat *mat;

        for (int index = 0; index < count; index++) {
            Tcl_ListObjIndex(interp, objv[2], index, &elemListPtr);
            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, elemListPtr);
            if (!mat) {
                return TCL_ERROR;
            }

            imagePoints.push_back(*mat);
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    cameraMatrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[5]);
    if (!cameraMatrix) {
        return TCL_ERROR;
    }

    distCoeffs = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[6]);
    if (!distCoeffs) {
        return TCL_ERROR;
    }

    try {
        ret = cv::calibrateCamera(objectPoints, imagePoints,
                                  cv::Size(width, height),
                                  *cameraMatrix, *distCoeffs, rvecs, tvecs);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "calibrateCamera failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(ret));

    pMatResultStr = objv[5];
    Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);

    pMatResultStr = objv[6];
    Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);

    pListResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < rvecs.size(); i++) {
        cv::Mat *dstmat;
        Tcl_Obj *pSubResultStr = NULL;

        dstmat = new cv::Mat(rvecs[i]);

        pSubResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
        Tcl_ListObjAppendElement(NULL, pListResultStr, pSubResultStr);
    }
    Tcl_ListObjAppendElement(NULL, pResultStr, pListResultStr);

    pListResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < tvecs.size(); i++) {
        cv::Mat *dstmat;
        Tcl_Obj *pSubResultStr = NULL;

        dstmat = new cv::Mat(tvecs[i]);

        pSubResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
        Tcl_ListObjAppendElement(NULL, pListResultStr, pSubResultStr);
    }
    Tcl_ListObjAppendElement(NULL, pResultStr, pListResultStr);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int getOptimalNewCameraMatrix(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int width = 0, height = 0, nwidth = 0, nheight = 0, centerPrincipalPoint = 0;
    double alpha = 0;
    cv::Mat result_matrix;
    cv::Mat *cameraMatrix, *distCoeffs, *dstmat;
    cv::Rect validPixROI;
    Tcl_Obj *pResultStr = NULL, *pMatResultStr = NULL;

    if (objc != 8 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "cameraMatrix distCoeffs width height alpha nwidth nheight ?centerPrincipalPoint?");
        return TCL_ERROR;
    }

    cameraMatrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!cameraMatrix) {
        return TCL_ERROR;
    }

    distCoeffs = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!distCoeffs) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &nwidth) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[7], &nheight) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if (Tcl_GetBooleanFromObj(interp, objv[8], &centerPrincipalPoint) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        result_matrix = cv::getOptimalNewCameraMatrix(*cameraMatrix, *distCoeffs,
                                            cv::Size(width, height), alpha,
                                            cv::Size(nwidth, nheight),
                                            &validPixROI, (bool) centerPrincipalPoint);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "getOptimalNewCameraMatrix failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat = new cv::Mat(result_matrix);
    pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(validPixROI.x));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(validPixROI.y));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(validPixROI.width));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(validPixROI.height));

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int undistort(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat result_matrix;
    cv::Mat *matrix, *cameraMatrix, *distCoeffs, *newCameraMatrix, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix cameraMatrix distCoeffs newCameraMatrix");
        return TCL_ERROR;
    }

    matrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!matrix) {
        return TCL_ERROR;
    }

    cameraMatrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!cameraMatrix) {
        return TCL_ERROR;
    }

    distCoeffs = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!distCoeffs) {
        return TCL_ERROR;
    }

    newCameraMatrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[4]);
    if (!newCameraMatrix) {
        return TCL_ERROR;
    }

    try {
        cv::undistort(*matrix, result_matrix, *cameraMatrix, *distCoeffs, *newCameraMatrix);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "undistort failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstmat = new cv::Mat(result_matrix);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int initUndistortRectifyMap(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat map1, map2;
    cv::Mat *cameraMatrix, *distCoeffs, *Rmatrix, *newCameraMatrix, *dstmat1, *dstmat2;
    int width = 0, height= 0, m1type = 0;
    Tcl_Obj *pResultStr = NULL, *pMatResultStr = NULL;

    if (objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "cameraMatrix distCoeffs R newCameraMatrix width height m1type");
        return TCL_ERROR;
    }

    cameraMatrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!cameraMatrix) {
        return TCL_ERROR;
    }

    distCoeffs = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!distCoeffs) {
        return TCL_ERROR;
    }

    /*
     * Optional rectification transformation.
     * This matrix can be empty.
     */
    Rmatrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!Rmatrix) {
        return TCL_ERROR;
    }

    newCameraMatrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[4]);
    if (!newCameraMatrix) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[7], &m1type) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::initUndistortRectifyMap(*cameraMatrix, *distCoeffs,
                                    *Rmatrix, *newCameraMatrix,
                                    cv::Size(width, height), m1type,
                                    map1, map2);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "initUndistortRectifyMap failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat1 = new cv::Mat(map1);
    pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);

    dstmat2 = new cv::Mat(map2);
    pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);
    Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int projectPoints(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat result_matrix;
    cv::Mat *objectPoints, *rvec, *tvec, *cameraMatrix, *distCoeffs, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "objectPoints rvec tvec cameraMatrix distCoeffs");
        return TCL_ERROR;
    }

    objectPoints = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!objectPoints) {
        return TCL_ERROR;
    }

    rvec = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!rvec) {
        return TCL_ERROR;
    }

    tvec = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!tvec) {
        return TCL_ERROR;
    }

    cameraMatrix = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[4]);
    if (!cameraMatrix) {
        return TCL_ERROR;
    }

    distCoeffs = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[5]);
    if (!distCoeffs) {
        return TCL_ERROR;
    }

    try {
        cv::projectPoints(*objectPoints, *rvec, *tvec,
                          *cameraMatrix, *distCoeffs, result_matrix);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "projectPoints failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstmat = new cv::Mat(result_matrix);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int findHomography(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double ransacReprojThreshold = 3;
    int method = 0;
    cv::Mat result;
    cv::Mat *mat1, *mat2, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 3 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "srcPoints dstPoints ?method ransacReprojThreshold?");
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

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[4], &ransacReprojThreshold) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        result = cv::findHomography(*mat1, *mat2,
                                    method, ransacReprojThreshold);
    } catch (...) {
        Tcl_SetResult(interp, (char *) "findHomography failed", TCL_STATIC);
        return TCL_ERROR;
    }

    dstmat = new cv::Mat(result);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


static void StereoBM_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->stereobm) {
        cvd->stereobm.release();
    }
    cvd->cmd_stereobm = NULL;
}


static int StereoBM_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "compute",
        "getPreFilterCap",
        "getPreFilterSize",
        "getPreFilterType",
        "getSmallerBlockSize",
        "getTextureThreshold",
        "getUniquenessRatio",
        "setPreFilterCap",
        "setPreFilterSize",
        "setPreFilterType",
        "setSmallerBlockSize",
        "setTextureThreshold",
        "setUniquenessRatio",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_COMPUTE,
        FUNC_getPreFilterCap,
        FUNC_getPreFilterSize,
        FUNC_getPreFilterType,
        FUNC_getSmallerBlockSize,
        FUNC_getTextureThreshold,
        FUNC_getUniquenessRatio,
        FUNC_setPreFilterCap,
        FUNC_setPreFilterSize,
        FUNC_setPreFilterType,
        FUNC_setSmallerBlockSize,
        FUNC_setTextureThreshold,
        FUNC_setUniquenessRatio,
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

    if (cvd->stereobm == nullptr) {
        Tcl_SetResult(interp, (char *) "singleton not instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_COMPUTE: {
            cv::Mat dst;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *mat1, *mat2, *dstmat;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix1 matrix2");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
            if (!mat2) {
                return TCL_ERROR;
            }

            try {
                cvd->stereobm->compute(*mat1, *mat2, dst);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "compute failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(dst);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getPreFilterCap: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereobm->getPreFilterCap();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getPreFilterCap failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getPreFilterSize: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereobm->getPreFilterSize();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getPreFilterSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getPreFilterType: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereobm->getPreFilterType();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getPreFilterType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getSmallerBlockSize: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereobm->getSmallerBlockSize();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getSmallerBlockSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getTextureThreshold: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereobm->getTextureThreshold();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getTextureThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getUniquenessRatio: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereobm->getUniquenessRatio();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getUniquenessRatio failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_setPreFilterCap: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereobm->setPreFilterCap(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setPreFilterCap failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setPreFilterSize: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereobm->setPreFilterSize(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setPreFilterSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setPreFilterType: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereobm->setPreFilterType(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setPreFilterType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setSmallerBlockSize: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereobm->setSmallerBlockSize(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setSmallerBlockSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setTextureThreshold: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereobm->setTextureThreshold(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setTextureThreshold failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setUniquenessRatio: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereobm->setUniquenessRatio(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setUniquenessRatio failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_stereobm) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_stereobm);
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
            if (cvd->cmd_stereobm) {
                Tcl_GetCommandFullName(interp, cvd->cmd_stereobm, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::StereoBM", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int StereoBM(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int numDisparities = 0, blockSize = 21;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::StereoBM> stereobm;

    if (objc != 1 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "?numDisparities blockSize?");
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[1], &numDisparities) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &blockSize) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        stereobm = cv::StereoBM::create(numDisparities, blockSize);

        if (stereobm == nullptr) {
            Tcl_SetResult(interp, (char *) "stereobm create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "stereobm failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-stereobm", -1);

    if (cvd->cmd_stereobm) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_stereobm);
    }
    cvd->cmd_stereobm =
        Tcl_CreateObjCommand(interp, "::cv-stereobm",
            (Tcl_ObjCmdProc *) StereoBM_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) StereoBM_DESTRUCTOR);

    cvd->stereobm = stereobm;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void StereoSGBM_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->stereosgbm) {
        cvd->stereosgbm.release();
    }
    cvd->cmd_stereosgbm = NULL;
}


static int StereoSGBM_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "compute",
        "getMode",
        "getP1",
        "getP2",
        "getPreFilterCap",
        "getUniquenessRatio",
        "setMode",
        "setP1",
        "setP2",
        "setPreFilterCap",
        "setUniquenessRatio",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_COMPUTE,
        FUMC_getMode,
        FUNC_getP1,
        FUNC_getP2,
        FUNC_getPreFilterCap,
        FUNC_getUniquenessRatio,
        FUNC_setMode,
        FUNC_setP1,
        FUNC_setP2,
        FUNC_setPreFilterCap,
        FUNC_setUniquenessRatio,
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

    if (cvd->stereosgbm == nullptr) {
        Tcl_SetResult(interp, (char *) "singleton not instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_COMPUTE: {
            cv::Mat dst;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *mat1, *mat2, *dstmat;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix1 matrix2");
                return TCL_ERROR;
            }

            mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat1) {
                return TCL_ERROR;
            }

            mat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
            if (!mat2) {
                return TCL_ERROR;
            }

            try {
                cvd->stereosgbm->compute(*mat1, *mat2, dst);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "compute failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(dst);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUMC_getMode: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereosgbm->getMode();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getMode failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getP1: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereosgbm->getP1();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getP1 failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getP2: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereosgbm->getP2();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getP2 failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getPreFilterCap: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereosgbm->getPreFilterCap();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getPreFilterCap failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getUniquenessRatio: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->stereosgbm->getUniquenessRatio();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getUniquenessRatio failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_setMode: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereosgbm->setMode(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setMode failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setP1: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereosgbm->setP1(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setP1 failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setP2: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereosgbm->setP2(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setP2 failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setPreFilterCap: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereosgbm->setPreFilterCap(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setPreFilterCap failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setUniquenessRatio: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->stereosgbm->setUniquenessRatio(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setUniquenessRatio failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_stereosgbm) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_stereosgbm);
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
            if (cvd->cmd_stereosgbm) {
                Tcl_GetCommandFullName(interp, cvd->cmd_stereosgbm, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::StereoSGBM", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int StereoSGBM(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int minDisparity = 0, numDisparities = 16,  blockSize = 3,  P1  = 0, P2  = 0;
    int disp12MaxDiff  = 0, preFilterCap  = 0, uniquenessRatio  = 0, speckleWindowSize  = 0;
    int speckleRange = 0, mode = cv::StereoSGBM::MODE_SGBM;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::StereoSGBM> stereosgbm;

    if (objc != 1 && objc != 12) {
        Tcl_WrongNumArgs(interp, 1, objv, "?minDisparity numDisparities blockSize P1 P2 disp12MaxDiff preFilterCap uniquenessRatio speckleWindowSize speckleRange mode?");
        return TCL_ERROR;
    }

    if (objc == 12) {
        if (Tcl_GetIntFromObj(interp, objv[1], &minDisparity) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &numDisparities) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &blockSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[4], &P1) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &P2) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &disp12MaxDiff) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &preFilterCap) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &uniquenessRatio) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[9], &speckleWindowSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[10], &speckleRange) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[11], &mode) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        stereosgbm = cv::StereoSGBM::create(minDisparity,
                                            numDisparities,
                                            blockSize,
                                            P1, P2,
                                            disp12MaxDiff,
                                            preFilterCap,
                                            uniquenessRatio,
                                            speckleWindowSize,
                                            speckleRange,
                                            mode);
        if (stereosgbm == nullptr) {
            Tcl_SetResult(interp, (char *) "StereoSGBM create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "StereoSGBM failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-stereosgbm", -1);

    if (cvd->cmd_stereosgbm) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_stereosgbm);
    }
    cvd->cmd_stereosgbm =
        Tcl_CreateObjCommand(interp, "::cv-stereosgbm",
            (Tcl_ObjCmdProc *) StereoSGBM_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) StereoSGBM_DESTRUCTOR);

    cvd->stereosgbm = stereosgbm;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
