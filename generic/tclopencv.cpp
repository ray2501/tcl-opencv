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

#ifdef TCL_USE_TKPHOTO
#include <tk.h>
#endif

#ifdef TCL_USE_VECTCL
#ifdef __cplusplus
extern "C" {
#endif
#include <vectcl.h>
#ifdef __cplusplus
}
#endif
#endif


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
                if (mat) {
                    delete mat;
                }
                break;
            }
            case OPENCV_FSTORAGE: {
                cv::FileStorage *fs = (cv::FileStorage *) cvo->obj;
                if (fs->isOpened()) {
                    fs->release();
                }
                delete fs;
                Tcl_DStringFree(&cvo->ds1);
                Tcl_DStringFree(&cvo->ds2);
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
            case OPENCV_PCA: {
                cv::PCA *pca = (cv::PCA *) cvo->obj;
                delete pca;
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
            case OPENCV_OOBJHOG: {
                cv::HOGDescriptor *hog = (cv::HOGDescriptor *) cvo->obj;
                delete hog;
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
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 1)
            case OPENCV_TEXTDETECTEAST: {
                cv::dnn::TextDetectionModel_EAST *east = (cv::dnn::TextDetectionModel_EAST *) cvo->obj;
                delete east;
                break;
            }
            case OPENCV_TEXTDETECTDB: {
                cv::dnn::TextDetectionModel_DB *db = (cv::dnn::TextDetectionModel_DB *) cvo->obj;
                delete db;
                break;
            }
            case OPENCV_TEXTRECOGNITION: {
                cv::dnn::TextRecognitionModel *recognizer = (cv::dnn::TextRecognitionModel *) cvo->obj;
                delete recognizer;
                break;
            }
#endif
#endif
            case OPENCV_BOWTRAINER: {
                cv::BOWKMeansTrainer *bowkmeanstrainer = (cv::BOWKMeansTrainer *) cvo->obj;
                delete bowkmeanstrainer;
                break;
            }
            case OPENCV_BOWEXTRACTOR: {
                cv::BOWImgDescriptorExtractor *bowimgextractor = (cv::BOWImgDescriptorExtractor *) cvo->obj;
                delete bowimgextractor;
                break;
            }
            default:
                Tcl_Panic("wrong Opencv type");
                break;
            }
            ckfree(cvo);
            Tcl_DeleteHashEntry(hashEntryPtr);
        }
        Tcl_DeleteHashTable(&cvd->tbl[i]);
    }
    if (cvd->clahe) {
        cvd->clahe.release();
    }
    if (cvd->houghballard) {
        cvd->houghballard.release();
    }
    if (cvd->houghbuil) {
        cvd->houghbuil.release();
    }
    if (cvd->lsdetector) {
        cvd->lsdetector.release();
    }
    if (cvd->fastdetector) {
        cvd->fastdetector.release();
    }
    if (cvd->agastdetector) {
        cvd->agastdetector.release();
    }
    if (cvd->mserextractor) {
        cvd->mserextractor.release();
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
    if (cvd->kazedetector) {
        cvd->kazedetector.release();
    }
#ifdef TCL_USE_SIFT
    if (cvd->siftdetector) {
        cvd->siftdetector.release();
    }
    if (cvd->asiftdetector) {
        cvd->asiftdetector.release();
    }
#endif
    if (cvd->bfmatcher) {
        cvd->bfmatcher.release();
    }
    if (cvd->flannbasedmatcher) {
        cvd->flannbasedmatcher.release();
    }
    if (cvd->bgsknn) {
        cvd->bgsknn.release();
    }
    if (cvd->bgsmog2) {
        cvd->bgsmog2.release();
    }
#ifdef TCL_USE_OPENCV4
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 1)
    if (cvd->trackerMIL) {
        cvd->trackerMIL.release();
    }
    if (cvd->trackerGOTURN) {
        cvd->trackerGOTURN.release();
    }
#endif
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 3)
    if (cvd->trackerDaSiamRPN) {
        cvd->trackerDaSiamRPN.release();
    }
#endif
#endif
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
    if (cvd->logistic) {
        cvd->logistic.release();
    }
    if (cvd->bayesclassifier) {
        cvd->bayesclassifier.release();
    }
    if (cvd->knearest) {
        cvd->knearest.release();
    }
    if (cvd->svm) {
        cvd->svm.release();
    }
    if (cvd->svmsgd) {
        cvd->svmsgd.release();
    }
    if (cvd->dtrees) {
        cvd->dtrees.release();
    }
    if (cvd->boost) {
        cvd->boost.release();
    }
    if (cvd->rtrees) {
        cvd->rtrees.release();
    }
    if (cvd->annmlp) {
        cvd->annmlp.release();
    }
    if (cvd->traindata) {
        cvd->traindata.release();
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


static const char *
Opencv_Err2Str(int code)
{
    const char *codebuf = NULL;

    switch (code) {
    case cv::Error::StsOk: codebuf = "StsOk"; break;
    case cv::Error::StsBackTrace: codebuf = "StsBackTrace"; break;
    case cv::Error::StsError: codebuf = "StsError"; break;
    case cv::Error::StsInternal: codebuf = "StsInternal"; break;
    case cv::Error::StsNoMem: codebuf = "StsNoMem"; break;
    case cv::Error::StsBadArg: codebuf = "StsBadArg"; break;
    case cv::Error::StsBadFunc: codebuf = "StsBadFunc"; break;
    case cv::Error::StsNoConv: codebuf = "StsNoConv"; break;
    case cv::Error::StsAutoTrace: codebuf = "StsAutoTrace"; break;
    case cv::Error::HeaderIsNull: codebuf = "HeaderIsNull"; break;
    case cv::Error::BadImageSize: codebuf = "BadImageSize"; break;
    case cv::Error::BadOffset: codebuf = "BadOffset"; break;
    case cv::Error::BadDataPtr: codebuf = "BadDataPtr"; break;
    case cv::Error::BadStep: codebuf = "BadStep"; break;
    case cv::Error::BadModelOrChSeq: codebuf = "BadModelOrChSeq"; break;
    case cv::Error::BadNumChannels: codebuf = "BadNumChannels"; break;
    case cv::Error::BadNumChannel1U: codebuf = "BadNumChannel1U"; break;
    case cv::Error::BadDepth: codebuf = "BadDepth"; break;
    case cv::Error::BadAlphaChannel: codebuf = "BadAlphaChannel"; break;
    case cv::Error::BadOrder: codebuf = "BadOrder"; break;
    case cv::Error::BadOrigin: codebuf = "BadOrigin"; break;
    case cv::Error::BadAlign: codebuf = "BadAlign"; break;
    case cv::Error::BadCallBack: codebuf = "BadCallBack"; break;
    case cv::Error::BadTileSize: codebuf = "BadTileSize"; break;
    case cv::Error::BadCOI: codebuf = "BadCOI"; break;
    case cv::Error::BadROISize: codebuf = "BadROISize"; break;
    case cv::Error::MaskIsTiled: codebuf = "MaskIsTiled"; break;
    case cv::Error::StsNullPtr: codebuf = "StsNullPtr"; break;
    case cv::Error::StsVecLengthErr: codebuf = "StsVecLengthErr"; break;
    case cv::Error::StsFilterStructContentErr: codebuf = "StsFilterStructContentErr"; break;
    case cv::Error::StsKernelStructContentErr: codebuf = "StsKernelStructContentErr"; break;
    case cv::Error::StsFilterOffsetErr: codebuf = "StsFilterOffsetErr"; break;
    case cv::Error::StsBadSize: codebuf = "StsBadSize"; break;
    case cv::Error::StsDivByZero: codebuf = "StsDivByZero"; break;
    case cv::Error::StsInplaceNotSupported: codebuf = "StsInplaceNotSupported"; break;
    case cv::Error::StsObjectNotFound: codebuf = "StsObjectNotFound"; break;
    case cv::Error::StsUnmatchedFormats: codebuf = "StsUnmatchedFormats"; break;
    case cv::Error::StsBadFlag: codebuf = "StsBadFlag"; break;
    case cv::Error::StsBadPoint: codebuf = "StsBadPoint"; break;
    case cv::Error::StsBadMask: codebuf = "StsBadMask"; break;
    case cv::Error::StsUnmatchedSizes: codebuf = "StsUnmatchedSizes"; break;
    case cv::Error::StsUnsupportedFormat: codebuf = "StsUnsupportedFormat"; break;
    case cv::Error::StsOutOfRange: codebuf = "StsOutOfRange"; break;
    case cv::Error::StsParseError: codebuf = "StsParseError"; break;
    case cv::Error::StsNotImplemented: codebuf = "StsNotImplemented"; break;
    case cv::Error::StsBadMemBlock: codebuf = "StsBadMemBlock"; break;
    case cv::Error::StsAssert: codebuf = "StsAssert"; break;
    case cv::Error::GpuNotSupported: codebuf = "GpuNotSupported"; break;
    case cv::Error::GpuApiCallError: codebuf = "GpuApiCallError"; break;
    case cv::Error::OpenGlNotSupported: codebuf = "OpenGlNotSupported"; break;
    case cv::Error::OpenGlApiCallError: codebuf = "OpenGlApiCallError"; break;
    case cv::Error::OpenCLApiCallError: codebuf = "OpenCLApiCallError"; break;
    case cv::Error::OpenCLDoubleNotSupported: codebuf = "OpenCLDoubleNotSupported"; break;
    case cv::Error::OpenCLInitError: codebuf = "OpenCLInitError"; break;
    case cv::Error::OpenCLNoAMDBlasFft: codebuf = "OpenCLNoAMDBlasFft"; break;
    }
    return codebuf;
}


int
Opencv_Exc2Tcl(Tcl_Interp *interp, const cv::Exception *ex)
{
    const char *codebuf;
    char numbuf[64];

    if (!ex) {
        Tcl_SetErrorCode(interp, "RUNTIME", "Unspecific", "-1", NULL);
        Tcl_SetResult(interp, (char *) "unspecific exception", TCL_STATIC);
        return TCL_ERROR;
    }
    sprintf(numbuf, "Unknown_%d", ex->code);
    codebuf = Opencv_Err2Str(ex->code);
    if (codebuf == NULL) {
        codebuf = numbuf;
    }
    Tcl_SetErrorCode(interp, "OPENCV", codebuf, numbuf + 8, NULL);
    Tcl_SetObjResult(interp, Tcl_NewStringObj(ex->msg.c_str(), ex->msg.length()));
    return (ex->code == cv::Error::StsOk) ? TCL_OK : TCL_ERROR;
}


int
Opencv_SetResult(Tcl_Interp *interp, int code, const char *msg)
{
    const char *codebuf;
    char numbuf[64];

    sprintf(numbuf, "Unknown_%d", code);
    codebuf = Opencv_Err2Str(code);
    if (codebuf == NULL) {
        codebuf = numbuf;
    }
    Tcl_SetErrorCode(interp, "OPENCV", codebuf, numbuf + 8, NULL);
    Tcl_SetObjResult(interp, Tcl_NewStringObj(msg, -1));
    return (code == cv::Error::StsOk) ? TCL_OK : TCL_ERROR;
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
    if (cvo->key) {
        hashEntryPtr = Tcl_FindHashEntry(&cvd->tbl[cvo->type], cvo->key);
        if (hashEntryPtr != NULL) {
            Tcl_DeleteHashEntry(hashEntryPtr);
        }
        cvo->key = NULL;
    }
    if (cvo->obj) {
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
        case OPENCV_FSTORAGE: {
            cv::FileStorage *fs = (cv::FileStorage *) cvo->obj;
            if (fs->isOpened()) {
                fs->release();
            }
            delete fs;
            Tcl_DStringFree(&cvo->ds1);
            Tcl_DStringFree(&cvo->ds2);
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
        case OPENCV_PCA: {
            cv::PCA *pca = (cv::PCA *) cvo->obj;
            delete pca;
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
        case OPENCV_OOBJHOG: {
            cv::HOGDescriptor *hog = (cv::HOGDescriptor *) cvo->obj;
            delete hog;
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
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 1)
        case OPENCV_TEXTDETECTEAST: {
            cv::dnn::TextDetectionModel_EAST *east = (cv::dnn::TextDetectionModel_EAST *) cvo->obj;
            delete east;
            break;
        }
        case OPENCV_TEXTDETECTDB: {
            cv::dnn::TextDetectionModel_DB *db = (cv::dnn::TextDetectionModel_DB *) cvo->obj;
            delete db;
            break;
        }
        case OPENCV_TEXTRECOGNITION: {
            cv::dnn::TextRecognitionModel *recognizer = (cv::dnn::TextRecognitionModel *) cvo->obj;
            delete recognizer;
            break;
        }
#endif
#endif
        case OPENCV_BOWTRAINER: {
            cv::BOWKMeansTrainer *bowtrainer = (cv::BOWKMeansTrainer *) cvo->obj;
            delete bowtrainer;
            break;
        }
        case OPENCV_BOWEXTRACTOR: {
            cv::BOWImgDescriptorExtractor *bowimgextractor = (cv::BOWImgDescriptorExtractor *) cvo->obj;
            delete bowimgextractor;
            break;
        }
        default:
            Tcl_Panic("wrong Opencv type");
            break;
        }
    }
    if (cvo->traced && cvo->obj) {
        cvo->obj = NULL;
        return;
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
    case OPENCV_FSTORAGE:
        prefix = "cv-fstorage";
        proc = FileStorage_FUNCTION;
        break;
    case OPENCV_VIDEOCAPTURE:
        prefix = "cv-videoc";
        proc = VideoCapture_FUNCTION;
        break;
    case OPENCV_VIDEOWRITER:
        prefix = "cv-videow";
        proc = VideoWriter_FUNCTION;
        break;
    case OPENCV_PCA:
        prefix = "cv-pca";
        proc = PCA_FUNCTION;
        break;
    case OPENCV_TERMCRITERIA:
        prefix = "cv-term";
        proc = TERM_FUNCTION;
        break;
    case OPENCV_ODETECT:
        prefix = "cv-odetect";
        proc = CascadeClassifier_FUNCTION;
        break;
    case OPENCV_OOBJHOG:
        prefix = "cv-oobjhog";
        proc = HOGDescriptor_FUNCTION;
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
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 1)
    case OPENCV_TEXTDETECTEAST:
        prefix = "cv-textdetecteast";
        proc = TEXTDETECT_EAST_FUNCTION;
        break;
    case OPENCV_TEXTDETECTDB:
        prefix = "cv-textdetectdb";
        proc = TEXTDETECT_DB_FUNCTION;
        break;
    case OPENCV_TEXTRECOGNITION:
        prefix = "cv-textrecognition";
        proc = TEXTRECOGNITION_FUNCTION;
        break;
#endif
#endif
    case OPENCV_BOWTRAINER:
        prefix = "cv-bowktrainer";
        proc = BOWKMeansTrainer_FUNCTION;
        break;
    case OPENCV_BOWEXTRACTOR:
        prefix = "cv-bowimgextractor";
        proc = BOWImgDescriptorExtractor_FUNCTION;
        break;
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
    cvo->traced = 0;
    cvo->flags = 0;
    if (type == OPENCV_FSTORAGE) {
        Tcl_DStringInit(&cvo->ds1);
        Tcl_DStringInit(&cvo->ds2);
    }
    if (proc) {
        name = Tcl_NewStringObj(buffer, -1);
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
        if (interp != NULL) {
            Tcl_SetObjResult(interp,
                    Tcl_ObjPrintf("handle \"%s\" not found", string));
        }
        return NULL;
    }
    cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);
    if (cvo == NULL) {
        Tcl_Panic("null CV handle");
    }
    if (cvo->obj == NULL) {
        Tcl_Panic("null CV object");
    }
    if (interp == NULL) {
        return cvo;
    }
    return cvo->obj;
}


void
Opencv_CloseHandle(Tcl_Interp *interp, Opencv_Obj *cvo)
{
    if (cvo->traced) {
        return;
    }
    if (cvo->cmd) {
        Tcl_DeleteCommandFromToken(interp, cvo->cmd);
    } else {
        Opencv_DESTRUCTOR((ClientData)cvo);
    }
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


static int
Opencv_FromByteArray(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    int width, height, bpp, len;
    unsigned char *data;
    Tcl_Obj *pResultStr;
    cv::Mat img, *mat;

    if (objc != 5) {
        Tcl_WrongNumArgs(interp, 1, objv, "width height bpp bytes");
        return TCL_ERROR;
    }

    if (Tcl_GetIntFromObj(interp, objv[1], &width) != TCL_OK) {
        return TCL_ERROR;
    }
    if (Tcl_GetIntFromObj(interp, objv[2], &height) != TCL_OK) {
        return TCL_ERROR;
    }
    if (Tcl_GetIntFromObj(interp, objv[3], &bpp) != TCL_OK) {
        return TCL_ERROR;
    }

    data = Tcl_GetByteArrayFromObj(objv[5], &len);
    if (!(bpp == 1 && len == width * height) &&
        !(bpp == 3 && len == width * height * 3)) {
        return Opencv_SetResult(interp, cv::Error::StsBadArg, "inconsistent pararameters");
    }

    try {
        if (bpp == 1) {
            img = cv::Mat(height, width, CV_8UC1, data, cv::Mat::AUTO_STEP);
        } else {
            img = cv::Mat(height, width, CV_8UC3, data, cv::Mat::AUTO_STEP);
            for (int y = 0; y < height; y++) {
                unsigned char *mdata = img.data + y * width * 3;
                for (int x = 0; x < width; x++) {
                    unsigned char r = mdata[0];
                    unsigned char b = mdata[2];
                    mdata[0] = b;
                    mdata[2] = r;
                    mdata += 3;
                }
            }
        }
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }
    if (img.empty() || !img.data) {
        return Opencv_SetResult(interp, cv::Error::StsError, "no image data");
    }
    mat = new cv::Mat(img);
    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, mat);
    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}


#ifdef TCL_USE_TKPHOTO
int Opencv_CheckForTk(void *cd, Tcl_Interp *interp)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->tkCheck < 0) {
        Tcl_SetResult(interp, (char *) "package Tk not available", TCL_STATIC);
        return TCL_ERROR;
    }
    if (cvd->tkCheck == 0) {
        if (Tcl_PkgPresent(interp, "Tk", "8.6", 0) == NULL) {
            return TCL_ERROR;
        }
#ifdef USE_TK_STUBS
        if (Tk_InitStubs(interp, "8.6", 0) == NULL) {
            cvd->tkCheck = -1;
            return TCL_ERROR;
        }
#else
        if (Tcl_PkgRequire(interp, "Tk", "8.6", 0) == NULL) {
            cvd->tkCheck = -1;
            return TCL_ERROR;
        }
#endif
        cvd->tkCheck = 1;
    }
    if (Tk_MainWindow(interp) == NULL) {
        Tcl_SetResult(interp, (char *) "application has been destroyed", TCL_STATIC);
        return TCL_ERROR;
    }
    return TCL_OK;
}


