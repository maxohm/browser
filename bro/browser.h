#ifndef BROWSER_H
#define BROWSER_H
//
#include <QVector>
#include <QWebEngineProfile>
#include <QDebug>
//
//#include "downloadmanagerwidget.h"

class BrowserWindow;

class Browser
{
public:
    Browser();
    QVector<BrowserWindow*> windows() { return m_windows; }
    BrowserWindow *createWindow(QString cwd);
    //    BrowserWindow *createDevToolsWindow();

    //DownloadManagerWidget &downloadManagerWidget() { return m_downloadManagerWidget; }

private:
    QVector<BrowserWindow*> m_windows;
    //DownloadManagerWidget m_downloadManagerWidget;
    QWebEngineProfile m_otrProfile;
};
#endif // BROWSER_H
