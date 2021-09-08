#ifndef _OPENCV
#define _OPENCV

#include <opencv2/opencv.hpp>
#include <exception>

#ifdef __cplusplus
extern "C" {
#endif

#include <tcl.h>

extern Tcl_HashTable *cv_hashtblPtr;
extern size_t matrix_count;
extern int video_count;
extern int callback_count;
extern int detect_count;
extern int term_count;

extern Tcl_Mutex myMutex;

#define NS "cv"

/*
 * OpenCV core
 */
int MATRIX_FUNCTION(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int tcl_CV_8UC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int tcl_CV_8SC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int tcl_CV_16UC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int tcl_CV_16SC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int tcl_CV_32SC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int tcl_CV_32FC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int tcl_CV_64FC(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_mat(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_matwithdims(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_diag(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_eye(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_ones(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_zeros(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_abs(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_absdiff(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_add(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_addWeighted(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_bitwise_and(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_bitwise_or(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_bitwise_xor(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_bitwise_not(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_cartToPolar(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_convertScaleAbs(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_copyMakeBorder(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_divide(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_flip(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_getOptimalDFTSize(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_dft(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_inRange(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_log(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_lut(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_magnitude(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_meanStdDev(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_minMaxLoc(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_multiply(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_mulSpectrums(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_split(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_merge(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_norm(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_normalize(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_pow(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_randu(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_subtract(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int mat_sum(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int perspectiveTransform(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int getTickCount(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int getTickFrequency(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int TermCriteria(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV imgcodecs
 */
int imread(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int imwrite(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV imgproc
 */

int applyColorMap(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int cvtColor(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int calcBackProject(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int calcHist(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int equalizeHist(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int floodFill(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int grabCut(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int matchTemplate(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int getRotationMatrix2D(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int getRectSubPix(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int remap(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int resize(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int threshold(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int adaptiveThreshold(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int getAffineTransform(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int warpAffine(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int getPerspectiveTransform(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int warpPerspective(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int filter2D(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int getGaborKernel(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int blur(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int GaussianBlur(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int medianBlur(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int bilateralFilter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int boxFilter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int sqrBoxFilter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int getStructuringElement(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int dilate(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int erode(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int morphologyEx(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int pyrUp(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int pyrDown(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int pyrMeanShiftFiltering(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int Canny(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int Sobel(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int Scharr(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int Laplacian(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int distanceTransform(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int connectedComponents(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int watershed(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int goodFeaturesToTrack(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int cornerHarris(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int cornerSubPix(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int HoughCircles(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int HoughLines(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int HoughLinesP(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int findContours(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int drawContours(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int contourArea(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int boundingRect(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int minAreaRect(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int boxPoints(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int minEnclosingCircle(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int convexHull(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int arrowedLine(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int circle(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int clipLine(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int drawMarker(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int ellipse(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int fillConvexPoly(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int line(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int polylines(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int putText(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int rectangle(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * For videoio
 */

int VideoCapture(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int VideoWriter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV highgui
 */
int namedWindow(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int imshow(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int waitKey(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int moveWindow(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int destroyWindow(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int destroyAllWindows(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int selectROI(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int setMouseCallback(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int createTrackbar(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int getTrackbarPos(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV features2d
 */

int drawKeypoints(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int drawMatches(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int FastFeatureDetector(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int ORB(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int BFMatcher(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV calib3d
 */

int findHomography(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV video
 */
int BackgroundSubtractorMOG2(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int meanShift(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int CamShift(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int calcOpticalFlowPyrLK(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int calcOpticalFlowFarneback(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV Photo
 */

int inpaint(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int decolor(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int fastNlMeansDenoisingColored(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int colorChange(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int illuminationChange(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int textureFlattening(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int seamlessClone(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int detailEnhance(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int edgePreservingFilter(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int pencilSketch(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int stylization(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int AlignMTB(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int CalibrateDebevec(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int MergeDebevec(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int TonemapDrago(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int TonemapMantiuk(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int TonemapReinhard(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV Stitcher
 */
int Stitcher(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV objdetect
 */
int CascadeClassifier(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

int QRCodeDetector(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);

/*
 * OpenCV dnn
 */
int dnn_blobFromImage(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);
int dnn_readNet(void *cd, Tcl_Interp *interp, int objc,Tcl_Obj *const*objv);


extern DLLEXPORT int	Opencv_Init(Tcl_Interp * interp);

#ifdef __cplusplus
}
#endif

typedef struct MatrixInfo {
    cv::Mat *matrix;
} MatrixInfo;

typedef struct VideoCaptureInfo {
    cv::VideoCapture *capture;
} VideoCaptureInfo;

typedef struct VideoWriterInfo {
    cv::VideoWriter *writer;
} VideoWriterInfo;

typedef struct TermCriteriaInfo {
    cv::TermCriteria *termCriteria;
} TermCriteriaInfo;

typedef struct CascadeClassifierInfo {
    cv::CascadeClassifier *classifier;
} CascadeClassifierInfo;

typedef struct QRCodeDetectorInfo {
    cv::QRCodeDetector *qrcodedetector;
} QRCodeDetectorInfo;

typedef struct DnnNetInfo {
    cv::dnn::Net *net;
} DnnNetInfo;

typedef struct CvCallbackInfo {
    Tcl_Interp *interp;
    char *callback_code;
    int value;
} CvCallbackInfo;

#endif /* _OPENCV */