static int
Opencv_FromPhoto(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    char *name;
    Tk_PhotoHandle photo;
    Tk_PhotoImageBlock blk;
    Tcl_Obj *pResultStr;
    cv::Mat img1, img2, *mat;

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "photo");
        return TCL_ERROR;
    }

    if (Opencv_CheckForTk(cd, interp) != TCL_OK) {
        return TCL_ERROR;
    }
    name = Tcl_GetString(objv[1]);
    photo = Tk_FindPhoto(interp, name);
    if (photo == NULL) {
        Tcl_SetObjResult(interp, Tcl_ObjPrintf("can't use \"%s\": not a photo image", name));
        return TCL_ERROR;
    }
    Tk_PhotoGetImage(photo, &blk);
    try {
        /* Tk always returns RGBA layout */
        img1 = cv::Mat(blk.height, blk.width, CV_8UC4, blk.pixelPtr, cv::Mat::AUTO_STEP);
        /* But OpenCV prefers BGRA layout */
        cv::cvtColor(img1, img2, cv::COLOR_RGBA2BGRA);
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }
    if (img2.empty() || !img2.data) {
        return Opencv_SetResult(interp, cv::Error::StsError, "no image data");
    }
    mat = new cv::Mat(img2);
    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, mat);
    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif /* TCL_USE_TKPHOTO */


#ifdef TCL_USE_VECTCL
int Opencv_CheckForVectcl(void *cd, Tcl_Interp *interp)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;

    if (cvd->vtCheck < 0) {
        Tcl_SetResult(interp, (char *) "package VecTcl not available", TCL_STATIC);
        return TCL_ERROR;
    }
    if (cvd->vtCheck == 0) {
        if (Tcl_PkgPresent(interp, "vectcl", "0.3", 0) == NULL) {
            return TCL_ERROR;
        }
#ifdef USE_VECTCL_STUBS
        if (Vectcl_InitStubs(interp, "0.3", 0) == NULL) {
            cvd->vtCheck = -1;
            return TCL_ERROR;
        }
#else
        if (Tcl_PkgRequire(interp, "vectcl", "0.3", 0) == NULL) {
            cvd->vtCheck = -1;
            return TCL_ERROR;
        }
#endif
        cvd->vtCheck = 1;
    }
    return TCL_OK;
}

static int
Opencv_FromNumArray(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    NumArrayInfo *info;
    Tcl_Obj *pResultStr;
    cv::Mat tmat, *mat;
    int cvtype, dims[2];

    if (objc != 2) {
        Tcl_WrongNumArgs(interp, 1, objv, "numarray");
        return TCL_ERROR;
    }
    if (Opencv_CheckForVectcl(cd, interp) != TCL_OK) {
        return TCL_ERROR;
    }
    info = NumArrayGetInfoFromObj(interp, objv[1]);
    if (info == NULL) {
        return TCL_ERROR;
    }
    switch (info->type) {
        case NumArray_Int8: cvtype = CV_8S; break;
        case NumArray_Uint8: cvtype = CV_8U; break;
        case NumArray_Int16: cvtype = CV_16S; break;
        case NumArray_Uint16: cvtype = CV_16U; break;
        case NumArray_Int32: cvtype = CV_32S; break;
        case NumArray_Uint32: cvtype = CV_32S; break;
        case NumArray_Float32: cvtype = CV_32F; break;
        case NumArray_Float64: cvtype = CV_64F; break;
        case NumArray_Int:
            if (sizeof(NaWideInt) == sizeof(int)) {
                cvtype = CV_32S;
                break;
            }
        default:
            return Opencv_SetResult(interp, cv::Error::StsNoConv, "unsupported data type");
    }
    if (info->nDim < 1 || info->nDim > 3) {
        return Opencv_SetResult(interp, cv::Error::StsNoConv, "invalid number dimensions");
    }
    dims[0] = info->dims[0];
    if (info->nDim > 1) {
        dims[1] = info->dims[1];
    } else {
        dims[1] = 1;
    }
    if (info->nDim > 2) {
        cvtype = CV_MAKE_TYPE(cvtype, info->dims[2]);
    } else {
        cvtype = CV_MAKE_TYPE(cvtype, 1);
    }
    try {
        tmat = cv::Mat(2, dims, cvtype, NumArrayGetPtrFromObj(interp, objv[1]));
    } catch (const cv::Exception &ex) {
        return Opencv_Exc2Tcl(interp, &ex);
    } catch (...) {
        return Opencv_Exc2Tcl(interp, NULL);
    }
    mat = new cv::Mat(tmat);
    pResultStr = Opencv_NewHandle(cd, interp, OPENCV_MAT, mat);
    Tcl_SetObjResult(interp, pResultStr);
    return TCL_OK;
}
#endif /* TCL_USE_VECTCL */


static char *
TraceProc(ClientData clientData, Tcl_Interp *interp, const char *name1, const char *name2, int flags)
{
    Opencv_Obj *cvo = (Opencv_Obj *)clientData;;

    if (flags & TCL_TRACE_UNSETS) {
        if (Tcl_InterpDeleted(interp)) {
            return NULL;
        }
        if (cvo->cmd) {
            Tcl_DeleteCommandFromToken(interp, cvo->cmd);
        } else {
            Opencv_DESTRUCTOR((ClientData)cvo);
        }
        return NULL;
    }
    Tcl_UntraceVar2(interp, name1, NULL, TCL_TRACE_WRITES | TCL_TRACE_UNSETS, TraceProc, cvo);
    cvo->traced = 0;
    if (cvo->cmd) {
        Tcl_DeleteCommandFromToken(interp, cvo->cmd);
    } else {
        Opencv_DESTRUCTOR((ClientData)cvo);
    }
    return NULL;
}


static int
Opencv_Matvar(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv)
{
    Opencv_Data *cvd = (Opencv_Data *)cd;
    Opencv_Obj *cvo;
    Tcl_Obj *value;
    Tcl_HashEntry *hashEntryPtr;
    char *string;
    int type;

    if (objc != 3) {
        Tcl_WrongNumArgs(interp, 1, objv, "varName newValue");
        return TCL_ERROR;
    }
    value = Tcl_ObjSetVar2(interp, objv[1], NULL, objv[2], TCL_LEAVE_ERR_MSG);
    if (value == NULL) {
        return TCL_ERROR;
    }
    string = Tcl_GetString(value);
    for (type = 0; type < OPENCV_MAXTYPE; type++) {
        hashEntryPtr = Tcl_FindHashEntry(&cvd->tbl[type], string);
        if (hashEntryPtr != NULL) {
            break;
        }
    }
    if (hashEntryPtr != NULL) {
        cvo = (Opencv_Obj *) Tcl_GetHashValue(hashEntryPtr);
        if (cvo == NULL) {
            Tcl_Panic("null CV handle");
        }
        if (cvo->obj != NULL && !cvo->traced) {
            string = Tcl_GetString(objv[1]);
            Tcl_UntraceVar2(interp, string, NULL, TCL_TRACE_WRITES | TCL_TRACE_UNSETS, TraceProc, cvo);
            Tcl_TraceVar2(interp, string, NULL, TCL_TRACE_WRITES | TCL_TRACE_UNSETS, TraceProc, cvo);
            cvo->traced = 1;
        }
    }
    Tcl_SetObjResult(interp, value);
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
    Opencv_Data *cvd;
    char nameBuffer[128];
    int nsLength;

#ifdef USE_TCL_STUBS
    if (Tcl_InitStubs(interp, "8.6", 0) == NULL) {
        return TCL_ERROR;
    }
#else
    if (Tcl_PkgRequire(interp, "Tcl", "8.6", 0) == NULL) {
        return TCL_ERROR;
    }
#endif
    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
        return TCL_ERROR;
    }

    nsPtr = Tcl_CreateNamespace(interp, "::" NS, NULL, NULL);
    if (nsPtr == NULL) {
        return TCL_ERROR;
    }
    Tcl_CreateNamespace(interp, "::" NS "::FileStorage", NULL, NULL);
    Tcl_CreateNamespace(interp, "::" NS "::ml", NULL, NULL);
#ifdef TCL_USE_OPENCV4
    Tcl_CreateNamespace(interp, "::" NS "::dnn", NULL, NULL);
#endif

    cvd = (Opencv_Data *) ckalloc(sizeof(Opencv_Data));
    memset((void *) cvd, 0, sizeof(Opencv_Data));
    for (int i = 0; i < OPENCV_MAXTYPE; i++) {
        Tcl_InitHashTable(&cvd->tbl[i], TCL_STRING_KEYS);
    }
#ifdef TCL_USE_TKPHOTO
    cvd->tkCheck = 0;
