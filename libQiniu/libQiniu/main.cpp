#include "libqiniu.h"

#include <QCoreApplication>


#include <QJsonDocument>


#include <QJsonObject>

#include <QUuid>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	//QJsonDocument doc;
	//

	//QJsonObject obj;

	//obj["scope"] = "my-bucket:sunflower.jpg";
	//obj["deadline"] = 1451491200; // QString::number(QDateTime::currentDateTimeUtc().toTime_t());


	//QJsonObject obj2;
	//obj2["name"] = "$(fname)";
	//obj2["size"] = "$(fsize)";
	//obj2["w"] = "$(imageInfo.width)";
	//obj2["h"] = "$(imageInfo.height)";
	//obj2["hash"] = "$(etag)";


	//obj["returnBody"] = obj2;

	//doc.setObject(obj);

	//QString putPolicy = doc.toJson(QJsonDocument::Compact);
	//QString token = Auth::genUploadToken(accssKey, seckeryKey, putPolicy);


	QString uuid = QUuid::createUuid().toString().remove("{").remove("}");
	QString bucket = "zaozao";
	QString key = QString("%1.jpg").arg(uuid);
	QString AK = "BppWG4AC-4SIaWUHMeHy8UXiJMseBhXz3qqdINvV";
	QString SK = "k-zses_vtZtGNGEUmbec3lOKi-FmiXy5nFTISfLr";


	HttpChannel* channel = libQiniu::Get()->newTask(bucket, key, AK, SK);

	QString filePath = "C:\\Users\\wuxiaojian.FOCUSTECH\\Desktop\\test.jpg";
	bool ret = channel->upload(filePath);

	return app.exec();
}
