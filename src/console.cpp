#include "console.h"

Console::Console(
		QMainWindow* parent,
		QPlainTextEdit* console,
		QLineEdit* lineEdit,
		QProcess* process,
		QString prefix_data,
		QString prefix_logs,
		QString name_exe) :
	parent(parent),
	console(console),
	lineEdit(lineEdit),
	process(process),
	logger(QTextStream()),
	prefix_data(prefix_data),
	prefix_logs(prefix_logs),
	name_exe(name_exe)
{
	// Change button state based on if processes are running.
	QObject::connect(
				process, &QProcess::stateChanged,
				this, &Console::state_changed );

	// Redirect process output to console widgets.
	QObject::connect(
				process, &QProcess::readyReadStandardOutput,
				this, &Console::pipe_output );

	// Grab context menu event on consoles (to add "clear" option).
	QObject::connect(
				console, &QPlainTextEdit::customContextMenuRequested,
				this, &Console::context_menu );

	// Initialize lineEdit text with the paths to their
	// corresponding .exe files.
	lineEdit->setText(load_exe_path());
}

Console::~Console()
{
	delete process;
}

void Console::start_process()
{
	// Clean up in case we are actually *re*starting.
	stop_process();
	clear_buffer();

	// Don't try to start if process isn't runnable.
	QString path = lineEdit->text();
	bool is_runnable = QFile::exists(path) && path.endsWith(".exe");
	if (!is_runnable) {
		Utils::show_warning("There is no valid executable specified.");
		return;
	}

	save_exe_path(path);

	// Open a handle to the log file.
	QString path_log =
			path_logs +
			prefix_logs +
			QDateTime::currentDateTime().toString(Qt::ISODate).remove(":") +
			".txt" ;
	QFile file = Utils::get_created_file(path_log);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	logger.setDevice(&file);

	// Set up executable options and run.
	process->setProgram(path);
	process->setWorkingDirectory(Utils::get_dir_of_path(path));
	process->setReadChannel(QProcess::StandardOutput);
	process->start();
}

void Console::stop_process()
{
	// no need to null-check--explicitly constructed with QProcess*
	process->close();
}

void Console::set_exe_path()
{
	// requiring *.exe is the most validation possible here,
	// without explicitly hooking into polybius.exe
	QString path =
			QFileDialog::getOpenFileName(
				parent,
				name_exe + " path",
				QDir::homePath(),
				"Executable (*.exe)" );
	if (path != nullptr) lineEdit->setText(path);
}

void Console::clear_buffer()
{
	console->setPlainText("");
}

void Console::open_logs()
{
	QDesktopServices::openUrl(path_logs);
}

void Console::pipe_output()
{
	while (process->canReadLine()) {
		QString line = process->readLine();
		line.remove("\r");

		// Log timestamped console output.
		QString date = QDateTime::currentDateTime().toString(Qt::ISODate);
		logger << date << "> " << line;
		logger.flush();	// write to file as soon as possible

		// Clear the oldest half of the buffer (to the nearest line),
		// then update console display.
		QString buffer = console->toPlainText();
		if (static_cast<unsigned int>(buffer.size()) > chars_buf) {
			int cutoff = buffer.indexOf("\n", chars_buf/2);
			buffer = buffer.remove(0, cutoff + 1);
		}
		console->setPlainText(buffer + line);
	}
}

void Console::context_menu(const QPoint& pos)
{
	QMenu* menu = console->createStandardContextMenu(pos);
	menu->addAction("Clea&r", this, &Console::clear_buffer);
	menu->exec(console->mapToGlobal(pos));
	delete menu;
}

QMap<QString, QString> Console::load_path_data(QString file)
{
	QMap<QString, QString> data = QMap<QString, QString>();
	QStringList lines = file.split("\n", QString::SkipEmptyParts);
	for (QString line : lines) {
		QString prefix_line = line.section(':', 0, 0);
		QString data_line = line.section(':', 1);
		data.insert(prefix_line, data_line);
	}
	return data;
}

QString Console::load_exe_path()
{
	// return user home directory as default if no paths.txt exists
	if (!QFile::exists(path_saved_paths)) {
		return QDir::homePath();
	}

	QFile file(path_saved_paths);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString data_str = file.readAll().trimmed();

	// read in current file's data
	QMap<QString, QString> data = load_path_data(data_str);

	QString path = data[prefix_data];
	if (path == "") {
		path = QDir::homePath();
	}
	return path;
}

void Console::save_exe_path(QString path)
{
	QFile file = Utils::get_created_file(path_saved_paths);
	file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
	QString data_str = file.readAll().trimmed();

	// read in current file's data
	QMap<QString, QString> data = load_path_data(data_str);

	// overwrites current value if it exists, otherwise inserts
	data.insert(prefix_data, path);

	// Write data out to file.
	QTextStream out(&file);
	for (QString key : data.keys()) {
		out << key << ":" << data[key];
	}
	out.flush();

	return;
}
