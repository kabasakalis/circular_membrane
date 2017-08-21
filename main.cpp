#include "membrane.h"
#include <QtWidgets/QApplication>

int main(int argc, char **argv)
{
    //! [0]
    QApplication app(argc, argv);
    Q3DSurface *graph = new Q3DSurface();
    graph-> setPolar(true);
    Solution* solution = new Solution(25,50, 20.0f, 100 );
    Membrane* modifier = new Membrane(graph, solution );

    // modifier-> enableGraph(true);

    Membrane::setUpUi( modifier, graph);

    return app.exec();
}
