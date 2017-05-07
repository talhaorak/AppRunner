/*
   SettingsDialog.cpp

Copyright (c) 2016 Talha ORAK<talhaorak at gmail.com>
                   talhaorak.net


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    Load();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_checkBox_Install_clicked()
{
    /*
    QString appPath = qApp->applicationFilePath().replace('/','\\');
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Classes\\*\\shell\\AppRunner", QSettings::NativeFormat);
    settings.setValue("Default","Create a custom launcher");
    settings.setValue("Icon", appPath);
    settings.setValue("/command/Default",QString("\"%1\" \"%2\"").arg(appPath).arg("%1"));
    */
    ui->checkBox_Install->setEnabled(false);
    ui->checkBox_Uninstall->setEnabled(true);
}

void SettingsDialog::on_checkBox_Uninstall_clicked()
{
    /*
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Classes\\*\\shell", QSettings::NativeFormat);
    settings.remove("AppRunner");
    */
    ui->checkBox_Install->setEnabled(true);
    ui->checkBox_Uninstall->setEnabled(false);
}

void SettingsDialog::on_pushButton_AddPath_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory();
    if(dirPath.isEmpty())return;
    AddPath("Path", dirPath.replace('/','\\'));
}

void SettingsDialog::on_pushButton_EditPath_clicked()
{
    if(ui->tableWidget_Paths->currentItem())
    {
        ui->tableWidget_Paths->editItem(ui->tableWidget_Paths->currentItem());
    }
}

void SettingsDialog::on_pushButton_RemovePath_clicked()
{
    if(ui->tableWidget_Paths->currentItem())
    {
        ui->tableWidget_Paths->removeRow(ui->tableWidget_Paths->currentRow());
    }
}

void SettingsDialog::Load()
{
    CSettings settings;
    settings.Load();

    if(settings.IsShellExtensionInstalled())
    {
        ui->checkBox_Install->setEnabled(false);
        ui->checkBox_Uninstall->setEnabled(true);
        ui->checkBox_Uninstall->setCheckState(Qt::Unchecked);
        ui->checkBox_Install->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox_Install->setEnabled(true);
        ui->checkBox_Uninstall->setEnabled(false);
        ui->checkBox_Uninstall->setCheckState(Qt::Checked);
        ui->checkBox_Install->setCheckState(Qt::Unchecked);
    }

//    ui->tableWidget_Paths->clearContents();
    while(ui->tableWidget_Paths->rowCount()>0)ui->tableWidget_Paths->removeRow(0);
    for(auto ppName : settings.PredefinedPaths().keys())
    {
        AddPath(ppName, settings.PredefinedPaths().value(ppName), false);
    }
}

void SettingsDialog::Save()
{
    CSettings settings;
    settings.Load();
    settings.SetShellExtensionsInstalled(ui->checkBox_Install->isChecked());
    TDictionary pps;
    for(int row=0; row<ui->tableWidget_Paths->rowCount(); ++row)
    {
        pps.insert(
               ui->tableWidget_Paths->item(row,0)->text(),
               ui->tableWidget_Paths->item(row,1)->text());
    }
    settings.SetPredefinedPaths(pps);
    settings.Save();

}

void SettingsDialog::AddPath(const QString &name, const QString &path, bool edit)
{
    int row = ui->tableWidget_Paths->rowCount();
    ui->tableWidget_Paths->setRowCount(row + 1);
    ui->tableWidget_Paths->setItem(row, 0, new QTableWidgetItem{name});
    ui->tableWidget_Paths->setItem(row, 1, new QTableWidgetItem{path});
    if(edit)ui->tableWidget_Paths->editItem(ui->tableWidget_Paths->item(row,0));
}

void SettingsDialog::on_buttonBox_accepted()
{
    Save();
}
