#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif


static void Logistic_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->logistic) {
        cvd->logistic.release();
    }
    cvd->cmd_logistic = NULL;
}


static int Logistic_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "get_learnt_thetas",
        "getIterations",
        "getLearningRate",
        "getMiniBatchSize",
        "getRegularization",
        "getTrainMethod",
        "setIterations",
        "setLearningRate",
        "setMiniBatchSize",
        "setRegularization",
        "setTrainMethod",
        "setTermCriteria",
        "train",
        "predict",
        "save",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_get_learnt_thetas,
        FUNC_getIterations,
        FUNC_getLearningRate,
        FUNC_getMiniBatchSize,
        FUNC_getRegularization,
        FUNC_getTrainMethod,
        FUNC_setIterations,
        FUNC_setLearningRate,
        FUNC_setMiniBatchSize,
        FUNC_setRegularization,
        FUNC_setTrainMethod,
        FUNC_setTermCriteria,
        FUNC_train,
        FUNC_predict,
        FUNC_save,
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

    if (cvd->logistic == nullptr) {
        Tcl_SetResult(interp, (char *) "no logistic instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_get_learnt_thetas: {
            cv::Mat value;
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->logistic->get_learnt_thetas();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "get_learnt_thetas failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(value);
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getIterations: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->logistic->getIterations();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getIterations failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getLearningRate: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->logistic->getLearningRate();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getLearningRate failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getMiniBatchSize: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->logistic->getMiniBatchSize();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getMiniBatchSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getRegularization: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->logistic->getRegularization();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getRegularization failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getTrainMethod: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->logistic->getTrainMethod();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getTrainMethod failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_setIterations: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->logistic->setIterations(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setIterations failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setLearningRate: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->logistic->setLearningRate(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setLearningRate failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setMiniBatchSize: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->logistic->setMiniBatchSize(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setMiniBatchSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setRegularization: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->logistic->setRegularization(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setRegularization failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setTrainMethod: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->logistic->setTrainMethod(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setTrainMethod failed", TCL_STATIC);
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
                cvd->logistic->setTermCriteria(*termCriteria);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setTermCriteria failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr< cv::ml::TrainData > trainData;
            char *command = NULL;
            int len = 0, flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "train invalid trainData", TCL_STATIC);
                return TCL_ERROR;
            }

            if (strcmp(command, "::cv-mltraindata")==0) {
                trainData = cvd->traindata;
            }

            if (objc == 4) {
                if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->logistic->train(trainData, flags);
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
                value = cvd->logistic->predict(*samples, results, flags);
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
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "save invalid file name", TCL_STATIC);
                return TCL_ERROR;
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->logistic->save(filename);
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

            if (cvd->cmd_logistic) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_logistic);
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
            if (cvd->cmd_logistic) {
                Tcl_GetCommandFullName(interp, cvd->cmd_logistic, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::LogisticRegression", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int LogisticRegression(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::LogisticRegression> logistic;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        logistic = cv::ml::LogisticRegression::create();

        if (logistic == nullptr) {
            Tcl_SetResult(interp, (char *) "LogisticRegression create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "LogisticRegression failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-mllogistic", -1);

    if (cvd->cmd_logistic) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_logistic);
    }
    cvd->cmd_logistic =
        Tcl_CreateObjCommand(interp, "::cv-mllogistic",
            (Tcl_ObjCmdProc *) Logistic_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) Logistic_DESTRUCTOR);

    cvd->logistic = logistic;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int LogisticRegression_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::LogisticRegression> logistic;
    char *filename = NULL;
    int len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "load invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        logistic = cv::ml::LogisticRegression::load(filename);

        if (logistic == nullptr) {
            Tcl_DStringFree(&ds);
            Tcl_SetResult(interp, (char *) "LogisticRegression load failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "LogisticRegression load failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mllogistic", -1);

    if (cvd->cmd_logistic) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_logistic);
    }
    cvd->cmd_logistic =
        Tcl_CreateObjCommand(interp, "::cv-mllogistic",
            (Tcl_ObjCmdProc *) Logistic_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) Logistic_DESTRUCTOR);

    cvd->logistic = logistic;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif


static void BayesClassifier_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->bayesclassifier) {
        cvd->bayesclassifier.release();
    }
    cvd->cmd_bayesclassifier = NULL;
}


static int BayesClassifier_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "train",
        "predict",
        "predictProb",
        "save",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_train,
        FUNC_predict,
        FUNC_predictProb,
        FUNC_save,
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

    if (cvd->bayesclassifier == nullptr) {
        Tcl_SetResult(interp, (char *) "no bayesclassifier instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_train: {
            cv::Ptr< cv::ml::TrainData > trainData;
            char *command = NULL;
            int len = 0, flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "train invalid trainData", TCL_STATIC);
                return TCL_ERROR;
            }

            if (strcmp(command, "::cv-mltraindata")==0) {
                trainData = cvd->traindata;
            }

            if (objc == 4) {
                if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->bayesclassifier->train(trainData, flags);
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
                value = cvd->bayesclassifier->predict(*samples, results, flags);
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
        case FUNC_predictProb: {
            cv::Mat *samples, *dstmat1, *dstmat2;
            cv::Mat outputs, outputProbs;
            Tcl_Obj *pResultStr = NULL, *pMatResultStr1 = NULL, *pMatResultStr2 = NULL;
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
                value = cvd->bayesclassifier->predictProb(*samples, outputs, outputProbs, flags);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "predictProb failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat1 = new cv::Mat(outputs);
            pMatResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

            dstmat2 = new cv::Mat(outputProbs);
            pMatResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(value));
            Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pMatResultStr2);
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
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "save invalid file name", TCL_STATIC);
                return TCL_ERROR;
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->bayesclassifier->save(filename);
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

            if (cvd->cmd_bayesclassifier) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_bayesclassifier);
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
            if (cvd->cmd_bayesclassifier) {
                Tcl_GetCommandFullName(interp, cvd->cmd_bayesclassifier, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::NormalBayesClassifier", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int NormalBayesClassifier(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::NormalBayesClassifier> bayesclassifier;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        bayesclassifier = cv::ml::NormalBayesClassifier::create();

        if (bayesclassifier == nullptr) {
            Tcl_SetResult(interp, (char *) "NormalBayesClassifier create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "NormalBayesClassifier failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-mlbayesclassifier", -1);

    if (cvd->cmd_bayesclassifier) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_bayesclassifier);
    }
    cvd->cmd_bayesclassifier =
        Tcl_CreateObjCommand(interp, "::cv-mlbayesclassifier",
            (Tcl_ObjCmdProc *) BayesClassifier_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) BayesClassifier_DESTRUCTOR);

    cvd->bayesclassifier = bayesclassifier;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int NormalBayesClassifier_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::NormalBayesClassifier> bayesclassifier;
    char *filename = NULL;
    int len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "load invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        bayesclassifier = cv::ml::NormalBayesClassifier::load(filename);

        if (bayesclassifier == nullptr) {
            Tcl_DStringFree(&ds);
            Tcl_SetResult(interp, (char *) "NormalBayesClassifier load failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "NormalBayesClassifier load failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mlbayesclassifier", -1);

    if (cvd->cmd_bayesclassifier) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_bayesclassifier);
    }
    cvd->cmd_bayesclassifier =
        Tcl_CreateObjCommand(interp, "::cv-mlbayesclassifier",
            (Tcl_ObjCmdProc *) BayesClassifier_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) BayesClassifier_DESTRUCTOR);

    cvd->bayesclassifier = bayesclassifier;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif


