#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QKeyEvent>

#include "filefinderthread.h"
#include "fileeditthread.h"
#include "settingsdialog.h"
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void keyPressEvent(QKeyEvent *event);
	
public slots:
	void addFileToList(QString path);
	void updateProgress();
	void finished();
	void searchFinished();

private:
	void loadValues(QString path);

	Ui::MainWindow *ui;
	QProgressBar *progressBar;
	FileFinderThread fileFinder;
	FileEditThread fileEdit;
	QStringList filesToEdit;
	SettingsDialog settingsDlg;
	AboutDialog aboutDlg;

private slots:
	void browse();
	void findFiles();
	void selectAll();
	void selectNone();
	void edit();
	void about();
	void checkItems();
};

#endif // MAINWINDOW_H
