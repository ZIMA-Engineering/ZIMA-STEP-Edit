#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("ZIMA-Construction");
	QCoreApplication::setOrganizationDomain("zima-contruction.cz");
	QCoreApplication::setApplicationName("ZIMA-Parts");

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

	QApplication a(argc, argv);

	QTranslator translator;

	QString filename = "zima-step-edit_" + QLocale::system().name();
	QStringList paths;

	paths
			<< filename
			<< QApplication::applicationDirPath() + "/" + filename
			<< ("locale/" + filename)
			<< (":/" + filename);

	foreach(QString path, paths)
		if( translator.load(path) )
		{
			a.installTranslator(&translator);
			break;
		}

	MainWindow w;
	w.show();
	
	return a.exec();
}
