#ifndef TEST_H
#define TEST_H

#include <QtWidgets/QMainWindow>
#include "ui_test.h"

class test : public QMainWindow
{
	Q_OBJECT

public:
	test(QWidget *parent = 0);
	~test();


private slots:
	void onUpload();
	void onSelectFile();


	void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);

	void onError(QString errorStr);

	void onFinished(int statusCode);

private:
	Ui::testClass ui;
};

#endif // TEST_H
