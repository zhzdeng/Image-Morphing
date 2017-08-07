#include <iostream>
#include <vector>
#include <fstream>
#include "Morphing.hpp"


using namespace std;
using namespace cimg_library;

int main(int argc, char const *argv[]) {
  cout << "------------- Morphing 程序运行 -------------" << endl;
  cout << "无参数运行: 读取目录下srcPoints.txt, detPoints.txt 文件" << endl;
  cout << "若需要指定图像, 需要输入参数 如: ./a.out src.jpg det.jpg" << endl;
  cout << "------------- Morphing 程序运行 -------------" << endl;
  cout << "程序运行中" << endl;

  Morphing morph;
  if (argc < 2) {
    // 无参数运行
    CImg<float> srcImg("1.jpg");
    CImg<float> detImg("2.jpg");
    try {
      morph.run(srcImg, detImg, "srcPoints.txt", "detPoints.txt");
    } catch (string s) {
      cout << s << endl;
    }
  } else {
    // 带参数运行
    CImg<float> srcImg, detImg;
    try {
      srcImg.load(argv[1]);
      detImg.load(argv[2]);
    } catch(...) {
      cout << "文件打开失败" << endl;
    }
    int width = detImg.width();
    int height = detImg.height();
    srcImg.resize(width, height);
    try {
      morph.run(srcImg, detImg);
    } catch(string s) {
      cout << s << endl;
    }
  }

  cout << "结果保存在 output/ 目录下" << endl;
  return 0;
}
