#ifndef MORPHING_HPP
#define MORPHING_HPP

#include <fstream>
#include <vector>
#include <string>
#include "delaunay.hpp"
#include "CImg.h"

#define Delaunay tri_delaunay2d_t
#define Point del_point2d_t

using std::vector;
using std::string;
using cimg_library::CImg;

struct Triangle {
  Point vertex[3];
  double area;
  Triangle() {}
  Triangle(Point a, Point b, Point c, double n_area = 0) {
    vertex[0] = a;
    vertex[1] = b;
    vertex[2] = c;
    area = 0;
  }
  Triangle operator* (double alpha) {
    Triangle result = *this;
    for (int i = 0; i < 3; i++) {
      result.vertex[i].x *= alpha;
      result.vertex[i].y *= alpha;
    }
    result.area *= alpha;
    return result;
  }
  Triangle operator+ (const Triangle& other) {
    Triangle result = *this;
    for (int i = 0; i < 3; i++) {
      result.vertex[i].x += other.vertex[i].x;
      result.vertex[i].y += other.vertex[i].y;
    }
    result.area += other.area;
    return result;
  }

};

class Morphing {
 private:
  // 记录三角剖分集合的头
  Delaunay* delaunay;

  /**
   * 计算映射矩阵 Amatrix * Src = Det
   */
  CImg<float> calculateMatrix(const Point& srcPoint1,
                                const Point& srcPoint2,
                                const Point& srcPoint3,
                                const Point& detPoint1,
                                const Point& detPoint2,
                                const Point& detPoint3
                                );

  /**
   * 计算三角形集合的映射矩阵集合，映射矩阵 AX = Y
   * @param src     原图三角形集合 X
   * @param det     目标图三角形集合 Y
   * @param matrixs 映射矩阵 A
   */
  void calculateMatrixs(const vector<Triangle> &src,
                        const vector<Triangle> &det,
                        vector<CImg<float> >& matrixs);

  /**
   * 找出给定的点再哪个三角形里面，返回三角形在triangles索引下标
   * @param  triangles 三角形集合
   * @return           三角形在triangles的索引下标
   */
  int whichtriangle(const vector<Triangle>& triangles, int x, int y);

  /**
   * 线性变换
   * @param matrix 变换矩阵
   * @param srcX   输入的X
   * @param srcY   输入的Y
   * @param detX   输出的X
   * @param detY   输出的Y
   */
  void lineTranf(CImg<float> matrix, const int srcX, const int srcY,
                                     int &detX, int &detY);

  /**
   * 计算每个三角形的面积
   * @param triangles 既是输入也是输出
   */
  void calculateTrisArea(vector<Triangle> & triangles);

  /**
   * 计算单个三角形面积, 使用海伦公式
   * @param a 顶点1
   * @param b 顶点2
   * @param c 顶点3
   */
  double calculateTriArea(const Point& a, const Point& b, const Point& c);

  /**
   * 三角剖分的内存释放函数, 析构函数或每次重新计算三角剖分都会调用,
   */
  void deleteDelaunay() {
    if (delaunay != NULL) {
      tri_delaunay2d_release(delaunay);
    }
  }

  /**
   * 计算对应三角形的映射矩阵
   * @param src 原图的标记点集
   * @param det 目标图的标记点集
   * @param tri 目标点集的三角剖分关系
   * @param num 生成中间图的数量
   */
  void morphing(const CImg<float> &srcImg,
                const CImg<float> &detImg,
                const vector<Point>& src, const vector<Point>& det,
                const Delaunay* tri, int num = 11);
    // 对给定的标记点集计算三角剖分
  Delaunay* triangulation(const vector<Point>&);


  /**
   * 需要保证两个图像大小一致
   */
  void readPointsFromScreen(const CImg<float>& src, const CImg<float>& det,
                            vector<Point>& srcPoint, vector<Point>& detPoint);


 public:
  Morphing(): delaunay(NULL) {}
  ~Morphing() { deleteDelaunay(); }

  /**
   * 封装好的函数，该函数将调用上面的方法完成整个 Morphing 流程
   * @param src 原图
   * @param det 目标图
   * @param num 生成中间图的数量
   */
  void run(const CImg<float>& src, const CImg<float>& det, int num = 12);

  Delaunay* getDelaunay() { return delaunay; }
};

#endif
