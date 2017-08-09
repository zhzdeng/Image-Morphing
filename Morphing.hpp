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
  Delaunay* delaunay;

  // calculate Transfor Matrix
  CImg<float> calculateMatrix(const Point& srcPoint1,
                                const Point& srcPoint2,
                                const Point& srcPoint3,
                                const Point& detPoint1,
                                const Point& detPoint2,
                                const Point& detPoint3
                                );

  // calculate Transfor matrixs
  void calculateMatrixs(const vector<Triangle> &src,
                        const vector<Triangle> &det,
                        vector<CImg<float> >& matrixs);

  // find the triangle when given the point(x, y)
  int whichtriangle(const vector<Triangle>& triangles, int x, int y);

  // linear transformation;
  void lineTranf(CImg<float> matrix, const int srcX, const int srcY,
                                     int &detX, int &detY);

  void calculateTrisArea(vector<Triangle> & triangles);

  double calculateTriArea(const Point& a, const Point& b, const Point& c);

  void deleteDelaunay() {
    if (delaunay != NULL) {
      tri_delaunay2d_release(delaunay);
    }
  }


  void morphing(const CImg<float> &srcImg,
                const CImg<float> &detImg,
                const vector<Point>& src, const vector<Point>& det,
                const Delaunay* tri, int num, string directory);

  Delaunay* triangulation(const vector<Point>&);


  void readPointsFromScreen(const CImg<float>& src, const CImg<float>& det,
                            vector<Point>& srcPoint, vector<Point>& detPoint);

 public:
  Morphing(): delaunay(NULL) {}
  ~Morphing() { deleteDelaunay(); }

  void run(const CImg<float>& src, const CImg<float>& det, int num,
           string directory);
};

#endif
