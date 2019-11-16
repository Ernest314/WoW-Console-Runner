#ifndef UTILS_H
#define UTILS_H

#include <QDir>
#include <QFile>
#include <QString>

namespace Utils
{
QString get_dir_of_path(QString path);

QFile get_created_file(QString path);
}

#endif // UTILS_H
