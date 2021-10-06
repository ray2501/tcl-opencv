#include "tclopencv.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

int applyColorMap(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int colormap = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix colormap");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &colormap) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::applyColorMap(*mat, image, colormap);
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


int cvtColor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int code = 0, dstCn = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix code ?dstCn?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &code) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[3], &dstCn) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::cvtColor(*mat, image, code, dstCn);
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


int calcBackProject(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int count = 0, rangecount = 0;
    int *channels = NULL;
    float **ranges = NULL;
    double scale = 1;
    int uniform = 1;
    cv::Mat dst;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *histmat, *dstmat;

    if (objc != 5 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "matrix channels hist_matrix ranges_list ?scale uniform?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty channels list");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int size;

        channels = (int *) attemptckalloc(sizeof(int) * count);
        if (!channels) {
            return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc channels failed");
        }

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &size) != TCL_OK) {
                ckfree(channels);
                return TCL_ERROR;
            }

            if (size < 0) {
                ckfree(channels);
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "channels out of range");
            }

            channels[i] = size;
        }
    }

    histmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!histmat) {
        ckfree(channels);
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        ckfree(channels);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0 || count%2 != 0) {
        ckfree(channels);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "ranges list invalid");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int range = count / 2;
        double min, max;
        rangecount = range;

        ranges = (float **) attemptckalloc(sizeof(float *) * count);
        if (!ranges) {
            ckfree(channels);
            return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc ranges failed");
        }
        for (int i = 0; i < range; i++) {
            ranges[i] = NULL;
        }
        for (int i = 0, j = 0; i < range; i++, j = j + 2) {
            Tcl_ListObjIndex(interp, objv[4], j, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &min) != TCL_OK) {
                ckfree(channels);
                for (--i; i >= 0; --i) {
                    ckfree(ranges[i]);
                }
                ckfree(ranges);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[4], j + 1, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &max) != TCL_OK) {
                ckfree(channels);
                for (--i; i >= 0; --i) {
                    ckfree(ranges[i]);
                }
                ckfree(ranges);
                return TCL_ERROR;
            }

            ranges[i] = (float *) attemptckalloc(sizeof(float) * 2);
            if (!ranges[i]) {
                ckfree(channels);
                for (--i; i >= 0; --i) {
                    ckfree(ranges[i]);
                }
                ckfree(ranges);
                return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc ranges failed");
            }

            ranges[i][0] = (float) min;
            ranges[i][1] = (float) max;
        }
    }

    if (objc == 7) {
        if (Tcl_GetDoubleFromObj(interp, objv[5], &scale) != TCL_OK) {
            ckfree(channels);
            for (int i = 0; i < rangecount; i++) {
                ckfree(ranges[i]);
            }
            ckfree(ranges);
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[6], &uniform) != TCL_OK) {
            ckfree(channels);
            for (int i = 0; i < rangecount; i++) {
                ckfree(ranges[i]);
            }
            ckfree(ranges);
            return TCL_ERROR;
        }
    }

    try {
        cv::calcBackProject(mat, 1, channels, *histmat,
                            dst, (const float**) ranges, scale, (bool) uniform);
    } catch (const cv::Exception &ex) {
        ckfree(channels);
        for (int i = 0; i < rangecount; i++) {
            ckfree(ranges[i]);
        }
        ckfree(ranges);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        ckfree(channels);
        for (int i = 0; i < rangecount; i++) {
            ckfree(ranges[i]);
        }
        ckfree(ranges);
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dst);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    ckfree(channels);

    for (int i = 0; i < rangecount; i++) {
        ckfree(ranges[i]);
    }
    ckfree(ranges);

    return TCL_OK;
}


int calcHist(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int dims = 0, count = 0, rangecount = 0;
    int *histSize = NULL, *channels = NULL;
    float **ranges = NULL;
    int uniform = 1, accumulate = 0;
    cv::Mat mask, dst;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *maskmat, *dstmat;

    if (objc != 7 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix channels mask dims histSize_list ranges_list ?uniform accumulate?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty channels list");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int size;

        channels = (int *) attemptckalloc(sizeof(int) * count);
        if (!channels) {
            return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc channels failed");
        }

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &size) != TCL_OK) {
                ckfree(channels);
                return TCL_ERROR;
            }

            if (size < 0) {
                ckfree(channels);
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "channels out of range");
            }

            channels[i] = size;
        }
    }

    maskmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!maskmat) {
        ckfree(channels);
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &dims) != TCL_OK) {
        ckfree(channels);
        return TCL_ERROR;
    }

    if (dims < 0 || dims > 32) {
        ckfree(channels);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "dimensions out of range");
    }

    if (Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        ckfree(channels);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        ckfree(channels);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "history size list empty");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int size;

        histSize = (int *) attemptckalloc(sizeof(int) * count);
        if (!histSize) {
            ckfree(channels);
            return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc history size list failed");
        }

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[5], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &size) != TCL_OK) {
                ckfree(channels);
                ckfree(histSize);
                return TCL_ERROR;
            }

            histSize[i] = size;
        }
    }

    if (Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
        ckfree(channels);
        ckfree(histSize);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0 || count%2 != 0) {
        ckfree(channels);
        ckfree(histSize);

        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid ranges list data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int range = count / 2;
        double min, max;
        rangecount = range;

        ranges = (float **) attemptckalloc(sizeof(float *) * count);
        if (!ranges) {
            ckfree(channels);
            ckfree(histSize);
            return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc ranges list failed");
        }
        for (int i = 0; i < range; i++) {
            ranges[i] = NULL;
        }
        for (int i = 0, j = 0; i < range; i++, j = j + 2) {
            Tcl_ListObjIndex(interp, objv[6], j, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &min) != TCL_OK) {
                ckfree(channels);
                ckfree(histSize);
                for (--i; i >= 0; --i) {
                    ckfree(ranges[i]);
                }
                ckfree(ranges);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[6], j + 1, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &max) != TCL_OK) {
                ckfree(channels);
                ckfree(histSize);
                for (--i; i >= 0; --i) {
                    ckfree(ranges[i]);
                }
                ckfree(ranges);
                return TCL_ERROR;
            }

            ranges[i] = (float *) attemptckalloc(sizeof(float) * 2);
            if (!ranges[i]) {
                ckfree(channels);
                ckfree(histSize);
                for (--i; i >= 0; --i) {
                    ckfree(ranges[i]);
                }
                ckfree(ranges);
                return Opencv_SetResult(interp, cv::Error::StsNoMem, "alloc ranges list failed");
            }

            ranges[i][0] = (float) min;
            ranges[i][1] = (float) max;
        }
    }

    if (objc == 9) {
        if (Tcl_GetBooleanFromObj(interp, objv[7], &uniform) != TCL_OK) {
            ckfree(channels);
            ckfree(histSize);
            for (int i = 0; i < rangecount; i++) {
                ckfree(ranges[i]);
            }
            ckfree(ranges);
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[8], &accumulate) != TCL_OK) {
            ckfree(channels);
            ckfree(histSize);
            for (int i = 0; i < rangecount; i++) {
                ckfree(ranges[i]);
            }
            ckfree(ranges);
            return TCL_ERROR;
        }
    }

    try {
        mask = *maskmat;
        cv::calcHist(mat, 1, channels, mask, dst, dims, histSize,
                     (const float**) ranges, (bool) uniform, (bool) accumulate);
    } catch (const cv::Exception &ex) {
        ckfree(channels);
        ckfree(histSize);
        for (int i = 0; i < rangecount; i++) {
            ckfree(ranges[i]);
        }
        ckfree(ranges);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        ckfree(channels);
        ckfree(histSize);
        for (int i = 0; i < rangecount; i++) {
            ckfree(ranges[i]);
        }
        ckfree(ranges);
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dst);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    ckfree(channels);
    ckfree(histSize);

    for (int i = 0; i < rangecount; i++) {
        ckfree(ranges[i]);
    }
    ckfree(ranges);

    return TCL_OK;
}


int compareHist(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int method = 0;
    double result = 0;
    cv::Mat *mat1, *mat2;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 method");
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

    if (Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        result = cv::compareHist(*mat1, *mat2, method);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));

    return TCL_OK;
}


