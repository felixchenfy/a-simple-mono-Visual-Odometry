// Test my wrapped up pcl functions for displaying:
//  * A moving camera.
//  * Trajecotry of the camera.
//  * Keep on adding a keypoint into the cloud.
// If this works, the lib funcs can be used for my SLAM project.

#include <iostream>

#include "my_display/pcl_display.h"
#include "my_basics/eigen_funcs.h"

using namespace std;
using namespace cv;
using namespace Eigen;

using namespace my_display;


int main()
{
    // Init my PclViewer class
    string viewer_name="my pcl viewer";
    my_display::PclViewer::Ptr pcl_displayer(
        new my_display::PclViewer(
            viewer_name
        )
    );
    
    // Set up camera pos.
    Eigen::Affine3d T_affine;
    cv::Mat R_vec = (cv::Mat_<double>(3, 1) << 0, 0, 0);
    cv::Mat t = (cv::Mat_<double>(3, 1) << 0, 0, 0);

    // Loop
    int cnt = 0;
    while (true)
    {
        cnt++;

        // Simulate camera motion.
        //      Return: R_vec, t
        const double period = 10.0;
        t.at<double>(0, 0) = sin(cnt / period);
        t.at<double>(1, 0) = cos(cnt / period);
        t.at<double>(2, 0) -= 0.01;
        enum
        {
            X_AXIS,
            Y_AXIS,
            Z_AXIS
        };
        R_vec.at<double>(X_AXIS, 0) += 0.01;

        // Simulate feature point captured by the camera.
        //      Return: kpt_3d_pos_in_world, r, g, b
        double x = 0, y = 0, z = 2;
        unsigned char r=255, g=0, b=0;
        cv::Mat kpt_3d_pos_in_cam = (Mat_<double>(3,1) << x, y, z);
        Mat R;
        cv::Rodrigues(R_vec, R);
        cv::Mat kpt_3d_pos_in_world = R * kpt_3d_pos_in_cam + t;

        // Display
        pcl_displayer->updateCameraPose(R_vec, t);
        pcl_displayer->addPoint(kpt_3d_pos_in_world, r, g, b);
        pcl_displayer->update();
        pcl_displayer->spinOnce(100);
        if (pcl_displayer->wasStopped())
            break;
    }
    return (1);
}