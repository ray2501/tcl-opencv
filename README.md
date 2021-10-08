tcl-opencv
=====

This project is a Tcl extension for [OpenCV](https://opencv.org/) library.

OpenCV (Open Source Computer Vision Library) is a library of programming
functions mainly aimed at real-time computer vision.

This extension requires OpenCV 4.5.0 or newer.


Building this extension
=====

See [BUILDING.md](BUILDING.md) to know how to build the extension.


Implement commands
=====

### Matrix

    MATRIX dims
    MATRIX size
    MATRIX rows
    MATRIX cols
    MATRIX channels
    MATRIX depth
    MATRIX type
    MATRIX empty
    MATRIX at index_list channel ?value?
    MATRIX inv ?method?
    MATRIX dot matrix
    MATRIX cross matrix
    MATRIX * matrix
    MATRIX add value
    MATRIX subtract value
    MATRIX multiply value
    MATRIX divide value
    MATRIX transpose
    MATRIX diag ?d?
    MATRIX crop x y width height
    MATRIX rect x y width height
    MATRIX copyTo matrix ?mask?
    MATRIX convertTo type ?scale shift?
    MATRIX colRange startcol endcol
    MATRIX rowRange startcol endcol
    MATRIX pop_back ?nelems?
    MATRIX push_back matrix
    MATRIX reshape cn ?rows?
    MATRIX setData list_data
    MATRIX setTo color_list ?mask?
    MATRIX toByteArray
    MATRIX fromByteArray width height bpp bytes
    MATRIX close

    ::cv::CV_8UC num
    ::cv::CV_8SC num
    ::cv::CV_16UC num
    ::cv::CV_16SC num
    ::cv::CV_32SC num
    ::cv::CV_32FC num
    ::cv::CV_64FC num

    ::cv::Mat::Mat rows cols type ?color_list?
    ::cv::Mat::MatWithDims dims size_list type ?color_list?
    ::cv::Mat::diag matrix
    ::cv::Mat::eye rows cols type
    ::cv::Mat::ones rows cols type
    ::cv::Mat::zeros rows cols type

    ::cv::abs matrix
    ::cv::absdiff matrix_1 matrix_2
    ::cv::add matrix_1 matrix_2
    ::cv::addWeighted matrix_1 alpha matrix_2 beta gamma
    ::cv::bitwise_and matrix_1 matrix_2 ?mask?
    ::cv::bitwise_or matrix_1 matrix_2 ?mask?
    ::cv::bitwise_xor matrix_1 matrix_2 ?mask?
    ::cv::bitwise_not matrix ?mask?
    ::cv::calcCovarMatrix matrix flags ?ctype?
    ::cv::cartToPolar matrix_1 matrix_2 ?angleInDegrees?
    ::cv::compare matrix_1 matrix_2 cmpop
    ::cv::convertScaleAbs matrix alpha beta
    ::cv::copyMakeBorder matrix top bottom left right borderType ?color_list?
    ::cv::countNonZero matrix
    ::cv::determinant matrix
    ::cv::divide matrix_1 matrix_2 ?scale?
    ::cv::eigen matrix
    ::cv::eigenNonSymmetric matrix
    ::cv::exp matrix
    ::cv::extractChannel matrix coi
    ::cv::flip matrix flipCode
    ::cv::getOptimalDFTSize vecsize
    ::cv::dft matrix ?falgs nonzeroRows?
    ::cv::inRange matrix color_list1 color_list2
    ::cv::insertChannel src_matrix dst_matrix coi
    ::cv::log matrix
    ::cv::LUT matrix1 matrix2
    ::cv::Mahalanobis matrix1 matrix2 icovar_matrix
    ::cv::magnitude matrix1 matrix2
    ::cv::max matrix1 matrix2
    ::cv::meanStdDev matrix
    ::cv::min matrix1 matrix2
    ::cv::minMaxIdx matrix
    ::cv::minMaxLoc matrix
    ::cv::multiply matrix_1 matrix_2 ?scale?
    ::cv::mulSpectrums matrix_1 matrix_2 flags ?conjB?
    ::cv::split matrix
    ::cv::merge matrix_list
    ::cv::norm matrix norm_type
    ::cv::norm matrix_1 matrix_2 norm_type
    ::cv::normalize matrix alpha beta norm_type
    ::cv::pow matrix power
    ::cv::randn matrix mean_list stddev_list
    ::cv::randu matrix min_list max_list
    ::cv::randShuffle matrix ?iterFactor?
    ::cv::reduce matrix dim rtype ?dtype?
    ::cv::repeat matrix ny nx
    ::cv::rotate matrix rotateCode
    ::cv::setRNGSeed seed
    ::cv::solve matrix_1 matrix_2 ?flags?
    ::cv::solveCubic matrix
    ::cv::solvePoly matrix ?maxIters?
    ::cv::sortIdx matrix flags
    ::cv::sqrt matrix
    ::cv::subtract matrix_1 matrix_2
    ::cv::sum matrix
    ::cv::SVBackSubst matrix_w matrix_u matrix_vt matrix
    ::cv::SVDecomp matrix ?flags?
    ::cv::trace matrix
    ::cv::transform matrix_1 matrix_2
    ::cv::hconcat matrix_1 matrix_2
    ::cv::vconcat matrix_1 matrix_2

    ::cv::kmeans matrix k bestLabels termCriteria attempts flags

If users setup bestLabels to `None`, then flags should not set to `$::cv::KMEANS_USE_INITIAL_LABELS`.
If flags is set to `$::cv::KMEANS_USE_INITIAL_LABELS`, users should provide a CV_32S matrix.

    ::cv::perspectiveTransform src_list transformation_matrix

    ::cv::getBuildInformation
    ::cv::getTickCount
    ::cv::getTickFrequency

    ::cv::PCA matrix flags ?maxComponents?
    PCA mean
    PCA eigenvalues
    PCA eigenvectors
    PCA backProject matrix
    PCA project matrix
    PCA close

    ::cv::TermCriteria ?type maxCount epsilon?
    TermCriteria close

    ::cv::fromByteArray width height bpp bytes

Creates a new `cv::Mat` from a gray (`bpp` equals 1) or RGB (`bpp` equals 3) byte array with `width` columns and `height` rows.
Same rule regarding arguments applies to `MATRIX fromByteArray ...`.
Likewise, the command `MATRIX toByteArray` returns a 4 element result list.

### fstorage

    ::cv::FileStorage
    FS open filename|data mode
    FS keys ?name ...?
    FS startMap name
    FS startSeq name
    FS endMap
    FS endSeq
    FS readDouble name ...
    FS readInt name ...
    FS readMat name ...
    FS readObj name ...
    FS readString name ...
    FS writeDouble name double ...
    FS writeInt name int ...
    FS writeMat name matrix
    FS writeObj name obj
    FS writeString name string ...
    FS close

For more FileStorage info, you can check
[XML/YAML Persistence](https://docs.opencv.org/master/d4/da4/group__core__xml.html).

### imgcodecs

    ::cv::imread filename ?flags?
    ::cv::imdecode bytes ?flags?
    ::cv::imwrite filename matrix
    ::cv::imencode fileext matrix

### imgproc

    ::cv::applyColorMap matrix colormap
    ::cv::cvtColor src_matrix code ?dstCn?
    ::cv::calcBackProject matrix channels hist_matrix ranges_list ?scale uniform?
    ::cv::calcHist matrix channels mask dims histSize_list ranges_list ?uniform accumulate?
    ::cv::compareHist matrix_1 matrix_2 method
    ::cv::equalizeHist matrix
    ::cv::EMD signature1 signature2 distType ?cost?
    ::cv::floodFill matrix seed_x seed_y color_list ?rect_list loDiff_color upDiff_color flags?
    ::cv::grabCut matrix x y width height iterCount
    ::cv::matchTemplate matrix templ_matrix method
    ::cv::moments matrix ?binaryImage?
    ::cv::getRotationMatrix2D x y angle scale
    ::cv::getRectSubPix matrix width height center_x center_y
    ::cv::HuMoments moments_list
    ::cv::integral matrix ?sdepth sqdepth?
    ::cv::remap matrix map1 map2 interpolation
    ::cv::resize matrix width height ?flags?
    ::cv::threshold matrix thresh maxval type
    ::cv::adaptiveThreshold matrix maxValue adaptiveMethod thresholdType blockSize C
    ::cv::getAffineTransform src_list dst_list
    ::cv::warpAffine src_matrix transformation_matrix width height ?flags?
    ::cv::getPerspectiveTransform src_list dst_list ?solveMethod?
    ::cv::warpPerspective src_matrix transformation_matrix width height ?flags?
    ::cv::warpPolar matrix dsize_width dsize_height center_x center_y maxRadius ?flags?

    ::cv::filter2D src_matrix kernel_matrix ?anchor_x anchor_y delta borderType?
    ::cv::sepFilter2D src_matrix kernelX kernelY ?anchor_x anchor_y delta borderType?
    ::cv::getGaborKernel ksize_width ksize_height sigma theta lambd gamma ?psi type?
    ::cv::getGaussianKernel ksize sigma ?type?
    ::cv::blur src_matrix ksize_width ksize_height ?anchor_x anchor_y borderType?
    ::cv::GaussianBlur src_matrix ksize_width ksize_height sigmaX ?sigmaY borderType?
    ::cv::medianBlur src_matrix ksize
    ::cv::bilateralFilter src_matrix d sigmaColor sigmaSpace ?borderType?
    ::cv::boxFilter src_matrix ksize_width ksize_height ?anchor_x anchor_y normalize borderType?
    ::cv::sqrBoxFilter src_matrix ksize_width ksize_height ?anchor_x anchor_y normalize borderType?

    ::cv::getStructuringElement shape ksize_width ksize_height ?anchor_x anchor_y?
    ::cv::dilate src_matrix kernel_matrix ?anchor_x anchor_y iterations?
    ::cv::erode src_matrix kernel_matrix ?anchor_x anchor_y iterations?
    ::cv::morphologyEx src_matrix op kernel_matrix ?anchor_x anchor_y iterations?
    ::cv::buildPyramid src_matrix maxlevel ?borderType?
    ::cv::pyrUp src_matrix ?dstsize_width dstsize_height borderType?
    ::cv::pyrDown src_matrix ?dstsize_width dstsize_height borderType?
    ::cv::pyrMeanShiftFiltering src_matrix sp sr ?maxLevel?

    ::cv::createHanningWindow winSize_width winSize_height type
    ::cv::phaseCorrelate matrix_1 matrix_2 ?window?

    ::cv::Canny matrix threshold1 threshold2 ?apertureSize L2gradient?
    ::cv::Sobel matrix dx dy ?ksize scale delta borderType?
    ::cv::Scharr matrix dx dy ?scale delta borderType?
    ::cv::Laplacian matrix ?ksize scale delta borderType?
    ::cv::distanceTransform matrix distanceType maskSize
    ::cv::connectedComponents matrix ?connectivity?
    ::cv::connectedComponentsWithStats matrix ?connectivity?
    ::cv::watershed matrix markers
    ::cv::goodFeaturesToTrack matrix maxCorners qualityLevel minDistance ?mask blockSize useHarrisDetector k?
    ::cv::cornerHarris matrix blockSize ksize k ?borderType?
    ::cv::cornerEigenValsAndVecs matrix blockSize ksize ?borderType?
    ::cv::cornerMinEigenVal matrix blockSize ksize ?borderType?
    ::cv::cornerSubPix matrix corners winSize_width winSize_height zeroZone_widht zeroZone_height termCriteria
    ::cv::HoughCircles matrix method dp minDist ?param1 param2 minRadius maxRadius?
    ::cv::HoughLines matrix rho theta threshold ?srn stn min_theta max_theta?
    ::cv::HoughLinesP matrix rho theta threshold ?minLineLength maxLineGap?

    ::cv::findContours matrix mode method ?offset_point_x offset_point_y?
    ::cv::findContoursWithHierarchy matrix mode method ?offset_point_x offset_point_y?
    ::cv::drawContours matrix contours_list contourIdx color_list thickness ?lineType maxLevel offset_point_x offset_point_y?
    ::cv::drawContoursWithHierarchy matrix contours_list contourIdx color_list thickness lineType hierarchy maxLevel ?offset_point_x offset_point_y?
    ::cv::approxPolyDP contour epsilon closed
    ::cv::arcLength contour closed
    ::cv::contourArea contour ?oriented?
    ::cv::boundingRect contour
    ::cv::minAreaRect contour
    ::cv::fitEllipse contour
    ::cv::fitLine contour distType param reps aeps
    ::cv::boxPoints contour
    ::cv::minEnclosingCircle contour
    ::cv::convexHull contour ?clockwise returnPoints?
    ::cv::convexityDefects contour convexhull
    ::cv::matchShapes contour1 contour2 method
    ::cv::pointPolygonTest contour x y measureDist

If users want to find the convexity defects of a contour,
convex hull obtained using `::cv::convexHull` that required to set returnPoints to 0.

If users need to know contour's moments, users need to use `::cv::Mat::Mat` to create
a matrix and fill the contour's data to the matrix, then use `::cv::moments` to calculate.

    ::cv::arrowedLine matrix point_x1 point_y1 point_x2 point_y2 color_list thickness ?lineType shift tipLength?
    ::cv::circle matrix center_x center_y radius color_list thickness ?lineType shift?
    ::cv::clipLine size_list point1_list point2_list
    ::cv::drawMarker matrix point_x point_y color_list ?markerType markerSize thickness line_type?
    ::cv::ellipse matrix center_x center_y width hgieht angle startAngle endAngle color_list thickness ?lineType shift?
    ::cv::fillConvexPoly matrix point_list color_list ?lineType shift?
    ::cv::fillPoly matrix point_lists color_list ?lineType shift offset_x offset_y?
    ::cv::line matrix point_x1 point_y1 point_x2 point_y2 color_list thickness ?lineType shift?
    ::cv::polylines matrix point_list ncontours isClosed color_list thickness ?lineType shift?
    ::cv::putText matrix text point_x point_y fontFace fontScale color_list thickness ?lineType bottomLeftOrigin?
    ::cv::rectangle matrix point_x1 point_y1 point_x2 point_y2 color_list thickness ?lineType shift?

    ::cv::CLAHE ?clipLimit tileGridSize_w tileGridSize_h?
    CLAHE apply matrix
    CLAHE close

Please notice, CLAHE command will only have 1 instance.

    ::cv::GeneralizedHoughBallard
    GeneralizedHoughBallard detect matrix
    GeneralizedHoughBallard getCannyHighThresh
    GeneralizedHoughBallard getCannyLowThresh
    GeneralizedHoughBallard getDp
    GeneralizedHoughBallard getLevels
    GeneralizedHoughBallard getMinDist
    GeneralizedHoughBallard getVotesThreshold
    GeneralizedHoughBallard setCannyHighThresh
    GeneralizedHoughBallard setCannyLowThresh value
    GeneralizedHoughBallard setDp value
    GeneralizedHoughBallard setLevels value
    GeneralizedHoughBallard setMinDist value
    GeneralizedHoughBallard setTemplate matrix
    GeneralizedHoughBallard setVotesThreshold value
    GeneralizedHoughBallard close

Please notice, GeneralizedHoughBallard command will only have 1 instance.

    ::cv::GeneralizedHoughGuil
    GeneralizedHoughGuil detect matrix
    GeneralizedHoughGuil getAngleEpsilon
    GeneralizedHoughGuil getAngleStep
    GeneralizedHoughGuil getAngleThresh
    GeneralizedHoughGuil getCannyHighThresh
    GeneralizedHoughGuil getCannyLowThresh
    GeneralizedHoughGuil getDp
    GeneralizedHoughGuil getLevels
    GeneralizedHoughGuil getMaxAngle
    GeneralizedHoughGuil getMaxScale
    GeneralizedHoughGuil getMinDist
    GeneralizedHoughGuil getMinAngle
    GeneralizedHoughGuil getMinScale
    GeneralizedHoughGuil getPosThresh
    GeneralizedHoughGuil getScaleStep
    GeneralizedHoughGuil getScaleThresh
    GeneralizedHoughGuil getXi
    GeneralizedHoughGuil setAngleEpsilon value
    GeneralizedHoughGuil setAngleStep value
    GeneralizedHoughGuil setAngleThresh value
    GeneralizedHoughGuil setCannyHighThresh value
    GeneralizedHoughGuil setCannyLowThresh value
    GeneralizedHoughGuil setDp value
    GeneralizedHoughGuil setLevels value
    GeneralizedHoughGuil setMaxAngle value
    GeneralizedHoughGuil setMaxScale value
    GeneralizedHoughGuil setMinDist value
    GeneralizedHoughGuil setMinAngle value
    GeneralizedHoughGuil setMinScale value
    GeneralizedHoughGuil setPosThresh value
    GeneralizedHoughGuil setScaleStep value
    GeneralizedHoughGuil setScaleThresh value
    GeneralizedHoughGuil setXi value
    GeneralizedHoughGuil setTemplate matrix
    GeneralizedHoughGuil close

Please notice, GeneralizedHoughGuil command will only have 1 instance.

### videoio

    ::cv::VideoCapture file/index filename/number ?flags?
    VideoCapture isOpened
    VideoCapture read
    VideoCapture get propId
    VideoCapture set propId value
    VideoCapture close

`::cv::VideoCapture` is using to open video file or open a camera for video
capturing. Second argument is `file` or `index`, then the third arugment
is filename or camera id.

    ::cv::VideoWriter filename fourcc fps width height ?isColor?
    VideoWriter isOpened
    VideoWriter write matrix
    VideoWriter get propId
    VideoWriter set propId value
    VideoWriter close

### highgui

    ::cv::namedWindow winname ?flags?
    ::cv::imshow winname matrix
    ::cv::waitKey delay
    ::cv::moveWindow winname x y
    ::cv::resizeWindow winname width height
    ::cv::destroyWindow winname
    ::cv::destroyAllWindows
    ::cv::selectROI matrix ?showCrosshair fromCenter?
    ::cv::setMouseCallback winname callback_code
    ::cv::createTrackbar trackbarname winname init_value max_value callback_code
    ::cv::getTrackbarPos trackbarname winname

### features2d

    ::cv::drawKeypoints matrix keypoints dst_image color_list ?flags?

    ::cv::drawMatches matrix1 keypoints1 matrix2 keypoints2 matches1to2 dst_image matchColor_list singlePointColor_list ?flags?

If users setup dst_image to `None`, then flags should not set to `$::cv::DRAW_MATCHES_FLAGS_DRAW_OVER_OUTIMG`.
If flags is set to `$::cv::DRAW_MATCHES_FLAGS_DRAW_OVER_OUTIMG`, users should provide a dst matrix.

    ::cv::FastFeatureDetector ?threshold nonmaxSuppression type?
    FastFeatureDetector detect matrix
    FastFeatureDetector getNonmaxSuppression
    FastFeatureDetector getThreshold
    FastFeatureDetector getType
    FastFeatureDetector setNonmaxSuppression value
    FastFeatureDetector setThreshold value
    FastFeatureDetector setType value
    FastFeatureDetector close

Please notice, FastFeatureDetector command will only have 1 instance.

    ::cv::AgastFeatureDetector ?threshold nonmaxSuppression type?
    AgastFeatureDetector detect matrix
    AgastFeatureDetector getNonmaxSuppression
    AgastFeatureDetector getThreshold
    AgastFeatureDetector getType
    AgastFeatureDetector setNonmaxSuppression value
    AgastFeatureDetector setThreshold value
    AgastFeatureDetector setType value
    AgastFeatureDetector close

Please notice, AgastFeatureDetector command will only have 1 instance.

    ::cv::MSER ?delta min_area max_area max_variation min_diversity max_evolution area_threshold min_margin edge_blur_size?
    MSER detectRegions matrix
    MSER getDelta
    MSER getMaxArea
    MSER getMinArea
    MSER getPass2Only
    MSER setDelta value
    MSER setMaxArea value
    MSER setMinArea value
    MSER setPass2Only value
    MSER close

Please notice, MSER command will only have 1 instance.

    ::cv::ORB ?nfeatures scaleFactor nlevels edgeThreshold firstLevel WTA_K scoreType patchSize fastThreshold?
    ORB detect matrix
    ORB compute matrix keypoints
    ORB detectAndCompute matrix
    ORB getEdgeThreshold
    ORB getFastThreshold
    ORB getFirstLevel
    ORB getMaxFeatures
    ORB getNLevels
    ORB getPatchSize
    ORB getScaleFactor
    ORB getScoreType
    ORB getWTA_K
    ORB setEdgeThreshold value
    ORB setFastThreshold value
    ORB setFirstLevel value
    ORB setMaxFeatures value
    ORB setNLevels value
    ORB setPatchSize value
    ORB setScaleFactor value
    ORB setScoreType value
    ORB setWTA_K value
    ORB close

Please notice, ORB command will only have 1 instance.

    ::cv::AKAZE ?descriptor_type descriptor_size descriptor_channels threshold nOctaves nOctaveLayers diffusivity?
    AKAZE detect matrix
    AKAZE compute matrix keypoints
    AKAZE detectAndCompute matrix
    AKAZE getDescriptorChannels
    AKAZE getDescriptorSize
    AKAZE getDescriptorType
    AKAZE getDiffusivity
    AKAZE getNOctaveLayers
    AKAZE getNOctaves
    AKAZE getThreshold
    AKAZE setDescriptorChannels value
    AKAZE setDescriptorSize value
    AKAZE setDescriptorType value
    AKAZE setDiffusivity value
    AKAZE setNOctaveLayers value
    AKAZE setNOctaves value
    AKAZE setThreshold value
    AKAZE close

Please notice, AKAZE command will only have 1 instance.

    ::cv::BRISK ?thresh octaves patternScale?
    BRISK detect matrix
    BRISK compute matrix keypoints
    BRISK detectAndCompute matrix
    BRISK close

Please notice, BRISK command will only have 1 instance.

    ::cv::KAZE ?extended upright threshold nOctaves nOctaveLayers diffusivity?
    KAZE detect matrix
    KAZE compute matrix keypoints
    KAZE detectAndCompute matrix
    KAZE getDiffusivity
    KAZE getExtended
    KAZE getNOctaveLayers
    KAZE getNOctaves
    KAZE getThreshold
    KAZE getUpright
    KAZE setDiffusivity value
    KAZE setExtended value
    KAZE setNOctaveLayers value
    KAZE setNOctaves value
    KAZE setThreshold value
    KAZE setUpright value
    KAZE close

Please notice, KAZE command will only have 1 instance.

    ::cv::SIFT ?nfeatures nOctaveLayers contrastThreshold edgeThreshold sigma?
    SIFT detect matrix
    SIFT compute matrix keypoints
    SIFT detectAndCompute matrix
    SIFT close

Please notice, SIFT command will only have 1 instance.

    ::cv::AffineFeature backend ?maxTilt minTilt tiltStep rotateStepBase?
    AffineFeature detect matrix
    AffineFeature compute matrix keypoints
    AffineFeature detectAndCompute matrix
    AffineFeature close

Please notice, AffineFeature command will only have 1 instance.
And backend supports SIFT, KAZE, ORB, AKAZE and BRISK.

    ::cv::BFMatcher normType crossCheck
    BFMatcher match queryDescriptors trainDescriptors
    BFMatcher knnMatch queryDescriptors trainDescriptors k
    BFMatcher close

Please notice, BFMatcher command will only have 1 instance.

    ::cv::FlannBasedMatcher ?algorithm indexParams?
    FlannBasedMatcher match queryDescriptors trainDescriptors
    FlannBasedMatcher knnMatch queryDescriptors trainDescriptors k
    FlannBasedMatcher close

`algorithm` can specify `FLANN_INDEX_LSH` or `FLANN_INDEX_KDTREE`. Default is FLANN_INDEX_LSH.

`indexParams` is a list of LshIndexParams parameters (table_number, key_size, multi_probe_level) or
KDTreeIndexParams parameters (trees).

Please notice, FlannBasedMatcher command will only have 1 instance.

    ::cv::SimpleBlobDetector ?-minThreshold value? ?-maxThreshold value? ?-filterByArea boolean? ?-minArea value? ?-maxArea value? ?-filterByCircularity boolean? ?-minCircularity value? ?-maxCircularity value? ?-filterByConvexity boolean? ?-minConvexity value? ?-maxConvexity value? ?-filterByInertia boolean? ?-minInertiaRatio value? ?-maxInertiaRatio value?
    SimpleBlobDetector detect matrix
    SimpleBlobDetector close

Please notice, SimpleBlobDetector command will only have 1 instance.

    ::cv::BOWKMeansTrainer clusterCount termCriteria ?attempts flags?
    BOWKMeansTrainer add descriptors
    BOWKMeansTrainer clear
    BOWKMeansTrainer cluster
    BOWKMeansTrainer descriptorsCount
    BOWKMeansTrainer getDescriptors
    BOWKMeansTrainer close

    ::cv::BOWImgDescriptorExtractor dextractor dmatcher
    BOWImgDescriptorExtractor compute matrix keypoints
    BOWImgDescriptorExtractor descriptorSize
    BOWImgDescriptorExtractor descriptorType
    BOWImgDescriptorExtractor getVocabulary
    BOWImgDescriptorExtractor setVocabulary vocabulary
    BOWImgDescriptorExtractor close

`::cv::BOWImgDescriptorExtractor` now supports SIFT or KAZE (dextractor) and FlannBasedMatcher (dmatcher).

### calib3d

    ::cv::findChessboardCorners image patternSize_width patternSize_height ?flags?
    ::cv::drawChessboardCorners image patternSize_width patternSize_height corners patternWasFound
    ::cv::calibrateCamera objectPoints_list imagePoints_list width height cameraMatrix distCoeffs
    ::cv::getOptimalNewCameraMatrix cameraMatrix distCoeffs width height alpha nwidth nheight ?centerPrincipalPoint?
    ::cv::undistort matrix cameraMatrix distCoeffs newCameraMatrix
    ::cv::initUndistortRectifyMap cameraMatrix distCoeffs R newCameraMatrix width height m1type
    ::cv::projectPoints objectPoints rvec tvec cameraMatrix distCoeffs
    ::cv::solvePnP objectPoints imagePoints cameraMatrix distCoeffs

`::cv::findChessboardCorners` returns a list of (retval, corners).
The retval value is a non-zero value if all of the corners are found and
they are placed in a certain order.
And the retval value should be passed to `::cv::drawChessboardCorners`'s
parameter `patternWasFound`.

    ::cv::computeCorrespondEpilines matrix whichImage F
    ::cv::estimateAffine2D matrix_1 matrix_2 ?method ransacReprojThreshold maxIters confidence refineIters?
    ::cv::estimateAffinePartial2D matrix_1 matrix_2 ?method ransacReprojThreshold maxIters confidence refineIters?
    ::cv::estimateAffine3D matrix_1 matrix_2 ?ransacThreshold confidence?
    ::cv::findFundamentalMat matrix_1 matrix_2 ?method ransacReprojThreshold confidence?
    ::cv::findHomography matrix_1 matrix_2 ?method ransacReprojThreshold maxIters confidence?

`::cv::estimateAffine2D`, `::cv::estimateAffinePartial2D`, `::cv::estimateAffine3D`,
`::cv::findFundamentalMat` and `::cv::findHomography` returns a list of (result, inliers or mask).

Note that whenever an H matrix cannot be estimated, an empty one will be returned.
You can use MATRIX `empty` to check this.

    ::cv::StereoBM ?numDisparities blockSize?
    StereoBM compute matrix1 matrix2
    StereoBM getPreFilterCap
    StereoBM getPreFilterSize
    StereoBM getPreFilterType
    StereoBM getSmallerBlockSize
    StereoBM getTextureThreshold
    StereoBM getUniquenessRatio
    StereoBM setPreFilterCap value
    StereoBM setPreFilterSize value
    StereoBM setPreFilterType value
    StereoBM setSmallerBlockSize value
    StereoBM setTextureThreshold value
    StereoBM setUniquenessRatio value
    StereoBM close

Please notice, StereoBM command will only have 1 instance.

    ::cv::StereoSGBM ?minDisparity numDisparities blockSize P1 P2 disp12MaxDiff preFilterCap uniquenessRatio speckleWindowSize speckleRange mode?
    StereoSGBM compute matrix1 matrix2
    StereoSGBM getMode
    StereoSGBM getP1
    StereoSGBM getP2
    StereoSGBM getPreFilterCap
    StereoSGBM getUniquenessRatio
    StereoSGBM setMode value
    StereoSGBM setP1 value
    StereoSGBM setP2 value
    StereoSGBM setPreFilterCap value
    StereoSGBM setUniquenessRatio value
    StereoSGBM close

Please notice, StereoSGBM command will only have 1 instance.

### video

    ::cv::BackgroundSubtractorKNN ?history dist2Threshold detectShadows?
    BackgroundSubtractorKNN apply matrix
    BackgroundSubtractorKNN getDetectShadows
    BackgroundSubtractorKNN getDist2Threshold
    BackgroundSubtractorKNN getHistory
    BackgroundSubtractorKNN setDetectShadows value
    BackgroundSubtractorKNN setDist2Threshold value
    BackgroundSubtractorKNN setHistory value
    BackgroundSubtractorKNN close

Please notice, BackgroundSubtractorKNN command will only have 1 instance.

    ::cv::BackgroundSubtractorMOG2 ?history varThreshold detectShadows?
    BackgroundSubtractorMOG2 apply matrix
    BackgroundSubtractorMOG2 getDetectShadows
    BackgroundSubtractorMOG2 getHistory
    BackgroundSubtractorMOG2 getVarThreshold
    BackgroundSubtractorMOG2 setDetectShadows value
    BackgroundSubtractorMOG2 setHistory value
    BackgroundSubtractorMOG2 setVarThreshold value
    BackgroundSubtractorMOG2 close

Please notice, BackgroundSubtractorMOG2 command will only have 1 instance.

    ::cv::meanShift matrix x y width height termCriteria
    ::cv::CamShift matrix x y width height termCriteria
    ::cv::calcOpticalFlowPyrLK prevImg nextImg prevPts winSize_width winSize_height maxLevel termCriteria
    ::cv::calcOpticalFlowFarneback prevImg nextImg pyr_scale levels winsize iterations poly_n poly_sigma flags
    ::cv::readOpticalFlow path
    ::cv::writeOpticalFlow path flow_matrix

    ::cv::computeECC matrix_1 matrix_2 ?mask?
    ::cv::findTransformECC matrix_1 matrix_2 warpMatrix motionType ?termCriteria maks gaussFiltSize?

Users should use `::cv::Mat::eye` to create a 3x3 (for `$::Cv::MOTION_HOMOGRAPHY`)
or 2x3 warpMatrix then pass to `::cv::findTransformECC`.

### photo

    ::cv::inpaint matrix inpaintMask inpaintRadius flags
    ::cv::decolor matrix

    ::cv::fastNlMeansDenoising matrix ?h templateWindowSize searchWindowSize?
    ::cv::fastNlMeansDenoisingColored matrix ?h hColor templateWindowSize searchWindowSize?

`::cv::fastNlMeansDenoising` expected to be applied to grayscale images.
`::cv::fastNlMeansDenoisingColored` is for colored images.

    ::cv::colorChange matrix mask red_mul green_mul blue_mul
    ::cv::illuminationChange matrix mask alpha beta
    ::cv::textureFlattening matrix mask low_threshold high_threshold ?kernel_size?
    ::cv::seamlessClone src_matrix dst_matrix mask x y flags

    ::cv::detailEnhance matrix ?sigma_s sigma_r?
    ::cv::edgePreservingFilter matrix ?flags sigma_s sigma_r?
    ::cv::pencilSketch matrix ?sigma_s sigma_r shade_factor?
    ::cv::stylization matrix ?sigma_s sigma_r?

Below are [High Dynamic Range Imaging](https://en.wikipedia.org/wiki/High-dynamic-range_imaging)
related commands:

    ::cv::AlignMTB ?max_bits exclude_range cut?
    AlignMTB process matrix_list
    AlignMTB close

Please notice, AlignMTB command will only have 1 instance.

    ::cv::CalibrateDebevec ?samples lambda random?
    CalibrateDebevec process matrix_list times_list
    CalibrateDebevec close

Please notice, CalibrateDebevec command will only have 1 instance.

    ::cv::MergeDebevec
    MergeDebevec process matrix_list times_list response
    MergeDebevec close

Please notice, MergeDebevec command will only have 1 instance.

    ::cv::MergeMertens ?contrast_weight saturation_weight exposure_weight?
    MergeMertens process matrix_list
    MergeMertens close

Please notice, MergeMertens command will only have 1 instance.

    ::cv::TonemapDrago ?gamma saturation bias?
    TonemapDrago process hdrDebevec
    TonemapDrago close

Please notice, TonemapDrago command will only have 1 instance.

    ::cv::TonemapMantiuk ?gamma scale saturation?
    TonemapMantiuk process hdrDebevec
    TonemapMantiuk close

Please notice, TonemapMantiuk command will only have 1 instance.

    ::cv::TonemapReinhard ?gamma intensity light_adapt color_adapt?
    TonemapReinhard process hdrDebevec
    TonemapReinhard close

Please notice, TonemapReinhard command will only have 1 instance.

### stitching

    ::cv::Stitcher mode
    Stitcher stitch image_list
    Stitcher close

Please notice, Stitcher command will only have 1 instance.

### objdetect

    ::cv::CascadeClassifier filename
    CascadeClassifier detectMultiScale matrix ?scaleFactor minNeighbors minWidth minHeight maxWidth maxHeight?
    CascadeClassifier close

    ::cv::HOGDescriptor winSize_width winSize_height blockSize_width blockStride_width blockStride_height blockSize_height cellSize_width cellSize_height nbins ?derivAperture winSigma L2HysThreshold gammaCorrection nlevels signedGradient?
    HOGDescriptor compute matrix ?winStride_width winStride_heigth padding_width padding_height?
    HOGDescriptor detectMultiScale matrix ?hitThreshold winStride_width winStride_height padding_width padding_height scale finalThreshold useMeanshiftGrouping?
    HOGDescriptor getDefaultPeopleDetector
    HOGDescriptor getDaimlerPeopleDetector
    HOGDescriptor setSVMDetector svmdetector
    HOGDescriptor close

    :cv::QRCodeDetector
    QRCodeDetector detect matrix
    QRCodeDetector detectAndDecode matrix
    QRCodeDetector close

Please notice, QRCodeDetector looks like have issues
if encoded QR Code data is long when I test it.

### ml

    ::cv::ml::LogisticRegression
    ::cv::ml::LogisticRegression::load filename
    LogisticRegression get_learnt_thetas
    LogisticRegression getIterations
    LogisticRegression getLearningRate
    LogisticRegression getMiniBatchSize
    LogisticRegression getRegularization
    LogisticRegression getTrainMethod
    LogisticRegression setIterations value
    LogisticRegression setLearningRate value
    LogisticRegression setMiniBatchSize value
    LogisticRegression setRegularization value
    LogisticRegression setTrainMethod value
    LogisticRegression setTermCriteria termCriteria
    LogisticRegression train trainData ?flags?
    LogisticRegression predict samples ?flags?
    LogisticRegression save filename
    LogisticRegression close

Please notice, LogisticRegression and LogisticRegression::load command will only have 1 instance.

    ::cv::ml::NormalBayesClassifier
    ::cv::ml::NormalBayesClassifier::load filename
    NormalBayesClassifier train trainData ?flags?
    NormalBayesClassifier predict samples ?flags?
    NormalBayesClassifier predictProb samples ?flags?
    NormalBayesClassifier save filename
    NormalBayesClassifier close

Please notice, NormalBayesClassifier and NormalBayesClassifier::load command will only have 1 instance.

    ::cv::ml::KNearest
    ::cv::ml::KNearest::load filename
    KNearest getAlgorithmType
    KNearest getDefaultK
    KNearest getEmax
    KNearest getIsClassifier
    KNearest setAlgorithmType value
    KNearest setDefaultK value
    KNearest setEmax
    KNearest setIsClassifier
    KNearest findNearest samples k
    KNearest train trainData ?flags?
    KNearest predict samples ?flags?
    KNearest save filename
    KNearest close

Please notice, KNearest and KNearest::load command will only have 1 instance.

    ::cv::ml::SVM
    ::cv::ml::SVM::load filename
    SVM getC
    SVM getCoef0
    SVM getDegree
    SVM getGamma
    SVM getNu
    SVM getP
    SVM getType
    SVM getKernelType
    SVM getDecisionFunction
    SVM getSupportVectors
    SVM getUncompressedSupportVectors
    SVM setC value
    SVM setCoef0 value
    SVM setDegree value
    SVM setGamma value
    SVM setNu value
    SVM setP value
    SVM setType value
    SVM setKernel value
    SVM setTermCriteria termCriteria
    SVM train trainData ?flags?
    SVM predict samples ?flags?
    SVM save filename
    SVM close

Please notice, SVM and SVM::load command will only have 1 instance.

    ::cv::ml::SVMSGD
    ::cv::ml::SVMSGD::load filename
    SVMSGD getInitialStepSize
    SVMSGD getMarginRegularization
    SVMSGD getMarginType
    SVMSGD getShift
    SVMSGD getStepDecreasingPower
    SVMSGD getSvmsgdType
    SVMSGD getWeights
    SVMSGD setInitialStepSize value
    SVMSGD setMarginRegularization value
    SVMSGD setMarginType value
    SVMSGD setOptimalParameters svmsgdType marginType
    SVMSGD setStepDecreasingPower value
    SVMSGD setSvmsgdType value
    SVMSGD setTermCriteria termCriteria
    SVMSGD train trainData ?flags?
    SVMSGD predict samples ?flags?
    SVMSGD save filename
    SVMSGD close

Please notice, SVMSGD and SVMSGD::load command will only have 1 instance.

    ::cv::ml::DTrees
    ::cv::ml::DTrees::load filename
    DTrees getCVFolds
    DTrees getMaxCategories
    DTrees getMaxDepth
    DTrees getMinSampleCount
    DTrees getPriors
    DTrees getRegressionAccuracy
    DTrees getTruncatePrunedTree
    DTrees getUse1SERule
    DTrees getUseSurrogates
    DTrees setCVFolds value
    DTrees setMaxCategories value
    DTrees setMaxDepth value
    DTrees setMinSampleCount value
    DTrees setPriors matrix
    DTrees setRegressionAccuracy value
    DTrees setTruncatePrunedTree value
    DTrees setUse1SERule value
    DTrees setUseSurrogates value
    DTrees train trainData ?flags?
    DTrees predict samples ?flags?
    DTrees save filename
    DTrees close

Please notice, DTrees and DTrees::load command will only have 1 instance.

    ::cv::ml::Boost
    ::cv::ml::Boost::load filename
    Boost getCVFolds
    Boost getMaxCategories
    Boost getMaxDepth
    Boost getMinSampleCount
    Boost getPriors
    Boost getRegressionAccuracy
    Boost getTruncatePrunedTree
    Boost getUse1SERule
    Boost getUseSurrogates
    Boost getBoostType
    Boost getWeakCount
    Boost getWeightTrimRate
    Boost setCVFolds value
    Boost setMaxCategories value
    Boost setMaxDepth value
    Boost setMinSampleCount value
    Boost setPriors matrix
    Boost setRegressionAccuracy value
    Boost setTruncatePrunedTree value
    Boost setUse1SERule value
    Boost setUseSurrogates value
    Boost setBoostType value
    Boost setWeakCount value
    Boost setWeightTrimRate value
    Boost train trainData ?flags?
    Boost predict samples ?flags?
    Boost save filename
    Boost close

Please notice, Boost and Boost::load command will only have 1 instance.

    ::cv::ml::RTrees
    ::cv::ml::RTrees::load filename
    RTrees getCVFolds
    RTrees getMaxCategories
    RTrees getMaxDepth
    RTrees getMinSampleCount
    RTrees getPriors
    RTrees getRegressionAccuracy
    RTrees getTruncatePrunedTree
    RTrees getUse1SERule
    RTrees getUseSurrogates
    RTrees getActiveVarCount
    RTrees getCalculateVarImportance
    RTrees getVarImportance
    RTrees getVotes
    RTrees setCVFolds value
    RTrees setMaxCategories value
    RTrees setMaxDepth value
    RTrees setMinSampleCount value
    RTrees setPriors matrix
    RTrees setRegressionAccuracy value
    RTrees setTruncatePrunedTree value
    RTrees setUse1SERule value
    RTrees setUseSurrogates value
    RTrees setActiveVarCount value
    RTrees setCalculateVarImportance value
    RTrees setTermCriteria termCriteria
    RTrees train trainData ?flags?
    RTrees predict samples ?flags?
    RTrees save filename
    RTrees close

Please notice, RTrees and RTrees::load command will only have 1 instance.

    ::cv::ml::ANN_MLP
    ::cv::ml::ANN_MLP::load filename
    ANN_MLP getAnnealCoolingRatio
    ANN_MLP getAnnealFinalT
    ANN_MLP getAnnealInitialT
    ANN_MLP getAnnealItePerStep
    ANN_MLP getBackpropMomentumScale
    ANN_MLP getBackpropWeightScale
    ANN_MLP getRpropDW0
    ANN_MLP getRpropDWMax
    ANN_MLP getRpropDWMin
    ANN_MLP getRpropDWMinus
    ANN_MLP getRpropDWPlus
    ANN_MLP getLayerSizes
    ANN_MLP getTrainMethod
    ANN_MLP getWeights layerIdx
    ANN_MLP setAnnealCoolingRatio value
    ANN_MLP setAnnealFinalT value
    ANN_MLP setAnnealInitialT value
    ANN_MLP setAnnealItePerStep value
    ANN_MLP setBackpropMomentumScale value
    ANN_MLP setBackpropWeightScale value
    ANN_MLP setRpropDW0 value
    ANN_MLP setRpropDWMax value
    ANN_MLP setRpropDWMin value
    ANN_MLP setRpropDWMinus value
    ANN_MLP setRpropDWPlus value
    ANN_MLP setActivationFunction value ?param1 param2?
    ANN_MLP setLayerSizes matrix
    ANN_MLP setTrainMethod value ?param1 param2?
    ANN_MLP setTermCriteria termCriteria
    ANN_MLP train trainData ?flags?
    ANN_MLP predict samples ?flags?
    ANN_MLP save filename
    ANN_MLP close

Please notice, ANN_MLP and ANN_MLP::load command will only have 1 instance.

    ::cv::ml::TrainData samples layout responses
    TrainData getTestResponses
    TrainData getTestSamples
    TrainData setTrainTestSplit count ?shuffle?
    TrainData setTrainTestSplitRatio ratio ?shuffle?
    TrainData close

Please notice, TrainData command will only have 1 instance.

### dnn

    ::cv::dnn::blobFromImage matrix scalefactor width height mean_color_list swapRB crop
    ::cv::dnn::readNet model ?config framework?
    READNET getLayerNames
    READNET setPreferableBackend backendId
    READNET setPreferableTarget targetId
    READNET setInput blob ?name scalefactor mean_color_list?
    READNET forward ?name?
    READNET close

If you want to know supported framework, you can check
[Deep Learning in OpenCV](https://github.com/opencv/opencv/wiki/Deep-Learning-in-OpenCV).

Users could check
[official OpenCV documentation](https://docs.opencv.org/master/d6/d0f/group__dnn.html#ga3b34fe7a29494a6a4295c169a7d32422) to know about the different frameworks,
their model files and the configuration files.

### thread support

    ::cv::thread::info ?tag?
    ::cv::thread::recv tag ?timeout?
    ::cv::thread::send tag mat|None ?string?

A simple mechanism to transfer `cv::Mat` from one Tcl thread to another
where `tag` is the name of a queue. The `info` command without arguments
returns all currently known `tags`, the `info` command with a `tag` returns
the number of items in that queue. With `send` an item consisting of a
`cv::Mat` or `None` and an optional `string` is added to the queue
named `tag`. With `recv` a zero, one, or two element list is returned
from the queue named `tag` with optional wait timeout in milliseconds
(default timeout is 10 seconds). Zero elements means timeout condition,
one or two elements are the name/command of the received `cv::Mat` or
empty element for `None`, and optional `string`.

Variables
=====

This extension defined below namespace variables:

Types -

    ::cv::CV_8U
    ::cv::CV_8S
    ::cv::CV_16U
    ::cv::CV_16S
    ::cv::CV_32S
    ::cv::CV_32F
    ::cv::CV_64F

    ::cv::CV_8UC1
    ::cv::CV_8UC2
    ::cv::CV_8UC3
    ::cv::CV_8UC4
    ::cv::CV_8SC1
    ::cv::CV_8SC2
    ::cv::CV_8SC3
    ::cv::CV_8SC4
    ::cv::CV_16UC1
    ::cv::CV_16UC2
    ::cv::CV_16UC3
    ::cv::CV_16UC4
    ::cv::CV_16SC1
    ::cv::CV_16SC2
    ::cv::CV_16SC3
    ::cv::CV_16SC4
    ::cv::CV_32SC1
    ::cv::CV_32SC2
    ::cv::CV_32SC3
    ::cv::CV_32SC4
    ::cv::CV_32FC1
    ::cv::CV_32FC2
    ::cv::CV_32FC3
    ::cv::CV_32FC4
    ::cv::CV_64FC1
    ::cv::CV_64FC2
    ::cv::CV_64FC3
    ::cv::CV_64FC4

`::cv::FileStorage` flags -

    ::cv::FileStorage::READ
    ::cv::FileStorage::WRITE
    ::cv::FileStorage::APPEND
    ::cv::FileStorage::MEMORY
    ::cv::FileStorage::FORMAT_AUTO
    ::cv::FileStorage::FORMAT_XML
    ::cv::FileStorage::FORMAT_YAML
    ::cv::FileStorage::FORMAT_JSON
    ::cv::FileStorage::BASE64
    ::cv::FileStorage::WRITE_BASE64

`::cv::sortIdx` flags -

    ::cv::SORT_EVERY_ROW
    ::cv::SORT_EVERY_COLUMN
    ::cv::SORT_ASCENDING
    ::cv::SORT_DESCENDING

`::cv::imread` flags -

    ::cv::IMREAD_UNCHANGED
    ::cv::IMREAD_GRAYSCALE
    ::cv::IMREAD_COLOR
    ::cv::IMREAD_ANYDEPTH
    ::cv::IMREAD_ANYCOLOR
    ::cv::IMREAD_LOAD_GDAL
    ::cv::IMREAD_REDUCED_GRAYSCALE_2
    ::cv::IMREAD_REDUCED_COLOR_2
    ::cv::IMREAD_REDUCED_GRAYSCALE_4
    ::cv::IMREAD_REDUCED_COLOR_4
    ::cv::IMREAD_REDUCED_GRAYSCALE_8
    ::cv::IMREAD_REDUCED_COLOR_8
    ::cv::IMREAD_IGNORE_ORIENTATION

`VideoCapture` command flags -

    ::cv::CAP_ANY
    ::cv::CAP_VFW
    ::cv::CAP_V4L
    ::cv::CAP_V4L2
    ::cv::CAP_FIREWIRE
    ::cv::CAP_FIREWARE
    ::cv::CAP_IEEE1394
    ::cv::CAP_DC1394
    ::cv::CAP_CMU1394
    ::cv::CAP_QT
    ::cv::CAP_UNICAP
    ::cv::CAP_DSHOW
    ::cv::CAP_PVAPI
    ::cv::CAP_OPENNI
    ::cv::CAP_OPENNI_ASUS
    ::cv::CAP_ANDROID
    ::cv::CAP_XIAPI
    ::cv::CAP_AVFOUNDATION
    ::cv::CAP_GIGANETIX
    ::cv::CAP_MSMF
    ::cv::CAP_WINRT
    ::cv::CAP_INTELPERC
    ::cv::CAP_REALSENSE
    ::cv::CAP_OPENNI2
    ::cv::CAP_OPENNI2_ASUS
    ::cv::CAP_OPENNI2_ASTRA
    ::cv::CAP_GPHOTO2
    ::cv::CAP_GSTREAMER
    ::cv::CAP_FFMPEG
    ::cv::CAP_IMAGES
    ::cv::CAP_ARAVIS
    ::cv::CAP_OPENCV_MJPEG
    ::cv::CAP_INTEL_MFX
    ::cv::CAP_XINE
    ::cv::CAP_UEYE

`VideoCapture` and `VideoWriter` propId -

    ::cv::CAP_PROP_POS_MSEC
    ::cv::CAP_PROP_POS_FRAMES
    ::cv::CAP_PROP_POS_AVI_RATIO
    ::cv::CAP_PROP_FRAME_WIDTH
    ::cv::CAP_PROP_FRAME_HEIGHT
    ::cv::CAP_PROP_FPS
    ::cv::CAP_PROP_FOURCC
    ::cv::CAP_PROP_FRAME_COUNT
    ::cv::CAP_PROP_FORMAT
    ::cv::CAP_PROP_MODE
    ::cv::CAP_PROP_BRIGHTNESS
    ::cv::CAP_PROP_CONTRAST
    ::cv::CAP_PROP_SATURATION
    ::cv::CAP_PROP_HUE
    ::cv::CAP_PROP_GAIN
    ::cv::CAP_PROP_EXPOSURE
    ::cv::CAP_PROP_CONVERT_RGB
    ::cv::CAP_PROP_WHITE_BALANCE_BLUE_U
    ::cv::CAP_PROP_RECTIFICATION
    ::cv::CAP_PROP_MONOCHROME
    ::cv::CAP_PROP_SHARPNESS
    ::cv::CAP_PROP_AUTO_EXPOSURE
    ::cv::CAP_PROP_GAMMA
    ::cv::CAP_PROP_TEMPERATURE
    ::cv::CAP_PROP_TRIGGER
    ::cv::CAP_PROP_TRIGGER_DELAY
    ::cv::CAP_PROP_WHITE_BALANCE_RED_V
    ::cv::CAP_PROP_ZOOM
    ::cv::CAP_PROP_FOCUS
    ::cv::CAP_PROP_GUID
    ::cv::CAP_PROP_ISO_SPEED
    ::cv::CAP_PROP_BACKLIGHT
    ::cv::CAP_PROP_PAN
    ::cv::CAP_PROP_TILT
    ::cv::CAP_PROP_ROLL
    ::cv::CAP_PROP_IRIS
    ::cv::CAP_PROP_SETTINGS
    ::cv::CAP_PROP_BUFFERSIZE
    ::cv::CAP_PROP_AUTOFOCUS
    ::cv::CAP_PROP_SAR_NUM
    ::cv::CAP_PROP_SAR_DEN
    ::cv::CAP_PROP_BACKEND
    ::cv::CAP_PROP_CHANNEL
    ::cv::CAP_PROP_AUTO_WB
    ::cv::CAP_PROP_WB_TEMPERATURE
    ::cv::CAP_PROP_CODEC_PIXEL_FORMAT
    ::cv::CAP_PROP_BITRATE
    ::cv::CAP_PROP_ORIENTATION_META
    ::cv::CAP_PROP_ORIENTATION_AUTO
    ::cv::CAP_PROP_HW_ACCELERATION
    ::cv::CAP_PROP_HW_DEVICE

`VideoWriter` propId -

    ::cv::VIDEOWRITER_PROP_QUALITY
    ::cv::VIDEOWRITER_PROP_FRAMEBYTES
    ::cv::VIDEOWRITER_PROP_NSTRIPES
    ::cv::VIDEOWRITER_PROP_IS_COLOR
    ::cv::VIDEOWRITER_PROP_DEPTH
    ::cv::VIDEOWRITER_PROP_HW_ACCELERATION
    ::cv::VIDEOWRITER_PROP_HW_DEVICE

`::cv::cvtColor` code -

    ::cv::COLOR_BGR2BGRA
    ::cv::COLOR_RGB2RGBA
    ::cv::COLOR_BGRA2BGR
    ::cv::COLOR_RGBA2RGB
    ::cv::COLOR_BGR2RGBA
    ::cv::COLOR_RGB2BGRA
    ::cv::COLOR_RGBA2BGR
    ::cv::COLOR_BGRA2RGB
    ::cv::COLOR_BGR2RGB
    ::cv::COLOR_RGB2BGR
    ::cv::COLOR_BGRA2RGBA
    ::cv::COLOR_RGBA2BGRA
    ::cv::COLOR_BGR2GRAY
    ::cv::COLOR_RGB2GRAY
    ::cv::COLOR_GRAY2BGR
    ::cv::COLOR_GRAY2RGB
    ::cv::COLOR_GRAY2BGRA
    ::cv::COLOR_GRAY2RGBA
    ::cv::COLOR_BGRA2GRAY
    ::cv::COLOR_RGBA2GRAY
    ::cv::COLOR_BGR2BGR565
    ::cv::COLOR_RGB2BGR565
    ::cv::COLOR_BGR5652BGR
    ::cv::COLOR_BGR5652RGB
    ::cv::COLOR_BGRA2BGR565
    ::cv::COLOR_RGBA2BGR565
    ::cv::COLOR_BGR5652BGRA
    ::cv::COLOR_BGR5652RGBA
    ::cv::COLOR_GRAY2BGR565
    ::cv::COLOR_BGR5652GRAY
    ::cv::COLOR_BGR2BGR555
    ::cv::COLOR_RGB2BGR555
    ::cv::COLOR_BGR5552BGR
    ::cv::COLOR_BGR5552RGB
    ::cv::COLOR_BGRA2BGR555
    ::cv::COLOR_RGBA2BGR555
    ::cv::COLOR_BGR5552BGRA
    ::cv::COLOR_BGR5552RGBA
    ::cv::COLOR_GRAY2BGR555
    ::cv::COLOR_BGR5552GRAY
    ::cv::COLOR_BGR2XYZ
    ::cv::COLOR_RGB2XYZ
    ::cv::COLOR_XYZ2BGR
    ::cv::COLOR_XYZ2RGB
    ::cv::COLOR_BGR2YCrCb
    ::cv::COLOR_RGB2YCrCb
    ::cv::COLOR_YCrCb2BGR
    ::cv::COLOR_YCrCb2RGB
    ::cv::COLOR_BGR2HSV
    ::cv::COLOR_RGB2HSV
    ::cv::COLOR_BGR2Lab
    ::cv::COLOR_RGB2Lab
    ::cv::COLOR_BGR2Luv
    ::cv::COLOR_RGB2Luv
    ::cv::COLOR_BGR2HLS
    ::cv::COLOR_RGB2HLS
    ::cv::COLOR_HSV2BGR
    ::cv::COLOR_HSV2RGB
    ::cv::COLOR_Lab2BGR
    ::cv::COLOR_Lab2RGB
    ::cv::COLOR_Luv2BGR
    ::cv::COLOR_Luv2RGB
    ::cv::COLOR_HLS2BGR
    ::cv::COLOR_HLS2RGB
    ::cv::COLOR_BGR2HSV_FULL
    ::cv::COLOR_RGB2HSV_FULL
    ::cv::COLOR_BGR2HLS_FULL
    ::cv::COLOR_RGB2HLS_FULL
    ::cv::COLOR_HSV2BGR_FULL
    ::cv::COLOR_HSV2RGB_FULL
    ::cv::COLOR_HLS2BGR_FULL
    ::cv::COLOR_HLS2RGB_FULL
    ::cv::COLOR_LBGR2Lab
    ::cv::COLOR_LRGB2Lab
    ::cv::COLOR_LBGR2Luv
    ::cv::COLOR_LRGB2Luv
    ::cv::COLOR_Lab2LBGR
    ::cv::COLOR_Lab2LRGB
    ::cv::COLOR_Luv2LBGR
    ::cv::COLOR_Luv2LRGB
    ::cv::COLOR_BGR2YUV
    ::cv::COLOR_RGB2YUV
    ::cv::COLOR_YUV2BGR
    ::cv::COLOR_YUV2RGB
    ::cv::COLOR_YUV2RGB_NV12
    ::cv::COLOR_YUV2BGR_NV12
    ::cv::COLOR_YUV2RGB_NV21
    ::cv::COLOR_YUV2BGR_NV21
    ::cv::COLOR_YUV420sp2RGB
    ::cv::COLOR_YUV420sp2BGR
    ::cv::COLOR_YUV2RGBA_NV12
    ::cv::COLOR_YUV2BGRA_NV12
    ::cv::COLOR_YUV2RGBA_NV21
    ::cv::COLOR_YUV2BGRA_NV21
    ::cv::COLOR_YUV420sp2RGBA
    ::cv::COLOR_YUV420sp2BGRA
    ::cv::COLOR_YUV2RGB_YV12
    ::cv::COLOR_YUV2BGR_YV12
    ::cv::COLOR_YUV2RGB_IYUV
    ::cv::COLOR_YUV2BGR_IYUV
    ::cv::COLOR_YUV2RGB_I420
    ::cv::COLOR_YUV2BGR_I420
    ::cv::COLOR_YUV420p2RGB
    ::cv::COLOR_YUV420p2BGR
    ::cv::COLOR_YUV2RGBA_YV12
    ::cv::COLOR_YUV2BGRA_YV12
    ::cv::COLOR_YUV2RGBA_IYUV
    ::cv::COLOR_YUV2BGRA_IYUV
    ::cv::COLOR_YUV2RGBA_I420
    ::cv::COLOR_YUV2BGRA_I420
    ::cv::COLOR_YUV420p2RGBA
    ::cv::COLOR_YUV420p2BGRA
    ::cv::COLOR_YUV2GRAY_420
    ::cv::COLOR_YUV2GRAY_NV21
    ::cv::COLOR_YUV2GRAY_NV12
    ::cv::COLOR_YUV2GRAY_YV12
    ::cv::COLOR_YUV2GRAY_IYUV
    ::cv::COLOR_YUV2GRAY_I420
    ::cv::COLOR_YUV420sp2GRAY
    ::cv::COLOR_YUV420p2GRAY
    ::cv::COLOR_YUV2RGB_UYVY
    ::cv::COLOR_YUV2BGR_UYVY
    ::cv::COLOR_YUV2RGB_Y422
    ::cv::COLOR_YUV2BGR_Y422
    ::cv::COLOR_YUV2RGB_UYNV
    ::cv::COLOR_YUV2BGR_UYNV
    ::cv::COLOR_YUV2RGBA_UYVY
    ::cv::COLOR_YUV2BGRA_UYVY
    ::cv::COLOR_YUV2RGBA_Y422
    ::cv::COLOR_YUV2BGRA_Y422
    ::cv::COLOR_YUV2RGBA_UYNV
    ::cv::COLOR_YUV2BGRA_UYNV
    ::cv::COLOR_YUV2RGB_YUY2
    ::cv::COLOR_YUV2BGR_YUY2
    ::cv::COLOR_YUV2RGB_YVYU
    ::cv::COLOR_YUV2BGR_YVYU
    ::cv::COLOR_YUV2RGB_YUYV
    ::cv::COLOR_YUV2BGR_YUYV
    ::cv::COLOR_YUV2RGB_YUNV
    ::cv::COLOR_YUV2BGR_YUNV
    ::cv::COLOR_YUV2RGBA_YUY2
    ::cv::COLOR_YUV2BGRA_YUY2
    ::cv::COLOR_YUV2RGBA_YVYU
    ::cv::COLOR_YUV2BGRA_YVYU
    ::cv::COLOR_YUV2RGBA_YUYV
    ::cv::COLOR_YUV2BGRA_YUYV
    ::cv::COLOR_YUV2RGBA_YUNV
    ::cv::COLOR_YUV2BGRA_YUNV
    ::cv::COLOR_YUV2GRAY_UYVY
    ::cv::COLOR_YUV2GRAY_YUY2
    ::cv::COLOR_YUV2GRAY_Y422
    ::cv::COLOR_YUV2GRAY_UYNV
    ::cv::COLOR_YUV2GRAY_YVYU
    ::cv::COLOR_YUV2GRAY_YUYV
    ::cv::COLOR_YUV2GRAY_YUNV
    ::cv::COLOR_RGBA2mRGBA
    ::cv::COLOR_mRGBA2RGBA
    ::cv::COLOR_RGB2YUV_I420
    ::cv::COLOR_BGR2YUV_I420
    ::cv::COLOR_RGB2YUV_IYUV
    ::cv::COLOR_BGR2YUV_IYUV
    ::cv::COLOR_RGBA2YUV_I420
    ::cv::COLOR_BGRA2YUV_I420
    ::cv::COLOR_RGBA2YUV_IYUV
    ::cv::COLOR_BGRA2YUV_IYUV
    ::cv::COLOR_RGB2YUV_YV12
    ::cv::COLOR_BGR2YUV_YV12
    ::cv::COLOR_RGBA2YUV_YV12
    ::cv::COLOR_BGRA2YUV_YV12
    ::cv::COLOR_BayerBG2BGR
    ::cv::COLOR_BayerGB2BGR
    ::cv::COLOR_BayerRG2BGR
    ::cv::COLOR_BayerGR2BGR
    ::cv::COLOR_BayerBG2RGB
    ::cv::COLOR_BayerGB2RGB
    ::cv::COLOR_BayerRG2RGB
    ::cv::COLOR_BayerGR2RGB
    ::cv::COLOR_BayerBG2GRAY
    ::cv::COLOR_BayerGB2GRAY
    ::cv::COLOR_BayerRG2GRAY
    ::cv::COLOR_BayerGR2GRAY
    ::cv::COLOR_BayerBG2BGR_VNG
    ::cv::COLOR_BayerGB2BGR_VNG
    ::cv::COLOR_BayerRG2BGR_VNG
    ::cv::COLOR_BayerGR2BGR_VNG
    ::cv::COLOR_BayerBG2RGB_VNG
    ::cv::COLOR_BayerGB2RGB_VNG
    ::cv::COLOR_BayerRG2RGB_VNG
    ::cv::COLOR_BayerGR2RGB_VNG
    ::cv::COLOR_BayerBG2BGR_EA
    ::cv::COLOR_BayerGB2BGR_EA
    ::cv::COLOR_BayerRG2BGR_EA
    ::cv::COLOR_BayerGR2BGR_EA
    ::cv::COLOR_BayerBG2RGB_EA
    ::cv::COLOR_BayerGB2RGB_EA
    ::cv::COLOR_BayerRG2RGB_EA
    ::cv::COLOR_BayerGR2RGB_EA
    ::cv::COLOR_BayerBG2BGRA
    ::cv::COLOR_BayerGB2BGRA
    ::cv::COLOR_BayerRG2BGRA
    ::cv::COLOR_BayerGR2BGRA
    ::cv::COLOR_BayerBG2RGBA
    ::cv::COLOR_BayerGB2RGBA
    ::cv::COLOR_BayerRG2RGBA
    ::cv::COLOR_BayerGR2RGBA
    ::cv::COLOR_COLORCVT_MAX

Line Types -

    ::cv::FILLED
    ::cv::LINE_4
    ::cv::LINE_8
    ::cv::LINE_AA

`::cv::drawMarker` Marker Types -

    ::cv::MARKER_CROSS
    ::cv::MARKER_TILTED_CROSS
    ::cv::MARKER_STAR
    ::cv::MARKER_DIAMOND
    ::cv::MARKER_SQUARE
    ::cv::MARKER_TRIANGLE_UP
    ::cv::MARKER_TRIANGLE_DOWN

`::cv::putText` fontFace -

    ::cv::FONT_HERSHEY_SIMPLEX
    ::cv::FONT_HERSHEY_PLAIN
    ::cv::FONT_HERSHEY_DUPLEX
    ::cv::FONT_HERSHEY_COMPLEX
    ::cv::FONT_HERSHEY_TRIPLEX
    ::cv::FONT_HERSHEY_COMPLEX_SMALL
    ::cv::FONT_HERSHEY_SCRIPT_SIMPLEX
    ::cv::FONT_HERSHEY_SCRIPT_COMPLEX
    ::cv::FONT_ITALIC

`::cv::threshold` type -

    ::cv::THRESH_BINARY
    ::cv::THRESH_BINARY_INV
    ::cv::THRESH_TRUNC
    ::cv::THRESH_TOZERO
    ::cv::THRESH_TOZERO_INV
    ::cv::THRESH_MASK
    ::cv::THRESH_OTSU
    ::cv::THRESH_TRIANGLE

`::cv::adaptiveThreshold` method -

    ::cv::ADAPTIVE_THRESH_MEAN_C
    ::cv::ADAPTIVE_THRESH_GAUSSIAN_C

`::cv::copyMakeBorder` border type -

    ::cv::BORDER_CONSTANT
    ::cv::BORDER_REPLICATE
    ::cv::BORDER_REFLECT
    ::cv::BORDER_WRAP
    ::cv::BORDER_REFLECT_101
    ::cv::BORDER_TRANSPARENT
    ::cv::BORDER_REFLECT101
    ::cv::BORDER_DEFAULT
    ::cv::BORDER_ISOLATED

`::cv::getStructuringElement` morph shapes -

    ::cv::MORPH_RECT
    ::cv::MORPH_CROSS
    ::cv::MORPH_ELLIPSE

`::cv::morphologyEx` op type -

    ::cv::MORPH_ERODE
    ::cv::MORPH_DILATE
    ::cv::MORPH_OPEN
    ::cv::MORPH_CLOSE
    ::cv::MORPH_GRADIENT
    ::cv::MORPH_TOPHAT
    ::cv::MORPH_BLACKHAT
    ::cv::MORPH_HITMISS

`::cv::matchShapes` method -

    ::cv::CONTOURS_MATCH_I1
    ::cv::CONTOURS_MATCH_I2
    ::cv::CONTOURS_MATCH_I3

`::cv::applyColorMap` colormap -

    ::cv::COLORMAP_AUTUMN
    ::cv::COLORMAP_BONE
    ::cv::COLORMAP_JET
    ::cv::COLORMAP_WINTER
    ::cv::COLORMAP_RAINBOW
    ::cv::COLORMAP_OCEAN
    ::cv::COLORMAP_SUMMER
    ::cv::COLORMAP_SPRING
    ::cv::COLORMAP_COOL
    ::cv::COLORMAP_HSV
    ::cv::COLORMAP_PINK
    ::cv::COLORMAP_HOT
    ::cv::COLORMAP_PARULA
    ::cv::COLORMAP_MAGMA
    ::cv::COLORMAP_INFERNO
    ::cv::COLORMAP_PLASMA
    ::cv::COLORMAP_VIRIDIS
    ::cv::COLORMAP_CIVIDIS
    ::cv::COLORMAP_TWILIGHT
    ::cv::COLORMAP_TWILIGHT_SHIFTED
    ::cv::COLORMAP_TURBO
    ::cv::COLORMAP_DEEPGREEN

`::cv::namedWindow` flags -

    ::cv::WINDOW_NORMAL
    ::cv::WINDOW_AUTOSIZE
    ::cv::WINDOW_OPENGL
    ::cv::WINDOW_FULLSCREEN
    ::cv::WINDOW_FREERATIO
    ::cv::WINDOW_KEEPRATIO
    ::cv::WINDOW_GUI_EXPANDED
    ::cv::WINDOW_GUI_NORMAL

`::cv::setMouseCallback` event -

    ::cv::EVENT_MOUSEMOVE
    ::cv::EVENT_LBUTTONDOWN
    ::cv::EVENT_RBUTTONDOWN
    ::cv::EVENT_MBUTTONDOWN
    ::cv::EVENT_LBUTTONUP
    ::cv::EVENT_RBUTTONUP
    ::cv::EVENT_MBUTTONUP
    ::cv::EVENT_LBUTTONDBLCLK
    ::cv::EVENT_RBUTTONDBLCLK
    ::cv::EVENT_MBUTTONDBLCLK
    ::cv::EVENT_MOUSEWHEEL
    ::cv::EVENT_MOUSEHWHEEL

    ::cv::EVENT_FLAG_LBUTTON
    ::cv::EVENT_FLAG_RBUTTON
    ::cv::EVENT_FLAG_MBUTTON
    ::cv::EVENT_FLAG_CTRLKEY
    ::cv::EVENT_FLAG_SHIFTKEY
    ::cv::EVENT_FLAG_ALTKEY

`::cv::compareHist` method -

    ::cv::HISTCMP_CORREL
    ::cv::HISTCMP_CHISQR
    ::cv::HISTCMP_INTERSECT
    ::cv::HISTCMP_BHATTACHARYYA
    ::cv::HISTCMP_HELLINGER
    ::cv::HISTCMP_CHISQR_ALT
    ::cv::HISTCMP_KL_DIV

`::cv::dft` command flags -

    ::cv::DFT_INVERSE
    ::cv::DFT_SCALE
    ::cv::DFT_ROWS
    ::cv::DFT_COMPLEX_OUTPUT
    ::cv::DFT_REAL_OUTPUT
    ::cv::DFT_COMPLEX_INPUT
    ::cv::DCT_INVERSE
    ::cv::DCT_ROWS

`::cv::SVDecomp` flags -

    ::cv::SVD_MODIFY_A
    ::cv::SVD_NO_UV
    ::cv::SVD_FULL_UV

`::cv::norm`, `::cv::normalize` and `BFMatcher`norm_type -

    ::cv::NORM_INF
    ::cv::NORM_L1
    ::cv::NORM_L2
    ::cv::NORM_L2SQR
    ::cv::NORM_HAMMING
    ::cv::NORM_HAMMING2
    ::cv::NORM_TYPE_MASK
    ::cv::NORM_RELATIVE
    ::cv::NORM_MINMAX

`::cv::floodFill` command flag -

    ::cv::FLOODFILL_FIXED_RANGE

`:cv::remap`interpolation,
`::cv::resize`, `::cv::warpAffine` and `::cv::warpPerspective` command flag -

    ::cv::INTER_NEAREST
    ::cv::INTER_LINEAR
    ::cv::INTER_CUBIC
    ::cv::INTER_AREA
    ::cv::INTER_LANCZOS4
    ::cv::INTER_LINEAR_EXACT
    ::cv::INTER_NEAREST_EXACT
    ::cv::INTER_MAX
    ::cv::WARP_FILL_OUTLIERS
    ::cv::WARP_INVERSE_MAP

`::cv::warpPolar` mode -

    ::cv::WARP_POLAR_LINEAR
    ::cv::WARP_POLAR_LOG

`::cv::compare` cmpop -

    ::cv::CMP_EQ
    ::cv::CMP_GT
    ::cv::CMP_GE
    ::cv::CMP_LT
    ::cv::CMP_LE
    ::cv::CMP_NE

`::cv::reduce` rtype -

    ::cv::REDUCE_SUM
    ::cv::REDUCE_AVG
    ::cv::REDUCE_MAX
    ::cv::REDUCE_MIN

`::cv::rotate` rotateCode -

    ::cv::ROTATE_90_CLOCKWISE
    ::cv::ROTATE_180
    ::cv::ROTATE_90_COUNTERCLOCKWISE

`MATRIX inv` method, `::cv::solve` flags and
`::cv::getPerspectiveTransform` command solveMethod -

    ::cv::DECOMP_LU
    ::cv::DECOMP_SVD
    ::cv::DECOMP_EIG
    ::cv::DECOMP_CHOLESKY
    ::cv::DECOMP_QR
    ::cv::DECOMP_NORMAL

`::cv::findContours` mode -

    ::cv::RETR_EXTERNAL
    ::cv::RETR_LIST
    ::cv::RETR_CCOMP
    ::cv::RETR_TREE
    ::cv::RETR_FLOODFILL

`::cv::findContours` method -

    ::cv::CHAIN_APPROX_NONE
    ::cv::CHAIN_APPROX_SIMPLE
    ::cv::CHAIN_APPROX_TC89_L1
    ::cv::CHAIN_APPROX_TC89_KCOS

`::cv::HoughCircles` method -

    ::cv::HOUGH_GRADIENT
    ::cv::HOUGH_GRADIENT_ALT

`::cv::matchTemplate` method -

    ::cv::TM_SQDIFF
    ::cv::TM_SQDIFF_NORMED
    ::cv::TM_CCORR
    ::cv::TM_CCORR_NORMED
    ::cv::TM_CCOEFF
    ::cv::TM_CCOEFF_NORMED

`::cv::EMD` distType, `::cv::fitLine` distType and
`::cv::distanceTransform` distanceType -

    ::cv::DIST_L1
    ::cv::DIST_L2
    ::cv::DIST_C
    ::cv::DIST_L12
    ::cv::DIST_FAIR
    ::cv::DIST_WELSCH
    ::cv::DIST_HUBER

GrabCut classes -

    ::cv::GC_BGD
    ::cv::GC_FGD
    ::cv::GC_PR_BGD
    ::cv::GC_PR_FGD

`::cv::calcCovarMatrix` flags -

    ::cv::COVAR_SCRAMBLED
    ::cv::COVAR_NORMAL
    ::cv::COVAR_USE_AVG
    ::cv::COVAR_SCALE
    ::cv::COVAR_ROWS
    ::cv::COVAR_COLS

Kmeans flags -

    ::cv::KMEANS_RANDOM_CENTERS
    ::cv::KMEANS_PP_CENTERS
    ::cv::KMEANS_USE_INITIAL_LABELS

PCA flags -

    ::cv::DATA_AS_ROW
    ::cv::DATA_AS_COL
    ::cv::USE_AVG

TermCriteria type -

    ::cv::COUNT
    ::cv::EPS

`FastFeatureDetector` detector type -

    ::cv::DetectorType_TYPE_5_8
    ::cv::DetectorType_TYPE_7_12
    ::cv::DetectorType_TYPE_9_16

`AgastFeatureDetector` detector type -

    ::cv::DetectorType_AGAST_5_8
    ::cv::DetectorType_AGAST_7_12d
    ::cv::DetectorType_AGAST_7_12s
    ::cv::DetectorType_OAST_9_16

`ORB` Score type -

    ::cv::ORB_HARRIS_SCORE
    ::cv::ORB_FAST_SCORE

`AKAZE` descriptor type -

    ::cv::AKAZE_DESCRIPTOR_KAZE_UPRIGHT
    ::cv::AKAZE_DESCRIPTOR_KAZE
    ::cv::AKAZE_DESCRIPTOR_MLDB_UPRIGHT
    ::cv::AKAZE_DESCRIPTOR_MLDB

`KAZE` and `AKAZE` Diffusivity type -

    ::cv::KAZE_DIFF_PM_G1
    ::cv::KAZE_DIFF_PM_G2
    ::cv::KAZE_DIFF_WEICKERT
    ::cv::KAZE_DIFF_CHARBONNIER

`drawKeypoints` and `drawMatches` flags

    ::cv::DRAW_MATCHES_FLAGS_DEFAULT
    ::cv::DRAW_MATCHES_FLAGS_DRAW_OVER_OUTIMG
    ::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS
    ::cv::DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS

`::cv::findTransformECC` motion type -

    ::cv::MOTION_TRANSLATION
    ::cv::MOTION_EUCLIDEAN
    ::cv::MOTION_AFFINE
    ::cv::MOTION_HOMOGRAPHY

`findChessboardCorners` flags -

    ::cv::CALIB_CB_ADAPTIVE_THRESH
    ::cv::CALIB_CB_NORMALIZE_IMAGE
    ::cv::CALIB_CB_FILTER_QUADS
    ::cv::CALIB_CB_FAST_CHECK

`findFundamentalMat` method -

    ::cv::FM_7POINT
    ::cv::FM_8POINT
    ::cv::FM_LMEDS
    ::cv::FM_RANSAC

`findHomography` method -

    ::cv::LMEDS
    ::cv::RANSAC
    ::cv::RHO

StereoBM PreFilter type -

    ::cv::PREFILTER_NORMALIZED_RESPONSE
    ::cv::PREFILTER_XSOBEL

StereoSGBM mode -

    ::cv::StereoSGBM_MODE_SGBM
    ::cv::StereoSGBM_MODE_HH
    ::cv::StereoSGBM_MODE_SGBM_3WAY
    ::cv::StereoSGBM_MODE_HH4

Photo inpaint flags -

    ::cv::INPAINT_NS
    ::cv::INPAINT_TELEA

Photo edgePreservingFilter flags -

    ::cv::RECURS_FILTER
    ::cv::NORMCONV_FILTER

Photo seamlessClone flags -

    ::cv::NORMAL_CLONE
    ::cv::MIXED_CLONE
    ::cv::MONOCHROME_TRANSFER

Stitcher mode -

    ::cv::PANORAMA
    ::cv::SCANS

ML sample types -

    ::cv::ml::ROW_SAMPLE
    ::cv::ml::COL_SAMPLE

Predict options -

    ::cv::ml::UPDATE_MODEL
    ::cv::ml::RAW_OUTPUT
    ::cv::ml::COMPRESSED_INPUT
    ::cv::ml::PREPROCESSED_INPUT

LogisticRegression Methods -

    ::cv::ml::LOGISTIC_BATCH
    ::cv::ml::LOGISTIC_MINI_BATCH

LogisticRegression Regularization Kinds -

    ::cv::ml::LOGISTIC_REG_DISABLE
    ::cv::ml::LOGISTIC_REG_L1
    ::cv::ml::LOGISTIC_REG_L2

KNearest algorithm -

    ::cv::ml::KNEAREST_BRUTE_FORCE
    ::cv::ml::KNEAREST_KDTREE

SVM types -

    ::cv::ml::SVM_C_SVC
    ::cv::ml::SVM_NU_SVC
    ::cv::ml::SVM_ONE_CLASS
    ::cv::ml::SVM_EPS_SVR
    ::cv::ml::SVM_NU_SVR

SVM kernel types -

    ::cv::ml::SVM_LINEAR
    ::cv::ml::SVM_POLY
    ::cv::ml::SVM_RBF
    ::cv::ml::SVM_SIGMOID
    ::cv::ml::SVM_CHI2
    ::cv::ml::SVM_INTER

SVMSGD Margin Type -

    ::cv::ml::SVMSGD_SOFT_MARGIN
    ::cv::ml::SVMSGD_HARD_MARGIN

SVMSGD Type -

    ::cv::ml::SVMSGD_SGD
    ::cv::ml::SVMSGD_ASGD

Boost Types -

    ::cv::ml::BOOST_DISCRETE
    ::cv::ml::BOOST_REAL
    ::cv::ml::BOOST_LOGIT
    ::cv::ml::BOOST_GENTLE

ANN_MLP ActivationFunctions -

    ::cv::ml::MLP_IDENTITY
    ::cv::ml::MLP_SIGMOID_SYM
    ::cv::ml::MLP_GAUSSIAN
    ::cv::ml::MLP_RELU
    ::cv::ml::MLP_LEAKYRELU

ANN_MLP Train Flags

    ::cv::ml::MLP_UPDATE_WEIGHTS
    ::cv::ml::MLP_NO_INPUT_SCALE
    ::cv::ml::MLP_NO_OUTPUT_SCALE

ANN_MLP Training Methods -

    ::cv::ml::MLP_BACKPROP
    ::cv::ml::MLP_RPROP
    ::cv::ml::MLP_ANNEAL

READNET `setPreferableBackend` backendId -

    ::cv::dnn::DNN_BACKEND_DEFAULT
    ::cv::dnn::DNN_BACKEND_HALIDE
    ::cv::dnn::DNN_BACKEND_OPENCV
    ::cv::dnn::DNN_BACKEND_VKCOM
    ::cv::dnn::DNN_BACKEND_CUDA

READNET `setPreferableTarget` targetId -

    ::cv::dnn::DNN_TARGET_CPU
    ::cv::dnn::DNN_TARGET_OPENCL
    ::cv::dnn::DNN_TARGET_OPENCL_FP16
    ::cv::dnn::DNN_TARGET_MYRIAD
    ::cv::dnn::DNN_TARGET_VULKAN
    ::cv::dnn::DNN_TARGET_CUDA
    ::cv::dnn::DNN_TARGET_CUDA_FP16
    ::cv::dnn::DNN_TARGET_HDDL

Examples
=====

Brighness and contrast -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]

        set img2 [$img convertTo -1 1 100]
        set img3 [$img convertTo -1 1 -100]
        set img4 [$img convertTo -1 2 0]
        set img5 [$img convertTo -1 0.5 0]

        ::cv::namedWindow "Source" $::cv::WINDOW_NORMAL
        ::cv::imshow "Source" $img
        ::cv::namedWindow "Brighness High" $::cv::WINDOW_NORMAL
        ::cv::imshow "Brighness High" $img2
        ::cv::namedWindow "Brighness Low" $::cv::WINDOW_NORMAL
        ::cv::imshow "Brighness Low" $img3
        ::cv::namedWindow "Contrast High" $::cv::WINDOW_NORMAL
        ::cv::imshow "Contrast High" $img4
        ::cv::namedWindow "Contrast Low" $::cv::WINDOW_NORMAL
        ::cv::imshow "Contrast Low" $img5

        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img close
        $img2 close
        $img3 close
        $img4 close
        $img5 close
    } on error {em} {
        puts $em
    }

Below is an example to apply a color map -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]

        set img2 [::cv::applyColorMap $img $::cv::COLORMAP_RAINBOW]

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $img
        ::cv::namedWindow "Display Image 2" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image 2" $img2
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img close
        $img2 close
    } on error {em} {
        puts $em
    }

Flip and Rotate -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]

        # 0 means flipping around the x-axis
        set f_img [::cv::flip $img 0]

        # Rotate by 270 degrees clockwise
        set r_img [::cv::rotate $img $::cv::ROTATE_90_COUNTERCLOCKWISE]

        ::cv::namedWindow "Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Image" $img
        ::cv::namedWindow "Flip" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Flip" $f_img
        ::cv::namedWindow "Rotate" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Rotate" $r_img
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img close
        $f_img close
        $r_img close
    } on error {em} {
        puts $em
    }