static void KNearest_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->knearest) {
        cvd->knearest.release();
    }
    cvd->cmd_knearest = NULL;
}


static int KNearest_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "getAlgorithmType",
        "getDefaultK",
        "getEmax",
        "getIsClassifier",
        "setAlgorithmType",
        "setDefaultK",
        "setEmax",
        "setIsClassifier",
        "findNearest",
        "train",
        "predict",
        "save",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_getAlgorithmType,
        FUNC_getDefaultK,
        FUNC_getEmax,
        FUNC_getIsClassifier,
        FUNC_setAlgorithmType,
        FUNC_setDefaultK,
        FUNC_setEmax,
        FUNC_setIsClassifier,
        FUNC_findNearest,
        FUNC_train,
        FUNC_predict,
        FUNC_save,
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

    if (cvd->knearest == nullptr) {
        Tcl_SetResult(interp, (char *) "no knearest instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_getAlgorithmType: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->knearest->getAlgorithmType();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getAlgorithmType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDefaultK: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->knearest->getDefaultK();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getDefaultK failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getEmax: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->knearest->getEmax();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getEmax failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getIsClassifier: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->knearest->getIsClassifier();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getIsClassifier failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj(value));
            break;
        }
        case FUNC_setAlgorithmType: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->knearest->setAlgorithmType(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setAlgorithmType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setDefaultK: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->knearest->setDefaultK(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setDefaultK failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setEmax: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->knearest->setEmax(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setEmax failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setIsClassifier: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->knearest->setIsClassifier( (bool) value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setIsClassifier failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_findNearest: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
            Tcl_Obj *pResultStr = NULL, *pMatResultStr = NULL;
            int k = 0;
            float value;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "samples k");
                return TCL_ERROR;
            }

            samples = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!samples) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &k) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                value = cvd->knearest->findNearest(*samples, k, results);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "findNearest failed", TCL_STATIC);
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
        case FUNC_train: {
            cv::Ptr< cv::ml::TrainData > trainData;
            char *command = NULL;
            int len = 0, flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "train invalid trainData", TCL_STATIC);
                return TCL_ERROR;
            }

            if (strcmp(command, "::cv-mltraindata")==0) {
                trainData = cvd->traindata;
            }

            if (objc == 4) {
                if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->knearest->train(trainData, flags);
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
                value = cvd->knearest->predict(*samples, results, flags);
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
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "save invalid file name", TCL_STATIC);
                return TCL_ERROR;
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->knearest->save(filename);
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

            if (cvd->cmd_knearest) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_knearest);
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
            if (cvd->cmd_knearest) {
                Tcl_GetCommandFullName(interp, cvd->cmd_knearest, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::KNearest", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int KNearest(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::KNearest> knearest;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        knearest = cv::ml::KNearest::create();

        if (knearest == nullptr) {
            Tcl_SetResult(interp, (char *) "KNearest create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "KNearest failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-mlknearest", -1);

    if (cvd->cmd_knearest) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_knearest);
    }
    cvd->cmd_knearest =
        Tcl_CreateObjCommand(interp, "::cv-mlknearest",
            (Tcl_ObjCmdProc *) KNearest_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) KNearest_DESTRUCTOR);

    cvd->knearest = knearest;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int KNearest_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::KNearest> knearest;
    char *filename = NULL;
    int len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "load invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        knearest = cv::ml::KNearest::load(filename);

        if (knearest == nullptr) {
            Tcl_DStringFree(&ds);
            Tcl_SetResult(interp, (char *) "KNearest load failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "KNearest load failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mlknearest", -1);

    if (cvd->cmd_knearest) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_knearest);
    }
    cvd->cmd_knearest =
        Tcl_CreateObjCommand(interp, "::cv-mlknearest",
            (Tcl_ObjCmdProc *) KNearest_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) KNearest_DESTRUCTOR);

    cvd->knearest = knearest;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
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
        "_command",
        "_name",
        "_type",
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
            cv::Ptr< cv::ml::TrainData > trainData;
            char *command = NULL;
            int len = 0, flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "train invalid trainData", TCL_STATIC);
                return TCL_ERROR;
            }

            if (strcmp(command, "::cv-mltraindata")==0) {
                trainData = cvd->traindata;
            }

            if (objc == 4) {
                if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->svm->train(trainData, flags);
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
            if (len < 1) {
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
        case FUNC__COMMAND:
        case FUNC__NAME: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            if (cvd->cmd_svm) {
                Tcl_GetCommandFullName(interp, cvd->cmd_svm, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::SVM", TCL_STATIC);
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

    if (cvd->cmd_svm) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_svm);
    }
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
    if (len < 1) {
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

    if (cvd->cmd_svm) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_svm);
    }
    cvd->cmd_svm =
        Tcl_CreateObjCommand(interp, "::cv-mlsvm",
            (Tcl_ObjCmdProc *) SVM_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) SVM_DESTRUCTOR);

    cvd->svm = svm;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


