#include "tclopencv.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct CvthrMsg {
    struct CvthrMsg *next;
    struct CvthrMsg *tail;
    cv::Mat *mat;
    Tcl_DString ds;
} CvthrMsg;


static int cvthrInitialized = 0;
static Tcl_HashTable cvthrHash;
static Tcl_Condition cvthrCond;
TCL_DECLARE_MUTEX(cvthrMutex);


static void ExitHandler(ClientData cd)
{
    Tcl_HashSearch search;
    Tcl_HashEntry *hashEntryPtr;

    Tcl_MutexLock(&cvthrMutex);
    hashEntryPtr = Tcl_FirstHashEntry(&cvthrHash, &search);
    while (hashEntryPtr != NULL) {
        CvthrMsg *msg, *next;

        msg = (CvthrMsg *) Tcl_GetHashValue(hashEntryPtr);
        while (msg != NULL) {
            next = msg->next;
            Tcl_DStringFree(&msg->ds);
            if (msg->mat) {
                delete msg->mat;
            }
            ckfree(msg);
            msg = next;
        }
        Tcl_DeleteHashEntry(hashEntryPtr);
        hashEntryPtr = Tcl_NextHashEntry(&search);
    }
    Tcl_DeleteHashTable(&cvthrHash);
    cvthrInitialized = 0;
    Tcl_MutexUnlock(&cvthrMutex);
}


static void InitProc(void)
{
    Tcl_MutexLock(&cvthrMutex);
    if (!cvthrInitialized) {
        const Tcl_Time time0 = { 0, 0 };

        if (cvthrMutex == NULL) {
            cvthrInitialized = -1;
            Tcl_MutexUnlock(&cvthrMutex);
            return;
        }
        cvthrInitialized = 1;
        Tcl_InitHashTable(&cvthrHash, TCL_STRING_KEYS);
        Tcl_ConditionWait(&cvthrCond, &cvthrMutex, &time0);
        Tcl_CreateExitHandler(ExitHandler, (ClientData) NULL);
    }
    Tcl_MutexUnlock(&cvthrMutex);
}


int Opencv_Tsend(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    char *name;
    Opencv_Obj *cvo = NULL;
    Tcl_HashEntry *hashEntryPtr;
    CvthrMsg *msg, *oldMsg;
    int isNew;

    if (objc != 3 && objc != 4) {
        Tcl_WrongNumArgs(interp, 1, objv, "tag mat|None ?string?");
        return TCL_ERROR;
    }
    name = Tcl_GetString(objv[2]);
    if (strcmp(name, "None") != 0) {
        hashEntryPtr = Tcl_FindHashEntry(&cvd->tbl[OPENCV_MAT], Tcl_GetString(objv[2]));
        if (hashEntryPtr == NULL) {
            return Opencv_SetResult(interp, cv::Error::StsBadArg, "handle not found");
        }
        cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);
    }
    if (!cvthrInitialized) {
        InitProc();
    }
    if (cvthrInitialized < 0) {
        return Opencv_SetResult(interp, cv::Error::StsNotImplemented, "no thread support");
    }
    if (cvo != NULL && cvo->traced) {
        return Opencv_SetResult(interp, cv::Error::StsError, "handle is traced");
    }
    msg = (CvthrMsg *) ckalloc(sizeof(CvthrMsg));
    msg->next = NULL;
    msg->tail = msg;
    msg->mat = 0;
    if (cvo != NULL) {
        msg->mat = (cv::Mat *) cvo->obj;
        cvo->obj = NULL;
        Opencv_CloseHandle(interp, cvo);
    }
    Tcl_DStringInit(&msg->ds);
    if (objc > 3) {
        Tcl_Size len;
        char *string = Tcl_GetStringFromObj(objv[3], &len);

        Tcl_DStringAppend(&msg->ds, string, len);
    }
    Tcl_MutexLock(&cvthrMutex);
    hashEntryPtr = Tcl_CreateHashEntry(&cvthrHash, Tcl_GetString(objv[1]), &isNew);
    if (isNew) {
        Tcl_SetHashValue(hashEntryPtr, msg);
    } else {
        oldMsg = (CvthrMsg *) Tcl_GetHashValue(hashEntryPtr);
        oldMsg->tail->next = msg;
        oldMsg->tail = msg;
    }
    Tcl_ConditionNotify(&cvthrCond);
    Tcl_MutexUnlock(&cvthrMutex);
    return TCL_OK;
}