Image Rotation and Translation -

    package require opencv

    if {$argc != 1} {
        puts "Please give a file name."
    }

    set filename [lindex $argv 0]

    try {
        set img1 [cv::imread $filename]

        # Rotation
        set matrix1 [::cv::getRotationMatrix2D [expr [$img1 cols]/2] [expr [$img1 rows]/2] 45 1]
        set img2 [::cv::warpAffine $img1 $matrix1 [$img1 cols] [$img1 rows]]

        # Translation
        set matrix2 [cv::Mat::Mat 2 3 $::cv::CV_64FC1]
        $matrix2 setData [list 1.0 0.0 100 0.0 1.0 100]
        set img3 [::cv::warpAffine $img1 $matrix2 [$img1 cols] [$img1 rows]]

        # Wrap
        set cols [$img1 cols]
        set rows [$img1 rows]
        set matrix3 [::cv::getAffineTransform \
              [list 0 0 [expr $cols-1] 0 0 [expr $rows-1]] \
              [list 0 [expr $rows * 0.33] [expr $cols*0.85] [expr $rows*0.25] \
                   [expr $cols*0.15] [expr $rows*0.7]]]
        set img4 [::cv::warpAffine $img1 $matrix3 [$img1 cols] [$img1 rows]]

        # Output
        ::cv::imwrite "rotation.jpg" $img2
        ::cv::imwrite "translation.jpg" $img3
        ::cv::imwrite "wrap.jpg" $img4

        $matrix1 close
        $matrix2 close
        $matrix3 close
        $img1 close
        $img2 close
        $img3 close
        $img4 close
    } on error {em} {
        puts $em
    }

