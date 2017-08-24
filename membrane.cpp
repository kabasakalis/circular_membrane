/*
  Normal modes of a vibrating circular membrane (drumhead).
  Membrane Class.
  Qt 3D Surface implementation.
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>

using namespace QtDataVisualization;
using namespace qt_helpers;

Membrane::Membrane(Solution* solution)
    : m_graph(new Q3DSurface()),
      m_membraneProxy(new QSurfaceDataProxy()),
      m_solution(solution),
      m_resetArray(0),
      m_selected_bessel_order{0.0f},
      m_selected_bessel_root{1}
{
  setUpUi();
  initializeGraph();
  initializeSeries();
  activateNormalMode();

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeSlice()));
  timer->start(50);
}

Membrane::~Membrane() {
  delete m_membraneSeries;
  delete m_graph;
  delete m_solution;
}

void Membrane::initializeSeries() {
  m_membraneSeries = new QSurface3DSeries(m_membraneProxy);
  m_membraneSeries->setName("Drumhead");
  m_membraneSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
  m_membraneSeries->setFlatShadingEnabled(true);
  QImage drumhead(":/maps/drumhead");
  m_membraneSeries->setTexture(drumhead);
  m_graph->addSeries(m_membraneSeries);
}

void Membrane::initializeGraph() {
  m_graph->setPolar(true);
  m_graph->setAxisX(new QValue3DAxis);
  m_graph->setAxisY(new QValue3DAxis);
  m_graph->setAxisZ(new QValue3DAxis);
  m_graph->axisX()->setLabelFormat("θ = %.2f");
  m_graph->axisZ()->setLabelFormat("r = %.2f");
  m_graph->axisX()->setRange(Solution::sampleMinTheta,
                             Solution::sampleMaxTheta);
  m_graph->axisY()->setRange(Solution::sampleMinY, Solution::sampleMaxY);
  m_graph->axisZ()->setRange(Solution::sampleMinR, m_solution->radius());
  m_graph->axisX()->setLabelAutoRotation(30);
  m_graph->axisY()->setLabelAutoRotation(90);
  m_graph->axisZ()->setLabelAutoRotation(30);
}

void Membrane::changeTheme(int theme) {
  m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void Membrane::updateTimeSlice() {
  m_timeSliceIndex++;
  if (m_timeSliceIndex > m_solution->getTimeSlices().size() - 1)
    m_timeSliceIndex = 0;
  auto qsurface_data_array = m_solution->getTimeSlices().at(m_timeSliceIndex);
  auto modifier = [](QSurfaceDataItem item) -> void { item.position(); };
  m_membraneProxy->resetArray(newSurfaceDataArrayFromSource(qsurface_data_array, modifier));
}

void Membrane::setSelectedBesselOrder( int n) {
  m_selected_bessel_order = static_cast<float>(n);
}

void Membrane::setSelectedBesselRoot(int m) {
  m_selected_bessel_root = m;
}

void Membrane::activateNormalMode() {
  m_solution->generateData(m_selected_bessel_order, m_selected_bessel_root);
  setModeLabel();
}

void Membrane::setModeLabel() {
  QString header = QString("<b>Mode (%1, %2)</b><br>")
                  .arg(m_selected_bessel_order).arg(m_selected_bessel_root);
  QString frequency_title = QString("<b>Frequency Ratio:</b><br>");
  QString frequency_ratio = QString("f(%1, %2) = <b>%3</b> * f(0, 1)\n")
                  .arg(m_selected_bessel_order)
                  .arg(m_selected_bessel_root)
                  .arg( m_solution->frequency_ratio( m_selected_bessel_order, m_selected_bessel_root));
  m_modeLabel->setText(header + frequency_title + frequency_ratio);
}

void Membrane::setUpUi() {
  QWidget *container = QWidget::createWindowContainer(m_graph);
  if (!m_graph->hasContext()) {

    QMessageBox msgBox;
    msgBox.setText("Couldn't initialize the OpenGL context.");
    msgBox.exec();
  }

  QSize screenSize = m_graph->screen()->size();
  container->setMinimumSize(
      QSize(screenSize.width() / 2, screenSize.height() / 1.6));
  container->setMaximumSize(screenSize);
  container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  container->setFocusPolicy(Qt::StrongFocus);

  QWidget *widget = new QWidget;
  QHBoxLayout *hLayout = new QHBoxLayout(widget);
  QVBoxLayout *vLayout = new QVBoxLayout();
  hLayout->addWidget(container, 1);
  hLayout->addLayout(vLayout);
  vLayout->setAlignment(Qt::AlignTop);

  widget->setWindowTitle(
      QStringLiteral("Normal modes of a vibrating circular membrane (drumhead)."));

  QGroupBox *normalModeGroupBox = new QGroupBox();
  QVBoxLayout *normalModeVBox = new QVBoxLayout;

  // Normal modes form
  m_modeLabel = new QLabel(widget);
  m_modeLabel->setTextFormat(Qt::RichText);
  setModeLabel();
  normalModeVBox->addWidget(m_modeLabel);

  QSpinBox *besselOrderSbx = new QSpinBox(widget);
  besselOrderSbx->setRange(0, 10000);
  besselOrderSbx->setPrefix("Bessel Function Order n:       ");
  normalModeVBox->addWidget(besselOrderSbx);


  QSpinBox *besselRootSbx = new QSpinBox(widget);
  besselRootSbx->setRange(1, 5000);
  besselRootSbx->setPrefix("Bessel Root m:                         ");
  normalModeVBox->addWidget(besselRootSbx);

  QPushButton *normalModeResetB = new QPushButton("&Reset Normal Mode", widget);
  normalModeVBox->addWidget(normalModeResetB);

  normalModeGroupBox->setLayout(normalModeVBox);

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
  modeSliceRowRB->setText(QStringLiteral("Theta Slice"));
  modeSliceRowRB->setChecked(true);

  QRadioButton *modeSliceColumnRB = new QRadioButton(widget);
  modeSliceColumnRB->setText(QStringLiteral("Radial Slice"));
  modeSliceColumnRB->setChecked(false);

  QVBoxLayout *selectionVBox = new QVBoxLayout;
  selectionVBox->addWidget(modeNoneRB);
  selectionVBox->addWidget(modeItemRB);
  selectionVBox->addWidget(modeSliceRowRB);
  selectionVBox->addWidget(modeSliceColumnRB);
  selectionGroupBox->setLayout(selectionVBox);

  // Theme
  QComboBox *themeList = new QComboBox(widget);

  themeList->addItem(QStringLiteral("Qt"));
  themeList->addItem(QStringLiteral("Primary Colors"));
  themeList->addItem(QStringLiteral("Digia"));
  themeList->addItem(QStringLiteral("Stone Moss"));
  themeList->addItem(QStringLiteral("Army Blue"));
  themeList->addItem(QStringLiteral("Retro"));
  themeList->addItem(QStringLiteral("Ebony"));
  themeList->addItem(QStringLiteral("Isabelle"));

  vLayout->addWidget(normalModeGroupBox);
  vLayout->addWidget(selectionGroupBox);
  vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
  vLayout->addWidget(themeList);
  widget->show();

  // Bindings
  QObject::connect(normalModeResetB, &QPushButton::clicked, this,
                   &Membrane::activateNormalMode);
  QObject::connect(besselOrderSbx, SIGNAL(valueChanged(int)), this,
                     SLOT(setSelectedBesselOrder(int))) ;
  QObject::connect(besselRootSbx, SIGNAL(valueChanged(int)), this,
                     SLOT(setSelectedBesselRoot(int))) ;
  QObject::connect(modeNoneRB, &QRadioButton::toggled, this,
                   &Membrane::toggleModeNone);
  QObject::connect(modeItemRB, &QRadioButton::toggled, this,
                   &Membrane::toggleModeItem);
  QObject::connect(modeSliceRowRB, &QRadioButton::toggled, this,
                   &Membrane::toggleModeSliceRow);
  QObject::connect(modeSliceColumnRB, &QRadioButton::toggled, this,
                   &Membrane::toggleModeSliceColumn);
  QObject::connect(themeList, SIGNAL(currentIndexChanged(int)), this,
                   SLOT(changeTheme(int)));

  themeList->setCurrentIndex(7);
}
