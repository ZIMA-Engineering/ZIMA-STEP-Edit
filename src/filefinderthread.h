#ifndef FILEFINDERTHREAD_H
#define FILEFINDERTHREAD_H

#include <QThread>
#include <QStringList>

class FileFinderThread : public QThread
{
	Q_OBJECT
public:
	explicit FileFinderThread(QObject *parent = nullptr);
	void run() override;
	void setStartPath(QString path, bool subdirs);
	
signals:
	void fileFound(QString path);
public slots:
	
private:
	void find(const QString &path);

	QString path;
	bool browseSubdirs;
};

#endif // FILEFINDERTHREAD_H
