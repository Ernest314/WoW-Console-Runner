#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFont>
#include <QHBoxLayout>
#include <QPalette>
#include <QProcess>
#include <QPushButton>
#include <QStyleFactory>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>

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

	std::vector<QString> load_saved_paths();

private slots:
	void add_tab();
	void remove_tab(int index);
};

#endif // MAINWINDOW_H
