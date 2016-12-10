#include "weatherapp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	weatherapp w;
	w.show();
	return a.exec();
}
