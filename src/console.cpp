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
		QMessageBox message_error;
		message_error.setWindowTitle("WoW Console Runner");
		message_error.setText("There is no valid executable specified.");
		message_error.setIcon(QMessageBox::Warning);
		message_error.exec();
		return;
	}

	save_exe_path(path);

	// Open a handle to the log file.
	QString path_log =
			path_logs +
			prefix_logs +
			QDateTime::currentDateTime().toString(Qt::ISODate).remove(":") +
			".txt" ;
	QDir dir(path_log);
	dir.mkpath(Utils::get_dir_of_file(path_log));
	QFile* file = new QFile(path_log);
	file->open(QIODevice::WriteOnly | QIODevice::Text);
	logger.setDevice(file);

	// Set up executable options and run.
	process->setProgram(path);
	process->setWorkingDirectory(Utils::get_dir_of_file(path));
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

QString Console::load_exe_path()
{
	if (!QFile::exists(path_saved_paths)) {
		return QDir::homePath();
	}
	QFile* file = new QFile(path_saved_paths);
	file->open(QIODevice::ReadOnly | QIODevice::Text);
	QString data = file->readAll();
	QStringList lines = data.split("\n", QString::SkipEmptyParts);
	QString path = "";
	for (QString line : lines) {
		if (line.startsWith(prefix_data)) {
			path = line.remove(prefix_data);
		}
	}
	if (path == "") {
		path = QDir::homePath();
	}
	return path;
}

void Console::save_exe_path(QString path)
{
	QDir dir(path_saved_paths);
	dir.mkpath(Utils::get_dir_of_file(path_saved_paths));
	QFile file(path_saved_paths);
	file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
	QString data = file.readAll().trimmed();
	if (!data.contains(prefix_data)) {
		data += "\n" + prefix_data + path;
	}
	QString data_new = "";
	QStringList lines = data.split("\n", QString::SkipEmptyParts);
	for (QString line : lines) {
		if (line.startsWith(prefix_data)) {
			line = prefix_data + path;
		}
		data_new += line + "\n";
	}
	data_new = data_new.trimmed();
	QTextStream out(&file);
	out << data_new;
	return;
}
