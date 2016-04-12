#include "libqiniu.h"

#include "qiniuupload.h"

static QHash<int, QString> g_statusCodeMsg;
libQiniu* libQiniu::instance = NULL;

libQiniu::libQiniu()
{
	g_statusCodeMsg[200] = QStringLiteral("�ϴ��ɹ���");
	g_statusCodeMsg[400] = QStringLiteral("�����ĸ�ʽ���󣬱��Ĺ��첻��ȷ����û���������͡�");
	g_statusCodeMsg[401] = QStringLiteral("�ϴ�ƾ֤��Ч��");
	g_statusCodeMsg[413] = QStringLiteral("�ϴ����ݳ��ȴ��� fsizeLimit��ָ���ĳ������ơ�");
	g_statusCodeMsg[579] = QStringLiteral("�ص�ҵ�������ʧ�ܡ�");
	g_statusCodeMsg[599] = QStringLiteral("����˲���ʧ�ܡ������˴����뽫����������Ϣ����������HTTP��Ӧͷ����ͨ���ʼ����͸����ǡ�");
	g_statusCodeMsg[614] = QStringLiteral("Ŀ����Դ�Ѵ��ڡ�");
}

libQiniu::~libQiniu()
{
	instance = nullptr;
}


libQiniu* libQiniu::Get()
{
	if (instance == nullptr)
	{
		instance = new libQiniu;
	}

	return instance;
}

HttpChannel* libQiniu::newTask(QString bucket, QString key, QString AK, QString SK)
{
	return QiniuUpload::Get()->newTask(bucket, key, AK, SK);
}


QString libQiniu::getErrorMsg(int statusCode)
{
	return g_statusCodeMsg[statusCode];
}