Morphological Operations example -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]

        set kernel [::cv::getStructuringElement $::cv::MORPH_ELLIPSE 5 5]
        set dst [::cv::morphologyEx $img $::cv::MORPH_ERODE $kernel]

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $img
        ::cv::namedWindow "Display Image 2" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image 2" $dst
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img close
        $dst close
        $kernel close
    } on error {em} {
        puts $em
    }

Below is an exmaple for Laplacian Operator:

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]

        # Remove noise by applying a Gaussian blur
        set img2 [::cv::GaussianBlur $img 3 3 0 0 $::cv::BORDER_DEFAULT]

        # Convert the original image to grayscale
        set img3 [::cv::cvtColor $img2 $::cv::COLOR_BGR2GRAY]

        # Applies a Laplacian operator to the grayscale image
        set img4 [::cv::Laplacian $img3 3 1.0 0.0 $::cv::BORDER_DEFAULT]

        # Display
        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::moveWindow "Display Image" 0 0
        ::cv::imshow "Display Image" $img4
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img close
        $img2 close
        $img3 close
        $img4 close
    } on error {em} {
        puts $em
    }

Below is an example that apply OpenCV Look Up Table (LUT) to an image -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]
        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE

        set luv_mat [cv::Mat::Mat 1 256 $::cv::CV_8UC1]
        for {set i 0} {$i < 256} {incr i} {
            if {$i > 64 && $i < 196} {
                $luv_mat at [list 0 $i] 0 0
            } else {
                $luv_mat at [list 0 $i] 0 $i
            }
        }

        set img2 [::cv::LUT $img $luv_mat]
        ::cv::imshow "Display Image" $img2

        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $luv_mat close
        $img close
        $img2 close
    } on error {em} {
        puts $em
    }