#endif

    Tcl_CallWhenDeleted(interp, InterpDelProc, (ClientData)cvd);

    strcpy(nameBuffer, "::" NS "::");
    nsLength = strlen(nameBuffer);

    static const struct {
        const char *name;
        Tcl_ObjCmdProc *proc;
    } cvCmds[] = {

        /*
         * For core
         */

        { "CV_8UC",
          (Tcl_ObjCmdProc *) tcl_CV_8UC },
        { "CV_8SC",
          (Tcl_ObjCmdProc *) tcl_CV_8SC },
        { "CV_16UC",
          (Tcl_ObjCmdProc *) tcl_CV_16UC },
        { "CV_16SC",
          (Tcl_ObjCmdProc *) tcl_CV_16SC },
        { "CV_32SC",
          (Tcl_ObjCmdProc *) tcl_CV_32SC },
        { "CV_32FC",
          (Tcl_ObjCmdProc *) tcl_CV_32FC },
        { "CV_64FC",
          (Tcl_ObjCmdProc *) tcl_CV_64FC },
        { "Mat::Mat",
          (Tcl_ObjCmdProc *) mat_mat },
        { "Mat::MatWithDims",
          (Tcl_ObjCmdProc *) mat_matwithdims },
        { "Mat::diag",
          (Tcl_ObjCmdProc *) mat_diag },
        { "Mat::eye",
          (Tcl_ObjCmdProc *) mat_eye },
        { "Mat::ones",
          (Tcl_ObjCmdProc *) mat_ones },
        { "Mat::zeros",
          (Tcl_ObjCmdProc *) mat_zeros },
        { "abs",
          (Tcl_ObjCmdProc *) mat_abs },
        { "absdiff",
          (Tcl_ObjCmdProc *) mat_absdiff },
        { "add",
          (Tcl_ObjCmdProc *) mat_add },
        { "addWeighted",
          (Tcl_ObjCmdProc *) mat_addWeighted },
        { "bitwise_and",
          (Tcl_ObjCmdProc *) mat_bitwise_and },
        { "bitwise_or",
          (Tcl_ObjCmdProc *) mat_bitwise_or },
        { "bitwise_xor",
          (Tcl_ObjCmdProc *) mat_bitwise_xor },
        { "bitwise_not",
          (Tcl_ObjCmdProc *) mat_bitwise_not },
        { "calcCovarMatrix",
          (Tcl_ObjCmdProc *) mat_calcCovarMatrix },
        { "cartToPolar",
          (Tcl_ObjCmdProc *) mat_cartToPolar },
        { "compare",
          (Tcl_ObjCmdProc *) mat_compare },
        { "convertScaleAbs",
          (Tcl_ObjCmdProc *) mat_convertScaleAbs },
        { "copyMakeBorder",
          (Tcl_ObjCmdProc *) mat_copyMakeBorder },
        { "countNonZero",
          (Tcl_ObjCmdProc *) mat_countNonZero },
        { "determinant",
          (Tcl_ObjCmdProc *) mat_determinant },
        { "eigen",
          (Tcl_ObjCmdProc *) mat_eigen },
#ifdef TCL_USE_OPENCV4
        { "eigenNonSymmetric",
          (Tcl_ObjCmdProc *) mat_eigenNonSymmetric },
#endif
        { "exp",
          (Tcl_ObjCmdProc *) mat_exp },
        { "extractChannel",
          (Tcl_ObjCmdProc *) mat_extractChannel },
        { "findNonZero",
          (Tcl_ObjCmdProc *) mat_findNonZero },
        { "flip",
          (Tcl_ObjCmdProc *) mat_flip },
        { "divide",
          (Tcl_ObjCmdProc *) mat_divide },
        { "getOptimalDFTSize",
          (Tcl_ObjCmdProc *) mat_getOptimalDFTSize },
        { "dft",
          (Tcl_ObjCmdProc *) mat_dft },
        { "inRange",
          (Tcl_ObjCmdProc *) mat_inRange },
        { "insertChannel",
          (Tcl_ObjCmdProc *) mat_insertChannel },
        { "log",
          (Tcl_ObjCmdProc *) mat_log },
        { "LUT",
          (Tcl_ObjCmdProc *) mat_lut },
        { "Mahalanobis",
          (Tcl_ObjCmdProc *) mat_Mahalanobis },
        { "magnitude",
          (Tcl_ObjCmdProc *) mat_magnitude },
        { "max",
          (Tcl_ObjCmdProc *) mat_max },
        { "meanStdDev",
          (Tcl_ObjCmdProc *) mat_meanStdDev },
        { "min",
          (Tcl_ObjCmdProc *) mat_min },
        { "minMaxIdx",
          (Tcl_ObjCmdProc *) mat_minMaxIdx },
        { "minMaxLoc",
          (Tcl_ObjCmdProc *) mat_minMaxLoc },
        { "multiply",
          (Tcl_ObjCmdProc *) mat_multiply },
        { "mulSpectrums",
          (Tcl_ObjCmdProc *) mat_mulSpectrums },
        { "split",
          (Tcl_ObjCmdProc *) mat_split },
        { "merge",
          (Tcl_ObjCmdProc *) mat_merge },
        { "norm",
          (Tcl_ObjCmdProc *) mat_norm },
        { "normalize",
          (Tcl_ObjCmdProc *) mat_normalize },
        { "pow",
          (Tcl_ObjCmdProc *) mat_pow },
        { "randn",
          (Tcl_ObjCmdProc *) mat_randn },
        { "randu",
          (Tcl_ObjCmdProc *) mat_randu },
        { "randShuffle",
          (Tcl_ObjCmdProc *) mat_randShuffle },
        { "reduce",
          (Tcl_ObjCmdProc *) mat_reduce },
        { "repeat",
          (Tcl_ObjCmdProc *) mat_repeat },
        { "rotate",
          (Tcl_ObjCmdProc *) mat_rotate },
        { "setRNGSeed",
          (Tcl_ObjCmdProc *) mat_setRNGSeed },
        { "solve",
          (Tcl_ObjCmdProc *) mat_solve },
        { "solveCubic",
          (Tcl_ObjCmdProc *) mat_solveCubic },
        { "solvePoly",
          (Tcl_ObjCmdProc *) mat_solvePoly },
        { "sortIdx",
          (Tcl_ObjCmdProc *) mat_sortIdx },
        { "sqrt",
          (Tcl_ObjCmdProc *) mat_sqrt },
        { "subtract",
          (Tcl_ObjCmdProc *) mat_subtract },
        { "sum",
          (Tcl_ObjCmdProc *) mat_sum },
        { "SVBackSubst",
          (Tcl_ObjCmdProc *) mat_SVBackSubst },
        { "SVDecomp",
          (Tcl_ObjCmdProc *) mat_SVDecomp },
        { "trace",
          (Tcl_ObjCmdProc *) mat_trace },
        { "transform",
          (Tcl_ObjCmdProc *) mat_transform },
        { "hconcat",
          (Tcl_ObjCmdProc *) mat_hconcat },
        { "vconcat",
          (Tcl_ObjCmdProc *) mat_vconcat },
        { "kmeans",
          (Tcl_ObjCmdProc *) kmeans },
        { "perspectiveTransform",
          (Tcl_ObjCmdProc *) perspectiveTransform },
        { "getBuildInformation",
          (Tcl_ObjCmdProc *) getBuildInformation },
        { "getTickCount",
          (Tcl_ObjCmdProc *) getTickCount },
        { "getTickFrequency",
          (Tcl_ObjCmdProc *) getTickFrequency },
        { "PCA",
          (Tcl_ObjCmdProc *) PCA },
        { "TermCriteria",
          (Tcl_ObjCmdProc *) TermCriteria },
        { "fromByteArray",
          (Tcl_ObjCmdProc *) Opencv_FromByteArray },
        { "matvar",
          (Tcl_ObjCmdProc *) Opencv_Matvar },
#ifdef TCL_USE_TKPHOTO
        { "fromPhoto",
          (Tcl_ObjCmdProc *) Opencv_FromPhoto },
#endif
#ifdef TCL_USE_VECTCL
        { "fromNumArray",
          (Tcl_ObjCmdProc *) Opencv_FromNumArray },
#endif

        /*
         * For imgcodecs
         */

        { "imread",
          (Tcl_ObjCmdProc *) imread },
        { "imdecode",
          (Tcl_ObjCmdProc *) imdecode },
        { "imwrite",
          (Tcl_ObjCmdProc *) imwrite },
        { "imencode",
          (Tcl_ObjCmdProc *) imencode },

        /*
         * For fstorage
         */

        { "FileStorage",
          (Tcl_ObjCmdProc *) FileStorage },

        /*
         * For imgproc
         */

        { "applyColorMap",
          (Tcl_ObjCmdProc *) applyColorMap },
        { "cvtColor",
          (Tcl_ObjCmdProc *) cvtColor },
        { "calcBackProject",
          (Tcl_ObjCmdProc *) calcBackProject },
        { "calcHist",
          (Tcl_ObjCmdProc *) calcHist },
        { "compareHist",
          (Tcl_ObjCmdProc *) compareHist },
        { "equalizeHist",
          (Tcl_ObjCmdProc *) equalizeHist },
        { "EMD",
          (Tcl_ObjCmdProc *) EMD },
        { "floodFill",
          (Tcl_ObjCmdProc *) floodFill },
        { "grabCut",
          (Tcl_ObjCmdProc *) grabCut },
        { "matchTemplate",
          (Tcl_ObjCmdProc *) matchTemplate },
        { "moments",
          (Tcl_ObjCmdProc *) moments },
        { "getRotationMatrix2D",
          (Tcl_ObjCmdProc *) getRotationMatrix2D },
        { "getRectSubPix",
          (Tcl_ObjCmdProc *) getRectSubPix },
        { "HuMoments",
          (Tcl_ObjCmdProc *) HuMoments },
        { "integral",
          (Tcl_ObjCmdProc *) integral },
        { "remap",
          (Tcl_ObjCmdProc *) remap },
        { "resize",
          (Tcl_ObjCmdProc *) resize },
        { "threshold",
          (Tcl_ObjCmdProc *) threshold },
        { "adaptiveThreshold",
          (Tcl_ObjCmdProc *) adaptiveThreshold },
        { "getAffineTransform",
          (Tcl_ObjCmdProc *) getAffineTransform },
        { "warpAffine",
          (Tcl_ObjCmdProc *) warpAffine },
        { "getPerspectiveTransform",
          (Tcl_ObjCmdProc *) getPerspectiveTransform },
        { "warpPerspective",
          (Tcl_ObjCmdProc *) warpPerspective },
#ifdef TCL_USE_OPENCV4
        { "warpPolar",
          (Tcl_ObjCmdProc *) warpPolar },
#endif
        { "filter2D",
          (Tcl_ObjCmdProc *) filter2D },
        { "sepFilter2D",
          (Tcl_ObjCmdProc *) sepFilter2D },
        { "getDerivKernels",
          (Tcl_ObjCmdProc *) getDerivKernels },
        { "getGaborKernel",
          (Tcl_ObjCmdProc *) getGaborKernel },
        { "getGaussianKernel",
          (Tcl_ObjCmdProc *) getGaussianKernel },
        { "blur",
          (Tcl_ObjCmdProc *) blur },
        { "GaussianBlur",
          (Tcl_ObjCmdProc *) GaussianBlur },
        { "medianBlur",
          (Tcl_ObjCmdProc *) medianBlur },
        { "bilateralFilter",
          (Tcl_ObjCmdProc *) bilateralFilter },
        { "boxFilter",
          (Tcl_ObjCmdProc *) boxFilter },
        { "sqrBoxFilter",
          (Tcl_ObjCmdProc *) sqrBoxFilter },
        { "getStructuringElement",
          (Tcl_ObjCmdProc *) getStructuringElement },
        { "dilate",
          (Tcl_ObjCmdProc *) dilate },
        { "erode",
          (Tcl_ObjCmdProc *) erode },
        { "morphologyEx",
          (Tcl_ObjCmdProc *) morphologyEx },
        { "buildPyramid",
          (Tcl_ObjCmdProc *) buildPyramid },
        { "pyrUp",
          (Tcl_ObjCmdProc *) pyrUp },
        { "pyrDown",
          (Tcl_ObjCmdProc *) pyrDown },
        { "pyrMeanShiftFiltering",
          (Tcl_ObjCmdProc *) pyrMeanShiftFiltering },
        { "createHanningWindow",
          (Tcl_ObjCmdProc *) createHanningWindow },
        { "phaseCorrelate",
          (Tcl_ObjCmdProc *) phaseCorrelate },
        { "Canny",
          (Tcl_ObjCmdProc *) Canny },
        { "Sobel",
          (Tcl_ObjCmdProc *) Sobel },
        { "Scharr",
          (Tcl_ObjCmdProc *) Scharr },
        { "Laplacian",
          (Tcl_ObjCmdProc *) Laplacian },
        { "distanceTransform",
          (Tcl_ObjCmdProc *) distanceTransform },
        { "connectedComponents",
          (Tcl_ObjCmdProc *) connectedComponents },
        { "connectedComponentsWithStats",
          (Tcl_ObjCmdProc *) connectedComponentsWithStats },
        { "watershed",
          (Tcl_ObjCmdProc *) watershed },
        { "goodFeaturesToTrack",
          (Tcl_ObjCmdProc *) goodFeaturesToTrack },
        { "cornerHarris",
          (Tcl_ObjCmdProc *) cornerHarris },
        { "cornerEigenValsAndVecs",
          (Tcl_ObjCmdProc *) cornerEigenValsAndVecs },
        { "cornerMinEigenVal",
          (Tcl_ObjCmdProc *) cornerMinEigenVal },
        { "cornerSubPix",
          (Tcl_ObjCmdProc *) cornerSubPix },
        { "HoughCircles",
          (Tcl_ObjCmdProc *) HoughCircles },
        { "HoughLines",
          (Tcl_ObjCmdProc *) HoughLines },
        { "HoughLinesP",
          (Tcl_ObjCmdProc *) HoughLinesP },
        { "findContours",
          (Tcl_ObjCmdProc *) findContours },
        { "findContoursWithHierarchy",
          (Tcl_ObjCmdProc *) findContoursWithHierarchy },
        { "drawContours",
          (Tcl_ObjCmdProc *) drawContours },
        { "drawContoursWithHierarchy",
          (Tcl_ObjCmdProc *) drawContoursWithHierarchy },
        { "approxPolyDP",
          (Tcl_ObjCmdProc *) approxPolyDP },
        { "arcLength",
          (Tcl_ObjCmdProc *) arcLength },
        { "contourArea",
          (Tcl_ObjCmdProc *) contourArea },
        { "boundingRect",
          (Tcl_ObjCmdProc *) boundingRect },
        { "minAreaRect",
          (Tcl_ObjCmdProc *) minAreaRect },
        { "fitEllipse",
          (Tcl_ObjCmdProc *) fitEllipse },
        { "fitLine",
          (Tcl_ObjCmdProc *) fitLine },
        { "boxPoints",
          (Tcl_ObjCmdProc *) boxPoints },
        { "minEnclosingCircle",
          (Tcl_ObjCmdProc *) minEnclosingCircle },
        { "convexHull",
          (Tcl_ObjCmdProc *) convexHull },
        { "convexityDefects",
          (Tcl_ObjCmdProc *) convexityDefects },
        { "matchShapes",
          (Tcl_ObjCmdProc *) matchShapes },
        { "pointPolygonTest",
          (Tcl_ObjCmdProc *) pointPolygonTest },
        { "arrowedLine",
          (Tcl_ObjCmdProc *) arrowedLine },
        { "circle",
          (Tcl_ObjCmdProc *) circle },
        { "clipLine",
          (Tcl_ObjCmdProc *) clipLine },
        { "drawMarker",
          (Tcl_ObjCmdProc *) drawMarker },
        { "ellipse",
          (Tcl_ObjCmdProc *) ellipse },
        { "fillConvexPoly",
          (Tcl_ObjCmdProc *) fillConvexPoly },
        { "fillPoly",
          (Tcl_ObjCmdProc *) fillPoly },
        { "getFontScaleFromHeight",
          (Tcl_ObjCmdProc *) getFontScaleFromHeight },
        { "getTextSize",
          (Tcl_ObjCmdProc *) getTextSize },
        { "line",
          (Tcl_ObjCmdProc *) line },
        { "polylines",
          (Tcl_ObjCmdProc *) polylines },
        { "putText",
          (Tcl_ObjCmdProc *) putText },
        { "rectangle",
          (Tcl_ObjCmdProc *) rectangle },
        { "CLAHE",
          (Tcl_ObjCmdProc *) CLAHE },
        { "GeneralizedHoughBallard",
          (Tcl_ObjCmdProc *) GeneralizedHoughBallard },
        { "GeneralizedHoughGuil",
          (Tcl_ObjCmdProc *) GeneralizedHoughGuil },
        { "LineSegmentDetector",
          (Tcl_ObjCmdProc *) LineSegmentDetector },

        /*
         * For videoio
         */

        { "VideoCapture",
          (Tcl_ObjCmdProc *) VideoCapture },
        { "VideoWriter",
          (Tcl_ObjCmdProc *) VideoWriter },

        /*
         * For highgui
         */

        { "namedWindow",
          (Tcl_ObjCmdProc *) namedWindow },
        { "imshow",
          (Tcl_ObjCmdProc *) imshow },
        { "waitKey",
          (Tcl_ObjCmdProc *) waitKey },
        { "moveWindow",
          (Tcl_ObjCmdProc *) moveWindow },
        { "resizeWindow",
          (Tcl_ObjCmdProc *) resizeWindow },
        { "destroyWindow",
          (Tcl_ObjCmdProc *) destroyWindow },
        { "destroyAllWindows",
          (Tcl_ObjCmdProc *) destroyAllWindows },
#ifdef TCL_USE_OPENCV4
        { "selectROI",
          (Tcl_ObjCmdProc *) selectROI },
#endif
        { "setMouseCallback",
          (Tcl_ObjCmdProc *) setMouseCallback },
        { "createTrackbar",
          (Tcl_ObjCmdProc *) createTrackbar },
        { "getTrackbarPos",
          (Tcl_ObjCmdProc *) getTrackbarPos },

        /*
         * For features2d
         */

        { "drawKeypoints",
          (Tcl_ObjCmdProc *) drawKeypoints },
        { "drawMatches",
          (Tcl_ObjCmdProc *) drawMatches },
        { "FastFeatureDetector",
          (Tcl_ObjCmdProc *) FastFeatureDetector },
        { "AgastFeatureDetector",
          (Tcl_ObjCmdProc *) AgastFeatureDetector },
        { "MSER",
          (Tcl_ObjCmdProc *) MSER },
        { "ORB",
          (Tcl_ObjCmdProc *) ORB },
        { "AKAZE",
          (Tcl_ObjCmdProc *) AKAZE },
        { "BRISK",
          (Tcl_ObjCmdProc *) BRISK },
        { "KAZE",
          (Tcl_ObjCmdProc *) KAZE },
#ifdef TCL_USE_SIFT
        { "SIFT",
          (Tcl_ObjCmdProc *) SIFT },
        { "AffineFeature",
          (Tcl_ObjCmdProc *) AffineFeature },
#endif
        { "BFMatcher",
          (Tcl_ObjCmdProc *) BFMatcher },
        { "FlannBasedMatcher",
          (Tcl_ObjCmdProc *) FlannBasedMatcher },
        { "SimpleBlobDetector",
          (Tcl_ObjCmdProc *) SimpleBlobDetector },
        { "BOWKMeansTrainer",
          (Tcl_ObjCmdProc *) BOWKMeansTrainer },
        { "BOWImgDescriptorExtractor",
          (Tcl_ObjCmdProc *) BOWImgDescriptorExtractor },

        /*
         * For calib3d
         */

        { "findChessboardCorners",
          (Tcl_ObjCmdProc *) findChessboardCorners },
        { "drawChessboardCorners",
          (Tcl_ObjCmdProc *) drawChessboardCorners },
        { "calibrateCamera",
          (Tcl_ObjCmdProc *) calibrateCamera },
        { "getOptimalNewCameraMatrix",
          (Tcl_ObjCmdProc *) getOptimalNewCameraMatrix },
        { "undistort",
          (Tcl_ObjCmdProc *) undistort },
        { "initUndistortRectifyMap",
          (Tcl_ObjCmdProc *) initUndistortRectifyMap },
        { "projectPoints",
          (Tcl_ObjCmdProc *) projectPoints },
        { "solvePnP",
          (Tcl_ObjCmdProc *) solvePnP },
        { "computeCorrespondEpilines",
          (Tcl_ObjCmdProc *) computeCorrespondEpilines },
        { "estimateAffine2D",
          (Tcl_ObjCmdProc *) estimateAffine2D },
        { "estimateAffinePartial2D",
          (Tcl_ObjCmdProc *) estimateAffinePartial2D },
        { "estimateAffine3D",
          (Tcl_ObjCmdProc *) estimateAffine3D },
        { "findFundamentalMat",
          (Tcl_ObjCmdProc *) findFundamentalMat },
        { "findHomography",
          (Tcl_ObjCmdProc *) findHomography },
        { "StereoBM",
          (Tcl_ObjCmdProc *) StereoBM },
        { "StereoSGBM",
          (Tcl_ObjCmdProc *) StereoSGBM },

        /*
         * For Video
         */

        { "BackgroundSubtractorKNN",
          (Tcl_ObjCmdProc *) BackgroundSubtractorKNN },
        { "BackgroundSubtractorMOG2",
          (Tcl_ObjCmdProc *) BackgroundSubtractorMOG2 },
#ifdef TCL_USE_OPENCV4
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 1)
        { "TrackerMIL",
          (Tcl_ObjCmdProc *) TrackerMIL },
        { "TrackerGOTURN",
          (Tcl_ObjCmdProc *) TrackerGOTURN },
