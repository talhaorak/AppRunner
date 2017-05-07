/*
   MainWindow.h

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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class CRunner;
class QListWidgetItem;
class QDragEnterEvent;
class QDropEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(CRunner *sRunner);
    ~MainWindow();

    void LoadSettings();

private slots:
    void on_pushButton_BrowseApp_clicked();

    void on_pushButton_BrowseWorkingDir_clicked();

    void on_pushButton_AddArgument_clicked();

    void on_pushButton_RemoveArgument_clicked();

    void on_label_Load_linkActivated(const QString &link);

    void on_label_Save_linkActivated(const QString &link);

    void on_lineEdit_AppPath_textChanged(const QString &arg1);

    void on_listWidget_Arguments_itemChanged(QListWidgetItem *item);

    void UpdateCommandLine();
    void on_label_Settings_linkActivated(const QString &link);

    void on_label_About_linkActivated(const QString &link);

    void on_pushButton_AddToPath_clicked();

    void on_pushButton_RemoveFromPath_clicked();

    void on_pushButton_AddPredefinedPath_clicked();

private:
    Ui::MainWindow *ui;
    CRunner *runner;
    void UpdateRunner();
    void UpdateGUI();

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MAINWINDOW_H
