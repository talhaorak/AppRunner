/*
   Settings.cpp

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
#include "Settings.h"
#include <QSettings>
#include <QApplication>

CSettings::CSettings()
{    
}

void CSettings::Load()
{
    QVariant installedValue = QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Classes\\*\\shell\\AppRunner", QSettings::NativeFormat).value("Icon");
    isShellExtensionInstalled = !installedValue.isNull();

    QSettings settings;
    int ppsCount = settings.beginReadArray("predefinedPaths");
    predefinedPaths.clear();
    for(int i=0; i<ppsCount; ++i)
    {
        settings.setArrayIndex(i);
        predefinedPaths.insert(settings.value("name").toString(), settings.value("path").toString());
    }
    settings.endArray();
}

void CSettings::Save()
{
    if(isShellExtensionInstalled)
    {
        QString appPath = qApp->applicationFilePath().replace('/','\\');
        QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Classes\\*\\shell\\AppRunner", QSettings::NativeFormat);
        settings.setValue("Default","Create a custom launcher");
        settings.setValue("Icon", appPath);
        settings.setValue("/command/Default",QString("\"%1\" \"%2\"").arg(appPath).arg("%1"));
    }
    else
    {
        QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Classes\\*\\shell", QSettings::NativeFormat);
        settings.remove("AppRunner");
    }

    QSettings settings;
    settings.beginWriteArray("predefinedPaths", predefinedPaths.count());
    for(int i=0; i<predefinedPaths.count(); ++i)
    {
        QString predefinedPathName = predefinedPaths.keys().at(i);
        QString predefinedPath = predefinedPaths.value(predefinedPathName);
        settings.setArrayIndex(i);
        settings.setValue("name", predefinedPathName);
        settings.setValue("path", predefinedPath);
    }
    settings.endArray();
}

const TDictionary &CSettings::PredefinedPaths() const
{
    return predefinedPaths;
}

bool CSettings::IsShellExtensionInstalled() const
{
    return isShellExtensionInstalled;
}

void CSettings::SetShellExtensionsInstalled(bool yea)
{
    isShellExtensionInstalled = yea;
}

void CSettings::SetPredefinedPaths(const TDictionary &sPredefinedPaths)
{
    predefinedPaths = sPredefinedPaths;
}
