
#include "qt_helpers.h"
#include <QtDataVisualization/QSurface3DSeries>


namespace qt_helpers {


 QSurfaceDataArray* newSurfaceDataArrayFromSource( QSurfaceDataArray* source_surface_data_array,
                                                  std::function<void(QSurfaceDataItem*)> modifier ){
  int sampleCount = source_surface_data_array -> size();
  auto newArray = new QSurfaceDataArray();
  newArray->reserve(sampleCount);
  for (int i(0); i < sampleCount; i++)
   newArray -> append(new QSurfaceDataRow(sampleCount));
  for (int i(0); i < sampleCount; i++) {
    const QSurfaceDataRow& sourceRow = *(source_surface_data_array->at(i));
    QSurfaceDataRow& row = *(*newArray)[i];
    for (int j(0); j < sampleCount; j++){
      row[j].setPosition(sourceRow.at(j).position());
      modifier(&row[j]);
    }
  }
return newArray;
 };

}
