
#ifndef QTHELPERS_H
#define QTHELPERS_H

#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

namespace qt_helpers {

 QSurfaceDataArray* newSurfaceDataArrayFromSource(QSurfaceDataArray* source_surface_data_array, std::function<void(QSurfaceDataItem*)> modifier );

}

#endif
