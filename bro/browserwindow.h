#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H
//
#include <QMainWindow>
#include <QTime>
#include <QLabel>
#include <QWebEnginePage>
#include <QDialog>
#include <QString>
//
#include <QStandardItem>
#include <QStandardItemModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QTreeView>
#include <QSettings>
//#include <QTextToSpeech>
//
#include "treeop.h"
#include "../catdir/ui_clist.h"
// Если тут ^^^ обвалилось - сначала собери CATDir !
//
class SearchWidget;
class QProgressBar;
class Browser;
class TabWidget;
class WebView;
class QSplitter;
//
//namespace Ui {
//    class clist;
//}
//
class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    BrowserWindow(Browser *browser, QWebEngineProfile *profile, QString conf);
    TabWidget *tabWidget() const;
    WebView *currentTab() const;
    Browser *browser() { return m_browser; }
    QDialog about;
    QString wtext;
    //
    QSize sizeHint() const override;

public slots:   
    void treeReload1();
    void treeReload2(const QModelIndex& index);
    void treeReload3(const QModelIndex& index);

protected:
    void closeEvent(QCloseEvent* event) override;
    void mousePressEvent (QMouseEvent* event);

private slots:
    void adjustTree1(const QModelIndex& index);
    void adjustTree2(const QModelIndex& index);
    //
    void favUrlOpen(int r, int c);
    void favList();
    void favListClosed(int r);
    //
    void handleNewWindowTriggered();
    void handleNewIncognitoWindowTriggered();
    //void handleFileOpenTriggered();
    void handleFindActionTriggered();
    void handleShowWindowTriggered();
    void handleWebViewLoadProgress(int);
    void handleWebViewTitleChanged(const QString &title);
    void handleWebActionEnabledChanged(QWebEnginePage::WebAction action, bool enabled);
    void handleDevToolsRequested(QWebEnginePage *source);
    //
    void findText(const QString& text);
    void findPrevious();
    void findNext();
    //
    void tabMenu(const QPoint& pos);
    void javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID);
    //
    // void maximize();

private:
    //
    //QTextToSpeech *m_speech;
    //
    QString lib;
    QString hlp;
    QRect m_geometry;
    QWidget* m_treeWidget = Q_NULLPTR;
    QTreeView* m_treeView;
    QTreeView* m_treeView2;
    //
    QSettings* icfg;
    //
#ifdef UI_CLIST_H
    Ui::clist clist;
#endif
    QString ffile;
    QDialog* flist;
    QString fMenu(const QPoint& pos);
    void favListUpdate();
    //
    QHash<unsigned int, TreeOp*> state;
    QHash<QString, QHash<QString, QString>*> reduc;
    unsigned int step = 0;
    void stateOp(unsigned int _step, QTreeView* _t, QString _u);
    void stateOps(unsigned int _step, QTreeView* _t);
    QString urlProc(QString _sel);
    //
    QToolBar* createLToolBar();
    QToolBar* createRToolBar();
    QToolBar* createTToolBar();
    //
    //    QMenu *createFileMenu(TabWidget *tabWidget);
    //    QMenu *createEditMenu();
    //    QMenu *createViewMenu(QToolBar *toolBar);
    //    QMenu *createWindowMenu(TabWidget *tabWidget);
    //    QMenu *createHelpMenu();
    //
    Browser* m_browser;
    QWebEngineProfile* m_profile = Q_NULLPTR;
    TabWidget* m_tabWidget;
    //QProgressBar* m_progressBar;
    QAction* m_favoritesAddAction;
    QAction* m_favoritesListDown;
    QAction* m_favoritesListShow;
    QAction* m_favoritesListRm;
    QAction* m_favoritesListUp;
    //
    QAction* m_goUpAction;
    QAction* m_goDownAction;
    QAction* m_driveOpticalAction;
    QAction* m_historyBackAction = Q_NULLPTR;
    QAction* m_historyForwardAction;
    QAction* m_historyHomeAction;
    //
    QAction* m_toggleFullScreenAction;
    QAction* m_requestHelpAction;
    QAction* m_requestAboutAction;
    QAction* m_requestCloseAction;
    QAction* m_viewPaneTreeAction;
    QAction* m_viewPaneDetailedAction = Q_NULLPTR;
    QAction* m_viewRefreshAction;
    QAction* m_viewSideTreeAction;
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    QAction* m_zoomOriginalAction  = Q_NULLPTR;
    QAction* search_action_;
    QAction* search_action_input_;
    SearchWidget* search_widget_;
    //
    QAction* m_stopAction;
    QAction* m_reloadAction;
    QAction* m_stopReloadAction;
    //
    QAction* m_tabCloseAction;
    QAction* m_tabReloadAction;
    QAction* m_tabReloadAllAction;
    QAction* m_tabNewBackgroundAction;
    //
    QLabel* dLabel;
    //
    //    QLineEdit *m_urlLineEdit;
    //    QAction *m_favAction;
    //
    QString m_lastSearch;
    QSplitter* h_splitter_;
    QSplitter* v_splitter_ = Q_NULLPTR;

    QString text_to_find_;
};

#endif // BROWSERWINDOW_H
