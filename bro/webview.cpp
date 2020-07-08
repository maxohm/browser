#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webpopupwindow.h"
#include "webview.h"
//
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QAction>
//
#include "../libengines/libengines.h"

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
    , m_loadProgress(100)
    , reduc(nullptr)
    //, m_speech(0)
{

    //*****************************************
    // ***** speech engine 20191118 ***********
    //*****************************************
#ifdef Q_OS_WIN32
    this->m_spengine = QTextToSpeech::availableEngines();
    qDebug() << " sppech engines " + this->m_spengine.join(" :: ");
    qDebug() << " sppech engines " + this->m_spengine.join(" :: ");
    qDebug() << " sppech engines " + this->m_spengine.join(" :: ");
    if (this->m_spengine.count()>0){
        this->m_speech = new QTextToSpeech(
                    this->m_spengine.at(0), this);
        this->m_speech->stop();
        //bool* ok = nullptr;
        //        this->m_speech->setRate(
        //                    cfg2str(this->icfg,"sndRate")
        //                    .toDouble(ok));
        //        this->m_speech->setVolume(
        //                    cfg2str(this->icfg,"sndVol")
        //                    .toDouble(ok));
        //        this->m_speech->setPitch(
        //                    cfg2str(this->icfg,"sndPitch")
        //                    .toDouble(ok));
        //
        _log::log("SPEECH ENGINE :: "
            +this->m_spengine.at(0));
        _log::log(" RATE   :: "
            +QString::number(
                this->m_speech->rate()));
        _log::log(" VOLUME :: "
            +QString::number(
                this->m_speech->volume()));
        _log::log(" PITCH  :: "
            +QString::number(
                this->m_speech->pitch()));
    };
#endif
    //*****************************************
    //*****************************************
    //*****************************************
    //
    connect(this, &QWebEngineView::selectionChanged, [this]() {
        _log::log("WebView::WebView() URL :: "+this->url().toString());
        // *********** sokr algo 20191118 ********
        QString tip = this->selectedText();
        if (tip.length()>2){
            if (nullptr!=this->reduc)
                if (this->reduc->contains(tip))
                    tip = this->reduc->value(tip);
            //
            _log::log("WebView::WebView() SEL :: "+tip);
            this->setToolTip(tip);
            this->setToolTipDuration(15000);
            #ifdef Q_OS_WIN32
            // ***** speech engine 20191118 *****
            if (this->m_spengine.count()>0){
                _log::log("SPEECH ENGINE :: "
                    +this->m_spengine.at(0));
                _log::log(" RATE   :: "
                    +QString::number(
                        this->m_speech->rate()));
                _log::log(" VOLUME :: "
                    +QString::number(
                        this->m_speech->volume()));
                _log::log(" PITCH  :: "
                    +QString::number(
                        this->m_speech->pitch()));

                //
                this->m_speech->stop();
                this->m_speech->say(tip);
            };
            // **********************************
#endif
        };
        //*****************************************
    });
    connect(this, &QWebEngineView::loadStarted, [this]() {
        m_loadProgress = 0;
        emit favIconChanged(favIcon());
    });
    connect(this, &QWebEngineView::loadProgress, [this](int progress) {
        m_loadProgress = progress;
    });
    connect(this, &QWebEngineView::loadFinished, [this](bool success) {
        m_loadProgress = success ? 100 : -1;
        if(forced_title_.isNull()) {
            forced_title_ = title();
            setWindowTitle(forced_title_);
            emit forcedTitleSet(forced_title_);
        }
        emit favIconChanged(favIcon());
    });
    connect(this, &QWebEngineView::iconChanged, [this](const QIcon &) {
        emit favIconChanged(favIcon());
    });

    connect(this, &QWebEngineView::renderProcessTerminated,
            [this](QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode) {
        QString status;
        switch (termStatus) {
        case QWebEnginePage::NormalTerminationStatus:
            status = tr("Процесс завершился нормально");
            break;
        case QWebEnginePage::AbnormalTerminationStatus:
            status = tr("Процесс завершился с ошибкой");
            break;
        case QWebEnginePage::CrashedTerminationStatus:
            status = tr("Процесс завершился аварийно");
            break;
        case QWebEnginePage::KilledTerminationStatus:
            status = tr("Процесс завершен системой");
            break;
        }
        QMessageBox::StandardButton btn = QMessageBox::question(window(), status,
                                                                tr("Процесс завершился с ошибкой. Код ошибки: %1\n"
                                                                   "Перезагрузить страницу ?").arg(statusCode));
        if (btn == QMessageBox::Yes)
            QTimer::singleShot(0, [this] { reload(); });
    });
}

void WebView::setPage(WebPage *page)
{
    createWebActionTrigger(page,QWebEnginePage::Forward);
    createWebActionTrigger(page,QWebEnginePage::Back);
    createWebActionTrigger(page,QWebEnginePage::Reload);
    createWebActionTrigger(page,QWebEnginePage::Stop);
    QWebEngineView::setPage(page);
}

int WebView::loadProgress() const
{
    return m_loadProgress;
}

void WebView::createWebActionTrigger(QWebEnginePage *page, QWebEnginePage::WebAction webAction)
{
    QAction *action = page->action(webAction);
    connect(action, &QAction::changed, [this, action, webAction]{
        emit webActionEnabledChanged(webAction, action->isEnabled());
    });
}

