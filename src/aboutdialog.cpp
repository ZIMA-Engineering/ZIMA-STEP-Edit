#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "zima-step-edit.h"

#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

	connect(ui->aboutText, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated(QString)));

	ui->aboutText->setText(QString(
			"<html><head><style>"
			"li{list-style-type:none;}"
			"</style></head><body>"
		)
		+ tr(
			"<h1>ZIMA-STEP-Edit</h1>"
			"<p class=\"version\">%1</p>"
			"<p>ZIMA-STEP-Edit was created by <a href=\"http://www.zima-construction.cz/\">ZIMA-Construction</a> "
			"and is released under the <a href=\"http://www.gnu.org/\">GNU/GPLv3</a> license."
			"</p>"
			"<h2>Authors:</h2>"
	    ).arg(VERSION)
	    + QString(
			"<ul><li>Developed by Jakub Skokan &lt;<a href=\"mailto:aither@havefun.cz\">aither@havefun.cz</a>&gt;</li>"
			"<li>Icon created by Aleš Kocur &lt;<a href=\"mailto:kafe@havefun.cz\">kafe@havefun.cz</a>&gt;</li>"
	    ) + QString("</body></html")
	);

	ui->aboutText->setWordWrap(true);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::linkActivated(QString url)
{
	QDesktopServices::openUrl( QUrl(url) );
}
