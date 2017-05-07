/*
   Runner.h

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
#ifndef CRUNNER_H
#define CRUNNER_H

#include <QStringList>

// class QProcess;

class CRunner
{    
public:
    struct SProcessInfo
    {
        QString appPath;
        QStringList arguments;
        QStringList pathAdditions;
        QString workingDirectory;
        qint64 pid;
    };

    CRunner();
    CRunner(const QString & sRunFilePath);
    ~CRunner();

    QString RunFilePath()const;
    void SetRunFilePath(const QString & sRunFilePath);
    const SProcessInfo & ProcessInfo()const;
    void SetProcessInfo(const SProcessInfo & sProcessInfo);
    bool Run();
    void Load();
    void Save(bool textMode=false);
    void Reset();
    CRunner& operator=(const CRunner & other);
    static CRunner FromFile(const QString & sFilePath);
private:
    // QProcess *process;
    QString runFilePath;
    SProcessInfo processInfo;
};

#endif // CRUNNER_H
