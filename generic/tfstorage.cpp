#include "tclopencv.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif


int FileStorage_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    Opencv_Data *cvd = cvo->top;
    int choice;
    cv::FileStorage *fs;

    static const char *FUNC_strs[] = {
        "open",
        "keys",
        "startMap",
        "startSeq",
        "endMap",
        "endSeq",
        "readDouble",
        "readInt",
        "readMat",
        "readObj",
        "readString",
        "writeDouble",
        "writeInt",
        "writeMat",
        "writeObj",
        "writeString",
        "close",
        "_command",
        "_name",
        "_type",
        0
    };

    enum FUNC_enum {
        FUNC_OPEN,
        FUNC_KEYS,
        FUNC_STARTM,
        FUNC_STARTS,
        FUNC_ENDM,
        FUNC_ENDS,
        FUNC_RDDBL,
        FUNC_RDINT,
        FUNC_RDMAT,
        FUNC_RDOBJ,
        FUNC_RDSTR,
        FUNC_WRDBL,
        FUNC_WRINT,
        FUNC_WRMAT,
        FUNC_WROBJ,
        FUNC_WRSTR,
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
    fs = (cv::FileStorage *) cvo->obj;
    if (!fs) {
        Tcl_Panic("null FileStorage object");
    }

    switch ((enum FUNC_enum)choice) {
        case FUNC_OPEN: {
            char *filename;
            Tcl_Size len;
            int mode;

            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "filename|data mode");
                return TCL_ERROR;
            }
            if (Tcl_GetIntFromObj(interp, objv[3], &mode) != TCL_OK) {
                return TCL_ERROR;
            }
            if ((mode & (cv::FileStorage::READ | cv::FileStorage::MEMORY)) ==
                (cv::FileStorage::READ | cv::FileStorage::MEMORY)) {
                filename = (char *) Tcl_GetByteArrayFromObj(objv[2], &len);
                Tcl_DStringFree(&cvo->ds1);
                Tcl_DStringSetLength(&cvo->ds1, 0);
                filename = Tcl_DStringAppend(&cvo->ds1, filename, len);
            } else {
                filename = Tcl_GetStringFromObj(objv[2], &len);
                if ((mode & (cv::FileStorage::WRITE | cv::FileStorage::MEMORY)) !=
                    (cv::FileStorage::WRITE | cv::FileStorage::MEMORY) && len < 1) {
                    return Opencv_SetResult(interp, cv::Error::StsBadArg, "invalid filename or data");
                }
                Tcl_DStringFree(&cvo->ds1);
                filename = Tcl_UtfToExternalDString(NULL, filename, len, &cvo->ds1);
            }
            if (!fs->open(filename, mode)) {
                Tcl_DStringFree(&cvo->ds1);
                return Opencv_SetResult(interp, cv::Error::StsError, "open failed");
            }
            cvo->flags = mode;
            break;
        }
        case FUNC_KEYS: {
            if (objc < 2) {
                Tcl_WrongNumArgs(interp, 2, objv, "?name ...?");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            Tcl_Encoding enc = Tcl_GetEncoding(NULL, "utf-8");
            Tcl_DString ds;
            Tcl_DStringInit(&ds);
            Tcl_Obj *listPtr = Tcl_NewListObj(0, NULL);
            /* WARNING: yields list of empty names in OpenCV 3.2 on Debian 10 */
            try {
                const char *name = NULL;
                cv::FileNode node;
                Tcl_Size len;
                if (objc == 2) {
                    node = fs->root();
                } else {
                    cv::FileNode dir = fs->root();
                    for (int i = 2; i < objc; i++) {
                        name = Tcl_GetStringFromObj(objv[i], &len);
                        name = Tcl_UtfToExternalDString(enc, name, len, &ds);
                        node = dir[name];
                        dir = node;
                        Tcl_DStringFree(&ds);
                    }
                }
                cv::FileNodeIterator it = node.begin(), it_end = node.end();
                while (it != it_end) {
                    cv::String nodename = (*it).name();

                    Tcl_ExternalToUtfDString(enc, nodename.c_str(), nodename.length(), &ds);
                    Tcl_ListObjAppendElement(NULL, listPtr,
                        Tcl_NewStringObj(Tcl_DStringValue(&ds), Tcl_DStringLength(&ds)));
                    Tcl_DStringFree(&ds);
                    it++;
                }
            } catch (const cv::Exception &ex) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            Tcl_SetObjResult(interp, listPtr);
            break;
        }
        case FUNC_STARTM:
        case FUNC_STARTS: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            if (!(cvo->flags & (cv::FileStorage::WRITE | cv::FileStorage::APPEND))) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened for writing");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            Tcl_FreeEncoding(enc);
            try {
                if (choice == FUNC_STARTS) {
                    (*fs) << name << "[";
                    Tcl_DStringAppend(&cvo->ds2, "]", 1);
                } else {
                    (*fs) << name << "{";
                    Tcl_DStringAppend(&cvo->ds2, "}", 1);
                }
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
        case FUNC_ENDM:
        case FUNC_ENDS: {
            char marker[2];
            int mlen;
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, NULL);
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            if (!(cvo->flags & (cv::FileStorage::WRITE | cv::FileStorage::APPEND))) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened for writing");
            }
            mlen = Tcl_DStringLength(&cvo->ds2) - 1;
            if (mlen < 0) {
                return Opencv_SetResult(interp, cv::Error::StsError, "no seq/map to end");
            }
            marker[0] = Tcl_DStringValue(&cvo->ds2)[mlen];
            marker[1] = '\0';
            if ((marker[0] == ']' && choice != FUNC_ENDS) ||
                (marker[0] == '}' && choice != FUNC_ENDM))  {
                return Opencv_SetResult(interp, cv::Error::StsError, "seq/map mismatch");
            }
            try {
                (*fs) << (const char *) marker;
            } catch (const cv::Exception &ex) {
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_DStringSetLength(&cvo->ds2, mlen);
            break;
        }
        case FUNC_RDDBL: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            Tcl_Obj *listPtr;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            listPtr = Tcl_NewListObj(0, NULL);
            try {
                double value;
                cv::FileNode node = (*fs)[name];

                for (int i = 3; i < objc; i++) {
                    Tcl_DStringFree(&ds);
                    name = Tcl_GetStringFromObj(objv[i], &nlen);
                    name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
                    node = node[name];
                }
                if (!(node.isSeq() || node.isReal())) {
                    CV_Error(cv::Error::StsParseError, "sequence or number expected");
                }
                if (node.isSeq()) {
                    for (size_t i = 0; i < node.size(); i++) {
                        value = node[i];
                        Tcl_ListObjAppendElement(NULL, listPtr, Tcl_NewDoubleObj(value));
                    }
                } else {
                    value = node.real();
                    Tcl_ListObjAppendElement(NULL, listPtr, Tcl_NewDoubleObj(value));
                }
            } catch (const cv::Exception &ex) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            Tcl_SetObjResult(interp, listPtr);
            break;
        }
        case FUNC_RDINT: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            Tcl_Obj *listPtr;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            listPtr = Tcl_NewListObj(0, NULL);
            try {
                int value;
                cv::FileNode node = (*fs)[name];

                for (int i = 3; i < objc; i++) {
                    Tcl_DStringFree(&ds);
                    name = Tcl_GetStringFromObj(objv[i], &nlen);
                    name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
                    node = node[name];
                }
                if (!(node.isSeq() || !node.isInt())) {
                    CV_Error(cv::Error::StsParseError, "sequence or integer expected");
                }
                if (node.isSeq()) {
                    for (size_t i = 0; i < node.size(); i++) {
                        node[i] >> value;
                        Tcl_ListObjAppendElement(NULL, listPtr, Tcl_NewIntObj(value));
                    }
                } else {
                    node >> value;
                    Tcl_ListObjAppendElement(NULL, listPtr, Tcl_NewIntObj(value));
                }
            } catch (const cv::Exception &ex) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            Tcl_SetObjResult(interp, listPtr);
            break;
        }
        case FUNC_RDMAT: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            cv::Mat rmat, *mat;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            try {
                cv::FileNode node = (*fs)[name];

                for (int i = 3; i < objc; i++) {
                    Tcl_DStringFree(&ds);
                    name = Tcl_GetStringFromObj(objv[i], &nlen);
                    name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
                    node = node[name];
                }
                if (node.isNone() || node.isSeq()) {
                    CV_Error(cv::Error::StsParseError, "wrong node type");
                }
                rmat = node.mat();
            } catch (const cv::Exception &ex) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            mat = new cv::Mat(rmat);
            Opencv_NewHandleResult(cd, interp, OPENCV_MAT, mat);
            break;
        }
        case FUNC_RDOBJ: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            Tcl_Obj *empty;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            empty = Tcl_NewObj();
            Tcl_IncrRefCount(empty);
            int keepInterpErr = 0;
            try {
                cv::FileNode node = (*fs)[name];
                cv::String typestring;
                const char *type;
#ifdef TCL_USE_SIFT
                const char *backend = NULL;
#endif

                for (int i = 3; i < objc; i++) {
                    Tcl_DStringFree(&ds);
                    name = Tcl_GetStringFromObj(objv[i], &nlen);
                    name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
                    node = node[name];
                }
                if (node.isNone() || !node.isMap()) {
                    CV_Error(cv::Error::StsParseError, "wrong node type");
                }
                typestring = node["type"].string();
                type = typestring.c_str();
#ifdef TCL_USE_SIFT
                if (node["backend"].isString()) {
                    cv::String str = node["backend"].string();
                    backend = str.c_str();
                }
#endif
                node = node["data"];
                if (!(node.isMap() || node.isNone()) || type == NULL) {
                    CV_Error(cv::Error::StsParseError, "wrong node type or unknown data type");
                } else if (strcmp(type, "opencv-matrix") == 0) {
                    cv::Mat *mat;
                    if (node.isNone()) {
                        mat = new cv::Mat();
                    } else {
                        mat = new cv::Mat(node.mat());
                    }
                    keepInterpErr = 1;
                    Opencv_NewHandleResult(cd, interp, OPENCV_MAT, mat);
                } else if (strcmp(type, "opencv-pca") == 0) {
                    cv::PCA *pca = new cv::PCA();
                    if (!node.isNone()) {
                        pca->read(node);
                    }
                    keepInterpErr = 1;
                    Opencv_NewHandleResult(cd, interp, OPENCV_PCA, pca);
                } else if (strcmp(type, "opencv-cascadeclassifier") == 0) {
                    cv::CascadeClassifier *cas = new cv::CascadeClassifier();
                    if (!node.isNone()) {
                        cas->read(node);
                    }
                    keepInterpErr = 1;
                    Opencv_NewHandleResult(cd, interp, OPENCV_ODETECT, cas);
                } else if (strcmp(type, "opencv-hogdescriptor") == 0) {
                    cv::HOGDescriptor *hog = new cv::HOGDescriptor();
                    if (!node.isNone()) {
                        hog->read(node);
                    }
                    keepInterpErr = 1;
                    Opencv_NewHandleResult(cd, interp, OPENCV_OOBJHOG, hog);
                } else if (strcmp(type, "opencv-termcriteria") == 0) {
                    cv::TermCriteria *tc = new cv::TermCriteria();
                    if (node.isMap()) {
                        node["type"] >> tc->type;
                        node["maxcount"] >> tc->maxCount;
                        node["epsilon"] >> tc->epsilon;
                    }
                    keepInterpErr = 1;
                    Opencv_NewHandleResult(cd, interp, OPENCV_TERMCRITERIA, tc);
                } else if (strcmp(type, "opencv-clahe") == 0) {
                    if (CLAHE(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->clahe->read(node);
                    }
                } else if (strcmp(type, "opencv-houghballard") == 0) {
                    if (GeneralizedHoughBallard(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->houghballard->read(node);
                    }
                } else if (strcmp(type, "opencv-houghbuil") == 0) {
                    if (GeneralizedHoughGuil(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->houghbuil->read(node);
                    }
                } else if (strcmp(type, "opencv-lsdetector") == 0) {
                    if (LineSegmentDetector(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->lsdetector->read(node);
                    }
                } else if (strcmp(type, "opencv-fastfeaturedetector") == 0) {
                    if (FastFeatureDetector(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->fastdetector->read(node);
                    }
                } else if (strcmp(type, "opencv-agastfeaturedetector") == 0) {
                    if (AgastFeatureDetector(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->agastdetector->read(node);
                    }
                } else if (strcmp(type, "opencv-mserextractor") == 0) {
                    if (MSER(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->mserextractor->read(node);
                    }
                } else if (strcmp(type, "opencv-orb") == 0) {
                    if (ORB(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->orbdetector->read(node);
                    }
                } else if (strcmp(type, "opencv-akaze") == 0) {
                    if (AKAZE(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->akazedetector->read(node);
                    }
                } else if (strcmp(type, "opencv-brisk") == 0) {
                    if (BRISK(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->briskdetector->read(node);
                    }
                } else if (strcmp(type, "opencv-kaze") == 0) {
                    if (KAZE(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->kazedetector->read(node);
                    }
#ifdef TCL_USE_SIFT
                } else if (strcmp(type, "opencv-sift") == 0) {
                    if (SIFT(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->siftdetector->read(node);
                    }
                } else if (strcmp(type, "opencv-asift") == 0) {
                    Tcl_Obj *args[2];
                    Tcl_DString ds;
                    int result;

                    /* Proper name for constructor: prefix is "::cv-", strip "opencv-", add "detector" */
                    Tcl_DStringInit(&ds);
                    if (backend) {
                        Tcl_DStringAppend(&ds, "::cv-", -1);
                        Tcl_DStringAppend(&ds, backend + 7, -1);
                        Tcl_DStringAppend(&ds, "detector", -1);
                    }
                    args[0] = empty;
                    args[1] = Tcl_NewStringObj(Tcl_DStringValue(&ds), -1);
                    Tcl_DStringFree(&ds);
                    Tcl_IncrRefCount(args[1]);
                    result = AffineFeature(cd, interp, 2, args);
                    Tcl_DecrRefCount(args[1]);
                    if (result != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->asiftdetector->read(node);
                    }
#endif
                } else if (strcmp(type, "opencv-bfmatcher") == 0) {
                    if (BFMatcher(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->bfmatcher->read(node);
                    }
                } else if (strcmp(type, "opencv-flannbasedmatcher") == 0) {
                    if (FlannBasedMatcher(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->flannbasedmatcher->read(node);
                    }
                } else if (strcmp(type, "opencv-simpleblobdetector") == 0) {
                    if (SimpleBlobDetector(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->sbdetector->read(node);
                    }
                } else if (strcmp(type, "opencv-backgroundsubtractorknn") == 0) {
                    if (BackgroundSubtractorKNN(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->bgsknn->read(node);
                    }
                } else if (strcmp(type, "opencv-backgroundsubtractormog2") == 0) {
                    if (BackgroundSubtractorMOG2(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->bgsmog2->read(node);
                    }
                } else if (strcmp(type, "opencv-stereobm") == 0) {
                    if (StereoBM(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->stereobm->read(node);
                    }
                } else if (strcmp(type, "opencv-stereosgbm") == 0) {
                    if (StereoSGBM(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->stereosgbm->read(node);
                    }
                } else if (strcmp(type, "opencv-alignmtb") == 0) {
                    if (AlignMTB(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->alignmtb->read(node);
                    }
                } else if (strcmp(type, "opencv-calibratedebevec") == 0) {
                    if (CalibrateDebevec(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->calibdeb->read(node);
                    }
                } else if (strcmp(type, "opencv-mergedebevec") == 0) {
                    if (MergeDebevec(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->mergedeb->read(node);
                    }
                } else if (strcmp(type, "opencv-mergemertens") == 0) {
                    if (MergeMertens(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->mergemer->read(node);
                    }
                } else if (strcmp(type, "opencv-tonemapdrago") == 0) {
                    if (TonemapDrago(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->tonemapdra->read(node);
                    }
                } else if (strcmp(type, "opencv-tonemapmantiuk") == 0) {
                    if (TonemapMantiuk(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->tonemapman->read(node);
                    }
                } else if (strcmp(type, "opencv-tonemapreinhard") == 0) {
                    if (TonemapReinhard(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->tonemaprei->read(node);
                    }
                } else if (strcmp(type, "opencv-logisticregression") == 0) {
                    if (LogisticRegression(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->logistic->read(node);
                    }
                } else if (strcmp(type, "opencv-normalbayesclassifier") == 0) {
                    if (NormalBayesClassifier(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->bayesclassifier->read(node);
                    }
                } else if (strcmp(type, "opencv-knearest") == 0) {
                    if (KNearest(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->knearest->read(node);
                    }
                } else if (strcmp(type, "opencv-svm") == 0) {
                    if (SVM(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->svm->read(node);
                    }
                } else if (strcmp(type, "opencv-svmsgd") == 0) {
                    if (SVMSGD(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->svmsgd->read(node);
                    }
                } else if (strcmp(type, "opencv-dtrees") == 0) {
                    if (DTrees(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->dtrees->read(node);
                    }
                } else if (strcmp(type, "opencv-boost") == 0) {
                    if (Boost(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->boost->read(node);
                    }
                } else if (strcmp(type, "opencv-rtrees") == 0) {
                    if (RTrees(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->rtrees->read(node);
                    }
                } else if (strcmp(type, "opencv-annmlp") == 0) {
                    if (ANN_MLP(cd, interp, 1, &empty) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                    if (!node.isNone()) {
                        cvd->annmlp->read(node);
                    }
                } else if (strcmp(type, "opencv-traindata") == 0) {
                    cv::Mat samples, responses;
                    int layout;
                    if (!node["samples"].isNone()) {
                        samples = node["samples"].mat();
                    }
                    node["layout"] >> layout;
                    if (!node["responses"].isNone()) {
                        responses = node["responses"].mat();
                    }
                    if (TrainData_CONSTRUCTOR(cd, interp, samples, layout, responses) != TCL_OK) {
                        keepInterpErr = 1;
                        throw cv::Exception();
                    }
                } else {
                    CV_Error(cv::Error::StsParseError, "unsupported data type");
                }
            } catch (const cv::Exception &ex) {
                Tcl_DecrRefCount(empty);
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                if (!keepInterpErr) {
                    return Opencv_Exc2Tcl(interp, &ex);
                }
                return TCL_ERROR;
            } catch (...) {
                Tcl_DecrRefCount(empty);
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_DecrRefCount(empty);
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            break;
        }
        case FUNC_RDSTR: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds, ds2;
            Tcl_Encoding enc;
            Tcl_Obj *listPtr;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            listPtr = Tcl_NewListObj(0, NULL);
            Tcl_DStringInit(&ds2);
            try {
                cv::String value;
                cv::FileNode node = (*fs)[name];

                for (int i = 3; i < objc; i++) {
                    Tcl_DStringFree(&ds);
                    name = Tcl_GetStringFromObj(objv[i], &nlen);
                    name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
                    node = node[name];
                }
                if (!(node.isSeq() || !node.isString())) {
                    CV_Error(cv::Error::StsParseError, "sequence or string expected");
                }
                if (node.isSeq()) {
                    for (size_t i = 0; i < node.size(); i++) {
                        value = node[i].string();
                        Tcl_ExternalToUtfDString(enc, value.c_str(), value.length(), &ds2);
                        Tcl_ListObjAppendElement(NULL, listPtr,
                                Tcl_NewStringObj(Tcl_DStringValue(&ds2), Tcl_DStringLength(&ds2)));
                        Tcl_DStringFree(&ds2);
                    }
                } else {
                    value = node.string();
                    Tcl_ExternalToUtfDString(enc, value.c_str(), value.length(), &ds2);
                    Tcl_ListObjAppendElement(NULL, listPtr,
                            Tcl_NewStringObj(Tcl_DStringValue(&ds2), Tcl_DStringLength(&ds2)));
                    Tcl_DStringFree(&ds2);
                }
            } catch (const cv::Exception &ex) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DStringFree(&ds2);
                Tcl_DecrRefCount(listPtr);
                return Opencv_Exc2Tcl(interp, &ex);
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DStringFree(&ds2);
                Tcl_DecrRefCount(listPtr);
                return Opencv_Exc2Tcl(interp, NULL);
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            Tcl_DStringFree(&ds2);
            Tcl_SetObjResult(interp, listPtr);
            break;
        }
        case FUNC_WRDBL: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            double value;
            std::vector<double> vec;
            if (objc < 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "name number ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            if (objc == 4) {
                if (Tcl_GetDoubleFromObj(interp, objv[3], &value) != TCL_OK) {
                    return TCL_ERROR;
                }
            } else {
                for (int i = 3; i < objc; i++) {
                    if (Tcl_GetDoubleFromObj(interp, objv[i], &value) != TCL_OK) {
                        return TCL_ERROR;
                    }
                    vec.push_back(value);
                }
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            Tcl_FreeEncoding(enc);
            try {
                if (objc == 4) {
                    (*fs) << name << value;
                } else {
                    (*fs) << name << "[";
                    for (size_t i = 0; i < vec.size(); i++) {
                        (*fs) << vec[i];
                    }
                    (*fs) << "]";
                }
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
        case FUNC_WRINT: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            int value;
            std::vector<int> vec;
            if (objc < 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "name integer ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            if (objc == 4) {
                if (Tcl_GetIntFromObj(interp, objv[3], &value) != TCL_OK) {
                    return TCL_ERROR;
                }
            } else {
                for (int i = 3; i < objc; i++) {
                    if (Tcl_GetIntFromObj(interp, objv[i], &value) != TCL_OK) {
                        return TCL_ERROR;
                    }
                    vec.push_back(value);
                }
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            Tcl_FreeEncoding(enc);
            try {
                if (objc == 4) {
                    (*fs) << name << value;
                } else {
                    (*fs) << name << "[";
                    for (size_t i = 0; i < vec.size(); i++) {
                        (*fs) << vec[i];
                    }
                    (*fs) << "]";
                }
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
        case FUNC_WRMAT: {
            const char *name;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            cv::Mat *mat;
            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "name mat");
                return TCL_ERROR;
            }
            mat = (cv::Mat *) Opencv_FindHandle(cd, interp, OPENCV_MAT, objv[3]);
            if (!mat) {
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            Tcl_FreeEncoding(enc);
            try {
                fs->write(name, *mat);
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
        case FUNC_WROBJ: {
            const char *name;
            int i;
            Tcl_Size nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            Opencv_Obj *obj = NULL;
            Tcl_Command cmd = NULL;
            if (objc != 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "name obj");
                return TCL_ERROR;
            }
            for (i = 0; i < OPENCV_MAXTYPE; i++) {
                obj = (Opencv_Obj *) Opencv_FindHandle(cd, NULL, (Opencv_Type) i, objv[3]);
                if (obj != NULL) {
                    break;
                }
            }
            if (obj != NULL) {
                switch ((int) obj->type) {
                    case OPENCV_MAT:
                    case OPENCV_PCA:
                    case OPENCV_ODETECT:
                    case OPENCV_OOBJHOG:
                    case OPENCV_TERMCRITERIA:
                        break;
                    default:
                        return Opencv_SetResult(interp, cv::Error::StsBadArg, "unsupported type");
                }
            }
            if (obj == NULL) {
                cmd = Tcl_FindCommand(interp, Tcl_GetString(objv[3]), NULL, TCL_LEAVE_ERR_MSG);
                if (cmd == NULL) {
                    return TCL_ERROR;
                }
                if (cmd == cvd->cmd_clahe ||
                    cmd == cvd->cmd_houghballard ||
                    cmd == cvd->cmd_houghbuil ||
                    cmd == cvd->cmd_lsdetector ||
                    cmd == cvd->cmd_fastdetector ||
                    cmd == cvd->cmd_agastdetector ||
                    cmd == cvd->cmd_mserextractor ||
                    cmd == cvd->cmd_orbdetector ||
                    cmd == cvd->cmd_akazedetector ||
                    cmd == cvd->cmd_briskdetector ||
                    cmd == cvd->cmd_kazedetector ||
#ifdef TCL_USE_SIFT
                    cmd == cvd->cmd_siftdetector ||
#endif
                    cmd == cvd->cmd_bfmatcher ||
                    cmd == cvd->cmd_flannbasedmatcher ||
                    cmd == cvd->cmd_bgsknn ||
                    cmd == cvd->cmd_bgsmog2 ||
                    cmd == cvd->cmd_sbdetector ||
                    cmd == cvd->cmd_stereobm ||
                    cmd == cvd->cmd_stereosgbm ||
                    cmd == cvd->cmd_alignmtb ||
                    cmd == cvd->cmd_calibdeb ||
                    cmd == cvd->cmd_mergedeb ||
                    cmd == cvd->cmd_mergemer ||
                    cmd == cvd->cmd_tonemapdra ||
                    cmd == cvd->cmd_tonemapman ||
                    cmd == cvd->cmd_tonemaprei ||
                    cmd == cvd->cmd_logistic ||
                    cmd == cvd->cmd_bayesclassifier ||
                    cmd == cvd->cmd_knearest ||
                    cmd == cvd->cmd_svm ||
                    cmd == cvd->cmd_svmsgd ||
                    cmd == cvd->cmd_dtrees ||
                    cmd == cvd->cmd_boost ||
                    cmd == cvd->cmd_rtrees ||
                    cmd == cvd->cmd_annmlp ||
                    cmd == cvd->cmd_traindata) {
                    /* type can be serialized */
                } else {
                    return Opencv_SetResult(interp, cv::Error::StsBadArg, "unsupported type");
                }
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            Tcl_FreeEncoding(enc);
            try {
                if (obj != NULL) {
                    switch ((int) obj->type) {
                        case OPENCV_MAT: {
                            cv::Mat *mat = (cv::Mat *) obj->obj;
                            (*fs) << name << "{";
                            (*fs) << "type" << "opencv-matrix";
                            fs->elname = "data";
                            fs->write("data", *mat);
                            (*fs) << "}";
                            break;
                        }
                        case OPENCV_PCA: {
                            cv::PCA *pca = (cv::PCA *) obj->obj;
                            (*fs) << name << "{";
                            (*fs) << "type" << "opencv-pca";
                            (*fs) << "data" << "{";
                            fs->elname = "data";
                            pca->write(*fs);
                            (*fs) << "}" << "}";
                            break;
                        }
                        case OPENCV_ODETECT: {
                            cv::CascadeClassifier *cas = (cv::CascadeClassifier *) obj->obj;
                            (*fs) << name << "{";
                            (*fs) << "type" << "opencv-cascadeclassifier";
                            (*fs) << "data" << "{";
                            fs->elname = "data";
                            cas->cc->write(*fs);
                            (*fs) << "}" << "}";
                            break;
                        }
                        case OPENCV_OOBJHOG: {
                            cv::HOGDescriptor *hog = (cv::HOGDescriptor *) obj->obj;
                            (*fs) << name << "{";
                            (*fs) << "type" << "opencv-hogdescriptor";
                            hog->write(*fs, "data");
                            (*fs) << "}";
                            break;
                        }
                        case OPENCV_TERMCRITERIA: {
                            cv::TermCriteria *tc = (cv::TermCriteria *) obj->obj;
                            (*fs) << name << "{";
                            (*fs) << "type" << "opencv-termcriteria";
                            (*fs) << "data" << "{";
                            (*fs) << "type" << tc->type;
                            (*fs) << "maxcount" << tc->maxCount;
                            (*fs) << "epsilon" << tc->epsilon;
                            (*fs) << "}" << "}";
                            break;
                        }
                    }
                } else if (cmd == cvd->cmd_clahe) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-clahe";
                    if (!cvd->clahe->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->clahe->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_houghballard) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-houghballard";
                    if (!cvd->houghballard->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->houghballard->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_houghbuil) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-houghbuil";
                    if (!cvd->houghbuil->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->houghbuil->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_lsdetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-lsdetector";
                    if (!cvd->lsdetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->lsdetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_fastdetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-fastfeaturedetector";
                    if (!cvd->fastdetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->fastdetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_agastdetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-agastfeaturedetector";
                    if (!cvd->agastdetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->agastdetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_mserextractor) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-mserextractor";
                    if (!cvd->mserextractor->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->mserextractor->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_orbdetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-orb";
                    if (!cvd->orbdetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->orbdetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_akazedetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-akaze";
                    if (!cvd->akazedetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->akazedetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_briskdetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-brisk";
                    if (!cvd->briskdetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->briskdetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_kazedetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-kaze";
                    if (!cvd->kazedetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->kazedetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
#ifdef TCL_USE_SIFT
                } else if (cmd == cvd->cmd_siftdetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-sift";
                    if (!cvd->siftdetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->siftdetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_asiftdetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-asift";
                    if (!cvd->asiftdetector->empty()) {
                        (*fs) << "backend" << cvd->asiftdetector_backend;
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->asiftdetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
#endif
                } else if (cmd == cvd->cmd_bfmatcher) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-bfmatcher";
                    if (!cvd->bfmatcher->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->bfmatcher->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_flannbasedmatcher) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-flannbasedmatcher";
                    if (!cvd->flannbasedmatcher->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->flannbasedmatcher->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_sbdetector) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-simpleblobdetector";
                    if (!cvd->sbdetector->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->sbdetector->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_bgsknn) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-backgroundsubtractorknn";
                    if (!cvd->bgsknn->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->bgsknn->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_bgsmog2) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-backgroundsubtractormog2";
                    if (!cvd->bgsmog2->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->bgsmog2->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_stereobm) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-stereobm";
                    if (!cvd->stereobm->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->stereobm->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_stereosgbm) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-stereosgbm";
                    if (!cvd->stereosgbm->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->stereosgbm->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_alignmtb) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-alignmtb";
                    if (!cvd->alignmtb->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->alignmtb->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_calibdeb) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-calibratedebevec";
                    if (!cvd->calibdeb->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->calibdeb->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_mergedeb) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-mergedebevec";
                    if (!cvd->mergedeb->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->mergedeb->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_mergemer) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-mergemertens";
                    if (!cvd->mergemer->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->mergemer->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_tonemapdra) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-tonemapdrago";
                    if (!cvd->tonemapdra->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->tonemapdra->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_tonemapman) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-tonemapmantiuk";
                    if (!cvd->tonemapman->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->tonemapman->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_tonemaprei) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-tonemapreinhard";
                    if (!cvd->tonemaprei->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->tonemaprei->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_logistic) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-logisticregression";
                    if (!cvd->logistic->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->logistic->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_bayesclassifier) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-normalbayesclassifier";
                    if (!cvd->bayesclassifier->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->bayesclassifier->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_knearest) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-knearest";
                    if (!cvd->knearest->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->knearest->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_svm) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-svm";
                    if (!cvd->svm->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->svm->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_svmsgd) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-svmsgd";
                    if (!cvd->svmsgd->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->svmsgd->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_dtrees) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-dtrees";
                    if (!cvd->dtrees->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->dtrees->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_boost) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-boost";
                    if (!cvd->boost->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->boost->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_rtrees) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-rtrees";
                    if (!cvd->rtrees->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->rtrees->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_annmlp) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-annmlp";
                    if (!cvd->annmlp->empty()) {
                        (*fs) << "data" << "{";
                        fs->elname = "data";
                        cvd->annmlp->write(*fs);
                        (*fs) << "}";
                    }
                    (*fs) << "}";
                } else if (cmd == cvd->cmd_traindata) {
                    (*fs) << name << "{";
                    (*fs) << "type" << "opencv-traindata";
                    (*fs) << "data" << "{";
                    (*fs) << "samples" << cvd->traindata->getSamples();
                    (*fs) << "layout" << cvd->traindata->getLayout();
                    (*fs) << "responses" << cvd->traindata->getResponses();
                    (*fs) << "}" << "}";
                } else {
                    Tcl_Panic("unhandled type/object");
                }
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
        case FUNC_WRSTR: {
            const char *name, *value;
            Tcl_Size nlen, vlen;
            Tcl_DString ds1, ds2;
            Tcl_Encoding enc;
            std::vector<std::string> vec;
            if (objc < 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "name string ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds1);
            if (objc == 4) {
                value = Tcl_GetStringFromObj(objv[3], &vlen);
                value = Tcl_UtfToExternalDString(enc, value, vlen, &ds2);
            } else {
                for (int i = 3; i < objc; i++) {
                    value = Tcl_GetStringFromObj(objv[i], &vlen);
                    value = Tcl_UtfToExternalDString(enc, value, vlen, &ds2);
                    vec.push_back(value);
                    Tcl_DStringFree(&ds2);
                }
            }
            Tcl_FreeEncoding(enc);
            try {
                if (objc == 4) {
                    (*fs) << name << value;
                } else {
                    (*fs) << name << "[";
                    for (size_t i = 0; i < vec.size(); i++) {
                        (*fs) << vec[i];
                    }
                    (*fs) << "]";
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
            break;
        }
        case FUNC_CLOSE: {
            if (objc != 2) {
                Tcl_WrongNumArgs(interp, 2, objv, 0);
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                return Opencv_SetResult(interp, cv::Error::StsError, "not opened");
            }
            int ret = TCL_OK;
            if (cvo->flags & (cv::FileStorage::WRITE | cv::FileStorage::APPEND)) {
                char marker[2];
                int mlen = Tcl_DStringLength(&cvo->ds2) - 1;
                while (mlen >= 0) {
                    marker[0] = Tcl_DStringValue(&cvo->ds2)[mlen];
                    marker[1] = '\0';
                    try {
                        (*fs) << (const char *) marker;
                    } catch (const cv::Exception &ex) {
                        ret = Opencv_Exc2Tcl(interp, &ex);
                    } catch (...) {
                        ret = Opencv_Exc2Tcl(interp, NULL);
                    }
                    mlen--;
                }
            }
            if (ret == TCL_OK &&
                (cvo->flags & (cv::FileStorage::WRITE | cv::FileStorage::MEMORY)) ==
                (cv::FileStorage::WRITE | cv::FileStorage::MEMORY)) {
                try {
                    std::string str = fs->releaseAndGetString();
                    Tcl_SetObjResult(interp, Tcl_NewByteArrayObj((unsigned char *) str.c_str(), str.length()));
                } catch (const cv::Exception &ex) {
                    ret = Opencv_Exc2Tcl(interp, &ex);
                } catch (...) {
                    ret = Opencv_Exc2Tcl(interp, NULL);
                }
            }
            Opencv_CloseHandle(interp, cvo);
            if (ret != TCL_OK) {
                return TCL_ERROR;
            }
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

            Tcl_SetResult(interp, (char *) "cv::FileStorage", TCL_STATIC);
            break;
        }
    }

    return TCL_OK;
}


int FileStorage(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    cv::FileStorage *fs;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, NULL);
        return TCL_ERROR;
    }

    fs = new cv::FileStorage();

    return Opencv_NewHandleResult(cd, interp, OPENCV_FSTORAGE, fs);
}
#ifdef __cplusplus
}
#endif
