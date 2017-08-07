#include "Morphing.hpp"
#include <iostream>
using namespace std;
using namespace cimg_library;

const unsigned char color[] = {0, 0, 255};

/**
 * calculate Transfor Matrix
 */
CImg<float> Morphing::calculateMatrix(const Point& srcPoint1,
                                      const Point& srcPoint2,
                                      const Point& srcPoint3,
                                      const Point& detPoint1,
                                      const Point& detPoint2,
                                      const Point& detPoint3
                                        ) {
  CImg<float> srcMatrix(3, 3, 1, 1);
  CImg<float> detMatrix(3, 3, 1, 1);

  srcMatrix(0, 0) = srcPoint1.x;
  srcMatrix(1, 0) = srcPoint2.x;
  srcMatrix(2, 0) = srcPoint3.x;

  srcMatrix(0, 1) = srcPoint1.y;
  srcMatrix(1, 1) = srcPoint2.y;
  srcMatrix(2, 1) = srcPoint3.y;

  srcMatrix(0, 2) = 1;
  srcMatrix(1, 2) = 1;
  srcMatrix(2, 2) = 1;

  detMatrix(0, 0) = detPoint1.x;
  detMatrix(1, 0) = detPoint2.x;
  detMatrix(2, 0) = detPoint3.x;

  detMatrix(0, 1) = detPoint1.y;
  detMatrix(1, 1) = detPoint2.y;
  detMatrix(2, 1) = detPoint3.y;

  detMatrix(0, 2) = 1;
  detMatrix(1, 2) = 1;
  detMatrix(2, 2) = 1;

  return detMatrix *= srcMatrix.invert();

}

/**
 * calculate Transfor matrixs
 * @param srcTri  [description]
 * @param detTri  [description]
 * @param matrixs output
 */
void Morphing::calculateMatrixs(const vector<Triangle> &srcTri,
                      const vector<Triangle> &detTri,
                      vector<CImg<float> >& matrixs) {
  int size = srcTri.size();
  for (int i = 0; i < size; i++) {
    matrixs.push_back(calculateMatrix(srcTri[i].vertex[0],
                                      srcTri[i].vertex[1],
                                      srcTri[i].vertex[2],
                                      detTri[i].vertex[0],
                                      detTri[i].vertex[1],
                                      detTri[i].vertex[2]
                                      ));
  }
}

/**
 * find the triangle when given the point(x, y)
 * @param  triangles [description]
 * @param  x         [description]
 * @param  y         [description]
 * @return           [description]
 */
int Morphing::whichtriangle(const vector<Triangle>& triangles, int x, int y) {
  int size = triangles.size();
  Point curPoint(x, y);
  for (int i = 0; i < size; i++) {
    double area = 0;
    for (int j = 0; j < 3; j++) {
      int nextIndex = (j + 1) % 3;
      area += calculateTriArea(curPoint, triangles[i].vertex[j],
                                         triangles[i].vertex[nextIndex]);
    }
    if (fabs(area - triangles[i].area) < 0.1) return i;
  }
  char intToStr[20];
  sprintf(intToStr, "(%d, %d)" , x, y);
  throw string("Morphing::whichtriangle 异常, 该点不在任何三角形中 ") + intToStr;
}

/**
 * linear transformation;
 * @param matrix [description]
 * @param srcX   [description]
 * @param srcY   [description]
 * @param detX   [description]
 * @param detY   [description]
 */
void Morphing::lineTranf(CImg<float> matrix, const int srcX, const int srcY,
                                   int &detX, int &detY) {
  detX = round(matrix(0, 0) * srcX + matrix(1, 0) * srcY + matrix(2, 0));
  detY = round(matrix(0, 1) * srcX + matrix(1, 1) * srcY + matrix(2, 1));
}


/**
 * calculate triangles area one by one
 * @param triangles [description]
 */
void Morphing::calculateTrisArea(vector<Triangle> & triangles) {
  int size = triangles.size();
  for (int i = 0; i < size; i++) {
    triangles[i].area = calculateTriArea(triangles[i].vertex[0],
                                         triangles[i].vertex[1],
                                         triangles[i].vertex[2]);
  }
}

/**
 * calculate triangle area
 * S=|(1/2)*(x1y2*1+x2y3*1+x3y1*1-x1y3*1-x2y1*1-x3y2*1)|
 */
double Morphing::calculateTriArea(const Point& a, const Point& b,
                                  const Point& c) {
  return fabs(0.5 * (a.x * b.y + b.x * c.y + c.x * a.y
                - a.x * c.y- b.x * a.y - c.x * b.y));
}


/**
 * 计算对应三角形的映射矩阵
 * @param src 原图的标记点集
 * @param det 目标图的标记点集
 * @param tri 目标点集的三角剖分关系
 * @param num 生成中间图的数量
 */
