#include "VisualSort_Qt.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1"); // to fix high dpi problem
	QApplication a(argc, argv);
	VisualSort_Qt w;
	w.show();
	return a.exec();
}