#include <lccv.hpp>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
  std::cout << "Sample program for LCCV video capture" << std::endl;
  std::cout << "Press ESC to stop." << std::endl;
  cv::Mat image;
  lccv::PiCamera cam(argc == 2 ? atoi(argv[1]) : 0);
  cam.options->video_width = 1024;
  cam.options->video_height = 768;
  cam.options->framerate = 25;
  cam.options->verbose = false;
  cv::namedWindow("Video", cv::WINDOW_NORMAL);
  cam.startVideo();
  int ch = 0;
  while (ch != 27) {
    if (!cam.getVideoFrame(image, 1000)) {
      std::cout << "Timeout error" << std::endl;
    } else {
      cv::imshow("Video", image);
      ch = cv::waitKey(10);
    }
  }
  cam.stopVideo();
  cv::destroyWindow("Video");
}