int Opencv_Trecv(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Tcl_HashEntry *hashEntryPtr;
    char *tag;
    CvthrMsg *msg = NULL;
    int ms = 10000;
    Tcl_Time t0, t1, timeout = { 10, 0 };

    if (objc != 2 && objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "tag ?timeout?");
        return TCL_ERROR;
    }
    if (!cvthrInitialized) {
        InitProc();
    }
    if (cvthrInitialized < 0) {
        return Opencv_SetResult(interp, cv::Error::StsNotImplemented, "no thread support");
    }
    if (objc > 2) {
        if (Tcl_GetIntFromObj(interp, objv[2], &ms) != TCL_OK) {
            return TCL_ERROR;
        }
        if (ms < 0) {
            ms = 0;
        }
        timeout.sec = ms / 1000;
        timeout.usec = (ms % 1000) * 1000;
    }
    tag = Tcl_GetString(objv[1]);
    Tcl_GetTime(&t0);
    t0.sec += timeout.sec;
    t0.usec += timeout.usec;
    if (t0.usec > 1000000) {
        t0.sec += 1;
        t0.usec -= 1000000;
    }
    Tcl_MutexLock(&cvthrMutex);
    while (1) {
        hashEntryPtr = Tcl_FindHashEntry(&cvthrHash, tag);
        if (hashEntryPtr == NULL) {
            if (ms == 0) {
                break;
            }
            Tcl_GetTime(&t1);
            timeout.sec = t0.sec - t1.sec;
            timeout.usec = t0.usec - t1.usec;
            if (timeout.sec < 0) {
                break;
            }
            if (timeout.sec == 0) {
                if (timeout.usec <= 0) {
                    break;
                }
            } else if (timeout.usec < 0) {
                timeout.sec -= 1;
                timeout.usec += 1000000;
            }
            if (timeout.sec * 1000 + timeout.usec / 1000 > ms) {
                timeout.sec = ms / 1000;
                timeout.usec = (ms % 1000) * 1000;
                t0 = t1;
                t0.sec += timeout.sec;
                t0.usec += timeout.usec;
                if (t0.usec > 1000000) {
                    t0.sec += 1;
                    t0.usec -= 1000000;
                }
            }
            Tcl_ConditionWait(&cvthrCond, &cvthrMutex, &timeout);
            continue;
        }
        msg = (CvthrMsg *) Tcl_GetHashValue(hashEntryPtr);
        if (msg->next != NULL) {
            msg->next->tail = msg->tail;
            Tcl_SetHashValue(hashEntryPtr, msg->next);
        } else {
            Tcl_DeleteHashEntry(hashEntryPtr);
        }
        break;
    }
    Tcl_MutexUnlock(&cvthrMutex);
    if (msg != NULL) {
        Tcl_Obj *pResultStr;

        if (msg->mat) {
            pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, msg->mat);
        } else {
            pResultStr = Tcl_NewObj();
        }
        if (Tcl_DStringLength(&msg->ds) > 0) {
            Tcl_Obj *list[2];
            list[0] = pResultStr;
            list[1] = Tcl_NewStringObj(Tcl_DStringValue(&msg->ds), Tcl_DStringLength(&msg->ds));
            Tcl_SetObjResult(interp, Tcl_NewListObj(2, list));
        } else {
            Tcl_SetObjResult(interp, pResultStr);
        }
        Tcl_DStringFree(&msg->ds);
        ckfree(msg);
    }
    return TCL_OK;
}


int Opencv_Tinfo(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Tcl_HashSearch search;
    Tcl_HashEntry *hashEntryPtr;

    if (objc != 1 && objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "?tag?");
        return TCL_ERROR;
    }
    if (!cvthrInitialized) {
        InitProc();
    }
    if (cvthrInitialized < 0) {
        return Opencv_SetResult(interp, cv::Error::StsNotImplemented, "no thread support");
    }
    if (objc > 1) {
        int count = 0;
        CvthrMsg *msg;

        Tcl_MutexLock(&cvthrMutex);
        hashEntryPtr = Tcl_FindHashEntry(&cvthrHash, Tcl_GetString(objv[1]));
        if (hashEntryPtr != NULL) {
            msg = (CvthrMsg *) Tcl_GetHashValue(hashEntryPtr);
            while (msg != NULL) {
                count++;
                msg = msg->next;
            }
        }
        Tcl_MutexUnlock(&cvthrMutex);
        Tcl_SetObjResult(interp, Tcl_NewIntObj(count));
    } else {
        Tcl_Obj *pResultList = Tcl_NewListObj(0, NULL);

        Tcl_MutexLock(&cvthrMutex);
        hashEntryPtr = Tcl_FirstHashEntry(&cvthrHash, &search);
        while (hashEntryPtr != NULL) {
            Tcl_ListObjAppendElement(NULL, pResultList,
                Tcl_NewStringObj((char *) Tcl_GetHashKey(&cvthrHash, hashEntryPtr), -1));
            hashEntryPtr = Tcl_NextHashEntry(&search);
        }
        Tcl_MutexUnlock(&cvthrMutex);
        Tcl_SetObjResult(interp, pResultList);
    }
    return TCL_OK;
}
#ifdef __cplusplus
}
#endif