#endif
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 3)
        { "TrackerDaSiamRPN",
          (Tcl_ObjCmdProc *) TrackerDaSiamRPN },
#endif
#endif
        { "meanShift",
          (Tcl_ObjCmdProc *) meanShift },
        { "CamShift",
          (Tcl_ObjCmdProc *) CamShift },
        { "calcOpticalFlowPyrLK",
          (Tcl_ObjCmdProc *) calcOpticalFlowPyrLK },
        { "calcOpticalFlowFarneback",
          (Tcl_ObjCmdProc *) calcOpticalFlowFarneback },
        { "readOpticalFlow",
          (Tcl_ObjCmdProc *) readOpticalFlow },
        { "writeOpticalFlow",
          (Tcl_ObjCmdProc *) writeOpticalFlow },
#ifdef TCL_USE_OPENCV4
        { "computeECC",
          (Tcl_ObjCmdProc *) computeECC },
#endif
        { "findTransformECC",
          (Tcl_ObjCmdProc *) findTransformECC },

        /*
         * For Photo
         */

        { "inpaint",
          (Tcl_ObjCmdProc *) inpaint },
        { "decolor",
          (Tcl_ObjCmdProc *) decolor },
        { "fastNlMeansDenoising",
          (Tcl_ObjCmdProc *) fastNlMeansDenoising },
        { "fastNlMeansDenoisingColored",
          (Tcl_ObjCmdProc *) fastNlMeansDenoisingColored },
        { "colorChange",
          (Tcl_ObjCmdProc *) colorChange },
        { "illuminationChange",
          (Tcl_ObjCmdProc *) illuminationChange },
        { "textureFlattening",
          (Tcl_ObjCmdProc *) textureFlattening },
        { "seamlessClone",
          (Tcl_ObjCmdProc *) seamlessClone },
        { "detailEnhance",
          (Tcl_ObjCmdProc *) detailEnhance },
        { "edgePreservingFilter",
          (Tcl_ObjCmdProc *) edgePreservingFilter },
        { "pencilSketch",
          (Tcl_ObjCmdProc *) pencilSketch },
        { "stylization",
          (Tcl_ObjCmdProc *) stylization },
        { "AlignMTB",
          (Tcl_ObjCmdProc *) AlignMTB },
        { "CalibrateDebevec",
          (Tcl_ObjCmdProc *) CalibrateDebevec },
        { "MergeDebevec",
          (Tcl_ObjCmdProc *) MergeDebevec },
        { "MergeMertens",
          (Tcl_ObjCmdProc *) MergeMertens },
        { "TonemapDrago",
          (Tcl_ObjCmdProc *) TonemapDrago },
        { "TonemapMantiuk",
          (Tcl_ObjCmdProc *) TonemapMantiuk },
        { "TonemapReinhard",
          (Tcl_ObjCmdProc *) TonemapReinhard },

        /*
         * For Stitcher
         */

        { "Stitcher",
          (Tcl_ObjCmdProc *) Stitcher },

        /*
         * For objdetect
         */

        { "CascadeClassifier",
          (Tcl_ObjCmdProc *) CascadeClassifier },
        { "HOGDescriptor",
          (Tcl_ObjCmdProc *) HOGDescriptor },
#ifdef TCL_USE_OPENCV4
        { "QRCodeDetector",
          (Tcl_ObjCmdProc *) QRCodeDetector },
#endif

        /*
         * For ml
         */

        { "ml::LogisticRegression",
          (Tcl_ObjCmdProc *) LogisticRegression },
#ifdef TCL_USE_OPENCV4
        { "ml::LogisticRegression::load",
          (Tcl_ObjCmdProc *) LogisticRegression_load },
#endif
        { "ml::NormalBayesClassifier",
          (Tcl_ObjCmdProc *) NormalBayesClassifier },
#ifdef TCL_USE_OPENCV4
        { "ml::NormalBayesClassifier::load",
          (Tcl_ObjCmdProc *) NormalBayesClassifier_load },
#endif
        { "ml::KNearest",
          (Tcl_ObjCmdProc *) KNearest },
#ifdef TCL_USE_OPENCV4
        { "ml::KNearest::load",
          (Tcl_ObjCmdProc *) KNearest_load },
#endif
        { "ml::SVM",
          (Tcl_ObjCmdProc *) SVM },
        { "ml::SVM::load",
          (Tcl_ObjCmdProc *) SVM_load },
        { "ml::SVMSGD",
          (Tcl_ObjCmdProc *) SVMSGD },
#ifdef TCL_USE_OPENCV4
        { "ml::SVMSGD::load",
          (Tcl_ObjCmdProc *) SVMSGD_load },
#endif
        { "ml::DTrees",
          (Tcl_ObjCmdProc *) DTrees },
#ifdef TCL_USE_OPENCV4
        { "ml::DTrees::load",
          (Tcl_ObjCmdProc *) DTrees_load },
#endif
        { "ml::Boost",
          (Tcl_ObjCmdProc *) Boost },
#ifdef TCL_USE_OPENCV4
        { "ml::Boost::load",
          (Tcl_ObjCmdProc *) Boost_load },
#endif
        { "ml::RTrees",
          (Tcl_ObjCmdProc *) RTrees },
#ifdef TCL_USE_OPENCV4
        { "ml::RTrees::load",
          (Tcl_ObjCmdProc *) RTrees_load },
#endif
        { "ml::ANN_MLP",
          (Tcl_ObjCmdProc *) ANN_MLP },
#ifdef TCL_USE_OPENCV4
        { "ml::ANN_MLP::load",
          (Tcl_ObjCmdProc *) ANN_MLP_load },
#endif
        { "ml::TrainData",
          (Tcl_ObjCmdProc *) TrainData },

        /*
         * For dnn
         */

#ifdef TCL_USE_OPENCV4
        { "dnn::blobFromImage",
          (Tcl_ObjCmdProc *) dnn_blobFromImage },
        { "dnn::readNet",
          (Tcl_ObjCmdProc *) dnn_readNet },
        { "dnn::NMSBoxes",
          (Tcl_ObjCmdProc *) dnn_NMSBoxes },
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 1)
        { "dnn::TextDetectionModel_EAST",
          (Tcl_ObjCmdProc *) TextDetectionModel_EAST },
        { "dnn::TextDetectionModel_DB",
          (Tcl_ObjCmdProc *) TextDetectionModel_DB },
        { "dnn::TextRecognitionModel",
          (Tcl_ObjCmdProc *) TextRecognitionModel },
