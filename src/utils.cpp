#include "utils.h"

QString Utils::get_dir_of_file(QString path)
{
	int i = path.lastIndexOf("/");
	QString trimmed = path.left(i);
	return trimmed;
}
