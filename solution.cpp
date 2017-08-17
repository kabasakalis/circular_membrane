
#include "solution.h"
#include <QtCore/qmath.h>
#include <boost/math/special_functions/bessel.hpp>

using namespace QtDataVisualization;


constexpr const float sampleMinTheta = 0.0f;
constexpr const float sampleMaxTheta = 2 * M_PI;
constexpr const float sampleMinR = 0.0f;
constexpr const float sampleMaxR = 20.0f;




// const float A = 1.0f;
// const float B = 1.0f;
// const float C = 1.0f;
// const float D = 1.0f;



Solution::Solution(int sampleCount, int timeSlicesCount, float radius, float wave_speed, QObject* parent) :
    QObject(parent),
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

Solution::~Solution()
{
    clearData();
}

void Solution::generate_row_data( QSurfaceDataArray* dataArray,int bessel_order_n,int root_order_m ){

// to the rounding errors.
float stepTheta = (sampleMaxTheta - sampleMinTheta) / float(m_sampleCount - 1);
float stepR = (sampleMaxR - sampleMinR) / float(m_sampleCount - 1);

  // QSurfaceDataArray *dataArray = new QSurfaceDataArray;
  dataArray->reserve(m_sampleCount);
  for (int i = 0; i < sampleCount; i++) {
    QSurfaceDataRow *newRow = new QSurfaceDataRow(m_sampleCount);
    float r = qMin(sampleMaxR, (i * stepR + sampleMinR));
    int index = 0;
    for (int j = 0; j < m_sampleCount; j++) {
      float theta = qMin(sampleMaxTheta, (j * stepTheta + sampleMinTheta));

      // auto bessel_order_n = 0.0f;
      // auto root_order_m = 2;
      auto bessel_root =
          boost::math::cyl_bessel_j_zero(bessel_order_n, root_order_m);

      auto radial =    boost::math::cyl_bessel_j(bessel_order_n, (bessel_root / m_radius) * r);
      auto angular =  qCos(bessel_order_n * theta) ;
      // auto  temporal = A * qCos(c*( bessel_root/ 20.00f) * t) + D * qSin(c *
      // (bessel_root/ 20.00f) * t);

      float z = radial * angular;
      (*newRow)[index++].setPosition(QVector3D(theta, z, r));
    }
    *dataArray << newRow;
  }
}

//! [0]
void Solution::generateData(  int cacheCount, int sampleCount,
                              float xMin, float xMax,
                              float zMin, float zMax,
                              float wave_speed, float radius,
                              float n_bessel_order, m_bessel_root)
{
    if (!cacheCount || !sampleCount ) return;
    clearData();
    // Re-create the cache array
    m_timeSlicesCount.resize(cacheCount);
    for (int i(0); i < cacheCount; i++) {
        QSurfaceDataArray &array = timeSlicesCount[i];
        array.reserve(m_sampleCount);
        for (int j(0); j < sampleCount; j++)
            array.append(new QSurfaceDataRow(sampleCount));
    }


      auto bessel_root =
          boost::math::cyl_bessel_j_zero( n_bessel_order , m_bessel_root  );
       auto radial =
          boost::math::cyl_bessel_j(bessel_order_n, (bessel_root / radius) * z);
      auto angular =
         qCos(bessel_order_n * x);

      // auto  temporal = A * qCos(c*( bessel_root/ 20.00f) * t) + D * qSin(c *
      // (bessel_root/ 20.00f) * t);


    float xRange = xMax - xMin;
    float yRange = yMax - yMin;
    float zRange = zMax - zMin;
    int cacheIndexStep = columnCount / cacheCount;
    float cacheStep = float(cacheIndexStep) * xRange / float(columnCount);

    // Populate caches
    for (int i(0); i < cacheCount; i++) {
        QSurfaceDataArray &cache = m_data[i];
        float cacheXAdjustment = cacheStep * i;
        float cacheIndexAdjustment = cacheIndexStep * i;
        for (int j(0); j < rowCount; j++) {
            QSurfaceDataRow &row = *(cache[j]);
            float rowMod = (float(j)) / float(rowCount);
            float yRangeMod = yRange * rowMod;
            float zRangeMod = zRange * rowMod;
            float z = zRangeMod + zMin;
            qreal rowColWaveAngleMul = M_PI * M_PI * rowMod;
            float rowColWaveMul = yRangeMod * 0.2f;
            for (int k(0); k < columnCount; k++) {
                float colMod = (float(k)) / float(columnCount);
                float xRangeMod = xRange * colMod;
                float x = xRangeMod + xMin + cacheXAdjustment;
                float colWave = float(qSin((2.0 * M_PI * colMod) - (1.0 / 2.0 * M_PI)) + 1.0);
                float y = (colWave * ((float(qSin(rowColWaveAngleMul * colMod) + 1.0))))
                        * rowColWaveMul
                        + (0.15f * float(qrand()) / float(RAND_MAX)) * yRangeMod;

                int index = k + cacheIndexAdjustment;
                if (index >= columnCount) {
                    // Wrap over
                    index -= columnCount;
                    x -= xRange;
                }
                row[index] = QVector3D(x, y, z);
            }
        }
    }
}
//! [0]

//! [1]
void Solution::update(QSurface3DSeries *series)
{
    if (series && m_data.size()) {
        // Each iteration uses data from a different cached array
        m_index++;
        if (m_index > m_data.count() - 1)
            m_index = 0;

        QSurfaceDataArray array = m_data.at(m_index);
        int newRowCount = array.size();
        int newColumnCount = array.at(0)->size();

        // If the first time or the dimensions of the cache array have changed,
        // reconstruct the reset array
        if (m_resetArray || series->dataProxy()->rowCount() != newRowCount
                || series->dataProxy()->columnCount() != newColumnCount) {
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

void Solution::clearData()
{
    for (int i(0); i < m_data.size(); i++) {
        QSurfaceDataArray &array = m_data[i];
        for (int j(0); j < array.size(); j++)
            delete array[j];
        array.clear();
    }
}