#endif
#endif

        /*
         * Thread support
         */

        { "thread::send",
          (Tcl_ObjCmdProc *) Opencv_Tsend },
        { "thread::recv",
          (Tcl_ObjCmdProc *) Opencv_Trecv },
        { "thread::info",
          (Tcl_ObjCmdProc *) Opencv_Tinfo },

        /*
         * Helper
         */

        { "_info",
          (Tcl_ObjCmdProc *) Opencv_Info },

    };

    for (size_t i = 0; i < sizeof(cvCmds) / sizeof(cvCmds[0]); i++) {
        strcpy(nameBuffer + nsLength, cvCmds[i].name);
        Tcl_CreateObjCommand(interp, nameBuffer, cvCmds[i].proc,
                             (ClientData)cvd, (Tcl_CmdDeleteProc *)NULL);
    }

    /*
     * Integer constants
     */

    static const struct {
        const char *name;
        int value;
    } cvConsts[] = {

        /*
         * Type
         */

        { "CV_8U", CV_8U },
        { "CV_8S", CV_8S },
        { "CV_16U", CV_16U },
        { "CV_16S", CV_16S },
        { "CV_32S", CV_32S },
        { "CV_32F", CV_32F },
        { "CV_64F", CV_64F },
        { "CV_8UC1", CV_8UC1 },
        { "CV_8UC2", CV_8UC2 },
        { "CV_8UC3", CV_8UC3 },
        { "CV_8UC4", CV_8UC4 },
        { "CV_8SC1", CV_8SC1 },
        { "CV_8SC2", CV_8SC2 },
        { "CV_8SC3", CV_8SC3 },
        { "CV_8SC4", CV_8SC4 },
        { "CV_16UC1", CV_16UC1 },
        { "CV_16UC2", CV_16UC2 },
        { "CV_16UC3", CV_16UC3 },
        { "CV_16UC4", CV_16UC4 },
        { "CV_16SC1", CV_16SC1 },
        { "CV_16SC2", CV_16SC2 },
        { "CV_16SC3", CV_16SC3 },
        { "CV_16SC4", CV_16SC4 },
        { "CV_32SC1", CV_32SC1 },
        { "CV_32SC2", CV_32SC2 },
        { "CV_32SC3", CV_32SC3 },
        { "CV_32SC4", CV_32SC4 },
        { "CV_32FC1", CV_32FC1 },
        { "CV_32FC2", CV_32FC2 },
        { "CV_32FC3", CV_32FC3 },
        { "CV_32FC4", CV_32FC4 },
        { "CV_64FC1", CV_64FC1 },
        { "CV_64FC2", CV_64FC2 },
        { "CV_64FC3", CV_64FC3 },
        { "CV_64FC4", CV_64FC4 },

        /*
         * fstorage flags
         */

        { "FileStorage::READ", cv::FileStorage::READ },
        { "FileStorage::WRITE", cv::FileStorage::WRITE },
        { "FileStorage::APPEND", cv::FileStorage::APPEND },
        { "FileStorage::MEMORY", cv::FileStorage::MEMORY },
        { "FileStorage::FORMAT_AUTO", cv::FileStorage::FORMAT_AUTO },
        { "FileStorage::FORMAT_XML", cv::FileStorage::FORMAT_XML },
        { "FileStorage::FORMAT_YAML", cv::FileStorage::FORMAT_YAML },
        { "FileStorage::FORMAT_JSON", cv::FileStorage::FORMAT_JSON },
        { "FileStorage::BASE64", cv::FileStorage::BASE64 },
        { "FileStorage::WRITE_BASE64", cv::FileStorage::WRITE_BASE64 },

        /*
         * sortIdx sort flags
         */

        { "SORT_EVERY_ROW", cv::SortFlags::SORT_EVERY_ROW },
        { "SORT_EVERY_COLUMN", cv::SortFlags::SORT_EVERY_COLUMN },
        { "SORT_ASCENDING", cv::SortFlags::SORT_ASCENDING },
        { "SORT_DESCENDING", cv::SortFlags::SORT_DESCENDING },

        /*
         * imread flags
         */

        { "IMREAD_UNCHANGED", cv::IMREAD_UNCHANGED },
        { "IMREAD_GRAYSCALE", cv::IMREAD_GRAYSCALE },
        { "IMREAD_COLOR", cv::IMREAD_COLOR },
        { "IMREAD_ANYDEPTH", cv::IMREAD_ANYDEPTH },
        { "IMREAD_ANYCOLOR", cv::IMREAD_ANYCOLOR },
        { "IMREAD_LOAD_GDAL", cv::IMREAD_LOAD_GDAL },
        { "IMREAD_REDUCED_GRAYSCALE_2", cv::IMREAD_REDUCED_GRAYSCALE_2 },
        { "IMREAD_REDUCED_COLOR_2", cv::IMREAD_REDUCED_COLOR_2 },
        { "IMREAD_REDUCED_GRAYSCALE_4", cv::IMREAD_REDUCED_GRAYSCALE_4 },
        { "IMREAD_REDUCED_COLOR_4", cv::IMREAD_REDUCED_COLOR_4 },
        { "IMREAD_REDUCED_GRAYSCALE_8", cv::IMREAD_REDUCED_GRAYSCALE_8 },
        { "IMREAD_REDUCED_COLOR_8", cv::IMREAD_REDUCED_COLOR_8 },
        { "IMREAD_IGNORE_ORIENTATION", cv::IMREAD_IGNORE_ORIENTATION },

        /*
         * VideoCapture flags
         */

        { "CAP_ANY", cv::CAP_ANY },
        { "CAP_VFW", cv::CAP_VFW },
        { "CAP_V4L", cv::CAP_V4L },
        { "CAP_V4L2", cv::CAP_V4L2 },
        { "CAP_FIREWIRE", cv::CAP_FIREWIRE },
        { "CAP_FIREWARE", cv::CAP_FIREWARE },
        { "CAP_IEEE1394", cv::CAP_IEEE1394 },
        { "CAP_DC1394", cv::CAP_DC1394 },
        { "CAP_CMU1394", cv::CAP_CMU1394 },
        { "CAP_QT", cv::CAP_QT },
        { "CAP_UNICAP", cv::CAP_UNICAP },
        { "CAP_DSHOW", cv::CAP_DSHOW },
        { "CAP_PVAPI", cv::CAP_PVAPI },
        { "CAP_OPENNI", cv::CAP_OPENNI },
        { "CAP_OPENNI_ASUS", cv::CAP_OPENNI_ASUS },
        { "CAP_ANDROID", cv::CAP_ANDROID },
        { "CAP_XIAPI", cv::CAP_XIAPI },
        { "CAP_AVFOUNDATION", cv::CAP_AVFOUNDATION },
        { "CAP_GIGANETIX", cv::CAP_GIGANETIX },
        { "CAP_MSMF", cv::CAP_MSMF },
#ifdef TCL_USE_OPENCV4
        { "CAP_WINRT", cv::CAP_WINRT },
#endif
        { "CAP_INTELPERC", cv::CAP_INTELPERC },
#ifdef TCL_USE_OPENCV4
        { "CAP_REALSENSE", cv::CAP_REALSENSE },
#endif
        { "CAP_OPENNI2", cv::CAP_OPENNI2 },
        { "CAP_OPENNI2_ASUS", cv::CAP_OPENNI2_ASUS },
#ifdef TCL_USE_OPENCV4
        { "CAP_OPENNI2_ASTRA", cv::CAP_OPENNI2_ASTRA },
#endif
        { "CAP_GPHOTO2", cv::CAP_GPHOTO2 },
        { "CAP_GSTREAMER", cv::CAP_GSTREAMER },
        { "CAP_FFMPEG", cv::CAP_FFMPEG },
        { "CAP_IMAGES", cv::CAP_IMAGES },
        { "CAP_ARAVIS", cv::CAP_ARAVIS },
#ifdef TCL_USE_OPENCV4
        { "CAP_OPENCV_MJPEG", cv::CAP_OPENCV_MJPEG },
        { "CAP_INTEL_MFX", cv::CAP_INTEL_MFX },
        { "CAP_XINE", cv::CAP_XINE },
        { "CAP_UEYE", cv::CAP_UEYE },
#endif

        /*
         * VideoCapture property
         */

        { "CAP_PROP_POS_MSEC", cv::CAP_PROP_POS_MSEC },
        { "CAP_PROP_POS_FRAMES", cv::CAP_PROP_POS_FRAMES },
        { "CAP_PROP_POS_AVI_RATIO", cv::CAP_PROP_POS_AVI_RATIO },
        { "CAP_PROP_FRAME_WIDTH", cv::CAP_PROP_FRAME_WIDTH },
        { "CAP_PROP_FRAME_HEIGHT", cv::CAP_PROP_FRAME_HEIGHT },
        { "CAP_PROP_FPS", cv::CAP_PROP_FPS },
        { "CAP_PROP_FOURCC", cv::CAP_PROP_FOURCC },
        { "CAP_PROP_FRAME_COUNT", cv::CAP_PROP_FRAME_COUNT },
        { "CAP_PROP_FORMAT", cv::CAP_PROP_FORMAT },
        { "CAP_PROP_MODE", cv::CAP_PROP_MODE },
        { "CAP_PROP_BRIGHTNESS", cv::CAP_PROP_BRIGHTNESS },
        { "CAP_PROP_CONTRAST", cv::CAP_PROP_CONTRAST },
        { "CAP_PROP_SATURATION", cv::CAP_PROP_SATURATION },
        { "CAP_PROP_HUE", cv::CAP_PROP_HUE },
        { "CAP_PROP_GAIN", cv::CAP_PROP_GAIN },
        { "CAP_PROP_EXPOSURE", cv::CAP_PROP_EXPOSURE },
        { "CAP_PROP_CONVERT_RGB", cv::CAP_PROP_CONVERT_RGB },
        { "CAP_PROP_WHITE_BALANCE_BLUE_U", cv::CAP_PROP_WHITE_BALANCE_BLUE_U },
        { "CAP_PROP_RECTIFICATION", cv::CAP_PROP_RECTIFICATION },
        { "CAP_PROP_MONOCHROME", cv::CAP_PROP_MONOCHROME },
        { "CAP_PROP_SHARPNESS", cv::CAP_PROP_SHARPNESS },
        { "CAP_PROP_AUTO_EXPOSURE", cv::CAP_PROP_AUTO_EXPOSURE },
        { "CAP_PROP_GAMMA", cv::CAP_PROP_GAMMA },
        { "CAP_PROP_TEMPERATURE", cv::CAP_PROP_TEMPERATURE },
        { "CAP_PROP_TRIGGER", cv::CAP_PROP_TRIGGER },
        { "CAP_PROP_TRIGGER_DELAY", cv::CAP_PROP_TRIGGER_DELAY },
        { "CAP_PROP_WHITE_BALANCE_RED_V", cv::CAP_PROP_WHITE_BALANCE_RED_V },
        { "CAP_PROP_ZOOM", cv::CAP_PROP_ZOOM },
        { "CAP_PROP_FOCUS", cv::CAP_PROP_FOCUS },
        { "CAP_PROP_GUID", cv::CAP_PROP_GUID },
        { "CAP_PROP_ISO_SPEED", cv::CAP_PROP_ISO_SPEED },
        { "CAP_PROP_BACKLIGHT", cv::CAP_PROP_BACKLIGHT },
        { "CAP_PROP_PAN", cv::CAP_PROP_PAN },
        { "CAP_PROP_TILT", cv::CAP_PROP_TILT },
        { "CAP_PROP_ROLL", cv::CAP_PROP_ROLL },
        { "CAP_PROP_IRIS", cv::CAP_PROP_IRIS },
        { "CAP_PROP_SETTINGS", cv::CAP_PROP_SETTINGS },
        { "CAP_PROP_BUFFERSIZE", cv::CAP_PROP_BUFFERSIZE },
        { "CAP_PROP_AUTOFOCUS", cv::CAP_PROP_AUTOFOCUS },
#ifdef TCL_USE_OPENCV4
        { "CAP_PROP_SAR_NUM", cv::CAP_PROP_SAR_NUM },
        { "CAP_PROP_SAR_DEN", cv::CAP_PROP_SAR_DEN },
        { "CAP_PROP_BACKEND", cv::CAP_PROP_BACKEND },
        { "CAP_PROP_CHANNEL", cv::CAP_PROP_CHANNEL },
        { "CAP_PROP_AUTO_WB", cv::CAP_PROP_AUTO_WB },
        { "CAP_PROP_WB_TEMPERATURE", cv::CAP_PROP_WB_TEMPERATURE },
        { "CAP_PROP_CODEC_PIXEL_FORMAT", cv::CAP_PROP_CODEC_PIXEL_FORMAT },
        { "CAP_PROP_BITRATE", cv::CAP_PROP_BITRATE },
        { "CAP_PROP_ORIENTATION_META", cv::CAP_PROP_ORIENTATION_META },
        { "CAP_PROP_ORIENTATION_AUTO", cv::CAP_PROP_ORIENTATION_AUTO },
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 2)
        { "CAP_PROP_HW_ACCELERATION", cv::CAP_PROP_HW_ACCELERATION },
        { "CAP_PROP_HW_DEVICE", cv::CAP_PROP_HW_DEVICE },
#endif
#endif

        /*
         * VideoWriter property
         */

        { "VIDEOWRITER_PROP_QUALITY", cv::VIDEOWRITER_PROP_QUALITY },
        { "VIDEOWRITER_PROP_FRAMEBYTES", cv::VIDEOWRITER_PROP_FRAMEBYTES },
        { "VIDEOWRITER_PROP_NSTRIPES", cv::VIDEOWRITER_PROP_NSTRIPES },
#ifdef TCL_USE_OPENCV4
        { "VIDEOWRITER_PROP_IS_COLOR", cv::VIDEOWRITER_PROP_IS_COLOR },
        { "VIDEOWRITER_PROP_DEPTH", cv::VIDEOWRITER_PROP_DEPTH },
#if CV_VERSION_GREATER_OR_EQUAL(4, 5, 2)
        { "VIDEOWRITER_PROP_HW_ACCELERATION", cv::VIDEOWRITER_PROP_HW_ACCELERATION },
        { "VIDEOWRITER_PROP_HW_DEVICE", cv::VIDEOWRITER_PROP_HW_DEVICE },
