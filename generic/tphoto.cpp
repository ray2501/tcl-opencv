#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif


int inpaint(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double inpaintRadius = 0;
    int flags = 0;
    cv::Mat dstimage;
    cv::Mat *mat1, *mat2, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix inpaintMask inpaintRadius flags");
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

    if (Tcl_GetDoubleFromObj(interp, objv[3], &inpaintRadius) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::inpaint(*mat1, *mat2, dstimage,
                    inpaintRadius, flags);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int decolor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dstimage1, dstimage2;
    cv::Mat *mat1, *dstmat1, *dstmat2;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    try {
        cv::decolor(*mat1, dstimage1, dstimage2);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_Obj *list[2];

    dstmat1 = new cv::Mat(dstimage1);
    list[0] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

    dstmat2 = new cv::Mat(dstimage2);
    list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

    Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

    return TCL_OK;
}


int fastNlMeansDenoising(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double h = 3;
    int templateWindowSize = 7, searchWindowSize = 21;
    cv::Mat dstimage;
    cv::Mat *mat1, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix ?h templateWindowSize searchWindowSize?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetDoubleFromObj(interp, objv[2], &h) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[3], &templateWindowSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[4], &searchWindowSize) != TCL_OK) {
            return TCL_ERROR;
        }
    }


    try {
        cv::fastNlMeansDenoising(*mat1, dstimage, (float) h,
                                        templateWindowSize, searchWindowSize);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int fastNlMeansDenoisingColored(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double h = 3, hColor = 3;
    int templateWindowSize = 7, searchWindowSize = 21;
    cv::Mat dstimage;
    cv::Mat *mat1, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix ?h hColor templateWindowSize searchWindowSize?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (objc == 6) {
        if (Tcl_GetDoubleFromObj(interp, objv[2], &h) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &hColor) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[4], &templateWindowSize) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[5], &searchWindowSize) != TCL_OK) {
            return TCL_ERROR;
        }
    }


    try {
        cv::fastNlMeansDenoisingColored(*mat1, dstimage,
                                        (float) h, (float) hColor,
                                        templateWindowSize, searchWindowSize);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int colorChange(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double red_mul = 1.0, green_mul = 1.0, blue_mul = 1.0;
    cv::Mat dstimage;
    cv::Mat *mat1, *mat2, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix mask red_mul green_mul blue_mul");
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

    if (Tcl_GetDoubleFromObj(interp, objv[3], &red_mul) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &green_mul) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[5], &blue_mul) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::colorChange(*mat1, *mat2, dstimage,
                        (float) red_mul, (float) green_mul, (float) blue_mul);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int illuminationChange(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double alpha = 0.2, beta = 0.4;
    cv::Mat dstimage;
    cv::Mat *mat1, *mat2, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix mask alpha beta");
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

    if (Tcl_GetDoubleFromObj(interp, objv[3], &alpha) != TCL_OK) {
        return TCL_ERROR;
    }

    if (alpha < 0 || alpha > 2) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "alpha out of range");
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &beta) != TCL_OK) {
        return TCL_ERROR;
    }

    if (beta < 0 || beta > 2) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "beta out of range");
    }

    try {
        cv::illuminationChange(*mat1, *mat2, dstimage,
                        (float) alpha, (float) beta);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int textureFlattening(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double low_threshold = 30, high_threshold = 145;
    int kernel_size = 3;
    cv::Mat dstimage;
    cv::Mat *mat1, *mat2, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "matrix mask low_threshold high_threshold ?kernel_size?");
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

    if (Tcl_GetDoubleFromObj(interp, objv[3], &low_threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (low_threshold < 0 || low_threshold > 100) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "low threshold out of range");
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &high_threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (high_threshold < 100) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "high threshold out of range");
    }

    if (objc == 6) {
        if (Tcl_GetIntFromObj(interp, objv[5], &kernel_size) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::textureFlattening(*mat1, *mat2, dstimage,
                (float) low_threshold, (float) high_threshold, kernel_size);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int seamlessClone(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int x = 0, y = 0, flags = cv::NORMAL_CLONE;
    cv::Mat dstimage;
    cv::Mat *mat1, *mat2, *mat3, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "src_matrix dst_matrix mask x y flags");
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

    /*
     * Location of the center of the source image in the destination image.
     * Get x location.
     */
    if (Tcl_GetIntFromObj(interp, objv[4], &x) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Location of the center of the source image in the destination image.
     * Get y location.
     */
    if (Tcl_GetIntFromObj(interp, objv[5], &y) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &flags) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::seamlessClone(*mat1, *mat2, *mat3,
                          cv::Point(x, y), dstimage, flags);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int detailEnhance(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double sigma_s = 10, sigma_r = 0.15;
    cv::Mat dstimage;
    cv::Mat *mat1, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?sigma_s sigma_r?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetDoubleFromObj(interp, objv[2], &sigma_s) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &sigma_r) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::detailEnhance(*mat1, dstimage, sigma_s, sigma_r);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int edgePreservingFilter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int flags = 1;
    double sigma_s = 60, sigma_r = 0.4;
    cv::Mat dstimage;
    cv::Mat *mat1, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?flags sigma_s sigma_r?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetIntFromObj(interp, objv[2], &flags) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &sigma_s) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[4], &sigma_r) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::edgePreservingFilter(*mat1, dstimage, flags, (float) sigma_s, (float) sigma_r);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int pencilSketch(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double sigma_s = 60, sigma_r = 0.07, shade_factor = 0.02;
    cv::Mat dstimage1, dstimage2;
    cv::Mat *mat1, *dstmat1, *dstmat2;

    if (objc != 2 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?sigma_s sigma_r shade_factor?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetDoubleFromObj(interp, objv[2], &sigma_s) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &sigma_r) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[4], &shade_factor) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::pencilSketch(*mat1, dstimage1, dstimage2,
                         (float) sigma_s, (float) sigma_r, (float) shade_factor);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_Obj *list[2];

    dstmat1 = new cv::Mat(dstimage1);
    list[0] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

    dstmat2 = new cv::Mat(dstimage2);
    list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

    Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

    return TCL_OK;
}


