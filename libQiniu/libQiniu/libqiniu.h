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


	//200	�ϴ��ɹ���
	//	400	�����ĸ�ʽ���󣬱��Ĺ��첻��ȷ����û���������͡�
	//	401	�ϴ�ƾ֤��Ч��
	//	413	�ϴ����ݳ��ȴ��� fsizeLimit��ָ���ĳ������ơ�
	//	579	�ص�ҵ�������ʧ�ܡ�
	//	599	����˲���ʧ�ܡ�
	//	�����˴����뽫����������Ϣ����������HTTP��Ӧͷ����ͨ���ʼ����͸����ǡ�

	//	614	Ŀ����Դ�Ѵ��ڡ�
	static QString getErrorMsg(int statusCode);

private:
	libQiniu();
	static libQiniu* instance;
};

#endif // LIBQINIU_H