int equalizeHist(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dst;
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
        cv::equalizeHist(*mat, dst);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dst);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int EMD(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat cost;
    cv::Mat *mat1, *mat2, *mat3 = NULL;
    int distType = 0;
    double ret = 0;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "signature1 signature2 distType ?cost?");
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

    if (Tcl_GetIntFromObj(interp, objv[3], &distType) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        mat3 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[4]);
        if (!mat3) {
            return TCL_ERROR;
        }

        cost = *mat3;
    }

    try {
        ret = cv::EMD(*mat1, *mat2, distType, cost);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(ret));
    return TCL_OK;
}


int floodFill(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int seed_x = 0, seed_y = 0;
    int new_B = 0, new_G = 0, new_R = 0, new_A = 0;
    int r_x = 0, r_y = 0, r_w = 0, r_h = 0;
    int lo_B = 0, lo_G= 0, lo_R = 0, lo_A = 0;
    int up_B = 0, up_G = 0, up_R = 0, up_A = 0;
    int flags = 4, count = 0;
    cv::Mat *mat;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
                "matrix seed_x seed_y color_list ?rect_list loDiff_color upDiff_color flags?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &seed_x) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &seed_y) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[4], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[4], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &new_B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &new_G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &new_R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[4], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &new_A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (objc == 9) {
        if (Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
        }

        if (count != 4) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid rect data");
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[5], 0, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &r_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &r_y) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &r_w) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &r_h) != TCL_OK) {
                return TCL_ERROR;
            }
        }

        if (Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
        }

        if (count != 4) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[6], 0, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &lo_B) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[6], 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &lo_G) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[6], 2, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &lo_R) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[6], 3, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &lo_A) != TCL_OK) {
                return TCL_ERROR;
            }
        }

        if (Tcl_ListObjLength(interp, objv[7], &count) != TCL_OK) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
        }

        if (count != 4) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
        } else {
            Tcl_Obj *elemListPtr = NULL;

            Tcl_ListObjIndex(interp, objv[7], 0, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &up_B) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &up_G) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 2, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &up_R) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[7], 3, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &up_A) != TCL_OK) {
                return TCL_ERROR;
            }
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar newColor(new_B, new_G, new_R, new_A);
        cv::Rect rect(r_x, r_y, r_w, r_h);
        cv::Scalar loColor(lo_B, lo_G, lo_R, lo_A);
        cv::Scalar upColor(up_B, up_G, up_R, up_A);

        if (objc == 9) {
            cv::floodFill(*mat, cv::Point(seed_x, seed_y), newColor,
                      &rect, loColor, upColor, flags);
        } else {
            cv::floodFill(*mat, cv::Point(seed_x, seed_y), newColor);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int grabCut(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int x = 0, y = 0, width = 0, height = 0;
    int iterCount = 0, mode = cv::GC_INIT_WITH_RECT;
    cv::Mat maskimage, bgdModel, fgdModel;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix x y width height iterCount");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
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
    if (Tcl_GetIntFromObj(interp, objv[6], &iterCount) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::Rect rect(x, y, width, height);

        cv::grabCut(*mat, maskimage, rect, bgdModel, fgdModel, iterCount, mode);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(maskimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int matchTemplate(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat result_image;
    int method = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *templmat, *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix templ_matrix method");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    templmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!templmat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        if (mat->cols < templmat->cols ||
            mat->rows < templmat->rows) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "source image too small");
        }

        cv::matchTemplate(*mat, *templmat,
                          result_image, method);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(result_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int moments(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int binaryImage = 0;
    cv::Mat *mat;
    cv::Moments result;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?binaryImage?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetBooleanFromObj(interp, objv[2], &binaryImage) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        result = cv::moments(*mat, (bool) binaryImage);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    /*
     * https://docs.opencv.org/master/d8/d23/classcv_1_1Moments.html
     */
    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m00));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m10));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m01));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m20));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m11));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m02));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m30));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m21));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m12));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.m03));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int getRotationMatrix2D(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int x = 0, y = 0;
    double angle = 0.0, scale = 1.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "x y angle scale");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &x) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &y) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &angle) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &scale) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        image = cv::getRotationMatrix2D(cv::Point2f(x, y), angle, scale);
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


int getRectSubPix(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat sub_image;
    int width = 0, height = 0, center_x = 0, center_y = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix width height center_x center_y");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (width <= 0 || height <= 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid range");
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &center_x) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &center_y) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::getRectSubPix(*mat, cv::Size(width, height),
                          cv::Point2f(center_x, center_y), sub_image);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(sub_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int HuMoments(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int count;
    double hu[7] ;
    Tcl_Obj *pResultStr = NULL;
    double m00, m10, m01, m20, m11, m02, m30, m21, m12, m03;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "moments_list");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 10) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[1], 0, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m00) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 1, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m10) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 2, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m01) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 3, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m20) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 4, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m11) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 5, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m02) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 6, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m30) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 7, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m21) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 8, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m12) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 9, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &m03) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Moments moments(m00, m10, m01, m20, m11, m02, m30, m21, m12, m03);
        cv::HuMoments(moments, hu);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(hu[0]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(hu[1]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(hu[2]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(hu[3]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(hu[4]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(hu[5]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(hu[6]));

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int integral(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat sum, sqsum, tilted;
    int sdepth = -1, sqdepth = -1;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL, *pResultStr3 = NULL;
    cv::Mat *mat, *dstmat1, *dstmat2, *dstmat3;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?sdepth sqdepth?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[2], &sdepth) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &sqdepth) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::integral(*mat, sum, sqsum, tilted, sqdepth, sqdepth);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat1 = new cv::Mat(sum);
    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

    dstmat2 = new cv::Mat(sqsum);
    pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

    dstmat3 = new cv::Mat(tilted);
    pResultStr3 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat3);

    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr3);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int remap(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int interpolation = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *mapmat1, *mapmat2, *dstmat;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix map1 map2 interpolation");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    mapmat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mapmat1) {
        return TCL_ERROR;
    }

    mapmat2 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!mapmat2) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &interpolation) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::remap(*mat, image, *mapmat1, *mapmat2, interpolation);
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


