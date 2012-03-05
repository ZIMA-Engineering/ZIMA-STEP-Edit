#include "filefinderthread.h"

#include <QDebug>
#include <QStringList>
#include <QDir>

#ifdef Q_OS_WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif


FileFinderThread::FileFinderThread(QObject *parent) :
        QThread(parent)
{
}

void FileFinderThread::run()
{
	find(path);
}

void FileFinderThread::setStartPath(QString path, bool subdirs)
{
	this->path = path;
	browseSubdirs = subdirs;
}

void FileFinderThread::find(const QString &path)
{
	QDir dir(path);
	QStringList filters, files, subdirs;

	filters << "*.stp" << "*.step";

	//qDebug() << "Searching files in directory" << path;

	files = dir.entryList(filters, QDir::Files);

	foreach(QString file, files)
	{
		emit fileFound(path + SEPARATOR + file);
	}

	if(browseSubdirs)
	{
		subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

		foreach(QString dir, subdirs)
		{
			//qDebug() << "Found dir" << path + "/" + dir;
			find(path + SEPARATOR + dir);
		}
	}
}
