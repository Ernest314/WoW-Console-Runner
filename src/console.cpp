#include "console.h"

Console::Console(QWidget* parent, QString path) :
	parent(parent),
	host(new ConsoleHost(parent)),
	process(new QProcess()),
	logger(QTextStream())
{
	// Redirect process output to console widget.
	QObject::connect(
			process, &QProcess::readyReadStandardOutput,
			this, &Console::pipe_output );

	// Initialize path to passed parameter.
	host->ui->lineEdit_path->setText(path);

	// Set path from a file picker dialog.
	QObject::connect(
			host->ui->button_path_set, &QPushButton::clicked,
			this, &Console::set_path);

	// Connect the exe name updated signals.
	QObject::connect(
			host, &ConsoleHost::exe_updated,
			this, &Console::exe_updated );

	// Change button states based on whether process is running.
	QObject::connect(
			process, &QProcess::stateChanged,
			host, &ConsoleHost::update_buttons );

	// Hook up UI buttons to functionality.
	QObject::connect(
			host->ui->button_run, &QPushButton::clicked,
			this, &Console::start_process );
	QObject::connect(
			host->ui->button_stop, &QPushButton::clicked,
			this, &Console::stop_process );
	QObject::connect(
			host->ui->button_logs, &QPushButton::clicked,
			this, &Console::open_logs );
}

Console::~Console()
{
	delete host;
	delete process;
}

void Console::pipe_output()
{
	while (process->canReadLine()) {
		QString line = process->readLine();
		// Console output line endings will naïvely render as doubled.
		// Remove one: QPlainTextEdit only needs LF, not CRLF / LFCR.
		line.replace(QRegularExpression("\r\n|\n\r"), "\n");

		// Log timestamped console output.
		QString date = QDateTime::currentDateTime().toString(Qt::ISODate);
		logger << date << "> " << line;
		logger.flush();	// write to file as soon as possible

		// When buffer fills up, clear the oldest half. (then,)
		// Update console display.
		QString buffer = host->ui->console->toPlainText();
		if (static_cast<unsigned int>(buffer.size()) > chars_buf) {
			int cutoff = buffer.indexOf("\n", lines_buf/2);
			buffer = buffer.remove(0, cutoff + 1);
		}
		host->ui->console->setPlainText(buffer + line);
	}
}

void Console::set_path()
{
	// The only validation the file picker needs to perform is checking that
	// the chosen file is indeed an .exe.
	QString path =
			QFileDialog::getOpenFileName(
				parent,
				"Executable path",
				QDir::homePath(),
				"Executable (*.exe)" );

	// Manually trigger `validate_path()` to ensure the chosen file does exist.
	// Otherwise this slot is only connected to `&QLineEdit::editingFinished`,
	// which only triggers
	// "when the Return or Enter key is pressed or the line edit loses focus".
	if (path != nullptr){
		host->ui->lineEdit_path->setText(path);
		host->validate_path();
	}
}

void Console::start_process()
{
	stop_process();		// clean up in case we are actually *re*starting
	host->ui->console->appendPlainText(runs_separator);

	// Don't try to start if process isn't runnable.
	QString path = host->get_path();
	bool is_runnable = QFile::exists(path) && path.endsWith(".exe");
	if (!is_runnable) {
		Utils::show_warning("There is no valid executable specified.");
		return;
	}

	// Open a handle to the log file.
	QString path_log =
			path_logs + host->get_exe_name() + " " +
			QDateTime::currentDateTime().toString(Qt::ISODate).remove(":") +
			".txt" ;
	QFile* file = Utils::get_created_file(path_log);
	bool is_opened = file->open(QIODevice::WriteOnly | QIODevice::Text);
	if (!is_opened) {
		Utils::show_warning("Could not create log file. Bot will run but output will not be logged.");
	}
	logger.setDevice(file);

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

void Console::open_logs()
{
	bool is_opened = QDesktopServices::openUrl(path_logs);
	if (!is_opened) {
		Utils::show_warning("Could not open Logs folder.");
	}
}
