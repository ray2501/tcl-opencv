#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TCL_USE_OPENCV4

int QRCodeDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::QRCodeDetector *qrdet;

    static const char *FUNC_strs[] = {
        "detect",
        "detectAndDecode",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_DETECT,
        FUNC_DETECTANDDECODE,
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

    cd = cvo->top;
    qrdet = (cv::QRCodeDetector *) cvo->obj;
    if (!qrdet) {
        Tcl_Panic("null QRCodeDetector object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECT: {
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *srcmat, *dstmat;
            cv::Mat points_matrix;
            bool result;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            srcmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!srcmat) {
                return TCL_ERROR;
            }

            try {
                result = qrdet->detect(*srcmat, points_matrix);
                if (!result) {
                    return Opencv_SetResult(interp, cv::Error::StsError, "no detect result");
                }
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(points_matrix);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_DETECTANDDECODE: {
            Tcl_Obj *pResultStr = NULL, *pResultStr1 = NULL, *pResultStr2 = NULL;
            cv::Mat *srcmat, *dstmat, *codemat;
            cv::Mat points_matrix, code_matrix;
            std::string result;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            srcmat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!srcmat) {
                return TCL_ERROR;
            }

            try {
                result = qrdet->detectAndDecode(*srcmat,
                                                points_matrix,
                                                code_matrix);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            dstmat = new cv::Mat(points_matrix);

            pResultStr1 = Opencv_NewHandle(cd, interp, OPENCV_MAT, dstmat);

            codemat = new cv::Mat(code_matrix);

            pResultStr2 = Opencv_NewHandle(cd, interp, OPENCV_MAT, codemat);

            pResultStr = Tcl_NewListObj(0, NULL);
            Tcl_ListObjAppendElement(NULL, pResultStr,
                Tcl_NewStringObj(result.c_str(), result.length()));

            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr1);
            Tcl_ListObjAppendElement(NULL, pResultStr, pResultStr2);

            Tcl_SetObjResult(interp, pResultStr);

            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_DeleteCommandFromToken(interp, cvo->cmd);

            break;
        }
        case FUNC__COMMAND: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__NAME: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(cvo->key, -1));
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::QRCodeDetector", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int QRCodeDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::QRCodeDetector *qrdet;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, 0);
        return TCL_ERROR;
    }

    try {
        qrdet = new cv::QRCodeDetector();
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_QDETECT, qrdet);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}

#endif /* TCL_USE_OPENCV4 */


