#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QFont>
#include <QHBoxLayout>
#include <QPalette>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTabBar>
#include <QTabWidget>
#include <QTextStream>
#include <QWidget>

#include <QMainWindow>

#include <unordered_map>
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
	// `path_saved_paths` also sounds bad...
	const QString path_paths =
			QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
			"/paths.txt";

	Ui::MainWindow* ui;
	std::unordered_map<QWidget*, Console*> consoles = {};

	std::vector<QString> load_saved_paths();

private slots:
	void update_saved_paths();
	void add_tab_blank() { add_tab(); }
	void add_tab(QString path="");
	void remove_tab(int index);
};

#endif // MAINWINDOW_H
