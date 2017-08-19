#include "membrane.h"
// #include <QApplication>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <Q3DScene>


#include <boost/math/special_functions/bessel.hpp>

int main(int argc, char **argv)
{
    //! [0]
    QApplication app(argc, argv);
    Q3DSurface *graph = new Q3DSurface();
    graph-> setPolar(true);

    // auto cam = *(graph -> scene())->setActiveCamera(new Q3DCamera());
       // *(graph -> scene())->activeCamera()->cameraPreset() = 1;

       graph -> scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetDirectlyBelow)  ;

    QWidget *container = QWidget::createWindowContainer(graph);

    //! [0]

    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
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

    widget->setWindowTitle(QStringLiteral("Normal modes of Circular Membrane Vibration."));



    QGroupBox *modelGroupBox = new QGroupBox(QStringLiteral("Model"));

    //Vibration Button and Layout
    QRadioButton *VibrationModelRB = new QRadioButton(widget);
    VibrationModelRB->setText(QStringLiteral("Vibration"));
    VibrationModelRB->setChecked(false);


    QVBoxLayout *modelVBox = new QVBoxLayout;
    modelVBox->addWidget(VibrationModelRB);
    modelGroupBox->setLayout(modelVBox);



//Selection
    QGroupBox *selectionGroupBox = new QGroupBox(QStringLiteral("Selection Mode"));

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


   //Slider
    QSlider *axisMinSliderX = new QSlider(Qt::Horizontal, widget);
    axisMinSliderX->setMinimum(0);
    axisMinSliderX->setTickInterval(1);
    axisMinSliderX->setEnabled(true);
    QSlider *axisMaxSliderX = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderX->setMinimum(1);
    axisMaxSliderX->setTickInterval(1);
    axisMaxSliderX->setEnabled(true);
    QSlider *axisMinSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMinSliderZ->setMinimum(0);
    axisMinSliderZ->setTickInterval(1);
    axisMinSliderZ->setEnabled(true);
    QSlider *axisMaxSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderZ->setMinimum(1);
    axisMaxSliderZ->setTickInterval(1);
    axisMaxSliderZ->setEnabled(true);

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
    vLayout->addWidget(new QLabel(QStringLiteral("Column range")));
    vLayout->addWidget(axisMinSliderX);
    vLayout->addWidget(axisMaxSliderX);
    vLayout->addWidget(new QLabel(QStringLiteral("Row range")));
    vLayout->addWidget(axisMinSliderZ);
    vLayout->addWidget(axisMaxSliderZ);
    vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(colorGroupBox);

    widget->show();



    Solution* solution = new Solution(25, 20, 20.0f, 400);


    Membrane* modifier = new Membrane(graph, solution );

    // modifier-> enableGraph(true);



    QObject::connect(VibrationModelRB, &QRadioButton::toggled,
                     modifier, &Membrane::enableGraph);



   QObject::connect(modeNoneRB, &QRadioButton::toggled,
                    modifier, &Membrane::toggleModeNone);
   QObject::connect(modeItemRB,  &QRadioButton::toggled,
                    modifier, &Membrane::toggleModeItem);
   QObject::connect(modeSliceRowRB,  &QRadioButton::toggled,
                    modifier, &Membrane::toggleModeSliceRow);
   QObject::connect(modeSliceColumnRB,  &QRadioButton::toggled,
                    modifier, &Membrane::toggleModeSliceColumn);
   QObject::connect(axisMinSliderX, &QSlider::valueChanged,
                    modifier, &Membrane::adjustXMin);
   QObject::connect(axisMaxSliderX, &QSlider::valueChanged,
                    modifier, &Membrane::adjustXMax);
   QObject::connect(axisMinSliderZ, &QSlider::valueChanged,
                    modifier, &Membrane::adjustZMin);
   QObject::connect(axisMaxSliderZ, &QSlider::valueChanged,
                    modifier, &Membrane::adjustZMax);
    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)),
                     modifier, SLOT(changeTheme(int)));
   QObject::connect(gradientBtoYPB, &QPushButton::pressed,
                    modifier, &Membrane::setBlackToYellowGradient);
   QObject::connect(gradientGtoRPB, &QPushButton::pressed,
                    modifier, &Membrane::setGreenToRedGradient);

    modifier->setAxisMinSliderX(axisMinSliderX);
    modifier->setAxisMaxSliderX(axisMaxSliderX);
    modifier->setAxisMinSliderZ(axisMinSliderZ);
    modifier->setAxisMaxSliderZ(axisMaxSliderZ);


    VibrationModelRB->setChecked(true);
    modeItemRB->setChecked(true);
    themeList->setCurrentIndex(2);

//    sqrtSinModelRB->setChecked(true);


     // modifier-> enableGraph(true);




    return app.exec();
}
