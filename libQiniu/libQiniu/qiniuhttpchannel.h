#ifndef QINIUHTTPCHANNEL_H
#define QINIUHTTPCHANNEL_H

#include <QObject>

#include <QNetworkAccessManager>

#include <QNetworkReply>
#include "libqiniu.h"

class QiniuHttpChannel : public HttpChannel
{
	Q_OBJECT

public:
	QiniuHttpChannel();
	~QiniuHttpChannel();

	void init(int id, QString bucket, QString key, QString AK, QString SK, QString token);

	bool upload(QString filePath);

private slots:
	void onError(QNetworkReply::NetworkError error);
	void onFinished();
	void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
	QNetworkReply* doUploadFile(QString url, QHash<QString, QString> data, QHash<QString, QString> files);

private:
	int m_id;
	QString m_bucket;
	QString m_key;
	QString m_AK;
	QString m_SK;
	QString m_scope;
	QString m_token;
	QString m_filePath;
	QNetworkReply* m_reply;

	QNetworkAccessManager		m_nam;
	
};

#endif // QINIUHTTPCHANNEL_H