int resize(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int width = 0, height = 0;
    int flags = cv::INTER_LINEAR;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix width height ?flags?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (width <= 0 || height <= 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid range");
    }

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[4], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::resize(*mat, image, cv::Size(width, height), 0, 0, flags);
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


int threshold(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    double thresh = 0, maxval = 0;
    int type;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix thresh maxval type");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &thresh) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &maxval) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::threshold(*mat, image, thresh, maxval, type);
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


int adaptiveThreshold(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    double maxValue = 0, C = 0;
    int adaptiveMethod = 0, thresholdType = 0, blockSize = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix maxValue adaptiveMethod thresholdType blockSize C");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &maxValue) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &adaptiveMethod) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &thresholdType) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &blockSize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[6], &C) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::adaptiveThreshold(*mat, image, maxValue,
                              adaptiveMethod, thresholdType, blockSize, C);
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


int getAffineTransform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int count = 0, npts = 0;
    cv::Mat mat_image;
    std::vector<cv::Point2f> src_points, dst_points;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_list dst_list");
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

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
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
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            cv::Point2f point;
            point.x = (float) number_from_list_x;
            point.y = (float) number_from_list_y;
            dst_points.push_back(point);
        }
    }

    try {
        mat_image = cv::getAffineTransform(src_points, dst_points);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(mat_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int warpAffine(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat rotated_image;
    int width = 0, height = 0;
    int flags = cv::INTER_LINEAR;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *rotmat, *dstmat;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix transformation_matrix width height ?flags?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    rotmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!rotmat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (width <= 0 || height <= 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid range");
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[5], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::warpAffine(*mat, rotated_image, *rotmat,
                        cv::Size(width, height), flags);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(rotated_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int getPerspectiveTransform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int count = 0, npts = 0;
    cv::Mat mat_image;
    std::vector<cv::Point2f> src_points, dst_points;
    int solveMethod = cv::DECOMP_LU;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_list dst_list ?solveMethod?");
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

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
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
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            cv::Point2f point;
            point.x = (float) number_from_list_x;
            point.y = (float) number_from_list_y;
            dst_points.push_back(point);
        }
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[3], &solveMethod) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
#ifdef TCL_USE_OPENCV4
        mat_image = cv::getPerspectiveTransform(src_points, dst_points, solveMethod);
#else
        mat_image = cv::getPerspectiveTransform(src_points, dst_points);
#endif
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(mat_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int warpPerspective(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat mat_image;
    int width = 0, height = 0;
    int flags = cv::INTER_LINEAR;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *matmat, *dstmat;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix transformation_matrix width height ?flags?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    matmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!matmat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (width <= 0 || height <= 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid range");
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[5], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::warpPerspective(*mat, mat_image, *matmat,
                        cv::Size(width, height), flags);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(mat_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int warpPolar(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat mat_image;
    int dsize_width = 0, dsize_height = 0;
    int flags = cv::WarpPolarMode::WARP_POLAR_LINEAR;
    double center_x = 0, center_y = 0, maxRadius = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 7 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix dsize_width dsize_height center_x center_y maxRadius ?flags?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &dsize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &dsize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &center_x) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &center_y) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[6], &maxRadius) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if (Tcl_GetIntFromObj(interp, objv[7], &flags) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::warpPolar(*mat, mat_image,
                      cv::Size(dsize_width, dsize_height),
                      cv::Point2f((float) center_x, (float) center_y),
                      maxRadius, flags);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(mat_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}
#endif


int filter2D(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat filter_image;
    int anchor_x = -1, anchor_y = -1, borderType = cv::BORDER_DEFAULT;
    double delta = 0.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *kmat, *dstmat;

    if (objc != 3 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix kernel_matrix ?anchor_x anchor_y delta borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    kmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!kmat) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if (Tcl_GetIntFromObj(interp, objv[3], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[4], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[5], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::filter2D(*mat, filter_image, -1, *kmat,
                     cv::Point(anchor_x, anchor_y), delta, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(filter_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int sepFilter2D(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat filter_image;
    int anchor_x = -1, anchor_y = -1, borderType = cv::BORDER_DEFAULT;
    double delta = 0.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *kmatx, *kmaty, *dstmat;

    if (objc != 4 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix kernelX kernelY ?anchor_x anchor_y delta borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    kmatx = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!kmatx) {
        return TCL_ERROR;
    }

    kmaty = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!kmaty) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if (Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[6], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::sepFilter2D(*mat, filter_image, -1, *kmatx, *kmaty,
                        cv::Point(anchor_x, anchor_y), delta, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(filter_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int getGaborKernel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat result_mat;
    int ksize_width = 0, ksize_height = 0, type = CV_64F;
    double sigma = 0, theta = 0, lambd = 0, gamma = 0, psi = CV_PI * 0.5;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 7 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "ksize_width ksize_height sigma theta lambd gamma ?psi type?");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &sigma) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &theta) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &lambd) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[6], &gamma) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if (Tcl_GetDoubleFromObj(interp, objv[7], &psi) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &type) != TCL_OK) {
            return TCL_ERROR;
        }

        if (type != CV_32F && type != CV_64F) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong Gabor kernel type");
        }
    }

    try {
        result_mat = cv::getGaborKernel(cv::Size(ksize_width, ksize_height),
                           sigma, theta, lambd, gamma, psi, type);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(result_mat);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int getGaussianKernel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat result_mat;
    int ksize = 0, type = CV_64F;
    double sigma = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "ksize sigma ?type?");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &ksize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &sigma) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
            return TCL_ERROR;
        }

        if (type != CV_32F && type != CV_64F) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "wrong Gaussian kernel type");
        }
    }

    try {
        result_mat = cv::getGaussianKernel(ksize, sigma, type);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(result_mat);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int blur(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat blur_image;
    int ksize_width = 0, ksize_height = 0, anchor_x = -1, anchor_y = -1;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix ksize_width ksize_height ?anchor_x anchor_y borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if (Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::blur(*mat, blur_image,
                 cv::Size(ksize_width, ksize_height),
                 cv::Point(anchor_x, anchor_y), borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(blur_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int GaussianBlur(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat blur_image;
    int ksize_width = 0, ksize_height = 0;
    double sigmaX = 0, sigmaY = 0;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 5 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix ksize_width ksize_height sigmaX ?sigmaY borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &sigmaX) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if (Tcl_GetDoubleFromObj(interp, objv[5], &sigmaY) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::GaussianBlur(*mat, blur_image,
                 cv::Size(ksize_width, ksize_height),
                 sigmaX, sigmaY, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(blur_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int medianBlur(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat blur_image;
    int ksize = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix ksize");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &ksize) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::medianBlur(*mat, blur_image, ksize);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(blur_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int bilateralFilter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat blur_image;
    int d = 0;
    double sigmaColor = 0, sigmaSpace = 0;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "src_matrix d sigmaColor sigmaSpace ?borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &d) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &sigmaColor) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &sigmaSpace) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[5], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::bilateralFilter(*mat, blur_image,
                            d, sigmaColor, sigmaSpace, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(blur_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int boxFilter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat filter_image;
    int ksize_width = 0, ksize_height = 0, anchor_x = -1, anchor_y = -1;
    int normalize = 1;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix ksize_width ksize_height ?anchor_x anchor_y normalize borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if (Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[6], &normalize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::boxFilter(*mat, filter_image, -1,
                      cv::Size(ksize_width, ksize_height),
                      cv::Point(anchor_x, anchor_y), (bool) normalize, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(filter_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int sqrBoxFilter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat filter_image;
    int ksize_width = 0, ksize_height = 0, anchor_x = -1, anchor_y = -1;
    int normalize = 1;
    int borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix ksize_width ksize_height ?anchor_x anchor_y normalize borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if (Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[6], &normalize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::sqrBoxFilter(*mat, filter_image, -1,
                         cv::Size(ksize_width, ksize_height),
                         cv::Point(anchor_x, anchor_y), (bool) normalize, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(filter_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int getStructuringElement(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat kernel;
    int shape = 0, ksize_width = 0, ksize_height = 0, anchor_x = -1, anchor_y = -1;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "shape ksize_width ksize_height ?anchor_x anchor_y?");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &shape) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &ksize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ksize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        kernel = cv::getStructuringElement(shape, cv::Size(ksize_width, ksize_height),
                                           cv::Point(anchor_x, anchor_y));
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(kernel);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int dilate(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int anchor_x = -1, anchor_y = -1, iterations = 1;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *kmat, *dstmat;

    if (objc != 3 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix kernel_matrix ?anchor_x anchor_y iterations?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    kmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!kmat) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[3], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[4], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[5], &iterations) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::dilate(*mat, image, *kmat,
                   cv::Point(anchor_x, anchor_y), iterations);
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


int erode(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int anchor_x = -1, anchor_y = -1, iterations = 1;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *kmat, *dstmat;

    if (objc != 3 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix kernel_matrix ?anchor_x anchor_y iterations?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    kmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!kmat) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[3], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[4], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[5], &iterations) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::erode(*mat, image, *kmat,
                   cv::Point(anchor_x, anchor_y), iterations);
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


int morphologyEx(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int op = 0, anchor_x = -1, anchor_y = -1, iterations = 1;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *kmat, *dstmat;

    if (objc != 4 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "src_matrix op kernel_matrix ?anchor_x anchor_y iterations?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &op) != TCL_OK) {
        return TCL_ERROR;
    }

    kmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!kmat) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if (Tcl_GetIntFromObj(interp, objv[4], &anchor_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[5], &anchor_y) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[6], &iterations) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::morphologyEx(*mat, image, op, *kmat,
                         cv::Point(anchor_x, anchor_y), iterations);
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


int buildPyramid(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int maxlevel = 0, borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat;
    std::vector<cv::Mat> dst;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix maxlevel ?borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &maxlevel) != TCL_OK) {
        return TCL_ERROR;
    }

    if (maxlevel < 0) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[3], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::buildPyramid(*mat, dst, maxlevel, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    for(size_t i = 0; i < dst.size(); i++) {
        cv::Mat *dstmat;
        Tcl_Obj *pMatResultStr = NULL;

        dstmat = new cv::Mat(dst[i]);
        pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
        Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);
    }

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int pyrUp(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int dstsize_width = 0, dstsize_height = 0, borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix ?dstsize_width dstsize_height borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[2], &dstsize_width) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &dstsize_height) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[4], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 5) {
            cv::Size dstsize(dstsize_width, dstsize_height);
            cv::pyrUp(*mat, image, dstsize, borderType);
        } else {
            cv::pyrUp(*mat, image);
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


int pyrDown(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int dstsize_width = 0, dstsize_height = 0, borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix ?dstsize_width dstsize_height borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[2], &dstsize_width) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &dstsize_height) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[4], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 5) {
            cv::Size dstsize(dstsize_width, dstsize_height);
            cv::pyrDown(*mat, image, dstsize, borderType);
        } else {
            cv::pyrDown(*mat, image);
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


int pyrMeanShiftFiltering(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    double sp = 0, sr = 0;
    int maxLevel = 1;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix sp sr ?maxLevel?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &sp) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &sr) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[4], &maxLevel) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::pyrMeanShiftFiltering(*mat, image, sp, sr, maxLevel);
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


int createHanningWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int winSize_width = 0, winSize_height = 0, type = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "winSize_width winSize_height type");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &winSize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &winSize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &type) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::createHanningWindow(image, cv::Size(winSize_width, winSize_height), type);
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


int phaseCorrelate(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat1, *mat2, *window;
    cv::Point2d result;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix_1 matrix_2 ?window?");
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
        window = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
        if (!window) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 3) {
            result = cv::phaseCorrelate(*mat1, *mat2);
        } else {
            result = cv::phaseCorrelate(*mat1, *mat2, *window);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.x));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(result.y));

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int Canny(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int apertureSize = 3, L2gradient = 0;
    double threshold1 = 0.0, threshold2 = 0.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix threshold1 threshold2 ?apertureSize L2gradient?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &threshold1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &threshold2) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[4], &apertureSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[5], &L2gradient) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Canny(*mat, image, threshold1, threshold2,
                  apertureSize, (bool) L2gradient);
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


int Sobel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int ddepth = CV_64F;
    int dx = 0, dy = 0, ksize= 3, borderType = cv::BORDER_DEFAULT;
    double scale = 1.0, delta = 0.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix dx dy ?ksize scale delta borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &dx) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &dy) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if (Tcl_GetIntFromObj(interp, objv[4], &ksize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[5], &scale) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[6], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Sobel(*mat, image, ddepth, dx, dy,
                  ksize, scale, delta, borderType);
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


int Scharr(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int ddepth = CV_64F;
    int dx = 0, dy = 0, borderType = cv::BORDER_DEFAULT;
    double scale = 1.0, delta = 0.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix dx dy ?scale delta borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &dx) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &dy) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if (Tcl_GetDoubleFromObj(interp, objv[4], &scale) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[5], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scharr(*mat, image, ddepth, dx, dy,
                   scale, delta, borderType);
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


int Laplacian(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int ddepth = CV_8U;
    int ksize = 1, borderType = cv::BORDER_DEFAULT;
    double scale = 1.0, delta = 0.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?ksize scale delta borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[2], &ksize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &scale) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[4], &delta) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Laplacian(*mat, image, ddepth, ksize,
                      scale, delta, borderType);
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


int distanceTransform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int dstType = CV_32F;
    int distanceType = 1, maskSize;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix distanceType maskSize");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &distanceType) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &maskSize) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::distanceTransform(*mat, image, distanceType, maskSize, dstType);
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


int connectedComponents(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat image;
    int ltype = CV_32S;
    int connectivity = 8;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?connectivity?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &connectivity) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::connectedComponents(*mat, image, connectivity, ltype);
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


int connectedComponentsWithStats(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat labels, stats, centroids;
    int ltype = CV_32S;
    int connectivity = 8;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL, *pResultStr3 = NULL;
    cv::Mat *mat, *dstmat1, *dstmat2, *dstmat3;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?connectivity?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (objc == 3) {
        if (Tcl_GetIntFromObj(interp, objv[2], &connectivity) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::connectedComponentsWithStats(*mat, labels, stats, centroids, connectivity, ltype);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    dstmat1 = new cv::Mat(labels);
    pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

    dstmat2 = new cv::Mat(stats);
    pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

    dstmat3 = new cv::Mat(centroids);
    pResultStr3 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat3);

    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr3);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int watershed(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *smat, *mmat;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix markers");
        return TCL_ERROR;
    }

    smat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!smat) {
        return TCL_ERROR;
    }

    mmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
    if (!mmat) {
        return TCL_ERROR;
    }

    try {
        cv::watershed(*smat, *mmat);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int goodFeaturesToTrack(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int maxCorners = 0, blockSize = 3, useHarrisDetector = 0;
    double qualityLevel = 0, minDistance = 0, k = 0.04;
    cv::Mat maskimage, result_image;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *maskmat, *dstmat;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix maxCorners qualityLevel minDistance ?mask blockSize useHarrisDetector k?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &maxCorners) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &qualityLevel) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &minDistance) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        maskmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[5]);
        if (!maskmat) {
            return TCL_ERROR;
        }

        maskimage = *maskmat;

        if (Tcl_GetIntFromObj(interp, objv[6], &blockSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[7], &useHarrisDetector) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[8], &k) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::goodFeaturesToTrack(*mat, result_image, maxCorners, qualityLevel, minDistance,
                                maskimage, blockSize, (bool) useHarrisDetector, k);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(result_image);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int cornerHarris(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dst;
    int blockSize = 0, ksize = 0, borderType = cv::BORDER_DEFAULT;
    double k = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix blockSize ksize k ?borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &blockSize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ksize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &k) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[5], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::cornerHarris(*mat, dst, blockSize, ksize, k, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dst);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int cornerEigenValsAndVecs(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dst;
    int blockSize = 0, ksize = 0, borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix blockSize ksize ?borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &blockSize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ksize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[4], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::cornerEigenValsAndVecs(*mat, dst, blockSize, ksize, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dst);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int cornerMinEigenVal(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dst;
    int blockSize = 0, ksize = 0, borderType = cv::BORDER_DEFAULT;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 4 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix blockSize ksize ?borderType?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &blockSize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ksize) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 5) {

        if (Tcl_GetIntFromObj(interp, objv[4], &borderType) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::cornerMinEigenVal(*mat, dst, blockSize, ksize, borderType);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dst);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int cornerSubPix(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int winSize_width = 0, winSize_height = 0, zeroZone_widht = 0, zeroZone_height;
    cv::Mat *mat1, *mat2;
    cv::TermCriteria *termCriteria;

    if (objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix corners winSize_width winSize_height zeroZone_widht zeroZone_height termCriteria");
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

    if (Tcl_GetIntFromObj(interp, objv[3], &winSize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &winSize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &zeroZone_widht) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &zeroZone_height) != TCL_OK) {
        return TCL_ERROR;
    }

    termCriteria = (cv::TermCriteria *) Opencv_FindHandle(cd, interp, OPENCV_TERMCRITERIA, objv[7]);
    if (!termCriteria) {
        return TCL_ERROR;
    }

    try {
        cv::cornerSubPix(*mat1, *mat2,
                         cv::Size(winSize_width, winSize_height),
                         cv::Size(zeroZone_widht, zeroZone_height),
                         *termCriteria);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int HoughCircles(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int method = 0, minRadius = 0, maxRadius = 0;
    double dp = 0, minDist = 0, param1 = 100, param2 = 100;
    cv::Mat *mat;
    Tcl_Obj *pResultStr = NULL;
    std::vector<cv::Vec3f> circles;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix method dp minDist ?param1 param2 minRadius maxRadius?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &dp) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &minDist) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if (Tcl_GetDoubleFromObj(interp, objv[5], &param1) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[6], &param2) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &minRadius) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &maxRadius) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::HoughCircles(*mat, circles, method, dp, minDist,
                         param1, param2, minRadius, maxRadius);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < circles.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj((int) circles[i][0]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj((int) circles[i][1]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj((int) circles[i][2]));

        Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int HoughLines(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int threshold = 0;
    double rho = 0.0, theta = 0.0, srn = 0.0, stn = 0.0, min_theta = 0.0, max_theta = CV_PI;
    cv::Mat *mat;
    Tcl_Obj *pResultStr = NULL;
    std::vector<cv::Vec2f> lines;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix rho theta threshold ?srn stn min_theta max_theta?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &rho) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &theta) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if (Tcl_GetDoubleFromObj(interp, objv[5], &srn) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[6], &stn) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[7], &min_theta) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[8], &max_theta) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::HoughLines(*mat, lines, rho, theta, threshold,
                       srn, stn, min_theta, max_theta);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < lines.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(lines[i][0]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewDoubleObj(lines[i][1]));

        Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int HoughLinesP(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int threshold = 0;
    double rho = 0.0, theta = 0.0, minLineLength = 0.0, maxLineGap = 0.0;
    cv::Mat *mat;
    Tcl_Obj *pResultStr = NULL;
    std::vector<cv::Vec4i> lines;

    if (objc != 5 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix rho theta threshold ?minLineLength maxLineGap?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &rho) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &theta) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if (Tcl_GetDoubleFromObj(interp, objv[5], &minLineLength) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[6], &maxLineGap) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::HoughLinesP(*mat, lines, rho, theta, threshold,
                        minLineLength, maxLineGap);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < lines.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(lines[i][0]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(lines[i][1]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(lines[i][2]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(lines[i][3]));

        Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int findContours(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int mode = 0, method = 0, offset_point_x = 0, offset_point_y = 0;
    cv::Mat *mat;
    Tcl_Obj *pResultStr = NULL;
    std::vector<std::vector<cv::Point> > contours;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix mode method ?offset_point_x offset_point_y?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &mode) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[4], &offset_point_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &offset_point_y) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 6) {
            cv::findContours(*mat, contours, mode, method,
                             cv::Point(offset_point_x, offset_point_y));
        } else {
            cv::findContours(*mat, contours, mode, method);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < contours.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);

        for (size_t j = 0; j < contours.at(i).size(); j++) {
            Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(contours.at(i).at(j).x));
            Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(contours.at(i).at(j).y));
        }

        Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int findContoursWithHierarchy(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int mode = 0, method = 0, offset_point_x = 0, offset_point_y = 0;
    cv::Mat *mat;
    Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix mode method ?offset_point_x offset_point_y?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &mode) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[4], &offset_point_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &offset_point_y) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (objc == 6) {
            cv::findContours(*mat, contours, hierarchy, mode, method,
                             cv::Point(offset_point_x, offset_point_y));
        } else {
            cv::findContours(*mat, contours, hierarchy, mode, method);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);

    pResultStr1 = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < contours.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);

        for (size_t j = 0; j < contours.at(i).size(); j++) {
            Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(contours.at(i).at(j).x));
            Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(contours.at(i).at(j).y));
        }

        Tcl_ListObjAppendElement(NULL, pResultStr1, pListStr);
    }

    pResultStr2 = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < hierarchy.size(); i++) {
        Tcl_Obj *pListStr = NULL;
        pListStr = Tcl_NewListObj(0, NULL);

        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(hierarchy[i][0]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(hierarchy[i][1]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(hierarchy[i][2]));
        Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(hierarchy[i][3]));

        Tcl_ListObjAppendElement(NULL, pResultStr2, pListStr);
    }

    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
    Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);
    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int drawContours(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int contourIdx = 0, thickness = 1, lineType = cv::LINE_8, maxLevel = INT_MAX;
    int B = 0, G = 0, R = 0, A = 0, count = 0;
    int offset_point_x = 0, offset_point_y = 0;
    cv::Mat *mat;
    std::vector<std::vector<cv::Point> > contours;

    if (objc != 6 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
                "matrix contours_list contourIdx color_list thickness ?lineType maxLevel offset_point_x offset_point_y?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count <= 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty list");
    } else {
        for (int contours_count = 0; contours_count < count; contours_count++) {
            Tcl_Obj *elemListPtr = NULL;
            int index_count = 0;
            std::vector<cv::Point> points;

            Tcl_ListObjIndex(interp, objv[2], contours_count, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &index_count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contours data");
            }

            if (index_count%2 != 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid points data");
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                int number_from_list_x;
                int number_from_list_y;
                int npts;

                npts = index_count / 2;
                for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
                    cv::Point point;
                    Tcl_ListObjIndex(interp, elemListPtr, i, &elemListSubPtr);
                    if (Tcl_GetIntFromObj(interp, elemListSubPtr, &number_from_list_x) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, i + 1, &elemListSubPtr);
                    if (Tcl_GetIntFromObj(interp, elemListSubPtr, &number_from_list_y) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    point.x = number_from_list_x;
                    point.y = number_from_list_y;
                    points.push_back (point);
                }
            }

            contours.push_back(points);
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &contourIdx) != TCL_OK) {
        return TCL_ERROR;
    }

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

    if (Tcl_GetIntFromObj(interp, objv[5], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 10) {
        if (Tcl_GetIntFromObj(interp, objv[6], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &maxLevel) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &offset_point_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[9], &offset_point_y) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);
        cv::drawContours(*mat, contours, contourIdx, color,
                        thickness, lineType, cv::noArray(), maxLevel,
                        cv::Point(offset_point_x, offset_point_y));
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int drawContoursWithHierarchy(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int contourIdx = 0, thickness = 1, lineType = cv::LINE_8, maxLevel = INT_MAX;
    int B = 0, G = 0, R = 0, A = 0, count = 0;
    int offset_point_x = 0, offset_point_y = 0;
    cv::Mat *mat;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    if (objc != 9 && objc != 11) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix contours_list contourIdx color_list thickness lineType hierarchy maxLevel ?offset_point_x offset_point_y?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count <= 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty list");
    } else {
        for (int contours_count = 0; contours_count < count; contours_count++) {
            Tcl_Obj *elemListPtr = NULL;
            int index_count = 0;
            std::vector<cv::Point> points;

            Tcl_ListObjIndex(interp, objv[2], contours_count, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &index_count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contours data");
            }

            if (index_count%2 != 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid points data");
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                int number_from_list_x;
                int number_from_list_y;
                int npts;

                npts = index_count / 2;
                for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
                    cv::Point point;
                    Tcl_ListObjIndex(interp, elemListPtr, i, &elemListSubPtr);
                    if (Tcl_GetIntFromObj(interp, elemListSubPtr, &number_from_list_x) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, i + 1, &elemListSubPtr);
                    if (Tcl_GetIntFromObj(interp, elemListSubPtr, &number_from_list_y) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    point.x = number_from_list_x;
                    point.y = number_from_list_y;
                    points.push_back (point);
                }
            }

            contours.push_back(points);
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &contourIdx) != TCL_OK) {
        return TCL_ERROR;
    }

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

    if (Tcl_GetIntFromObj(interp, objv[5], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &lineType) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[7], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count <= 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty list");
    } else {
        for (int hierarchy_count = 0; hierarchy_count < count; hierarchy_count++) {
            Tcl_Obj *elemListPtr = NULL;
            int index_count = 0;
            cv::Vec4i veci;

            Tcl_ListObjIndex(interp, objv[7], hierarchy_count, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &index_count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid hierarchy data");
            }

            if (index_count != 4) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid hierarchy data");
            } else {
                Tcl_Obj *elemListSubPtr = NULL;
                int number;

                for (int i = 0; i < 4; i++) {

                    Tcl_ListObjIndex(interp, elemListPtr, i, &elemListSubPtr);
                    if (Tcl_GetIntFromObj(interp, elemListSubPtr, &number) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    veci[i] = number;
                }
            }

            hierarchy.push_back(veci);
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[8], &maxLevel) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 11) {

        if (Tcl_GetIntFromObj(interp, objv[9], &offset_point_x) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[10], &offset_point_y) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);
        cv::drawContours(*mat, contours, contourIdx, color,
                        thickness, lineType, hierarchy, maxLevel,
                        cv::Point(offset_point_x, offset_point_y));
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int approxPolyDP(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points, approxCurve;
    int closed = 0, count = 0;
    double epsilon = 0;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour epsilon closed");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid points data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &epsilon) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetBooleanFromObj(interp, objv[3], &closed) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::approxPolyDP(points, approxCurve, epsilon, (bool) closed);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < approxCurve.size(); i++) {
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(approxCurve.at(i).x));
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(approxCurve.at(i).y));
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int arcLength(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    int closed = 0, count = 0;
    double result = 0;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour closed");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid points data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if (Tcl_GetBooleanFromObj(interp, objv[2], &closed) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        result = cv::arcLength(points, (bool) closed);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int contourArea(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    int oriented = 0, count = 0;
    double result = 0;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour ?oriented?");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid points data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if (objc == 3) {
        if (Tcl_GetBooleanFromObj(interp, objv[2], &oriented) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        result = cv::contourArea(points, (bool) oriented);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int boundingRect(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    cv::Rect rect;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    try {
        rect = cv::boundingRect(points);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(rect.x));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(rect.y));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(rect.width));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(rect.height));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int minAreaRect(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    cv::RotatedRect rect;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    try {
        rect = cv::minAreaRect(points);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(rect.center.x));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(rect.center.y));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(rect.size.width));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(rect.size.height));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(rect.angle));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int fitEllipse(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    cv::RotatedRect rect;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
        return TCL_ERROR;
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    try {
        rect = cv::fitEllipse(points);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.center.x));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.center.y));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.size.width));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.size.height));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) rect.angle));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int fitLine(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    cv::Vec4f line;
    cv::RotatedRect rect;
    Tcl_Obj *pResultStr = NULL;
    int count = 0, distType = 0;
    double param = 0, reps = 0, aeps = 0;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour distType param reps aeps");
        return TCL_ERROR;
    }

    /*
     * For 2D point set.
     */
    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &distType) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &param) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &reps) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &aeps) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::fitLine(points, line, distType, param, reps, aeps);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) line[0]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) line[1]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) line[2]));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) line[3]));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int boxPoints(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat points;
    double x, y, width, height, angle;
    cv::RotatedRect rect;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "box");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid box data");
    }

    if (count != 5) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid rotated rect data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[1], 0, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &x) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 1, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &y) != TCL_OK) {
            return TCL_ERROR;
        }
        Tcl_ListObjIndex(interp, objv[1], 2, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &width) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 3, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &height) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 4, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &angle) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Point2f center((float) x, (float) y);
        cv::Size2f size((float) width, (float) height);
        cv::boxPoints(cv::RotatedRect(center, size, (float) angle),
                      points);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    cv::MatIterator_<float> _it = points.begin<float>();
    for (;_it != points.end<float>(); _it++) {
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) *_it));
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int minEnclosingCircle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    cv::Point2f center;
    float radius;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    try {
        cv::minEnclosingCircle(points, center, radius);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) center.x));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) center.y));
    Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj((int) radius));

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int convexHull(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points, hull;
    std::vector<int> hull_ind;
    Tcl_Obj *pResultStr = NULL;
    int count = 0, clockwise = 0, returnPoints = 1;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour ?clockwise returnPoints?");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if (objc == 4) {
        if (Tcl_GetBooleanFromObj(interp, objv[2], &clockwise) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[3], &returnPoints) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (returnPoints == 1) {
            cv::convexHull(points, hull, clockwise);
        } else {
            cv::convexHull(points, hull_ind, clockwise, 0);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    if (returnPoints == 1) {
        for (size_t i = 0; i < hull.size(); i++) {
            Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(hull.at(i).x));
            Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(hull.at(i).y));
        }
    } else {
        for (size_t i = 0; i < hull_ind.size(); i++) {
            Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(hull_ind[i]));
        }
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int convexityDefects(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    std::vector<int> hull_ind;
    std::vector<cv::Vec4i> results;
    Tcl_Obj *pResultStr = NULL;
    int count = 0;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour convexhull");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid convexhull data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid convexhull data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        for (int i = 0; i < count; i++) {
            int number = 0;

            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number) != TCL_OK) {
                return TCL_ERROR;
            }

            hull_ind.push_back (number);
        }
    }

    try {
        cv::convexityDefects(points, hull_ind, results);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(0, NULL);
    for (size_t i = 0; i < results.size(); i++) {
        Tcl_Obj *pListResultStr = NULL;

        pListResultStr = Tcl_NewListObj(0, NULL);

        /*
         * start_index, end_index, farthest_pt_index, fixpt_depth
         */
        Tcl_ListObjAppendElement(NULL, pListResultStr, Tcl_NewIntObj(results[i][0]));
        Tcl_ListObjAppendElement(NULL, pListResultStr, Tcl_NewIntObj(results[i][1]));
        Tcl_ListObjAppendElement(NULL, pListResultStr, Tcl_NewIntObj(results[i][2]));
        Tcl_ListObjAppendElement(NULL, pListResultStr, Tcl_NewIntObj(results[i][3]));

        Tcl_ListObjAppendElement(NULL, pResultStr, pListResultStr);
    }

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int matchShapes(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points1, points2;
    int count = 0, method = 0;
    double result = 0;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour1 contour2 method");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points1.push_back (point);
        }
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points2.push_back (point);
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &method) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        result = cv::matchShapes(points1, points2, method, 0.0);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int pointPolygonTest(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Point> points;
    int count = 0, measureDist = 0;
    double x = 0, y = 0, result = 0;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "contour x y measureDist");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid contour data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;
        int npts;

        npts = count / 2;
        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            cv::Point point;
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[1], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                return TCL_ERROR;
            }

            point.x = number_from_list_x;
            point.y = number_from_list_y;
            points.push_back (point);
        }
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &x) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &y) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetBooleanFromObj(interp, objv[4], &measureDist) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        /*
         * When measureDist=false , the return value is +1, -1, and 0,
         * respectively. Otherwise, the return value is a signed distance
         * between the point and the nearest contour edge.
         */
        result = cv::pointPolygonTest(points, cv::Point2f((float) x, (float) y), measureDist);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(result));
    return TCL_OK;
}


