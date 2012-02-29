#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>

#include "filefinderthread.h"
#include "fileeditthread.h"
#include "settingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
public slots:
	void addFileToList(QString path);
	void updateProgress();
	void finished();

private:
	Ui::MainWindow *ui;
	QProgressBar *progressBar;
	FileFinderThread fileFinder;
	FileEditThread fileEdit;
	QStringList filesToEdit;
	SettingsDialog settingsDlg;

private slots:
	void browse();
	void findFiles();
	void selectAll();
	void selectNone();
	void edit();
};

#endif // MAINWINDOW_H
