#ifndef AUTH_H
#define AUTH_H

#include <QString>
#include <QByteArray>

class Auth
{
public:
	Auth();
	~Auth();

public:
	static QString genUploadToken(QString accessKey, QString secretKey, QString putPolicy);

private:


	static QByteArray Auth::hmacSha1(QByteArray key, QByteArray baseString);
	
};

#endif // AUTH_H
