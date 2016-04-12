#include "libqiniu.h"

#include "qiniuupload.h"

static QHash<int, QString> g_statusCodeMsg;
libQiniu* libQiniu::instance = NULL;

libQiniu::libQiniu()
{
	g_statusCodeMsg[200] = QStringLiteral("上传成功。");
	g_statusCodeMsg[400] = QStringLiteral("请求报文格式错误，报文构造不正确或者没有完整发送。");
	g_statusCodeMsg[401] = QStringLiteral("上传凭证无效。");
	g_statusCodeMsg[413] = QStringLiteral("上传内容长度大于 fsizeLimit中指定的长度限制。");
	g_statusCodeMsg[579] = QStringLiteral("回调业务服务器失败。");
	g_statusCodeMsg[599] = QStringLiteral("服务端操作失败。如遇此错误，请将完整错误信息（包括所有HTTP响应头部）通过邮件发送给我们。");
	g_statusCodeMsg[614] = QStringLiteral("目标资源已存在。");
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
