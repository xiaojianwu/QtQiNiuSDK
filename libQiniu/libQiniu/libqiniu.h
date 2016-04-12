#ifndef LIBQINIU_H
#define LIBQINIU_H

#include <QObject>
#include <QString>

#include <QHash>


class HttpChannel : public QObject
{
	Q_OBJECT
public:
	virtual bool upload(QString filePath) = 0;

signals:
	void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

	void error(QString errorStr);

	void finished(int statusCode);
};

class libQiniu : public QObject
{
	Q_OBJECT
public:
	~libQiniu();

	static libQiniu* Get();


	HttpChannel* newTask(QString bucket, QString key, QString AK, QString SK);


	//200	上传成功。
	//	400	请求报文格式错误，报文构造不正确或者没有完整发送。
	//	401	上传凭证无效。
	//	413	上传内容长度大于 fsizeLimit中指定的长度限制。
	//	579	回调业务服务器失败。
	//	599	服务端操作失败。
	//	如遇此错误，请将完整错误信息（包括所有HTTP响应头部）通过邮件发送给我们。

	//	614	目标资源已存在。
	static QString getErrorMsg(int statusCode);

private:
	libQiniu();
	static libQiniu* instance;
};

#endif // LIBQINIU_H
