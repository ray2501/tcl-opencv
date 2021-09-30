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
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_STITCH,
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

    if (cvd->stitcher == nullptr) {
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
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
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty list data");
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

                if (status != cv::Stitcher::OK) {
                    CV_Error(cv::Error::StsError, "Stitcher status not OK");
                }
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_stitcher) {
                Tcl_GetCommandFullName(interp, cvd->cmd_stitcher, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::Stitcher", TCL_STATIC);
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
            CV_Error(cv::Error::StsNullPtr, "Stitcher nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-stitcher", -1);

    if (cvd->cmd_stitcher) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_stitcher);
    }
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
