#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <stdexcept>

const int cameraID = 0;
const int cameraHeight = 3684;
const int cameraWidth = 4912;
const int cameraFPS = 4;

int main(int argc, char **argv) {
  cv::VideoCapture cap;
  cap.open(cameraID);
  if (!cap.isOpened())
    throw std::runtime_error("Could not open selected device");

  cap.set(cv::CAP_PROP_FRAME_WIDTH, cameraWidth);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, cameraHeight);
  cap.set(cv::CAP_PROP_FPS, cameraFPS);
  cap.set(cv::CAP_PROP_CONVERT_RGB, 0);

  cv::Mat frame;

  std::cout << "Starting capture..." << std::endl;

  bool capturing = true;
  int frameWrote = 0;
  while (capturing) {
    cap >> frame;

    cv::Mat raw(cameraHeight, cameraWidth, CV_16UC1, frame.data);
    if (raw.empty()) {
      std::cout << "Captured an empty image." << std::endl;
      continue;
    }

    cv::Mat bayer8BitMat = raw.clone();
    // Convert the Bayer data from 16-bit to to 8-bit
    // The 3rd parameter here scales the data by 1/16 so that it fits in 8 bits.
    // Without it, convertTo() just seems to chop off the high order bits.
    bayer8BitMat.convertTo(bayer8BitMat, CV_8UC1, 0.0625);

    // Convert the Bayer data to 8-bit RGB
    cv::Mat rgb8BitMat(cameraHeight, cameraWidth, CV_8UC3);
    cv::cvtColor(bayer8BitMat, rgb8BitMat, CV_BayerGB2RGB);

    cv::imshow("Frame", rgb8BitMat);
    char cmd = cv::waitKey(10);

    if (cmd == 'c') {
      std::ostringstream s_path;
      s_path << "leopard_sample_" << frameWrote++ << ".bmp";
      cv::imwrite(s_path.str(), rgb8BitMat);
    } else if (cmd == 'q')
      capturing = false;
  }

  return 0;
}