int arrowedLine(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int count = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int B = 0, G = 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;
    double tipLength = 0.1;

    if (objc != 8 && objc != 11) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_x1 point_y point2_x2 point2_y2 color_list thickness ?lineType shift tipLength?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[6], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[7], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 11) {
        if (Tcl_GetIntFromObj(interp, objv[8], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[9], &shift) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[10], &tipLength) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::arrowedLine(*mat, cv::Point(x1, y1), cv::Point(x2, y2),
                color, thickness, lineType, shift, tipLength);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int circle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int count = 0;
    int x1 = 0, y1 = 0, radius = 0;
    int B = 0, G = 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;

    if (objc != 7 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix center_x center_y radius color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &radius) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[5], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 9) {
        if (Tcl_GetIntFromObj(interp, objv[7], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &shift) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::circle(*mat, cv::Point(x1, y1), radius,
                   color, thickness, lineType, shift);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int clipLine(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int count = 0;
    int width, height;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    bool result;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "size_list point1_list point2_list");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 2) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid size data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[1], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &width) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[1], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &height) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 2) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[2], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &x1) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[2], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &y1) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 2) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &x2) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &y2) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Point point1(x1, y1);
        cv::Point point2(x2, y2);
        result = cv::clipLine(cv::Size(width, height), point1, point2);
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

    return TCL_OK;
}


