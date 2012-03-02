#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QApplication>
#include <QCursor>
#include <QFileDialog>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QStringList args = QApplication::arguments();

	if(args.count() > 1)
		ui->pathLineEdit->setText(args[1]);
	else
		ui->pathLineEdit->setText(QDir::homePath());

	progressBar = new QProgressBar(this);
	progressBar->hide();

	ui->statusBar->addWidget(progressBar, 100);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(findFiles()));
	connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browse()));
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
}

MainWindow::~MainWindow()
{
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

	ui->searchButton->setEnabled(false);
	ui->browseButton->setEnabled(false);
	ui->editButton->setEnabled(false);

	progressBar->setMinimum(0);
	progressBar->setMaximum(0);
	progressBar->setFormat(tr("Searching files..."));
	progressBar->show();

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

	ui->searchButton->setEnabled(false);
	ui->browseButton->setEnabled(false);
	ui->editButton->setEnabled(false);

	progressBar->setMaximum(cnt-1);
	progressBar->setFormat("%v/%m (%p %)");
	progressBar->show();

	fileEdit.setFileList(&filesToEdit);
	fileEdit.setValues(ui->nameLineEdit->text(), ui->dateLineEdit->text(), ui->authorLineEdit->text(), ui->organizationLineEdit->text(), ui->cadLineEdit->text());
	fileEdit.start();
}

void MainWindow::updateProgress()
{
	progressBar->setValue( progressBar->value()+1 );
}

void MainWindow::finished()
{
	progressBar->hide();
	ui->statusBar->showMessage(tr("Done"));

	ui->searchButton->setEnabled(true);
	ui->browseButton->setEnabled(true);
	ui->editButton->setEnabled(true);

	QApplication::restoreOverrideCursor();
}

void MainWindow::searchFinished()
{
	finished();

	ui->statusBar->showMessage(tr("Found %1 files.").arg(ui->fileListWidget->count()));

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
		ui->cadLineEdit->setText(values[4]);
	}
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	switch( event->key() )
	{
	case Qt::Key_Escape:
		if(fileFinder.isRunning())
			fileFinder.terminate();
		else if(fileEdit.isRunning())
			fileEdit.terminate();

		finished();
		ui->statusBar->showMessage(tr("Aborted."));
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
