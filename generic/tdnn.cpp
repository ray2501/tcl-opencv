#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TCL_USE_OPENCV4

int dnn_blobFromImage(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dstimage;
    double scalefactor = 1.0, B = 0, G = 0, R = 0, A = 0;;
    int width = 0, height = 0, swapRB = 0, crop = 0, count = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::Mat *mat, *dstmat;

    if (objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv,
                         "matrix scalefactor width height mean_color_list swapRB crop");
        return TCL_ERROR;
    }

    mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[1]);
    if (!mat) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[2], &scalefactor) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
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
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &B) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &G) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &R) != TCL_OK) {
            return TCL_ERROR;
        }

        Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
        if (Tcl_GetDoubleFromObj(interp, elemListPtr, &A) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    if (Tcl_GetBooleanFromObj(interp, objv[6], &swapRB) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetBooleanFromObj(interp, objv[7], &crop) != TCL_OK) {
        return TCL_ERROR;
    }

    try {
        cv::Scalar color(B, G, R, A);
        cv::Size size(width, height);

        dstimage = cv::dnn::blobFromImage(*mat, scalefactor, size, color, swapRB, crop);
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


int READNET_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::dnn::Net *net;

    static const char *FUNC_strs[] = {
        "getLayerNames",
        "getUnconnectedOutLayers",
        "getUnconnectedOutLayersNames",
        "setPreferableBackend",
        "setPreferableTarget",
        "setInput",
        "forward",
        "forwardWithNames",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_getLayerNames,
        FUNC_getUnconnectedOutLayers,
        FUNC_getUnconnectedOutLayersNames,
        FUNC_setPreferableBackend,
        FUNC_setPreferableTarget,
        FUNC_setInput,
        FUNC_forward,
        FUNC_forwardWithNames,
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

    cd = (void *) cvo->top;
    net = (cv::dnn::Net *) cvo->obj;
    if (!net) {
        Tcl_Panic("null DNN object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_getLayerNames: {
            std::vector<cv::String> value;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = net->getLayerNames();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(value.size(), NULL);

            for (size_t i = 0; i < value.size(); i++) {
                Tcl_ListObjAppendElement(NULL, pResultStr,
                                         Tcl_NewStringObj(value[i].c_str(), -1));
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getUnconnectedOutLayers: {
            std::vector<int> value;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = net->getUnconnectedOutLayers();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(value.size(), NULL);

            for (size_t i = 0; i < value.size(); i++) {
                Tcl_ListObjAppendElement(NULL, pResultStr,
                                         Tcl_NewIntObj(value[i]));
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_getUnconnectedOutLayersNames: {
            std::vector<cv::String> value;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = net->getUnconnectedOutLayersNames();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(value.size(), NULL);

            for (size_t i = 0; i < value.size(); i++) {
                Tcl_ListObjAppendElement(NULL, pResultStr,
                                         Tcl_NewStringObj(value[i].c_str(), -1));
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_setPreferableBackend: {
            int backendId;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "backendId");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &backendId) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                net->setPreferableBackend(backendId);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setPreferableTarget: {
            int targetId;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "targetId");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &targetId) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                net->setPreferableTarget(targetId);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setInput: {
            cv::Mat blob;
            char *name = NULL;
            int len;
            double scalefactor = 1.0;
            int count = 0, B = 0, G = 0, R = 0, A = 0;
            cv::Mat *mat;

            if (objc != 3 && objc != 6) {
                Tcl_WrongNumArgs(interp, 2, objv, "blob ?name scalefactor mean_color_list?");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            if (objc == 6) {
                /* name could be an empty string */
                name = Tcl_GetStringFromObj(objv[3], &len);

                if (Tcl_GetDoubleFromObj(interp, objv[4], &scalefactor) != TCL_OK) {
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
            }

            try {
                blob = *mat;
                if (objc == 3) {
                    net->setInput(blob);
                } else {
                    cv::Scalar color(B, G, R, A);
                    net->setInput(blob, name, scalefactor, color);
                }
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_forward: {
            cv::Mat result_mat;
            char *name = NULL;
            int len;
            Tcl_Obj *pResultStr = NULL;
            cv::Mat *mat;

            if (objc != 2 && objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "?name?");
                return TCL_ERROR;
            }

            if (objc == 3) {
                name = Tcl_GetStringFromObj(objv[2], &len);
                if (len < 1) {
                    return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid name");
                }
            }

            try {
                if (objc == 2) {
                    result_mat = net->forward();
                } else {
                    result_mat = net->forward(name);
                }
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            mat = new cv::Mat(result_mat);

            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, mat);

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_forwardWithNames: {
            std::vector<cv::String> outBlobNames;
            std::vector<cv::Mat> outputBlobs;
            int count = 0;
            Tcl_Obj *pResultStr = NULL;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "outBlobNames");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty outBlobNames list");
            } else {
                Tcl_Obj *elemListPtr = NULL;
                char *name;
                int len = 0;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                    name = Tcl_GetStringFromObj(elemListPtr, &len);
                    if (len < 1) {
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid name");
                    }

                    outBlobNames.push_back(name);
                }
            }

            try {
                net->forward(outputBlobs, outBlobNames);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            pResultStr = Tcl_NewListObj(outputBlobs.size(), NULL);

            for (size_t i = 0; i < outputBlobs.size(); i++) {
                cv::Mat *mat = new cv::Mat(outputBlobs[i]);
                Tcl_Obj *dstmat = Opencv_NewHandle(cd, interp, OPENCV_MAT, mat);

                Tcl_ListObjAppendElement(NULL, pResultStr, dstmat);
            }

            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            Opencv_CloseHandle(interp, cvo);

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

            Tcl_SetResult(interp, (char *) "cv::dnn:Net", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int dnn_readNet(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *model = NULL, *config = NULL, *framework = NULL;
    int len = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::dnn::Net *net;
    Tcl_DString ds1, ds2, ds3;

    if (objc != 2 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "model ?config framework?");
        return TCL_ERROR;
    }

    model = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid model name");
    }
    model = Tcl_UtfToExternalDString(NULL, model, len, &ds1);

    if (objc == 4) {
        config = Tcl_GetStringFromObj(objv[2], &len);
        if (len < 1) {
            Tcl_DStringFree(&ds1);
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid config name");
        }
        config = Tcl_UtfToExternalDString(NULL, config, len, &ds2);

        framework = Tcl_GetStringFromObj(objv[3], &len);
        if (len < 1) {
            Tcl_DStringFree(&ds1);
            Tcl_DStringFree(&ds2);
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid framework name");
        }
        framework = Tcl_UtfToExternalDString(NULL, framework, len, &ds3);
    } else {
        Tcl_DStringInit(&ds2);
        Tcl_DStringInit(&ds3);
    }

    try {
        net = new cv::dnn::Net();
        if (objc == 2) {
            *net = cv::dnn::readNet(model);
        } else {
            *net = cv::dnn::readNet(model, config, framework);
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        Tcl_DStringFree(&ds3);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        Tcl_DStringFree(&ds3);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);
    Tcl_DStringFree(&ds3);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_NDETECT, net);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int dnn_NMSBoxes(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Rect> bboxes;
    std::vector<cv::RotatedRect> bboxes_r;
    std::vector<float> scores;
    double  score_threshold;
    double  nms_threshold;
    std::vector<int> indices;
    double eta = 1.0;
    int top_k = 0, count = 0;
    Tcl_Obj *pResultStr = NULL;
    int isRect = 1;

    if (objc != 5 && objc != 7) {
        Tcl_WrongNumArgs(interp, 1, objv, "bboxes scores score_threshold nms_threshold ?eta top_k?");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty bboxes list");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int subCount = 0;

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &subCount) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (subCount != 4 && subCount != 5) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid rect list");
            } else {
                if (subCount == 4) {
                    isRect = 1;
                    Tcl_Obj *elemSubListPtr = NULL;
                    int left = 0, top = 0, width = 0, height = 0;

                    Tcl_ListObjIndex(interp, elemListPtr, 0, &elemSubListPtr);
                    if (Tcl_GetIntFromObj(interp, elemSubListPtr, &left) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, 1, &elemSubListPtr);
                    if (Tcl_GetIntFromObj(interp, elemSubListPtr, &top) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, 2, &elemSubListPtr);
                    if (Tcl_GetIntFromObj(interp, elemSubListPtr, &width) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, 3, &elemSubListPtr);
                    if (Tcl_GetIntFromObj(interp, elemSubListPtr, &height) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    bboxes.push_back(cv::Rect(left, top, width, height));
                } else {
                    isRect = 0;
                    Tcl_Obj *elemSubListPtr = NULL;
                    double center_x = 0, center_y = 0, width = 0, height = 0, angle = 0;

                    Tcl_ListObjIndex(interp, elemListPtr, 0, &elemSubListPtr);
                    if (Tcl_GetDoubleFromObj(interp, elemSubListPtr, &center_x) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, 1, &elemSubListPtr);
                    if (Tcl_GetDoubleFromObj(interp, elemSubListPtr, &center_y) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, 2, &elemSubListPtr);
                    if (Tcl_GetDoubleFromObj(interp, elemSubListPtr, &width) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, 3, &elemSubListPtr);
                    if (Tcl_GetDoubleFromObj(interp, elemSubListPtr, &height) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    Tcl_ListObjIndex(interp, elemListPtr, 4, &elemSubListPtr);
                    if (Tcl_GetDoubleFromObj(interp, elemSubListPtr, &angle) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    bboxes_r.push_back(cv::RotatedRect(cv::Point2f((float) center_x, (float) center_y),
                                                     cv::Size2f((float) width, (float) height),
                                                     angle));
                }
            }
        }
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty scores list");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        double score = 0;

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &score) != TCL_OK) {
                return TCL_ERROR;
            }

            scores.push_back((float) score);
        }
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &score_threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &nms_threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 7) {
        if (Tcl_GetDoubleFromObj(interp, objv[5], &eta) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[6], &top_k) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        if (isRect == 1) {
            cv::dnn::NMSBoxes(bboxes, scores,
                              (float) score_threshold,
                              (float) nms_threshold,
                              indices, (float) eta, top_k);
        } else {
            cv::dnn::NMSBoxes(bboxes_r, scores,
                              (float) score_threshold,
                              (float) nms_threshold,
                              indices, (float) eta, top_k);
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    pResultStr = Tcl_NewListObj(indices.size(), NULL);

    for (size_t i = 0; i < indices.size(); i++) {
        Tcl_ListObjAppendElement(NULL, pResultStr, Tcl_NewIntObj(indices[i]));
    }

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 4)
int dnn_softNMSBoxes(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    std::vector<cv::Rect> bboxes;
    std::vector<float> scores;
    std::vector<float> updated_scores;
    double  score_threshold;
    double  nms_threshold;
    std::vector<int> indices;
    double sigma = 0.5;
    int top_k = 0, method = (int) cv::dnn::SoftNMSMethod::SOFTNMS_GAUSSIAN, count = 0;

    if (objc != 5 && objc != 8) {
        Tcl_WrongNumArgs(interp, 1, objv, "bboxes scores score_threshold nms_threshold ?top_k sigma method?");
        return TCL_ERROR;
    }

    if (Tcl_ListObjLength(interp, objv[1], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty bboxes list");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        int subCount = 0;

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[1], i, &elemListPtr);

            if (Tcl_ListObjLength(interp, elemListPtr, &subCount) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (subCount != 4) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid rect list");
            } else {
                Tcl_Obj *elemSubListPtr = NULL;
                int left = 0, top = 0, width = 0, height = 0;

                Tcl_ListObjIndex(interp, elemListPtr, 0, &elemSubListPtr);
                if (Tcl_GetIntFromObj(interp, elemSubListPtr, &left) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 1, &elemSubListPtr);
                if (Tcl_GetIntFromObj(interp, elemSubListPtr, &top) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 2, &elemSubListPtr);
                if (Tcl_GetIntFromObj(interp, elemSubListPtr, &width) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, elemListPtr, 3, &elemSubListPtr);
                if (Tcl_GetIntFromObj(interp, elemSubListPtr, &height) != TCL_OK) {
                    return TCL_ERROR;
                }

                bboxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }

    if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
    }

    if (count == 0) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty scores list");
    } else {
        Tcl_Obj *elemListPtr = NULL;
        double score = 0;

        for (int i = 0; i < count; i++) {
            Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
            if (Tcl_GetDoubleFromObj(interp, elemListPtr, &score) != TCL_OK) {
                return TCL_ERROR;
            }

            scores.push_back((float) score);
        }
    }

    if (Tcl_GetDoubleFromObj(interp, objv[3], &score_threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (Tcl_GetDoubleFromObj(interp, objv[4], &nms_threshold) != TCL_OK) {
        return TCL_ERROR;
    }

    if (objc == 8) {
        if (Tcl_GetIntFromObj(interp, objv[5], &top_k) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetDoubleFromObj(interp, objv[6], &sigma) != TCL_OK) {
            return TCL_ERROR;
        }

        if (Tcl_GetIntFromObj(interp, objv[7], &method) != TCL_OK) {
            return TCL_ERROR;
        }
    }

    try {
        cv::dnn::softNMSBoxes(bboxes, scores, updated_scores,
                        (float) score_threshold,
                        (float) nms_threshold,
                        indices, top_k, (float) sigma,
                        (cv::dnn::SoftNMSMethod) method);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }

    Tcl_Obj *list[2];

    list[0] =  Tcl_NewListObj(updated_scores.size(), NULL);
    for (size_t i = 0; i < updated_scores.size(); i++) {
        Tcl_ListObjAppendElement(NULL, list[0], Tcl_NewDoubleObj(updated_scores[i]));
    }

    list[1] =  Tcl_NewListObj(indices.size(), NULL);
    for (size_t i = 0; i < indices.size(); i++) {
        Tcl_ListObjAppendElement(NULL, list[1], Tcl_NewIntObj(indices[i]));
    }

    Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));

    return TCL_OK;
}
#endif


#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 1)
int TEXTDETECT_EAST_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::dnn::TextDetectionModel_EAST *east;

    static const char *FUNC_strs[] = {
        "detect",
        "getConfidenceThreshold",
        "getNMSThreshold",
        "setConfidenceThreshold",
        "setNMSThreshold",
        "setInputParams",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_detect,
        FUNC_getConfidenceThreshold,
        FUNC_getNMSThreshold,
        FUNC_setConfidenceThreshold,
        FUNC_setNMSThreshold,
        FUNC_setInputParams,
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

    cd = (void *) cvo->top;
    east = (cv::dnn::TextDetectionModel_EAST *) cvo->obj;
    if (!east) {
        Tcl_Panic("null TextDetectionModel_EAST object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_detect: {
            cv::Mat *mat;
            std::vector<std::vector<cv::Point>> detections;
            std::vector<float> confidences;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                east->detect(*mat, detections, confidences);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] =  Tcl_NewListObj(detections.size(), NULL);
            for (size_t i = 0; i < detections.size(); i++) {
                Tcl_Obj *pListResultStr = Tcl_NewListObj(0, NULL);

                for (size_t j = 0; j < detections[i].size(); j++) {
                    Tcl_ListObjAppendElement(NULL, pListResultStr, Tcl_NewIntObj(detections[i][j].x));
                    Tcl_ListObjAppendElement(NULL, pListResultStr, Tcl_NewIntObj(detections[i][j].y));
                }

                Tcl_ListObjAppendElement(NULL, list[0], pListResultStr);
            }

            list[1] =  Tcl_NewListObj(confidences.size(), NULL);
            for (size_t i = 0; i < confidences.size(); i++) {
                Tcl_ListObjAppendElement(NULL, list[1], Tcl_NewDoubleObj(confidences[i]));
            }

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));
            break;
        }
        case FUNC_getConfidenceThreshold: {
            double threshold = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                threshold = east->getConfidenceThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(threshold));
            break;
        }
        case FUNC_getNMSThreshold: {
            double threshold = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                threshold = east->getNMSThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(threshold));
            break;
        }
        case FUNC_setConfidenceThreshold: {
            double threshold = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &threshold) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                *east = east->setConfidenceThreshold((float) threshold);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setNMSThreshold: {
            double threshold = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &threshold) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                *east = east->setNMSThreshold((float) threshold);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setInputParams: {
            double scalefactor = 1.0, B = 0, G = 0, R = 0, A = 0;;
            int width = 0, height = 0, swapRB = 0, crop = 0, count = 0;

            if (objc != 7 && objc != 8) {
                Tcl_WrongNumArgs(interp, 1, objv,
                                "scalefactor width height mean_color_list swapRB ?crop?");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &scalefactor) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
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
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &B) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &G) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &R) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &A) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            if (Tcl_GetBooleanFromObj(interp, objv[6], &swapRB) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 8) {
                if (Tcl_GetBooleanFromObj(interp, objv[7], &crop) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cv::Scalar color(B, G, R, A);
                cv::Size size(width, height);

                east->setInputParams(scalefactor, size, color, (bool) swapRB, (bool) crop);
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

            Opencv_CloseHandle(interp, cvo);

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

            Tcl_SetResult(interp, (char *) "cv::dnn:TextDetectionModel_EAST", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int TextDetectionModel_EAST(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *model = NULL, *config = NULL;
    int len = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::dnn::TextDetectionModel_EAST *east;
    Tcl_DString ds1, ds2;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "model ?config?");
        return TCL_ERROR;
    }

    model = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid model name");
    }
    model = Tcl_UtfToExternalDString(NULL, model, len, &ds1);

    if (objc == 3) {
        config = Tcl_GetStringFromObj(objv[2], &len);
        if (len < 1) {
            Tcl_DStringFree(&ds1);
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid config name");
        }
        config = Tcl_UtfToExternalDString(NULL, config, len, &ds2);
    } else {
        Tcl_DStringInit(&ds2);
    }

    try {
        if (objc == 2) {
            east = new cv::dnn::TextDetectionModel_EAST(model);
        } else {
            east = new cv::dnn::TextDetectionModel_EAST(model, config);
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_TEXTDETECTEAST, east);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int TEXTDETECT_DB_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::dnn::TextDetectionModel_DB *db;

    static const char *FUNC_strs[] = {
        "detect",
        "getBinaryThreshold",
        "getMaxCandidates",
        "getPolygonThreshold",
        "getUnclipRatio",
        "setBinaryThreshold",
        "setMaxCandidates",
        "setPolygonThreshold",
        "setUnclipRatio",
        "setInputParams",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_detect,
        FUNC_getBinaryThreshold,
        FUNC_getMaxCandidates,
        FUNC_getPolygonThreshold,
        FUNC_getUnclipRatio,
        FUNC_setBinaryThreshold,
        FUNC_setMaxCandidates,
        FUNC_setPolygonThreshold,
        FUNC_setUnclipRatio,
        FUNC_setInputParams,
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

    cd = (void *) cvo->top;
    db = (cv::dnn::TextDetectionModel_DB *) cvo->obj;
    if (!db) {
        Tcl_Panic("null TextDetectionModel_DB object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_detect: {
            cv::Mat *mat;
            std::vector<std::vector<cv::Point>> detections;
            std::vector<float> confidences;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                db->detect(*mat, detections, confidences);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_Obj *list[2];

            list[0] =  Tcl_NewListObj(detections.size(), NULL);
            for (size_t i = 0; i < detections.size(); i++) {
                Tcl_Obj *pListResultStr = Tcl_NewListObj(0, NULL);

                for (size_t j = 0; j < detections[i].size(); j++) {
                    Tcl_ListObjAppendElement(NULL, pListResultStr, Tcl_NewIntObj(detections[i][j].x));
                    Tcl_ListObjAppendElement(NULL, pListResultStr, Tcl_NewIntObj(detections[i][j].y));
                }

                Tcl_ListObjAppendElement(NULL, list[0], pListResultStr);
            }

            list[1] =  Tcl_NewListObj(confidences.size(), NULL);
            for (size_t i = 0; i < confidences.size(); i++) {
                Tcl_ListObjAppendElement(NULL, list[1], Tcl_NewDoubleObj(confidences[i]));
            }

            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));
            break;
        }
        case FUNC_getBinaryThreshold: {
            double value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = db->getBinaryThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getMaxCandidates: {
            int value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = db->getMaxCandidates();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewIntObj(value));
            break;
        }
        case FUNC_getPolygonThreshold: {
            double value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = db->getPolygonThreshold();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_getUnclipRatio: {
            double value = 0;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = db->getUnclipRatio();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewDoubleObj(value));
            break;
        }
        case FUNC_setBinaryThreshold: {
            double value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                *db = db->setBinaryThreshold((float) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setMaxCandidates: {
            int value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                *db = db->setMaxCandidates(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setPolygonThreshold: {
            double value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                *db = db->setPolygonThreshold((float) value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setUnclipRatio: {
            double value = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &value) != TCL_OK) {
                return TCL_ERROR;
            }

            try {
                *db = db->setUnclipRatio(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setInputParams: {
            double scalefactor = 1.0, B = 0, G = 0, R = 0, A = 0;;
            int width = 0, height = 0, swapRB = 0, crop = 0, count = 0;

            if (objc != 7 && objc != 8) {
                Tcl_WrongNumArgs(interp, 1, objv,
                                "scalefactor width height mean_color_list swapRB ?crop?");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &scalefactor) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
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
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &B) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &G) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &R) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &A) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            if (Tcl_GetBooleanFromObj(interp, objv[6], &swapRB) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 8) {
                if (Tcl_GetBooleanFromObj(interp, objv[7], &crop) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cv::Scalar color(B, G, R, A);
                cv::Size size(width, height);

                db->setInputParams(scalefactor, size, color, (bool) swapRB, (bool) crop);
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

            Opencv_CloseHandle(interp, cvo);

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

            Tcl_SetResult(interp, (char *) "cv::dnn:TextDetectionModel_DB", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int TextDetectionModel_DB(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *model = NULL, *config = NULL;
    int len = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::dnn::TextDetectionModel_DB *db;
    Tcl_DString ds1, ds2;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "model ?config?");
        return TCL_ERROR;
    }

    model = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid model name");
    }
    model = Tcl_UtfToExternalDString(NULL, model, len, &ds1);

    if (objc == 3) {
        config = Tcl_GetStringFromObj(objv[2], &len);
        if (len < 1) {
            Tcl_DStringFree(&ds1);
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid config name");
        }
        config = Tcl_UtfToExternalDString(NULL, config, len, &ds2);
    } else {
        Tcl_DStringInit(&ds2);
    }

    try {
        if (objc == 2) {
            db = new cv::dnn::TextDetectionModel_DB(model);
        } else {
            db = new cv::dnn::TextDetectionModel_DB(model, config);
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_TEXTDETECTDB, db);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}


int TEXTRECOGNITION_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    int choice;
    cv::dnn::TextRecognitionModel *recognizer;

    static const char *FUNC_strs[] = {
        "recognize",
        "getDecodeType",
        "getVocabulary",
        "setDecodeType",
        "setVocabulary",
        "setInputParams",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_recognize,
        FUNC_getDecodeType,
        FUNC_getVocabulary,
        FUNC_setDecodeType,
        FUNC_setVocabulary,
        FUNC_setInputParams,
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

    cd = (void *) cvo->top;
    recognizer = (cv::dnn::TextRecognitionModel *) cvo->obj;
    if (!recognizer) {
        Tcl_Panic("null TextRecognitionModel object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_recognize: {
            cv::Mat *mat;
            std::string value;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "matrix");
                return TCL_ERROR;
            }

            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[2]);
            if (!mat) {
                return TCL_ERROR;
            }

            try {
                value = recognizer->recognize(*mat);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(value.c_str(), value.length()));
            break;
        }
        case FUNC_getDecodeType: {
            std::string value;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                value = recognizer->getDecodeType();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            Tcl_SetObjResult(interp, Tcl_NewStringObj(value.c_str(), value.length()));
            break;
        }
        case FUNC_getVocabulary: {
            std::vector<std::string> vocabulary;
            Tcl_Obj *listPtr;

            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }

            try {
                vocabulary = recognizer->getVocabulary();
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            listPtr = Tcl_NewListObj(vocabulary.size(), NULL);
            for (size_t i = 0; i < vocabulary.size(); i++) {
                std::string value = vocabulary[i];
                Tcl_ListObjAppendElement(NULL, listPtr, Tcl_NewStringObj(value.c_str(), value.length()));
            }

            Tcl_SetObjResult(interp, listPtr);

            break;
        }
        case FUNC_setDecodeType: {
            char *value = NULL;
            int len = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "value");
                return TCL_ERROR;
            }

            value = Tcl_GetStringFromObj(objv[2], &len);
            if (len < 1) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid value");
            }

            try {
                /*
                 * The decoding method of translating the network output into string:
                 * {'CTC-greedy': greedy decoding for the output of CTC-based methods}
                 */
                *recognizer = recognizer->setDecodeType(value);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setVocabulary: {
            std::vector<std::string> vocabulary;
            int count = 0;

            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "vocabulary");
                return TCL_ERROR;
            }

            if (Tcl_ListObjLength(interp, objv[2], &count) != TCL_OK) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid list data");
            }

            if (count == 0) {
                return Opencv_SetResult(interp, cv::Error::StsBadArg, "empty list");
            } else {
                Tcl_Obj *elemListPtr = NULL;
                char *vocstring = NULL;
                int len = 0;

                for (int i = 0; i < count; i++) {
                    Tcl_ListObjIndex(interp, objv[2], i, &elemListPtr);
                    vocstring = Tcl_GetStringFromObj(elemListPtr, &len);
                    if (len < 1) {
                        return TCL_ERROR;
                    }

                    vocabulary.push_back(vocstring);
                }
            }

            try {
                *recognizer = recognizer->setVocabulary(vocabulary);
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }

            break;
        }
        case FUNC_setInputParams: {
            double scalefactor = 1.0, B = 0, G = 0, R = 0, A = 0;;
            int width = 0, height = 0, swapRB = 0, crop = 0, count = 0;

            if (objc != 7 && objc != 8) {
                Tcl_WrongNumArgs(interp, 1, objv,
                                "scalefactor width height mean_color_list swapRB ?crop?");
                return TCL_ERROR;
            }

            if (Tcl_GetDoubleFromObj(interp, objv[2], &scalefactor) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[3], &width) != TCL_OK) {
                return TCL_ERROR;
            }

            if (Tcl_GetIntFromObj(interp, objv[4], &height) != TCL_OK) {
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
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &B) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 1, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &G) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 2, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &R) != TCL_OK) {
                    return TCL_ERROR;
                }

                Tcl_ListObjIndex(interp, objv[5], 3, &elemListPtr);
                if (Tcl_GetDoubleFromObj(interp, elemListPtr, &A) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            if (Tcl_GetBooleanFromObj(interp, objv[6], &swapRB) != TCL_OK) {
                return TCL_ERROR;
            }

            if (objc == 8) {
                if (Tcl_GetBooleanFromObj(interp, objv[7], &crop) != TCL_OK) {
                    return TCL_ERROR;
                }
            }

            try {
                cv::Scalar color(B, G, R, A);
                cv::Size size(width, height);

                recognizer->setInputParams(scalefactor, size, color, (bool) swapRB, (bool) crop);
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

            Opencv_CloseHandle(interp, cvo);

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

            Tcl_SetResult(interp, (char *) "cv::dnn:TextRecognitionModel", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int TextRecognitionModel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *model = NULL, *config = NULL;
    int len = 0;
    Tcl_Obj *pResultStr = NULL;
    cv::dnn::TextRecognitionModel *recognizer;
    Tcl_DString ds1, ds2;

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "model ?config?");
        return TCL_ERROR;
    }

    model = Tcl_GetStringFromObj(objv[1], &len);
    if (len < 1) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid model name");
    }
    model = Tcl_UtfToExternalDString(NULL, model, len, &ds1);

    if (objc == 3) {
        config = Tcl_GetStringFromObj(objv[2], &len);
        if (len < 1) {
            Tcl_DStringFree(&ds1);
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid config name");
        }
        config = Tcl_UtfToExternalDString(NULL, config, len, &ds2);
    } else {
        Tcl_DStringInit(&ds2);
    }

    try {
        if (objc == 2) {
            recognizer = new cv::dnn::TextRecognitionModel(model);
        } else {
            recognizer = new cv::dnn::TextRecognitionModel(model, config);
        }
    } catch (const cv::Exception &ex) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        Tcl_DStringFree(&ds1);
        Tcl_DStringFree(&ds2);
        return Opencv_Exc2Tcl(interp, NULL);
    }
    Tcl_DStringFree(&ds1);
    Tcl_DStringFree(&ds2);

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_TEXTRECOGNITION, recognizer);

    Tcl_SetObjResult(interp, pResultStr);

    return TCL_OK;
}
#endif
#endif /* TCL_USE_OPENCV4 */

#ifdef __cplusplus
}
#endif