static void SVMSGD_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->svmsgd) {
        cvd->svmsgd.release();
    }
    cvd->cmd_svmsgd = NULL;
}


static int SVMSGD_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "getInitialStepSize",
        "getMarginRegularization",
        "getMarginType",
        "getShift",
        "getStepDecreasingPower",
        "getSvmsgdType",
        "getWeights",
        "setInitialStepSize",
        "setMarginRegularization",
        "setMarginType",
        "setOptimalParameters",
        "setStepDecreasingPower",
        "setSvmsgdType",
        "setTermCriteria",
        "train",
        "predict",
        "save",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_getInitialStepSize,
        FUNC_getMarginRegularization,
        FUNC_getMarginType,
        FUNC_getShift,
        FUNC_getStepDecreasingPower,
        FUNC_getSvmsgdType,
        FUNC_getWeights,
        FUNC_setInitialStepSize,
        FUNC_setMarginRegularization,
        FUNC_setMarginType,
        FUNC_setOptimalParameters,
        FUNC_setStepDecreasingPower,
        FUNC_setSvmsgdType,
        FUNC_setTermCriteria,
        FUNC_train,
        FUNC_predict,
        FUNC_save,
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

    if (cvd->svmsgd == nullptr) {
        Tcl_SetResult(interp, (char *) "no svmsgd instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_getInitialStepSize: {
            float value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svmsgd->getInitialStepSize();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getInitialStepSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getMarginRegularization: {
            float value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svmsgd->getMarginRegularization();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getMarginRegularization failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getMarginType: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svmsgd->getMarginType();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getMarginType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getShift: {
            float value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svmsgd->getShift();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getShift failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getStepDecreasingPower: {
            float value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svmsgd->getStepDecreasingPower();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getStepDecreasingPower failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getSvmsgdType: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svmsgd->getSvmsgdType();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getSvmsgdType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getWeights: {
            cv::Mat value;
            cv::Mat *dstmat;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svmsgd->getWeights();
            } catch (...) {
                Tcl_SetResult(interp, (char *) "getWeights failed", TCL_STATIC);
                return TCL_ERROR;
            }

            dstmat = new cv::Mat(value);
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_setInitialStepSize: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svmsgd->setInitialStepSize((float) value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setInitialStepSize failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setMarginRegularization: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svmsgd->setMarginRegularization((float) value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setMarginRegularization failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setMarginType: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svmsgd->setMarginType(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setMarginType failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setOptimalParameters: {
            int value1, value2;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "svmsgdType marginType");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value1) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &value2) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svmsgd->setOptimalParameters(value1, value2);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setOptimalParameters failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setStepDecreasingPower: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svmsgd->setStepDecreasingPower((float) value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setStepDecreasingPower failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_setSvmsgdType: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->svmsgd->setSvmsgdType(value);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setSvmsgdType failed", TCL_STATIC);
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
                cvd->svmsgd->setTermCriteria(*termCriteria);
            } catch (...) {
                Tcl_SetResult(interp, (char *) "setTermCriteria failed", TCL_STATIC);
                return TCL_ERROR;
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr< cv::ml::TrainData > trainData;
            char *command = NULL;
            int len = 0, flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "train invalid trainData", TCL_STATIC);
                return TCL_ERROR;
            }

            if (strcmp(command, "::cv-mltraindata")==0) {
                trainData = cvd->traindata;
            }

            if (objc == 4) {
                if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->svmsgd->train(trainData, flags);
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
                value = cvd->svmsgd->predict(*samples, results, flags);
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
            if (len < 1) {
                Tcl_SetResult(interp, (char *) "save invalid file name", TCL_STATIC);
                return TCL_ERROR;
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->svmsgd->save(filename);
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

            if (cvd->cmd_svmsgd) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_svmsgd);
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
            if (cvd->cmd_svmsgd) {
                Tcl_GetCommandFullName(interp, cvd->cmd_svmsgd, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::SVMSGD", TCL_STATIC);
            break;
        }

    }

    return TCL_OK;
}


int SVMSGD(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::SVMSGD> svmsgd;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        svmsgd = cv::ml::SVMSGD::create();

        if (svmsgd == nullptr) {
            Tcl_SetResult(interp, (char *) "SVMSGD create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "SVMSGD failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-mlsvmsgd", -1);

    if (cvd->cmd_svmsgd) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_svmsgd);
    }
    cvd->cmd_svmsgd =
        Tcl_CreateObjCommand(interp, "::cv-mlsvmsgd",
            (Tcl_ObjCmdProc *) SVMSGD_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) SVMSGD_DESTRUCTOR);

    cvd->svmsgd = svmsgd;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int SVMSGD_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::SVMSGD> svmsgd;
    char *filename = NULL;
    int len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        Tcl_SetResult(interp, (char *) "load invalid file name", TCL_STATIC);
        return TCL_ERROR;
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        svmsgd = cv::ml::SVMSGD::load(filename);

        if (svmsgd == nullptr) {
            Tcl_DStringFree(&ds);
            Tcl_SetResult(interp, (char *) "SVMSGD load failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_DStringFree(&ds);
        Tcl_SetResult(interp, (char *) "SVMSGD load failed", TCL_STATIC);
        return TCL_ERROR;
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mlsvmsgd", -1);

    if (cvd->cmd_svmsgd) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_svmsgd);
    }
    cvd->cmd_svmsgd =
        Tcl_CreateObjCommand(interp, "::cv-mlsvmsgd",
            (Tcl_ObjCmdProc *) SVMSGD_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) SVMSGD_DESTRUCTOR);

    cvd->svmsgd = svmsgd;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif


static void TrainData_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->traindata) {
        cvd->traindata.release();
    }
    cvd->cmd_traindata = NULL;
}


static int TrainData_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
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

    if (cvd->traindata == nullptr) {
        Tcl_SetResult(interp, (char *) "no traindata instantiated", TCL_STATIC);
        return TCL_ERROR;
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_traindata) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_traindata);
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
            if (cvd->cmd_traindata) {
                Tcl_GetCommandFullName(interp, cvd->cmd_traindata, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::TrainData", TCL_STATIC);
            break;
        }

    }

    return TCL_OK;
}


int TrainData(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::TrainData> traindata;
    cv::Mat *samples, *responses;
    int layout;

    if (objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "samples layout responses");
        return TCL_ERROR;
    }

    samples = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!samples) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &layout) != TCL_OK) {
        return TCL_ERROR;
    }

    responses = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
    if (!responses) {
        return TCL_ERROR;
    }

    try {
        traindata = cv::ml::TrainData::create(*samples, layout, *responses);

        if (traindata == nullptr) {
            Tcl_SetResult(interp, (char *) "TrainData create failed", TCL_STATIC);
            return TCL_ERROR;
        }
    } catch (...) {
        Tcl_SetResult(interp, (char *) "TrainData failed", TCL_STATIC);
        return TCL_ERROR;
    }

    pResultStr = Tcl_NewStringObj("::cv-mltraindata", -1);

    if (cvd->cmd_traindata) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_traindata);
    }
    cvd->cmd_traindata =
        Tcl_CreateObjCommand(interp, "::cv-mltraindata",
            (Tcl_ObjCmdProc *) TrainData_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) TrainData_DESTRUCTOR);

    cvd->traindata = traindata;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