#endif
#endif

        /*
         * cvtColor color space code
         */

        { "COLOR_BGR2BGRA", cv::COLOR_BGR2BGRA },
        { "COLOR_RGB2RGBA", cv::COLOR_RGB2RGBA },
        { "COLOR_BGRA2BGR", cv::COLOR_BGRA2BGR },
        { "COLOR_RGBA2RGB", cv::COLOR_RGBA2RGB },
        { "COLOR_BGR2RGBA", cv::COLOR_BGR2RGBA },
        { "COLOR_RGB2BGRA", cv::COLOR_RGB2BGRA },
        { "COLOR_RGBA2BGR", cv::COLOR_RGBA2BGR },
        { "COLOR_BGRA2RGB", cv::COLOR_BGRA2RGB },
        { "COLOR_BGR2RGB", cv::COLOR_BGR2RGB },
        { "COLOR_RGB2BGR", cv::COLOR_RGB2BGR },
        { "COLOR_BGRA2RGBA", cv::COLOR_BGRA2RGBA },
        { "COLOR_RGBA2BGRA", cv::COLOR_RGBA2BGRA },
        { "COLOR_BGR2GRAY", cv::COLOR_BGR2GRAY },
        { "COLOR_RGB2GRAY", cv::COLOR_RGB2GRAY },
        { "COLOR_GRAY2BGR", cv::COLOR_GRAY2BGR },
        { "COLOR_GRAY2RGB", cv::COLOR_GRAY2RGB },
        { "COLOR_GRAY2BGRA", cv::COLOR_GRAY2BGRA },
        { "COLOR_GRAY2RGBA", cv::COLOR_GRAY2RGBA },
        { "COLOR_BGRA2GRAY", cv::COLOR_BGRA2GRAY },
        { "COLOR_RGBA2GRAY", cv::COLOR_RGBA2GRAY },
        { "COLOR_BGR2BGR565", cv::COLOR_BGR2BGR565 },
        { "COLOR_RGB2BGR565", cv::COLOR_RGB2BGR565 },
        { "COLOR_BGR5652BGR", cv::COLOR_BGR5652BGR },
        { "COLOR_BGR5652RGB", cv::COLOR_BGR5652RGB },
        { "COLOR_BGRA2BGR565", cv::COLOR_BGRA2BGR565 },
        { "COLOR_RGBA2BGR565", cv::COLOR_RGBA2BGR565 },
        { "COLOR_BGR5652BGRA", cv::COLOR_BGR5652BGRA },
        { "COLOR_BGR5652RGBA", cv::COLOR_BGR5652RGBA },
        { "COLOR_GRAY2BGR565", cv::COLOR_GRAY2BGR565 },
        { "COLOR_BGR5652GRAY", cv::COLOR_BGR5652GRAY },
        { "COLOR_BGR2BGR555", cv::COLOR_BGR2BGR555 },
        { "COLOR_RGB2BGR555", cv::COLOR_RGB2BGR555 },
        { "COLOR_BGR5552BGR", cv::COLOR_BGR5552BGR },
        { "COLOR_BGR5552RGB", cv::COLOR_BGR5552RGB },
        { "COLOR_BGRA2BGR555", cv::COLOR_BGRA2BGR555 },
        { "COLOR_RGBA2BGR555", cv::COLOR_RGBA2BGR555 },
        { "COLOR_BGR5552BGRA", cv::COLOR_BGR5552BGRA },
        { "COLOR_BGR5552RGBA", cv::COLOR_BGR5552RGBA },
        { "COLOR_GRAY2BGR555", cv::COLOR_GRAY2BGR555 },
        { "COLOR_BGR5552GRAY", cv::COLOR_BGR5552GRAY },
        { "COLOR_BGR2XYZ", cv::COLOR_BGR2XYZ },
        { "COLOR_RGB2XYZ", cv::COLOR_RGB2XYZ },
        { "COLOR_XYZ2BGR", cv::COLOR_XYZ2BGR },
        { "COLOR_XYZ2RGB", cv::COLOR_XYZ2RGB },
        { "COLOR_BGR2YCrCb", cv::COLOR_BGR2YCrCb },
        { "COLOR_RGB2YCrCb", cv::COLOR_RGB2YCrCb },
        { "COLOR_YCrCb2BGR", cv::COLOR_YCrCb2BGR },
        { "COLOR_YCrCb2RGB", cv::COLOR_YCrCb2RGB },
        { "COLOR_BGR2HSV", cv::COLOR_BGR2HSV },
        { "COLOR_RGB2HSV", cv::COLOR_RGB2HSV },
        { "COLOR_BGR2Lab", cv::COLOR_BGR2Lab },
        { "COLOR_RGB2Lab", cv::COLOR_RGB2Lab },
        { "COLOR_BGR2Luv", cv::COLOR_BGR2Luv },
        { "COLOR_RGB2Luv", cv::COLOR_RGB2Luv },
        { "COLOR_BGR2HLS", cv::COLOR_BGR2HLS },
        { "COLOR_RGB2HLS", cv::COLOR_RGB2HLS },
        { "COLOR_HSV2BGR", cv::COLOR_HSV2BGR },
        { "COLOR_HSV2RGB", cv::COLOR_HSV2RGB },
        { "COLOR_Lab2BGR", cv::COLOR_Lab2BGR },
        { "COLOR_Lab2RGB", cv::COLOR_Lab2RGB },
        { "COLOR_Luv2BGR", cv::COLOR_Luv2BGR },
        { "COLOR_Luv2RGB", cv::COLOR_Luv2RGB },
        { "COLOR_HLS2BGR", cv::COLOR_HLS2BGR },
        { "COLOR_HLS2RGB", cv::COLOR_HLS2RGB },
        { "COLOR_BGR2HSV_FULL", cv::COLOR_BGR2HSV_FULL },
        { "COLOR_RGB2HSV_FULL", cv::COLOR_RGB2HSV_FULL },
        { "COLOR_BGR2HLS_FULL", cv::COLOR_BGR2HLS_FULL },
        { "COLOR_RGB2HLS_FULL", cv::COLOR_RGB2HLS_FULL },
        { "COLOR_HSV2BGR_FULL", cv::COLOR_HSV2BGR_FULL },
        { "COLOR_HSV2RGB_FULL", cv::COLOR_HSV2RGB_FULL },
        { "COLOR_HLS2BGR_FULL", cv::COLOR_HLS2BGR_FULL },
        { "COLOR_HLS2RGB_FULL", cv::COLOR_HLS2RGB_FULL },
        { "COLOR_LBGR2Lab", cv::COLOR_LBGR2Lab },
        { "COLOR_LRGB2Lab", cv::COLOR_LRGB2Lab },
        { "COLOR_LBGR2Luv", cv::COLOR_LBGR2Luv },
        { "COLOR_LRGB2Luv", cv::COLOR_LRGB2Luv },
        { "COLOR_Lab2LBGR", cv::COLOR_Lab2LBGR },
        { "COLOR_Lab2LRGB", cv::COLOR_Lab2LRGB },
        { "COLOR_Luv2LBGR", cv::COLOR_Luv2LBGR },
        { "COLOR_Luv2LRGB", cv::COLOR_Luv2LRGB },
        { "COLOR_BGR2YUV", cv::COLOR_BGR2YUV },
        { "COLOR_RGB2YUV", cv::COLOR_RGB2YUV },
        { "COLOR_YUV2BGR", cv::COLOR_YUV2BGR },
        { "COLOR_YUV2RGB", cv::COLOR_YUV2RGB },
        { "COLOR_YUV2RGB_NV12", cv::COLOR_YUV2RGB_NV12 },
        { "COLOR_YUV2BGR_NV12", cv::COLOR_YUV2BGR_NV12 },
        { "COLOR_YUV2RGB_NV21", cv::COLOR_YUV2RGB_NV21 },
        { "COLOR_YUV2BGR_NV21", cv::COLOR_YUV2BGR_NV21 },
        { "COLOR_YUV420sp2RGB", cv::COLOR_YUV420sp2RGB },
        { "COLOR_YUV420sp2BGR", cv::COLOR_YUV420sp2BGR },
        { "COLOR_YUV2RGBA_NV12", cv::COLOR_YUV2RGBA_NV12 },
        { "COLOR_YUV2BGRA_NV12", cv::COLOR_YUV2BGRA_NV12 },
        { "COLOR_YUV2RGBA_NV21", cv::COLOR_YUV2RGBA_NV21 },
        { "COLOR_YUV2BGRA_NV21", cv::COLOR_YUV2BGRA_NV21 },
        { "COLOR_YUV420sp2RGBA", cv::COLOR_YUV420sp2RGBA },
        { "COLOR_YUV420sp2BGRA", cv::COLOR_YUV420sp2BGRA },
        { "COLOR_YUV2RGB_YV12", cv::COLOR_YUV2RGB_YV12 },
        { "COLOR_YUV2BGR_YV12", cv::COLOR_YUV2BGR_YV12 },
        { "COLOR_YUV2RGB_IYUV", cv::COLOR_YUV2RGB_IYUV },
        { "COLOR_YUV2BGR_IYUV", cv::COLOR_YUV2BGR_IYUV },
        { "COLOR_YUV2RGB_I420", cv::COLOR_YUV2RGB_I420 },
        { "COLOR_YUV2BGR_I420", cv::COLOR_YUV2BGR_I420 },
        { "COLOR_YUV420p2RGB", cv::COLOR_YUV420p2RGB },
        { "COLOR_YUV420p2BGR", cv::COLOR_YUV420p2BGR },
        { "COLOR_YUV2RGBA_YV12", cv::COLOR_YUV2RGBA_YV12 },
        { "COLOR_YUV2BGRA_YV12", cv::COLOR_YUV2BGRA_YV12 },
        { "COLOR_YUV2RGBA_IYUV", cv::COLOR_YUV2RGBA_IYUV },
        { "COLOR_YUV2BGRA_IYUV", cv::COLOR_YUV2BGRA_IYUV },
        { "COLOR_YUV2RGBA_I420", cv::COLOR_YUV2RGBA_I420 },
        { "COLOR_YUV2BGRA_I420", cv::COLOR_YUV2BGRA_I420 },
        { "COLOR_YUV420p2RGBA", cv::COLOR_YUV420p2RGBA },
        { "COLOR_YUV420p2BGRA", cv::COLOR_YUV420p2BGRA },
        { "COLOR_YUV2GRAY_420", cv::COLOR_YUV2GRAY_420 },
        { "COLOR_YUV2GRAY_NV21", cv::COLOR_YUV2GRAY_NV21 },
        { "COLOR_YUV2GRAY_NV12", cv::COLOR_YUV2GRAY_NV12 },
        { "COLOR_YUV2GRAY_YV12", cv::COLOR_YUV2GRAY_YV12 },
        { "COLOR_YUV2GRAY_IYUV", cv::COLOR_YUV2GRAY_IYUV },
        { "COLOR_YUV2GRAY_I420", cv::COLOR_YUV2GRAY_I420 },
        { "COLOR_YUV420sp2GRAY", cv::COLOR_YUV420sp2GRAY },
        { "COLOR_YUV420p2GRAY", cv::COLOR_YUV420p2GRAY },
        { "COLOR_YUV2RGB_UYVY", cv::COLOR_YUV2RGB_UYVY },
        { "COLOR_YUV2BGR_UYVY", cv::COLOR_YUV2BGR_UYVY },
        { "COLOR_YUV2RGB_Y422", cv::COLOR_YUV2RGB_Y422 },
        { "COLOR_YUV2BGR_Y422", cv::COLOR_YUV2BGR_Y422 },
        { "COLOR_YUV2RGB_UYNV", cv::COLOR_YUV2RGB_UYNV },
        { "COLOR_YUV2BGR_UYNV", cv::COLOR_YUV2BGR_UYNV },
        { "COLOR_YUV2RGBA_UYVY", cv::COLOR_YUV2RGBA_UYVY },
        { "COLOR_YUV2BGRA_UYVY", cv::COLOR_YUV2BGRA_UYVY },
        { "COLOR_YUV2RGBA_Y422", cv::COLOR_YUV2RGBA_Y422 },
        { "COLOR_YUV2BGRA_Y422", cv::COLOR_YUV2BGRA_Y422 },
        { "COLOR_YUV2RGBA_UYNV", cv::COLOR_YUV2RGBA_UYNV },
        { "COLOR_YUV2BGRA_UYNV", cv::COLOR_YUV2BGRA_UYNV },
        { "COLOR_YUV2RGB_YUY2", cv::COLOR_YUV2RGB_YUY2 },
        { "COLOR_YUV2BGR_YUY2", cv::COLOR_YUV2BGR_YUY2 },
        { "COLOR_YUV2RGB_YVYU", cv::COLOR_YUV2RGB_YVYU },
        { "COLOR_YUV2BGR_YVYU", cv::COLOR_YUV2BGR_YVYU },
        { "COLOR_YUV2RGB_YUYV", cv::COLOR_YUV2RGB_YUYV },
        { "COLOR_YUV2BGR_YUYV", cv::COLOR_YUV2BGR_YUYV },
        { "COLOR_YUV2RGB_YUNV", cv::COLOR_YUV2RGB_YUNV },
        { "COLOR_YUV2BGR_YUNV", cv::COLOR_YUV2BGR_YUNV },
        { "COLOR_YUV2RGBA_YUY2", cv::COLOR_YUV2RGBA_YUY2 },
        { "COLOR_YUV2BGRA_YUY2", cv::COLOR_YUV2BGRA_YUY2 },
        { "COLOR_YUV2RGBA_YVYU", cv::COLOR_YUV2RGBA_YVYU },
        { "COLOR_YUV2BGRA_YVYU", cv::COLOR_YUV2BGRA_YVYU },
        { "COLOR_YUV2RGBA_YUYV", cv::COLOR_YUV2RGBA_YUYV },
        { "COLOR_YUV2BGRA_YUYV", cv::COLOR_YUV2BGRA_YUYV },
        { "COLOR_YUV2RGBA_YUNV", cv::COLOR_YUV2RGBA_YUNV },
        { "COLOR_YUV2BGRA_YUNV", cv::COLOR_YUV2BGRA_YUNV },
        { "COLOR_YUV2GRAY_UYVY", cv::COLOR_YUV2GRAY_UYVY },
        { "COLOR_YUV2GRAY_YUY2", cv::COLOR_YUV2GRAY_YUY2 },
        { "COLOR_YUV2GRAY_Y422", cv::COLOR_YUV2GRAY_Y422 },
        { "COLOR_YUV2GRAY_UYNV", cv::COLOR_YUV2GRAY_UYNV },
        { "COLOR_YUV2GRAY_YVYU", cv::COLOR_YUV2GRAY_YVYU },
        { "COLOR_YUV2GRAY_YUYV", cv::COLOR_YUV2GRAY_YUYV },
        { "COLOR_YUV2GRAY_YUNV", cv::COLOR_YUV2GRAY_YUNV },
        { "COLOR_RGBA2mRGBA", cv::COLOR_RGBA2mRGBA },
        { "COLOR_mRGBA2RGBA", cv::COLOR_mRGBA2RGBA },
        { "COLOR_RGB2YUV_I420", cv::COLOR_RGB2YUV_I420 },
        { "COLOR_BGR2YUV_I420", cv::COLOR_BGR2YUV_I420 },
        { "COLOR_RGB2YUV_IYUV", cv::COLOR_RGB2YUV_IYUV },
        { "COLOR_BGR2YUV_IYUV", cv::COLOR_BGR2YUV_IYUV },
        { "COLOR_RGBA2YUV_I420", cv::COLOR_RGBA2YUV_I420 },
        { "COLOR_BGRA2YUV_I420", cv::COLOR_BGRA2YUV_I420 },
        { "COLOR_RGBA2YUV_IYUV", cv::COLOR_RGBA2YUV_IYUV },
        { "COLOR_BGRA2YUV_IYUV", cv::COLOR_BGRA2YUV_IYUV },
        { "COLOR_RGB2YUV_YV12", cv::COLOR_RGB2YUV_YV12 },
        { "COLOR_BGR2YUV_YV12", cv::COLOR_BGR2YUV_YV12 },
        { "COLOR_RGBA2YUV_YV12", cv::COLOR_RGBA2YUV_YV12 },
        { "COLOR_BGRA2YUV_YV12", cv::COLOR_BGRA2YUV_YV12 },
        { "COLOR_BayerBG2BGR", cv::COLOR_BayerBG2BGR },
        { "COLOR_BayerGB2BGR", cv::COLOR_BayerGB2BGR },
        { "COLOR_BayerRG2BGR", cv::COLOR_BayerRG2BGR },
        { "COLOR_BayerGR2BGR", cv::COLOR_BayerGR2BGR },
        { "COLOR_BayerBG2RGB", cv::COLOR_BayerBG2RGB },
        { "COLOR_BayerGB2RGB", cv::COLOR_BayerGB2RGB },
        { "COLOR_BayerRG2RGB", cv::COLOR_BayerRG2RGB },
        { "COLOR_BayerGR2RGB", cv::COLOR_BayerGR2RGB },
        { "COLOR_BayerBG2GRAY", cv::COLOR_BayerBG2GRAY },
        { "COLOR_BayerGB2GRAY", cv::COLOR_BayerGB2GRAY },
        { "COLOR_BayerRG2GRAY", cv::COLOR_BayerRG2GRAY },
        { "COLOR_BayerGR2GRAY", cv::COLOR_BayerGR2GRAY },
        { "COLOR_BayerBG2BGR_VNG", cv::COLOR_BayerBG2BGR_VNG },
        { "COLOR_BayerGB2BGR_VNG", cv::COLOR_BayerGB2BGR_VNG },
        { "COLOR_BayerRG2BGR_VNG", cv::COLOR_BayerRG2BGR_VNG },
        { "COLOR_BayerGR2BGR_VNG", cv::COLOR_BayerGR2BGR_VNG },
        { "COLOR_BayerBG2RGB_VNG", cv::COLOR_BayerBG2RGB_VNG },
        { "COLOR_BayerGB2RGB_VNG", cv::COLOR_BayerGB2RGB_VNG },
        { "COLOR_BayerRG2RGB_VNG", cv::COLOR_BayerRG2RGB_VNG },
        { "COLOR_BayerGR2RGB_VNG", cv::COLOR_BayerGR2RGB_VNG },
        { "COLOR_BayerBG2BGR_EA", cv::COLOR_BayerBG2BGR_EA },
        { "COLOR_BayerGB2BGR_EA", cv::COLOR_BayerGB2BGR_EA },
        { "COLOR_BayerRG2BGR_EA", cv::COLOR_BayerRG2BGR_EA },
        { "COLOR_BayerGR2BGR_EA", cv::COLOR_BayerGR2BGR_EA },
        { "COLOR_BayerBG2RGB_EA", cv::COLOR_BayerBG2RGB_EA },
        { "COLOR_BayerGB2RGB_EA", cv::COLOR_BayerGB2RGB_EA },
        { "COLOR_BayerRG2RGB_EA", cv::COLOR_BayerRG2RGB_EA },
        { "COLOR_BayerGR2RGB_EA", cv::COLOR_BayerGR2RGB_EA },
#ifdef TCL_USE_OPENCV4
        { "COLOR_BayerBG2BGRA", cv::COLOR_BayerBG2BGRA },
        { "COLOR_BayerGB2BGRA", cv::COLOR_BayerGB2BGRA },
        { "COLOR_BayerRG2BGRA", cv::COLOR_BayerRG2BGRA },
        { "COLOR_BayerGR2BGRA", cv::COLOR_BayerGR2BGRA },
        { "COLOR_BayerBG2RGBA", cv::COLOR_BayerBG2RGBA },
        { "COLOR_BayerGB2RGBA", cv::COLOR_BayerGB2RGBA },
        { "COLOR_BayerRG2RGBA", cv::COLOR_BayerRG2RGBA },
        { "COLOR_BayerGR2RGBA", cv::COLOR_BayerGR2RGBA },
