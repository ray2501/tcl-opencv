#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
}
#endif

#include "tclopencv.h"


static void
InterpDelProc(ClientData clientdata, Tcl_Interp *interp)
{
    Opencv_Data *cvd = (Opencv_Data *)clientdata;
    Opencv_Obj *cvo;
    int i;
    Tcl_HashEntry *hashEntryPtr;
    Tcl_HashSearch search;

    for (i = 0; i < OPENCV_MAXTYPE; i++) {
        for (hashEntryPtr = Tcl_FirstHashEntry(&cvd->tbl[i], &search);
             hashEntryPtr != NULL; hashEntryPtr = Tcl_NextHashEntry(&search)) {
            cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);
            cvo->top = NULL;
            cvo->key = NULL;
            cvo->cmd = NULL;
            switch ((Opencv_Type) i) {
            case OPENCV_CALLBACK: {
                CvCallbackInfo *cbinfo = (CvCallbackInfo *) cvo->obj;
                ckfree(cbinfo);
                break;
            }
            case OPENCV_MAT: {
                cv::Mat *mat = (cv::Mat *) cvo->obj;
                delete mat;
                break;
            }
            case OPENCV_VIDEOCAPTURE: {
                cv::VideoCapture *capture = (cv::VideoCapture *) cvo->obj;
                capture->release();
                delete capture;
                break;
            }
            case OPENCV_VIDEOWRITER: {
                cv::VideoWriter *writer = (cv::VideoWriter *) cvo->obj;
                writer->release();
                delete writer;
                break;
            }
            case OPENCV_TERMCRITERIA: {
                cv::TermCriteria *termCriteria = (cv::TermCriteria *) cvo->obj;
                delete termCriteria;
                break;
            }
            case OPENCV_ODETECT: {
                cv::CascadeClassifier *cas = (cv::CascadeClassifier *) cvo->obj;
                delete cas;
                break;
            }
#ifdef TCL_USE_OPENCV4
            case OPENCV_NDETECT: {
                cv::dnn::Net *net = (cv::dnn::Net *) cvo->obj;
                delete net;
                break;
            }
            case OPENCV_QDETECT: {
                cv::QRCodeDetector *qrdet = (cv::QRCodeDetector *) cvo->obj;
                delete qrdet;
                break;
            }
#endif
            default:
                Tcl_Panic("wrong Opencv type");
                break;
            }
            ckfree(cvo);
            Tcl_DeleteHashEntry(hashEntryPtr);
        }
        Tcl_DeleteHashTable(&cvd->tbl[i]);
    }
    if (cvd->fastdetector) {
        cvd->fastdetector.release();
    }
    if (cvd->agastdetector) {
        cvd->agastdetector.release();
    }
    if (cvd->orbdetector) {
        cvd->orbdetector.release();
    }
    if (cvd->akazedetector) {
        cvd->akazedetector.release();
    }
    if (cvd->briskdetector) {
        cvd->briskdetector.release();
    }
#ifdef TCL_USE_SIFT
    if (cvd->siftdetector) {
        cvd->siftdetector.release();
    }
#endif
    if (cvd->bfmatcher) {
        cvd->bfmatcher.release();
    }
    if (cvd->flannbasedmatcher) {
        cvd->flannbasedmatcher.release();
    }
    if (cvd->bgsmog2) {
        cvd->bgsmog2.release();
    }
    if (cvd->stitcher) {
        cvd->stitcher.release();
    }
    if (cvd->sbdetector) {
        cvd->sbdetector.release();
    }
    if (cvd->stereobm) {
        cvd->stereobm.release();
    }
    if (cvd->stereosgbm) {
        cvd->stereosgbm.release();
    }
    if (cvd->alignmtb) {
        cvd->alignmtb.release();
    }
    if (cvd->calibdeb) {
        cvd->calibdeb.release();
    }
    if (cvd->mergedeb) {
        cvd->mergedeb.release();
    }
    if (cvd->mergemer) {
        cvd->mergemer.release();
    }
    if (cvd->tonemapdra) {
        cvd->tonemapdra.release();
    }
    if (cvd->tonemapman) {
        cvd->tonemapman.release();
    }
    if (cvd->tonemaprei) {
        cvd->tonemaprei.release();
    }
    ckfree(cvd);
}


static char *
Opencv_HandleName(const char *prefix, void *obj, char *buffer)
{
    int n, l = strlen(prefix);
    char *p;

    sprintf(buffer, "%p", obj);
    p = buffer;
    n = strlen(p) + 1;
    if (buffer[0] == '0' && buffer[1] == 'x') {
        n -= 2;
        p += 2;
    }
    memmove(buffer + l + 1, p, n);
    memcpy(buffer, prefix, l);
    buffer[l] = '-';
    return buffer;
}


static void
Opencv_DESTRUCTOR(ClientData cd)
{
    Opencv_Obj *cvo = (Opencv_Obj *)cd;
    Opencv_Data *cvd = cvo->top;
    Tcl_HashEntry *hashEntryPtr;

    if ((unsigned) cvo->type >= OPENCV_MAXTYPE) {
        Tcl_Panic("wrong Opencv type");
    }
    hashEntryPtr = Tcl_FindHashEntry(&cvd->tbl[cvo->type], cvo->key);
    if (hashEntryPtr != NULL) {
        Tcl_DeleteHashEntry(hashEntryPtr);
    }
    switch (cvo->type) {
    case OPENCV_CALLBACK: {
        CvCallbackInfo *cbinfo = (CvCallbackInfo *) cvo->obj;
        ckfree(cbinfo);
        break;
    }
    case OPENCV_MAT: {
        cv::Mat *mat = (cv::Mat *) cvo->obj;
        delete mat;
        break;
    }
    case OPENCV_VIDEOCAPTURE: {
        cv::VideoCapture *capture = (cv::VideoCapture *) cvo->obj;
        capture->release();
        delete capture;
        break;
    }
    case OPENCV_VIDEOWRITER: {
        cv::VideoWriter *writer = (cv::VideoWriter *) cvo->obj;
        writer->release();
        delete writer;
        break;
    }
    case OPENCV_TERMCRITERIA: {
        cv::TermCriteria *termCriteria = (cv::TermCriteria *) cvo->obj;
        delete termCriteria;
        break;
    }
    case OPENCV_ODETECT: {
        cv::CascadeClassifier *cas = (cv::CascadeClassifier *) cvo->obj;
        delete cas;
        break;
    }
#ifdef TCL_USE_OPENCV4
    case OPENCV_NDETECT: {
        cv::dnn::Net *net = (cv::dnn::Net *) cvo->obj;
        delete net;
        break;
    }
    case OPENCV_QDETECT: {
        cv::QRCodeDetector *qrdet = (cv::QRCodeDetector *) cvo->obj;
        delete qrdet;
        break;
    }
#endif
    default:
        Tcl_Panic("wrong Opencv type");
        break;
    }
    ckfree(cvo);
}


Tcl_Obj *
Opencv_NewHandle(void *cd, Tcl_Interp *interp, Opencv_Type type, void *obj)
{
    Opencv_Data *cvd = (Opencv_Data *) cd;
    Opencv_Obj *cvo;
    Tcl_HashEntry *hashEntryPtr;
    int isNew;
    const char *prefix = NULL;
    Tcl_ObjCmdProc *proc = NULL;
    char buffer[64 + TCL_INTEGER_SPACE];
    Tcl_Obj *name;

    switch ((unsigned) type) {
    case OPENCV_CALLBACK:
        prefix = "cv-callback";
        break;
    case OPENCV_MAT:
        prefix = "cv-mat";
        proc = MATRIX_FUNCTION;
        break;
    case OPENCV_VIDEOCAPTURE:
        prefix = "cv-videoc";
        proc = VideoCapture_FUNCTION;
        break;
    case OPENCV_VIDEOWRITER:
        prefix = "cv-videow";
        proc = VideoWriter_FUNCTION;
        break;
    case OPENCV_TERMCRITERIA:
        prefix = "cv-term";
        proc = TERM_FUNCTION;
        break;
    case OPENCV_ODETECT:
        prefix = "cv-odetect";
        proc = CascadeClassifier_FUNCTION;
        break;
#ifdef TCL_USE_OPENCV4
    case OPENCV_NDETECT:
        prefix = "cv-ndetect";
        proc = READNET_FUNCTION;
        break;
    case OPENCV_QDETECT:
        prefix = "cv-qdetect";
        proc = QRCodeDetector_FUNCTION;
        break;
#endif
    default:
        Tcl_Panic("wrong Opencv type");
        break;
    }
    cvo = (Opencv_Obj *) ckalloc(sizeof(Opencv_Obj));
    cvo->type = type;
    cvo->top = cvd;
    cvo->obj = obj;
    Opencv_HandleName(prefix, cvo, buffer);
    hashEntryPtr = Tcl_CreateHashEntry(&cvd->tbl[type], buffer, &isNew);
    if (!isNew) {
        Tcl_Panic("duplicate hash entry");
    }
    Tcl_SetHashValue(hashEntryPtr, cvo);
    cvo->key = (char *) Tcl_GetHashKey(&cvd->tbl[type], hashEntryPtr);
    cvo->cmd = NULL;
    if (proc) {
        name = Tcl_NewStringObj(buffer, -1);
        Tcl_IncrRefCount(name);
        cvo->cmd = Tcl_CreateObjCommand(interp, buffer, proc, cvo, Opencv_DESTRUCTOR);
        return name;
    }
    return NULL;
}