int drawMarker(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int count = 0;
    int x1 = 0, y1 = 0;
    int B = 0, G = 0, R = 0, A = 0;
    int markerType = cv::MARKER_CROSS, markerSize = 20, thickness = 1, line_type = cv::LINE_8;

    if (objc != 5 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_x point_y color_list ?markerType markerSize thickness line_type?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

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

    if (objc == 9) {
        if (Tcl_GetIntFromObj(interp, objv[5], &markerType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &markerSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &thickness) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &line_type) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::drawMarker(*mat, cv::Point(x1, y1), color,
                       markerType, markerSize, thickness, line_type);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int ellipse(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int count = 0;
    int x1 = 0, y1 = 0, width = 0, height;
    int B = 0, G = 0, R = 0, A = 0;
    double angle = 0.0, startAngle = 0.0, endAngle = 0.0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;

    if (objc != 11 && objc != 13) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix center_x center_y width hgieht angle startAngle endAngle color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[6], &angle) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[7], &startAngle) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[8], &endAngle) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[9], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[9], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[9], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[9], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[9], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[10], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 13) {
        if (Tcl_GetIntFromObj(interp, objv[11], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[12], &shift) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::ellipse(*mat, cv::Point(x1, y1), cv::Size(width, height),
                    angle, startAngle, endAngle, color,
                    thickness, lineType, shift);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int fillConvexPoly(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    int lineType = cv::LINE_8, shift = 0;
    cv::Point *pts;
    int npts = 0;

    if (objc != 4 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "matrix point_list color_list ?lineType shift?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;

        npts = count / 2;
        pts = (cv::Point *) ckalloc(sizeof(cv::Point) * npts);

        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                ckfree(pts);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                ckfree(pts);
                return TCL_ERROR;
            }

            pts[j].x = number_from_list_x;
            pts[j].y = number_from_list_y;
        }
    }

    if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        ckfree(pts);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        ckfree(pts);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[3], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[4], &lineType) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &shift) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::fillConvexPoly(*mat,  (const cv::Point *) pts, npts,
                           color, lineType, shift);
    } catch (const cv::Exception &ex) {
        ckfree(pts);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        ckfree(pts);
        return Opencv_Exc2Tcl(interp, NULL);
    }

    ckfree(pts);
    return TCL_OK;
}


