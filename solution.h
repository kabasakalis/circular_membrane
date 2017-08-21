
#ifndef SOLUTION_H
#define SOLUTION_H

#include <QtDataVisualization/QSurface3DSeries>
#include "qt_helpers.h"
#include <QtCore/qmath.h>

using namespace QtDataVisualization;
using namespace qt_helpers;

class Solution : public QObject
{



    Q_OBJECT
public:
      const static float sampleMinTheta ;
      const static float sampleMaxTheta ;
      const static float sampleMinR ;



    float radius() const;
    explicit Solution(int sampleCount, int timeSlicesCount, float radius,
                      float wave_speed, QObject *parent = 0);
    virtual ~Solution();

    //! [0]


QVector<QSurfaceDataArray*> getTimeSlices() ;
// static  QSurfaceDataArray* newSurfaceDataArrayFromSource(QSurfaceDataArray* source_surface_data_array);
public Q_SLOTS:

private:
    void clearData();
    void generateData( float bessel_order_n,int root_order_m );
    // void generateData( float bessel_order_n,int root_order_m );

    float get_bessel_root(float bessel_order_n, int root_order_m);
    float radial_solution(float r, float bessel_root, int bessel_order_n);

    float angular_solution(float theta, float bessel_order_n);
    float temporal_solution(float t, float bessel_root);

    QVector<QSurfaceDataArray*> m_timeSlices;
    QSurfaceDataArray* m_resetArray;
    float  m_radius;
    float  m_wave_speed;
    int m_sampleCount;
    int m_timeSlicesCount;
    float m_sampleMaxR;
    float m_stepR;
    float m_stepTheta;





};


#endif
