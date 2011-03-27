#include <cv.h>
#include <highgui.h>

CvCapture* g_capture = NULL;

IplImage* doCanny(
	IplImage* in,
	double    lowThresh,
	double    highThresh,
	int       aperture
	) 
{
	IplImage* inGS;
	bool bFreeGS = false;
	CvSize inSize = cvGetSize( in );
	if(in->nChannels != 1)
	{
		inGS = cvCreateImage(
			cvSize( inSize.width, inSize.height ),
			IPL_DEPTH_8U,   
			1
			);
		/* CV_RGB2GRAY: convert RGB image to grayscale */
		cvCvtColor( in, inGS, CV_RGB2GRAY );
		bFreeGS = true;
	}
	else
	{
		inGS = in;
	}

	IplImage* out = cvCreateImage(
		cvSize( inSize.width, inSize.height ),
		IPL_DEPTH_8U,   
		1
		);
	cvCanny( inGS, out, lowThresh, highThresh, aperture );

	if( bFreeGS )
	{
		cvReleaseImage( &inGS );
	}
	return( out );
};

int main( int argc, char* argv[] )
{
	/* Create 3 windows */
	cvNamedWindow( "Exercise_4_1_a_input", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Exercise_4_1_a_gray", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Exercise_4_1_a_canny", CV_WINDOW_AUTOSIZE );

	g_capture = cvCreateCameraCapture(0);

	IplImage* frame = cvQueryFrame( g_capture );
	IplImage* frameGray = cvCreateImage( 
		cvGetSize(frame), 
		IPL_DEPTH_8U, 
		1);
	IplImage* frameCanny;

	while(1) 
	{
		frame = cvQueryFrame( g_capture );
		if( !frame ) break;
		cvShowImage( "Exercise_4_1_a_input", frame );
		cvCvtColor( frame, frameGray, CV_RGB2GRAY );
		cvShowImage( "Exercise_4_1_a_gray", frameGray );
		frameCanny = doCanny( frame, 10, 100, 3 );
		cvShowImage( "Exercise_4_1_a_canny", frameCanny );
		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}

	/* Cleanup */
	cvReleaseCapture( &g_capture );	
	cvDestroyWindow( "Exercise_4_1_a_canny" );
	cvDestroyWindow( "Exercise_4_1_a_gray" );
	cvDestroyWindow( "Exercise_4_1_a_input" );
	return 0;
}