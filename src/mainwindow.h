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
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void keyPressEvent(QKeyEvent *event) override;
	
public slots:
	void addFileToList(QString path);
	void updateProgress();
	void finished();
	void searchFinished();

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	void loadValues(QString path);

	Ui::MainWindow *ui;
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
	void abortAction();
};

#endif // MAINWINDOW_H