int fillPoly(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    int lineType = cv::LINE_8, shift = 0, offset_x = 0, offset_y = 0;
    cv::Point **pts = NULL;
    int *npts = NULL;
    int ncontours = 0;
    int isDone = 0;

    if (objc != 4 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_lists color_list ?lineType shift offset_x offset_y?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &ncontours) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid lists");
    }

    if (ncontours == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        pts = (cv::Point **) ckalloc(sizeof(cv::Point *) * ncontours);
        npts = (int *) ckalloc(sizeof(int) * ncontours);
        Tcl_Obj *elemPointListPtr = NULL;
        int count = 0;

        for (int number = 0; number < ncontours; number++) {
            pts[number] = NULL;
        }

        for (int number = 0; number < ncontours; number++) {
            Tcl_ListObjIndex(interp, objv[2], number, &elemPointListPtr);
            if (Tcl_ListObjLength(interp, elemPointListPtr, &count) != TCL_OK) {
                Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list");
                goto End;
            }

            if (count%2 != 0) {
                Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
                goto End;
            } else {
                Tcl_Obj *elemListPtr = NULL;
                int number_from_list_x;
                int number_from_list_y;

                npts[number] = count / 2;
                pts[number] = (cv::Point *) ckalloc(sizeof(cv::Point) * npts[number]);

                for (int i = 0, j = 0; j < npts[number]; i = i + 2, j = j + 1) {
                    Tcl_ListObjIndex(interp, elemPointListPtr, i, &elemListPtr);
                    if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                        goto End;
                    }

                    Tcl_ListObjIndex(interp, elemPointListPtr, i + 1, &elemListPtr);
                    if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                        goto End;
                    }

                    pts[number][j].x = number_from_list_x;
                    pts[number][j].y = number_from_list_y;
                }
            }
        }
    }

    if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
        Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
        goto End;
    }

    if (count != 4) {
        Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");

        goto End;
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[3], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            goto End;
        }

        Tcl_ListObjIndex(interp, objv[3], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            goto End;
        }

        Tcl_ListObjIndex(interp, objv[3], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            goto End;
        }

        Tcl_ListObjIndex(interp, objv[3], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            goto End;
        }
    }

    if (objc == 8) {
        if (Tcl_GetIntFromObj(interp, objv[4], &lineType) != TCL_OK) {
            goto End;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &shift) != TCL_OK) {
            goto End;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &offset_x) != TCL_OK) {
            goto End;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &offset_y) != TCL_OK) {
            goto End;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::fillPoly(*mat,  (const cv::Point **) pts, (const int *) npts,
                     ncontours, color, lineType, shift,
                     cv::Point(offset_x, offset_y));

        isDone = 1;
    } catch (const cv::Exception &ex) {
        Opencv_Exc2Tcl(interp, &ex);

        isDone = 0;
        goto End;
    } catch (...) {
        Opencv_Exc2Tcl(interp, NULL);

        isDone = 0;
        goto End;
    }

