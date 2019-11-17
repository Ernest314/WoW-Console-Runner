#ifndef UTILS_H
#define UTILS_H

#include <QDir>
#include <QFile>
#include <QString>

#include <QMessageBox>

namespace Utils
{
void show_warning(QString warning);

QString get_dir_of_path(QString path);

QFile* get_created_file(QString path);
}

#endif // UTILS_H
