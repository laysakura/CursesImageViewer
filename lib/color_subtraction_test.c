#include <opencv/cv.h>
#include <opencv/highgui.h>

int
main()
{
  IplImage *img = cvLoadImage("../img/gakki-.jpg", CV_LOAD_IMAGE_COLOR);

    /* height = img.height */
    /* width = img.width */

    /* term_colors = get_term_colors() */

    /* # Calc distance of a pixel from red one-by-one */
    /* max_distance = CvScalar_distance(CV_RGB(0,0,0), CV_RGB(255,255,255)) */
    /* for i in range(height): */
    /*     for j in range(width): */

    /*         min_distance = max_distance */
    /*         min_distance_idx = -1 */
    /*         for distance_idx, term_color_scalar in enumerate(term_colors): */
    /*             pix_scalar = cvGet2D(img,i,j) */
    /*             distance = CvScalar_distance(pix_scalar, term_color_scalar) */
    /*             if distance < min_distance: */
    /*                 min_distance = distance */
    /*                 min_distance_idx = distance_idx */
    /*         #     print("term_color_scalar[%d].distance = %f" % (distance_idx, distance)) */

    /*         # print("") */
    /*         # print("term_color_scalar.min_distance = %f (at %d)" % (min_distance, min_distance_idx)) */
    /*         # print("") */
    /*         newpix = term_colors[min_distance_idx] */
    /*         cvSet2D(img,i,j, newpix) */


    //画像の表示
  cvNamedWindow ("test OpenCV", CV_WINDOW_AUTOSIZE);
  cvShowImage ("test OpenCV", img);
  cvWaitKey (0);
  cvDestroyWindow ("test OpenCV");

  //画像の解放
  cvReleaseImage(&img);


  return 0;
}
