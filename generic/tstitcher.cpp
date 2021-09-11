#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif


static void Stitcher_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->stitcher) {
        cvd->stitcher.release();
    }
    cvd->cmd_stitcher = NULL;
}


static int Stitcher_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "stitch",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_STITCH,
        FUNC_CLOSE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->stitcher == nullptr) {
        Tcl_SetResult(interp, (char *) "no stitcher instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_STITCH: {
            int count = 0;
            std::vector<cv::Mat> imgs;
            cv::Mat result_image;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "image_list");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                Tcl_SetResult(interp, (char *) "stitch invalid list data", TCL_STATIC);
                return TCL_ERROR;
            }

            if (count == 0) {
                Tcl_SetResult(interp, (char *) "stitch empty list", TCL_STATIC);
                return TCL_ERROR;
            } else {
                Tcl_Obj *elem = NULL;
                cv::Mat image;
                cv::Mat *mat;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elem);
                    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, elem);
                    if (!mat) {
                        return TCL_ERROR;
                    }
                    image = *mat;
                    imgs.push_back(image);
                }
            }

            try {
                cv::Stitcher::Status status = cvd->stitcher->stitch(imgs, result_image);

                if (status != cv::Stitcher::OK)
                {
                    Tcl_SetResult(interp, (char *) "stitch can't stitch images", TCL_STATIC);
                    return TCL_ERROR;
                }
            } catch (...) {
                Tcl_SetResult(interp, (char *) "stitch failed", TCL_STATIC);
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

            if (cvd->cmd_stitcher) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_stitcher);
            }

            break;
        }
    }

    return TCL_OK;
}


int Stitcher(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int mode = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::Stitcher> stitcher;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "mode");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &mode) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        stitcher = cv::Stitcher::create((cv::Stitcher::Mode) mode);

        if (stitcher == nullptr) {
            Tcl_SetResult(interp, (char *) "Stitcher create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "Stitcher failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-stitcher", -1);

    cvd->cmd_stitcher =
        Tcl_CreateObjCommand(interp, "::cv-stitcher",
            (Tcl_ObjCmdProc *) Stitcher_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) Stitcher_DESTRUCTOR);

    cvd->stitcher = stitcher;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}

#ifdef __cplusplus
}
#endif