#endif
        { "COLOR_COLORCVT_MAX", cv::COLOR_COLORCVT_MAX },

        /*
         * Line Types
         */

        { "FILLED", cv::FILLED },
        { "LINE_4", cv::LINE_4 },
        { "LINE_8", cv::LINE_8 },
        { "LINE_AA", cv::LINE_AA },

        /*
         * Marker Types
         */

        { "MARKER_CROSS", cv::MARKER_CROSS },
        { "MARKER_TILTED_CROSS", cv::MARKER_TILTED_CROSS },
        { "MARKER_STAR", cv::MARKER_STAR },
        { "MARKER_DIAMOND", cv::MARKER_DIAMOND },
        { "MARKER_SQUARE", cv::MARKER_SQUARE },
        { "MARKER_TRIANGLE_UP", cv::MARKER_TRIANGLE_UP },
        { "MARKER_TRIANGLE_DOWN", cv::MARKER_TRIANGLE_DOWN },

        /*
         * fontFace
         */

        { "FONT_HERSHEY_SIMPLEX", cv::FONT_HERSHEY_SIMPLEX },
        { "FONT_HERSHEY_PLAIN", cv::FONT_HERSHEY_PLAIN },
        { "FONT_HERSHEY_DUPLEX", cv::FONT_HERSHEY_DUPLEX },
        { "FONT_HERSHEY_COMPLEX", cv::FONT_HERSHEY_COMPLEX },
        { "FONT_HERSHEY_TRIPLEX", cv::FONT_HERSHEY_TRIPLEX },
        { "FONT_HERSHEY_COMPLEX_SMALL", cv::FONT_HERSHEY_COMPLEX_SMALL },
        { "FONT_HERSHEY_SCRIPT_SIMPLEX", cv::FONT_HERSHEY_SCRIPT_SIMPLEX },
        { "FONT_HERSHEY_SCRIPT_COMPLEX", cv::FONT_HERSHEY_SCRIPT_COMPLEX },
        { "FONT_ITALIC", cv::FONT_ITALIC },

        /*
         * threshold
         */

        { "THRESH_BINARY", cv::THRESH_BINARY },
        { "THRESH_BINARY_INV", cv::THRESH_BINARY_INV },
        { "THRESH_TRUNC", cv::THRESH_TRUNC },
        { "THRESH_TOZERO", cv::THRESH_TOZERO },
        { "THRESH_TOZERO_INV", cv::THRESH_TOZERO_INV },
        { "THRESH_MASK", cv::THRESH_MASK },
        { "THRESH_OTSU", cv::THRESH_OTSU },
        { "THRESH_TRIANGLE", cv::THRESH_TRIANGLE },

        /*
         * Adaptive Thresholding
         */

        { "ADAPTIVE_THRESH_MEAN_C", cv::ADAPTIVE_THRESH_MEAN_C },
        { "ADAPTIVE_THRESH_GAUSSIAN_C", cv::ADAPTIVE_THRESH_GAUSSIAN_C },

        /*
         * border types
         */

        { "BORDER_CONSTANT", cv::BORDER_CONSTANT },
        { "BORDER_REPLICATE", cv::BORDER_REPLICATE },
        { "BORDER_REFLECT", cv::BORDER_REFLECT },
        { "BORDER_WRAP", cv::BORDER_WRAP },
        { "BORDER_REFLECT_101", cv::BORDER_REFLECT_101 },
        { "BORDER_TRANSPARENT", cv::BORDER_TRANSPARENT },
        { "BORDER_REFLECT101", cv::BORDER_REFLECT101 },
        { "BORDER_DEFAULT", cv::BORDER_DEFAULT },
        { "BORDER_ISOLATED", cv::BORDER_ISOLATED },

        /*
         * Morph Shapes
         */

        { "MORPH_RECT", cv::MORPH_RECT },
        { "MORPH_CROSS", cv::MORPH_CROSS },
        { "MORPH_ELLIPSE", cv::MORPH_ELLIPSE },

        /*
         * Morph Types
         */

        { "MORPH_ERODE", cv::MORPH_ERODE },
        { "MORPH_DILATE", cv::MORPH_DILATE },
        { "MORPH_OPEN", cv::MORPH_OPEN },
        { "MORPH_CLOSE", cv::MORPH_CLOSE },
        { "MORPH_GRADIENT", cv::MORPH_GRADIENT },
        { "MORPH_TOPHAT", cv::MORPH_TOPHAT },
        { "MORPH_BLACKHAT", cv::MORPH_BLACKHAT },
        { "MORPH_HITMISS", cv::MORPH_HITMISS },

        /*
         * For Shape Match Modes
         */

#ifdef TCL_USE_OPENCV4
        { "CONTOURS_MATCH_I1", cv::ShapeMatchModes::CONTOURS_MATCH_I1 },
        { "CONTOURS_MATCH_I2", cv::ShapeMatchModes::CONTOURS_MATCH_I2 },
        { "CONTOURS_MATCH_I3", cv::ShapeMatchModes::CONTOURS_MATCH_I3 },
#else
        { "CONTOURS_MATCH_I1", CV_CONTOURS_MATCH_I1 },
        { "CONTOURS_MATCH_I2", CV_CONTOURS_MATCH_I2 },
        { "CONTOURS_MATCH_I3", CV_CONTOURS_MATCH_I3 },
#endif

        /*
         * For color map
         */

        { "COLORMAP_AUTUMN", cv::COLORMAP_AUTUMN },
        { "COLORMAP_BONE", cv::COLORMAP_BONE },
        { "COLORMAP_JET", cv::COLORMAP_JET },
        { "COLORMAP_WINTER", cv::COLORMAP_WINTER },
        { "COLORMAP_RAINBOW", cv::COLORMAP_RAINBOW },
        { "COLORMAP_OCEAN", cv::COLORMAP_OCEAN },
        { "COLORMAP_SUMMER", cv::COLORMAP_SUMMER },
        { "COLORMAP_SPRING", cv::COLORMAP_SPRING },
        { "COLORMAP_COOL", cv::COLORMAP_COOL },
        { "COLORMAP_HSV", cv::COLORMAP_HSV },
        { "COLORMAP_PINK", cv::COLORMAP_PINK },
        { "COLORMAP_HOT", cv::COLORMAP_HOT },
        { "COLORMAP_PARULA", cv::COLORMAP_PARULA },
#ifdef TCL_USE_OPENCV4
        { "COLORMAP_MAGMA", cv::COLORMAP_MAGMA },
        { "COLORMAP_INFERNO", cv::COLORMAP_INFERNO },
        { "COLORMAP_PLASMA", cv::COLORMAP_PLASMA },
        { "COLORMAP_VIRIDIS", cv::COLORMAP_VIRIDIS },
        { "COLORMAP_CIVIDIS", cv::COLORMAP_CIVIDIS },
        { "COLORMAP_TWILIGHT", cv::COLORMAP_TWILIGHT },
        { "COLORMAP_TWILIGHT_SHIFTED", cv::COLORMAP_TWILIGHT_SHIFTED },
        { "COLORMAP_TURBO", cv::COLORMAP_TURBO },
        { "COLORMAP_DEEPGREEN", cv::COLORMAP_DEEPGREEN },
#endif

        /*
         * Windows flags
         */

        { "WINDOW_NORMAL", cv::WINDOW_NORMAL },
        { "WINDOW_AUTOSIZE", cv::WINDOW_AUTOSIZE },
        { "WINDOW_OPENGL", cv::WINDOW_OPENGL },
        { "WINDOW_FULLSCREEN", cv::WINDOW_FULLSCREEN },
        { "WINDOW_FREERATIO", cv::WINDOW_FREERATIO },
        { "WINDOW_KEEPRATIO", cv::WINDOW_KEEPRATIO },
        { "WINDOW_GUI_EXPANDED", cv::WINDOW_GUI_EXPANDED },
        { "WINDOW_GUI_NORMAL", cv::WINDOW_GUI_NORMAL },

        /*
         * Mouse event
         */

        { "EVENT_MOUSEMOVE", cv::EVENT_MOUSEMOVE },
        { "EVENT_LBUTTONDOWN", cv::EVENT_LBUTTONDOWN },
        { "EVENT_RBUTTONDOWN", cv::EVENT_RBUTTONDOWN },
        { "EVENT_MBUTTONDOWN", cv::EVENT_MBUTTONDOWN },
        { "EVENT_LBUTTONUP", cv::EVENT_LBUTTONUP },
        { "EVENT_RBUTTONUP", cv::EVENT_RBUTTONUP },
        { "EVENT_MBUTTONUP", cv::EVENT_MBUTTONUP },
        { "EVENT_LBUTTONDBLCLK", cv::EVENT_LBUTTONDBLCLK },
        { "EVENT_RBUTTONDBLCLK", cv::EVENT_RBUTTONDBLCLK },
        { "EVENT_MBUTTONDBLCLK", cv::EVENT_MBUTTONDBLCLK },
        { "EVENT_MOUSEWHEEL", cv::EVENT_MOUSEWHEEL },
        { "EVENT_MOUSEHWHEEL", cv::EVENT_MOUSEHWHEEL },

        /*
         * Mouse event flag
         */

        { "EVENT_FLAG_LBUTTON", cv::EVENT_FLAG_LBUTTON },
        { "EVENT_FLAG_RBUTTON", cv::EVENT_FLAG_RBUTTON },
        { "EVENT_FLAG_MBUTTON", cv::EVENT_FLAG_MBUTTON },
        { "EVENT_FLAG_CTRLKEY", cv::EVENT_FLAG_CTRLKEY },
        { "EVENT_FLAG_SHIFTKEY", cv::EVENT_FLAG_SHIFTKEY },
        { "EVENT_FLAG_ALTKEY", cv::EVENT_FLAG_ALTKEY },

        /*
         * compareHist method
         */

        { "HISTCMP_CORREL", cv::HISTCMP_CORREL },
        { "HISTCMP_CHISQR", cv::HISTCMP_CHISQR },
        { "HISTCMP_INTERSECT", cv::HISTCMP_INTERSECT },
        { "HISTCMP_BHATTACHARYYA", cv::HISTCMP_BHATTACHARYYA },
        { "HISTCMP_HELLINGER", cv::HISTCMP_HELLINGER },
        { "HISTCMP_CHISQR_ALT", cv::HISTCMP_CHISQR_ALT },
        { "HISTCMP_KL_DIV", cv::HISTCMP_KL_DIV },

        /*
         * LineSegmentDetector modes
         */

        { "LSD_REFINE_NONE", cv::LSD_REFINE_NONE },
        { "LSD_REFINE_STD", cv::LSD_REFINE_STD },
        { "LSD_REFINE_ADV", cv::LSD_REFINE_ADV },

        /*
         * dft flags
         */

        { "DFT_INVERSE", cv::DFT_INVERSE },
        { "DFT_SCALE", cv::DFT_SCALE },
        { "DFT_ROWS", cv::DFT_ROWS },
        { "DFT_COMPLEX_OUTPUT", cv::DFT_COMPLEX_OUTPUT },
        { "DFT_REAL_OUTPUT", cv::DFT_REAL_OUTPUT },
#ifdef TCL_USE_OPENCV4
        { "DFT_COMPLEX_INPUT", cv::DFT_COMPLEX_INPUT },
#endif
        { "DCT_INVERSE", cv::DCT_INVERSE },
        { "DCT_ROWS", cv::DCT_ROWS },

        /*
         * SVD flags
         */

        { "SVD_MODIFY_A", cv::SVD::Flags::MODIFY_A },
        { "SVD_NO_UV", cv::SVD::Flags::NO_UV },
        { "SVD_FULL_UV", cv::SVD::Flags::FULL_UV },

        /*
         * NormTypes
         */

        { "NORM_INF", cv::NORM_INF },
        { "NORM_L1", cv::NORM_L1 },
        { "NORM_L2", cv::NORM_L2 },
        { "NORM_L2SQR", cv::NORM_L2SQR },
        { "NORM_HAMMING", cv::NORM_HAMMING },
        { "NORM_HAMMING2", cv::NORM_HAMMING2 },
        { "NORM_TYPE_MASK", cv::NORM_TYPE_MASK },
        { "NORM_RELATIVE", cv::NORM_RELATIVE },
        { "NORM_MINMAX", cv::NORM_MINMAX },

        /*
         * FloodFill Flags
         */

        { "FLOODFILL_FIXED_RANGE", cv::FLOODFILL_FIXED_RANGE },

        /*
         * Interpolation Flags
         */

        { "INTER_NEAREST", cv::INTER_NEAREST },
        { "INTER_LINEAR", cv::INTER_LINEAR },
        { "INTER_CUBIC", cv::INTER_CUBIC },
        { "INTER_AREA", cv::INTER_AREA },
        { "INTER_LANCZOS4", cv::INTER_LANCZOS4 },
#ifdef TCL_USE_OPENCV4
        { "INTER_LINEAR_EXACT", cv::INTER_LINEAR_EXACT },
        { "INTER_NEAREST_EXACT", cv::INTER_NEAREST_EXACT },
#endif
        { "INTER_MAX", cv::INTER_MAX },
        { "WARP_FILL_OUTLIERS", cv::WARP_FILL_OUTLIERS },
        { "WARP_INVERSE_MAP", cv::WARP_INVERSE_MAP },

#ifdef TCL_USE_OPENCV4
        /*
         * WarpPolar Mode
         */

        { "WARP_POLAR_LINEAR", cv::WarpPolarMode::WARP_POLAR_LINEAR },
        { "WARP_POLAR_LOG", cv::WarpPolarMode::WARP_POLAR_LOG },
#endif

        /*
         * Cmp Types
         */

        { "CMP_EQ", cv::CMP_EQ },
        { "CMP_GT", cv::CMP_GT },
        { "CMP_GE", cv::CMP_GE },
        { "CMP_LT", cv::CMP_LT },
        { "CMP_LE", cv::CMP_LE },
        { "CMP_NE", cv::CMP_NE },

        /*
         * Reduce Types
         */

        { "REDUCE_SUM", cv::REDUCE_SUM },
        { "REDUCE_AVG", cv::REDUCE_AVG },
        { "REDUCE_MAX", cv::REDUCE_MAX },
        { "REDUCE_MIN", cv::REDUCE_MIN },

        /*
         * Rotate Flags (rotateCode)
         */

        { "ROTATE_90_CLOCKWISE", cv::ROTATE_90_CLOCKWISE },
        { "ROTATE_180", cv::ROTATE_180 },
        { "ROTATE_90_COUNTERCLOCKWISE", cv::ROTATE_90_COUNTERCLOCKWISE },

        /*
         * Decomp Types
         */

        { "DECOMP_LU", cv::DECOMP_LU },
        { "DECOMP_SVD", cv::DECOMP_SVD },
        { "DECOMP_EIG", cv::DECOMP_EIG },
        { "DECOMP_CHOLESKY", cv::DECOMP_CHOLESKY },
        { "DECOMP_QR", cv::DECOMP_QR },
        { "DECOMP_NORMAL", cv::DECOMP_NORMAL },

        /*
         * Retrieval Modes
         */

        { "RETR_EXTERNAL", cv::RETR_EXTERNAL },
        { "RETR_LIST", cv::RETR_LIST },
        { "RETR_CCOMP", cv::RETR_CCOMP },
        { "RETR_TREE", cv::RETR_TREE },
        { "RETR_FLOODFILL", cv::RETR_FLOODFILL },

        /*
         * Contour Approximation Modes
         */

        { "CHAIN_APPROX_NONE", cv::CHAIN_APPROX_NONE },
        { "CHAIN_APPROX_SIMPLE", cv::CHAIN_APPROX_SIMPLE },
        { "CHAIN_APPROX_TC89_L1", cv::CHAIN_APPROX_TC89_L1 },
        { "CHAIN_APPROX_TC89_KCOS", cv::CHAIN_APPROX_TC89_KCOS },

        /*
         * Hough Modes for HoughCircles
         */

        { "HOUGH_GRADIENT", cv::HOUGH_GRADIENT },
#ifdef TCL_USE_OPENCV4
        { "HOUGH_GRADIENT_ALT", cv::HOUGH_GRADIENT_ALT },
