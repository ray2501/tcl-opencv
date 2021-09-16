#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif


static void SVM_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->svm) {
        cvd->svm.release();
    }
    cvd->cmd_svm = NULL;
}


static int SVM_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "getC",
        "getCoef0",
        "getDegree",
        "getGamma",
        "getNu",
        "getP",
        "getType",
        "getKernelType",
        "getDecisionFunction",
        "getSupportVectors",
        "getUncompressedSupportVectors",
        "setC",
        "setCoef0",
        "setDegree",
        "setGamma",
        "setNu",
        "setP",
        "setType",
        "setKernel",
        "setTermCriteria",
        "train",
        "predict",
        "save",
        "close",
        0
    };

    enum FUNC_enum {
        FUNC_getC,
        FUNC_getCoef0,
        FUNC_getDegree,
        FUNC_getGamma,
        FUNC_getNu,
        FUNC_getP,
        FUNC_getType,
        FUNC_getKernelType,
        FUNC_getDecisionFunction,
        FUNC_getSupportVectors,
        FUNC_getUncompressedSupportVectors,
        FUNC_setC,
        FUNC_setCoef0,
        FUNC_setDegree,
        FUNC_setGamma,
        FUNC_setNu,
        FUNC_setP,
        FUNC_setType,
        FUNC_setKernel,
        FUNC_setTermCriteria,
        FUNC_train,
        FUNC_predict,
        FUNC_save,
        FUNC_CLOSE,
    };

    if (objc < 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "SUBCOMMAND ...");
        return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], FUNC_strs, "option", 0, &choice)) {
        return TCL_ERROR;
    }

    if (cvd->svm == nullptr) {
        Tcl_SetResult(interp, (char *) "no svm instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_getC: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getC();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getC failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getCoef0: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getCoef0();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getCoef0 failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getDegree: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getDegree();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getDegree failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getGamma: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getGamma();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getGamma failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getNu: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getNu();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getNu failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getP: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getP();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getP failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getType: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getType();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getKernelType: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getKernelType();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getKernelType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDecisionFunction: {
            cv::Mat alpha, svidx;
            cv::Mat *alphamat, *svidxmat;
            Tcl_Obj *pResultStr = NULL, *pAlphaResultStr = NULL, *pSvidxResultStr = NULL;;
            int index = 0;
            double value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "index");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &index) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                value = cvd->svm->getDecisionFunction(index, alpha, svidx);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getSupportVectors failed", TCL_STATIC);
                return TCL_ERROR;
            }

            alphamat = new cv::Mat(alpha);
            pAlphaResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, alphamat);

            svidxmat = new cv::Mat(svidx);
            pSvidxResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, svidxmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(value));
            Tcl_ListObjAppendElement(NULL, pResultStr, pAlphaResultStr);
            Tcl_ListObjAppendElement(NULL, pResultStr, pSvidxResultStr);
            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getSupportVectors: {
            cv::Mat vectors;
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                vectors = cvd->svm->getSupportVectors();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getSupportVectors failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(vectors);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getUncompressedSupportVectors: {
            cv::Mat vectors;
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                vectors = cvd->svm->getUncompressedSupportVectors();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getUncompressedSupportVectors failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(vectors);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);
            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_setC: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setC(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setC failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setCoef0: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setCoef0(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setCoef0 failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setDegree: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setDegree(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setDegree failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setGamma: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setGamma(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setGamma failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setNu: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setNu(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setNu failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setP: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setP(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setP failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setType: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setType(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setKernel: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setKernel(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setKernel failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setTermCriteria: {
            cv::TermCriteria *termCriteria;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "termCriteria");
                return TCL_ERROR;
            }

            termCriteria = (cv::TermCriteria *) Opencv_FindHandle(cd, interp, OPENCV_TERMCRITERIA, objv[2]);
            if (!termCriteria) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->setTermCriteria(*termCriteria);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setTermCriteria failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_train: {
            cv::Mat *samples, *responses;
            int layout;

           if (objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "samples layout responses");
                return TCL_ERROR;
            }

            samples = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!samples) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &layout) != TCL_OK) {
                return TCL_ERROR;
            }

            responses = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[4]);
            if (!responses) {
                return TCL_ERROR;
            }

            try {
                cvd->svm->train(*samples, layout, *responses);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "train failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
            Tcl_Obj *pResultStr = NULL, *pMatResultStr = NULL;
            int flags = 0;
            float value;

           if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "samples ?flags?");
                return TCL_ERROR;
            }

            samples = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!samples) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                value = cvd->svm->predict(*samples, results, flags);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "predict failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(results);
            pMatResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(value));
            Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr);
            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            int len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (!filename || len < 1) {
                Tcl_SetResult(interp, (char *) "save invalid file name", TCL_STATIC);
                return TCL_ERROR;
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->svm->save(filename);
            } catch (...) {
                Tcl_DStringFree(&ds);
                Tcl_SetResult(interp, (char *) "save failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_DStringFree(&ds);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_svm) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_svm);
            }

            break;
        }
    }

    return TCL_OK;
}


int SVM(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::SVM> svm;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        svm = cv::ml::SVM::create();

        if (svm == nullptr) {
            Tcl_SetResult(interp, (char *) "SVM create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "SVM failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-mlsvm", -1);

    cvd->cmd_svm =
        Tcl_CreateObjCommand(interp, "::cv-mlsvm",
            (Tcl_ObjCmdProc *) SVM_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) SVM_DESTRUCTOR);

    cvd->svm = svm;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


int SVM_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::SVM> svm;
    char *filename = NULL;
    int len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (!filename || len < 1) {
        Tcl_SetResult(interp, (char *) "load invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        svm = cv::ml::SVM::load(filename);

        if (svm == nullptr) {
            Tcl_DStringFree(&ds);
            Tcl_SetResult(interp, (char *) "SVM load failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "SVM load failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mlsvm", -1);

    cvd->cmd_svm =
        Tcl_CreateObjCommand(interp, "::cv-mlsvm",
            (Tcl_ObjCmdProc *) SVM_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) SVM_DESTRUCTOR);

    cvd->svm = svm;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}

#ifdef __cplusplus
}
#endif
