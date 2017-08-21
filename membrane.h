#ifndef MEMBRANE_H
#define MEMBRANE_H

#include <QMainWindow>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtWidgets/QSlider>
#include <atomic>
#include "qt_helpers.h"
#include "solution.h"

using namespace QtDataVisualization;

class Membrane : public QObject {
  Q_OBJECT

 public:
  explicit Membrane(Solution* solution);
  ~Membrane();

  void enableGraph(bool enable);
  void toggleModeNone() {
    m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone);
  }
  void toggleModeItem() {
    m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem);
  }
  void toggleModeSliceRow() {
    m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow |
                              QAbstract3DGraph::SelectionSlice);
  }
  void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
 void setUpUi();

 public Q_SLOTS:
    void changeTheme(int theme);
    void updateTimeSlice();
private:
  Q3DSurface *m_graph;
  QSurfaceDataProxy *m_membraneProxy;
  QSurface3DSeries *m_membraneSeries;

  // void setAxisXRange(float min, float max);
  // void setAxisZRange(float min, float max);

  std::atomic<int> m_timeSliceIndex{0};
  Solution* m_solution;
  QVector<QSurfaceDataArray*> m_timeSlices;
  QSurfaceDataArray* m_resetArray;
  };

#endif  // MEMBRANE_H
