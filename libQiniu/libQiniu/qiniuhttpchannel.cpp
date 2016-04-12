#include "qiniuhttpchannel.h"

#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>

#include "libqiniu.h"
QiniuHttpChannel::QiniuHttpChannel()
	: HttpChannel()
{

}

QiniuHttpChannel::~QiniuHttpChannel()
{

}

void QiniuHttpChannel::init(int id, QString bucket, QString key, QString AK, QString SK, QString token)
{
	m_id = id;
	m_bucket = bucket;
	m_key = key;
	m_AK = AK;
	m_SK = SK;
	m_token = token;
}

bool QiniuHttpChannel::upload(QString filePath)
{
	QString uploadURL = "http://upload.qiniu.com/";


	QHash<QString, QString> formData;
	//资源名，必须是UTF - 8编码。
	//	注意： 如果在生成上传凭证的上传策略中 scope指定为：<bucket> : <key>, 则该字段也必须指定。

	formData["key"] = m_key;

	// 必须是一个符合相应规格的上传凭证，否则会返回401表示权限认证失败。
	formData["token"] = m_token;

	QHash<QString, QString> files;
	// 文件本身。
	files["file"] = filePath;

	QNetworkReply* reply = doUploadFile(uploadURL, formData, files);

	m_reply = reply;

	return true;
}


QNetworkReply* QiniuHttpChannel::doUploadFile(QString url, QHash<QString, QString> data, QHash<QString, QString> files)
{
	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	QHttpPart paramPart;
	QString httpPacket;

	QHash<QString, QString>::const_iterator itor = data.constBegin();
	while (itor != data.constEnd())
	{
		QString key = itor.key();
		QString val = itor.value();

		paramPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + key + "\""));
		paramPart.setBody(val.toUtf8());
		multiPart->append(paramPart);

		itor++;
	}


	// 文件上传
	QHash<QString, QString>::const_iterator itorFile = files.constBegin();
	QString filePath;
	QString key;
	while (itorFile != files.constEnd())
	{
		key = itorFile.key();
		filePath = itorFile.value();

		QFile file(filePath);
		QHttpPart filePart;
		filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + key + "\"; filename=\"" + file.fileName() + "\""));

		file.open(QIODevice::ReadOnly);
		filePart.setBody(file.readAll());
		//filePart.setBodyDevice(file);
		//file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

		file.close();

		multiPart->append(filePart);
		itorFile++;
	}

	QNetworkRequest req;
	req.setUrl(url);

	//req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // MIME类型，固定为application/json。
	req.setRawHeader("Cache-Control", "no-store"); // Cache-Control	是	缓存控制，固定为no-store，不缓存。

	QNetworkReply* reply = m_nam.post(req, multiPart);

	connect(reply, SIGNAL(uploadProgress(qint64, qint64)), SLOT(onUploadProgress(qint64, qint64)));
	connect(reply, SIGNAL(finished()), SLOT(onFinished()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));

	return reply;
}


void QiniuHttpChannel::onError(QNetworkReply::NetworkError errorCode)
{
	// network layer error error >= QNetworkReply::ConnectionRefusedError && error <= QNetworkReply::UnknownNetworkError && 
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	qDebug() << QDateTime::currentDateTime().toString("hh:mm:dd-ss.zzz") << "onError code=" << errorCode << "msg=" << reply->errorString();

	emit error(reply->errorString());
	

}

void QiniuHttpChannel::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
	QNetworkReply *reply = NULL;

	if (reply = qobject_cast<QNetworkReply *>(sender()))
	{
		emit uploadProgress(bytesSent, bytesTotal);
	}

	qDebug() << QDateTime::currentDateTime().toString("hh:mm:dd-ss.zzz") << "onUploadProgress bytesSent=" << bytesSent << "bytesTotal=" << bytesTotal;
}

void QiniuHttpChannel::onFinished()
{
	QByteArray content;
	QNetworkReply *reply = NULL;

	if (reply = qobject_cast<QNetworkReply *>(sender()))
	{

		int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

		content = reply->readAll();

		emit finished(statusCode);

		qDebug() << QDateTime::currentDateTime().toString("hh:mm:dd-ss.zzz") << "onFinished statusCode=" << statusCode << "data=" << content;
	}


}
