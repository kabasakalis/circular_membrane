#include "membrane.h"

#include <QtCore/qmath.h>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QValue3DAxis>
#include <QtMath>
#include <QTimer>

#include <boost/math/special_functions/bessel.hpp>

using namespace QtDataVisualization;

const int Radius = 20;
const int sampleCountX = 50;
const int sampleCountZ = 50;

const float sampleMinX = 0.0f;
const float sampleMaxX = 2 * M_PI;

const float sampleMinZ = 0.0f;
const float sampleMaxZ = 20.0f;

const float A = 1.0f;
const float B = 1.0f;
const float C = 1.0f;
const float D = 1.0f;

Membrane::Membrane(Q3DSurface *surface, Solution* solution)
    : m_graph(surface), m_solution(solution) {
  m_graph->setAxisX(new QValue3DAxis);
  m_graph->setAxisY(new QValue3DAxis);
  m_graph->setAxisZ(new QValue3DAxis);

  //! [0]
  m_membraneProxy = new QSurfaceDataProxy();
  m_membraneSeries = new QSurface3DSeries(m_membraneProxy);
  //! [0]

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeSlice()));
  timer->start(100);

  // fillGraphProxy();
}

Membrane::~Membrane() { delete m_graph; }

//! [1]
void Membrane::fillGraphProxy() {
  float stepX = (sampleMaxX - sampleMinX) / float(sampleCountX - 1);
  float stepZ = (sampleMaxZ - sampleMinZ) / float(sampleCountZ - 1);

  QSurfaceDataArray *dataArray = new QSurfaceDataArray;
  dataArray->reserve(sampleCountZ);
  for (int i = 0; i < sampleCountZ; i++) {
    QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
    // Keep values within range bounds, since just adding step can cause minor
    // drift due
    // to the rounding errors.
    float z = qMin(sampleMaxZ, (i * stepZ + sampleMinZ));
    int index = 0;
    for (int j = 0; j < sampleCountX; j++) {
      float x = qMin(sampleMaxX, (j * stepX + sampleMinX));

      auto c = 1;
      auto bessel_order_n = 0.0f;
      auto root_order_m = 2;
      auto bessel_root =
          boost::math::cyl_bessel_j_zero(bessel_order_n, root_order_m);

      auto radial =
          boost::math::cyl_bessel_j(bessel_order_n, (bessel_root / 20.00f) * z);
      auto angular =
          C * qCos(bessel_order_n * x) + D * qSin(bessel_order_n * x);
      // auto  temporal = A * qCos(c*( bessel_root/ 20.00f) * t) + D * qSin(c *
      // (bessel_root/ 20.00f) * t);

      float y = radial * angular;
      (*newRow)[index++].setPosition(QVector3D(x, y, z));
    }
    *dataArray << newRow;
  }

  m_membraneProxy->resetArray(dataArray);
}
//! [1]

void Membrane::enableGraph(bool enable) {
  if (enable) {
    //! [3]
    m_membraneSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_membraneSeries->setFlatShadingEnabled(true);

    m_graph->axisX()->setLabelFormat("%.2f");
    m_graph->axisZ()->setLabelFormat("%.2f");
    m_graph->axisX()->setRange(sampleMinX, sampleMaxX);
    m_graph->axisY()->setRange(0.0f, 2.0f);
    m_graph->axisZ()->setRange(sampleMinZ, sampleMaxZ);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);

    //        m_graph->removeSeries(m_heightMapSeries);
    m_graph->addSeries(m_membraneSeries);
    //! [3]

    //! [8]
    // Reset range sliders for Sqrt&Sin
    m_rangeMinX = sampleMinX;
    m_rangeMinZ = sampleMinZ;
    m_stepX = (sampleMaxX - sampleMinX) / float(sampleCountX - 1);
    m_stepZ = (sampleMaxZ - sampleMinZ) / float(sampleCountZ - 1);
    m_axisMinSliderX->setMaximum(sampleCountX - 2);
    m_axisMinSliderX->setValue(0);
    m_axisMaxSliderX->setMaximum(sampleCountX - 1);
    m_axisMaxSliderX->setValue(sampleCountX - 1);
    m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
    m_axisMinSliderZ->setValue(0);
    m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
    m_axisMaxSliderZ->setValue(sampleCountZ - 1);
    //! [8]
  }
}

void Membrane::changeTheme(int theme) {
  m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void Membrane::updateTimeSlice() {
  auto timeSlices = m_solution->getTimeSlices();
  m_timeSliceIndex++;
  if (m_timeSliceIndex > timeSlices.count() - 2) m_timeSliceIndex = 0;
  // m_membraneProxy->resetArray(timeSlices.at(m_timeSliceIndex));
  m_membraneProxy->resetArray( timeSlices.at(m_timeSliceIndex));
  // m_membraneProxy->resetArray( Qt::black );
}

void Membrane::setBlackToYellowGradient() {
  //! [7]
  QLinearGradient gr;
  gr.setColorAt(0.0, Qt::black);
  gr.setColorAt(0.33, Qt::blue);
  gr.setColorAt(0.67, Qt::red);
  gr.setColorAt(1.0, Qt::yellow);

  m_graph->seriesList().at(0)->setBaseGradient(gr);
  m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
  //! [7]
}

void Membrane::setGreenToRedGradient() {
  QLinearGradient gr;
  gr.setColorAt(0.0, Qt::darkGreen);
  gr.setColorAt(0.5, Qt::yellow);
  gr.setColorAt(0.8, Qt::red);
  gr.setColorAt(1.0, Qt::darkRed);

  m_graph->seriesList().at(0)->setBaseGradient(gr);
  m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void Membrane::adjustXMin(int min) {
  float minX = m_stepX * float(min) + m_rangeMinX;

  int max = m_axisMaxSliderX->value();
  if (min >= max) {
    max = min + 1;
    m_axisMaxSliderX->setValue(max);
  }
  float maxX = m_stepX * max + m_rangeMinX;

  setAxisXRange(minX, maxX);
}

void Membrane::adjustXMax(int max) {
  float maxX = m_stepX * float(max) + m_rangeMinX;

  int min = m_axisMinSliderX->value();
  if (max <= min) {
    min = max - 1;
    m_axisMinSliderX->setValue(min);
  }
  float minX = m_stepX * min + m_rangeMinX;

  setAxisXRange(minX, maxX);
}

void Membrane::adjustZMin(int min) {
  float minZ = m_stepZ * float(min) + m_rangeMinZ;

  int max = m_axisMaxSliderZ->value();
  if (min >= max) {
    max = min + 1;
    m_axisMaxSliderZ->setValue(max);
  }
  float maxZ = m_stepZ * max + m_rangeMinZ;

  setAxisZRange(minZ, maxZ);
}

void Membrane::adjustZMax(int max) {
  float maxX = m_stepZ * float(max) + m_rangeMinZ;

  int min = m_axisMinSliderZ->value();
  if (max <= min) {
    min = max - 1;
    m_axisMinSliderZ->setValue(min);
  }
  float minX = m_stepZ * min + m_rangeMinZ;

  setAxisZRange(minX, maxX);
}

//! [5]
void Membrane::setAxisXRange(float min, float max) {
  m_graph->axisX()->setRange(min, max);
}

void Membrane::setAxisZRange(float min, float max) {
  m_graph->axisZ()->setRange(min, max);
}
