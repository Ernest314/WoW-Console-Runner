#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPalette>
#include <QProcess>
#include <QStyleFactory>

#include <QMainWindow>

#include <vector>

#include "utils.h"
#include "console.h"
#include "palettes.h"

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

	std::vector<QString> load_saved_paths();
	static void load_saved_data();
};

#endif // MAINWINDOW_H
