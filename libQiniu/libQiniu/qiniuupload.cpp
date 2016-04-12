#include "qiniuupload.h"

#include "putpolicy.h"

#include "libqiniu.h"

#include "qiniuhttpchannel.h"

QiniuUpload* QiniuUpload::instance = NULL;
int QiniuUpload::m_uploadSeq = 0;

HttpChannel* QiniuUpload::newTask(QString bucket, QString key, QString AK, QString SK)
{
	QString scope = QString("%1:%2").arg(bucket).arg(key);
	QString token = PutPolicy::token(AK, SK, scope);
	m_uploadSeq++;

	QiniuHttpChannel* channel = new QiniuHttpChannel;

	channel->init(m_uploadSeq, bucket, key, AK, SK, token);
	return channel;
}