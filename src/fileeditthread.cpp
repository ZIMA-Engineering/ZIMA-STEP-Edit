#include "fileeditthread.h"

#include <QDebug>
#include <QFile>
#include <QTemporaryFile>
#include <QTextStream>

FileEditThread::FileEditThread(QObject *parent) :
        QThread(parent)
{
}

void FileEditThread::run()
{
	foreach(QString file, *files)
	{
		//qDebug() << "Editing file" << file;

		QFile fIn(file);
		QTemporaryFile fOut;

		fOut.setAutoRemove(false);

		if(!fIn.open(QIODevice::ReadWrite | QIODevice::Text) || !fOut.open())
		{
			qDebug() << "Failed to open" << file;
			continue;
		}

		QTextStream streamIn(&fIn);
		QTextStream streamOut(&fOut);

		QString fileName;
		bool foundStart = false;
		bool edited = false;

		while(!streamIn.atEnd())
		{
			QString line = streamIn.readLine();

			if(!edited && line.startsWith("FILE_NAME"))
			{
				foundStart = true;
				fileName += line;
			} else if(foundStart) {
				fileName += line;

				if(line.endsWith(");"))
				{
					QString newFileName;

					if(name.isEmpty())
					{
						QString oldName;

						fileName.remove(0, 9);
						fileName.trimmed().remove(0, 1);

						oldName = parseSection(fileName, 0);

						qDebug() << "Not changing old name" << oldName;

						newFileName = QString("FILE_NAME('%1',\r\n '%2',\r\n ('%3'),\r\n ('%4'),\r\n '%5',\r\n '%6','');")
								.arg(oldName)
								.arg(date)
								.arg(author)
								.arg(organization)
								.arg(cad)
								.arg(cad);
					} else {
						newFileName = QString("FILE_NAME('%1',\n'%2',\n('%3'),\n('%4'),\n'%5',\n'%6','');")
								.arg(name)
								.arg(date)
								.arg(author)
								.arg(organization)
								.arg(cad)
								.arg(cad);
					}

					streamOut << newFileName << "\r\n";

					edited = true;
					foundStart = false;
				}
			} else {
				streamOut << line << "\r\n";
			}
		}

		fIn.close();

		QString tempFilePath = fOut.fileName();

		fOut.close();

		QFile::remove(file);
		QFile::copy(tempFilePath, file);
		QFile::remove(tempFilePath);

		qDebug() << "Copied" << tempFilePath << "to" << file;

		emit fileEdited(file);
	}
}

QStringList FileEditThread::readValues(QString path)
{
	QFile f(path);

	if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return QStringList();

	QTextStream stream(&f);

	QString fileName;
	QStringList res;
	bool foundStart = false;

	while(!stream.atEnd())
	{
		QString line = stream.readLine();

		if(foundStart)
		{
			fileName += line;

			if(line.endsWith(");"))
			{
				//qDebug() << "Got" << fileName;

				// Remove starting FILE_NAME and bracket
				fileName.remove(0, 9);
				fileName.trimmed().remove(0, 1);

//				QString name = fileName.section(',', 0, 0).trimmed().remove(0, 1);
//				name.chop(1);
//				QString date = fileName.section(',', 1, 1).trimmed().remove(0, 1);
//				date.chop(1);
//				QString author = fileName.section(',', 2, 2).trimmed();
//				author.remove(0, 1+author.indexOf('\''));
//				author.chop(author.count()-author.lastIndexOf('\''));

//				QString organization = fileName.section(',', 2, 2).trimmed();
//				organization.remove(0, 1+organization.indexOf('\''));
//				organization.chop(organization.count()-organization.lastIndexOf('\''));

//				QString cad = fileName.section(',', 4, 4).trimmed().remove(0, 1);
//				cad.chop(1);

//				res << name << date << author << organization << cad;

				res << parseSection(fileName, 0)
				       << parseSection(fileName, 1)
				       << parseSection(fileName, 2, true)
				       << parseSection(fileName, 3, true)
				       << parseSection(fileName, 4);

				//qDebug() << "Found" << res;

				foundStart = false;
			}
		} else if(line.startsWith("FILE_NAME"))
		{
			foundStart = true;
			fileName += line;
		}
	}

	f.close();

	return res;
}

void FileEditThread::setValues(QString name, QString date, QString author, QString organization, QString cad)
{
	this->name = name;
	this->date = date;
	this->author = author;
	this->organization = organization;
	this->cad = cad;
}

void FileEditThread::setFileList(QStringList *files)
{
	this->files = files;
}

QString FileEditThread::parseSection(const QString &from, const int pos, const bool brackets)
{
	QString section = from.section(',', pos, pos).trimmed();

	if(brackets)
	{
		section.remove(0, 1+section.indexOf('('));
		section.chop(section.count()-section.lastIndexOf(')'));
	}

	section.remove(0, 1+section.indexOf('\''));
	section.chop(section.count()-section.lastIndexOf('\''));

	return section;
}