Below is an example to add 2 images:

    package require opencv

    if {$argc != 2} {
        puts "Please give 2 image file name."
        exit
    }

    set file1 [lindex $argv 0]
    set file2 [lindex $argv 1]

    try {
        set img1 [::cv::imread $file1 $::cv::IMREAD_COLOR]
        set img2 [::cv::imread $file2 $::cv::IMREAD_COLOR]

        if {[$img1 rows] < [$img2 rows] || [$img1 cols] < [$img2 cols]} {
            puts "Image 1 has to bigger than image 2."
        }

        set img1rect [$img1 rect 0 0 [$img2 cols] [$img2 rows]]
        set addimage [::cv::addWeighted $img1rect 0.5 $img2 0.5 0]
        $addimage copyTo $img1rect

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $img1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img1rect close
        $addimage close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

Play a video file -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]
    set v [::cv::VideoCapture file $filename]
    if {[$v isOpened]==0} {
        puts "Open Video file $filename failed."
        exit
    } else {
        set fps [$v get $::cv::CAP_PROP_FPS]
        puts "Frames per second : $fps"

        set frame_count [$v get $::cv::CAP_PROP_FRAME_COUNT]
        puts "Frame count : $frame_count"
    }

    while {[$v isOpened]==1} {
        try {
            set f [$v read]
            ::cv::imshow "Frame" $f
            $f close
        } on error {em} {
            break
        }

        set key [::cv::waitKey 10]
        if {$key==[scan "q" %c] || $key == 27} {
            break
        }
    }
    $v close
    ::cv::destroyAllWindows

