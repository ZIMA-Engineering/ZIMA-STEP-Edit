#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QApplication>
#include <QCursor>
#include <QFileDialog>
#include <QDir>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->progressBar->hide();
	ui->browseButton->setFocus(Qt::OtherFocusReason);

	QStringList args = QApplication::arguments();

	//connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(findFiles()));
	connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(ui->pathLineEdit, SIGNAL(editingFinished()), this, SLOT(findFiles()));
	connect(ui->subdirsCheckBox, SIGNAL(toggled(bool)), this, SLOT(findFiles()));
	connect(ui->selectAllButton, SIGNAL(clicked()), this, SLOT(selectAll()));
	connect(ui->selectNoneButton, SIGNAL(clicked()), this, SLOT(selectNone()));
	connect(ui->editButton, SIGNAL(clicked()), this, SLOT(edit()));

	connect(&fileFinder, SIGNAL(fileFound(QString)), this, SLOT(addFileToList(QString)));
	connect(&fileFinder, SIGNAL(finished()), this, SLOT(searchFinished()));

	connect(&fileEdit, SIGNAL(fileEdited(QString)), this, SLOT(updateProgress()));
	connect(&fileEdit, SIGNAL(finished()), this, SLOT(finished()));

	connect(ui->fileListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(checkItems()));

	connect(ui->optionsButton, SIGNAL(clicked()), &settingsDlg, SLOT(exec()));
	connect(ui->aboutButton, SIGNAL(clicked()), this, SLOT(about()));

	if(args.count() > 1)
	{
		ui->pathLineEdit->setText(args[1]);
		findFiles();
	} else
		ui->pathLineEdit->setText(QDir::homePath());
}

MainWindow::~MainWindow()
{
	abortAction();

	delete ui;
}

void MainWindow::browse()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Select directory"), ui->pathLineEdit->text());

	if(!path.isEmpty())
	{
		ui->pathLineEdit->setText(path);
		findFiles();
	}
}

void MainWindow::findFiles()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	ui->browseButton->setEnabled(false);
	ui->editButton->setEnabled(false);

	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(0);
	ui->progressBar->setFormat(tr("Searching files..."));
	ui->progressBar->show();

	ui->fileListWidget->clear();

	fileFinder.setStartPath(ui->pathLineEdit->text(), ui->subdirsCheckBox->isChecked());
	fileFinder.start();
}

void MainWindow::addFileToList(QString path)
{
	QListWidgetItem* it = new QListWidgetItem(path);
	it->setCheckState(Qt::Checked);
	ui->fileListWidget->addItem(it);
}

void MainWindow::selectAll()
{
	int cnt = ui->fileListWidget->count();

	for(int i = 0; i < cnt; i++)
		ui->fileListWidget->item(i)->setCheckState(Qt::Checked);
}

void MainWindow::selectNone()
{
	int cnt = ui->fileListWidget->count();

	for(int i = 0; i < cnt; i++)
		ui->fileListWidget->item(i)->setCheckState(Qt::Unchecked);
}

void MainWindow::edit()
{
	filesToEdit.clear();

	int cnt = ui->fileListWidget->count();

	for(int i = 0; i < cnt; i++)
	{
		QListWidgetItem* it = ui->fileListWidget->item(i);

		if(it->checkState() == Qt::Checked)
			filesToEdit << it->text();
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	ui->browseButton->setEnabled(false);
	ui->editButton->setEnabled(false);

	ui->progressBar->setMaximum(cnt-1);
	ui->progressBar->setFormat("%v/%m (%p %)");
	ui->progressBar->show();

	fileEdit.setFileList(&filesToEdit);
	fileEdit.setValues(ui->nameLineEdit->text(), ui->dateLineEdit->text(), ui->authorLineEdit->text(),
			   ui->organizationLineEdit->text(), ui->preprocessorLineEdit->text(), ui->systemLineEdit->text(),
			   ui->authorizationLineEdit->text());
	fileEdit.start();
}

void MainWindow::updateProgress()
{
	ui->progressBar->setValue( ui->progressBar->value()+1 );
}

void MainWindow::finished()
{
	ui->progressBar->hide();
	ui->statusText->setText(tr("Done"));

	ui->browseButton->setEnabled(true);
	ui->editButton->setEnabled(true);

	QApplication::restoreOverrideCursor();
}

void MainWindow::searchFinished()
{
	finished();

	ui->statusText->setText(tr("Found %1 files.").arg(ui->fileListWidget->count()));

	if(ui->fileListWidget->count() > 0)
	{
		loadValues(ui->fileListWidget->item(0)->text());
	}

	if(ui->fileListWidget->count() > 1)
	{
		ui->nameLineEdit->setEnabled(false);
		ui->nameLineEdit->setText("");
	}
}

void MainWindow::loadValues(QString path)
{
	QStringList values = fileEdit.readValues(path);

	if(!values.isEmpty())
	{
		ui->nameLineEdit->setText(values[0]);
		ui->dateLineEdit->setText(values[1]);
		ui->authorLineEdit->setText(values[2]);
		ui->organizationLineEdit->setText(values[3]);
		ui->preprocessorLineEdit->setText(values[4]);
		ui->systemLineEdit->setText(values[5]);
		ui->authorizationLineEdit->setText(values[6]);
	}
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	switch( event->key() )
	{
	case Qt::Key_Escape:
		abortAction();
		ui->statusText->setText(tr("Aborted."));
		break;
	default:break;
	}
}

void MainWindow::about()
{
	aboutDlg.exec();
}

void MainWindow::checkItems()
{
	int cnt = ui->fileListWidget->count();
	int checked = 0;
	QListWidgetItem* checkedIt;

	for(int i = 0; i < cnt; i++)
	{
		QListWidgetItem* it = ui->fileListWidget->item(i);

		if(it->checkState() == Qt::Checked)
		{
			checkedIt = it;
			if(++checked > 1)
			{
				ui->nameLineEdit->setEnabled(false);
				ui->nameLineEdit->setText("");
				return;
			}
		}
	}

	if(checked == 1)
	{
		ui->nameLineEdit->setEnabled(true);
		loadValues(checkedIt->text());
	}
}

void MainWindow::abortAction()
{
	if(fileFinder.isRunning())
		fileFinder.terminate();
	else if(fileEdit.isRunning())
		fileEdit.terminate();

	finished();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	disconnect(ui->pathLineEdit, SIGNAL(editingFinished()), this, SLOT(findFiles()));
	QMainWindow::closeEvent(event);
}