#endif

        /*
         * Template Match Modes
         */

        { "TM_SQDIFF", cv::TM_SQDIFF },
        { "TM_SQDIFF_NORMED", cv::TM_SQDIFF_NORMED },
        { "TM_CCORR", cv::TM_CCORR },
        { "TM_CCORR_NORMED", cv::TM_CCORR_NORMED },
        { "TM_CCOEFF", cv::TM_CCOEFF },
        { "TM_CCOEFF_NORMED", cv::TM_CCOEFF_NORMED },

        /*
         * Distance Types
         */

        { "DIST_L1", cv::DIST_L1 },
        { "DIST_L2", cv::DIST_L2 },
        { "DIST_C", cv::DIST_C },
        { "DIST_L12", cv::DIST_L12 },
        { "DIST_FAIR", cv::DIST_FAIR },
        { "DIST_WELSCH", cv::DIST_WELSCH },
        { "DIST_HUBER", cv::DIST_HUBER },

        /*
         * GrabCut Classes
         */

        { "GC_BGD", cv::GC_BGD },
        { "GC_FGD", cv::GC_FGD },
        { "GC_PR_BGD", cv::GC_PR_BGD },
        { "GC_PR_FGD", cv::GC_PR_FGD },

        /*
         * Covar Flags
         */

        { "COVAR_SCRAMBLED", cv::COVAR_SCRAMBLED },
        { "COVAR_NORMAL", cv::COVAR_NORMAL },
        { "COVAR_USE_AVG", cv::COVAR_USE_AVG },
        { "COVAR_SCALE", cv::COVAR_SCALE },
        { "COVAR_ROWS", cv::COVAR_ROWS },
        { "COVAR_COLS", cv::COVAR_COLS },

        /*
         * Kmeans flags
         */

        { "KMEANS_RANDOM_CENTERS", cv::KMEANS_RANDOM_CENTERS },
        { "KMEANS_PP_CENTERS", cv::KMEANS_PP_CENTERS },
        { "KMEANS_USE_INITIAL_LABELS", cv::KMEANS_USE_INITIAL_LABELS },

        /*
         * PCA flags
         */

        { "DATA_AS_ROW", cv::PCA::DATA_AS_ROW },
        { "DATA_AS_COL", cv::PCA::DATA_AS_COL },
        { "USE_AVG", cv::PCA::USE_AVG },

        /*
         * TermCriteria type
         */

        { "COUNT", cv::TermCriteria::COUNT },
        { "EPS", cv::TermCriteria::EPS },

        /*
         * FastFeature Detector type
         */

        { "DetectorType_TYPE_5_8", cv::FastFeatureDetector::TYPE_5_8 },
        { "DetectorType_TYPE_7_12", cv::FastFeatureDetector::TYPE_7_12 },
        { "DetectorType_TYPE_9_16", cv::FastFeatureDetector::TYPE_9_16 },

        /*
         * AgastFeatureDetector Detector type
         */

        { "DetectorType_AGAST_5_8", cv::AgastFeatureDetector::AGAST_5_8 },
        { "DetectorType_AGAST_7_12d", cv::AgastFeatureDetector::AGAST_7_12d },
        { "DetectorType_AGAST_7_12s", cv::AgastFeatureDetector::AGAST_7_12s },
        { "DetectorType_OAST_9_16", cv::AgastFeatureDetector::OAST_9_16 },

        /*
         * ORB Detector type
         */

#ifdef TCL_USE_OPENCV4
        { "ORB_HARRIS_SCORE", cv::ORB::ScoreType::HARRIS_SCORE },
        { "ORB_FAST_SCORE", cv::ORB::ScoreType::FAST_SCORE },
#else
        { "ORB_HARRIS_SCORE", cv::ORB::HARRIS_SCORE },
        { "ORB_FAST_SCORE", cv::ORB::FAST_SCORE },
#endif

        /*
         * AKAZE descriptor type
         */

        { "AKAZE_DESCRIPTOR_KAZE_UPRIGHT", cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT },
        { "AKAZE_DESCRIPTOR_KAZE", cv::AKAZE::DESCRIPTOR_KAZE },
        { "AKAZE_DESCRIPTOR_MLDB_UPRIGHT", cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT },
        { "AKAZE_DESCRIPTOR_MLDB", cv::AKAZE::DESCRIPTOR_MLDB },

#ifdef TCL_USE_OPENCV4
        /*
         * KAZE and AKAZE Diffusivity type
         */

        { "KAZE_DIFF_PM_G1", cv::KAZE::DiffusivityType::DIFF_PM_G1 },
        { "KAZE_DIFF_PM_G2", cv::KAZE::DiffusivityType::DIFF_PM_G2 },
        { "KAZE_DIFF_WEICKERT", cv::KAZE::DiffusivityType::DIFF_WEICKERT },
        { "KAZE_DIFF_CHARBONNIER", cv::KAZE::DiffusivityType::DIFF_CHARBONNIER },
#endif

        /*
         * drawKeypoints DrawMatches Flags
         */

        { "DRAW_MATCHES_FLAGS_DEFAULT", (int) cv::DrawMatchesFlags::DEFAULT },
        { "DRAW_MATCHES_FLAGS_DRAW_OVER_OUTIMG", (int) cv::DrawMatchesFlags::DRAW_OVER_OUTIMG },
        { "DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS", (int) cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS },
        { "DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS", (int) cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS },

#ifdef TCL_USE_OPENCV4
        /*
         * findTransformECC motion type
         */

        { "MOTION_TRANSLATION", cv::MOTION_TRANSLATION },
        { "MOTION_EUCLIDEAN", cv::MOTION_EUCLIDEAN },
        { "MOTION_AFFINE", cv::MOTION_AFFINE },
        { "MOTION_HOMOGRAPHY", cv::MOTION_HOMOGRAPHY },
#endif

        /*
         * findChessboardCorners flags
         */

        { "CALIB_CB_ADAPTIVE_THRESH", cv::CALIB_CB_ADAPTIVE_THRESH },
        { "CALIB_CB_NORMALIZE_IMAGE", cv::CALIB_CB_NORMALIZE_IMAGE },
        { "CALIB_CB_FILTER_QUADS", cv::CALIB_CB_FILTER_QUADS },
        { "CALIB_CB_FAST_CHECK", cv::CALIB_CB_FAST_CHECK },

        /*
         * findFundamentalMat method
         */

        { "FM_7POINT", cv::FM_7POINT },
        { "FM_8POINT",cv::FM_8POINT },
        { "FM_LMEDS", cv::FM_LMEDS },
        { "FM_RANSAC", cv::FM_RANSAC },

        /*
         * findHomography method
         */

        { "LMEDS", cv::LMEDS },
        { "RANSAC", cv::RANSAC },
        { "RHO", cv::RHO },

        /*
         * StereoBM PreFilter type
         */

        { "PREFILTER_NORMALIZED_RESPONSE", cv::StereoBM::PREFILTER_NORMALIZED_RESPONSE },
        { "PREFILTER_XSOBEL", cv::StereoBM::PREFILTER_XSOBEL },

        /*
         * StereoSGBM mode
         */

        { "StereoSGBM_MODE_SGBM", cv::StereoSGBM::MODE_SGBM },
        { "StereoSGBM_MODE_HH", cv::StereoSGBM::MODE_HH },
        { "StereoSGBM_MODE_SGBM_3WAY", cv::StereoSGBM::MODE_SGBM_3WAY },
#ifdef TCL_USE_OPENCV4
        { "StereoSGBM_MODE_HH4", cv::StereoSGBM::MODE_HH4 },
#endif

        /*
         * Photo inpaint flags
         */

        { "INPAINT_NS", cv::INPAINT_NS },
        { "INPAINT_TELEA", cv::INPAINT_TELEA },

        /*
         * Photo edgePreservingFilter flags
         */

        { "RECURS_FILTER", cv::RECURS_FILTER },
        { "NORMCONV_FILTER", cv::NORMCONV_FILTER },

        /*
         * Photo seamlessClone flags
         */

        { "NORMAL_CLONE", cv::NORMAL_CLONE },
        { "MIXED_CLONE", cv::MIXED_CLONE },
        { "MONOCHROME_TRANSFER", cv::MONOCHROME_TRANSFER },

        /*
         * Stitcher mode
         */

        { "PANORAMA", cv::Stitcher::PANORAMA },
        { "SCANS", cv::Stitcher::SCANS },

        /*
         * ML sample types
         */

        { "ml::ROW_SAMPLE", cv::ml::SampleTypes::ROW_SAMPLE },
        { "ml::COL_SAMPLE", cv::ml::SampleTypes::COL_SAMPLE },

        /*
         * Predict options -
         */

        { "ml::UPDATE_MODEL", cv::ml::StatModel::Flags::UPDATE_MODEL },
        { "ml::RAW_OUTPUT", cv::ml::StatModel::Flags::RAW_OUTPUT },
        { "ml::COMPRESSED_INPUT", cv::ml::StatModel::Flags::COMPRESSED_INPUT },
        { "ml::PREPROCESSED_INPUT", cv::ml::StatModel::Flags::PREPROCESSED_INPUT },

        /*
         * LogisticRegression Methods
         */

        { "ml::LOGISTIC_BATCH", cv::ml::LogisticRegression::Methods::BATCH },
        { "ml::LOGISTIC_MINI_BATCH", cv::ml::LogisticRegression::Methods::MINI_BATCH },

        /*
         * LogisticRegression RegKinds
         */

        { "ml::LOGISTIC_REG_DISABLE", cv::ml::LogisticRegression::RegKinds::REG_DISABLE },
        { "ml::LOGISTIC_REG_L1", cv::ml::LogisticRegression::RegKinds::REG_L1 },
        { "ml::LOGISTIC_REG_L2", cv::ml::LogisticRegression::RegKinds::REG_L2 },

        /*
         * KNearest algorithm
         */

        { "ml::KNEAREST_BRUTE_FORCE", cv::ml::KNearest::Types::BRUTE_FORCE },
        { "ml::KNEAREST_KDTREE", cv::ml::KNearest::Types::KDTREE },

        /*
         * SVM types
         */

        { "ml::SVM_C_SVC", cv::ml::SVM::Types::C_SVC },
        { "ml::SVM_NU_SVC", cv::ml::SVM::Types::NU_SVC },
        { "ml::SVM_ONE_CLASS", cv::ml::SVM::Types::ONE_CLASS },
        { "ml::SVM_EPS_SVR", cv::ml::SVM::Types::EPS_SVR },
        { "ml::SVM_NU_SVR", cv::ml::SVM::Types::NU_SVR },

        /*
         * SVM kernel types
         */

        { "ml::SVM_LINEAR", cv::ml::SVM::KernelTypes::LINEAR },
        { "ml::SVM_POLY", cv::ml::SVM::KernelTypes::POLY },
        { "ml::SVM_RBF", cv::ml::SVM::KernelTypes::RBF },
        { "ml::SVM_SIGMOID", cv::ml::SVM::KernelTypes::SIGMOID },
        { "ml::SVM_CHI2", cv::ml::SVM::KernelTypes::CHI2 },
        { "ml::SVM_INTER", cv::ml::SVM::KernelTypes::INTER },

        /*
         * SVMSGD Margin Type
         */

        { "ml::SVMSGD_SOFT_MARGIN", cv::ml::SVMSGD::MarginType::SOFT_MARGIN },
        { "ml::SVMSGD_HARD_MARGIN", cv::ml::SVMSGD::MarginType::HARD_MARGIN },

        /*
         * SVMSGD Type
         */

        { "ml::SVMSGD_SGD", cv::ml::SVMSGD::SvmsgdType::SGD },
        { "ml::SVMSGD_ASGD", cv::ml::SVMSGD::SvmsgdType::ASGD },

        /*
         * Boost Types
         */

        { "ml::BOOST_DISCRETE", cv::ml::Boost::Types::DISCRETE },
        { "ml::BOOST_REAL", cv::ml::Boost::Types::REAL },
        { "ml::BOOST_LOGIT", cv::ml::Boost::Types::LOGIT },
        { "ml::BOOST_GENTLE", cv::ml::Boost::Types::GENTLE },

        /*
         * ANN_MLP ActivationFunctions
         */

        { "ml::MLP_IDENTITY", cv::ml::ANN_MLP::ActivationFunctions::IDENTITY },
        { "ml::MLP_SIGMOID_SYM", cv::ml::ANN_MLP::ActivationFunctions::SIGMOID_SYM },
        { "ml::MLP_GAUSSIAN", cv::ml::ANN_MLP::ActivationFunctions::GAUSSIAN },
#ifdef TCL_USE_OPENCV4
        { "ml::MLP_RELU", cv::ml::ANN_MLP::ActivationFunctions::RELU },
        { "ml::MLP_LEAKYRELU", cv::ml::ANN_MLP::ActivationFunctions::LEAKYRELU },
#endif

        /*
         * ANN_MLP Train Flags
         */

        { "ml::MLP_UPDATE_WEIGHTS", cv::ml::ANN_MLP::TrainFlags::UPDATE_WEIGHTS },
        { "ml::MLP_NO_INPUT_SCALE", cv::ml::ANN_MLP::TrainFlags::NO_INPUT_SCALE },
        { "ml::MLP_NO_OUTPUT_SCALE", cv::ml::ANN_MLP::TrainFlags::NO_OUTPUT_SCALE },

        /*
         * ANN_MLP Training Methods
         */

        { "ml::MLP_BACKPROP", cv::ml::ANN_MLP::TrainingMethods::BACKPROP },
        { "ml::MLP_RPROP", cv::ml::ANN_MLP::TrainingMethods::RPROP },
#ifdef TCL_USE_OPENCV4
        { "ml::MLP_ANNEAL", cv::ml::ANN_MLP::TrainingMethods::ANNEAL },
#endif

#ifdef TCL_USE_OPENCV4
        /*
         * DNN backend
         */

        { "dnn::DNN_BACKEND_DEFAULT", cv::dnn::DNN_BACKEND_DEFAULT },
        { "dnn::DNN_BACKEND_HALIDE", cv::dnn::DNN_BACKEND_HALIDE },
        { "dnn::DNN_BACKEND_OPENCV", cv::dnn::DNN_BACKEND_OPENCV },
        { "dnn::DNN_BACKEND_VKCOM", cv::dnn::DNN_BACKEND_VKCOM },
        { "dnn::DNN_BACKEND_CUDA", cv::dnn::DNN_BACKEND_CUDA },
#endif

#ifdef TCL_USE_OPENCV4
        /*
         * DNN target
         */

        { "dnn::DNN_TARGET_CPU", cv::dnn::DNN_TARGET_CPU },
        { "dnn::DNN_TARGET_OPENCL", cv::dnn::DNN_TARGET_OPENCL },
        { "dnn::DNN_TARGET_OPENCL_FP16", cv::dnn::DNN_TARGET_OPENCL_FP16 },
        { "dnn::DNN_TARGET_MYRIAD", cv::dnn::DNN_TARGET_MYRIAD },
        { "dnn::DNN_TARGET_VULKAN", cv::dnn::DNN_TARGET_VULKAN },
        { "dnn::DNN_TARGET_CUDA", cv::dnn::DNN_TARGET_CUDA },
        { "dnn::DNN_TARGET_CUDA_FP16", cv::dnn::DNN_TARGET_CUDA_FP16 },
        { "dnn::DNN_TARGET_HDDL", cv::dnn::DNN_TARGET_HDDL },
#endif
    };

    for (size_t i = 0; i < sizeof(cvConsts) / sizeof(cvConsts[0]); i++) {
        strcpy(nameBuffer + nsLength, cvConsts[i].name);
        Tcl_ObjSetVar2(interp, Tcl_NewStringObj(nameBuffer, -1), NULL,
                       Tcl_NewIntObj(cvConsts[i].value), TCL_NAMESPACE_ONLY);
    }

    return TCL_OK;
}
#ifdef __cplusplus
}
#endif  /* __cplusplus */