void Morphing::morphing(const CImg<float> &srcImg,
                        const CImg<float> &detImg,
                        const vector<Point>& srcPoints,
                        const vector<Point>& detPoints,
                        const Delaunay* p_delaunay, int num) {
  /**
   * 1. 计算中间图像的三角形位置
   * 先将原图和目标图的三角形位置保存在数据结构 vector<Triangle>数据结构中
   * 中间图像的三角形位置用数据结构 vector<Triangle> triLoc;
   */
  vector<Triangle> srcTriangles, detTriangles;
  calculateTrisArea(srcTriangles);
  calculateTrisArea(detTriangles);
  const int triSize = p_delaunay->num_triangles;
  Point* points = p_delaunay->points;
  unsigned int* tris = p_delaunay->tris;
  for (int i = 0; i < triSize; i++) {
    srcTriangles.push_back(Triangle(srcPoints[tris[i*3 + 0]],
                                    srcPoints[tris[i*3 + 1]],
                                    srcPoints[tris[i*3 + 2]]));
    detTriangles.push_back(Triangle(detPoints[tris[i*3 + 0]],
                                    detPoints[tris[i*3 + 1]],
                                    detPoints[tris[i*3 + 2]]));
  }
  double alpha = 1.0 / num;
  const double STEP = alpha;
  for (int i = 0; i < num; i++, alpha += STEP) {
    vector<Triangle> midTriangels;
    for (int i = 0; i < triSize; i++) {
      // 三角形运算
      midTriangels.push_back(srcTriangles[i] * (1 - alpha)
                           + detTriangles[i] * alpha);
    }

    vector<CImg<float> > matrixsToSrc, matrixsToDet;
  /**
   * 2. 计算映射矩阵 输入 vector<Triangle> vector<Triangle>
   *               输出 vector<CImg<flost> > matrixs
   *               calculateMatrixs 的到映射矩阵
   */
    calculateMatrixs(midTriangels, srcTriangles, matrixsToSrc);
    calculateMatrixs(midTriangels, detTriangles, matrixsToDet);

  /**
   * 3. 进行三角映射,需要有原图的映射点A 和 结果图的映射点B
   *   3.1 判断点再哪个三角形中
   *     a) 计算三角形面积 calculateTrisArea
   *     b) 调用whichtriangle
   *   3.2 找到映射之后，用 alpha * A + (1 - alpha) * B
   */
    calculateTrisArea(midTriangels);
    CImg<float> midImg = detImg;
    cimg_forXY(midImg, x, y) {
      int index = whichtriangle(midTriangels, x, y);
      // 找到对应的原图和目标图的点
      int srcX, srcY, detX, detY;
      lineTranf(matrixsToSrc[index], x, y, srcX, srcY);
      lineTranf(matrixsToDet[index], x, y, detX, detY);

      midImg(x, y, 0) =  round((1 - alpha) * srcImg(srcX, srcY, 0)
                        + alpha * detImg(detX, detY, 0));
      midImg(x, y, 1) =  round((1 - alpha) * srcImg(srcX, srcY, 1)
                        + alpha * detImg(detX, detY, 1));
      midImg(x, y, 2) = round((1 - alpha) * srcImg(srcX, srcY, 2)
                        + alpha * detImg(detX, detY, 2));
    }

  /**
   * 4. 保存中间结果
   */
    char intToStr[20];
    sprintf(intToStr, "%d" , i);
    string fileName("result/");
    fileName = fileName + intToStr + ".jpg";
    midImg.save_jpeg(fileName.c_str());
  }
}



// 对给定的标记点集计算三角剖分
Delaunay* Morphing::triangulation(const vector<Point>& points) {
  int size = points.size();
  Point * p_points = new Point[size];
  if (!points.empty()) {
    memcpy(p_points, &points[0], size * sizeof(Point));
  }

  deleteDelaunay();
  delaunay = tri_delaunay2d_from(delaunay2d_from(p_points, size));

  return delaunay;
}

void Morphing::readPointsFromScreen(const CImg<float>& src,
                                    const CImg<float>& det,
                                    vector<Point>& srcPoints,
                                    vector<Point>& detPoints) {
  srcPoints.clear();
  detPoints.clear();

  srcPoints.push_back(Point(0          , 0));
  srcPoints.push_back(Point(0          , src.height()));
  srcPoints.push_back(Point(src.width(), 0));
  srcPoints.push_back(Point(src.width(), src.height()));

  detPoints.push_back(Point(0, 0));
  detPoints.push_back(Point(0, det.height()));
  detPoints.push_back(Point(det.width(), 0));
  detPoints.push_back(Point(det.width(), det.height()));


  CImg<float> srcImg = src;
  CImg<float> detImg = det;

  bool issrcImg = true;
  CImgDisplay disp;
  disp.display(srcImg);
  while (!disp.is_closed()) {
    // Left button clicked.
    if (disp.button()&1) {
      int x = disp.mouse_x();
      int y = disp.mouse_y();
      cout << "x: "   << x << ", y: " << y << endl;
      if (issrcImg) {
        disp.display(detImg);
        srcImg.draw_circle(x, y, 3, color);
        srcPoints.push_back(Point(x, y));
      }
      else {
        disp.display(srcImg);
        detImg.draw_circle(x, y, 3, color);
        detPoints.push_back(Point(x, y));
      }
      issrcImg = !issrcImg;
    }
    disp.wait();
  }
}


/**
 * 封装好的函数，该函数将调用上面的方法完整整个 Morphing 流程
 * @param src 原图
 * @param det 目标图
 * @param num 生成中间图的数量
 */
void Morphing::run(const CImg<float>& src, const CImg<float>& det, int num) {
  vector<Point> srcPoints, detPoints;
  readPointsFromScreen(src, det, srcPoints, detPoints);
  triangulation(detPoints);
  morphing(src, det, srcPoints, detPoints, delaunay, num);
}
