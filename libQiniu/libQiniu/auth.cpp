#include "auth.h"

#include <QCryptographicHash>


#include <QDebug>

Auth::Auth()
{

}

Auth::~Auth()
{

}

QString Auth::genUploadToken(QString accessKey, QString secretKey, QString putPolicy)
{
	//SETP 2
	qDebug() << "put_policy = " << putPolicy;

	//SETP 3
	QByteArray encodedPutPolicy = putPolicy.toUtf8().toBase64();
	qDebug() <<  "encoded = " <<  encodedPutPolicy;

	//SETP 4
	QByteArray encodedSign = hmacSha1(secretKey.toUtf8(), encodedPutPolicy);
	encodedSign.replace('+', '-').replace('/', '_');
	qDebug() << "encoded_signed=" << encodedSign;

	//SETP 5
	QString upload_token = QString("%1:%2:%3").arg(accessKey).arg(QString::fromUtf8(encodedSign)).arg(QString::fromUtf8(encodedPutPolicy));
	qDebug() << "upload_token=" << upload_token;

	return upload_token;
}


QByteArray Auth::hmacSha1(QByteArray key, QByteArray baseString)
{
	int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
	if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
		key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
	}
	QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
	QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "/"
													// ascii characters 0x36 ("6") and 0x5c ("/") are selected because they have large
													// Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)
	for (int i = 0; i < key.length(); i++) {
		innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
		outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
	}
	// result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
	QByteArray total = outerPadding;
	QByteArray part = innerPadding;
	part.append(baseString);
	total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
	QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
	return hashed.toBase64();
}
