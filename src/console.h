#ifndef CONSOLE_H
#define CONSOLE_H

#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QObject>
#include <QPoint>
#include <QProcess>
#include <QStandardPaths>
#include <QString>
#include <QTextStream>

#include <QMenu>

#include <QFileDialog>
#include <QMainWindow>

#include "utils.h"
#include "tabconsole.h"

class Console : public QObject
{
	Q_OBJECT

public:
	static const unsigned int lines_buf = 1024;
	static const unsigned int chars_buf = 80 * lines_buf;

	// Cannot be static, or <APPNAME> isn't registered correctly
	const QString path_appdata =
			QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
	const QString path_saved_paths	= path_appdata + "paths.txt";
	const QString path_logs			= path_appdata + "Logs/";

	Console(
			QMainWindow* parent,
			QProcess* process,
			QString prefix_data,
			QString prefix_logs,
			QString name_exe );
	~Console();

signals:
	void state_changed(QProcess::ProcessState new_state);

public slots:
	void context_menu(const QPoint& pos);

	void start_process();
	void stop_process();
	void set_path();
	void clear_buffer();
	void open_logs();

private:
	QMainWindow* parent;
	TabConsole* host;

	QProcess* process;
	QTextStream logger;

	QString prefix_data;
	QString prefix_logs;
	QString name_exe;

	QMap<QString, QString> load_path_data(QString file);

	QString load_exe_path();
	void save_exe_path(QString path);

private slots:
	void pipe_output();
};

#endif // CONSOLE_H