void *
Opencv_FindHandle(void *cd, Tcl_Interp *interp, Opencv_Type type, Tcl_Obj *name)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Opencv_Obj *cvo;
    Tcl_HashEntry *hashEntryPtr;
    char *string;

    if ((unsigned) type >= OPENCV_MAXTYPE) {
        Tcl_Panic("wrong Opencv type");
    }
    string = Tcl_GetString(name);
    hashEntryPtr = Tcl_FindHashEntry(&cvd->tbl[type], string);
    if (hashEntryPtr == NULL) {
        Tcl_SetObjResult(interp,
            Tcl_ObjPrintf("handle \"%s\" not found", string));
        return NULL;
    }
    cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);
    if (cvo == NULL) {
        Tcl_Panic("null CV handle");
    }
    if (cvo->obj == NULL) {
        Tcl_Panic("null CV object");
    }
    return cvo->obj;
}


static int
Opencv_Info(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Opencv_Obj *cvo;
    int i;
    Tcl_HashEntry *hashEntryPtr;
    Tcl_HashSearch search;
    Tcl_Obj *list, *obj;

    if (objc != 1) {
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
    }

    list = Tcl_NewListObj(0, NULL);
    for (i = 0; i < OPENCV_MAXTYPE; i++) {
        for (hashEntryPtr = Tcl_FirstHashEntry(&cvd->tbl[i], &search);
             hashEntryPtr != NULL; hashEntryPtr = Tcl_NextHashEntry(&search)) {
            cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);
            obj = Tcl_NewObj();
            Tcl_GetCommandFullName(interp, cvo->cmd, obj);
            Tcl_ListObjAppendElement(NULL, list, obj);
        }
    }
    Tcl_SetObjResult(interp, list);
    return TCL_OK;
}