End:
    if (pts) {
        for (int index = 0; index < ncontours; index++) {
            if (pts[index]) {
                ckfree(pts[index]);
            }
        }

        ckfree(pts);
    }

    if (npts) {
        ckfree(npts);
    }

    if (isDone) {
        return TCL_OK;
    } else {
        return TCL_ERROR;
    }
}


int line(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int count = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int B = 0, G = 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;

    if (objc != 8 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_x1 point_y1 point_x2 point_y2 color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[6], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[7], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 10) {
        if (Tcl_GetIntFromObj(interp, objv[8], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[9], &shift) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::line(*mat, cv::Point(x1, y1), cv::Point(x2, y2),
                 color, thickness, lineType, shift);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


int polylines(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int ncontours = 0, isClosed = 0;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;
    cv::Point *pts;
    int npts = 0;

    if (objc != 7 && objc != 9) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_list ncontours isClosed color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count%2 != 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid point data");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int number_from_list_x;
        int number_from_list_y;

        npts = count / 2;
        pts = (cv::Point *) ckalloc(sizeof(cv::Point) * npts);

        for (int i = 0, j = 0; j < npts; i = i + 2, j = j + 1) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_x) != TCL_OK) {
                ckfree(pts);
                return TCL_ERROR;
            }

            Tcl_ListObjIndex(interp, objv[2], i + 1, &elemListPtr);
            if (Tcl_GetIntFromObj(interp, elemListPtr, &number_from_list_y) != TCL_OK) {
                ckfree(pts);
                return TCL_ERROR;
            }

            pts[j].x = number_from_list_x;
            pts[j].y = number_from_list_y;
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &ncontours) != TCL_OK) {
        ckfree(pts);
        return TCL_ERROR;
    }

    if (Tcl_GetBooleanFromObj(interp, objv[4], &isClosed) != TCL_OK) {
        ckfree(pts);
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[5], &count) != TCL_OK) {
        ckfree(pts);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        ckfree(pts);
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[5], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &thickness) != TCL_OK) {
        ckfree(pts);
        return TCL_ERROR;
    }

    if (objc == 9) {
        if (Tcl_GetIntFromObj(interp, objv[7], &lineType) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[8], &shift) != TCL_OK) {
            ckfree(pts);
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::polylines(*mat,  (const cv::Point* const*) &pts, (const int*) &npts,
                      ncontours, (bool) isClosed, color,
                      thickness, lineType, shift);
    } catch (const cv::Exception &ex) {
        ckfree(pts);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        ckfree(pts);
        return Opencv_Exc2Tcl(interp, NULL);
    }

    ckfree(pts);
    return TCL_OK;
}


int putText(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    char *text = NULL;
    int len = 0;
    int count = 0;
    int x1 = 0, y1 = 0, fontFace = 0, B = 0, G = 0, R = 0, A = 0;
    double fontScale = 1.0;
    int thickness = 1, lineType = cv::LINE_8, bottomLeftOrigin = 0;
    Tcl_DString ds;
    Tcl_Encoding enc;

    if (objc != 9 && objc != 11) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix text point_x point_y fontFace fontScale color_list thickness ?lineType bottomLeftOrigin?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    text = Tcl_GetStringFromObj(objv[2], &len);

    if (Tcl_GetIntFromObj(interp, objv[3], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &fontFace) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[6], &fontScale) != TCL_OK) {
        return TCL_ERROR;
    }

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

    if (Tcl_GetIntFromObj(interp, objv[8], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 11) {
        if (Tcl_GetIntFromObj(interp, objv[9], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[10], &bottomLeftOrigin) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    enc = Tcl_GetEncoding(NULL, "utf-8");
    text = Tcl_UtfToExternalDString(enc, text, len, &ds);
    Tcl_FreeEncoding(enc);
    try {
        cv::Scalar color(B, G, R, A);

        cv::putText(*mat, text, cv::Point(x1, y1), fontFace, fontScale,
                    color, thickness, lineType, (bool) bottomLeftOrigin);
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


int rectangle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat *mat;
    int count = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    int B = 0, G= 0, R = 0, A = 0;
    int thickness = 1, lineType = cv::LINE_8, shift = 0;

    if (objc != 8 && objc != 10) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix point_x1 point_y1 point_x2 point_y2 color_list thickness ?lineType shift?");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &x1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &y1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &x2) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &y2) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[6], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count != 4) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid color data");
    } else {
        Tcl_Obj *elemListPtr = NULL;

        Tcl_ListObjIndex(interp, objv[6], 0, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 1, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 2, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[6], 3, &elemListPtr);
        if (Tcl_GetIntFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_GetIntFromObj(interp, objv[7], &thickness) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 10) {
        if (Tcl_GetIntFromObj(interp, objv[8], &lineType) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[9], &shift) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::Scalar color(B, G, R, A);

        cv::rectangle(*mat, cv::Point(x1, y1), cv::Point(x2, y2),
                      color, thickness, lineType, shift);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    return TCL_OK;
}


static void CLAHE_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->clahe) {
        cvd->clahe.release();
    }
    cvd->cmd_clahe = NULL;
}


static int CLAHE_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
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

    if (cvd->clahe == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_APPLY: {
            cv::Mat *mat, *dstmat;
            cv::Mat result;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->clahe->apply(*mat, result);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(result);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_clahe) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_clahe);
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
            if (cvd->cmd_clahe) {
                Tcl_GetCommandFullName(interp, cvd->cmd_clahe, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::CLAHE", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int CLAHE(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    double clipLimit = 40.0;
    int tileGridSize_w = 8, tileGridSize_h = 8;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::CLAHE> clahe;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?clipLimit tileGridSize_w tileGridSize_h?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetDoubleFromObj(interp, objv[1], &clipLimit) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &tileGridSize_w) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &tileGridSize_h) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        clahe = cv::createCLAHE(clipLimit, cv::Size(tileGridSize_w, tileGridSize_h));
        if (clahe == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "CLAHE nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-clahe", -1);

    if (cvd->cmd_clahe) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_clahe);
    }
    cvd->cmd_clahe =
        Tcl_CreateObjCommand(interp, "::cv-clahe",
            (Tcl_ObjCmdProc *) CLAHE_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) CLAHE_DESTRUCTOR);

    cvd->clahe = clahe;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void GeneralizedHoughBallard_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->houghballard) {
        cvd->houghballard.release();
    }
    cvd->cmd_houghballard = NULL;
}