Access camera using OpenCV and save to a video file -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set index [lindex $argv 0]
    set v [::cv::VideoCapture index $index]
    if {[$v isOpened]==0} {
        puts "Open camera $index failed."
        exit
    }

    set width [$v get 3]
    set height [$v get 4]
    set w [::cv::VideoWriter output.avi MJPG 20.0 $width $height 1]
    while {[$v isOpened]==1} {
        try {
            set f [$v read]
            ::cv::imshow "Frame" $f
            $w write $f
            $f close
        } on error {em} {
            puts $em
            break
        }

        set key [::cv::waitKey 1]
        if {$key==[scan "q" %c]} {
            break
        }
    }
    $w close
    $v close
    ::cv::destroyAllWindows

Video CamShift example -

    package require opencv

    # The example file can be downloaded from:
    # https://www.bogotobogo.com/python/OpenCV_Python/images/mean_shift_tracking/slow_traffic_small.mp4

    set filename "slow_traffic_small.mp4"
    set v [::cv::VideoCapture file $filename]
    if {[$v isOpened]==0} {
        puts "Open Video file $filename failed."
        exit
    }

    # Setup initial location of track window, simply hardcoded the values
    set x 300
    set y 200
    set width 100
    set height 50

    # calcHist parameters
    set channels [list 0]
    set histSize [list 180]
    set ranges [list 0 180]

    set frame [$v read]

    set roi [$frame crop $x $y $width $height]
    set hsv_roi [::cv::cvtColor $roi $::cv::COLOR_BGR2HSV]
    set mask [::cv::inRange $hsv_roi [list 0 60 32 0] [list 180 255 255 0]]
    set hsv_hist [::cv::calcHist $hsv_roi $channels $mask 1 $histSize $ranges]
    set roi_hist [::cv::normalize $hsv_hist 0 255 $::cv::NORM_MINMAX]

    $frame close
    $roi close
    $hsv_roi close
    $mask close
    $hsv_hist close

    set term [::cv::TermCriteria [expr $::cv::EPS | $::cv::COUNT] 10 1]

    while {[$v isOpened]==1} {
        try {
            set frame [$v read]

            if {$x < 550 || $y < 200} {
                set hsv [cv::cvtColor $frame $::cv::COLOR_BGR2HSV]
                set dst [cv::calcBackProject $hsv [list 0] $roi_hist [list 0 180]]
                set result [::cv::CamShift $dst $x $y $width $height $term]
                set x [lindex [lindex $result 0] 0]
                set y [lindex [lindex $result 0] 1]
                set width [lindex [lindex $result 0] 2]
                set hehgit [lindex [lindex $result 0] 3]

                set points [lindex $result 1]
                set p0_x [lindex $points 0]
                set p0_y [lindex $points 1]
                set p1_x [lindex $points 2]
                set p1_y [lindex $points 3]
                set p2_x [lindex $points 4]
                set p2_y [lindex $points 5]
                set p3_x [lindex $points 6]
                set p3_y [lindex $points 7]

                ::cv::line $frame $p0_x $p0_y $p1_x $p1_y [list 255 0 0 0] 3
                ::cv::line $frame $p1_x $p1_y $p2_x $p2_y [list 255 0 0 0] 3
                ::cv::line $frame $p2_x $p2_y $p3_x $p3_y [list 255 0 0 0] 3
                ::cv::line $frame $p3_x $p3_y $p0_x $p0_y [list 255 0 0 0] 3

                $hsv close
                $dst close
            }

            ::cv::imshow "Frame" $frame

            $frame close
        } on error {em} {
            break
        }

        set key [::cv::waitKey 10]
        if {$key==[scan "q" %c] || $key == 27} {
            break
        }
    }
    $term close
    $v close
    ::cv::destroyAllWindows

