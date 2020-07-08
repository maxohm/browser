#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QIcon>
#include <QWebEngineView>
//
#ifdef Q_OS_WIN32
#include <QtTextToSpeech/QTextToSpeech>
#endif

class WebPage;

class WebView : public QWebEngineView
{
    Q_OBJECT

public:
    WebView(QWidget *parent = nullptr);
    void setPage(WebPage *page);
    //
    int loadProgress() const;
    bool isWebActionEnabled(QWebEnginePage::WebAction webAction) const;
    QIcon favIcon() const;
    //
    QHash<QString, QString>* reduc;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;
//    void wheelEvent(QWheelEvent* event) override; //!< см. в определении

signals:
    void webActionEnabledChanged(QWebEnginePage::WebAction webAction, bool enabled);
    void favIconChanged(const QIcon &icon);
    void devToolsRequested(QWebEnginePage *source);
    void forcedTitleSet(const QString& title);

private:
#ifdef Q_OS_WIN32
    QTextToSpeech* m_speech;
#endif
    QStringList m_spengine;
    //
    void createWebActionTrigger(QWebEnginePage *page, QWebEnginePage::WebAction);
    int m_loadProgress = 0;
    QString forced_title_;
    //
};

#endif //WEBVIEW_H
