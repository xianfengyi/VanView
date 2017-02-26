#include "VanViewComman.h"

using std::string;
using std::vector;

void TransformStringVectorToQStringList(const vector<string>* stringVector, QStringList* qStringList)
{
	qStringList->clear();

	for (vector<string>::const_iterator it = stringVector->begin(); it != stringVector->end(); it++)
	{
		QString item = QString::fromLocal8Bit((*it).c_str());
		*qStringList << item;
	}
}