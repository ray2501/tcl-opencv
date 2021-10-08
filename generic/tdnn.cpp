#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TCL_USE_OPENCV4

int dnn_blobFromImage(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::Mat dstimage;
    double scalefactor = 1.0;
    int width = 0, height = 0, swapRB = 0, crop = 0;
    int count = 0, B = 0, G = 0, R = 0, A = 0;
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
        "setPreferableBackend",
        "setPreferableTarget",
        "setInput",
        "forward",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_getLayerNames,
        FUNC_getUnconnectedOutLayers,
        FUNC_setPreferableBackend,
        FUNC_setPreferableTarget,
        FUNC_setInput,
        FUNC_forward,
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

            pResultStr = Tcl_NewListObj(0, NULL);

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

            pResultStr = Tcl_NewListObj(0, NULL);

            for (size_t i = 0; i < value.size(); i++) {
                Tcl_ListObjAppendElement(NULL, pResultStr,
                                         Tcl_NewIntObj(value[i]));
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

#endif /* TCL_USE_OPENCV4 */

#ifdef __cplusplus
}
#endif
