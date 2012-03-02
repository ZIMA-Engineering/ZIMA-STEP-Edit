#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	settings = new QSettings("ZIMA-Construction", "ZIMA-STEP-Edit", this);

#ifdef Q_OS_WIN32
	contextMenuSettings = new QSettings("HKEY_CLASSES_ROOT\\Directory\\shell", QSettings::NativeFormat, this);
	connect(ui->enableSystemContextMenuCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableSystemContextMenuChanged(bool)));

	bool enabled = settings->value("EnableDirectoryContextMenu", true).toBool();
	ui->enableSystemContextMenuCheckBox->setChecked(enabled);

	if( enabled )
		enableSystemContextMenuChanged(true);
#else
	ui->enableSystemContextMenuCheckBox->setEnabled(false);
#endif
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

#ifdef Q_OS_WIN32
void SettingsDialog::enableSystemContextMenuChanged(bool checked)
{
	if( checked )
	{
		contextMenuSettings->setValue("ZIMA-STEP-Edit/.", tr("Open with ZIMA-STEP-Edit"));
		contextMenuSettings->setValue("ZIMA-STEP-Edit/command/.", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\")) + " %1");
	} else {
		contextMenuSettings->remove("ZIMA-STEP-Edit");
	}
}
#endif
