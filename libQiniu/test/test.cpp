#include "test.h"

#include <QFileDialog>

#include <libqiniu.h>

#pragma comment(lib, "libQiniu.lib")

test::test(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.progressBar->setRange(0, 100);

	connect(ui.pushButton_upload, SIGNAL(clicked()), this, SLOT(onUpload()));

	connect(ui.pushButton_selectFile, SIGNAL(clicked()), this, SLOT(onSelectFile()));
}

test::~test()
{

}


void test::onSelectFile()
{
	QString fileName = QFileDialog::getOpenFileName(this);

	ui.lineEdit_filePath->setText(fileName);

	QFileInfo fi(fileName);
	ui.lineEdit_KEY->setText(fi.fileName());
}

void test::onUpload()
{
	QString bucket = ui.lineEdit_bucket->text();
	QString key = ui.lineEdit_KEY->text();
	QString AK = ui.lineEdit_AK->text();
	QString SK = ui.lineEdit_SK->text();


	HttpChannel* channel = libQiniu::Get()->newTask(bucket, key, AK, SK);

	connect(channel, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(onUploadProgress(qint64, qint64)));
	connect(channel, SIGNAL(error(QString)), this, SLOT(onError(QString)));
	connect(channel, SIGNAL(finished(int)), this, SLOT(onFinished(int)));


	QString filePath = ui.lineEdit_filePath->text();
	bool ret = channel->upload(filePath);
}


void test::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
	if (bytesTotal != 0)
	{
		int pos = (bytesSent * 100 / bytesTotal);
		ui.progressBar->setValue(pos);
	}

}

void test::onError(QString errorStr)
{
	ui.textEdit->append(errorStr);
}

void test::onFinished(int statusCode)
{
	QString msg = QString("code=%1, msg=%2").arg(statusCode).arg(libQiniu::getErrorMsg(statusCode));
	ui.textEdit->append(msg);
}
