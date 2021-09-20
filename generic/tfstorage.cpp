#include "tclopencv.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif


int FileStorage_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
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
        "readString",
        "writeDouble",
        "writeInt",
        "writeMat",
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
        FUNC_RDSTR,
        FUNC_WRDBL,
        FUNC_WRINT,
        FUNC_WRMAT,
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
            int len, mode;
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
                    Tcl_SetResult(interp, (char *) "invalid filename or data", TCL_STATIC);
                    return TCL_ERROR;
                }
                Tcl_DStringFree(&cvo->ds1);
                filename = Tcl_UtfToExternalDString(NULL, filename, len, &cvo->ds1);
            }
            if (!fs->open(filename, mode)) {
                Tcl_DStringFree(&cvo->ds1);
                Tcl_SetResult(interp, (char *) "open failed", TCL_STATIC);
                return TCL_ERROR;
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
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_Encoding enc = Tcl_GetEncoding(NULL, "utf-8");
            Tcl_DString ds;
            Tcl_DStringInit(&ds);
            Tcl_Obj *listPtr = Tcl_NewListObj(0, NULL);
            /* WARNING: yields list of empty names in OpenCV 3.2 on Debian 10 */
            try {
                const char *name = NULL;
                cv::FileNode node;
                int len;
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
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                Tcl_SetResult(interp, (char *) "retrieve keys failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            Tcl_SetObjResult(interp, listPtr);
            break;
        }
        case FUNC_STARTM:
        case FUNC_STARTS: {
            const char *name;
            int nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            if (objc != 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
            }
            if (!(cvo->flags & (cv::FileStorage::WRITE | cv::FileStorage::APPEND))) {
                Tcl_SetResult(interp, (char *) "not opened for writing", TCL_STATIC);
                return TCL_ERROR;
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
            } catch (...) {
                Tcl_DStringFree(&ds);
                Tcl_SetResult(interp, (char *) "write failed", TCL_STATIC);
                return TCL_ERROR;
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
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
            }
            if (!(cvo->flags & (cv::FileStorage::WRITE | cv::FileStorage::APPEND))) {
                Tcl_SetResult(interp, (char *) "not opened for writing", TCL_STATIC);
                return TCL_ERROR;
            }
            mlen = Tcl_DStringLength(&cvo->ds2) - 1;
            if (mlen < 0) {
                Tcl_SetResult(interp, (char *) "no seq/map to end", TCL_STATIC);
                return TCL_ERROR;
            }
            marker[0] = Tcl_DStringValue(&cvo->ds2)[mlen];
            marker[1] = '\0';
            if ((marker[0] == ']' && choice != FUNC_ENDS) ||
                (marker[0] == '}' && choice != FUNC_ENDM))  {
                Tcl_SetResult(interp, (char *) "seq/map mismatch", TCL_STATIC);
                return TCL_ERROR;
            }
            try {
                (*fs) << (const char *) marker;
            } catch (...) {
                Tcl_SetResult(interp, (char *) "write failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_DStringSetLength(&cvo->ds2, mlen);
            break;
        }
        case FUNC_RDDBL: {
            const char *name;
            int nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            Tcl_Obj *listPtr;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
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
                    throw cv::Exception();
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
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                Tcl_SetResult(interp, (char *) "read failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            Tcl_SetObjResult(interp, listPtr);
            break;
        }
        case FUNC_RDINT: {
            const char *name;
            int nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            Tcl_Obj *listPtr;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
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
                    throw cv::Exception();
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
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DecrRefCount(listPtr);
                Tcl_SetResult(interp, (char *) "read failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            Tcl_SetObjResult(interp, listPtr);
            break;
        }
        case FUNC_RDMAT: {
            const char *name;
            int nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            Tcl_Obj *pResultStr;
            cv::Mat rmat, *mat;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
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
                    throw cv::Exception();
                }
                rmat = node.mat();
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_SetResult(interp, (char *) "read failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            mat = new cv::Mat(rmat);
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, mat);
            Tcl_SetObjResult(interp, pResultStr);
            break;
        }
        case FUNC_RDSTR: {
            const char *name;
            int nlen;
            Tcl_DString ds, ds2;
            Tcl_Encoding enc;
            Tcl_Obj *listPtr;
            if (objc < 3) {
                Tcl_WrongNumArgs(interp, 2, objv, "name ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
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
                    throw cv::Exception();
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
            } catch (...) {
                Tcl_FreeEncoding(enc);
                Tcl_DStringFree(&ds);
                Tcl_DStringFree(&ds2);
                Tcl_DecrRefCount(listPtr);
                Tcl_SetResult(interp, (char *) "read failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_FreeEncoding(enc);
            Tcl_DStringFree(&ds);
            Tcl_DStringFree(&ds2);
            Tcl_SetObjResult(interp, listPtr);
            break;
        }
        case FUNC_WRDBL: {
            const char *name;
            int nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            double value;
            std::vector<double> vec;
            if (objc < 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "name number ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
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
            } catch (...) {
                Tcl_DStringFree(&ds);
                Tcl_SetResult(interp, (char *) "write failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_DStringFree(&ds);
            break;
        }
        case FUNC_WRINT: {
            const char *name;
            int nlen;
            Tcl_DString ds;
            Tcl_Encoding enc;
            int value;
            std::vector<int> vec;
            if (objc < 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "name integer ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
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
            } catch (...) {
                Tcl_DStringFree(&ds);
                Tcl_SetResult(interp, (char *) "write failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_DStringFree(&ds);
            break;
        }
        case FUNC_WRMAT: {
            const char *name;
            int nlen;
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
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
            }
            enc = Tcl_GetEncoding(NULL, "utf-8");
            name = Tcl_GetStringFromObj(objv[2], &nlen);
            name = Tcl_UtfToExternalDString(enc, name, nlen, &ds);
            Tcl_FreeEncoding(enc);
            try {
                fs->write(name, *mat);
            } catch (...) {
                Tcl_DStringFree(&ds);
                Tcl_SetResult(interp, (char *) "write failed", TCL_STATIC);
                return TCL_ERROR;
            }
            Tcl_DStringFree(&ds);
            break;
        }
        case FUNC_WRSTR: {
            const char *name, *value;
            int nlen, vlen;
            Tcl_DString ds1, ds2;
            Tcl_Encoding enc;
            std::vector<std::string> vec;
            if (objc < 4) {
                Tcl_WrongNumArgs(interp, 2, objv, "name string ...");
                return TCL_ERROR;
            }
            if (!fs->isOpened()) {
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
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
            } catch (...) {
                Tcl_DStringFree(&ds1);
                Tcl_DStringFree(&ds2);
                Tcl_SetResult(interp, (char *) "write failed", TCL_STATIC);
                return TCL_ERROR;
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
                Tcl_SetResult(interp, (char *) "not opened", TCL_STATIC);
                return TCL_ERROR;
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
                    } catch (...) {
                        Tcl_SetResult(interp, (char *) "write failed", TCL_STATIC);
                        ret = TCL_ERROR;
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
                } catch (...) {
                    Tcl_SetResult(interp, (char *) "error on close", TCL_STATIC);
                    ret = TCL_ERROR;
                }
            }
            Tcl_DeleteCommandFromToken(interp, cvo->cmd);
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
    Tcl_Obj *pResultStr = NULL;
    cv::FileStorage *fs;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 1, objv, NULL);
        return TCL_ERROR;
    }

    fs = new cv::FileStorage();

    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_FSTORAGE, fs);

    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