int stylization(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    double sigma_s = 60, sigma_r = 0.45;
    cv::Mat dstimage;
    cv::Mat *mat1, *dstmat;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "matrix ?sigma_s sigma_r?");
        return TCL_ERROR;
    }

    mat1 = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat1) {
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetDoubleFromObj(interp, objv[2], &sigma_s) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &sigma_r) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::stylization(*mat1, dstimage, sigma_s, sigma_r);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    dstmat = new cv::Mat(dstimage);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


static void AlignMTB_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->alignmtb) {
        cvd->alignmtb.release();
    }
    cvd->cmd_alignmtb = NULL;
}


static int AlignMTB_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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

    if (cvd->alignmtb == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_PROCESS: {
            std::vector<cv::Mat> src;
            std::vector<cv::Mat> dst;
            cv::Mat image;
            int count;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix_list");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0 || count%2 != 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid matrix list data");
            } else {
                Tcl_Obj *elem = NULL;
                cv::Mat *mat;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elem);
                    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, elem);
                    if (!mat) {
                        return TCL_ERROR;
                    }
                    image = *mat;
                    src.push_back(image);
                }
            }

            try {
                cvd->alignmtb->process(src, dst);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(dst.size(), NULL);
            for (size_t i = 0; i < dst.size(); i++) {
                Tcl_Obj *pSubResultStr = NULL;
                cv::Mat *dstmat;

                dstmat = new cv::Mat(dst[i]);

                pSubResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

                Tcl_ListObjAppendElement(NULL, pResultStr, pSubResultStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_alignmtb) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_alignmtb);
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
            if (cvd->cmd_alignmtb) {
                Tcl_GetCommandFullName(interp, cvd->cmd_alignmtb, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::AlignMTB", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int AlignMTB(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int max_bits = 6, exclude_range = 4, cut = 1;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::AlignMTB> alignmtb;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?max_bits exclude_range cut?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[1], &max_bits) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[2], &exclude_range) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[3], &cut) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        alignmtb = cv::createAlignMTB(max_bits, exclude_range, (bool) cut);

        if (alignmtb == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "AlignMTB nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-alignmtb", -1);

    if (cvd->cmd_alignmtb) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_alignmtb);
    }
    cvd->cmd_alignmtb =
        Tcl_CreateObjCommand(interp, "::cv-alignmtb",
            (Tcl_ObjCmdProc *) AlignMTB_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) AlignMTB_DESTRUCTOR);

    cvd->alignmtb = alignmtb;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void CalibrateDebevec_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->calibdeb) {
        cvd->calibdeb.release();
    }
    cvd->cmd_calibdeb = NULL;
}


