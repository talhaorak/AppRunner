/*
   main.cpp

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
#include "Runner.h"
#include "Version.h"
#include <QApplication>
#include <QFileInfo>
#include <QVersionNumber>
#include <QMessageBox>
#include <iostream>

/*
  AppRunner (noparam)         : Show GUI
  AppRunner *.run             : Launch the app
  AppRunner *.run -e          : Edit the launch details
  AppRunner *.*               : Create a runner for given (executable) file or command

*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("AppRunner - Launch applications with custom parameters");
    a.setApplicationName("AppRunner");
    QVersionNumber version = {Version::MAJOR, Version::MINOR, Version::BUILD };
    a.setApplicationVersion(version.toString());
    a.setOrganizationName("Tachyon Software");
    a.setOrganizationDomain("takyon.com.tr");

    CRunner runner;        
    bool showGUI = false;
    if(argc == 1)showGUI = true;
    else if(argc == 2)
    {
        QFileInfo fi(argv[1]);
        QString fileExtension = fi.completeSuffix();
        if(fileExtension == "run")
        {
            runner.SetRunFilePath(fi.absoluteFilePath());
        }
        else
        {
            runner = CRunner::FromFile(fi.absoluteFilePath());
            runner.Save();
            showGUI = true;
        }
    }    
    else if(argc == 3)
    {
        showGUI = (QString(argv[2]) == "-e");
    }

    if(!showGUI)
    {
        runner.Load();
        runner.Run();
        //QMessageBox::information(nullptr, "title", "text");
        std::cout << "running";
        return 0;
    }
    else
    {
        MainWindow w(&runner);
        w.show();
        return a.exec();
    }
}
