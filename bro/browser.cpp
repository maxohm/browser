#include "browser.h"
#include "browserwindow.h"
//
//#include <QWebEngineProfile>
//
#include "../libengines/libengines.h"

Browser::Browser()
{
    _log::log("Browser::Browser()");

    // Quit application if the download manager window is the only remaining window
    //m_downloadManagerWidget.setAttribute(Qt::WA_QuitOnClose, false);

    //    QObject::connect(
    //        QWebEngineProfile::defaultProfile(), &QWebEngineProfile::downloadRequested,
    //        &m_downloadManagerWidget, &DownloadManagerWidget::downloadRequested);
    //    QObject::connect(
    //        &m_otrProfile, &QWebEngineProfile::downloadRequested,
    //        &m_downloadManagerWidget, &DownloadManagerWidget::downloadRequested);
}

BrowserWindow *Browser::createWindow(QString cwd)
{
    _log::log("Browser::createWindow()");
    //
    //auto profile = offTheRecord ? &m_otrProfile : QWebEngineProfile::defaultProfile();
    //auto profile = offTheRecord ? &m_otrProfile : QWebEngineProfile::defaultProfile();
    auto mainWindow = new BrowserWindow(this, &m_otrProfile, cwd);
    m_windows.append(mainWindow);
    QObject::connect(mainWindow, &QObject::destroyed, [this, mainWindow]() {
        m_windows.removeOne(mainWindow);
    });
    mainWindow->show();
    //
    return mainWindow;
}

//BrowserWindow *Browser::createDevToolsWindow()
//{
//    auto profile = QWebEngineProfile::defaultProfile();
//    auto mainWindow = new BrowserWindow(this, profile, true);
//    m_windows.append(mainWindow);
//    QObject::connect(mainWindow, &QObject::destroyed, [this, mainWindow]() {
//        m_windows.removeOne(mainWindow);
//    });
//    mainWindow->show();
//    return mainWindow;
//}
