#include "membrane.h"

#include <QtCore/qmath.h>
#include <QTimer>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QValue3DAxis>
#include <QtMath>
#include <boost/math/special_functions/bessel.hpp>

#include <Q3DScene>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

using namespace QtDataVisualization;
using namespace qt_helpers;

const int Radius = 20;
const int sampleCountX = 25;
const int sampleCountZ = 25;

const float sampleMinX = 0.0f;
const float sampleMaxX = 2 * M_PI;

const float sampleMinZ = 0.0f;
const float sampleMaxZ = 20.0f;

Membrane::Membrane(Q3DSurface *surface, Solution *solution)
    : m_graph(surface),
      m_solution(solution),
      m_timeSlices(solution->getTimeSlices()),
      m_resetArray(0) {
  m_graph->setAxisX(new QValue3DAxis);
  m_graph->setAxisY(new QValue3DAxis);
  m_graph->setAxisZ(new QValue3DAxis);
  m_membraneProxy = new QSurfaceDataProxy();
  m_membraneSeries = new QSurface3DSeries(m_membraneProxy);
  enableGraph(true);

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeSlice()));
  timer->start(50);

}

Membrane::~Membrane() { delete m_graph; }

void Membrane::enableGraph(bool enable) {
  if (enable) {
    //! [3]

    m_membraneSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    m_membraneSeries->setFlatShadingEnabled(true);
    QImage drumhead(":/maps/drumhead");
    m_membraneSeries->setTexture(drumhead);
    m_membraneSeries->setName("Drumhead");

    m_graph->axisX()->setLabelFormat("θ = %.2f");
    m_graph->axisZ()->setLabelFormat("r = %.2f");
    m_graph->axisX()->setRange(sampleMinX, sampleMaxX);
    m_graph->axisY()->setRange(-1.0f, 1.0f);
    m_graph->axisZ()->setRange(sampleMinZ, sampleMaxZ);
    m_graph->axisX()->setLabelAutoRotation(30);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(30);
    m_graph->addSeries(m_membraneSeries);


    // m_rangeMinX = sampleMinX;
    // m_rangeMinZ = sampleMinZ;
    // m_stepX = (sampleMaxX - sampleMinX) / float(sampleCountX - 1);
    // m_stepZ = (sampleMaxZ - sampleMinZ) / float(sampleCountZ - 1);

    // m_axisMinSliderX->setMaximum(sampleCountX - 2);
    // m_axisMinSliderX->setValue(0);
    // m_axisMaxSliderX->setMaximum(sampleCountX - 1);
    // m_axisMaxSliderX->setValue(sampleCountX - 1);
    // m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
    // m_axisMinSliderZ->setValue(0);
    // m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
    // m_axisMaxSliderZ->setValue(sampleCountZ - 1);

    //  m_axisMinSliderZ = new QSlider(Qt::Horizontal, widget);
    //
    // m_axisMinSliderZ->setMaximum(2.0f);
    // m_axisMinSliderZ->setValue(00.0f);
    // m_axisMaxSliderZ->setMaximum(20.0f);
    // m_axisMaxSliderZ->setValue(20.0f);

    //! [8]
  }
}

