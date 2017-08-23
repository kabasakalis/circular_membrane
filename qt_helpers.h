/*
  Normal modes of a vibrating circular membrane (drumhead).
  Qt helpers.
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

#ifndef QTHELPERS_H
#define QTHELPERS_H

#include <QtDataVisualization/QSurface3DSeries>

using namespace QtDataVisualization;

namespace qt_helpers {

//  QSurfaceDataArray* newSurfaceDataArrayFromSource(QSurfaceDataArray* source_surface_data_array, std::function<void(QSurfaceDataItem*)> modifier );
//  void clearSurfaceDataArray( QSurfaceDataArray& array);
// }

 QSurfaceDataArray  newSurfaceDataArrayFromSource( QSurfaceDataArray& source_surface_data_array,
                                                  std::function<void(QSurfaceDataItem)> modifier );


void clearSurfaceDataArray( QSurfaceDataArray& array);
}

#endif