static int CalibrateDebevec_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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

    if (cvd->calibdeb == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_PROCESS: {
            int count;
            cv::Mat image, responseDebevec;
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;
            std::vector<cv::Mat> src;
            std::vector<float> timesArray;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix_list times_list");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid matrix list data");
            } else {
                Tcl_Obj *elem = NULL;
                cv::Mat *mat;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elem);
                    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, elem);
                    if (!mat) {
                        return TCL_ERROR;
                    }
                    image = *mat;
                    src.push_back(image);
                }
            }

            if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid times list data");
            } else {
                Tcl_Obj *elemListPtr = NULL;
                double value = 0;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);
                    if (Tcl_GetDoubleFromObj(interp, elemListPtr, &value) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    timesArray.push_back((float) value);
                }
            }

            try {
                cvd->calibdeb->process(src, responseDebevec, timesArray);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(responseDebevec);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_calibdeb) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_calibdeb);
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
            if (cvd->cmd_calibdeb) {
                Tcl_GetCommandFullName(interp, cvd->cmd_calibdeb, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::CalibrateDebevec", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int CalibrateDebevec(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int samples = 70, random = 0;
    double lambda = 10.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::CalibrateDebevec> calibdeb;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?samples lambda random?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetIntFromObj(interp, objv[1], &samples) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[2], &lambda) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[3], &random) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        calibdeb = cv::createCalibrateDebevec(samples, (float) lambda, (bool) random);

        if (calibdeb == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "CalibrateDebevec nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-calibdeb", -1);

    if (cvd->cmd_calibdeb) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_calibdeb);
    }
    cvd->cmd_calibdeb =
        Tcl_CreateObjCommand(interp, "::cv-calibdeb",
            (Tcl_ObjCmdProc *) CalibrateDebevec_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) CalibrateDebevec_DESTRUCTOR);

    cvd->calibdeb = calibdeb;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void MergeDebevec_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->mergedeb) {
        cvd->mergedeb.release();
    }
    cvd->cmd_mergedeb = NULL;
}


static int MergeDebevec_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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

    if (cvd->mergedeb == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_PROCESS: {
            int count;
            cv::Mat image, dst;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL;
            std::vector<cv::Mat> src;
            std::vector<float> timesArray;

            if (objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix_list times_list response");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid matrix list data");
            } else {
                Tcl_Obj *elem = NULL;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elem);
                    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, elem);
                    if (!mat) {
                        return TCL_ERROR;
                    }
                    image = *mat;
                    src.push_back(image);
                }
            }

            if (Tcl_ListObjLength(interp, objv[3], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid times list data");
            } else {
                Tcl_Obj *elemListPtr = NULL;
                double value = 0;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[3], i, &elemListPtr);
                    if (Tcl_GetDoubleFromObj(interp, elemListPtr, &value) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    timesArray.push_back((float) value);
                }
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[4]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->mergedeb->process(src, dst, timesArray, *mat);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(dst);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_mergedeb) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_mergedeb);
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
            if (cvd->cmd_mergedeb) {
                Tcl_GetCommandFullName(interp, cvd->cmd_mergedeb, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::MergeDebevec", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int MergeDebevec(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::MergeDebevec> mergedeb;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        mergedeb = cv::createMergeDebevec();

        if (mergedeb == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "MergeDebevec nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-mergedeb", -1);

    if (cvd->cmd_mergedeb) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_mergedeb);
    }
    cvd->cmd_mergedeb =
        Tcl_CreateObjCommand(interp, "::cv-mergedeb",
            (Tcl_ObjCmdProc *) MergeDebevec_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) MergeDebevec_DESTRUCTOR);

    cvd->mergedeb = mergedeb;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void MergeMertens_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->mergemer) {
        cvd->mergemer.release();
    }
    cvd->cmd_mergemer = NULL;
}


static int MergeMertens_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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

    if (cvd->mergemer == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_PROCESS: {
            int count;
            cv::Mat image, dst;
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;
            std::vector<cv::Mat> src;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix_list");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid matrix list data");
            } else {
                Tcl_Obj *elem = NULL;
                cv::Mat *mat;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elem);
                    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, elem);
                    if (!mat) {
                        return TCL_ERROR;
                    }
                    image = *mat;
                    src.push_back(image);
                }
            }

            try {
                cvd->mergemer->process(src, dst);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(dst);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_mergemer) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_mergemer);
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
            if (cvd->cmd_mergemer) {
                Tcl_GetCommandFullName(interp, cvd->cmd_mergemer, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::MergeMertens", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int MergeMertens(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    double contrast_weight = 1.0, saturation_weight = 1.0, exposure_weight = 0.0;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::MergeMertens> mergemer;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?contrast_weight saturation_weight exposure_weight?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetDoubleFromObj(interp, objv[1], &contrast_weight) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[2], &saturation_weight) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &exposure_weight) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        mergemer = cv::createMergeMertens((float) contrast_weight,
                                          (float) saturation_weight,
                                          (float) exposure_weight);

        if (mergemer == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "MergeMertens nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-mergemer", -1);

    if (cvd->cmd_mergemer) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_mergemer);
    }
    cvd->cmd_mergemer =
        Tcl_CreateObjCommand(interp, "::cv-mergemer",
            (Tcl_ObjCmdProc *) MergeMertens_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) MergeMertens_DESTRUCTOR);

    cvd->mergemer = mergemer;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void TonemapDrago_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->tonemapdra) {
        cvd->tonemapdra.release();
    }
    cvd->cmd_tonemapdra = NULL;
}


