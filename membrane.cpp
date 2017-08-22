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

Membrane::Membrane(Solution *solution)
    : m_graph(new Q3DSurface()),
      m_solution(solution),
      m_resetArray(0),
      m_selected_bessel_order{0.0f},
      m_selected_bessel_root{1}
{
  initializeGraph();
  initializeSeries();
  setUpUi();

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeSlice()));
  timer->start(50);
}

Membrane::~Membrane() { delete m_graph; }


void Membrane::initializeSeries() {
  m_graph->removeSeries(m_membraneSeries);
  m_membraneProxy = new QSurfaceDataProxy();
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
  // qDebug() << "m_timeSliceIndex" << m_timeSliceIndex;
  auto qsurface_data_array = m_solution->getTimeSlices().at(m_timeSliceIndex);
  auto modifier = [](QSurfaceDataItem *item) -> void { item->position(); };
  m_resetArray = newSurfaceDataArrayFromSource(qsurface_data_array, modifier);
  m_membraneProxy->resetArray(m_resetArray);
}


void Membrane::setSelectedBesselOrder( int n) {
  m_selected_bessel_order = static_cast<float>(n);
  qDebug() << "Setting bessel order" << m_selected_bessel_order;
}


void Membrane::setSelectedBesselRoot(int m) {
  m_selected_bessel_root = m;
  qDebug() << "Setting root order" << m_selected_bessel_root;
}


void Membrane::activateNormalMode() {
  initializeSeries();
  // qDebug() << "Selected bessel order" << m_selected_bessel_order;
  // qDebug() << "Selected bessel root" << m_selected_bessel_root;
  m_solution->generateData(m_selected_bessel_order, m_selected_bessel_root);
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
  //! [1]

  widget->setWindowTitle(
      QStringLiteral("Normal modes of Circular Membrane Vibration."));

  QGroupBox *normalModeGroupBox = new QGroupBox(QStringLiteral("Normal Modes"));
  QVBoxLayout *normalModeVBox = new QVBoxLayout;


  modeLabel = new QLabel(widget);
  modeLabel->setTextFormat(Qt::RichText);
  modeLabel->setText("<b>Tesdting</b>\n");
  QString status = QString("Processing file %1 of %2:")
                .arg(m_selected_bessel_order).arg(m_selected_bessel_root);
  modeLabel->setText( modeLabel->text() + status);
  normalModeVBox->addWidget(modeLabel);

  QSpinBox *besselOrderSbx = new QSpinBox(widget);
  besselOrderSbx->setRange(0, 10000);
  besselOrderSbx->setPrefix("Bessel Function Order n:       ");
  normalModeVBox->addWidget(besselOrderSbx);


  QSpinBox *besselRootSbx = new QSpinBox(widget);
  besselRootSbx->setRange(1, 5000);
  besselRootSbx->setPrefix("Bessel Root m:                 ");
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
