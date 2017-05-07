/*
   Runner.cpp

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
#include "Runner.h"
#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QTextStream>
#include <QList>
#include <QThread>
#include <QMessageBox>

CRunner::CRunner()
{

}

CRunner::CRunner(const QString &sRunFilePath)
    :runFilePath(sRunFilePath)
{
}

CRunner::~CRunner()
{
}

QString CRunner::RunFilePath() const
{
    return runFilePath;
}

void CRunner::SetRunFilePath(const QString &sRunFilePath)
{
    runFilePath = sRunFilePath;
}

const CRunner::SProcessInfo &CRunner::ProcessInfo() const
{
    return processInfo;
}

void CRunner::SetProcessInfo(const CRunner::SProcessInfo &sProcessInfo)
{
    processInfo = sProcessInfo;
}

void CRunner::Load()
{
    if(runFilePath.isEmpty())return;
    QFile runFile(runFilePath);
    if(!runFile.open(QIODevice::ReadOnly))return;
    QDataStream stream(&runFile);
    stream >> processInfo.appPath;
    stream >> processInfo.arguments;
    stream >> processInfo.workingDirectory;
    stream >> processInfo.pathAdditions;
    runFile.close();
}

void CRunner::Save(bool textMode)
{
    if(runFilePath.isEmpty())return;
    QFile runFile(runFilePath);
    if(!runFile.open(QIODevice::WriteOnly))return;
    if(textMode)
    {
        QTextStream stream(&runFile);
        // TODO
//        stream << processInfo.appPath;
//        stream << processInfo.arguments;
//        stream << processInfo.workingDirectory;
//        stream << processInfo.pathAdditions;
    }
    else
    {
        QDataStream stream(&runFile);
        stream << processInfo.appPath;
        stream << processInfo.arguments;
        stream << processInfo.workingDirectory;
        stream << processInfo.pathAdditions;
    }
    runFile.close();
}

void CRunner::Reset()
{
    runFilePath = QString();
    processInfo = SProcessInfo();
}

CRunner &CRunner::operator=(const CRunner &other)
{
    runFilePath = other.RunFilePath();
    processInfo = other.ProcessInfo();
    return *this;
}

CRunner CRunner::FromFile(const QString &sFilePath)
{
    QFileInfo fi(sFilePath);
    CRunner::SProcessInfo pi;
    pi.appPath = fi.absoluteFilePath();
    pi.workingDirectory = fi.absolutePath();
    QString runFilePath = QString("%1/%2.run").arg(fi.absolutePath()).arg(fi.baseName());
    CRunner runner(runFilePath);
    runner.SetProcessInfo(pi);
    return runner;
}

bool CRunner::Run()
{    
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString originalPath{env.value("PATH")};
    originalPath.prepend(processInfo.pathAdditions.join(';') + ";");
    originalPath = originalPath.replace('/','\\');
    qputenv("PATH", originalPath.toLatin1());
    // QMessageBox::information(nullptr, "new env:", originalPath);
    return QProcess::startDetached(processInfo.appPath, processInfo.arguments, processInfo.workingDirectory, &processInfo.pid);
    /*
    QProcess *process{new QProcess};
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString originalPath{env.value("PATH")};
    originalPath.prepend(processInfo.pathAdditions.join(';') + ";");
    process->setProcessEnvironment(env);
    process->setArguments(processInfo.arguments);
    process->setProgram(processInfo.appPath);
    process->setWorkingDirectory(processInfo.workingDirectory);
    process->start();
    return true;
*/
    //return QProcess::startDetached(processInfo.appPath, processInfo.arguments, processInfo.workingDirectory, &processInfo.pid);
}

