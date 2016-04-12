#ifndef QINIUUPLOAD_H
#define QINIUUPLOAD_H

#include <QObject>

class HttpChannel;

class QiniuUpload : public QObject
{
	Q_OBJECT
public:
	~QiniuUpload() { instance = nullptr; }
	
	static QiniuUpload* Get()
	{
		if (instance == nullptr)
		{
			m_uploadSeq = 0;
			instance = new QiniuUpload;
		}
			
		return instance;
	}

	HttpChannel* newTask(QString bucket, QString key, QString AK, QString SK);


private:
	static QiniuUpload* instance;


private:
	static int					m_uploadSeq;
	
};

#endif // QINIUUPLOAD_H
