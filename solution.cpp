
#include "solution.h"
#include <QtCore/qmath.h>
#include <boost/math/special_functions/bessel.hpp>

using namespace QtDataVisualization;

constexpr const float sampleMinTheta = 0.0f;
constexpr const float sampleMaxTheta = 2 * M_PI;
constexpr const float sampleMinR = 0.0f;
constexpr const float sampleMaxR = 20.0f;
constexpr const float sampleMinR = 0.0f;
constexpr const float sampleMaxR = 20.0f;

Solution::Solution(int sampleCount, int timeSlicesCount, float radius,
                   float wave_speed, QObject *parent)
    : QObject(parent),
      m_index(-1),
      m_resetArray(0),
      m_radius(radius),
      m_wave_speed(wave_speed),
      m_sampleCount(sampleCount),
      m_timeSlicesCount(timeSlicesCount)

{
  //! [4]
  // qRegisterMetaType<QSurface3DSeries *>();
  //! [4]
}

Solution::~Solution() { clearData(); }

void Solution::generate_row_data(QSurfaceDataArray *dataArray,
                                 int bessel_order_n, int root_order_m) {
  // to the rounding errors.
  float stepTheta =
      (sampleMaxTheta - sampleMinTheta) / float(m_sampleCount - 1);
  float stepR = (sampleMaxR - sampleMinR) / float(m_sampleCount - 1);

  // QSurfaceDataArray *dataArray = new QSurfaceDataArray;
  dataArray->reserve(m_sampleCount);

  for (int i = 0; i < sampleCount; i++) {
    QSurfaceDataRow *newRow = new QSurfaceDataRow(m_sampleCount);
    float r = qMin(sampleMaxR, (i * stepR + sampleMinR));
    int index = 0;
    for (int j = 0; j < m_sampleCount; j++) {
      float theta = qMin(sampleMaxTheta, (j * stepTheta + sampleMinTheta));
      auto bessel_root =
          boost::math::cyl_bessel_j_zero(bessel_order_n, root_order_m);

      auto radial = boost::math::cyl_bessel_j(bessel_order_n,
      auto angular = qCos(bessel_order_n * theta);

      // const float sampleMinT = 0.0f;
      // const float sampleMaxT = (2 * M_PI) / (m_wave_speed * bessel_root);
      // float stepR = (sampleMaxT - sampleMinT) / float(m_timeSlicesCount - 1);
      // float t = qMin(sampleMaxT, (k * stepT + sampleMinT));
      // auto temporal = qCos(m_wave_speed * (bessel_root / m_radius) * t);


        auto z = radial * angular;
        (*newRow)[index++].setPosition(QVector3D(theta, z, r));
    }
    *dataArray << newRow;
  }
}

//! [0]
void Solution::generateData(int m_timeSlicesCount, int sampleCount,
                            float wave_speed, float radius,
                            int bessel_order_n, int root_order_m) {
  if (!m_timeSlicesCount || !sampleCount) return;
  clearData();

      auto bessel_root = boost::math::cyl_bessel_j_zero(bessel_order_n, root_order_m);
      const float sampleMaxT = (2 * M_PI) / (m_wave_speed * bessel_root);
      float stepR = (sampleMaxT - sampleMinT) / float(m_timeSlicesCount - 1);

      // float t = qMin(sampleMaxT, (k * stepT + sampleMinT));
      // auto temporal = qCos(m_wave_speed * (bessel_root / m_radius) * t);

      // Re-create the cache array
      m_timeSlices.resize(m_timeSlicesCount);
        // const float sampleMinT = 0.0f;

    //initialize slices with datarows
    for (int i(0); i < m_timeSlicesCount; i++) {
        QSurfaceDataArray &array = m_timeSlices[i];
        array.reserve(m_sampleCount);
        for (int j(0); j < m_sampleCount; j++)
            array.append(new QSurfaceDataRow(m_sampleCount));
    }


  // Populate slices
  for (int i(0); i < m_timeSlicesCount; i++) {

    QSurfaceDataArray& slice = m_timeSlices[i];



    // float cacheXAdjustment = cacheStep * i;
    // float cacheIndexAdjustment = cacheIndexStep * i;
    for (int j(0); j < m_sampleCount; j++) {
      QSurfaceDataRow& row = *(slice[j]);

    // QSurfaceDataRow *newRow = new QSurfaceDataRow(m_sampleCount);
    float r = qMin(sampleMaxR, (j * stepR + sampleMinR));
    int index = 0;
      for (int k = 0; k < m_sampleCount; k++) {
        float theta = qMin(sampleMaxTheta, (k * stepTheta + sampleMinTheta));
        auto bessel_root =
            boost::math::cyl_bessel_j_zero(bessel_order_n, root_order_m);
        auto radial = boost::math::cyl_bessel_j(bessel_order_n,
        auto angular = qCos(bessel_order_n * theta);

        // const float sampleMinT = 0.0f;
        // const float sampleMaxT = (2 * M_PI) / (m_wave_speed * bessel_root);
        // float stepR = (sampleMaxT - sampleMinT) / float(m_timeSlicesCount - 1);
        // float t = qMin(sampleMaxT, (k * stepT + sampleMinT));
        // auto temporal = qCos(m_wave_speed * (bessel_root / m_radius) * t);

          auto z = radial * angular;
          (*row)[index++].setPosition(QVector3D(theta, z, r));
      }

    } //rows

  } //slices
}
//! [0]

//! [1]
void Solution::update(QSurface3DSeries *series) {
  if (series && m_data.size()) {
    // Each iteration uses data from a different cached array
    m_index++;
    if (m_index > m_data.count() - 1) m_index = 0;

    QSurfaceDataArray array = m_data.at(m_index);
    int newRowCount = array.size();
    int newColumnCount = array.at(0)->size();

    // If the first time or the dimensions of the cache array have changed,
    // reconstruct the reset array
    if (m_resetArray || series->dataProxy()->rowCount() != newRowCount ||
        series->dataProxy()->columnCount() != newColumnCount) {
      m_resetArray = new QSurfaceDataArray();
      m_resetArray->reserve(newRowCount);
      for (int i(0); i < newRowCount; i++)
        m_resetArray->append(new QSurfaceDataRow(newColumnCount));
    }

    // Copy items from our cache to the reset array
    for (int i(0); i < newRowCount; i++) {
      const QSurfaceDataRow &sourceRow = *(array.at(i));
      QSurfaceDataRow &row = *(*m_resetArray)[i];
      for (int j(0); j < newColumnCount; j++)
        row[j].setPosition(sourceRow.at(j).position());
    }

    // Notify the proxy that data has changed
    series->dataProxy()->resetArray(m_resetArray);
  }
}
//! [1]

void Solution::clearData() {
  for (int i(0); i < m_data.size(); i++) {
    QSurfaceDataArray &array = m_data[i];
    for (int j(0); j < array.size(); j++) delete array[j];
    array.clear();
  }
}
