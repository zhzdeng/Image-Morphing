#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>

#include "Morphing.hpp"


using namespace std;
using namespace cimg_library;

void help() {
  cout << "usage: morphing [-o|-n]-s srcImage -d desImage" << endl;
  cout << "                -o output_directory" << endl;
  cout << "                -n output images number" << endl;
  cout << "example:" << endl;
  cout << "morphing -s 1.jpg -d 2.jpg -o output/ -n 24" << endl;
}

int main(int argc, char *const *argv) {
  char ch;
  string directory("./");
  string srcPath, desPath;
  int number = 12;
  while((ch = getopt(argc, argv, "o:n:s:d:")) != -1){
    switch(ch) {
      case 'o':
        directory = optarg;
        break;
      case 'n':
        number = atof(optarg);
        if (number <= 0) { cout << "error: -n num, num > 0" << endl; return -1; }
        break;
      case 's':
        srcPath = optarg;
        break;
      case 'd':
        desPath = optarg;
        break;
      default:
        break;
    }
  }

  Morphing morph;
  CImg<float> srcImg, detImg;
  try {
    srcImg.load(srcPath.c_str());
    detImg.load(desPath.c_str());
  } catch(...) {
    cout << "open image fault, please check it" << endl;
    return -2;
  }
  int width = detImg.width();
  int height = detImg.height();
  srcImg.resize(width, height);
  try {
    cout << "please mark the relative points in these images" << endl;
    morph.run(srcImg, detImg, number, directory);
  } catch(string s) {
    cout << s << endl;
  }

  cout << "the result save in " << directory << endl;
  return 0;
}
