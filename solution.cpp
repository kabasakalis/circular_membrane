
#include "solution.h"
#include <QtCore/qmath.h>
#include <boost/math/special_functions/bessel.hpp>

using namespace QtDataVisualization;

constexpr const float sampleMinTheta = 0.0f;
constexpr const float sampleMaxTheta = 2 * M_PI;
constexpr const float sampleMinR = 0.0f;
// constexpr const float sampleMaxR = 20.0f;

Solution::Solution(int sampleCount, int timeSlicesCount, float radius,
                   float wave_speed, QObject *parent)
    : QObject(parent),
      m_resetArray(0),
      m_radius(radius),
      m_wave_speed(wave_speed),
      m_sampleCount(sampleCount),
      m_timeSlicesCount(timeSlicesCount)

{

    generateData(0.0, 1);

  //! [4]
  // qRegisterMetaType<QSurface3DSeries *>();
  //! [4]
}

Solution::~Solution() { clearData(); }

//! [0]
void Solution::generateData(float bessel_order_n, int root_order_m) {
  if (!m_timeSlicesCount || !m_sampleCount) return;
      clearData();
      auto bessel_root = boost::math::cyl_bessel_j_zero(bessel_order_n, root_order_m);
      // Create slices

      m_timeSlices.reserve( m_timeSlicesCount);
      // m_timeSlices.resize(m_timeSlicesCount);

        // const float sampleMinT = 0.0f;
    //initialize slices with datarows
    // for (int i(0); i < m_timeSlicesCount; i++) {
    //
    //     QSurfaceDataArray* array = new QSurfaceDataArray;
    //     array->reserve(m_sampleCount);
    //     // QSurfaceDataArray &array = m_timeSlices[i];
    //     for (int j(0); j < m_sampleCount; j++)
    //         array->append(new QSurfaceDataRow(m_sampleCount));
    //     m_timeSlices[i] = array;
    // }

  // Populate slices
  for (int i(0); i < m_timeSlicesCount; i++) {

    // QSurfaceDataArray* slice = m_timeSlices[i];

    QSurfaceDataArray* slice = new QSurfaceDataArray;
    slice->reserve(m_sampleCount);


    const float sampleMinT = 0.0f;
    const float sampleMaxT = (2 * M_PI * m_radius) / (m_wave_speed * bessel_root);
    float stepT = (sampleMaxT - sampleMinT) / float(m_timeSlicesCount - 1);
    float t = qMin(sampleMaxT, (i * stepT + sampleMinT));
    auto temporal = qCos(m_wave_speed * (bessel_root / m_radius) * t);

    for (int j(0); j < m_sampleCount; j++) {
      // QSurfaceDataRow& row = *(slice->at(j));
    QSurfaceDataRow *newRow = new QSurfaceDataRow(m_sampleCount);
    auto sampleMaxR = m_radius;
    float stepR = (sampleMaxR - sampleMinR) / float(m_sampleCount - 1);
    float r = qMin(sampleMaxR, (j * stepR + sampleMinR));
    auto radial =  boost::math::cyl_bessel_j(bessel_order_n, (bessel_root / m_radius) * r);
    int index = 0;
      for (int k = 0; k < m_sampleCount; k++) {

        float stepTheta = (sampleMaxTheta - sampleMinTheta) / float(m_sampleCount - 1);
        float theta = qMin(sampleMaxTheta, (k * stepTheta + sampleMinTheta));

        auto angular = qCos(bessel_order_n * theta);
        auto z = radial * angular * temporal ;
          (*newRow)[index++].setPosition(QVector3D(theta, z, r));
      }

    *slice << newRow;
    } //rows
   m_timeSlices << slice;
  } //slices
}


 QVector<QSurfaceDataArray*>
Solution::getTimeSlices( )  {
      // qDebug() << "size:" << m_timeSlices.size();
      // qDebug() << "cont" <<  m_timeSlices.count();

return m_timeSlices;
}


void Solution::clearData() {
  for (int i(0); i < m_timeSlices.size(); i++) {
    QSurfaceDataArray* array = m_timeSlices[i];
    for (int j(0); j < array->size(); j++) delete (*array)[j];
    array -> clear();
  }
}
