#ifndef FILEEDITTHREAD_H
#define FILEEDITTHREAD_H

#include <QThread>
#include <QStringList>

class FileEditThread : public QThread
{
	Q_OBJECT
public:
	explicit FileEditThread(QObject *parent = 0);
	void run();
	void setValues(QString name, QString date, QString author, QString organization, QString preprocessor, QString system, QString authorization);
	void setFileList(QStringList *files);
	QStringList readValues(QString path);
	
signals:
	void fileEdited(QString path);
public slots:
	
private:
	QString parseSection(const QString &from, const int pos, const bool brackets = false);

	QString name;
	QString date;
	QString author;
	QString organization;
	QString preprocessor;
	QString system;
	QString authorization;
	QStringList *files;
};

#endif // FILEEDITTHREAD_H
