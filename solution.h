
#ifndef SOLUTION_H
#define SOLUTION_H

#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

class Solution : public QObject
{
    Q_OBJECT
public:
    explicit Solution(int sampleCount, int timeSlicesCount, float radius,
                      float wave_speed, QObject *parent = 0);
    virtual ~Solution();

    //! [0]

QVector<QSurfaceDataArray*> getTimeSlices() ;

public Q_SLOTS:

private:
    void clearData();
    void generateData( float bessel_order_n,int root_order_m );

    QVector<QSurfaceDataArray*> m_timeSlices;
    int m_index;
    QSurfaceDataArray* m_resetArray;
    float  m_radius;
    float  m_wave_speed;
    int m_sampleCount;
    int m_timeSlicesCount;
};

#endif
