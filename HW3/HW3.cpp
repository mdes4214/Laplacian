#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/** @function main */
int main(int argc, char** argv)
{
	Mat src, src_tmp, dst;
	int kernel_size = 1;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	char filename_lap[100], filename_lap_scale[100], filename_lap_light[100], filename_output[100];
	string filename;

	int c;

	/// Load an image
	src = imread(argv[1]);
	src_tmp = src.clone();

	filename = argv[1];
	unsigned dot = filename.find_last_of(".");
	sprintf(filename_lap, "%s_lap.jpg", filename.substr(0, dot).c_str());
	sprintf(filename_lap_scale, "%s_lap_scale.jpg", filename.substr(0, dot).c_str());
	sprintf(filename_lap_light, "%s_lap_light.jpg", filename.substr(0, dot).c_str());
	sprintf(filename_output, "%s_output.jpg", filename.substr(0, dot).c_str());

	if (!src.data)
	{
		return -1;
	}

	/// Remove noise by blurring with a Gaussian filter
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// Convert the image to grayscale
	//cvtColor(src, src_gray, CV_RGB2GRAY);

	/// Create window
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	namedWindow("Laplace", CV_WINDOW_AUTOSIZE);
	namedWindow("Laplace_ScaleAbs", CV_WINDOW_AUTOSIZE);
	namedWindow("Laplace_ScaleAbs_light", CV_WINDOW_AUTOSIZE);
	namedWindow("dst", CV_WINDOW_AUTOSIZE);

	/// Apply Laplace function
	Mat abs_dst, abs_dst_light, fdst;

	Laplacian(src, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
	imwrite(filename_lap_scale, dst);
	convertScaleAbs(dst, abs_dst, 1, 0);
	convertScaleAbs(dst, abs_dst_light, 1, 150);
	src = src_tmp.clone();

	imwrite(filename_lap, abs_dst);
	imwrite(filename_lap_light, abs_dst_light);
	
	/// The Lapalcian for image enhancement
#if 0	/// method 1 => FAIL: The output is GRAY mode.
	src_gray.convertTo(src_gray, CV_16S, 1, 0);
	fdst = src_gray - dst;
	fdst.convertTo(fdst, CV_8U, 1, 0);
	cvtColor(fdst, fdst, CV_GRAY2RGB);
	imwrite(filename_output, fdst);
#endif

#if 0	/// method 2 => FAIL: The mask of Laplacian no negative value.
	src.convertTo(src, CV_16S, 1, 0);
	//dst.convertTo(dst, CV_8U, 1, 0);
	//cvtColor(dst, dst, CV_GRAY2RGB);
	fdst = src - dst;
	dst.convertTo(dst, CV_8U, 1, 0);
	src.convertTo(src, CV_8U, 1, 0);
	fdst.convertTo(fdst, CV_8U, 1, 0);
	imwrite("moon2_test.jpg", fdst);
#endif

#if 1	/// method 3 => SUCESS: Don't use GRAY mode when Laplacian.
	src.convertTo(src, CV_16S, 1, 0);
	fdst = src - dst;
	dst.convertTo(dst, CV_8U, 1, 0);
	src.convertTo(src, CV_8U, 1, 0);
	fdst.convertTo(fdst, CV_8U, 1, 0);
	imwrite(filename_output, fdst);
#endif

	/// Show images
	imshow("src", src);
	imshow("Laplace", dst);
	imshow("Laplace_ScaleAbs", abs_dst);
	imshow("Laplace_ScaleAbs_light", abs_dst_light);
	imshow("dst", fdst);

	waitKey(0);

	return 0;
}