static int TonemapDrago_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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

    if (cvd->tonemapdra == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_PROCESS: {
            cv::Mat image, dst;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "hdrDebevec");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->tonemapdra->process(*mat, dst);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(dst);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_tonemapdra) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_tonemapdra);
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
            if (cvd->cmd_tonemapdra) {
                Tcl_GetCommandFullName(interp, cvd->cmd_tonemapdra, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::TonemapDrago", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int TonemapDrago(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    double gamma = 1.0, saturation = 1.0, bias = 0.85;
    cv::Ptr<cv::TonemapDrago> tonemapdra;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?gamma saturation bias?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetDoubleFromObj(interp, objv[1], &gamma) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[2], &saturation) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &bias) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        tonemapdra = cv::createTonemapDrago(gamma, saturation, bias);

        if (tonemapdra == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "TonemapDrago nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-tonemapdra", -1);

    if (cvd->cmd_tonemapdra) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_tonemapdra);
    }
    cvd->cmd_tonemapdra =
        Tcl_CreateObjCommand(interp, "::cv-tonemapdra",
            (Tcl_ObjCmdProc *) TonemapDrago_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) TonemapDrago_DESTRUCTOR);

    cvd->tonemapdra = tonemapdra;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void TonemapMantiuk_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->tonemapman) {
        cvd->tonemapman.release();
    }
    cvd->cmd_tonemapman = NULL;
}


static int TonemapMantiuk_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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

    if (cvd->tonemapman == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_PROCESS: {
            cv::Mat image, dst;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "hdrDebevec");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->tonemapman->process(*mat, dst);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(dst);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_tonemapman) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_tonemapman);
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
            if (cvd->cmd_tonemapman) {
                Tcl_GetCommandFullName(interp, cvd->cmd_tonemapman, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::TonemapMantiuk", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int TonemapMantiuk(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    double gamma = 1.0, scale = 0.7, saturation = 1.0;
    cv::Ptr<cv::TonemapMantiuk> tonemapman;

    if (objc != 1 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "?gamma scale saturation?");
        return TCL_ERROR;
    }

    if (objc == 4) {
        if (Tcl_GetDoubleFromObj(interp, objv[1], &gamma) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[2], &scale) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &saturation) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        tonemapman = cv::createTonemapMantiuk((float) gamma, (float) scale, (float) saturation);

        if (tonemapman == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "TonemapMantiuk nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-tonemapman", -1);

    if (cvd->cmd_tonemapman) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_tonemapman);
    }
    cvd->cmd_tonemapman =
        Tcl_CreateObjCommand(interp, "::cv-tonemapman",
            (Tcl_ObjCmdProc *) TonemapMantiuk_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) TonemapMantiuk_DESTRUCTOR);

    cvd->tonemapman = tonemapman;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void TonemapReinhard_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->tonemaprei) {
        cvd->tonemaprei.release();
    }
    cvd->cmd_tonemaprei = NULL;
}


static int TonemapReinhard_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "process",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_PROCESS,
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

    if (cvd->tonemaprei == nullptr) {
        Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_PROCESS: {
            cv::Mat image, dst;
            cv::Mat *mat, *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "hdrDebevec");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                cvd->tonemaprei->process(*mat, dst);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(dst);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_tonemaprei) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_tonemaprei);
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
            if (cvd->cmd_tonemaprei) {
                Tcl_GetCommandFullName(interp, cvd->cmd_tonemaprei, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::TonemapReinhard", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int TonemapReinhard(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    double gamma = 1.0, intensity = 0.0, light_adapt = 1.0, color_adapt = 0.0;
    cv::Ptr<cv::TonemapReinhard> tonemaprei;

    if (objc != 1 && objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "?gamma intensity light_adapt color_adapt?");
        return TCL_ERROR;
    }

    if (objc == 5) {
        if (Tcl_GetDoubleFromObj(interp, objv[1], &gamma) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[2], &intensity) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[3], &light_adapt) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[4], &color_adapt) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        tonemaprei = cv::createTonemapReinhard((float) gamma, (float) intensity,
                                               (float) light_adapt, (float) color_adapt);

        if (tonemaprei == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "TonemapReinhard nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-tonemaprei", -1);

    if (cvd->cmd_tonemaprei) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_tonemaprei);
    }
    cvd->cmd_tonemaprei =
        Tcl_CreateObjCommand(interp, "::cv-tonemaprei",
            (Tcl_ObjCmdProc *) TonemapReinhard_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) TonemapReinhard_DESTRUCTOR);

    cvd->tonemaprei = tonemaprei;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
