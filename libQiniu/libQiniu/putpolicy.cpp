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

	// 	指定上传的目标资源空间(Bucket) 和资源键(Key) 。
	// 		有两种格式：
	// 		● <bucket>，表示允许用户上传文件到指定的 bucket。在这种格式下文件只能“新增”，若已存在同名资源上传则会失败。
	// 		● <bucket> : <key>，表示只允许用户上传指定key的文件。在这种格式下文件默认允许“修改”，
	// 若已存在同名资源则会被覆盖。如果只希望上传指定key的文件，并且不允许修改，那么可以将下面的 insertOnly 属性值设为1	
	obj["scope"] = scope;

	// 文件在多少天后被删除，七牛将文件上传时间与指定的deleteAfterDays天数相加，
	// 得到的时间入到后一天的午夜(CST, 中国标准时间)，从而得到文件删除开始时间。
	//例如文件在2015年1月1日上午10:00 CST上传，指定deleteAfterDays为3天，那么会在2015年1月5日00 : 00 CST之后当天内删除文件。
	obj["deleteAfterDays"] = 7;

	// 限定为“新增”语意。
	// 如果设置为非0值，则无论scope设置为什么形式，仅能以“新增”模式上传文件。
	obj["insertOnly"] = 0;

	// 上传凭证有效截止时间。
	// Unix时间戳，单位：秒。该截止时间为上传完成后，在七牛空间生成文件的校验时间，而非上传的开始时间，
	// 一般建议设置为“上传开始时间 + 3600s”，用户可根据具体的业务场景对凭证截止时间进行调整。
	int deadline = QDateTime::currentDateTimeUtc().toTime_t() + 3600;
	obj["deadline"] = deadline;

	doc.setObject(obj);


	return Auth::genUploadToken(ak, sk, doc.toJson(QJsonDocument::Compact));
}
