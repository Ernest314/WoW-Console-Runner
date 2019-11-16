#include "utils.h"

QString Utils::get_dir_of_file(QString path)
{
	int i = path.lastIndexOf("/");
	// if "/" isn't found, i = -1 and we'll return the entire path
	QString trimmed = path.left(i);
	return trimmed;
}
