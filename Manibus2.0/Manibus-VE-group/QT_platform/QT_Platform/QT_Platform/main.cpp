#include "QT_Platform.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QT_Platform w;
	w.show();
	return a.exec();
}
