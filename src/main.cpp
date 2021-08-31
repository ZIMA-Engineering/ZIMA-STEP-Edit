#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("ZIMA-Construction");
	QCoreApplication::setOrganizationDomain("zima-contruction.cz");
	QCoreApplication::setApplicationName("ZIMA-STEP-Edit");

	QApplication a(argc, argv);

	QTranslator translator;

	QString filename = "ZIMA-STEP-Edit_" + QLocale::system().name();
	QStringList paths;

	paths
			<< filename
			<< QApplication::applicationDirPath() + "/" + filename
			<< QApplication::applicationDirPath() + "/locale/" + filename
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
