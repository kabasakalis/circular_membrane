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
  explicit Membrane(Solution solution);
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
    void setSelectedBesselOrder(int n);
    void setSelectedBesselRoot(int m);
private:
  void activateNormalMode();
  void setUpUi();
  void setModeLabel();
  Q3DSurface *m_graph;
  QSurfaceDataProxy *m_membraneProxy{0};
  QSurface3DSeries *m_membraneSeries{0};
  std::atomic<int> m_timeSliceIndex{0};
  Solution m_solution;
  QSurfaceDataArray* m_resetArray;
  float m_selected_bessel_order;
  int   m_selected_bessel_root;
  QLabel* m_modeLabel;
  };

#endif  // MEMBRANE_H
