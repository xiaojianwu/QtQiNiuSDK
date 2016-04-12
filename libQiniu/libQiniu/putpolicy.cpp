#include "putpolicy.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

#include "auth.h"

PutPolicy::PutPolicy()
{

}

PutPolicy::~PutPolicy()
{

}


QString PutPolicy::token(QString ak, QString sk, QString scope)
{

	QJsonDocument doc;

	QJsonObject obj;

	// 	ָ���ϴ���Ŀ����Դ�ռ�(Bucket) ����Դ��(Key) ��
	// 		�����ָ�ʽ��
	// 		�� <bucket>����ʾ�����û��ϴ��ļ���ָ���� bucket�������ָ�ʽ���ļ�ֻ�ܡ������������Ѵ���ͬ����Դ�ϴ����ʧ�ܡ�
	// 		�� <bucket> : <key>����ʾֻ�����û��ϴ�ָ��key���ļ��������ָ�ʽ���ļ�Ĭ�������޸ġ���
	// ���Ѵ���ͬ����Դ��ᱻ���ǡ����ֻϣ���ϴ�ָ��key���ļ������Ҳ������޸ģ���ô���Խ������ insertOnly ����ֵ��Ϊ1	
	obj["scope"] = scope;

	// �ļ��ڶ������ɾ������ţ���ļ��ϴ�ʱ����ָ����deleteAfterDays������ӣ�
	// �õ���ʱ���뵽��һ�����ҹ(CST, �й���׼ʱ��)���Ӷ��õ��ļ�ɾ����ʼʱ�䡣
	//�����ļ���2015��1��1������10:00 CST�ϴ���ָ��deleteAfterDaysΪ3�죬��ô����2015��1��5��00 : 00 CST֮������ɾ���ļ���
	obj["deleteAfterDays"] = 7;

	// �޶�Ϊ�����������⡣
	// �������Ϊ��0ֵ��������scope����Ϊʲô��ʽ�������ԡ�������ģʽ�ϴ��ļ���
	obj["insertOnly"] = 0;

	// �ϴ�ƾ֤��Ч��ֹʱ�䡣
	// Unixʱ�������λ���롣�ý�ֹʱ��Ϊ�ϴ���ɺ�����ţ�ռ������ļ���У��ʱ�䣬�����ϴ��Ŀ�ʼʱ�䣬
	// һ�㽨������Ϊ���ϴ���ʼʱ�� + 3600s�����û��ɸ��ݾ����ҵ�񳡾���ƾ֤��ֹʱ����е�����
	int deadline = QDateTime::currentDateTimeUtc().toTime_t() + 3600;
	obj["deadline"] = deadline;

	doc.setObject(obj);


	return Auth::genUploadToken(ak, sk, doc.toJson(QJsonDocument::Compact));
}