static int GeneralizedHoughBallard_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detect",
        "getCannyHighThresh",
        "getCannyLowThresh",
        "getDp",
        "getLevels",
        "getMinDist",
        "getVotesThreshold",
        "setCannyHighThresh",
        "setCannyLowThresh",
        "setDp",
        "setLevels",
        "setMinDist",
        "setTemplate",
        "setVotesThreshold",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_detect,
        FUNC_getCannyHighThresh,
        FUNC_getCannyLowThresh,
        FUNC_getDp,
        FUNC_getLevels,
        FUNC_getMinDist,
        FUNC_getVotesThreshold,
        FUNC_setCannyHighThresh,
        FUNC_setCannyLowThresh,
        FUNC_setDp,
        FUNC_setLevels,
        FUNC_setMinDist,
        FUNC_setTemplate,
        FUNC_setVotesThreshold,
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

    if (cvd->houghballard == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_detect: {
            cv::Mat positions, votes;
            cv::Mat *mat, *dstmat1, *dstmat2;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->houghballard->detect(*mat, positions, votes);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat1 = new cv::Mat(positions);

            pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

            dstmat2 = new cv::Mat(votes);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_getCannyHighThresh: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghballard->getCannyHighThresh();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getCannyLowThresh: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghballard->getCannyLowThresh();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDp: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghballard->getDp();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getLevels: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghballard->getLevels();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMinDist: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghballard->getMinDist();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getVotesThreshold: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghballard->getVotesThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_setCannyHighThresh: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghballard->setCannyHighThresh(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setCannyLowThresh: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghballard->setCannyLowThresh(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setDp: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghballard->setDp(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setLevels: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghballard->setLevels(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMinDist: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghballard->setMinDist(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setTemplate: {
            int x = -1, y = -1;
            cv::Mat *mat;

            if (objc != 3 && objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix ?x y?");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (objc == 5) {
                if (Tcl_GetIntFromObj(interp, objv[3], &x) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[4], &y) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->houghballard->setTemplate(*mat, cv::Point(x, y));
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setVotesThreshold: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghballard->setVotesThreshold(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_houghballard) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_houghballard);
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
            if (cvd->cmd_houghballard) {
                Tcl_GetCommandFullName(interp, cvd->cmd_houghballard, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::GeneralizedHoughBallard", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int GeneralizedHoughBallard(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::GeneralizedHoughBallard> houghballard;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        houghballard = cv::createGeneralizedHoughBallard();
        if (houghballard == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "GeneralizedHoughBallard nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-houghballard", -1);

    if (cvd->cmd_houghballard) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_houghballard);
    }
    cvd->cmd_houghballard =
        Tcl_CreateObjCommand(interp, "::cv-houghballard",
            (Tcl_ObjCmdProc *) GeneralizedHoughBallard_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) GeneralizedHoughBallard_DESTRUCTOR);

    cvd->houghballard = houghballard;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void GeneralizedHoughGuil_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->houghbuil) {
        cvd->houghbuil.release();
    }
    cvd->cmd_houghbuil = NULL;
}


static int GeneralizedHoughGuil_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "detect",
        "getAngleEpsilon",
        "getAngleStep",
        "getAngleThresh",
        "getCannyHighThresh",
        "getCannyLowThresh",
        "getDp",
        "getLevels",
        "getMaxAngle",
        "getMaxScale",
        "getMinDist",
        "getMinAngle",
        "getMinScale",
        "getPosThresh",
        "getScaleStep",
        "getScaleThresh",
        "getXi",
        "setAngleEpsilon",
        "setAngleStep",
        "setAngleThresh",
        "setCannyHighThresh",
        "setCannyLowThresh",
        "setDp",
        "setLevels",
        "setMaxAngle",
        "setMaxScale",
        "setMinDist",
        "setMinAngle",
        "setMinScale",
        "setPosThresh",
        "setScaleStep",
        "setScaleThresh",
        "setXi",
        "setTemplate",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_detect,
        FUNC_getAngleEpsilon,
        FUNC_getAngleStep,
        FUNC_getAngleThresh,
        FUNC_getCannyHighThresh,
        FUNC_getCannyLowThresh,
        FUNC_getDp,
        FUNC_getLevels,
        FUNC_getMaxAngle,
        FUNC_getMaxScale,
        FUNC_getMinDist,
        FUNC_getMinAngle,
        FUNC_getMinScale,
        FUNC_getPosThresh,
        FUNC_getScaleStep,
        FUNC_getScaleThresh,
        FUNC_getXi,
        FUNC_setAngleEpsilon,
        FUNC_setAngleStep,
        FUNC_setAngleThresh,
        FUNC_setCannyHighThresh,
        FUNC_setCannyLowThresh,
        FUNC_setDp,
        FUNC_setLevels,
        FUNC_setMaxAngle,
        FUNC_setMaxScale,
        FUNC_setMinDist,
        FUNC_setMinAngle,
        FUNC_setMinScale,
        FUNC_setPosThresh,
        FUNC_setScaleStep,
        FUNC_setScaleThresh,
        FUNC_setXi,
        FUNC_setTemplate,
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

    if (cvd->houghbuil == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_detect: {
            cv::Mat positions, votes;
            cv::Mat *mat, *dstmat1, *dstmat2;
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->detect(*mat, positions, votes);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat1 = new cv::Mat(positions);

            pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

            dstmat2 = new cv::Mat(votes);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_getAngleEpsilon: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getAngleEpsilon();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getAngleStep: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getAngleStep();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getAngleThresh: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getAngleThresh();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getCannyHighThresh: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getCannyHighThresh();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getCannyLowThresh: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getCannyLowThresh();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDp: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getDp();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getLevels: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getLevels();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMaxAngle: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getMaxAngle();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getMaxScale: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getMaxScale();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getMinDist: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getMinDist();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getMinAngle: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getMinAngle();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getMinScale: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getMinScale();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getPosThresh: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getPosThresh();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getScaleStep: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getScaleStep();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getScaleThresh: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getScaleThresh();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getXi: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->houghbuil->getXi();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_setAngleEpsilon: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setAngleEpsilon(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setAngleStep: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setAngleStep(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setAngleThresh: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setAngleThresh(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setCannyHighThresh: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setCannyHighThresh(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setCannyLowThresh: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setCannyLowThresh(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setDp: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setDp(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setLevels: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setLevels(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }

        case FUNC_setMaxAngle: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setMaxAngle(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxScale: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setMaxScale(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMinDist: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setMinDist(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMinAngle: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setMinAngle(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMinScale: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setMinScale(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setPosThresh: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setPosThresh(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setScaleStep: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setScaleStep(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setScaleThresh: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setScaleThresh(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setXi: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->houghbuil->setXi(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setTemplate: {
            int x = -1, y = -1;
            cv::Mat *mat;

            if (objc != 3 && objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix ?x y?");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (objc == 5) {
                if (Tcl_GetIntFromObj(interp, objv[3], &x) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[4], &y) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->houghbuil->setTemplate(*mat, cv::Point(x, y));
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_houghbuil) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_houghbuil);
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
            if (cvd->cmd_houghbuil) {
                Tcl_GetCommandFullName(interp, cvd->cmd_houghbuil, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::GeneralizedHoughGuil", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int GeneralizedHoughGuil(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::GeneralizedHoughGuil> houghbuil;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        houghbuil = cv::createGeneralizedHoughGuil();
        if (houghbuil == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "GeneralizedHoughGuil nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-houghbuil", -1);

    if (cvd->cmd_houghbuil) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_houghbuil);
    }
    cvd->cmd_houghbuil =
        Tcl_CreateObjCommand(interp, "::cv-houghbuil",
            (Tcl_ObjCmdProc *) GeneralizedHoughGuil_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) GeneralizedHoughGuil_DESTRUCTOR);

    cvd->houghbuil = houghbuil;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
