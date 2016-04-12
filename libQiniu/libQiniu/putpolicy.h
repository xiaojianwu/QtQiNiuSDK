#ifndef PUTPOLICY_H
#define PUTPOLICY_H


#include <QJsonDocument>

class PutPolicy
{
public:
	PutPolicy();
	~PutPolicy();
public:

	static QString token(QString ak, QString sk, QString scope);

private:

};

#endif // PUTPOLICY_H