int CascadeClassifier_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::CascadeClassifier *cas;

    static const char *FUNC_strs[] = {
        "detectMultiScale",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_DETECTMULTISCALE,
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

    cd = cvo->top;
    cas = (cv::CascadeClassifier *) cvo->obj;
    if (!cas) {
        Tcl_Panic("null CascadeClassifier object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_DETECTMULTISCALE: {
            double scaleFactor = 1.1;
            int minNeighbors = 3, minWidth = 0, minHeight = 0, maxWidth = 0, maxHeight = 0;
            std::vector<cv::Rect> rect;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *mat;

            if (objc != 3 && objc != 9) {
                Tcl_WrongNumArgs(interp, 2, objv,
                        "matrix ?scaleFactor minNeighbors minWidth minHeight maxWidth maxHeight?");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (objc == 9) {
                if (Tcl_GetDoubleFromObj(interp, objv[3], &scaleFactor) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[4], &minNeighbors) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[5], &minWidth) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[6], &minHeight) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[7], &maxWidth) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[8], &maxHeight) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cas->detectMultiScale(*mat,
                                      rect, scaleFactor, minNeighbors, 0,
                                      cv::Size(minWidth, minHeight),
                                      cv::Size(maxWidth, maxHeight));
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < rect.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].width));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].height));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_DeleteCommandFromToken(interp, cvo->cmd);

            break;
        }
        case FUNC__COMMAND: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__NAME: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(cvo->key, -1));
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::CascadeClassifier", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int CascadeClassifier(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *filename = NULL;
    int len = 0;
    cv::CascadeClassifier *cas;
    Tcl_Obj *pResultStr = NULL;
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
        cas = new cv::CascadeClassifier(filename);
        if (cas->empty() == true) {
            Tcl_DStringFree(&ds);
            return Opencv_SetResult(interp, cv::Error::StsError, "loading from file failed");
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_ODETECT, cas);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int HOGDescriptor_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::HOGDescriptor *hog;

    static const char *FUNC_strs[] = {
        "compute",
        "detectMultiScale",
        "getDefaultPeopleDetector",
        "getDaimlerPeopleDetector",
        "setSVMDetector",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_COMPUTE,
        FUNC_DETECTMULTISCALE,
        FUNC_getDefaultPeopleDetector,
        FUNC_getDaimlerPeopleDetector,
        FUNC_setSVMDetector,
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

    cd = cvo->top;
    hog = (cv::HOGDescriptor *) cvo->obj;
    if (!hog) {
        Tcl_Panic("null HOGDescriptor object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_COMPUTE: {
            std::vector<float> descriptors;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *mat;
            int winStride_width = 8, winStride_height = 8, padding_width = 0, padding_height = 0;

            if (objc != 3 && objc != 7) {
                Tcl_WrongNumArgs(interp, 2, objv,
                    "matrix ?winStride_width winStride_heigth padding_width padding_height?");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (objc == 7) {
                if (Tcl_GetIntFromObj(interp, objv[3], &winStride_width) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[4], &winStride_height) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[5], &padding_width) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[6], &padding_height) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                if (objc == 3) {
                    hog->compute(*mat, descriptors);
                } else {
                    hog->compute(*mat, descriptors,
                                 cv::Size(winStride_width, winStride_height),
                                 cv::Size(padding_width, padding_height));
                }
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < descriptors.size(); i++) {
                Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(descriptors[i]));
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_DETECTMULTISCALE: {
            std::vector<cv::Rect> rect;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *mat;
            double hitThreshold = 0, scale = 1.05, finalThreshold = 2.0;
            int winStride_width = 8, winStride_height = 8, padding_width = 0, padding_height = 0;
            int useMeanshiftGrouping = 0;

            if (objc != 3 && objc != 11) {
                Tcl_WrongNumArgs(interp, 2, objv,
                    "matrix ?hitThreshold winStride_width winStride_heigth padding_width padding_height scale finalThreshold useMeanshiftGrouping?");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (objc == 11) {
                if (Tcl_GetDoubleFromObj(interp, objv[3], &hitThreshold) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[4], &winStride_width) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[5], &winStride_height) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[6], &padding_width) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetIntFromObj(interp, objv[7], &padding_height) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetDoubleFromObj(interp, objv[8], &scale) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetDoubleFromObj(interp, objv[9], &finalThreshold) != TCL_OK) {
                    return TCL_ERROR;
                }

                if (Tcl_GetBooleanFromObj(interp, objv[10], &useMeanshiftGrouping) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                hog->detectMultiScale(*mat, rect, hitThreshold,
                                      cv::Size(winStride_width, winStride_height),
                                      cv::Size(padding_width, padding_height),
                                      scale, finalThreshold,
                                      (bool) useMeanshiftGrouping);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < rect.size(); i++) {
                Tcl_Obj *pListStr = NULL;
                pListStr = Tcl_NewListObj(0, NULL);
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].x));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].y));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].width));
                Tcl_ListObjAppendElement(NULL, pListStr, Tcl_NewIntObj(rect[i].height));

                Tcl_ListObjAppendElement(NULL, pResultStr, pListStr);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getDefaultPeopleDetector: {
            std::vector<float> coefficients;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                coefficients = cv::HOGDescriptor::getDefaultPeopleDetector();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < coefficients.size(); i++) {
                Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(coefficients[i]));
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getDaimlerPeopleDetector: {
            std::vector<float> coefficients;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                coefficients = cv::HOGDescriptor::getDaimlerPeopleDetector();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(0, NULL);
            for (size_t i = 0; i < coefficients.size(); i++) {
                Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewDoubleObj(coefficients[i]));
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_setSVMDetector: {
            std::vector<float> svmdetector;
            int count = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "svmdetector");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid data");
            } else {
                Tcl_Obj *elemListPtr = NULL;
                double number;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                    if (Tcl_GetDoubleFromObj(interp, elemListPtr, &number) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    svmdetector.push_back((float) number);
                }
            }

            try {
                hog->setSVMDetector(svmdetector);
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

            Tcl_DeleteCommandFromToken(interp, cvo->cmd);

            break;
        }
        case FUNC__COMMAND: {
            Tcl_Obj *obj;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_SetObjResult(interp, obj);
            break;
        }
        case FUNC__NAME: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(cvo->key, -1));
            break;
        }
        case FUNC__TYPE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Tcl_SetResult(interp, (char *) "cv::HOGDescriptor", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int HOGDescriptor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int winSize_width = 0, winSize_height = 0, blockSize_width = 0, blockSize_height = 0;
    int blockStride_width = 0, blockStride_height = 0, cellSize_width = 0, cellSize_height = 0, nbins = 0;
    int derivAperture = 1, gammaCorrection = 0, nlevels = cv::HOGDescriptor::DEFAULT_NLEVELS, signedGradient = 0;
    double winSigma = -1, L2HysThreshold = 0.2;
#ifdef TCL_USE_OPENCV4
    cv::HOGDescriptor::HistogramNormType histogramNormType = cv::HOGDescriptor::L2Hys;
#else
    int histogramNormType = cv::HOGDescriptor::L2Hys;
#endif
    cv::HOGDescriptor *hog;
    Tcl_Obj *pResultStr = NULL;

    if (objc != 10 && objc != 16) {
        Tcl_WrongNumArgs(interp, 1, objv,
            "winSize_width winSize_height blockSize_width blockSize_height blockStride_width blockStride_height cellSize_width cellSize_height nbins ?derivAperture winSigma L2HysThreshold gammaCorrection nlevels signedGradient?");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &winSize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[2], &winSize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &blockSize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &blockSize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[5], &blockStride_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[6], &blockStride_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[7], &cellSize_width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[8], &cellSize_height) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[9], &nbins) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 16) {
        if (Tcl_GetIntFromObj(interp, objv[10], &derivAperture) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[11], &winSigma) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[12], &L2HysThreshold) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[13], &gammaCorrection) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[14], &nlevels) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetBooleanFromObj(interp, objv[15], &signedGradient) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        hog = new cv::HOGDescriptor(cv::Size(winSize_width, winSize_height),
                                    cv::Size(blockSize_width, blockSize_height),
                                    cv::Size(blockStride_width, blockStride_height),
                                    cv::Size(cellSize_width, cellSize_height), nbins,
                                    derivAperture, winSigma, histogramNormType,
                                    L2HysThreshold, gammaCorrection, nlevels,
                                    signedGradient);

        if (hog == nullptr) {
            CV_Error(cv::Error::StsNullPtr, "HOGDescriptor nullptr");
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_OOBJHOG, hog);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