/*
 *----------------------------------------------------------------------
 *
 * Opencv_Init --
 *
 *      Initialize the new package.
 *
 * Results:
 *      A standard Tcl result
 *
 * Side effects:
 *      The Opencv package is created.
 *
 *----------------------------------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
DLLEXPORT int
Opencv_Init(Tcl_Interp *interp)
{
    Tcl_Namespace *nsPtr;
    Tcl_Obj *strValue;
    Tcl_Obj *setupValue;
    Opencv_Data *cvd;
    int i;

    if (Tcl_InitStubs(interp, "8.6", 0) == NULL) {
        return TCL_ERROR;
    }
    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
        return TCL_ERROR;
    }

    nsPtr = Tcl_CreateNamespace(interp, NS, NULL, NULL);
    if (nsPtr == NULL) {
        return TCL_ERROR;
    }

    cvd = (Opencv_Data *) ckalloc(sizeof(Opencv_Data));
    memset((void *) cvd, 0, sizeof(Opencv_Data));
    for (i = 0; i < OPENCV_MAXTYPE; i++) {
        Tcl_InitHashTable(&cvd->tbl[i], TCL_STRING_KEYS);
    }

    Tcl_CallWhenDeleted(interp, InterpDelProc, (ClientData)cvd);

    /*
     * For core
     */

    Tcl_CreateObjCommand(interp, "::" NS "::CV_8UC",
        (Tcl_ObjCmdProc *) tcl_CV_8UC,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::CV_8SC",
        (Tcl_ObjCmdProc *) tcl_CV_8SC,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::CV_16UC",
        (Tcl_ObjCmdProc *) tcl_CV_16UC,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::CV_16SC",
        (Tcl_ObjCmdProc *) tcl_CV_16SC,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::CV_32SC",
        (Tcl_ObjCmdProc *) tcl_CV_32SC,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::CV_32FC",
        (Tcl_ObjCmdProc *) tcl_CV_32FC,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::CV_64FC",
        (Tcl_ObjCmdProc *) tcl_CV_64FC,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Mat::Mat",
        (Tcl_ObjCmdProc *) mat_mat,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Mat::MatWithDims",
        (Tcl_ObjCmdProc *) mat_matwithdims,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Mat::diag",
        (Tcl_ObjCmdProc *) mat_diag,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Mat::eye",
        (Tcl_ObjCmdProc *) mat_eye,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Mat::ones",
        (Tcl_ObjCmdProc *) mat_ones,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Mat::zeros",
        (Tcl_ObjCmdProc *) mat_zeros,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::abs",
        (Tcl_ObjCmdProc *) mat_abs,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::absdiff",
        (Tcl_ObjCmdProc *) mat_absdiff,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::add",
        (Tcl_ObjCmdProc *) mat_add,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::addWeighted",
        (Tcl_ObjCmdProc *) mat_addWeighted,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::bitwise_and",
        (Tcl_ObjCmdProc *) mat_bitwise_and,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::bitwise_or",
        (Tcl_ObjCmdProc *) mat_bitwise_or,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::bitwise_xor",
        (Tcl_ObjCmdProc *) mat_bitwise_xor,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::bitwise_not",
        (Tcl_ObjCmdProc *) mat_bitwise_not,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::cartToPolar",
        (Tcl_ObjCmdProc *) mat_cartToPolar,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::convertScaleAbs",
        (Tcl_ObjCmdProc *) mat_convertScaleAbs,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::copyMakeBorder",
        (Tcl_ObjCmdProc *) mat_copyMakeBorder,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::exp",
        (Tcl_ObjCmdProc *) mat_exp,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::flip",
        (Tcl_ObjCmdProc *) mat_flip,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::divide",
        (Tcl_ObjCmdProc *) mat_divide,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getOptimalDFTSize",
        (Tcl_ObjCmdProc *) mat_getOptimalDFTSize,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::dft",
        (Tcl_ObjCmdProc *) mat_dft,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::inRange",
        (Tcl_ObjCmdProc *) mat_inRange,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::log",
        (Tcl_ObjCmdProc *) mat_log,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::LUT",
        (Tcl_ObjCmdProc *) mat_lut,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::magnitude",
        (Tcl_ObjCmdProc *) mat_magnitude,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::meanStdDev",
        (Tcl_ObjCmdProc *) mat_meanStdDev,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::minMaxLoc",
        (Tcl_ObjCmdProc *) mat_minMaxLoc,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::multiply",
        (Tcl_ObjCmdProc *) mat_multiply,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::mulSpectrums",
        (Tcl_ObjCmdProc *) mat_mulSpectrums,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::split",
        (Tcl_ObjCmdProc *) mat_split,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::merge",
        (Tcl_ObjCmdProc *) mat_merge,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::norm",
        (Tcl_ObjCmdProc *) mat_norm,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::normalize",
        (Tcl_ObjCmdProc *) mat_normalize,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::pow",
        (Tcl_ObjCmdProc *) mat_pow,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::randu",
        (Tcl_ObjCmdProc *) mat_randu,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::sqrt",
        (Tcl_ObjCmdProc *) mat_sqrt,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::subtract",
        (Tcl_ObjCmdProc *) mat_subtract,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::sum",
        (Tcl_ObjCmdProc *) mat_sum,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::hconcat",
        (Tcl_ObjCmdProc *) mat_hconcat,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::vconcat",
        (Tcl_ObjCmdProc *) mat_vconcat,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::perspectiveTransform",
        (Tcl_ObjCmdProc *) perspectiveTransform,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getTickCount",
        (Tcl_ObjCmdProc *) getTickCount,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getTickFrequency",
        (Tcl_ObjCmdProc *) getTickFrequency,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::TermCriteria",
        (Tcl_ObjCmdProc *) TermCriteria,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For imgcodecs
     */

    Tcl_CreateObjCommand(interp, "::" NS "::imread",
        (Tcl_ObjCmdProc *) imread,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::imwrite",
        (Tcl_ObjCmdProc *) imwrite,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For imgproc
     */

    Tcl_CreateObjCommand(interp, "::" NS "::applyColorMap",
        (Tcl_ObjCmdProc *) applyColorMap,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::cvtColor",
        (Tcl_ObjCmdProc *) cvtColor,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::calcBackProject",
        (Tcl_ObjCmdProc *) calcBackProject,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::calcHist",
        (Tcl_ObjCmdProc *) calcHist,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::equalizeHist",
        (Tcl_ObjCmdProc *) equalizeHist,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::floodFill",
        (Tcl_ObjCmdProc *) floodFill,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::grabCut",
        (Tcl_ObjCmdProc *) grabCut,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::matchTemplate",
        (Tcl_ObjCmdProc *) matchTemplate,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getRotationMatrix2D",
        (Tcl_ObjCmdProc *) getRotationMatrix2D,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getRectSubPix",
        (Tcl_ObjCmdProc *) getRectSubPix,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::remap",
        (Tcl_ObjCmdProc *) remap,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::resize",
        (Tcl_ObjCmdProc *) resize,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::threshold",
        (Tcl_ObjCmdProc *) threshold,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::adaptiveThreshold",
        (Tcl_ObjCmdProc *) adaptiveThreshold,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getAffineTransform",
        (Tcl_ObjCmdProc *) getAffineTransform,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::warpAffine",
        (Tcl_ObjCmdProc *) warpAffine,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getPerspectiveTransform",
        (Tcl_ObjCmdProc *) getPerspectiveTransform,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::warpPerspective",
        (Tcl_ObjCmdProc *) warpPerspective,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::filter2D",
        (Tcl_ObjCmdProc *) filter2D,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getGaborKernel",
        (Tcl_ObjCmdProc *) getGaborKernel,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::blur",
        (Tcl_ObjCmdProc *) blur,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::GaussianBlur",
        (Tcl_ObjCmdProc *) GaussianBlur,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::medianBlur",
        (Tcl_ObjCmdProc *) medianBlur,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::bilateralFilter",
        (Tcl_ObjCmdProc *) bilateralFilter,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::boxFilter",
        (Tcl_ObjCmdProc *) boxFilter,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::sqrBoxFilter",
        (Tcl_ObjCmdProc *) sqrBoxFilter,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getStructuringElement",
        (Tcl_ObjCmdProc *) getStructuringElement,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::dilate",
        (Tcl_ObjCmdProc *) dilate,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::erode",
        (Tcl_ObjCmdProc *) erode,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::morphologyEx",
        (Tcl_ObjCmdProc *) morphologyEx,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::pyrUp",
        (Tcl_ObjCmdProc *) pyrUp,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::pyrDown",
        (Tcl_ObjCmdProc *) pyrDown,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::pyrMeanShiftFiltering",
        (Tcl_ObjCmdProc *) pyrMeanShiftFiltering,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Canny",
        (Tcl_ObjCmdProc *) Canny,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Sobel",
        (Tcl_ObjCmdProc *) Sobel,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Scharr",
        (Tcl_ObjCmdProc *) Scharr,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::Laplacian",
        (Tcl_ObjCmdProc *) Laplacian,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::distanceTransform",
        (Tcl_ObjCmdProc *) distanceTransform,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::connectedComponents",
        (Tcl_ObjCmdProc *) connectedComponents,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::watershed",
        (Tcl_ObjCmdProc *) watershed,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::goodFeaturesToTrack",
        (Tcl_ObjCmdProc *) goodFeaturesToTrack,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::cornerHarris",
        (Tcl_ObjCmdProc *) cornerHarris,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::cornerSubPix",
        (Tcl_ObjCmdProc *) cornerSubPix,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::HoughCircles",
        (Tcl_ObjCmdProc *) HoughCircles,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::HoughLines",
        (Tcl_ObjCmdProc *) HoughLines,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::HoughLinesP",
        (Tcl_ObjCmdProc *) HoughLinesP,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::findContours",
        (Tcl_ObjCmdProc *) findContours,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::drawContours",
        (Tcl_ObjCmdProc *) drawContours,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::contourArea",
        (Tcl_ObjCmdProc *) contourArea,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::boundingRect",
        (Tcl_ObjCmdProc *) boundingRect,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::minAreaRect",
        (Tcl_ObjCmdProc *) minAreaRect,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::boxPoints",
        (Tcl_ObjCmdProc *) boxPoints,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::minEnclosingCircle",
        (Tcl_ObjCmdProc *) minEnclosingCircle,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::convexHull",
        (Tcl_ObjCmdProc *) convexHull,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::arrowedLine",
        (Tcl_ObjCmdProc *) arrowedLine,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::circle",
        (Tcl_ObjCmdProc *) circle,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::clipLine",
        (Tcl_ObjCmdProc *) clipLine,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::drawMarker",
        (Tcl_ObjCmdProc *) drawMarker,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::ellipse",
        (Tcl_ObjCmdProc *) ellipse,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::fillConvexPoly",
        (Tcl_ObjCmdProc *) fillConvexPoly,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::line",
        (Tcl_ObjCmdProc *) line,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::polylines",
        (Tcl_ObjCmdProc *) polylines,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::putText",
        (Tcl_ObjCmdProc *) putText,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::rectangle",
        (Tcl_ObjCmdProc *) rectangle,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For videoio
     */

    Tcl_CreateObjCommand(interp, "::" NS "::VideoCapture",
        (Tcl_ObjCmdProc *) VideoCapture,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::VideoWriter",
        (Tcl_ObjCmdProc *) VideoWriter,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For highgui
     */

    Tcl_CreateObjCommand(interp, "::" NS "::namedWindow",
        (Tcl_ObjCmdProc *) namedWindow,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::imshow",
        (Tcl_ObjCmdProc *) imshow,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::waitKey",
        (Tcl_ObjCmdProc *) waitKey,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::moveWindow",
        (Tcl_ObjCmdProc *) moveWindow,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::destroyWindow",
        (Tcl_ObjCmdProc *) destroyWindow,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::destroyAllWindows",
        (Tcl_ObjCmdProc *) destroyAllWindows,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

#ifdef TCL_USE_OPENCV4
    Tcl_CreateObjCommand(interp, "::" NS "::selectROI",
        (Tcl_ObjCmdProc *) selectROI,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);
#endif

    Tcl_CreateObjCommand(interp, "::" NS "::setMouseCallback",
        (Tcl_ObjCmdProc *) setMouseCallback,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::createTrackbar",
        (Tcl_ObjCmdProc *) createTrackbar,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::getTrackbarPos",
        (Tcl_ObjCmdProc *) getTrackbarPos,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For features2d
     */

    Tcl_CreateObjCommand(interp, "::" NS "::drawKeypoints",
        (Tcl_ObjCmdProc *) drawKeypoints,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::drawMatches",
        (Tcl_ObjCmdProc *) drawMatches,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::FastFeatureDetector",
        (Tcl_ObjCmdProc *) FastFeatureDetector,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::AgastFeatureDetector",
        (Tcl_ObjCmdProc *) AgastFeatureDetector,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::ORB",
        (Tcl_ObjCmdProc *) ORB,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::AKAZE",
        (Tcl_ObjCmdProc *) AKAZE,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::BRISK",
        (Tcl_ObjCmdProc *) BRISK,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

#ifdef TCL_USE_SIFT
    Tcl_CreateObjCommand(interp, "::" NS "::SIFT",
        (Tcl_ObjCmdProc *) SIFT,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);
#endif

    Tcl_CreateObjCommand(interp, "::" NS "::BFMatcher",
        (Tcl_ObjCmdProc *) BFMatcher,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::FlannBasedMatcher",
        (Tcl_ObjCmdProc *) FlannBasedMatcher,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::SimpleBlobDetector",
        (Tcl_ObjCmdProc *) SimpleBlobDetector,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For calib3d
     */

    Tcl_CreateObjCommand(interp, "::" NS "::findHomography",
        (Tcl_ObjCmdProc *) findHomography,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::StereoBM",
        (Tcl_ObjCmdProc *) StereoBM,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::StereoSGBM",
        (Tcl_ObjCmdProc *) StereoSGBM,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);


    /*
     * For Video
     */

    Tcl_CreateObjCommand(interp, "::" NS "::BackgroundSubtractorMOG2",
        (Tcl_ObjCmdProc *) BackgroundSubtractorMOG2,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::meanShift",
        (Tcl_ObjCmdProc *) meanShift,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::CamShift",
        (Tcl_ObjCmdProc *) CamShift,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::calcOpticalFlowPyrLK",
        (Tcl_ObjCmdProc *) calcOpticalFlowPyrLK,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::calcOpticalFlowFarneback",
        (Tcl_ObjCmdProc *) calcOpticalFlowFarneback,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For Photo
     */

    Tcl_CreateObjCommand(interp, "::" NS "::inpaint",
        (Tcl_ObjCmdProc *) inpaint,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::decolor",
        (Tcl_ObjCmdProc *) decolor,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::fastNlMeansDenoisingColored",
        (Tcl_ObjCmdProc *) fastNlMeansDenoisingColored,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::colorChange",
        (Tcl_ObjCmdProc *) colorChange,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::illuminationChange",
        (Tcl_ObjCmdProc *) illuminationChange,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::textureFlattening",
        (Tcl_ObjCmdProc *) textureFlattening,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::seamlessClone",
        (Tcl_ObjCmdProc *) seamlessClone,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::detailEnhance",
        (Tcl_ObjCmdProc *) detailEnhance,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::edgePreservingFilter",
        (Tcl_ObjCmdProc *) edgePreservingFilter,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::pencilSketch",
        (Tcl_ObjCmdProc *) pencilSketch,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::stylization",
        (Tcl_ObjCmdProc *) stylization,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::AlignMTB",
        (Tcl_ObjCmdProc *) AlignMTB,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::CalibrateDebevec",
        (Tcl_ObjCmdProc *) CalibrateDebevec,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::MergeDebevec",
        (Tcl_ObjCmdProc *) MergeDebevec,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::MergeMertens",
        (Tcl_ObjCmdProc *) MergeMertens,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::TonemapDrago",
        (Tcl_ObjCmdProc *) TonemapDrago,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::TonemapMantiuk",
        (Tcl_ObjCmdProc *) TonemapMantiuk,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::TonemapReinhard",
        (Tcl_ObjCmdProc *) TonemapReinhard,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For Stitcher
     */

    Tcl_CreateObjCommand(interp, "::" NS "::Stitcher",
        (Tcl_ObjCmdProc *) Stitcher,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * For objdetect
     */

    Tcl_CreateObjCommand(interp, "::" NS "::CascadeClassifier",
        (Tcl_ObjCmdProc *) CascadeClassifier,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

#ifdef TCL_USE_OPENCV4
    Tcl_CreateObjCommand(interp, "::" NS "::QRCodeDetector",
        (Tcl_ObjCmdProc *) QRCodeDetector,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);
#endif

    /*
     * For dnn
     */

#ifdef TCL_USE_OPENCV4
    Tcl_CreateObjCommand(interp, "::" NS "::dnn::blobFromImage",
        (Tcl_ObjCmdProc *) dnn_blobFromImage,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    Tcl_CreateObjCommand(interp, "::" NS "::dnn::readNet",
        (Tcl_ObjCmdProc *) dnn_readNet,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);
#endif

    /*
     * Helper.
     */

    Tcl_CreateObjCommand(interp, "::" NS "::_info",
        (Tcl_ObjCmdProc *) Opencv_Info,
        (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);

    /*
     * Type
     */

    strValue = Tcl_NewStringObj("::" NS "::CV_8U", -1);
    setupValue = Tcl_NewIntObj(CV_8U);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8S", -1);
    setupValue = Tcl_NewIntObj(CV_8S);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16U", -1);
    setupValue = Tcl_NewIntObj(CV_16U);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16S", -1);
    setupValue = Tcl_NewIntObj(CV_16S);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32S", -1);
    setupValue = Tcl_NewIntObj(CV_32S);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32F", -1);
    setupValue = Tcl_NewIntObj(CV_32F);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_64F", -1);
    setupValue = Tcl_NewIntObj(CV_64F);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8UC1", -1);
    setupValue = Tcl_NewIntObj(CV_8UC1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8UC2", -1);
    setupValue = Tcl_NewIntObj(CV_8UC2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8UC3", -1);
    setupValue = Tcl_NewIntObj(CV_8UC3);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8UC4", -1);
    setupValue = Tcl_NewIntObj(CV_8UC4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8SC1", -1);
    setupValue = Tcl_NewIntObj(CV_8SC1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8SC2", -1);
    setupValue = Tcl_NewIntObj(CV_8SC2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8SC3", -1);
    setupValue = Tcl_NewIntObj(CV_8SC3);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_8SC4", -1);
    setupValue = Tcl_NewIntObj(CV_8SC4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16UC1", -1);
    setupValue = Tcl_NewIntObj(CV_16UC1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16UC2", -1);
    setupValue = Tcl_NewIntObj(CV_16UC2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16UC3", -1);
    setupValue = Tcl_NewIntObj(CV_16UC3);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16UC4", -1);
    setupValue = Tcl_NewIntObj(CV_16UC4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16SC1", -1);
    setupValue = Tcl_NewIntObj(CV_16SC1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16SC2", -1);
    setupValue = Tcl_NewIntObj(CV_16SC2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16SC3", -1);
    setupValue = Tcl_NewIntObj(CV_16SC3);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_16SC4", -1);
    setupValue = Tcl_NewIntObj(CV_16SC4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32SC1", -1);
    setupValue = Tcl_NewIntObj(CV_32SC1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32SC2", -1);
    setupValue = Tcl_NewIntObj(CV_32SC2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32SC3", -1);
    setupValue = Tcl_NewIntObj(CV_32SC3);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32SC4", -1);
    setupValue = Tcl_NewIntObj(CV_32SC4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32FC1", -1);
    setupValue = Tcl_NewIntObj(CV_32FC1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32FC2", -1);
    setupValue = Tcl_NewIntObj(CV_32FC2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32FC3", -1);
    setupValue = Tcl_NewIntObj(CV_32FC3);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_32FC4", -1);
    setupValue = Tcl_NewIntObj(CV_32FC4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_64FC1", -1);
    setupValue = Tcl_NewIntObj(CV_64FC1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_64FC2", -1);
    setupValue = Tcl_NewIntObj(CV_64FC2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_64FC3", -1);
    setupValue = Tcl_NewIntObj(CV_64FC3);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CV_64FC4", -1);
    setupValue = Tcl_NewIntObj(CV_64FC4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * imread flags
     */

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_UNCHANGED", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_UNCHANGED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_GRAYSCALE", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_GRAYSCALE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_COLOR", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_COLOR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_ANYDEPTH", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_ANYDEPTH);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_ANYCOLOR", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_ANYCOLOR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_LOAD_GDAL", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_LOAD_GDAL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_REDUCED_GRAYSCALE_2", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_REDUCED_GRAYSCALE_2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_REDUCED_COLOR_2", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_REDUCED_COLOR_2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_REDUCED_GRAYSCALE_4", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_REDUCED_GRAYSCALE_4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_REDUCED_COLOR_4", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_REDUCED_COLOR_4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_REDUCED_GRAYSCALE_8", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_REDUCED_GRAYSCALE_8);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_REDUCED_COLOR_8", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_REDUCED_COLOR_8);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::IMREAD_IGNORE_ORIENTATION", -1);
    setupValue = Tcl_NewIntObj(cv::IMREAD_IGNORE_ORIENTATION);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * VideoCapture flags
     */

    strValue = Tcl_NewStringObj("::" NS "::CAP_ANY", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_ANY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_VFW", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_VFW);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_V4L", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_V4L);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_V4L2", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_V4L2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_FIREWIRE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_FIREWIRE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_FIREWARE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_FIREWARE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_IEEE1394", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_IEEE1394);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_DC1394", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_DC1394);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_CMU1394", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_CMU1394);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_QT", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_QT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_UNICAP", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_UNICAP);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_DSHOW", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_DSHOW);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PVAPI", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PVAPI);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_OPENNI", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_OPENNI);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_OPENNI_ASUS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_OPENNI_ASUS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_ANDROID", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_ANDROID);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_XIAPI", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_XIAPI);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_AVFOUNDATION", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_AVFOUNDATION);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_GIGANETIX", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_GIGANETIX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_MSMF", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_MSMF);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_WINRT", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_WINRT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    strValue = Tcl_NewStringObj("::" NS "::CAP_INTELPERC", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_INTELPERC);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_REALSENSE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_REALSENSE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    strValue = Tcl_NewStringObj("::" NS "::CAP_OPENNI2", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_OPENNI2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_OPENNI2_ASUS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_OPENNI2_ASUS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_OPENNI2_ASTRA", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_OPENNI2_ASTRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    strValue = Tcl_NewStringObj("::" NS "::CAP_GPHOTO2", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_GPHOTO2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_GSTREAMER", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_GSTREAMER);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_FFMPEG", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_FFMPEG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_IMAGES", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_IMAGES);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_ARAVIS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_ARAVIS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_OPENCV_MJPEG", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_OPENCV_MJPEG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_INTEL_MFX", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_INTEL_MFX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_XINE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_XINE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_UEYE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_UEYE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    /*
     * VideoCapture property
     */

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_POS_MSEC", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_POS_MSEC);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_POS_FRAMES", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_POS_FRAMES);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_POS_AVI_RATIO", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_POS_AVI_RATIO);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_FRAME_WIDTH", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_FRAME_WIDTH);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_FRAME_HEIGHT", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_FRAME_HEIGHT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_FPS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_FPS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_FOURCC", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_FOURCC);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_FRAME_COUNT", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_FRAME_COUNT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_FORMAT", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_FORMAT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_MODE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_MODE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_BRIGHTNESS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_BRIGHTNESS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_CONTRAST", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_CONTRAST);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_SATURATION", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_SATURATION);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_HUE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_HUE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_GAIN", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_GAIN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_EXPOSURE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_EXPOSURE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_CONVERT_RGB", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_CONVERT_RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_WHITE_BALANCE_BLUE_U", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_WHITE_BALANCE_BLUE_U);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_RECTIFICATION", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_RECTIFICATION);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_MONOCHROME", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_MONOCHROME);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_SHARPNESS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_SHARPNESS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_AUTO_EXPOSURE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_AUTO_EXPOSURE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_GAMMA", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_GAMMA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_TEMPERATURE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_TEMPERATURE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_TRIGGER", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_TRIGGER);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_TRIGGER_DELAY", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_TRIGGER_DELAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_WHITE_BALANCE_RED_V", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_WHITE_BALANCE_RED_V);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_ZOOM", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_ZOOM);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_FOCUS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_FOCUS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_GUID", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_GUID);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_ISO_SPEED", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_ISO_SPEED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_BACKLIGHT", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_BACKLIGHT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_PAN", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_PAN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_TILT", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_TILT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_ROLL", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_ROLL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_IRIS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_IRIS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_SETTINGS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_SETTINGS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_BUFFERSIZE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_BUFFERSIZE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_AUTOFOCUS", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_AUTOFOCUS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_SAR_NUM", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_SAR_NUM);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_SAR_DEN", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_SAR_DEN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_BACKEND", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_BACKEND);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_CHANNEL", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_CHANNEL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_AUTO_WB", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_AUTO_WB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_WB_TEMPERATURE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_WB_TEMPERATURE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_CODEC_PIXEL_FORMAT", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_CODEC_PIXEL_FORMAT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_BITRATE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_BITRATE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_ORIENTATION_META", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_ORIENTATION_META);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_ORIENTATION_AUTO", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_ORIENTATION_AUTO);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_HW_ACCELERATION", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_HW_ACCELERATION);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::CAP_PROP_HW_DEVICE", -1);
    setupValue = Tcl_NewIntObj(cv::CAP_PROP_HW_DEVICE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    /*
     * VideoWriter property
     */

    strValue = Tcl_NewStringObj("::" NS "::VIDEOWRITER_PROP_QUALITY", -1);
    setupValue = Tcl_NewIntObj(cv::VIDEOWRITER_PROP_QUALITY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::VIDEOWRITER_PROP_FRAMEBYTES", -1);
    setupValue = Tcl_NewIntObj(cv::VIDEOWRITER_PROP_FRAMEBYTES);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::VIDEOWRITER_PROP_NSTRIPES", -1);
    setupValue = Tcl_NewIntObj(cv::VIDEOWRITER_PROP_NSTRIPES);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::VIDEOWRITER_PROP_IS_COLOR", -1);
    setupValue = Tcl_NewIntObj(cv::VIDEOWRITER_PROP_IS_COLOR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::VIDEOWRITER_PROP_DEPTH", -1);
    setupValue = Tcl_NewIntObj(cv::VIDEOWRITER_PROP_DEPTH);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::VIDEOWRITER_PROP_HW_ACCELERATION", -1);
    setupValue = Tcl_NewIntObj(cv::VIDEOWRITER_PROP_HW_ACCELERATION);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::VIDEOWRITER_PROP_HW_DEVICE", -1);
    setupValue = Tcl_NewIntObj(cv::VIDEOWRITER_PROP_HW_DEVICE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    /*
     * cvtColor color space code
     */

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_GRAY2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_GRAY2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_GRAY2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_GRAY2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_GRAY2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_GRAY2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_GRAY2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_GRAY2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2BGR565", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2BGR565);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2BGR565", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2BGR565);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5652BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5652BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5652RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5652RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2BGR565", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2BGR565);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2BGR565", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2BGR565);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5652BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5652BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5652RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5652RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_GRAY2BGR565", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_GRAY2BGR565);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5652GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5652GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2BGR555", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2BGR555);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2BGR555", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2BGR555);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5552BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5552BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5552RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5552RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2BGR555", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2BGR555);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2BGR555", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2BGR555);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5552BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5552BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5552RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5552RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_GRAY2BGR555", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_GRAY2BGR555);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR5552GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR5552GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2XYZ", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2XYZ);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2XYZ", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2XYZ);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_XYZ2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_XYZ2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_XYZ2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_XYZ2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2YCrCb", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2YCrCb);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2YCrCb", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2YCrCb);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YCrCb2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YCrCb2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YCrCb2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YCrCb2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2HSV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2HSV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2HSV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2HSV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2Lab", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2Lab);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2Lab", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2Lab);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2Luv", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2Luv);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2Luv", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2Luv);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2HLS", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2HLS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2HLS", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2HLS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_HSV2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_HSV2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_HSV2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_HSV2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_Lab2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_Lab2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_Lab2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_Lab2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_Luv2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_Luv2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_Luv2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_Luv2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_HLS2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_HLS2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_HLS2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_HLS2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2HSV_FULL", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2HSV_FULL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2HSV_FULL", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2HSV_FULL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2HLS_FULL", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2HLS_FULL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2HLS_FULL", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2HLS_FULL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_HSV2BGR_FULL", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_HSV2BGR_FULL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_HSV2RGB_FULL", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_HSV2RGB_FULL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_HLS2BGR_FULL", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_HLS2BGR_FULL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_HLS2RGB_FULL", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_HLS2RGB_FULL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_LBGR2Lab", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_LBGR2Lab);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_LRGB2Lab", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_LRGB2Lab);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_LBGR2Luv", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_LBGR2Luv);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_LRGB2Luv", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_LRGB2Luv);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_Lab2LBGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_Lab2LBGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_Lab2LRGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_Lab2LRGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_Luv2LBGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_Luv2LBGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_Luv2LRGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_Luv2LRGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2YUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2YUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2YUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2YUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_NV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_NV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_NV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_NV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_NV21", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_NV21);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_NV21", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_NV21);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420sp2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420sp2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420sp2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420sp2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_NV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_NV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_NV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_NV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_NV21", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_NV21);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_NV21", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_NV21);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420sp2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420sp2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420sp2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420sp2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420p2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420p2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420p2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420p2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420p2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420p2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420p2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420p2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_NV21", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_NV21);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_NV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_NV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420sp2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420sp2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV420p2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV420p2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_UYVY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_UYVY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_UYVY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_UYVY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_Y422", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_Y422);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_Y422", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_Y422);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_UYNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_UYNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_UYNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_UYNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_UYVY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_UYVY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_UYVY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_UYVY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_Y422", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_Y422);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_Y422", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_Y422);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_UYNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_UYNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_UYNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_UYNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_YUY2", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_YUY2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_YUY2", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_YUY2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_YVYU", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_YVYU);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_YVYU", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_YVYU);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_YUYV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_YUYV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_YUYV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_YUYV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGB_YUNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGB_YUNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGR_YUNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGR_YUNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_YUY2", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_YUY2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_YUY2", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_YUY2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_YVYU", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_YVYU);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_YVYU", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_YVYU);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_YUYV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_YUYV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_YUYV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_YUYV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2RGBA_YUNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2RGBA_YUNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2BGRA_YUNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2BGRA_YUNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_UYVY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_UYVY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_YUY2", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_YUY2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_Y422", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_Y422);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_UYNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_UYNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_YVYU", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_YVYU);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_YUYV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_YUYV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_YUV2GRAY_YUNV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_YUV2GRAY_YUNV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2mRGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2mRGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_mRGBA2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_mRGBA2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2YUV_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2YUV_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2YUV_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2YUV_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2YUV_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2YUV_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2YUV_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2YUV_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2YUV_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2YUV_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2YUV_I420", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2YUV_I420);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2YUV_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2YUV_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2YUV_IYUV", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2YUV_IYUV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGB2YUV_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGB2YUV_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGR2YUV_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGR2YUV_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_RGBA2YUV_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_RGBA2YUV_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BGRA2YUV_YV12", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BGRA2YUV_YV12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2BGR", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2BGR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2RGB", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2RGB);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2GRAY", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2GRAY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2BGR_VNG", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2BGR_VNG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2BGR_VNG", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2BGR_VNG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2BGR_VNG", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2BGR_VNG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2BGR_VNG", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2BGR_VNG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2RGB_VNG", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2RGB_VNG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2RGB_VNG", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2RGB_VNG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2RGB_VNG", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2RGB_VNG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2RGB_VNG", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2RGB_VNG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2BGR_EA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2BGR_EA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2BGR_EA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2BGR_EA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2BGR_EA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2BGR_EA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2BGR_EA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2BGR_EA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2RGB_EA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2RGB_EA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2RGB_EA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2RGB_EA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2RGB_EA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2RGB_EA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2RGB_EA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2RGB_EA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2BGRA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2BGRA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerBG2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerBG2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGB2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGB2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerRG2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerRG2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLOR_BayerGR2RGBA", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_BayerGR2RGBA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    strValue = Tcl_NewStringObj("::" NS "::COLOR_COLORCVT_MAX", -1);
    setupValue = Tcl_NewIntObj(cv::COLOR_COLORCVT_MAX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Line Types
     */

    strValue = Tcl_NewStringObj("::" NS "::FILLED", -1);
    setupValue = Tcl_NewIntObj(cv::FILLED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::LINE_4", -1);
    setupValue = Tcl_NewIntObj(cv::LINE_4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::LINE_8", -1);
    setupValue = Tcl_NewIntObj(cv::LINE_8);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::LINE_AA", -1);
    setupValue = Tcl_NewIntObj(cv::LINE_AA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Marker Types
     */

    strValue = Tcl_NewStringObj("::" NS "::MARKER_CROSS", -1);
    setupValue = Tcl_NewIntObj(cv::MARKER_CROSS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MARKER_TILTED_CROSS", -1);
    setupValue = Tcl_NewIntObj(cv::MARKER_TILTED_CROSS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MARKER_STAR", -1);
    setupValue = Tcl_NewIntObj(cv::MARKER_STAR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MARKER_DIAMOND", -1);
    setupValue = Tcl_NewIntObj(cv::MARKER_DIAMOND);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MARKER_SQUARE", -1);
    setupValue = Tcl_NewIntObj(cv::MARKER_SQUARE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MARKER_TRIANGLE_UP", -1);
    setupValue = Tcl_NewIntObj(cv::MARKER_TRIANGLE_UP);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MARKER_TRIANGLE_DOWN", -1);
    setupValue = Tcl_NewIntObj(cv::MARKER_TRIANGLE_DOWN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * fontFace
     */

    strValue = Tcl_NewStringObj("::" NS "::FONT_HERSHEY_SIMPLEX", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_HERSHEY_SIMPLEX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::FONT_HERSHEY_PLAIN", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_HERSHEY_PLAIN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::FONT_HERSHEY_DUPLEX", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_HERSHEY_DUPLEX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::FONT_HERSHEY_COMPLEX", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_HERSHEY_COMPLEX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::FONT_HERSHEY_TRIPLEX", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_HERSHEY_TRIPLEX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::FONT_HERSHEY_COMPLEX_SMALL", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_HERSHEY_COMPLEX_SMALL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::FONT_HERSHEY_SCRIPT_SIMPLEX", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_HERSHEY_SCRIPT_SIMPLEX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::FONT_HERSHEY_SCRIPT_COMPLEX", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_HERSHEY_SCRIPT_COMPLEX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::FONT_ITALIC", -1);
    setupValue = Tcl_NewIntObj(cv::FONT_ITALIC);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * threshold
     */

    strValue = Tcl_NewStringObj("::" NS "::THRESH_BINARY", -1);
    setupValue = Tcl_NewIntObj(cv::THRESH_BINARY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::THRESH_BINARY_INV", -1);
    setupValue = Tcl_NewIntObj(cv::THRESH_BINARY_INV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::THRESH_TRUNC", -1);
    setupValue = Tcl_NewIntObj(cv::THRESH_TRUNC);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::THRESH_TOZERO", -1);
    setupValue = Tcl_NewIntObj(cv::THRESH_TOZERO);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::THRESH_TOZERO_INV", -1);
    setupValue = Tcl_NewIntObj(cv::THRESH_TOZERO_INV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::THRESH_MASK", -1);
    setupValue = Tcl_NewIntObj(cv::THRESH_MASK);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::THRESH_OTSU", -1);
    setupValue = Tcl_NewIntObj(cv::THRESH_OTSU);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::THRESH_TRIANGLE", -1);
    setupValue = Tcl_NewIntObj(cv::THRESH_TRIANGLE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Adaptive Thresholding
     */

    strValue = Tcl_NewStringObj("::" NS "::ADAPTIVE_THRESH_MEAN_C", -1);
    setupValue = Tcl_NewIntObj(cv::ADAPTIVE_THRESH_MEAN_C);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::ADAPTIVE_THRESH_GAUSSIAN_C", -1);
    setupValue = Tcl_NewIntObj(cv::ADAPTIVE_THRESH_GAUSSIAN_C);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * border types
     */

    strValue = Tcl_NewStringObj("::" NS "::BORDER_CONSTANT", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_CONSTANT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::BORDER_REPLICATE", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_REPLICATE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::BORDER_REFLECT", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_REFLECT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::BORDER_WRAP", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_WRAP);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::BORDER_REFLECT_101", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_REFLECT_101);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::BORDER_TRANSPARENT", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_TRANSPARENT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::BORDER_REFLECT101", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_REFLECT101);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::BORDER_DEFAULT", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_DEFAULT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::BORDER_ISOLATED", -1);
    setupValue = Tcl_NewIntObj(cv::BORDER_ISOLATED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Morph Shapes
     */

    strValue = Tcl_NewStringObj("::" NS "::MORPH_RECT", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_RECT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_CROSS", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_CROSS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_ELLIPSE", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_ELLIPSE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Morph Types
     */

    strValue = Tcl_NewStringObj("::" NS "::MORPH_ERODE", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_ERODE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_DILATE", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_DILATE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_OPEN", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_OPEN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_CLOSE", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_CLOSE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_GRADIENT", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_GRADIENT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_TOPHAT", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_TOPHAT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_BLACKHAT", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_BLACKHAT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MORPH_HITMISS", -1);
    setupValue = Tcl_NewIntObj(cv::MORPH_HITMISS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * For color map
     */

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_AUTUMN", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_AUTUMN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_BONE", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_BONE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_JET", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_JET);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_WINTER", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_WINTER);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_RAINBOW", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_RAINBOW);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_OCEAN", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_OCEAN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_SUMMER", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_SUMMER);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_SPRING", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_SPRING);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_COOL", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_COOL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_HSV", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_HSV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_PINK", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_PINK);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_HOT", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_HOT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_PARULA", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_PARULA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_MAGMA", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_MAGMA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_INFERNO", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_INFERNO);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_PLASMA", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_PLASMA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_VIRIDIS", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_VIRIDIS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_CIVIDIS", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_CIVIDIS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_TWILIGHT", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_TWILIGHT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_TWILIGHT_SHIFTED", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_TWILIGHT_SHIFTED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_TURBO", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_TURBO);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::COLORMAP_DEEPGREEN", -1);
    setupValue = Tcl_NewIntObj(cv::COLORMAP_DEEPGREEN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    /*
     * Windows flags
     */

    strValue = Tcl_NewStringObj("::" NS "::WINDOW_NORMAL", -1);
    setupValue = Tcl_NewIntObj(cv::WINDOW_NORMAL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WINDOW_AUTOSIZE", -1);
    setupValue = Tcl_NewIntObj(cv::WINDOW_AUTOSIZE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WINDOW_OPENGL", -1);
    setupValue = Tcl_NewIntObj(cv::WINDOW_OPENGL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WINDOW_FULLSCREEN", -1);
    setupValue = Tcl_NewIntObj(cv::WINDOW_FULLSCREEN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WINDOW_FREERATIO", -1);
    setupValue = Tcl_NewIntObj(cv::WINDOW_FREERATIO);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WINDOW_KEEPRATIO", -1);
    setupValue = Tcl_NewIntObj(cv::WINDOW_KEEPRATIO);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WINDOW_GUI_EXPANDED", -1);
    setupValue = Tcl_NewIntObj(cv::WINDOW_GUI_EXPANDED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WINDOW_GUI_NORMAL", -1);
    setupValue = Tcl_NewIntObj(cv::WINDOW_GUI_NORMAL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Mouse event
     */

    strValue = Tcl_NewStringObj("::" NS "::EVENT_MOUSEMOVE", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_MOUSEMOVE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_LBUTTONDOWN", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_LBUTTONDOWN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_RBUTTONDOWN", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_RBUTTONDOWN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_MBUTTONDOWN", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_MBUTTONDOWN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_LBUTTONUP", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_LBUTTONUP);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_RBUTTONUP", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_RBUTTONUP);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_MBUTTONUP", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_MBUTTONUP);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_LBUTTONDBLCLK", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_LBUTTONDBLCLK);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_RBUTTONDBLCLK", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_RBUTTONDBLCLK);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_MBUTTONDBLCLK", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_MBUTTONDBLCLK);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_MOUSEWHEEL", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_MOUSEWHEEL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_MOUSEHWHEEL", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_MOUSEHWHEEL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Mouse event flag
     */

    strValue = Tcl_NewStringObj("::" NS "::EVENT_FLAG_LBUTTON", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_FLAG_LBUTTON);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_FLAG_RBUTTON", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_FLAG_RBUTTON);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_FLAG_MBUTTON", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_FLAG_MBUTTON);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_FLAG_CTRLKEY", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_FLAG_CTRLKEY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_FLAG_SHIFTKEY", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_FLAG_SHIFTKEY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EVENT_FLAG_ALTKEY", -1);
    setupValue = Tcl_NewIntObj(cv::EVENT_FLAG_ALTKEY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * dft flags
     */

    strValue = Tcl_NewStringObj("::" NS "::DFT_INVERSE", -1);
    setupValue = Tcl_NewIntObj(cv::DFT_INVERSE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DFT_SCALE", -1);
    setupValue = Tcl_NewIntObj(cv::DFT_SCALE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DFT_ROWS", -1);
    setupValue = Tcl_NewIntObj(cv::DFT_ROWS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DFT_COMPLEX_OUTPUT", -1);
    setupValue = Tcl_NewIntObj(cv::DFT_COMPLEX_OUTPUT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DFT_REAL_OUTPUT", -1);
    setupValue = Tcl_NewIntObj(cv::DFT_REAL_OUTPUT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::DFT_COMPLEX_INPUT", -1);
    setupValue = Tcl_NewIntObj(cv::DFT_COMPLEX_INPUT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    strValue = Tcl_NewStringObj("::" NS "::DCT_INVERSE", -1);
    setupValue = Tcl_NewIntObj(cv::DCT_INVERSE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DCT_ROWS", -1);
    setupValue = Tcl_NewIntObj(cv::DCT_ROWS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * NormTypes
     */

    strValue = Tcl_NewStringObj("::" NS "::NORM_INF", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_INF);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORM_L1", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_L1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORM_L2", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_L2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORM_L2SQR", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_L2SQR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORM_HAMMING", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_HAMMING);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORM_HAMMING2", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_HAMMING2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORM_TYPE_MASK", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_TYPE_MASK);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORM_RELATIVE", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_RELATIVE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORM_MINMAX", -1);
    setupValue = Tcl_NewIntObj(cv::NORM_MINMAX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);


    /*
     * FloodFill Flags
     */

    strValue = Tcl_NewStringObj("::" NS "::FLOODFILL_FIXED_RANGE", -1);
    setupValue = Tcl_NewIntObj(cv::FLOODFILL_FIXED_RANGE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Interpolation Flags
     */

    strValue = Tcl_NewStringObj("::" NS "::INTER_NEAREST", -1);
    setupValue = Tcl_NewIntObj(cv::INTER_NEAREST);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::INTER_LINEAR", -1);
    setupValue = Tcl_NewIntObj(cv::INTER_LINEAR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::INTER_CUBIC", -1);
    setupValue = Tcl_NewIntObj(cv::INTER_CUBIC);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::INTER_AREA", -1);
    setupValue = Tcl_NewIntObj(cv::INTER_AREA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::INTER_LANCZOS4", -1);
    setupValue = Tcl_NewIntObj(cv::INTER_LANCZOS4);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::INTER_LINEAR_EXACT", -1);
    setupValue = Tcl_NewIntObj(cv::INTER_LINEAR_EXACT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::INTER_NEAREST_EXACT", -1);
    setupValue = Tcl_NewIntObj(cv::INTER_NEAREST_EXACT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    strValue = Tcl_NewStringObj("::" NS "::INTER_MAX", -1);
    setupValue = Tcl_NewIntObj(cv::INTER_MAX);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WARP_FILL_OUTLIERS", -1);
    setupValue = Tcl_NewIntObj(cv::WARP_FILL_OUTLIERS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::WARP_INVERSE_MAP", -1);
    setupValue = Tcl_NewIntObj(cv::WARP_INVERSE_MAP);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Decomp Types
     */

    strValue = Tcl_NewStringObj("::" NS "::DECOMP_LU", -1);
    setupValue = Tcl_NewIntObj(cv::DECOMP_LU);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DECOMP_SVD", -1);
    setupValue = Tcl_NewIntObj(cv::DECOMP_SVD);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DECOMP_EIG", -1);
    setupValue = Tcl_NewIntObj(cv::DECOMP_EIG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DECOMP_CHOLESKY", -1);
    setupValue = Tcl_NewIntObj(cv::DECOMP_CHOLESKY);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DECOMP_QR", -1);
    setupValue = Tcl_NewIntObj(cv::DECOMP_QR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DECOMP_NORMAL", -1);
    setupValue = Tcl_NewIntObj(cv::DECOMP_NORMAL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Retrieval Modes
     */

    strValue = Tcl_NewStringObj("::" NS "::RETR_EXTERNAL", -1);
    setupValue = Tcl_NewIntObj(cv::RETR_EXTERNAL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::RETR_LIST", -1);
    setupValue = Tcl_NewIntObj(cv::RETR_LIST);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::RETR_CCOMP", -1);
    setupValue = Tcl_NewIntObj(cv::RETR_CCOMP);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::RETR_TREE", -1);
    setupValue = Tcl_NewIntObj(cv::RETR_TREE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::RETR_FLOODFILL", -1);
    setupValue = Tcl_NewIntObj(cv::RETR_FLOODFILL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Contour Approximation Modes
     */

    strValue = Tcl_NewStringObj("::" NS "::CHAIN_APPROX_NONE", -1);
    setupValue = Tcl_NewIntObj(cv::CHAIN_APPROX_NONE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CHAIN_APPROX_SIMPLE", -1);
    setupValue = Tcl_NewIntObj(cv::CHAIN_APPROX_SIMPLE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CHAIN_APPROX_TC89_L1", -1);
    setupValue = Tcl_NewIntObj(cv::CHAIN_APPROX_TC89_L1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::CHAIN_APPROX_TC89_KCOS", -1);
    setupValue = Tcl_NewIntObj(cv::CHAIN_APPROX_TC89_KCOS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Hough Modes for HoughCircles
     */

    strValue = Tcl_NewStringObj("::" NS "::HOUGH_GRADIENT", -1);
    setupValue = Tcl_NewIntObj(cv::HOUGH_GRADIENT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::HOUGH_GRADIENT_ALT", -1);
    setupValue = Tcl_NewIntObj(cv::HOUGH_GRADIENT_ALT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    /*
     * Template Match Modes
     */

    strValue = Tcl_NewStringObj("::" NS "::TM_SQDIFF", -1);
    setupValue = Tcl_NewIntObj(cv::TM_SQDIFF);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::TM_SQDIFF_NORMED", -1);
    setupValue = Tcl_NewIntObj(cv::TM_SQDIFF_NORMED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::TM_CCORR", -1);
    setupValue = Tcl_NewIntObj(cv::TM_CCORR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::TM_CCORR_NORMED", -1);
    setupValue = Tcl_NewIntObj(cv::TM_CCORR_NORMED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::TM_CCOEFF", -1);
    setupValue = Tcl_NewIntObj(cv::TM_CCOEFF);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::TM_CCOEFF_NORMED", -1);
    setupValue = Tcl_NewIntObj(cv::TM_CCOEFF_NORMED);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Distance Types
     */

    strValue = Tcl_NewStringObj("::" NS "::DIST_L1", -1);
    setupValue = Tcl_NewIntObj(cv::DIST_L1);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DIST_L2", -1);
    setupValue = Tcl_NewIntObj(cv::DIST_L2);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DIST_C", -1);
    setupValue = Tcl_NewIntObj(cv::DIST_C);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DIST_L12", -1);
    setupValue = Tcl_NewIntObj(cv::DIST_L12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DIST_FAIR", -1);
    setupValue = Tcl_NewIntObj(cv::DIST_FAIR);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DIST_WELSCH", -1);
    setupValue = Tcl_NewIntObj(cv::DIST_WELSCH);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DIST_HUBER", -1);
    setupValue = Tcl_NewIntObj(cv::DIST_HUBER);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * GrabCut Classes
     */

    strValue = Tcl_NewStringObj("::" NS "::GC_BGD", -1);
    setupValue = Tcl_NewIntObj(cv::GC_BGD);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::GC_FGD", -1);
    setupValue = Tcl_NewIntObj(cv::GC_FGD);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::GC_PR_BGD", -1);
    setupValue = Tcl_NewIntObj(cv::GC_PR_BGD);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::GC_PR_FGD", -1);
    setupValue = Tcl_NewIntObj(cv::GC_PR_FGD);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * TermCriteria type
     */

    strValue = Tcl_NewStringObj("::" NS "::COUNT", -1);
    setupValue = Tcl_NewIntObj(cv::TermCriteria::COUNT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::EPS", -1);
    setupValue = Tcl_NewIntObj(cv::TermCriteria::EPS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * FastFeature Detector type
     */

    strValue = Tcl_NewStringObj("::" NS "::DetectorType_TYPE_5_8", -1);
    setupValue = Tcl_NewIntObj(cv::FastFeatureDetector::TYPE_5_8);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DetectorType_TYPE_7_12", -1);
    setupValue = Tcl_NewIntObj(cv::FastFeatureDetector::TYPE_7_12);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DetectorType_TYPE_9_16", -1);
    setupValue = Tcl_NewIntObj(cv::FastFeatureDetector::TYPE_9_16);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * AgastFeatureDetector Detector type
     */

    strValue = Tcl_NewStringObj( "::" NS "::DetectorType_AGAST_5_8", -1 );
    setupValue = Tcl_NewIntObj( cv::AgastFeatureDetector::AGAST_5_8 );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::DetectorType_AGAST_7_12d", -1 );
    setupValue = Tcl_NewIntObj( cv::AgastFeatureDetector::AGAST_7_12d );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::DetectorType_AGAST_7_12s", -1 );
    setupValue = Tcl_NewIntObj( cv::AgastFeatureDetector::AGAST_7_12s );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::DetectorType_OAST_9_16", -1 );
    setupValue = Tcl_NewIntObj( cv::AgastFeatureDetector::OAST_9_16 );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * ORB Detector type
     */

    strValue = Tcl_NewStringObj("::" NS "::ORB_HARRIS_SCORE", -1);
#ifdef TCL_USE_OPENCV4
    setupValue = Tcl_NewIntObj(cv::ORB::ScoreType::HARRIS_SCORE);
#else
    setupValue = Tcl_NewIntObj(cv::ORB::HARRIS_SCORE);
#endif
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::ORB_FAST_SCORE", -1);
#ifdef TCL_USE_OPENCV4
    setupValue = Tcl_NewIntObj(cv::ORB::ScoreType::FAST_SCORE);
#else
    setupValue = Tcl_NewIntObj(cv::ORB::FAST_SCORE);
#endif
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * AKAZE descriptor type
     */

    strValue = Tcl_NewStringObj( "::" NS "::AKAZE_DESCRIPTOR_KAZE_UPRIGHT", -1 );
    setupValue = Tcl_NewIntObj( cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::AKAZE_DESCRIPTOR_KAZE", -1 );
    setupValue = Tcl_NewIntObj( cv::AKAZE::DESCRIPTOR_KAZE  );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::AKAZE_DESCRIPTOR_MLDB_UPRIGHT", -1 );
    setupValue = Tcl_NewIntObj( cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::AKAZE_DESCRIPTOR_MLDB", -1 );
    setupValue = Tcl_NewIntObj( cv::AKAZE::DESCRIPTOR_MLDB  );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * AKAZE Diffusivity type
     */

    strValue = Tcl_NewStringObj( "::" NS "::AKAZE_DIFF_PM_G1", -1 );
    setupValue = Tcl_NewIntObj( cv::KAZE::DiffusivityType::DIFF_PM_G1 );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::AKAZE_DIFF_PM_G2", -1 );
    setupValue = Tcl_NewIntObj( cv::KAZE::DiffusivityType::DIFF_PM_G2  );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::AKAZE_DIFF_WEICKERT", -1 );
    setupValue = Tcl_NewIntObj( cv::KAZE::DiffusivityType::DIFF_WEICKERT );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::AKAZE_DIFF_CHARBONNIER", -1 );
    setupValue = Tcl_NewIntObj( cv::KAZE::DiffusivityType::DIFF_CHARBONNIER  );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * drawKeypoints DrawMatches Flags
     */

    strValue = Tcl_NewStringObj("::" NS "::DRAW_MATCHES_FLAGS_DEFAULT", -1);
    setupValue = Tcl_NewIntObj((int) cv::DrawMatchesFlags::DEFAULT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DRAW_MATCHES_FLAGS_DRAW_OVER_OUTIMG", -1);
    setupValue = Tcl_NewIntObj((int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS", -1);
    setupValue = Tcl_NewIntObj((int) cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS", -1);
    setupValue = Tcl_NewIntObj((int) cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);


    /*
     * findHomography method
     */

    strValue = Tcl_NewStringObj("::" NS "::LMEDS", -1);
    setupValue = Tcl_NewIntObj(cv::LMEDS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::RANSAC", -1);
    setupValue = Tcl_NewIntObj(cv::RANSAC);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::RHO", -1);
    setupValue = Tcl_NewIntObj(cv::RHO);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * StereoBM PreFilter type
     */

    strValue = Tcl_NewStringObj( "::" NS "::PREFILTER_NORMALIZED_RESPONSE", -1 );
    setupValue = Tcl_NewIntObj( cv::StereoBM::PREFILTER_NORMALIZED_RESPONSE );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::PREFILTER_XSOBEL", -1 );
    setupValue = Tcl_NewIntObj( cv::StereoBM::PREFILTER_XSOBEL );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * StereoSGBM mode
     */

    strValue = Tcl_NewStringObj( "::" NS "::StereoSGBM_MODE_SGBM", -1 );
    setupValue = Tcl_NewIntObj( cv::StereoSGBM::MODE_SGBM );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::StereoSGBM_MODE_HH", -1 );
    setupValue = Tcl_NewIntObj( cv::StereoSGBM::MODE_HH );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::StereoSGBM_MODE_SGBM_3WAY", -1 );
    setupValue = Tcl_NewIntObj( cv::StereoSGBM::MODE_SGBM_3WAY );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj( "::" NS "::StereoSGBM_MODE_HH4", -1 );
    setupValue = Tcl_NewIntObj( cv::StereoSGBM::MODE_HH4 );
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Photo inpaint flags
     */

    strValue = Tcl_NewStringObj("::" NS "::INPAINT_NS", -1);
    setupValue = Tcl_NewIntObj(cv::INPAINT_NS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::INPAINT_TELEA", -1);
    setupValue = Tcl_NewIntObj(cv::INPAINT_TELEA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Photo edgePreservingFilter flags
     */

    strValue = Tcl_NewStringObj("::" NS "::RECURS_FILTER", -1);
    setupValue = Tcl_NewIntObj(cv::RECURS_FILTER);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::NORMCONV_FILTER", -1);
    setupValue = Tcl_NewIntObj(cv::NORMCONV_FILTER);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Photo seamlessClone flags
     */

    strValue = Tcl_NewStringObj("::" NS "::NORMAL_CLONE", -1);
    setupValue = Tcl_NewIntObj(cv::NORMAL_CLONE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MIXED_CLONE", -1);
    setupValue = Tcl_NewIntObj(cv::MIXED_CLONE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::MONOCHROME_TRANSFER", -1);
    setupValue = Tcl_NewIntObj(cv::MONOCHROME_TRANSFER);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * Stitcher mode
     */

    strValue = Tcl_NewStringObj("::" NS "::PANORAMA", -1);
    setupValue = Tcl_NewIntObj(cv::Stitcher::PANORAMA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::SCANS", -1);
    setupValue = Tcl_NewIntObj(cv::Stitcher::SCANS);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    /*
     * DNN backend
     */

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_BACKEND_DEFAULT", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_BACKEND_DEFAULT);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_BACKEND_HALIDE", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_BACKEND_HALIDE);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_BACKEND_OPENCV", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_BACKEND_OPENCV);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_BACKEND_VKCOM", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_BACKEND_VKCOM);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_BACKEND_CUDA", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_BACKEND_CUDA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    /*
     * DNN target
     */

#ifdef TCL_USE_OPENCV4
    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_TARGET_CPU", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_TARGET_CPU);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_TARGET_OPENCL", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_TARGET_OPENCL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_TARGET_OPENCL_FP16", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_TARGET_OPENCL_FP16);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_TARGET_MYRIAD", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_TARGET_MYRIAD);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_TARGET_VULKAN", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_TARGET_VULKAN);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_TARGET_CUDA", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_TARGET_CUDA);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_TARGET_CUDA_FP16", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_TARGET_CUDA_FP16);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);

    strValue = Tcl_NewStringObj("::" NS "::dnn::DNN_TARGET_HDDL", -1);
    setupValue = Tcl_NewIntObj(cv::dnn::DNN_TARGET_HDDL);
    Tcl_ObjSetVar2 (interp, strValue, NULL, setupValue, TCL_NAMESPACE_ONLY);
#endif

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif  /* __cplusplus */
