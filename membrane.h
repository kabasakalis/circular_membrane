#ifndef MEMBRANE_H
#define MEMBRANE_H

#include <QMainWindow>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <atomic>
#include "solution.h"
#include "qt_helpers.h"

using namespace QtDataVisualization;

//namespace Ui {
//  class Membrane;
//}

class Membrane : public QObject
{
  Q_OBJECT

public:
  explicit Membrane(Q3DSurface* surface, Solution* solution );
  ~Membrane();

   void enableGraph(bool enable);

//   void enableVibrationMode(size_t bessel_kind, size_t root);




// static  QSurfaceDataArray* newSurfaceDataArrayFromSource(QSurfaceDataArray* source_surface_data_array);

//     //! [0]
 void toggleModeNone() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
 void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
 void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                       | QAbstract3DGraph::SelectionSlice); }
 void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                          | QAbstract3DGraph::SelectionSlice); }
//  //! [0]

 void setBlackToYellowGradient();
 void setGreenToRedGradient();

  void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
  void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
  void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
  void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }
 void adjustXMin(int min);
 void adjustXMax(int max);
 void adjustZMin(int min);
 void adjustZMax(int max);

static void setUpUi( Membrane* membrane, Q3DSurface* graph);

 public Q_SLOTS:
    void changeTheme(int theme);
    // void updateGraph(QSurface3DSeries series);
    void updateTimeSlice();

    // QVector<QSurfaceDataArray*> getTimeSlices() ;
private:
  Q3DSurface *m_graph;
  QSurfaceDataProxy *m_membraneProxy;
  QSurface3DSeries *m_membraneSeries;

  QSlider *m_axisMinSliderX;
  QSlider *m_axisMaxSliderX;
  QSlider *m_axisMinSliderZ;
  QSlider *m_axisMaxSliderZ;
  float m_rangeMinX;
  float m_rangeMinZ;
  float m_stepX;
  float m_stepZ;



  void setAxisXRange(float min, float max);
  void setAxisZRange(float min, float max);
  // void fillGraphProxy();


  std::atomic<int> m_timeSliceIndex{0};
  Solution* m_solution;
  QVector<QSurfaceDataArray*> m_timeSlices;
  QSurfaceDataArray* m_resetArray;
//  Ui::Membrane *ui;


};

#endif // MEMBRANE_H
