#ifndef VANVIEW_COMMON_H
#define VANVIEW_COMMON_H

#include <QtCore/QtCore>
#include <vector>
#include <string>

#define  SAFE_DELETE(X)  { if( (X) ) { delete (X) ;  (X)=NULL ; } }

void TransformStringVectorToQStringList(const std::vector<std::string>* stringVector, QStringList* qStringList);

#endif