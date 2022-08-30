#include "Subway.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Subway w;
	w.show();
	w.Check();
	return a.exec();
}