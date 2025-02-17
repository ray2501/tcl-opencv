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
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_get_learnt_thetas: {
            cv::Mat value;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->logistic->get_learnt_thetas();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(value);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->logistic->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
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
            CV_Error(cv::Error::StsNullPtr, "LogisticRegression nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
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
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        logistic = cv::ml::LogisticRegression::load(filename);

        if (logistic == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "LogisticRegression nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
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
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_predictProb: {
            cv::Mat *samples, *dstmat1, *dstmat2;
            cv::Mat outputs, outputProbs;
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[3];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat1 = new cv::Mat(outputs);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat1);

            dstmat2 = new cv::Mat(outputProbs);
            list[2] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat2);

            Tcl_SetObjResult(interp, Tcl_NewListObj(3, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->bayesclassifier->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
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
            CV_Error(cv::Error::StsNullPtr, "NormalBayesClassifier nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
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
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        bayesclassifier = cv::ml::NormalBayesClassifier::load(filename);

        if (bayesclassifier == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "NormalBayesClassifier nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
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
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
                cvd->knearest->setIsClassifier((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_findNearest: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->knearest->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
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
            CV_Error(cv::Error::StsNullPtr, "KNearest nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
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
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        knearest = cv::ml::KNearest::load(filename);

        if (knearest == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "KNearest nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
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
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getDecisionFunction: {
            cv::Mat alpha, svidx;
            cv::Mat *alphamat, *svidxmat;
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[3];

            list[0] = Tcl_NewDoubleObj(value);

            alphamat = new cv::Mat(alpha);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, alphamat);

            svidxmat = new cv::Mat(svidx);
            list[2] = Opencv_NewHandle(cd, interp, OPENCV_MAT, svidxmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(3, list));

            break;
        }
        case FUNC_getSupportVectors: {
            cv::Mat vectors;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                vectors = cvd->svm->getSupportVectors();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(vectors);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
        case FUNC_getUncompressedSupportVectors: {
            cv::Mat vectors;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                vectors = cvd->svm->getUncompressedSupportVectors();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(vectors);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->svm->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
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
            CV_Error(cv::Error::StsNullPtr, "SVM nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
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
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        svm = cv::ml::SVM::load(filename);

        if (svm == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "SVM nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
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
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getWeights: {
            cv::Mat value;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->svmsgd->getWeights();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(value);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->svmsgd->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
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
            CV_Error(cv::Error::StsNullPtr, "SVMSGD nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
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
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        svmsgd = cv::ml::SVMSGD::load(filename);

        if (svmsgd == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "SVMSGD nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
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


static void DTrees_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->dtrees) {
        cvd->dtrees.release();
    }
    cvd->cmd_dtrees = NULL;
}


static int DTrees_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "getCVFolds",
        "getMaxCategories",
        "getMaxDepth",
        "getMinSampleCount",
        "getPriors",
        "getRegressionAccuracy",
        "getTruncatePrunedTree",
        "getUse1SERule",
        "getUseSurrogates",
        "setCVFolds",
        "setMaxCategories",
        "setMaxDepth",
        "setMinSampleCount",
        "setPriors",
        "setRegressionAccuracy",
        "setTruncatePrunedTree",
        "setUse1SERule",
        "setUseSurrogates",
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
        FUNC_getCVFolds,
        FUNC_getMaxCategories,
        FUNC_getMaxDepth,
        FUNC_getMinSampleCount,
        FUNC_getPriors,
        FUNC_getRegressionAccuracy,
        FUNC_getTruncatePrunedTree,
        FUNC_getUse1SERule,
        FUNC_getUseSurrogates,
        FUNC_setCVFolds,
        FUNC_setMaxCategories,
        FUNC_setMaxDepth,
        FUNC_setMinSampleCount,
        FUNC_setPriors,
        FUNC_setRegressionAccuracy,
        FUNC_setTruncatePrunedTree,
        FUNC_setUse1SERule,
        FUNC_setUseSurrogates,
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

    if (cvd->dtrees == nullptr) {
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_getCVFolds: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->dtrees->getCVFolds();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMaxCategories: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->dtrees->getMaxCategories();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMaxDepth: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->dtrees->getMaxDepth();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMinSampleCount: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->dtrees->getMinSampleCount();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getPriors: {
            cv::Mat results;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                results = cvd->dtrees->getPriors();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(results);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
        case FUNC_getRegressionAccuracy: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->dtrees->getRegressionAccuracy();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getTruncatePrunedTree: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->dtrees->getTruncatePrunedTree();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getUse1SERule: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->dtrees->getUse1SERule();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getUseSurrogates: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->dtrees->getUseSurrogates();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_setCVFolds: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setCVFolds(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxCategories: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setMaxCategories(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxDepth: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setMaxDepth(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMinSampleCount: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setMinSampleCount(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setPriors: {
            cv::Mat *priori;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            priori = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!priori) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setPriors(*priori);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setRegressionAccuracy: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setRegressionAccuracy((float) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setTruncatePrunedTree: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setTruncatePrunedTree((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setUse1SERule: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setUse1SERule((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setUseSurrogates: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->dtrees->setUseSurrogates((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
                cvd->dtrees->train(trainData, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
                value = cvd->dtrees->predict(*samples, results, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->dtrees->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_DStringFree(&ds);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_dtrees) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_dtrees);
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
            if (cvd->cmd_dtrees) {
                Tcl_GetCommandFullName(interp, cvd->cmd_dtrees, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::DTrees", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int DTrees(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::DTrees> dtrees;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        dtrees = cv::ml::DTrees::create();

        if (dtrees == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "DTrees nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-mldtrees", -1);

    if (cvd->cmd_dtrees) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_dtrees);
    }
    cvd->cmd_dtrees =
        Tcl_CreateObjCommand(interp, "::cv-mldtrees",
            (Tcl_ObjCmdProc *) DTrees_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) DTrees_DESTRUCTOR);

    cvd->dtrees = dtrees;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int DTrees_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::DTrees> dtrees;
    char *filename = NULL;
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        dtrees = cv::ml::DTrees::load(filename);

        if (dtrees == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "DTrees nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mldtrees", -1);

    if (cvd->cmd_dtrees) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_dtrees);
    }
    cvd->cmd_dtrees =
        Tcl_CreateObjCommand(interp, "::cv-mldtrees",
            (Tcl_ObjCmdProc *) DTrees_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) DTrees_DESTRUCTOR);

    cvd->dtrees = dtrees;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif


static void Boost_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->boost) {
        cvd->boost.release();
    }
    cvd->cmd_boost = NULL;
}


static int Boost_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "getCVFolds",
        "getMaxCategories",
        "getMaxDepth",
        "getMinSampleCount",
        "getPriors",
        "getRegressionAccuracy",
        "getTruncatePrunedTree",
        "getUse1SERule",
        "getUseSurrogates",
        "getBoostType",
        "getWeakCount",
        "getWeightTrimRate",
        "setCVFolds",
        "setMaxCategories",
        "setMaxDepth",
        "setMinSampleCount",
        "setPriors",
        "setRegressionAccuracy",
        "setTruncatePrunedTree",
        "setUse1SERule",
        "setUseSurrogates",
        "setBoostType",
        "setWeakCount",
        "setWeightTrimRate",
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
        FUNC_getCVFolds,
        FUNC_getMaxCategories,
        FUNC_getMaxDepth,
        FUNC_getMinSampleCount,
        FUNC_getPriors,
        FUNC_getRegressionAccuracy,
        FUNC_getTruncatePrunedTree,
        FUNC_getUse1SERule,
        FUNC_getUseSurrogates,
        FUNC_getBoostType,
        FUNC_getWeakCount,
        FUNC_getWeightTrimRate,
        FUNC_setCVFolds,
        FUNC_setMaxCategories,
        FUNC_setMaxDepth,
        FUNC_setMinSampleCount,
        FUNC_setPriors,
        FUNC_setRegressionAccuracy,
        FUNC_setTruncatePrunedTree,
        FUNC_setUse1SERule,
        FUNC_setUseSurrogates,
        FUNC_setBoostType,
        FUNC_setWeakCount,
        FUNC_setWeightTrimRate,
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

    if (cvd->boost == nullptr) {
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_getCVFolds: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getCVFolds();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMaxCategories: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getMaxCategories();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMaxDepth: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getMaxDepth();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMinSampleCount: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getMinSampleCount();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getPriors: {
            cv::Mat results;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                results = cvd->boost->getPriors();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(results);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
        case FUNC_getRegressionAccuracy: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getRegressionAccuracy();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getTruncatePrunedTree: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getTruncatePrunedTree();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getUse1SERule: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getUse1SERule();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getUseSurrogates: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getUseSurrogates();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getBoostType: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getBoostType();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getWeakCount: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getWeakCount();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getWeightTrimRate: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->boost->getWeightTrimRate();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_setCVFolds: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setCVFolds(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxCategories: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setMaxCategories(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxDepth: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setMaxDepth(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMinSampleCount: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setMinSampleCount(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setPriors: {
            cv::Mat *priori;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            priori = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!priori) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setPriors(*priori);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setRegressionAccuracy: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setRegressionAccuracy((float) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setTruncatePrunedTree: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setTruncatePrunedTree((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setUse1SERule: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setUse1SERule((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setUseSurrogates: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setUseSurrogates((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setBoostType: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setBoostType(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setWeakCount: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setWeakCount(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setWeightTrimRate: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->boost->setWeightTrimRate(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
                cvd->boost->train(trainData, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
                value = cvd->boost->predict(*samples, results, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->boost->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_DStringFree(&ds);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_boost) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_boost);
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
            if (cvd->cmd_boost) {
                Tcl_GetCommandFullName(interp, cvd->cmd_boost, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::Boost", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int Boost(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::Boost> boost;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        boost = cv::ml::Boost::create();

        if (boost == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "Boost nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-mlboost", -1);

    if (cvd->cmd_boost) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_boost);
    }
    cvd->cmd_boost =
        Tcl_CreateObjCommand(interp, "::cv-mlboost",
            (Tcl_ObjCmdProc *) Boost_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) Boost_DESTRUCTOR);

    cvd->boost = boost;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int Boost_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::Boost> boost;
    char *filename = NULL;
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        boost = cv::ml::Boost::load(filename);

        if (boost == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "Boost nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mlboost", -1);

    if (cvd->cmd_boost) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_boost);
    }
    cvd->cmd_boost =
        Tcl_CreateObjCommand(interp, "::cv-mlboost",
            (Tcl_ObjCmdProc *) Boost_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) Boost_DESTRUCTOR);

    cvd->boost = boost;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif


static void RTrees_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->rtrees) {
        cvd->rtrees.release();
    }
    cvd->cmd_rtrees = NULL;
}


static int RTrees_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
        "getCVFolds",
        "getMaxCategories",
        "getMaxDepth",
        "getMinSampleCount",
        "getPriors",
        "getRegressionAccuracy",
        "getTruncatePrunedTree",
        "getUse1SERule",
        "getUseSurrogates",
        "getActiveVarCount",
        "getCalculateVarImportance",
        "getVarImportance",
#ifdef TCL_USE_OPENCV4
        "getVotes",
#endif
        "setCVFolds",
        "setMaxCategories",
        "setMaxDepth",
        "setMinSampleCount",
        "setPriors",
        "setRegressionAccuracy",
        "setTruncatePrunedTree",
        "setUse1SERule",
        "setUseSurrogates",
        "setActiveVarCount",
        "setCalculateVarImportance",
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
        FUNC_getCVFolds,
        FUNC_getMaxCategories,
        FUNC_getMaxDepth,
        FUNC_getMinSampleCount,
        FUNC_getPriors,
        FUNC_getRegressionAccuracy,
        FUNC_getTruncatePrunedTree,
        FUNC_getUse1SERule,
        FUNC_getUseSurrogates,
        FUNC_getActiveVarCount,
        FUNC_getCalculateVarImportance,
        FUNC_getVarImportance,
#ifdef TCL_USE_OPENCV4
        FUNC_getVotes,
#endif
        FUNC_setCVFolds,
        FUNC_setMaxCategories,
        FUNC_setMaxDepth,
        FUNC_setMinSampleCount,
        FUNC_setPriors,
        FUNC_setRegressionAccuracy,
        FUNC_setTruncatePrunedTree,
        FUNC_setUse1SERule,
        FUNC_setUseSurrogates,
        FUNC_setActiveVarCount,
        FUNC_setCalculateVarImportance,
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

    if (cvd->rtrees == nullptr) {
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_getCVFolds: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getCVFolds();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMaxCategories: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getMaxCategories();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMaxDepth: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getMaxDepth();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getMinSampleCount: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getMinSampleCount();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getPriors: {
            cv::Mat results;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                results = cvd->rtrees->getPriors();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(results);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
        case FUNC_getRegressionAccuracy: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getRegressionAccuracy();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getTruncatePrunedTree: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getTruncatePrunedTree();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getUse1SERule: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getUse1SERule();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getUseSurrogates: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getUseSurrogates();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getActiveVarCount: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getActiveVarCount();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getCalculateVarImportance: {
            bool value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->rtrees->getCalculateVarImportance();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewBooleanObj((int) value));
            break;
        }
        case FUNC_getVarImportance: {
            cv::Mat results;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                results = cvd->rtrees->getVarImportance();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(results);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
#ifdef TCL_USE_OPENCV4
        case FUNC_getVotes: {
            int flags = 0;
            cv::Mat results;
            cv::Mat *samples, *dstmat;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "samples flags");
                return TCL_ERROR;
            }

            samples = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!samples) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &flags) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->getVotes(*samples, results, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(results);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
#endif
        case FUNC_setCVFolds: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setCVFolds(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxCategories: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setMaxCategories(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxDepth: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setMaxDepth(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMinSampleCount: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setMinSampleCount(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setPriors: {
            cv::Mat *priori;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            priori = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!priori) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setPriors(*priori);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setRegressionAccuracy: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setRegressionAccuracy((float) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setTruncatePrunedTree: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setTruncatePrunedTree((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setUse1SERule: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setUse1SERule((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setUseSurrogates: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setUseSurrogates((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setActiveVarCount: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setActiveVarCount(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setCalculateVarImportance: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetBooleanFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->rtrees->setCalculateVarImportance((bool) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
                cvd->rtrees->setTermCriteria(*termCriteria);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
                cvd->rtrees->train(trainData, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
                value = cvd->rtrees->predict(*samples, results, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->rtrees->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_DStringFree(&ds);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_rtrees) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_rtrees);
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
            if (cvd->cmd_rtrees) {
                Tcl_GetCommandFullName(interp, cvd->cmd_rtrees, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::RTrees", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int RTrees(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::RTrees> rtrees;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        rtrees = cv::ml::RTrees::create();

        if (rtrees == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "RTrees nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-mlrtrees", -1);

    if (cvd->cmd_rtrees) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_rtrees);
    }
    cvd->cmd_rtrees =
        Tcl_CreateObjCommand(interp, "::cv-mlrtrees",
            (Tcl_ObjCmdProc *) RTrees_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) RTrees_DESTRUCTOR);

    cvd->rtrees = rtrees;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int RTrees_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::RTrees> rtrees;
    char *filename = NULL;
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        rtrees = cv::ml::RTrees::load(filename);

        if (rtrees == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "RTress nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mlrtrees", -1);

    if (cvd->cmd_rtrees) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_rtrees);
    }
    cvd->cmd_rtrees =
        Tcl_CreateObjCommand(interp, "::cv-mlrtrees",
            (Tcl_ObjCmdProc *) RTrees_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) RTrees_DESTRUCTOR);

    cvd->rtrees = rtrees;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif


static void ANN_MLP_DESTRUCTOR(void *cd)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->annmlp) {
        cvd->annmlp.release();
    }
    cvd->cmd_annmlp = NULL;
}


static int ANN_MLP_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    int choice;

    static const char *FUNC_strs[] = {
#ifdef TCL_USE_OPENCV4
        "getAnnealCoolingRatio",
        "getAnnealFinalT",
        "getAnnealInitialT",
        "getAnnealItePerStep",
#endif
        "getBackpropMomentumScale",
        "getBackpropWeightScale",
        "getRpropDW0",
        "getRpropDWMax",
        "getRpropDWMin",
        "getRpropDWMinus",
        "getRpropDWPlus",
        "getLayerSizes",
        "getTrainMethod",
        "getWeights",
#ifdef TCL_USE_OPENCV4
        "setAnnealCoolingRatio",
        "setAnnealFinalT",
        "setAnnealInitialT",
        "setAnnealItePerStep",
#endif
        "setBackpropMomentumScale",
        "setBackpropWeightScale",
        "setRpropDW0",
        "setRpropDWMax",
        "setRpropDWMin",
        "setRpropDWMinus",
        "setRpropDWPlus",
        "setActivationFunction",
        "setLayerSizes",
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
#ifdef TCL_USE_OPENCV4
        FUNC_getAnnealCoolingRatio,
        FUNC_getAnnealFinalT,
        FUNC_getAnnealInitialT,
        FUNC_getAnnealItePerStep,
#endif
        FUNC_getBackpropMomentumScale,
        FUNC_getBackpropWeightScale,
        FUNC_getRpropDW0,
        FUNC_getRpropDWMax,
        FUNC_getRpropDWMin,
        FUNC_getRpropDWMinus,
        FUNC_getRpropDWPlus,
        FUNC_getLayerSizes,
        FUNC_getTrainMethod,
        FUNC_getWeights,
#ifdef TCL_USE_OPENCV4
        FUNC_setAnnealCoolingRatio,
        FUNC_setAnnealFinalT,
        FUNC_setAnnealInitialT,
        FUNC_setAnnealItePerStep,
#endif
        FUNC_setBackpropMomentumScale,
        FUNC_setBackpropWeightScale,
        FUNC_setRpropDW0,
        FUNC_setRpropDWMax,
        FUNC_setRpropDWMin,
        FUNC_setRpropDWMinus,
        FUNC_setRpropDWPlus,
        FUNC_setActivationFunction,
        FUNC_setLayerSizes,
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

    if (cvd->annmlp == nullptr) {
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
    }

    switch ((enum FUNC_enum)choice) {
#ifdef TCL_USE_OPENCV4
        case FUNC_getAnnealCoolingRatio: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getAnnealCoolingRatio();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getAnnealFinalT: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getAnnealFinalT();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getAnnealInitialT: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getAnnealInitialT();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getAnnealItePerStep: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getAnnealItePerStep();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
#endif
        case FUNC_getBackpropMomentumScale: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getBackpropMomentumScale();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getBackpropWeightScale: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getBackpropWeightScale();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getRpropDW0: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getRpropDW0();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getRpropDWMax: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getRpropDWMax();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getRpropDWMin: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getRpropDWMin();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getRpropDWMinus: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getRpropDWMinus();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getRpropDWPlus: {
            double value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getRpropDWPlus();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getLayerSizes: {
            cv::Mat layer;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                layer = cvd->annmlp->getLayerSizes();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(layer);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
        case FUNC_getTrainMethod: {
            int value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->annmlp->getTrainMethod();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getWeights: {
            int layerIdx;
            cv::Mat weights;
            cv::Mat *dstmat;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "layerIdx");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &layerIdx) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                weights = cvd->annmlp->getWeights(layerIdx);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }


            dstmat = new cv::Mat(weights);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
#ifdef TCL_USE_OPENCV4
        case FUNC_setAnnealCoolingRatio: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setAnnealCoolingRatio(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setAnnealFinalT: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setAnnealFinalT(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setAnnealInitialT: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setAnnealInitialT(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setAnnealItePerStep: {
            int value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setAnnealItePerStep(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
#endif
        case FUNC_setBackpropMomentumScale: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setBackpropMomentumScale(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setBackpropWeightScale: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setBackpropWeightScale(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setRpropDW0: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setRpropDW0(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setRpropDWMax: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setRpropDWMax(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setRpropDWMin: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setRpropDWMin(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setRpropDWMinus: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setRpropDWMinus(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setRpropDWPlus: {
            double value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setRpropDWPlus(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setActivationFunction: {
            int value;
            double param1 = 0, param2 = 0;

            if (objc != 3 && objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "value ?param1 param2?");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 5) {
                if (Tcl_GetDoubleFromObj(interp, objv[3], &param1) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetDoubleFromObj(interp, objv[4], &param2) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->annmlp->setActivationFunction(value, param1, param2);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setLayerSizes: {
            cv::Mat *layer;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            layer = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!layer) {
                return TCL_ERROR;
            }

            try {
                cvd->annmlp->setLayerSizes(*layer);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setTrainMethod: {
            int value;
            double param1 = 0, param2 = 0;

            if (objc != 3 && objc != 5) {
                Tcl_WrongNumArgs(interp, 2, objv, "value ?param1 param2?");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 5) {
                if (Tcl_GetDoubleFromObj(interp, objv[3], &param1) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetDoubleFromObj(interp, objv[4], &param2) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->annmlp->setTrainMethod(value, param1, param2);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
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
                cvd->annmlp->setTermCriteria(*termCriteria);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_train: {
            cv::Ptr<cv::ml::TrainData> trainData;
            char *command = NULL;
            Tcl_Size len = 0;
            int flags = 0;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "trainData ?flags?");
                return TCL_ERROR;
            }

            command = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid train data");
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
                cvd->annmlp->train(trainData, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_predict: {
            cv::Mat *samples, *dstmat;
            cv::Mat results;
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
                value = cvd->annmlp->predict(*samples, results, flags);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] = Tcl_NewDoubleObj(value);

            dstmat = new cv::Mat(results);
            list[1] = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

            break;
        }
        case FUNC_save: {
            char *filename = NULL;
            Tcl_Size len = 0;
            Tcl_DString ds;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename");
                return TCL_ERROR;
            }

            filename = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
            }

            filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
            try {
                cvd->annmlp->save(filename);
            } catch (const cv::Exception &ex) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_DStringFree(&ds);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            if (cvd->cmd_annmlp) {
                Tcl_DeleteCommandFromToken(interp, cvd->cmd_annmlp);
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
            if (cvd->cmd_annmlp) {
                Tcl_GetCommandFullName(interp, cvd->cmd_annmlp, obj);
            }
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::ml::ANN_MLP", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int ANN_MLP(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::ANN_MLP> annmlp;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        annmlp = cv::ml::ANN_MLP::create();

        if (annmlp == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "ANN_MLP nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewStringObj("::cv-mlannmlp", -1);

    if (cvd->cmd_annmlp) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_annmlp);
    }
    cvd->cmd_annmlp =
        Tcl_CreateObjCommand(interp, "::cv-mlannmlp",
            (Tcl_ObjCmdProc *) ANN_MLP_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) ANN_MLP_DESTRUCTOR);

    cvd->annmlp = annmlp;

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_OPENCV4
int ANN_MLP_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::ANN_MLP> annmlp;
    char *filename = NULL;
    Tcl_Size len = 0;
    Tcl_DString ds;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "filename");
        return TCL_ERROR;
    }

    filename = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid file name");
    }

    filename = Tcl_UtfToExternalDString(NULL, filename, len, &ds);
    try {
        annmlp = cv::ml::ANN_MLP::load(filename);

        if (annmlp == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "ANN_MLP nullptr");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    pResultStr = Tcl_NewStringObj("::cv-mlannmlp", -1);

    if (cvd->cmd_annmlp) {
        Tcl_DeleteCommandFromToken(interp, cvd->cmd_annmlp);
    }
    cvd->cmd_annmlp =
        Tcl_CreateObjCommand(interp, "::cv-mlannmlp",
            (Tcl_ObjCmdProc *) ANN_MLP_FUNCTION,
            cd, (Tcl_CmdDeleteProc *) ANN_MLP_DESTRUCTOR);

    cvd->annmlp = annmlp;

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
        "getTestResponses",
        "getTestSamples",
        "setTrainTestSplit",
        "setTrainTestSplitRatio",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_getTestResponses,
        FUNC_getTestSamples,
        FUNC_setTrainTestSplit,
        FUNC_setTrainTestSplitRatio,
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
        return Opencv_SetResult(interp, cv::Error::StsNullPtr, "singleton not instantiated");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_getTestResponses: {
            cv::Mat value;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->traindata->getTestResponses();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(value);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
        case FUNC_getTestSamples: {
            cv::Mat value;
            cv::Mat *dstmat;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = cvd->traindata->getTestSamples();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(value);

            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, dstmat);
            break;
        }
        case FUNC_setTrainTestSplit: {
            int count = 0;
            int shuffle = 1;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "count ?shuffle?");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &count) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                if (Tcl_GetBooleanFromObj(interp, objv[3], &shuffle) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->traindata->setTrainTestSplit(count, (bool) shuffle);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setTrainTestSplitRatio: {
            double ratio = 0;
            int shuffle = 1;

            if (objc != 3 && objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "ratio ?shuffle?");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &ratio) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 4) {
                if (Tcl_GetBooleanFromObj(interp, objv[3], &shuffle) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cvd->traindata->setTrainTestSplitRatio(ratio, (bool) shuffle);
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


int TrainData_CONSTRUCTOR(void *cd, Tcl_Interp *interp, cv::Mat &samples, int layout, cv::Mat &responses)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Tcl_Obj *pResultStr = NULL;
    cv::Ptr<cv::ml::TrainData> traindata;

    try {
        traindata = cv::ml::TrainData::create(samples, layout, responses);

        if (traindata == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "TrainData nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
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
            CV_Error(cv::Error::StsNullPtr, "TrainData nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
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
