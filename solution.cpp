
#include "solution.h"
#include <QtCore/qmath.h>
#include <boost/math/special_functions/bessel.hpp>

using namespace QtDataVisualization;

const float Solution::sampleMinTheta = 0.0f;
const float Solution::sampleMaxTheta = 2 * M_PI;
const float Solution::sampleMinR = 0.0f;
const float Solution::sampleMinY = -1.0f;
const float Solution::sampleMaxY = 1.0f;

Solution::Solution(int sampleCount, int timeSlicesCount, float radius,
                   float wave_speed, QObject* parent)
    : QObject(parent),
      m_radius(radius),
      m_wave_speed(wave_speed),
      m_sampleCount(sampleCount),
      m_timeSlicesCount(timeSlicesCount),
      m_sampleMaxR(radius),
      m_stepR{(radius - sampleMinR) / float(sampleCount - 1)},
      m_stepTheta{(sampleMaxTheta - sampleMinTheta) / float(sampleCount - 1)},
      m_resetArray(0)

{
  generateData(6.0, 3);
}

Solution::~Solution() { clearData(); }


float Solution::radius() const {
return m_radius;
}

float Solution::radial_solution(float r, float bessel_root,
                                int bessel_order_n) {
  return boost::math::cyl_bessel_j(bessel_order_n,
                                   (bessel_root / m_radius) * r);
}

float Solution::angular_solution(float theta, float bessel_order_n) {
  return qCos(bessel_order_n * theta);
}

float Solution::temporal_solution(float t, float bessel_root) {
  return qCos(m_wave_speed * (bessel_root / m_radius) * t);
}

float Solution::get_bessel_root(float bessel_order_n, int root_order_m) {
  return boost::math::cyl_bessel_j_zero(bessel_order_n, root_order_m);
}

void Solution::generateData(float bessel_order_n, int root_order_m) {
  if (!m_timeSlicesCount || !m_sampleCount) return;
  clearData();
  auto bessel_root = get_bessel_root(bessel_order_n, root_order_m);
  const float sampleMinT = 0.0f;
  const float sampleMaxT = (2 * M_PI * m_radius) / (m_wave_speed * bessel_root);
  float stepT = (sampleMaxT - sampleMinT) / float(m_timeSlicesCount - 1);
  m_timeSlices.reserve(m_timeSlicesCount);
  QSurfaceDataArray* base_surface_data_array = new QSurfaceDataArray;
  for (int j(0); j < m_sampleCount; j++) {
    QSurfaceDataRow* newRow = new QSurfaceDataRow(m_sampleCount);
    float r = qMin(m_sampleMaxR, (j * m_stepR + sampleMinR));
    auto radial = radial_solution(r, bessel_root, bessel_order_n);
    int index = 0;
    for (int k = 0; k < m_sampleCount; k++) {
      float theta = qMin(sampleMaxTheta, (k * m_stepTheta + sampleMinTheta));
      auto angular = angular_solution(theta, bessel_order_n);
      auto z = radial * angular;
      (*newRow)[index++].setPosition(QVector3D(theta, z, r));
    }
    *base_surface_data_array << newRow;
  }
  // Populate time slices
  for (int i(0); i < m_timeSlicesCount; i++) {
    QSurfaceDataArray* slice = new QSurfaceDataArray;
    slice->reserve(m_sampleCount);
    float t = qMin(sampleMaxT, (i * stepT + sampleMinT));
    auto temporal = temporal_solution(t, bessel_root);
    auto modifier = [&temporal](QSurfaceDataItem* item) -> void {
      item->setY(temporal * item->y());
    };
    slice = newSurfaceDataArrayFromSource(base_surface_data_array, modifier);
    m_timeSlices << slice;
  }  // slices
}


QVector<QSurfaceDataArray*> Solution::getTimeSlices() {
  return m_timeSlices;
}

void Solution::clearData() {
  for (int i(0); i < m_timeSlices.size(); i++) {
    QSurfaceDataArray* array = m_timeSlices[i];
    for (int j(0); j < array->size(); j++) delete (*array)[j];
    array->clear();
  }
}
