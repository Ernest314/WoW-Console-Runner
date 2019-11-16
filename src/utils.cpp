#include "utils.h"

QString Utils::get_dir_of_path(QString path)
{
	int i = -1;

	// go up an additional level if path is a directory ending in "/"
	if (path.endsWith("/")) {
		i = path.lastIndexOf("/", -2);
	} else {
		i = path.lastIndexOf("/");
	}

	// if "/" isn't found, i = -1 and we'll return the entire path
	QString trimmed = path.left(i);
	return trimmed;
}