bool WebView::isWebActionEnabled(QWebEnginePage::WebAction webAction) const
{
    return page()->action(webAction)->isEnabled();
}

QIcon WebView::favIcon() const
{
    QIcon favIcon = icon();
    if (!favIcon.isNull())
        return favIcon;

    if (m_loadProgress < 0) {
        static QIcon errorIcon(QStringLiteral(":dialog-error.png"));
        return errorIcon;
    } else if (m_loadProgress < 100) {
        static QIcon loadingIcon(QStringLiteral(":view-refresh.png"));
        return loadingIcon;
    } else {
        static QIcon defaultIcon(QStringLiteral(":text-html.png"));
        return defaultIcon;
    }
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    BrowserWindow *mainWindow = qobject_cast<BrowserWindow*>(window());
    if (!mainWindow)
        return nullptr;

    switch (type) {
    case QWebEnginePage::WebBrowserTab: {
        return mainWindow->tabWidget()->createTab();
    }
    case QWebEnginePage::WebBrowserBackgroundTab: {
        return mainWindow->tabWidget()->createBackgroundTab();
    }
    case QWebEnginePage::WebBrowserWindow: {
        ////        return mainWindow->browser()->createWindow("")->currentTab();
        //        auto webview = new WebView;
        //        webview->setAttribute(Qt::WA_DeleteOnClose);
        ////        webview->setWindowFlag(Qt::WindowStaysOnTopHint, true);   //Поверх всех окон
        //        webview->setContextMenuPolicy(Qt::NoContextMenu);   //Без контекстного меню.
        //        webview->show();
        //        return webview;
        auto new_window = new WebPopupWindow(page()->profile());
        new_window->show();
        return new_window->view();
    }
    case QWebEnginePage::WebDialog: {
        WebPopupWindow *popup = new WebPopupWindow(page()->profile());
        connect(popup->view(), &WebView::devToolsRequested, this, &WebView::devToolsRequested);
        return popup->view();
    }
    }
    return nullptr;
}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = page()->createStandardContextMenu();
    //    menu->popup(event->globalPos());
    //Можно фильтровать так:
    /*
    auto new_menu = new QMenu(this);
    for(auto menu_action: menu->actions()) {
        if(menu_action == page()->action(QWebEnginePage::InspectElement)
        || menu_action == page()->action(QWebEnginePage::ViewSource))
            continue;
        new_menu->addAction(menu_action);
    }
    new_menu->popup(event->globalPos());
    */
    //Можно фильтровать так:
    //    const QList<QAction*> blacklist = {   //static наверное не стоит. Вдруг оно в какой-то момент меняется.Внутри mutable массив actions[];
    //        page()->action(QWebEnginePage::SavePage),
    //        page()->action(QWebEnginePage::ViewSource),
    //        page()->action(QWebEnginePage::CopyLinkToClipboard),
    //        page()->action(QWebEnginePage::DownloadLinkToDisk),
    //        page()->action(QWebEnginePage::CopyImageToClipboard),
    //        page()->action(QWebEnginePage::CopyImageUrlToClipboard),
    //        page()->action(QWebEnginePage::DownloadImageToDisk)
    //    };
    //    for(auto action:blacklist)
    //        menu->removeAction(action);
    //Или так:

    menu->removeAction(page()->action(QWebEnginePage::SavePage));
    menu->removeAction(page()->action(QWebEnginePage::ViewSource));
    menu->removeAction(page()->action(QWebEnginePage::CopyLinkToClipboard));
    menu->removeAction(page()->action(QWebEnginePage::DownloadLinkToDisk));
    menu->removeAction(page()->action(QWebEnginePage::CopyImageToClipboard));
    menu->removeAction(page()->action(QWebEnginePage::CopyImageUrlToClipboard));
    menu->removeAction(page()->action(QWebEnginePage::DownloadImageToDisk));
    menu->popup(event->globalPos());

    //Можно формировать меню самому, но непонятно, как узнать что у нас за меню. у QContextMenuEvent нет информации по чему кликнули

    //    auto context_menu = new QMenu;
    //    context_menu->addAction(page()->action(QWebEnginePage::Back));
    //    context_menu->addAction(page()->action(QWebEnginePage::Forward));
    //    context_menu->addAction(page()->action(QWebEnginePage::Reload));
    //#ifdef QT_DEBUG
    //    context_menu->addSeparator();
    //    context_menu->addAction(page()->action(QWebEnginePage::InspectElement));
    //    context_menu->addAction(page()->action(QWebEnginePage::ViewSource));
    //#endif

    //    context_menu->popup(event->globalPos());
}

/*
// Попытка реализации зума по Ctrl+колесо (см. http://192.168.1.18/redmine/issues/804). Неудачная, т.к. кроме зума делается ещё и прокрутка.
void WebView::wheelEvent(QWheelEvent* event)
{
    if(event->modifiers().testFlag(Qt::ControlModifier)) {
        if(event->delta()>0)
            setZoomFactor(zoomFactor()+0.1);
        else if(event->delta()<0)
            setZoomFactor(zoomFactor()-0.1);
        event->accept();
    } else {
        event->ignore();
    }
}
*/
