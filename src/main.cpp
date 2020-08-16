#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFontDatabase::addApplicationFont(":/rc/FiraCode-Regular.ttf");
	MainWindow w;
	w.show();
	return a.exec();
}
