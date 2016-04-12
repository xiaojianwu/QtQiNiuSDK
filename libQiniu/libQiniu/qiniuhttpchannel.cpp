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
	//��Դ����������UTF - 8���롣
	//	ע�⣺ ����������ϴ�ƾ֤���ϴ������� scopeָ��Ϊ��<bucket> : <key>, ����ֶ�Ҳ����ָ����

	formData["key"] = m_key;

	// ������һ��������Ӧ�����ϴ�ƾ֤������᷵��401��ʾȨ����֤ʧ�ܡ�
	formData["token"] = m_token;

	QHash<QString, QString> files;
	// �ļ�����
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


	// �ļ��ϴ�
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

	//req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json"); // MIME���ͣ��̶�Ϊapplication/json��
	req.setRawHeader("Cache-Control", "no-store"); // Cache-Control	��	������ƣ��̶�Ϊno-store�������档

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
