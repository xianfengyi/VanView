#include "vanview.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VanView w;
	w.show();
	return a.exec();
}
