#ifndef LCCV_HPP
#define LCCV_HPP

#include <atomic>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <pthread.h>

#include "libcamera_app.hpp"
#include "libcamera_app_options.hpp"

namespace lccv {

class PiCamera {
public:
  PiCamera(int camera = 0);
  ~PiCamera();

  Options *options;

  // Photo mode
  bool startPhoto();
  bool capturePhoto(cv::Mat &frame);
  bool stopPhoto();

  // Video mode
  bool startVideo();
  bool getVideoFrame(cv::Mat &frame, unsigned int timeout);
  void stopVideo();

  // Applies new zoom options. Before invoking this func modify options->roi.
  void ApplyZoomOptions();

protected:
  void run();

protected:
  LibcameraApp *app;
  void getImage(cv::Mat &frame, CompletedRequestPtr &payload);
  static void *videoThreadFunc(void *p);
  pthread_t videothread;
  unsigned int still_flags;
  unsigned int vw, vh, vstr;
  std::atomic<bool> running, frameready;
  uint8_t *framebuffer;
  std::mutex mtx;
  bool camerastarted;
};

} // namespace lccv
#endif
