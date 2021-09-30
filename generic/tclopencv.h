#ifndef _OPENCV
#define _OPENCV

#include <opencv2/opencv.hpp>
#include <exception>

#ifdef __cplusplus
extern "C" {
#endif

#include <tcl.h>

#define NS "cv"

#ifndef MODULE_SCOPE
#define MODULE_SCOPE
#endif

/*
 * OpenCV core
 */

MODULE_SCOPE int tcl_CV_8UC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int tcl_CV_8SC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int tcl_CV_16UC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int tcl_CV_16SC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int tcl_CV_32SC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int tcl_CV_32FC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int tcl_CV_64FC(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_mat(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_matwithdims(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_diag(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_eye(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_ones(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_zeros(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_abs(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_absdiff(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_add(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_addWeighted(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_bitwise_and(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_bitwise_or(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_bitwise_xor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_bitwise_not(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_calcCovarMatrix(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_cartToPolar(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_compare(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_convertScaleAbs(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_copyMakeBorder(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_countNonZero(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_determinant(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_divide(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_eigen(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int mat_eigenNonSymmetric(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif
MODULE_SCOPE int mat_exp(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_extractChannel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_flip(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_getOptimalDFTSize(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_dft(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_inRange(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_log(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_lut(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_Mahalanobis(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_magnitude(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_meanStdDev(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_minMaxIdx(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_minMaxLoc(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_multiply(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_mulSpectrums(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_split(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_merge(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_norm(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_normalize(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_pow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_randn(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_randu(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_randShuffle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_reduce(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_rotate(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_setRNGSeed(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_solve(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_sqrt(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_subtract(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_sum(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_SVBackSubst(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_SVDecomp(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_trace(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_transform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_hconcat(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int mat_vconcat(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int kmeans(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int perspectiveTransform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int getBuildInformation(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getTickCount(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getTickFrequency(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int PCA(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int TermCriteria(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV fstorage
 */

MODULE_SCOPE int FileStorage(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV imgcodecs
 */

MODULE_SCOPE int imread(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int imdecode(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int imwrite(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int imencode(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV imgproc
 */

MODULE_SCOPE int applyColorMap(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int cvtColor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int calcBackProject(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int calcHist(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int compareHist(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int equalizeHist(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int EMD(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int floodFill(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int grabCut(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int matchTemplate(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int moments(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getRotationMatrix2D(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getRectSubPix(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int HuMoments(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int integral(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int remap(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int resize(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int threshold(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int adaptiveThreshold(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getAffineTransform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int warpAffine(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getPerspectiveTransform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int warpPerspective(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int warpPolar(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int filter2D(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int sepFilter2D(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getGaborKernel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getGaussianKernel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int blur(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int GaussianBlur(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int medianBlur(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int bilateralFilter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int boxFilter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int sqrBoxFilter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int getStructuringElement(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int dilate(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int erode(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int morphologyEx(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int pyrUp(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int pyrDown(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int pyrMeanShiftFiltering(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int createHanningWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int phaseCorrelate(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int Canny(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int Sobel(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int Scharr(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int Laplacian(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int distanceTransform(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int connectedComponents(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int connectedComponentsWithStats(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int watershed(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int goodFeaturesToTrack(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int cornerHarris(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int cornerSubPix(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int HoughCircles(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int HoughLines(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int HoughLinesP(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int findContours(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int drawContours(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int contourArea(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int boundingRect(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int minAreaRect(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int fitEllipse(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int fitLine(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int boxPoints(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int minEnclosingCircle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int convexHull(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int arrowedLine(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int circle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int clipLine(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int drawMarker(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int ellipse(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int fillConvexPoly(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int fillPoly(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int line(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int polylines(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int putText(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int rectangle(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int CLAHE(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * For videoio
 */

MODULE_SCOPE int VideoCapture(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int VideoWriter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV highgui
 */
MODULE_SCOPE int namedWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int imshow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int waitKey(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int moveWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int resizeWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int destroyWindow(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int destroyAllWindows(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int selectROI(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif
MODULE_SCOPE int setMouseCallback(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int createTrackbar(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getTrackbarPos(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV features2d
 */

MODULE_SCOPE int drawKeypoints(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int drawMatches(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int FastFeatureDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int AgastFeatureDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int MSER(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int ORB(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int AKAZE(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int BRISK(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int KAZE(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_SIFT
MODULE_SCOPE int SIFT(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif
MODULE_SCOPE int BFMatcher(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int FlannBasedMatcher(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int SimpleBlobDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int BOWKMeansTrainer(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int BOWImgDescriptorExtractor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV calib3d
 */

MODULE_SCOPE int findChessboardCorners(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int drawChessboardCorners(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int calibrateCamera(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int getOptimalNewCameraMatrix(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int undistort(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int initUndistortRectifyMap(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int projectPoints(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int solvePnP(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int computeCorrespondEpilines(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int findFundamentalMat(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int findHomography(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int StereoBM(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int StereoSGBM(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV video
 */

MODULE_SCOPE int BackgroundSubtractorMOG2(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int meanShift(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int CamShift(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int calcOpticalFlowPyrLK(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int calcOpticalFlowFarneback(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV Photo
 */

MODULE_SCOPE int inpaint(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int decolor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int fastNlMeansDenoising(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int fastNlMeansDenoisingColored(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int colorChange(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int illuminationChange(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int textureFlattening(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int seamlessClone(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int detailEnhance(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int edgePreservingFilter(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int pencilSketch(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int stylization(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int AlignMTB(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int CalibrateDebevec(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int MergeDebevec(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int MergeMertens(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int TonemapDrago(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int TonemapMantiuk(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int TonemapReinhard(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV Stitcher
 */

MODULE_SCOPE int Stitcher(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV ml
 */

MODULE_SCOPE int LogisticRegression(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int LogisticRegression_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int NormalBayesClassifier(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int NormalBayesClassifier_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int KNearest(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int KNearest_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int SVM(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int SVM_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int SVMSGD(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int SVMSGD_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int DTrees(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int DTrees_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int Boost(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int Boost_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int RTrees(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int RTrees_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int ANN_MLP(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int ANN_MLP_load(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

MODULE_SCOPE int TrainData(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

/*
 * OpenCV objdetect
 */

MODULE_SCOPE int CascadeClassifier(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

MODULE_SCOPE int HOGDescriptor(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int QRCodeDetector(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

/*
 * OpenCV dnn
 */

#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int dnn_blobFromImage(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int dnn_readNet(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif

/*
 * Thread support
 */

MODULE_SCOPE int Opencv_Tsend(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int Opencv_Trecv(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int Opencv_Tinfo(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);

extern DLLEXPORT int Opencv_Init(Tcl_Interp * interp);

#ifdef __cplusplus
}
#endif


typedef enum {
    OPENCV_CALLBACK = 0,
    OPENCV_MAT,
    OPENCV_FSTORAGE,
    OPENCV_VIDEOCAPTURE,
    OPENCV_VIDEOWRITER,
    OPENCV_PCA,
    OPENCV_TERMCRITERIA,
    OPENCV_ODETECT,
    OPENCV_OOBJHOG,
#ifdef TCL_USE_OPENCV4
    OPENCV_NDETECT,
    OPENCV_QDETECT,
#endif
    OPENCV_BOWTRAINER,
    OPENCV_BOWEXTRACTOR,
    OPENCV_MAXTYPE
} Opencv_Type;


typedef struct {
    Tcl_HashTable tbl[OPENCV_MAXTYPE];

    cv::Ptr<cv::CLAHE> clahe;
    cv::Ptr<cv::FastFeatureDetector> fastdetector;
    cv::Ptr<cv::AgastFeatureDetector> agastdetector;
    cv::Ptr<cv::MSER> mserextractor;
    cv::Ptr<cv::ORB> orbdetector;
    cv::Ptr<cv::AKAZE> akazedetector;
    cv::Ptr<cv::BRISK> briskdetector;
    cv::Ptr<cv::KAZE> kazedetector;
#ifdef TCL_USE_SIFT
    cv::Ptr<cv::SIFT> siftdetector;
#endif
    cv::Ptr<cv::BFMatcher> bfmatcher;
    cv::Ptr<cv::FlannBasedMatcher> flannbasedmatcher;
    cv::Ptr<cv::BackgroundSubtractorMOG2> bgsmog2;
    cv::Ptr<cv::Stitcher> stitcher;
    cv::Ptr<cv::SimpleBlobDetector> sbdetector;
    cv::Ptr<cv::StereoBM> stereobm;
    cv::Ptr<cv::StereoSGBM> stereosgbm;
    cv::Ptr<cv::AlignMTB> alignmtb;
    cv::Ptr<cv::CalibrateDebevec> calibdeb;
    cv::Ptr<cv::MergeDebevec> mergedeb;
    cv::Ptr<cv::MergeMertens> mergemer;
    cv::Ptr<cv::TonemapDrago> tonemapdra;
    cv::Ptr<cv::TonemapMantiuk> tonemapman;
    cv::Ptr<cv::TonemapReinhard> tonemaprei;
    cv::Ptr<cv::ml::LogisticRegression> logistic;
    cv::Ptr<cv::ml::NormalBayesClassifier> bayesclassifier;
    cv::Ptr<cv::ml::KNearest> knearest;
    cv::Ptr<cv::ml::SVM> svm;
    cv::Ptr<cv::ml::SVMSGD> svmsgd;
    cv::Ptr<cv::ml::DTrees> dtrees;
    cv::Ptr<cv::ml::Boost> boost;
    cv::Ptr<cv::ml::RTrees> rtrees;
    cv::Ptr<cv::ml::ANN_MLP> annmlp;
    cv::Ptr<cv::ml::TrainData> traindata;

    Tcl_Command cmd_clahe;
    Tcl_Command cmd_fastdetector;
    Tcl_Command cmd_agastdetector;
    Tcl_Command cmd_mserextractor;
    Tcl_Command cmd_orbdetector;
    Tcl_Command cmd_akazedetector;
    Tcl_Command cmd_briskdetector;
    Tcl_Command cmd_kazedetector;
#ifdef TCL_USE_SIFT
    Tcl_Command cmd_siftdetector;
#endif
    Tcl_Command cmd_bfmatcher;
    Tcl_Command cmd_flannbasedmatcher;
    Tcl_Command cmd_bgsmog2;
    Tcl_Command cmd_stitcher;
    Tcl_Command cmd_sbdetector;
    Tcl_Command cmd_stereobm;
    Tcl_Command cmd_stereosgbm;
    Tcl_Command cmd_alignmtb;
    Tcl_Command cmd_calibdeb;
    Tcl_Command cmd_mergedeb;
    Tcl_Command cmd_mergemer;
    Tcl_Command cmd_tonemapdra;
    Tcl_Command cmd_tonemapman;
    Tcl_Command cmd_tonemaprei;
    Tcl_Command cmd_logistic;
    Tcl_Command cmd_bayesclassifier;
    Tcl_Command cmd_knearest;
    Tcl_Command cmd_svm;
    Tcl_Command cmd_svmsgd;
    Tcl_Command cmd_dtrees;
    Tcl_Command cmd_boost;
    Tcl_Command cmd_rtrees;
    Tcl_Command cmd_annmlp;
    Tcl_Command cmd_traindata;

#ifdef TCL_USE_TKPHOTO
    int tkCheck;
#endif
} Opencv_Data;


typedef struct {
    Opencv_Type type;
    Opencv_Data *top;
    char *key;
    void *obj;
    Tcl_Command cmd;
    int flags;             /* for OPENCV_FSTORAGE */
    Tcl_DString ds1, ds2;  /* for OPENCV_FSTORAGE */
} Opencv_Obj;


#ifdef __cplusplus
extern "C" {
#endif

MODULE_SCOPE int Opencv_Exc2Tcl(Tcl_Interp *interp, const cv::Exception *ex);
MODULE_SCOPE int Opencv_SetResult(Tcl_Interp *interp, int code, const char *msg);
MODULE_SCOPE Tcl_Obj *Opencv_NewHandle(void *cd, Tcl_Interp *interp, Opencv_Type type, void *obj);
MODULE_SCOPE void *Opencv_FindHandle(void *cd, Tcl_Interp *interp, Opencv_Type type, Tcl_Obj *name);
#ifdef TCL_USE_TKPHOTO
MODULE_SCOPE int Opencv_CheckForTk(void *cd, Tcl_Interp *interp);
#endif

/*
 * Instance handlers and callbacks.
 */

MODULE_SCOPE void CallBackFunc(int event, int x, int y, int flags, void *userdata);
MODULE_SCOPE int CascadeClassifier_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int HOGDescriptor_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int FileStorage_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int MATRIX_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int QRCodeDetector_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif
#ifdef TCL_USE_OPENCV4
MODULE_SCOPE int READNET_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
#endif
MODULE_SCOPE int BOWKMeansTrainer_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int BOWImgDescriptorExtractor_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int PCA_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int TERM_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE void TrackbarCallback(int value, void *userdata);
MODULE_SCOPE int VideoCapture_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int VideoWriter_FUNCTION(void *cd, Tcl_Interp *interp, int objc, Tcl_Obj *const*objv);
MODULE_SCOPE int TrainData_CONSTRUCTOR(void *cd, Tcl_Interp *interp, cv::Mat &samples, int layout, cv::Mat &responses);

#ifdef __cplusplus
}
#endif

typedef struct CvCallbackInfo {
    Tcl_Interp *interp;
    char *callback_code;
    int value;
} CvCallbackInfo;

#endif /* _OPENCV */