void Membrane::changeTheme(int theme) {
  m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void Membrane::updateTimeSlice() {
  m_timeSliceIndex++;
  if (m_timeSliceIndex > m_timeSlices.size() - 1) m_timeSliceIndex = 0;
  qDebug() << "m_timeSliceIndex" << m_timeSliceIndex;
  auto qsurface_data_array = m_timeSlices.at(m_timeSliceIndex);
  auto modifier = [](QSurfaceDataItem *item) -> void { item->position(); };
  m_resetArray = newSurfaceDataArrayFromSource(qsurface_data_array, modifier);
  m_membraneProxy->resetArray(m_resetArray);
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

void Membrane::setUpUi(Membrane *modifier, Q3DSurface *graph) {
  // Q3DSurface *graph = new Q3DSurface();
  // graph-> setPolar(true);

  // graph ->
  // scene()->activeCamera()->setCameraPreset(Q3DCamera::Q3DCamera::CameraPresetBehindBelow)
  // ;

  QWidget *container = QWidget::createWindowContainer(graph);

  //! [0]

  if (!graph->hasContext()) {
    QMessageBox msgBox;
    msgBox.setText("Couldn't initialize the OpenGL context.");
    msgBox.exec();
    // return -1;
  }

  QSize screenSize = graph->screen()->size();
  container->setMinimumSize(
      QSize(screenSize.width() / 2, screenSize.height() / 1.6));
  container->setMaximumSize(screenSize);
  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  container->setFocusPolicy(Qt::StrongFocus);

  //! [1]
  QWidget *widget = new QWidget;
  QHBoxLayout *hLayout = new QHBoxLayout(widget);
  QVBoxLayout *vLayout = new QVBoxLayout();
  hLayout->addWidget(container, 1);
  hLayout->addLayout(vLayout);
  vLayout->setAlignment(Qt::AlignTop);
  //! [1]

  widget->setWindowTitle(
      QStringLiteral("Normal modes of Circular Membrane Vibration."));

  QGroupBox *modelGroupBox = new QGroupBox(QStringLiteral("Model"));

  // Vibration Button and Layout
  // QRadioButton *VibrationModelRB = new QRadioButton(widget);
  // VibrationModelRB->setText(QStringLiteral("Vibration"));
  // VibrationModelRB->setChecked(false);

  QVBoxLayout *modelVBox = new QVBoxLayout;
  // modelVBox->addWidget(VibrationModelRB);
  modelGroupBox->setLayout(modelVBox);

  // Selection
  QGroupBox *selectionGroupBox =
      new QGroupBox(QStringLiteral("Selection Mode"));

  QRadioButton *modeNoneRB = new QRadioButton(widget);
  modeNoneRB->setText(QStringLiteral("No selection"));
  modeNoneRB->setChecked(false);

  QRadioButton *modeItemRB = new QRadioButton(widget);
  modeItemRB->setText(QStringLiteral("Item"));
  modeItemRB->setChecked(false);

  QRadioButton *modeSliceRowRB = new QRadioButton(widget);
  modeSliceRowRB->setText(QStringLiteral("Row Slice"));
  modeSliceRowRB->setChecked(true);

  QRadioButton *modeSliceColumnRB = new QRadioButton(widget);
  modeSliceColumnRB->setText(QStringLiteral("Column Slice"));
  modeSliceColumnRB->setChecked(false);

  QVBoxLayout *selectionVBox = new QVBoxLayout;
  selectionVBox->addWidget(modeNoneRB);
  selectionVBox->addWidget(modeItemRB);
  selectionVBox->addWidget(modeSliceRowRB);
  selectionVBox->addWidget(modeSliceColumnRB);
  selectionGroupBox->setLayout(selectionVBox);

  // Slider

  // QSlider *axisMinSliderX = new QSlider(Qt::Horizontal, widget);
  // axisMinSliderX->setMinimum(0);
  // axisMinSliderX->setTickInterval(1);
  // axisMinSliderX->setEnabled(true);
  // QSlider *axisMaxSliderX = new QSlider(Qt::Horizontal, widget);
  // axisMaxSliderX->setMinimum(1);
  // axisMaxSliderX->setTickInterval(1);
  // axisMaxSliderX->setEnabled(true);
  // QSlider *axisMinSliderZ = new QSlider(Qt::Horizontal, widget);
  // axisMinSliderZ->setMinimum(0);
  // axisMinSliderZ->setTickInterval(1);
  // axisMinSliderZ->setEnabled(true);
  // QSlider *axisMaxSliderZ = new QSlider(Qt::Horizontal, widget);
  // axisMaxSliderZ->setMinimum(1);
  // axisMaxSliderZ->setTickInterval(1);
  // axisMaxSliderZ->setEnabled(true);

  // axisMinSliderX->setMaximum(sampleCountX - 2);
  // axisMinSliderX->setValue(0);
  // axisMaxSliderX->setMaximum(sampleCountX - 1);
  // axisMaxSliderX->setValue(sampleCountX - 1);
  // axisMinSliderZ->setMaximum(sampleCountZ - 2);
  // axisMinSliderZ->setValue(0);
  // axisMaxSliderZ->setMaximum(sampleCountZ - 1);
  // axisMaxSliderZ->setValue(sampleCountZ - 1);

  QComboBox *themeList = new QComboBox(widget);
  themeList->addItem(QStringLiteral("Qt"));
  themeList->addItem(QStringLiteral("Primary Colors"));
  themeList->addItem(QStringLiteral("Digia"));
  themeList->addItem(QStringLiteral("Stone Moss"));
  themeList->addItem(QStringLiteral("Army Blue"));
  themeList->addItem(QStringLiteral("Retro"));
  themeList->addItem(QStringLiteral("Ebony"));
  themeList->addItem(QStringLiteral("Isabelle"));

  QGroupBox *colorGroupBox = new QGroupBox(QStringLiteral("Custom gradient"));

  QLinearGradient grBtoY(0, 0, 1, 100);
  grBtoY.setColorAt(1.0, Qt::black);
  grBtoY.setColorAt(0.67, Qt::blue);
  grBtoY.setColorAt(0.33, Qt::red);
  grBtoY.setColorAt(0.0, Qt::yellow);
  QPixmap pm(24, 100);
  QPainter pmp(&pm);
  pmp.setBrush(QBrush(grBtoY));
  pmp.setPen(Qt::NoPen);
  pmp.drawRect(0, 0, 24, 100);
  QPushButton *gradientBtoYPB = new QPushButton(widget);
  gradientBtoYPB->setIcon(QIcon(pm));
  gradientBtoYPB->setIconSize(QSize(24, 100));

  QLinearGradient grGtoR(0, 0, 1, 100);
  grGtoR.setColorAt(1.0, Qt::darkGreen);
  grGtoR.setColorAt(0.5, Qt::yellow);
  grGtoR.setColorAt(0.2, Qt::red);
  grGtoR.setColorAt(0.0, Qt::darkRed);
  pmp.setBrush(QBrush(grGtoR));
  pmp.drawRect(0, 0, 24, 100);
  QPushButton *gradientGtoRPB = new QPushButton(widget);
  gradientGtoRPB->setIcon(QIcon(pm));
  gradientGtoRPB->setIconSize(QSize(24, 100));

  QHBoxLayout *colorHBox = new QHBoxLayout;
  colorHBox->addWidget(gradientBtoYPB);
  colorHBox->addWidget(gradientGtoRPB);
  colorGroupBox->setLayout(colorHBox);

  vLayout->addWidget(modelGroupBox);
  vLayout->addWidget(selectionGroupBox);
  // vLayout->addWidget(new QLabel(QStringLiteral("Column range")));
  // vLayout->addWidget(axisMinSliderX);
  // vLayout->addWidget(axisMaxSliderX);
  // vLayout->addWidget(new QLabel(QStringLiteral("Row range")));
  // vLayout->addWidget(axisMinSliderZ);
  // vLayout->addWidget(axisMaxSliderZ);
  vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
  vLayout->addWidget(themeList);
  vLayout->addWidget(colorGroupBox);

  widget->show();

  // modifier-> enableGraph(true);

  // QObject::connect(VibrationModelRB, &QRadioButton::toggled,
  //                  modifier, &Membrane::enableGraph);

  QObject::connect(modeNoneRB, &QRadioButton::toggled, modifier,
                   &Membrane::toggleModeNone);
  QObject::connect(modeItemRB, &QRadioButton::toggled, modifier,
                   &Membrane::toggleModeItem);
  QObject::connect(modeSliceRowRB, &QRadioButton::toggled, modifier,
                   &Membrane::toggleModeSliceRow);
  QObject::connect(modeSliceColumnRB, &QRadioButton::toggled, modifier,
                   &Membrane::toggleModeSliceColumn);
  // QObject::connect(axisMinSliderX, &QSlider::valueChanged,
  //                  modifier, &Membrane::adjustXMin);
  // QObject::connect(axisMaxSliderX, &QSlider::valueChanged,
  //                  modifier, &Membrane::adjustXMax);
  // QObject::connect(axisMinSliderZ, &QSlider::valueChanged,
  //                  modifier, &Membrane::adjustZMin);
  // QObject::connect(axisMaxSliderZ, &QSlider::valueChanged,
  //                  modifier, &Membrane::adjustZMax);
  QObject::connect(themeList, SIGNAL(currentIndexChanged(int)), modifier,
                   SLOT(changeTheme(int)));
  QObject::connect(gradientBtoYPB, &QPushButton::pressed, modifier,
                   &Membrane::setBlackToYellowGradient);
  QObject::connect(gradientGtoRPB, &QPushButton::pressed, modifier,
                   &Membrane::setGreenToRedGradient);

  // modifier->setAxisMinSliderX(axisMinSliderX);
  // modifier->setAxisMaxSliderX(axisMaxSliderX);
  // modifier->setAxisMinSliderZ(axisMinSliderZ);
  // modifier->setAxisMaxSliderZ(axisMaxSliderZ);

  // VibrationModelRB->setChecked(true);
  // modeItemRB->setChecked(true);
  themeList->setCurrentIndex(2);
}
