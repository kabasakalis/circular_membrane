#include "membrane.h"
#include <QtWidgets/QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Solution* solution = new Solution(25,50, 20.0f, 100 );
    new Membrane(solution);
    return app.exec();
}
