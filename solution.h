
#ifndef SOLUTION_H
#define SOLUTION_H

#include <QtCore/qmath.h>
#include <QtDataVisualization/QSurface3DSeries>
#include "qt_helpers.h"

using namespace QtDataVisualization;
using namespace qt_helpers;

class Solution : public QObject {
  Q_OBJECT
 public:
  const static float sampleMinTheta;
  const static float sampleMaxTheta;
  const static float sampleMinY;
  const static float sampleMaxY;
  const static float sampleMinR;

  void generateData(float bessel_order_n, int root_order_m);
  float frequency(float bessel_order_n, int root_order_m);
  float frequency_ratio(float bessel_order_n, int root_order_m);
  float radius() const;
  explicit Solution(int sampleCount, int timeSlicesCount, float radius,
                    float wave_speed, QObject* parent = 0);
  virtual ~Solution();

 QVector<QSurfaceDataArray*> getTimeSlices();
 public Q_SLOTS:
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
  QVector<QSurfaceDataArray*> m_timeSlices;
  QSurfaceDataArray* m_resetArray;
};

#endif
