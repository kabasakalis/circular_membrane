
#ifndef SOLUTION_H
#define SOLUTION_H

#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

class Solution : public QObject
{
    Q_OBJECT
public:
    explicit Solution(int sampleCount, int timeSlicesCount, float radius, float wave_speed, QObject *parent = 0);
    virtual ~Solution();

    //! [0]
public Q_SLOTS:
    void generateData(int cacheCount, int rowCount, int columnCount,
                      float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

    void update(QSurface3DSeries *series);
    // void generate_row_data(QSurface3DSeries *series);
    void generate_row_data(  QSurfaceDataArray* dataArray, int bessel_order_n,int root_order_m );
    //! [0]
private:
    void clearData();

    QVector<QSurfaceDataArray> m_timeSlices;
    int m_index;
    QSurfaceDataArray* m_resetArray;
    float  m_radius;
    float  m_wave_speed;
    int m_sampleCount;
    int m_timeSlicesCount;
};

#endif
