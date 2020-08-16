#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPalette>
#include <QProcess>
#include <QStyleFactory>

#include <QMainWindow>

#include <vector>

#include "utils.h"
#include "console.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow* ui;
	std::vector<Console*> consoles;

	static QPalette dark_palette();
	static void load_saved_data();
};

#endif // MAINWINDOW_H
