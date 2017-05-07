/*
   MainWindow.cpp

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
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Runner.h"
#include "AboutDialog.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include <QFileDialog>

#include <QListWidgetItem>
#include <QTimer>
#include <QMimeData>
#include <QUrl>


MainWindow::MainWindow(CRunner *sRunner) :
    ui(new Ui::MainWindow),
    runner(sRunner)
{
    ui->setupUi(this);    
    LoadSettings();
    if(!runner->RunFilePath().isEmpty())
    {
        runner->Load();
        UpdateGUI();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadSettings()
{
    CSettings settings;
    settings.Load();
    ui->comboBox_PredefinedPaths->clear();
    for(auto ppName: settings.PredefinedPaths().keys())
    {
        ui->comboBox_PredefinedPaths->addItem(ppName);
    }
}

void MainWindow::on_pushButton_BrowseApp_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select app..");
    if(!filePath.isEmpty())
    {
        ui->lineEdit_AppPath->setText(filePath.replace('/','\\'));
        ui->lineEdit_WorkingDir->setText(QFileInfo(filePath).absolutePath().replace('/','\\'));
    }
}

void MainWindow::on_pushButton_BrowseWorkingDir_clicked()
{
    QString workingDir = QFileDialog::getExistingDirectory(this, "Select working dir..");
    if(!workingDir.isEmpty())ui->lineEdit_WorkingDir->setText(workingDir.replace('/','\\'));
}

void MainWindow::on_pushButton_AddArgument_clicked()
{
    QListWidgetItem *newArgumentItem = new QListWidgetItem("New Argument");
    newArgumentItem->setFlags(newArgumentItem->flags() | Qt::ItemIsEditable);
    ui->listWidget_Arguments->addItem(newArgumentItem);
    // ui->listWidget_Arguments->setCurrentItem(newArgumentItem);
    ui->listWidget_Arguments->editItem(newArgumentItem);
}

void MainWindow::on_pushButton_RemoveArgument_clicked()
{
    if(ui->listWidget_Arguments->currentItem())
    {
        ui->listWidget_Arguments->takeItem(ui->listWidget_Arguments->currentRow());
        UpdateCommandLine();
    }
}

void MainWindow::on_label_Load_linkActivated(const QString &link)
{
    Q_UNUSED(link)
    QString runFilePath = QFileDialog::getOpenFileName(this, "Select run file..");
    if(!runFilePath.isEmpty())
    {
        runner->Reset();
        runner->SetRunFilePath(runFilePath);
        runner->Load();
        UpdateGUI();
    }
}

void MainWindow::on_label_Save_linkActivated(const QString &link)
{
    Q_UNUSED(link)
    if(runner->RunFilePath().isEmpty())
    {
        QString runFilePath = QFileDialog::getSaveFileName(this, "Save run file as..");
        if(!runFilePath.isEmpty())
        {
            runner->SetRunFilePath(runFilePath);
        }
    }
    UpdateRunner();
    runner->Save();
}

void MainWindow::UpdateRunner()
{
    CRunner::SProcessInfo pi;
    pi.appPath = ui->lineEdit_AppPath->text();
    for(int i=0; i<ui->listWidget_Arguments->count(); ++i)
        pi.arguments << ui->listWidget_Arguments->item(i)->text();
    pi.workingDirectory = ui->lineEdit_WorkingDir->text();
    for(int i=0; i<ui->listWidget_PathEnvVar->count(); ++i)
        pi.pathAdditions << ui->listWidget_PathEnvVar->item(i)->text();
    runner->SetProcessInfo(pi);
}

void MainWindow::UpdateGUI()
{
    ui->lineEdit_AppPath->setText(runner->ProcessInfo().appPath);
    ui->lineEdit_WorkingDir->setText(runner->ProcessInfo().workingDirectory);
    ui->listWidget_Arguments->clear();
    ui->listWidget_Arguments->addItems(runner->ProcessInfo().arguments);
    ui->listWidget_PathEnvVar->clear();
    ui->listWidget_PathEnvVar->addItems(runner->ProcessInfo().pathAdditions);
    UpdateCommandLine();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    QUrl fileUrl(event->mimeData()->text());
    if(fileUrl.isValid() && fileUrl.isLocalFile())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{    
    QUrl fileUrl(event->mimeData()->text());
    *runner = CRunner::FromFile(fileUrl.toLocalFile());
    UpdateGUI();
    event->acceptProposedAction();
}

void MainWindow::on_lineEdit_AppPath_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    UpdateCommandLine();
}

void MainWindow::on_listWidget_Arguments_itemChanged(QListWidgetItem *item)
{
    Q_UNUSED(item)
    UpdateCommandLine();
}

void MainWindow::UpdateCommandLine()
{
    QString argumentsString;
    for(int i=0; i<ui->listWidget_Arguments->count(); ++i)argumentsString += ui->listWidget_Arguments->item(i)->text() + " ";
    QString cmdLine = QString("%1 %2").arg(ui->lineEdit_AppPath->text()).arg(argumentsString);
    ui->plainTextEdit_CommandLine->setPlainText(cmdLine);
}

void MainWindow::on_label_Settings_linkActivated(const QString &link)
{
    Q_UNUSED(link)
    SettingsDialog dlg;
    int dialogResult = dlg.exec();
    if(dialogResult == QDialog::Accepted)
    {
        LoadSettings();
    }
}

void MainWindow::on_label_About_linkActivated(const QString &link)
{
    Q_UNUSED(link)
    AboutDialog aboutDlg;
    aboutDlg.exec();
}

void MainWindow::on_pushButton_AddToPath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Folder to add into PATH..");
    if(dir.isEmpty())return;
    ui->listWidget_PathEnvVar->addItem(dir.replace('/','\\'));
}

void MainWindow::on_pushButton_RemoveFromPath_clicked()
{
    if(ui->listWidget_PathEnvVar->currentItem())
    {
        ui->listWidget_PathEnvVar->takeItem(ui->listWidget_PathEnvVar->currentRow());
    }
}

void MainWindow::on_pushButton_AddPredefinedPath_clicked()
{
    QString predefinedPathName = ui->comboBox_PredefinedPaths->currentText();
    if(!predefinedPathName.isEmpty())
    {
        CSettings setting;
        setting.Load();
        QString predefinedPath = setting.PredefinedPaths().value(predefinedPathName);
        ui->listWidget_PathEnvVar->addItem(predefinedPath);
    }
}