Lucas-Kanade Optical Flow in OpenCV -

    package require opencv

    # The example file can be downloaded from:
    # https://www.bogotobogo.com/python/OpenCV_Python/images/mean_shift_tracking/slow_traffic_small.mp4

    set filename "slow_traffic_small.mp4"
    set v [::cv::VideoCapture file $filename]
    if {[$v isOpened]==0} {
        puts "Open Video file $filename failed."
        exit
    }

    # Create some random colors
    set colors [list]
    for {set i 0} {$i < 100} {incr i} {
        lappend colors [list [expr int(rand() * 255)] [expr int(rand() * 255)] [expr int(rand() * 255)] 0]
    }

    set frame [$v read]
    set oldGray [::cv::cvtColor $frame $::cv::COLOR_BGR2GRAY]
    set emptymask [::cv::Mat::Mat 0 0 $::cv::CV_8U]
    set p0 [::cv::goodFeaturesToTrack $oldGray 100 0.3 7 $emptymask 7 0 0.04]

    # Create a mask image for drawing purposes
    set mask [::cv::Mat::Mat [$frame rows] [$frame cols] [$frame type] [list 0 0 0 0]]

    $emptymask close
    $frame close

    while {[$v isOpened]==1} {
        try {
            set term [::cv::TermCriteria [expr $::cv::EPS | $::cv::COUNT] 10 0.03]
            set frame [$v read]
            set frameGray [::cv::cvtColor $frame $::cv::COLOR_BGR2GRAY]

            set result [::cv::calcOpticalFlowPyrLK $oldGray $frameGray $p0 15 15 2 $term]
            set p1 [lindex $result 0]
            set status [lindex $result 1]
            set err [lindex $result 2]

            for {set i 0} {$i < [$p1 rows]} {incr i} {
                for {set j 0} {$j < [$p1 cols]} {incr j} {
                    set oldx [expr int([$p0 at [list $i $j] 0])]
                    set oldy [expr int([$p0 at [list $i $j] 1])]
                    set x [expr int([$p1 at [list $i $j] 0])]
                    set y [expr int([$p1 at [list $i $j] 1])]

                    if {[$status at [list $i $j] 0]==1} {
                        ::cv::line $mask $x $y $oldx $oldy [lindex $colors $i] 2
                        ::cv::circle $frame $x $y 5 [lindex $colors $i] -1
                    }
                }
            }
            set frame2 [::cv::add $frame $mask]

            ::cv::imshow "Frame" $frame2

            $err close
            $status close
            $p0 close
            $oldGray close

            set p0 $p1
            set oldGray $frameGray

            $frame close
            $frame2 close
            $term close
        } on error {em} {
            break
        }

        set key [::cv::waitKey 10]
        if {$key==[scan "q" %c] || $key == 27} {
            break
        }
    }
    $p1 close
    $frameGray close
    $v close
    ::cv::destroyAllWindows

Dense Optical Flow Example -

    package require opencv

    # The example file can be downloaded from:
    # https://github.com/opencv/opencv/blob/master/samples/data/vtest.avi

    set filename "vtest.avi"
    set v [::cv::VideoCapture file $filename]
    if {[$v isOpened]==0} {
        puts "Open Video file $filename failed."
        exit
    }

    set frame [$v read]
    set oldGray [::cv::cvtColor $frame $::cv::COLOR_BGR2GRAY]
    $frame close

    while {[$v isOpened]==1} {
        try {
            set frame [$v read]
            set frameGray [::cv::cvtColor $frame $::cv::COLOR_BGR2GRAY]

            set flow [cv::calcOpticalFlowFarneback $oldGray $frameGray 0.5 3 15 3 5 1.2 0]
            set flowlist [cv::split $flow]
            set uvalue [lindex $flowlist 0]
            set vvalue [lindex $flowlist 1]

            set result [cv::cartToPolar $uvalue $vvalue 1]
            set mag [lindex $result 0]
            set ang [lindex $result 1]

            set hsv0 [$ang multiply [expr ((1.0 / 360.0) * (180.0 / 255.0))]]
            set hsv1 [::cv::Mat::ones [$frame rows] [$frame cols] $::cv::CV_32F]
            set hsv2 [cv::normalize $mag 0 1 $::cv::NORM_MINMAX]
            set hsv [::cv::merge [list $hsv0 $hsv1 $hsv2]]

            set hsv8 [$hsv convertTo $::cv::CV_8U 255.0 0]
            set frame2 [::cv::cvtColor $hsv8 $::cv::COLOR_HSV2BGR]
            ::cv::imshow "Frame" $frame2

            $flow close
            $uvalue close
            $vvalue close
            $mag close
            $ang close
            $hsv0 close
            $hsv1 close
            $hsv2 close
            $hsv close
            $hsv8 close
            $oldGray close

            set oldGray $frameGray

            $frame close
            $frame2 close
        } on error {em} {
            break
        }

        set key [::cv::waitKey 10]
        if {$key==[scan "q" %c] || $key == 27} {
            break
        }
    }
    $frameGray close
    $v close
    ::cv::destroyAllWindows

Below is a selectROI example -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]

        set rect [::cv::selectROI $img]
        set x [lindex $rect 0]
        set y [lindex $rect 1]
        set width [lindex $rect 2]
        set height [lindex $rect 3]

        # Create a mask for the ROI
        set mask [::cv::Mat::zeros [$img rows] [$img cols] [$img type]]
        ::cv::rectangle $mask $x $y [expr $x+$width] [expr $y+$height] [list 255 255 255 0] -1

        set img2 [::cv::colorChange $img $mask 2.0 0.5 0.5]

        ::cv::namedWindow "Display Result" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Result" $img2
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img close
        $mask close
        $img2 close
    } on error {em} {
        puts $em
    }

Below is a setMouseCallback test -

    package require opencv

    if {$argc != 1} {
        exit
    }

    proc drawit {event x y flags} {
        if {$event == $::cv::EVENT_LBUTTONUP} {
            set scalar [list 0 0 255 0]
            ::cv::circle $::img $x $y 25 $scalar 3 $::cv::LINE_8 0
            ::cv::imshow "Display Image" $::img
        }
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]
        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::setMouseCallback "Display Image" drawit
        ::cv::imshow "Display Image" $img
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img close
    } on error {em} {
        puts $em
    }

Below is a createTrackbar test -

    package require opencv

    if {$argc != 1} {
        exit
    }

    proc changeit {value} {
        set img2 [::cv::Sobel $::img 1 0 $value 1.0 0.0 $::cv::BORDER_DEFAULT]
        ::cv::imshow "Display Image" $img2
        $img2 close
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]
        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::createTrackbar "Trackbar1" "Display Image" 3 21 changeit
        changeit 3

        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img close
    } on error {em} {
        puts $em
    }

Contours test -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set image1 [::cv::imread $filename]
        set image2 [::cv::cvtColor $image1 $::cv::COLOR_RGB2GRAY]
        set image3 [::cv::threshold $image2 20 255 $::cv::THRESH_BINARY]
        set contours [::cv::findContours $image3 $::cv::RETR_EXTERNAL $::cv::CHAIN_APPROX_SIMPLE]

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $image1

        ::cv::namedWindow "Display Image 2" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image 2" $image3

        set length [llength $contours]
        set max 0
        set maxindex 0
        set bounding_rect [list]
        set mycircle [list]
        set myrect [list]
        set myellipse [list]
        set isellipse 1
        for {set i 0} {$i < $length} {incr i} {
            set value [::cv::contourArea [lindex $contours $i] 0]
            if {$value > $max} {
                set max $value
                set maxindex $i
                set contour [lindex $contours $i]
                set bounding_rect [::cv::boundingRect $contour]
                set mycircle [::cv::minEnclosingCircle $contour]

                if {[catch {set myellipse [::cv::fitEllipse $contour]}]} {
                    set myrect [::cv::minAreaRect $contour]
                    set isellipse 0
                }
            }
        }

        ::cv::drawContours $image1 $contours -1 [list 255 0 0 0] -1 $::cv::LINE_8 2 0 0

        set x1 [lindex $bounding_rect 0]
        set y1 [lindex $bounding_rect 1]
        set x2 [expr $x1 + [lindex $bounding_rect 2]]
        set y2 [expr $y1 + [lindex $bounding_rect 3]]
        set color [list 255 255 0 0]
        ::cv::rectangle $image1 $x1 $y1 $x2 $y2 $color 1

        set x1 [lindex $mycircle 0]
        set y1 [lindex $mycircle 1]
        set r [lindex $mycircle 2]
        set color [list 0 255 255 0]
        ::cv::circle $image1 $x1 $y1 $r $color 1

        set color [list 255 0 255 0]
        if {$isellipse} {
            set x1 [lindex $myellipse 0]
            set y1 [lindex $myellipse 1]
            set w [expr [lindex $myellipse 2]/2]
            set h [expr [lindex $myellipse 3]/2]
            set r [lindex $myellipse 4]

            ::cv::ellipse $image1 $x1 $y1 $w $h $r 0 360 $color 3
        } else {
            set box [::cv::boxPoints $myrect]
            ::cv::drawContours $image1 [list $box] -1 $color 3 $::cv::LINE_8 2 0 0
        }

        ::cv::namedWindow "Display Image 3" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image 3" $image1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $image3 close
        $image2 close
        $image1 close
    } on error {em} {
        puts $em
    }

Convex Hull test -

    package require opencv

    proc mysortproc {c1 c2} {
        set value1 [::cv::contourArea $c1 0]
        set value2 [::cv::contourArea $c2 0]

        if {$value1 > $value2} {
            return -1
        } elseif {$value1 < $value2} {
            return 1
        } else {
            return 0
        }
    }

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set image1 [::cv::imread $filename]

        set skinColorUpper [list 15 204 153 0]
        set skinColorLower [list 0 25 13 0]

        # Try to filter
        set hlsimage [cv::cvtColor $image1 $::cv::COLOR_BGR2HLS]
        set rangeMask [cv::inRange $hlsimage $skinColorLower $skinColorUpper]
        set blurred [cv::blur $rangeMask 10 10]
        set image2 [::cv::threshold $blurred 200 255 $::cv::THRESH_BINARY]

        $hlsimage close
        $rangeMask close
        $blurred close

        set contours [::cv::findContours $image2 $::cv::RETR_EXTERNAL $::cv::CHAIN_APPROX_SIMPLE]
        set scontours [lsort -command mysortproc $contours]
        set contour [lindex $scontours 0]

        set hull [cv::convexHull $contour 0 1]
        for {set i 0} {$i < [llength $hull]} {incr i 2} {
            set x [lindex $hull $i]
            set y [lindex $hull [expr $i + 1]]
            cv::circle $image1 $x $y 5 [list 255 0 0 0] 3
        }

        ::cv::drawContours $image1 [list $hull] -1 [list 0 255 0 0] 1 $::cv::LINE_AA 2 0 0

        set hull [cv::convexHull $contour 0 0]
        set defects [::cv::convexityDefects $contour $hull]
        foreach d $defects {
            set startx [lindex $contour [expr [lindex $d 0] * 2]]
            set starty [lindex $contour [expr [lindex $d 0] * 2 + 1]]
            set endx [lindex $contour [expr [lindex $d 1] * 2]]
            set endy [lindex $contour [expr [lindex $d 1] * 2 + 1]]
            set farx [lindex $contour [expr [lindex $d 2] * 2]]
            set fary [lindex $contour [expr [lindex $d 2] * 2 + 1]]

            set depth [expr [lindex $d 3]/256]

            cv::line $image1 $startx $starty $farx $fary [list 0 0 255 0] 2
            cv::line $image1 $endx $endy $farx $fary [list 0 0 255 0] 2
            cv::circle $image1 $farx $fary 5 [list 255 255 0 0] 3
        }

        ::cv::namedWindow "Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Image" $image1
        ::cv::namedWindow "Check" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Check" $image2

        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $image2 close
        $image1 close
    } on error {em} {
        puts $em
    }

GeneralizedHoughBallard test -

    package require opencv

    try {
        #
        # Download files from:
        # https://github.com/opencv/opencv/tree/master/samples/data
        #
        set img [::cv::imread "pic1.png" 0]
        set templ [::cv::imread "templ.png" 0]

        set hough [::cv::GeneralizedHoughBallard]
        $hough setMinDist 100
        $hough setLevels  360
        $hough setDp 2
        $hough setVotesThreshold 50
        $hough setTemplate $templ
        set result [$hough detect $img]
        set pos [lindex $result 0]
        set votes [lindex $result 1]

        set img2 [::cv::cvtColor $img $::cv::COLOR_GRAY2BGR]
        for {set i 0} {$i < [$pos rows]} {incr i} {
            for {set j 0} {$j < [$pos cols]} {incr j} {
                set x [expr int([$pos at [list $i $j] 0])]
                set y [expr int([$pos at [list $i $j] 1])]
                set scale [$pos at [list $i $j] 2]
                set angle [$pos at [list $i $j] 3]

                set h [expr int($scale * [$templ rows])]
                set w [expr int($scale * [$templ cols])]

                set rect [list $x $y $w $h $angle]
                set box [::cv::boxPoints $rect]

                set color [list 0 0 255 0]
                ::cv::drawContours $img2 [list $box] -1 $color 3
            }
        }

        $pos close
        $votes close
        $hough close
        $templ close
        $img close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $img2
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $img2 close
    } on error {em} {
        puts $em
    }

AGAST Algorithm for Corner Detection -

    package require opencv

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "blox.jpg"

    try {
        set img1 [::cv::imread $filename1 0]

        set agast [::cv::AgastFeatureDetector]
        set kp1 [$agast detect $img1]

        set kpoint1 [::cv::drawKeypoints $img1 $kp1 None [list 255 0 0 0] \
                    $::cv::DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS]

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $kpoint1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $kpoint1 close
        $img1 close
    } on error {em} {
        puts $em
    }

