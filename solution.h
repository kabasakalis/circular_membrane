/*
  Normal modes of a vibrating circular membrane (drumhead).
  Solution Class.
  Works out mathematical computations and provides data to graph.
  Copyright  2017 Spiros Kabasakalis <kabasakalis@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
  OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **/

#ifndef SOLUTION_H
#define SOLUTION_H

#include <QtCore/qmath.h>
#include <QtDataVisualization/QSurface3DSeries>
#include "qt_helpers.h"

using namespace QtDataVisualization;
using namespace qt_helpers;

class Solution  {
 public:
  const static float sampleMinTheta;
  const static float sampleMaxTheta;
  const static float sampleMinY;
  const static float sampleMaxY;
  const static float sampleMinR;

  explicit Solution(int sampleCount, int timeSlicesCount, float radius,
      float wave_speed);
  virtual ~Solution();
  void generateData(float bessel_order_n, int root_order_m);
  float frequency(float bessel_order_n, int root_order_m);
  float frequency_ratio(float bessel_order_n, int root_order_m);
  float radius() const;
  QVector<QSurfaceDataArray>& getTimeSlices();
 private:
  void clearData();
  float get_bessel_root(float bessel_order_n, int root_order_m);
  float radial_solution(float r, float bessel_root, int bessel_order_n);
  float angular_solution(float theta, float bessel_order_n);
  float temporal_solution(float t, float bessel_root);
  float m_radius;
  float m_wave_speed;
  int m_sampleCount;
  int m_timeSlicesCount;
  float m_sampleMaxR;
  float m_stepR;
  float m_stepTheta;
  QVector<QSurfaceDataArray> m_timeSlices;
  QSurfaceDataArray* m_resetArray;
};

#endif
