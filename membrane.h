#ifndef MEMBRANE_H
#define MEMBRANE_H

#include <QMainWindow>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <atomic>
#include "qt_helpers.h"
#include "solution.h"

using namespace QtDataVisualization;

class Membrane : public QObject {
  Q_OBJECT

 public:
  explicit Membrane(Solution* solution);
  ~Membrane();

    void initializeGraph();
    void initializeSeries();
    void toggleModeNone() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice); }


 public Q_SLOTS:
    void changeTheme(int theme);
    void updateTimeSlice();
    // void setSelectedBesselOrder(float n);
    void setSelectedBesselOrder(int n);
    void setSelectedBesselRoot(int m);
private:
  void activateNormalMode();
  void setUpUi();
  void setModeLabel();
  Q3DSurface *m_graph;
  QSurfaceDataProxy *m_membraneProxy;
  QSurface3DSeries *m_membraneSeries;
  std::atomic<int> m_timeSliceIndex{0};
  Solution* m_solution;
  QSurfaceDataArray* m_resetArray;
  float m_selected_bessel_order;
  int   m_selected_bessel_root;
  QLabel* m_modeLabel;
  };

#endif  // MEMBRANE_H