Brute-Force Matching with ORB Descriptors -

    package require opencv

    proc mysortproc {x y} {
        set distance1 [lindex $x 3]
        set distance2 [lindex $y 3]

        if {$distance1 > $distance2} {
            return 1
        } elseif {$distance1 < $distance2} {
            return -1
        } else {
            return 0
        }
    }

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "box.png"
    set filename2 "box_in_scene.png"

    try {
        set img1 [::cv::imread $filename1 0]
        set img2 [::cv::imread $filename2 0]

        set orb [::cv::ORB]

        set result1 [$orb detectAndCompute $img1]
        set result2 [$orb detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set bmatcher [::cv::BFMatcher $::cv::NORM_HAMMING 1]
        set match [$bmatcher match $d1 $d2]
        set match [lsort -command mysortproc $match]
        set matches [lrange $match 0 10]

        set mcolor [list 255 0 0 0]
        set scolor [list 0 0 255 0]
        set match1 [::cv::drawMatches $img1 $kp1 $img2 $kp2 $matches None \
                    $mcolor $scolor $::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS]

        $d1 close
        $d2 close
        $orb close
        $bmatcher close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $match1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $match1 close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

Flann-based descriptor matcher with BRISK Descriptors -

    package require opencv

    proc mysortproc {x y} {
        set distance1 [lindex $x 3]
        set distance2 [lindex $y 3]

        if {$distance1 > $distance2} {
            return 1
        } elseif {$distance1 < $distance2} {
            return -1
        } else {
            return 0
        }
    }

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "box.png"
    set filename2 "box_in_scene.png"

    try {
        set img1 [::cv::imread $filename1 0]
        set img2 [::cv::imread $filename2 0]

        set brisk [::cv::BRISK]

        set result1 [$brisk detectAndCompute $img1]
        set result2 [$brisk detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set fmatcher [::cv::FlannBasedMatcher FLANN_INDEX_LSH [list 6 12 1]]
        set match [$fmatcher match $d1 $d2]
        set match [lsort -command mysortproc $match]
        set matches [lrange $match 0 10]

        set mcolor [list 255 0 0 0]
        set scolor [list 0 0 255 0]
        set match1 [::cv::drawMatches $img1 $kp1 $img2 $kp2 $matches None \
                    $mcolor $scolor $::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS]

        $d1 close
        $d2 close
        $brisk close
        $fmatcher close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $match1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $match1 close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

Brute-Force Matching with ORB Descriptors and Ratio Test -

    package require opencv

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "box.png"
    set filename2 "box_in_scene.png"

    try {
        set img1 [::cv::imread $filename1 0]
        set img2 [::cv::imread $filename2 0]

        set orb [::cv::ORB]
        $orb setWTA_K 3

        set result1 [$orb detectAndCompute $img1]
        set result2 [$orb detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set bmatcher [::cv::BFMatcher $::cv::NORM_HAMMING 0]
        set matches [$bmatcher knnMatch $d1 $d2 2]

        # Apply ratio test
        set good [list]
        foreach match $matches {
            foreach {m n} $match {
                set mdistance [lindex $m 3]
                set ndistance [lindex $n 3]
                if {$mdistance < [expr 0.7 * $ndistance]} {
                    lappend good $m
                }
            }
        }

        set mcolor [list 255 0 0 0]
        set scolor [list 0 0 255 0]
        set match1 [::cv::drawMatches $img1 $kp1 $img2 $kp2 $good None \
                    $mcolor $scolor $::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS]

        $d1 close
        $d2 close
        $orb close
        $bmatcher close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $match1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $match1 close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

Flann-based descriptor matcher with AKAZE Descriptors and Ratio Test -

    package require opencv

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "box.png"
    set filename2 "box_in_scene.png"

    try {
        set img1 [::cv::imread $filename1 0]
        set img2 [::cv::imread $filename2 0]

        set akaze [::cv::AKAZE]

        set result1 [$akaze detectAndCompute $img1]
        set result2 [$akaze detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set fmatcher [::cv::FlannBasedMatcher FLANN_INDEX_LSH [list 6 12 1]]
        set matches [$fmatcher knnMatch $d1 $d2 2]

        # Apply ratio test
        set good [list]
        foreach match $matches {
            foreach {m n} $match {
                set mdistance [lindex $m 3]
                set ndistance [lindex $n 3]
                if {$mdistance < [expr 0.7 * $ndistance]} {
                    lappend good $m
                }
            }
        }

        set mcolor [list 255 0 0 0]
        set scolor [list 0 0 255 0]
        set match1 [::cv::drawMatches $img1 $kp1 $img2 $kp2 $good None \
                    $mcolor $scolor $::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS]

        $d1 close
        $d2 close
        $akaze close
        $fmatcher close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $match1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $match1 close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

ORB, Feature Matching+ Homography to find Objects -

    package require opencv

    proc mysortproc {x y} {
        set distance1 [lindex $x 3]
        set distance2 [lindex $y 3]

        if {$distance1 > $distance2} {
            return 1
        } elseif {$distance1 < $distance2} {
            return -1
        } else {
            return 0
        }
    }

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "box.png"
    set filename2 "box_in_scene.png"

    try {
        set img1 [::cv::imread $filename1 0]
        set img2 [::cv::imread $filename2 0]

        set orb [::cv::ORB]

        set result1 [$orb detectAndCompute $img1]
        set result2 [$orb detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set bmatcher [::cv::BFMatcher $::cv::NORM_HAMMING 1]
        set match [$bmatcher match $d1 $d2]
        set match [lsort -command mysortproc $match]
        set dmatches [lrange $match 0 20]

        set srcPts [list]
        set dstPts [list]
        for {set i 0} {$i < [llength $dmatches]} {incr i} {
            set m [lindex $match $i]
            set spoint [lindex $kp1 [lindex $m 0]]
            set dpoint [lindex $kp2 [lindex $m 1]]

            lappend srcPts [lindex $spoint 0] [lindex $spoint 1]
            lappend dstPts [lindex $dpoint 0] [lindex $dpoint 1]
        }

        # Find homography matrix and do perspective transform
        set src_pts [::cv::Mat::Mat 1 [expr [llength $srcPts]/2] $::cv::CV_32FC2]
        $src_pts setData $srcPts
        set dst_pts [::cv::Mat::Mat 1 [expr [llength $dstPts]/2] $::cv::CV_32FC2]
        $dst_pts setData $dstPts
        set MRes [::cv::findHomography $src_pts $dst_pts $::cv::RANSAC 5.0 2000 0.995]
        set M [lindex $MRes 0]
        set Mask [lindex $MRes 1]
        if {![$M empty]} {
            set h [$img1 rows]
            set w [$img1 cols]
            set pts [list 0 0 0 [expr $h-1] [expr $w-1] [expr $h-1] [expr $w-1] 0]
            set dst [::cv::perspectiveTransform $pts $M]
            ::cv::polylines $img2 $dst 1 1 [list 255 255 255 0] 5
        }

        $M close
        $Mask close
        $src_pts close
        $dst_pts close

        set mcolor [list 255 0 0 0]
        set scolor [list 0 0 255 0]
        set match1 [::cv::drawMatches $img1 $kp1 $img2 $kp2 $dmatches None \
                    $mcolor $scolor $::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS]

        $d1 close
        $d2 close
        $orb close
        $bmatcher close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $match1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $match1 close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

AKAZE, Feature Matching + Homography to find Objects -

    package require opencv

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "box.png"
    set filename2 "box_in_scene.png"

    try {
        set img1 [::cv::imread $filename1 0]
        set img2 [::cv::imread $filename2 0]

        set akaze [::cv::AKAZE]

        set result1 [$akaze detectAndCompute $img1]
        set result2 [$akaze detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set bmatcher [::cv::BFMatcher $::cv::NORM_HAMMING 0]
        set matches [$bmatcher knnMatch $d1 $d2 2]

        # Apply ratio test
        set dmatches [list]
        foreach match $matches {
            foreach {m n} $match {
                set mdistance [lindex $m 3]
                set ndistance [lindex $n 3]
                if {$mdistance < [expr 0.7 * $ndistance]} {
                    lappend dmatches $m
                }
            }
        }

        set srcPts [list]
        set dstPts [list]
        foreach match $dmatches {
            set spoint [lindex $kp1 [lindex $match 0]]
            set dpoint [lindex $kp2 [lindex $match 1]]

            lappend srcPts [lindex $spoint 0] [lindex $spoint 1]
            lappend dstPts [lindex $dpoint 0] [lindex $dpoint 1]
        }

        # Find homography matrix and do perspective transform
        set src_pts [::cv::Mat::Mat 1 [expr [llength $srcPts]/2] $::cv::CV_32FC2]
        $src_pts setData $srcPts
        set dst_pts [::cv::Mat::Mat 1 [expr [llength $dstPts]/2] $::cv::CV_32FC2]
        $dst_pts setData $dstPts
        set MRes [::cv::findHomography $src_pts $dst_pts $::cv::RANSAC 5.0 2000 0.995]
        set M [lindex $MRes 0]
        set Mask [lindex $MRes 1]
        if {![$M empty]} {
            set h [$img1 rows]
            set w [$img1 cols]
            set pts [list 0 0 0 [expr $h-1] [expr $w-1] [expr $h-1] [expr $w-1] 0]
            set dst [::cv::perspectiveTransform $pts $M]
            ::cv::polylines $img2 $dst 1 1 [list 255 255 255 0] 5
        }

        $M close
        $Mask close
        $src_pts close
        $dst_pts close

        set mcolor [list 255 0 0 0]
        set scolor [list 0 0 255 0]
        set match1 [::cv::drawMatches $img1 $kp1 $img2 $kp2 $dmatches None \
                    $mcolor $scolor $::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS]

        $d1 close
        $d2 close
        $akaze close
        $bmatcher close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $match1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $match1 close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

SIFT, Feature Matching + Homography to find Objects -

    package require opencv

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "box.png"
    set filename2 "box_in_scene.png"

    try {
        set img1 [::cv::imread $filename1 0]
        set img2 [::cv::imread $filename2 0]

        set sift [::cv::SIFT]

        set result1 [$sift detectAndCompute $img1]
        set result2 [$sift detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set fmatcher [::cv::FlannBasedMatcher FLANN_INDEX_KDTREE [list 5]]
        set matches [$fmatcher knnMatch $d1 $d2 2]

        # Apply ratio test
        set dmatches [list]
        foreach match $matches {
            foreach {m n} $match {
                set mdistance [lindex $m 3]
                set ndistance [lindex $n 3]
                if {$mdistance < [expr 0.7 * $ndistance]} {
                    lappend dmatches $m
                }
            }
        }

        set srcPts [list]
        set dstPts [list]
        foreach match $dmatches {
            set spoint [lindex $kp1 [lindex $match 0]]
            set dpoint [lindex $kp2 [lindex $match 1]]

            lappend srcPts [lindex $spoint 0] [lindex $spoint 1]
            lappend dstPts [lindex $dpoint 0] [lindex $dpoint 1]
        }

        # Find homography matrix and do perspective transform
        set src_pts [::cv::Mat::Mat 1 [expr [llength $srcPts]/2] $::cv::CV_32FC2]
        $src_pts setData $srcPts
        set dst_pts [::cv::Mat::Mat 1 [expr [llength $dstPts]/2] $::cv::CV_32FC2]
        $dst_pts setData $dstPts
        set MRes [::cv::findHomography $src_pts $dst_pts $::cv::RANSAC 5.0 2000 0.995]
        set M [lindex $MRes 0]
        set Mask [lindex $MRes 1]
        if {![$M empty]} {
            set h [$img1 rows]
            set w [$img1 cols]
            set pts [list 0 0 0 [expr $h-1] [expr $w-1] [expr $h-1] [expr $w-1] 0]
            set dst [::cv::perspectiveTransform $pts $M]
            ::cv::polylines $img2 $dst 1 1 [list 255 255 255 0] 5
        }

        $M close
        $Mask close
        $src_pts close
        $dst_pts close

        set mcolor [list 255 0 0 0]
        set scolor [list 0 0 255 0]
        set match1 [::cv::drawMatches $img1 $kp1 $img2 $kp2 $dmatches None \
                    $mcolor $scolor $::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS]

        $d1 close
        $d2 close
        $sift close
        $fmatcher close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $match1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $match1 close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }


AffineFeature with KAZE, Feature Matching + Homography to find Objects -

    package require opencv

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "box.png"
    set filename2 "box_in_scene.png"

    try {
        set img1 [::cv::imread $filename1 0]
        set img2 [::cv::imread $filename2 0]

        set backend [::cv::KAZE]
        set asift [::cv::AffineFeature $backend]

        set result1 [$asift detectAndCompute $img1]
        set result2 [$asift detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set matcher [::cv::FlannBasedMatcher FLANN_INDEX_KDTREE [list 5]]
        set matches [$matcher knnMatch $d1 $d2 2]

        # Apply ratio test
        set dmatches [list]
        foreach match $matches {
            foreach {m n} $match {
                set mdistance [lindex $m 3]
                set ndistance [lindex $n 3]
                if {$mdistance < [expr 0.7 * $ndistance]} {
                    lappend dmatches $m
                }
            }
        }

        set srcPts [list]
        set dstPts [list]
        foreach match $dmatches {
            set spoint [lindex $kp1 [lindex $match 0]]
            set dpoint [lindex $kp2 [lindex $match 1]]

            lappend srcPts [lindex $spoint 0] [lindex $spoint 1]
            lappend dstPts [lindex $dpoint 0] [lindex $dpoint 1]
        }

        # Find homography matrix and do perspective transform
        set src_pts [::cv::Mat::Mat 1 [expr [llength $srcPts]/2] $::cv::CV_32FC2]
        $src_pts setData $srcPts
        set dst_pts [::cv::Mat::Mat 1 [expr [llength $dstPts]/2] $::cv::CV_32FC2]
        $dst_pts setData $dstPts
        set MRes [::cv::findHomography $src_pts $dst_pts $::cv::RANSAC 5.0 2000 0.995]
        set M [lindex $MRes 0]
        set Mask [lindex $MRes 1]
        if {![$M empty]} {
            set h [$img1 rows]
            set w [$img1 cols]
            set pts [list 0 0 0 [expr $h-1] [expr $w-1] [expr $h-1] [expr $w-1] 0]
            set dst [::cv::perspectiveTransform $pts $M]
            ::cv::polylines $img2 $dst 1 1 [list 255 255 255 0] 5
        }

        $M close
        $Mask close
        $src_pts close
        $dst_pts close

        set mcolor [list 255 0 0 0]
        set scolor [list 0 0 255 0]
        set match1 [::cv::drawMatches $img1 $kp1 $img2 $kp2 $dmatches None \
                    $mcolor $scolor $::cv::DRAW_MATCHES_FLAGS_NOT_DRAW_SINGLE_POINTS]

        $d1 close
        $d2 close
        $asift close
        $backend close
        $matcher close

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $match1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $match1 close
        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

Camera Calibration test -

    package require opencv

    set term [::cv::TermCriteria [expr $::cv::EPS | $::cv::COUNT] 30 0.001]

    # prepare object points
    set objp [::cv::Mat::zeros [expr 6*9] 3 $::cv::CV_32F]
    for {set i 0} {$i < 9} {incr i} {
        for {set j 0} {$j < 6} {incr j} {
            $objp at [list [expr $i * 6 + $j] 0] 0 $j
            $objp at [list [expr $i * 6 + $j] 1] 0 $i
            $objp at [list [expr $i * 6 + $j] 2] 0 0
        }
    }

    set objpoints [list]
    set imgpoints [list]
    set width 0
    set height 0

    #
    # Download files from:
    # https://github.com/opencv/opencv/tree/master/samples/data
    # left01.jpg ~ left14.jpg (except left12.jpg)
    #
    set filenames [glob data/left*.jpg]
    foreach file $filenames {
        set img [::cv::imread $file]
        set gray [cv::cvtColor $img $::cv::COLOR_BGR2GRAY]
        set res [cv::findChessboardCorners $gray 6 9]
        set ret [lindex $res 0]
        set corners [lindex $res 1]

        if {$ret == 1} {
            cv::cornerSubPix $gray $corners 11 11 -1 -1 $term

            set width [$img cols]
            set height [$img rows]

            cv::drawChessboardCorners $img 6 9 $corners $ret
            cv::imshow "img" $img
            cv::waitKey 500

            lappend imgpoints $corners
            lappend objpoints $objp
        }
        $img close
        $gray close
    }

    $term close

    set cameraMatrix [::cv::Mat::eye 3 3 $::cv::CV_64F]
    set distCoeffs [::cv::Mat::zeros 5 1 $::cv::CV_64F]

    set res [cv::calibrateCamera $objpoints $imgpoints $width $height $cameraMatrix $distCoeffs]
    set ret [lindex $res 0]
    set cameraMatrix [lindex $res 1]
    set distCoeffs [lindex $res 2]
    set rvecs [lindex $res 3]
    set tvecs [lindex $res 4]

    # Save to FileStorage
    set fs [::cv::FileStorage]
    $fs open "calibration.yaml" $::cv::FileStorage::WRITE
    $fs writeMat "mtx" $cameraMatrix
    $fs writeMat "dist" $distCoeffs
    $fs close

    # Undistortion
    set img [::cv::imread "left12.jpg"]
    set width [$img cols]
    set height [$img rows]
    set result [::cv::getOptimalNewCameraMatrix $cameraMatrix $distCoeffs \
                                                $width $height 1 $width $height]
    set newcameramtx [lindex $result 0]
    set roix [lindex $result 1]
    set roiy [lindex $result 2]
    set roiw [lindex $result 3]
    set roih [lindex $result 4]

    set dst [::cv::undistort $img $cameraMatrix $distCoeffs $newcameramtx]
    set dst2 [$dst rect $roix $roiy $roiw $roih]
    ::cv::imwrite "calibresult.png" $dst2
    $dst close
    $dst2 close

    set R [cv::Mat::Mat 0 0 $::cv::CV_32F]
    set rdst [::cv::initUndistortRectifyMap $cameraMatrix $distCoeffs $R $newcameramtx \
                                            $width $height $::cv::CV_32F]
    set mapx [lindex $rdst 0]
    set mapy [lindex $rdst 1]
    set dst [::cv::remap $img $mapx $mapy $::cv::INTER_LINEAR]
    set dst2 [$dst rect $roix $roiy $roiw $roih]
    ::cv::imwrite "calibresult2.png" $dst2
    $mapx close
    $mapy close
    $dst close
    $dst2 close
    $R close
    $newcameramtx close
    $img close

    $cameraMatrix close
    $distCoeffs close
    foreach rvec $rvecs {
        $rvec close
    }
    foreach tvec $tvecs {
        $tvec close
    }
    $objp close
    foreach imgp $imgpoints {
        $imgp close
    }

Pose Estimation, Render a Cube -

    package require opencv

    proc draw {img imgpts} {
        set imgpts_c [list]
        set imgpts_sub [list]
        for {set i 0} {$i < 4} {incr i} {
            set img_x [expr int([$imgpts at [list $i 0] 0])]
            set img_y [expr int([$imgpts at [list $i 0] 1])]
            lappend imgpts_sub $img_x $img_y
        }
        lappend imgpts_c $imgpts_sub

        cv::drawContours $img $imgpts_c -1 [list 0 255 0 0] -3 $::cv::LINE_8 2 0 0

        for {set i 0; set j 4} {$i < 4} {incr i; incr j} {
            set img_x1 [expr int([$imgpts at [list $i 0] 0])]
            set img_y1 [expr int([$imgpts at [list $i 0] 1])]
            set img_x2 [expr int([$imgpts at [list $j 0] 0])]
            set img_y2 [expr int([$imgpts at [list $j 0] 1])]

            cv::line $img $img_x1 $img_y1 $img_x2 $img_y2 [list 255 0 0 0] 3
        }

        set imgpts_c [list]
        set imgpts_sub [list]
        for {set i 4} {$i < 8} {incr i} {
            set img_x [expr int([$imgpts at [list $i 0] 0])]
            set img_y [expr int([$imgpts at [list $i 0] 1])]
            lappend imgpts_sub $img_x $img_y
        }
        lappend imgpts_c $imgpts_sub

        cv::drawContours $img $imgpts_c -1 [list 0 0 255 0] 3 $::cv::LINE_8 2 0 0
    }


    # Load data
    set fs [::cv::FileStorage ]
    $fs open "calibration.yaml" $::cv::FileStorage::READ
    set cameraMatrix [$fs readMat "mtx"]
    set distCoeffs [$fs readMat "dist"]
    $fs close

    # prepare object points
    set objp [::cv::Mat::zeros [expr 6*9] 3 $::cv::CV_32F]
    for {set i 0} {$i < 9} {incr i} {
        for {set j 0} {$j < 6} {incr j} {
            $objp at [list [expr $i * 6 + $j] 0] 0 $j
            $objp at [list [expr $i * 6 + $j] 1] 0 $i
            $objp at [list [expr $i * 6 + $j] 2] 0 0
        }
    }

    set axis [cv::Mat::Mat 8 3 $::cv::CV_32F]
    $axis setData [list 0 0 0 0 3 0 3 3 0 3 0 0 \
                        0 0 -3 0 3 -3 3 3 -3 3 0 -3]

    set term [::cv::TermCriteria [expr $::cv::EPS | $::cv::COUNT] 30 0.001]

    set filenames [glob data/left*.jpg]
    foreach file $filenames {
        set img [::cv::imread $file]
        set gray [cv::cvtColor $img $::cv::COLOR_BGR2GRAY]
        set res [cv::findChessboardCorners $gray 6 9]
        set ret [lindex $res 0]
        set corners [lindex $res 1]

        if {$ret == 1} {
            cv::cornerSubPix $gray $corners 11 11 -1 -1 $term

            set r [cv::solvePnP $objp $corners $cameraMatrix $distCoeffs]
            set rvec [lindex $r 1]
            set tvec [lindex $r 2]
            set imgpts [cv::projectPoints $axis $rvec $tvec $cameraMatrix $distCoeffs]

            draw $img $imgpts

            cv::imshow "img" $img
            cv::waitKey 500

            $imgpts close
            $rvec close
            $tvec close
        }

        $corners close
        $img close
        $gray close
    }

    $term close
    $objp close
    $axis close

Epipolar Geometry -

    package require opencv

    proc drawlines {img1 lines pts1 pts2} {
        set col [$img1 cols]
        set row [$img1 rows]

        for {set i 0} {$i < [$lines rows]} {incr i} {
            set color [list [expr int(rand()*255)] [expr int(rand()*255)] [expr int(rand()*255)] 0]
            set r0 [$lines at [list $i 0] 0]
            set r1 [$lines at [list $i 0] 1]
            set r2 [$lines at [list $i 0] 2]

            set x0 0
            set y0 [expr int(-$r2/$r1)]
            set x1 $col
            set y1 [expr int(-($r2+$r0*$col)/$r1)]
            ::cv::line $img1 $x0 $y0 $x1 $y1 $color 1

            set p1 [$pts1 at [list $i 0] 0]
            set p2 [$pts1 at [list $i 0] 1]
            ::cv::circle $img1 $p1 $p2 5 $color -1
        }
    }

    #
    # From https://github.com/opencv/opencv/tree/master/samples/data
    #
    set filename1 "left.jpg"
    set filename2 "right.jpg"

    try {
        set img1 [::cv::imread $filename1 $::cv::IMREAD_GRAYSCALE]
        set img2 [::cv::imread $filename2 $::cv::IMREAD_GRAYSCALE]

        set akaze [::cv::AKAZE]

        set result1 [$akaze detectAndCompute $img1]
        set result2 [$akaze detectAndCompute $img2]
        set kp1 [lindex $result1 0]
        set d1 [lindex $result1 1]
        set kp2 [lindex $result2 0]
        set d2 [lindex $result2 1]

        set bmatcher [::cv::BFMatcher $::cv::NORM_HAMMING 0]
        set matches [$bmatcher knnMatch $d1 $d2 2]

        $d1 close
        $d2 close
        $akaze close
        $bmatcher close

        set dmatches [list]
        foreach match $matches {
            foreach {m n} $match {
                set mdistance [lindex $m 3]
                set ndistance [lindex $n 3]
                if {$mdistance < [expr 0.8 * $ndistance]} {
                    lappend dmatches $m
                }
            }
        }

        set srcPts [list]
        set dstPts [list]
        foreach match $dmatches {
            set spoint [lindex $kp1 [lindex $match 0]]
            set dpoint [lindex $kp2 [lindex $match 1]]

            lappend srcPts [expr int([lindex $spoint 0])] [expr int([lindex $spoint 1])]
            lappend dstPts [expr int([lindex $dpoint 0])] [expr int([lindex $dpoint 1])]
        }

        # Find homography matrix and do perspective transform
        set src_pts [::cv::Mat::Mat 1 [expr [llength $srcPts]/2] $::cv::CV_32SC2]
        $src_pts setData $srcPts
        set dst_pts [::cv::Mat::Mat 1 [expr [llength $dstPts]/2] $::cv::CV_32SC2]
        $dst_pts setData $dstPts

        set result [::cv::findFundamentalMat $src_pts $dst_pts $::cv::FM_LMEDS 3.0 0.99]
        set F [lindex $result 0]
        set mask [lindex $result 1]

        set nsrc_pts [cv::Mat::Mat 0 0 $::cv::CV_32SC2]
        set ndst_pts [cv::Mat::Mat 0 0 $::cv::CV_32SC2]
        for {set i 0} {$i < [$mask rows]} {incr i} {
            set value [$mask at [list $i 0] 0]

            # We select only inlier points
            if {$value == 1} {
                set srccol [$src_pts colRange $i [expr $i + 1]]
                $nsrc_pts push_back $srccol

                set dstcol [$dst_pts colRange $i [expr $i + 1]]
                $ndst_pts push_back $dstcol

                $srccol close
                $dstcol close
            }
        }

        $src_pts close
        $dst_pts close

        set colorimg1 [::cv::cvtColor $img1 $::cv::COLOR_GRAY2BGR]
        set colorimg2 [::cv::cvtColor $img2 $::cv::COLOR_GRAY2BGR]

        set lines1 [::cv::computeCorrespondEpilines $ndst_pts 2 $F]
        drawlines $colorimg1 $lines1 $nsrc_pts $ndst_pts
        ::cv::imshow "imgag1" $colorimg1

        set lines2 [::cv::computeCorrespondEpilines $nsrc_pts 1 $F]
        drawlines $colorimg2 $lines2 $ndst_pts $nsrc_pts
        ::cv::imshow "imgag2" $colorimg2

        ::cv::waitKey 0

        $nsrc_pts close
        $ndst_pts close
        $F close
        $mask close
        $lines1 close
        $lines2 close
        $colorimg1 close
        $colorimg2 close

        $img1 close
        $img2 close
    } on error {em} {
        puts $em
    }

Non-Photorealistic Rendering -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]
        set img2 [::cv::detailEnhance $img]
        set img3 [::cv::edgePreservingFilter $img2]

        set img4 [::cv::stylization $img3]
        ::cv::imwrite "stylization.png" $img4

        set penresult [::cv::pencilSketch $img3]
        set img5 [lindex $penresult 0]
        set img6 [lindex $penresult 1]
        ::cv::imwrite "pencilSketch_1.png" $img5
        ::cv::imwrite "pencilSketch_2.png" $img6

        $img close
        $img2 close
        $img3 close
        $img4 close
        $img5 close
        $img6 close
    } on error {em} {
        puts $em
    }

High Dynamic Range Imaging test -

    package require opencv

    #
    # From https://en.wikipedia.org/wiki/High-dynamic-range_imaging
    # Download the four exposured images and test.
    #
    set times [list 0.0333 0.25 2.5 15.0]
    set files [list 1.JPG 2.JPG 3.JPG 4.JPG]
    set images [list]

    try {

        foreach f $files {
            set img [::cv::imread $f]
            lappend images $img
        }

        set a [::cv::AlignMTB]
        set newimages [$a process $images]

        for {set i 0} {$i < [llength $images]} {incr i} {
            [lindex $images $i] close
        }

        set c [::cv::CalibrateDebevec]
        set responseDebevec [$c process $newimages $times]

        set mergeDebevec [::cv::MergeDebevec]
        set hdrDebevec [$mergeDebevec process $newimages $times $responseDebevec]
        ::cv::imwrite "hdrDebevec.exr" $hdrDebevec

        for {set i 0} {$i < [llength $newimages]} {incr i} {
            [lindex $newimages $i] close
        }

        $responseDebevec close

        set tonemapDrago [::cv::TonemapDrago 1.0 0.7 0.85]
        set result [$tonemapDrago process $hdrDebevec]
        set result2 [$result multiply 3]
        set result3 [$result2 multiply 255]
        ::cv::imwrite "ldr-Drago.jpg" $result3

        $tonemapDrago close
        $result close
        $result2 close
        $result3 close

        set tonemapMantiuk [::cv::TonemapMantiuk 2.2 0.85 1.2]
        set result [$tonemapMantiuk process $hdrDebevec]
        set result2 [$result multiply 3]
        set result3 [$result2 multiply 255]
        ::cv::imwrite "ldr-Mantiuk.jpg" $result3

        $tonemapMantiuk close
        $result close
        $result2 close
        $result3 close

        set tonemapReinhard [::cv::TonemapReinhard 1.5 0 0 0]
        set result [$tonemapReinhard process $hdrDebevec]
        set result2 [$result multiply 255]
        ::cv::imwrite "ldr-Reinhard.jpg" $result2

        $tonemapReinhard close
        $result close
        $result2 close

        $hdrDebevec close

    } on error {em} {
        puts $em
    }

Stitcher test -

    package require opencv

    if {$argc < 1} {
        puts "Please give an image list."
        exit
    }

    try {
        set imagelist [list]
        for {set i 0} {$i < $argc} {incr i} {
            set img [::cv::imread [lindex $argv $i] $::cv::IMREAD_COLOR]
            lappend imagelist $img
        }

        set s [::cv::Stitcher $::cv::PANORAMA]
        set result [$s stitch $imagelist]
        $s close

        ::cv::imwrite output.png $result
        $result close
    } on error {em} {
        puts $em
    }

PCA example -

    package require opencv

    proc drawAxis {matrix x1 y1 x2 y2 color scale} {
        set PI 3.1415926535897931
        set angle [expr atan2($y1 - $y2, $x1 - $x2)]
        set hypotenuse [expr sqrt(($y1 - $y2)*($y1 - $y2) + ($x1 - $x2)*($x1 - $x2))]

        set qx [expr int($x1 - $scale * $hypotenuse * cos($angle))]
        set qy [expr int($y1 - $scale * $hypotenuse * sin($angle))]

        cv::line $matrix $x1 $y1 $qx $qy $color 1 $::cv::LINE_AA 0

        set px [expr int($qx + 9 * cos($angle + $PI / 4))]
        set py [expr int($qy + 9 * sin($angle + $PI / 4))]
        cv::line $matrix $px $py $qx $qy $color 1 $::cv::LINE_AA 0

        set px [expr int($qx + 9 * cos($angle - $PI / 4))]
        set py [expr int($qy + 9 * sin($angle - $PI / 4))]
        cv::line $matrix $px $py $qx $qy $color 1 $::cv::LINE_AA 0
    }

    proc getOrientation {matrix contour} {
        set sz [expr [llength $contour] / 2]
        set data_pts [::cv::Mat::Mat $sz 2 $::cv::CV_64F]
        for {set i 0; set j 0} {$i < $sz} {incr i 1; incr j 2} {
            $data_pts at [list $i 0] 0 [lindex $contour $j]
            $data_pts at [list $i 1] 0 [lindex $contour [expr $j + 1]]
        }

        set pca [::cv::PCA $data_pts $::cv::DATA_AS_ROW]
        set mean [$pca mean]
        set cntr_x [expr int([$mean at [list 0 0] 0])]
        set cntr_y [expr int([$mean at [list 0 1] 0])]

        set eigenvalues [$pca eigenvalues]
        set eigenvectors [$pca eigenvectors]

        set eigen_vecs [list]
        set eigen_val [list]
        for {set i 0} {$i < 2} {incr i} {
            set x [$eigenvectors at [list $i 0] 0]
            set y [$eigenvectors at [list $i 1] 0]
            lappend eigen_vecs $x $y
            lappend eigen_val [$eigenvalues at [list $i 0] 0]
        }

        set p1_x [expr int($cntr_x + 0.02 * [lindex $eigen_vecs 0] * [lindex $eigen_val 0])]
        set p1_y [expr int($cntr_y + 0.02 * [lindex $eigen_vecs 1] * [lindex $eigen_val 0])]
        set p2_x [expr int($cntr_x - 0.02 * [lindex $eigen_vecs 2] * [lindex $eigen_val 1])]
        set p2_y [expr int($cntr_y - 0.02 * [lindex $eigen_vecs 3] * [lindex $eigen_val 1])]

        cv::circle $matrix $cntr_x $cntr_y 3 [list 255 0 255 0] 2
        drawAxis $matrix $cntr_x $cntr_y $p1_x $p1_y [list 0 255 0 0] 1
        drawAxis $matrix $cntr_x $cntr_y $p2_x $p2_y [list 255 255 0 0] 5

        $mean close
        $eigenvectors close
        $eigenvalues close
        $pca close
    }

    #
    # Download from https://github.com/opencv/opencv/blob/master/samples/data/pca_test1.jpg
    #
    set filename "pca_test1.jpg"

    try {
        set src [::cv::imread $filename $::cv::IMREAD_COLOR]

        ::cv::namedWindow "Src" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Src" $src

        set gray [::cv::cvtColor $src $::cv::COLOR_BGR2GRAY]
        set bw [cv::threshold $gray 50 255 [expr $::cv::THRESH_BINARY | $::cv::THRESH_OTSU]]
        set contours [cv::findContours $bw $::cv::RETR_LIST $::cv::CHAIN_APPROX_NONE]

        for {set i 0} {$i < [llength $contours]} {incr i} {
            set contour [lindex $contours $i]
            set area [cv::contourArea $contour]
            if {$area < 100 || $area > 100000} {
                continue
            }

            cv::drawContours $src $contours $i [list 0 0 255 0] 2 $::cv::LINE_8 2 0 0
            getOrientation $src $contour
        }

        $gray close
        $bw close

        ::cv::namedWindow "Output" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Output" $src
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $src close
    } on error {em} {
        puts $em
    }

QRCodeDetector example -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img [::cv::imread $filename $::cv::IMREAD_COLOR]
        set qrdetect [::cv::QRCodeDetector]
        set result [$qrdetect detectAndDecode $img]

        puts "Decode result: [lindex $result 0]"

        set box [lindex $result 1]
        set code [lindex $result 2]
        set m [$box rows]
        set n [$box cols]

        if {[$box empty] != 1} {
            for {set i 0} {$i < $m} {incr i} {
                for {set j 0} {$j < $n} {incr j} {
                    set x1 [expr int([$box at [list $i $j] 0])]
                    set y1 [expr int([$box at [list $i $j] 1])]
                    set x2 [expr int([$box at [list $i [expr ($j+1)%$n]] 0])]
                    set y2 [expr int([$box at [list $i [expr ($j+1)%$n]] 1])]

                    cv::line $img $x1 $y1 $x2 $y2 [list 255 128 0 0] 5
                }
            }
        }

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $img

        if {[$code empty] != 1} {
            ::cv::namedWindow "Display QR code image" $::cv::WINDOW_AUTOSIZE
            ::cv::imshow "Display QR code image" $code
        }

        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $box close
        $code close
        $qrdetect close
        $img close
    } on error {em} {
        puts $em
    }

People detection by using HOGDescriptor -

    package require opencv

    # The example file can be downloaded from:
    # https://github.com/opencv/opencv/blob/master/samples/data/vtest.avi

    set filename "vtest.avi"
    set v [::cv::VideoCapture file $filename]
    if {[$v isOpened]==0} {
        puts "Open Video file $filename failed."
        exit
    }

    set hog [cv::HOGDescriptor 64 128 16 16 8 8 8 8 9]
    $hog setSVMDetector [$hog getDefaultPeopleDetector]
    set hog_d [cv::HOGDescriptor 48 96 16 16 8 8 8 8 9 1 -1 0.2 0 64 0]
    $hog_d setSVMDetector [$hog getDaimlerPeopleDetector]
    set mode 0

    while {[$v isOpened]==1} {
        try {
            set frame [$v read]

            set t [::cv::getTickCount]
            if {$mode == 0} {
                set rects [$hog detectMultiScale $frame 0 8 8 0 0 1.05 2 0]
            } else {
                set rects [$hog_d detectMultiScale $frame 0 8 8 0 0 1.05 2 1]
            }
            set t [expr [cv::getTickCount] - $t]

            if {$mode == 0} {
                set buf "Mode: Default ||| FPS: [format "%0.1f" [expr [::cv::getTickFrequency] / $t]]"
            } else {
                set buf "Mode: Daimler ||| FPS: [format "%0.1f" [expr [::cv::getTickFrequency] / $t]]"
            }
            ::cv::putText $frame $buf 10 30 $::cv::FONT_HERSHEY_PLAIN 2.0 [list 0 0 255 0] 2
            set length [llength $rects]
            for {set i 0} {$i < $length} {incr i} {
                set rlist [lindex $rects $i]

                # Slightly shrink the rectangles to get a nicer output.
                set width [lindex $rlist 2]
                set height [lindex $rlist 3]
                set x1 [expr int([lindex $rlist 0] + $width * 0.1)]
                set y1 [expr int([lindex $rlist 1] + $height * 0.07)]
                set x2 [expr int($x1 + $width * 0.8)]
                set y2 [expr int($y1 + $height * 0.8)]
                set color [list 0 255 0 0]
                ::cv::rectangle $frame $x1 $y1 $x2 $y2 $color 2
            }

            ::cv::imshow "People detector" $frame

            $frame close
            set key [::cv::waitKey 1]
            if {$key==[scan "q" %c] || $key == 27} {
                break
            } elseif {$key == [scan " " %c]} {
                if {$mode == 0} {
                    set mode 1
                } else {
                    set mode 0
                }
            }
        } on error {em} {
            break
        }
    }

    $hog close
    $hog_d close
    $v close
    ::cv::destroyAllWindows

The following program shows how to detect faces in an image (using CascadeClassifier).

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img1 [::cv::imread $filename $::cv::IMREAD_COLOR]
        set img2 [::cv::cvtColor $img1 $::cv::COLOR_BGR2GRAY]
        set img3 [::cv::equalizeHist $img2]

        $img2 close

        #
        # From https://github.com/opencv/opencv/tree/master/data/lbpcascades
        # For test ::cv::CascadeClassifier command
        #
        set xmlfile "lbpcascades/lbpcascade_frontalface.xml"
        set classifier [::cv::CascadeClassifier $xmlfile]
        set rects [$classifier detectMultiScale $img3]
        set length [llength $rects]
        for {set i 0} {$i < $length} {incr i} {
            set rlist [lindex $rects $i]
			set x1 [lindex $rlist 0]
			set y1 [lindex $rlist 1]
			set x2 [expr $x1 + [lindex $rlist 2]]
			set y2 [expr $y1 + [lindex $rlist 3]]
			set color [list 255 0 0 0]
			::cv::rectangle $img1 $x1 $y1 $x2 $y2 $color 2 $::cv::LINE_AA 0
        }

        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $img1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $classifier close
        $img1 close
        $img3 close
    } on error {em} {
        puts $em
    }

Below is an DNN example (face detector) -

    package require opencv

    if {$argc != 1} {
        exit
    }

    set filename [lindex $argv 0]

    try {
        set img1 [::cv::imread $filename $::cv::IMREAD_COLOR]
        set blob [::cv::dnn::blobFromImage $img1 1.0 300 300 \
                [list 104 117 123 0] 1 1]

        # From https://github.com/opencv/opencv/tree/master/samples/dnn
        # model is from models.yml and config is from face_detector folder's file
        # Just download to test
        set model "res10_300x300_ssd_iter_140000.caffemodel"
        set config "deploy.prototxt"
        set net [::cv::dnn::readNet $model $config Caffe]
        $net setPreferableBackend $::cv::dnn::DNN_BACKEND_DEFAULT
        $net setPreferableTarget $::cv::dnn::DNN_TARGET_CPU
        $net setInput $blob
        set data [$net forward]

        set msize [$data size]
        for {set i 0} {$i < [lindex $msize 2]} {incr i} {
            set confidence [$data at [list 0 0 $i 2] 0]
            if {$confidence > 0.9} {
                set startX [expr int([$data at [list 0 0 $i 3] 0] * [$img1 cols])]
                set startY [expr int([$data at [list 0 0 $i 4] 0] * [$img1 rows])]
                set endX [expr int([$data at [list 0 0 $i 5] 0] * [$img1 cols])]
                set endY [expr int([$data at [list 0 0 $i 6] 0] * [$img1 rows])]

                cv::rectangle $img1 $startX $startY $endX $endY [list 255 0 0 0] 1
            }
        }
        ::cv::namedWindow "Display Image" $::cv::WINDOW_AUTOSIZE
        ::cv::imshow "Display Image" $img1
        ::cv::waitKey 0
        ::cv::destroyAllWindows

        $net close
        $blob close
        $img1 close
    } on error {em} {
        puts $em
    }

