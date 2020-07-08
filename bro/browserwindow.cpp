#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include "webview.h"
#include "searchwidget.h"
//#include "downloadmanagerwidget.h"
//
#include <QApplication>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolBar>
#include <QSplitter>
#include <QStandardItemModel>
#include <QMenu>
#include <QStyleFactory>
#include <QFileDialog>
#include <QTimer>
//
#include "../libengines/libengines.h"


BrowserWindow::BrowserWindow(Browser *browser, QWebEngineProfile *profile, QString conf)
    : m_browser(browser)
    , m_profile(profile)
    , m_treeWidget(
          new QWidget(this))
    , m_tabWidget(
          new TabWidget(profile, this))
    //, this->m_progressBar(nullptr)
    , m_favoritesAddAction(nullptr)
    , m_favoritesListDown(nullptr)
    , m_favoritesListRm(nullptr)
    , m_favoritesListShow(nullptr)
    , m_favoritesListUp(nullptr)
    //
    , m_goUpAction(nullptr)
    , m_goDownAction(nullptr)
    , m_historyBackAction(nullptr)
    , m_driveOpticalAction(nullptr)
    , m_historyForwardAction(nullptr)
    , m_historyHomeAction(nullptr)
    //
    , m_toggleFullScreenAction(nullptr)
    , m_requestHelpAction(nullptr)
    , m_requestAboutAction(nullptr)
    , m_requestCloseAction(nullptr)
    , m_viewPaneDetailedAction(nullptr)
    , m_viewPaneTreeAction(nullptr)
    , m_viewRefreshAction(nullptr)
    , m_viewSideTreeAction(nullptr)
    , m_zoomInAction(nullptr)
    , m_zoomOutAction(nullptr)
    , m_zoomOriginalAction(nullptr)
    //
    , m_tabCloseAction(nullptr)
    , m_tabReloadAction(nullptr)
    , m_tabReloadAllAction(nullptr)
    , m_tabNewBackgroundAction(nullptr)
    //, this->m_stopAction(nullptr)
    //, this->m_reloadAction(nullptr)
    //, this->m_stopReloadAction(nullptr)
    //, this->m_urlLineEdit(nullptr)
    //, this->m_favAction(nullptr)
    , flist(new QDialog())
    , icfg(new QSettings(
               conf, QSettings::IniFormat))
    //
    , h_splitter_(nullptr)
    , v_splitter_(nullptr)
    , dLabel(nullptr)
{
    _log::log("BrowserWindow::BrowserWindow()"+ conf);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setFocusPolicy(Qt::ClickFocus);
    //
    _log::log("BrowserWinstatedow::BrowserWindow() 2");
    //**************** frame 1 20190703 **************

    //    this->m_treeView->setStyleSheet(
    setStyleSheet(
                "QAbstractScrollArea {"
                "   background-color: white;"
                "}"
                //                "QTreeView::item:hover {"
                //                "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);"
                //                "   border: 1px solid #bfcde4;"
                //                "}"
                "QSplitter::handle {"
                //                "background-color: #888;"
                "}"
                );
    //************************************************

    _log::log("BrowserWindow::BrowserWindow() 3");
    //**************** frame 2 20190703 **************

    //************************************************

    _log::log("BrowserWindow::BrowserWindow() 4");
    //**************** layout 2 20190703 *************
    QVBoxLayout *layout2 = new QVBoxLayout(this);
    layout2->setSpacing(0);
    layout2->setMargin(0);
    layout2->setContentsMargins(0,0,0,0);
    layout2->setDirection(QBoxLayout::TopToBottom);
    //    layout2->addWidget(
    //                this->createTToolBar(),Qt::AlignLeft);



    //    layout2->addWidget(l2);
    //    layout2->addWidget(this->m_treeView2,Qt::AlignTop);
    //************************************************

    this->v_splitter_ = new QSplitter(this);
    this->v_splitter_->setOrientation(Qt::Vertical);
    auto vs_widget1 = new QWidget();
    auto vs_layout1 = new QVBoxLayout(vs_widget1);
    vs_layout1->setContentsMargins(0, 0, 0, 0);
    vs_layout1->setSpacing(0);  //Крутить размеры тут
    //
    QLabel* l1 = new QLabel("Структура ОК МНСК", vs_widget1);
    l1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    l1->setStyleSheet("min-width: 28px; min-height: 28px;");
    l1->setIndent(5);
    //
    vs_layout1->addWidget(l1);
    this->m_treeView = new QTreeView(vs_widget1);
    this->m_treeView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    //    this->m_treeView->resizeColumnToContents(0);
    this->m_treeView->setHeaderHidden(true);
    this->m_treeView->setToolTipDuration(1);
    //


    //this->m_treeView->setStyleSheet("border-width: 2px; border-color: gray; border-style: solid;");
    //this->m_treeView->setStyleSheet("border-width: 2px; border-color: gray; border-style: solid; border-radius: 7;");

    vs_layout1->addWidget(this->m_treeView, Qt::AlignTop);

    auto vs_widget2 = new QWidget();
    auto vs_layout2 = new QVBoxLayout(vs_widget2);
    vs_layout2->setContentsMargins(0, 0, 0, 0);
    vs_layout2->setSpacing(0);  //Крутить размеры тут
    QLabel* l2 = new QLabel("Содержание", vs_widget2);
    l2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    l2->setStyleSheet("min-width: 28px; min-height: 28px;");
    l2->setIndent(5);
    vs_layout2->addWidget(l2);
    this->m_treeView2 = new QTreeView(vs_widget2);

    //    QPalette palette = vs_widget2->palette();
    //    QColor bcolor = palette.color(QPalette::Base);
    //    int r;
    //    int g;
    //    int b;
    //    bcolor.getRgb(&r,&g,&b);
    //    r++;
    //    g++;
    //    b++;
    //    bcolor.setRgb(r,g,b);

    //    palette.setBrush(QPalette::Base,bcolor);
    //    vs_widget2->setPalette(palette);

    this->m_treeView2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    //    this->m_treeView2->resizeColumnToContents(0);
    this->m_treeView2->setHeaderHidden(true);
    this->m_treeView2->setToolTipDuration(1);
    //this->m_treeView2->setStyleSheet("border-width: 2px; border-color: gray; border-style: solid;");
    //    this->m_treeView2->setStyleSheet("border-width: 2px; border-color: gray; border-style: solid; border-radius: 7;");

    //this->m_treeView2->hide();
    vs_layout2->addWidget(this->m_treeView2, Qt::AlignTop);

    //    QLabel* l3 = new QLabel(" ", vs_widget2);
    //    l3->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    //    l3->setStyleSheet("min-width: 28px; min-height: 28px;");
    //    l3->setIndent(5);
    //    vs_layout2->addWidget(l3);
    this->dLabel = new QLabel(" ", vs_widget2);
    this->dLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->dLabel->setStyleSheet("min-width: 28px; min-height: 28px;");
    this->dLabel->setIndent(5);
    vs_layout2->addWidget(this->dLabel);

    this->v_splitter_->addWidget(vs_widget1);
    this->v_splitter_->addWidget(vs_widget2);
    //************************************************

    this->m_treeWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    this->m_treeWidget->setLayout(layout2);

    _log::log("BrowserWindow::BrowserWindow() 3");
    //**************** layout 1 20190703 *************
    QHBoxLayout *layout = new QHBoxLayout;
    //
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    //    layout->addWidget(this->m_treeWidget,Qt::AlignLeft);
    //    layout->addWidget(this->m_tabWidget,Qt::AlignJustify);
    //************************************************

    _log::log("BrowserWindow::BrowserWindow() 4");

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);

    this->h_splitter_ = new QSplitter(this);
    this->h_splitter_->setContentsMargins(0, 0, 0, 0);
    this->h_splitter_->addWidget(this->v_splitter_);
    this->h_splitter_->addWidget(this->m_tabWidget);
    setCentralWidget(this->h_splitter_);
    //***************************************************

    _log::log("BrowserWindow::BrowserWindow() 5");
    //********************************************
    //********  GEOM ENGINE 20190604  ************
    //********************************************
    //this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //this->show();
    //********************************************
    //********************************************
    //********************************************
    //
    _log::log("BrowserWindow::BrowserWindow() 6");
    this->m_treeWidget->hide();
    this->m_tabWidget->hide();
    //

    this->h_splitter_->hide();
    //***************************************************

    //
    this->icfg->beginGroup("main");
    this->wtext = cfg2str(this->icfg,"prodName")+
            +"\n\n Разработчик:\n"+cfg2str(this->icfg,"prodOrg")
        #ifdef QT_DEBUG
            +"\n\n Версия: " + VERSION_STRING;
#else
            +"\n\n Версия: "+cfg2str(this->icfg,"prodVer");
#endif

    this->setPalette(
                welcome(
                    cfg2str(this->icfg,"prodWel"),wtext));
    QString _lib = cfg2str(this->icfg,"libPath");

    _log::log("BrowserWindow::BrowserWindow() 7");
    //*** library ops 20200316 **************
    this->icfg->endGroup();
    this->icfg->beginGroup("browserwindow");
    this->lib = cdUp(
                objdir(conf));
    this->hlp = objpath(
                this->lib, cfg2str(this->icfg,"helpPath"));
    _log::log("HLP :: " + this->hlp);
    this->lib = objpath(
                this->lib, _lib);
    _log::log("LIB :: " + this->lib);
    //****************************************
    //
    _log::log("BrowserWindow::BrowserWindow() 8");
    this->m_geometry = this->geometry();
    showMaximized();

    setMinimumSize(1000, 600);
    this->h_splitter_->setSizes({
                                    this->m_treeView->minimumWidth(),
                                    width()});
    connect(this->m_treeView, &QTreeView::expanded, this, &BrowserWindow::adjustTree1);
    connect(this->m_treeView, &QTreeView::collapsed, this, &BrowserWindow::adjustTree1);
    connect(this->m_treeView2, &QTreeView::expanded, this, &BrowserWindow::adjustTree2);
    connect(this->m_treeView2, &QTreeView::collapsed, this, &BrowserWindow::adjustTree2);
    //***************************************************
}

void BrowserWindow::mousePressEvent(QMouseEvent* event) {

    if (0<this->m_tabWidget->count())
        if (this->m_tabWidget->currentWebView()->hasSelection()){
            _log::log("BrowserWindow::mousePressEvent() "+this->m_tabWidget->currentWebView()->selectedText());
            //this->m_tabWidget->currentWebView()->setToolTip(
            _log::log(this->m_tabWidget->currentWebView()->selectedText());
            this->m_tabWidget->currentWebView()->setToolTipDuration(15000);
        };

    _log::log("BrowserWindow::mousePressEvent()");
    if(!this->m_tabWidget->isHidden())
        if(Q_NULLPTR != event)
            return;
    //
    QPalette palette;
    palette.setBrush(QPalette::Base, Qt::transparent);
    this->setPalette(palette);
    //
    _log::log("BrowserWindow::mousePressEvent() 2");

    //*** library ops 20190710 **************
    _log::log("BrowserWindow::mousePressEvent() LIB :: " + this->lib);
    QStringList* _lib = biblio(this->lib,
                               cfg2str(this->icfg,"indexName"));
    //
    _log::log("BrowserWindow::mousePressEvent() TABS :: " + QString::number(
                  this->m_tabWidget->tabBar()->count()));
    //
    //
    if(0==this->m_tabWidget->tabBar()->count())
        for (int i=0; i<_lib->size(); i++){
            QString _url = _lib->at(i);
            //
            this->m_tabWidget->createTab();
            this->urlProc(_url);
            this->m_tabWidget->currentWebView()->setStatusTip(_url);
            this->m_tabWidget->currentWebView()->setZoomFactor(1);
            if(0==this->m_tabWidget->currentIndex())
                this->m_tabWidget->currentWebView()->setZoomFactor(1.5);
            //
            connect(this->m_tabWidget->currentWebView(), &QWebEngineView::loadFinished, [this](bool ok) {
                if(!ok){
                    _log::log("QWebEngineView::loadFinished :: FAIL");
                    return;
                };
                _log::log("QWebEngineView::loadFinished :: OK");

                QString _rrl = this->m_tabWidget->currentWebView()->url().toLocalFile();

                _log::log("QWebEngineView::loadFinished :: URL "+_rrl);
                _log::log("QWebEngineView::loadFinished :: BUILDING REDUC HASH TABLE ...");

                //****************************************************
                //**************** reduc algo 20190911 ***************
                //****************************************************

                QString* rfile = objrel1(
                            objdir(_rrl),
                            cfg2str(this->icfg,"reducName"));

                if (nullptr==rfile)
                {
                    _log::log("QWebEngineView::loadFinished :: ERROR :: NO REDUC FILE FOR "+_rrl);
                    return;
                };

                QString _rfile = QString(
                            rfile->data());
                QString _rpath = objdir(_rfile);
                QString _rtag = cfg2str(this->icfg,"reducTag");

                _log::log("  rrl = "+_rrl);
                _log::log("rfile = "+_rfile);
                _log::log(" path = "+_rpath);
                _log::log(" rtag = "+_rtag);
                //
                if(!this->reduc.contains(_rpath))
                    this->reduc[_rpath] = rhash1(_rfile,"<"+_rtag+">","</"+_rtag+">");
                //
                this->m_tabWidget->currentWebView()->reduc = this->reduc.value(_rpath);

                //********************************
                //********************************
                //********************************

                //***** fav engine 20200317 ******
                //this->m_favoritesAddAction->setDisabled(true);
                //this->m_favoritesListShow->setDisabled(true);
                //
                this->ffile = this->lib
                        +QDir::separator()
                        +cfg2str(this->icfg,"favName");
                //
                if(""!=objchk(this->ffile)) {
                    _log::log("QWebEngineView::loadFinished :: INFO :: FAVORITES FILE TOUCHED "+this->ffile);
                    this->m_favoritesListShow->setDisabled(false);
//                    if(this->m_treeView2->selectionModel()->hasSelection())
//                        this->m_favoritesAddAction->setDisabled(false);
                };
                //********************************

            });
            //
            //            connect(this->m_tabWidget->currentWebView()->page(),
            //                    SIGNAL(
            //                        javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)), this,
            //                    SLOT(
            //                        javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)));

            //  ::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

            //void BrowserWindow::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)

            //
            _log::log("Tab "+QString::number(i)+" from cabinet "+_url);
        };
    //****************************************

    if(this->h_splitter_->isHidden())
        this->h_splitter_->show();

    _log::log("BrowserWindow::mousePressEvent() 3");
    if(this->m_tabWidget->isHidden()){
        // ********************************************
        this->m_tabWidget->setCornerWidget(
                    createLToolBar(),Qt::TopLeftCorner);
        this->m_tabWidget->setCornerWidget(
                    createRToolBar(),Qt::TopRightCorner);
        connect(this->m_tabWidget->tabBar(),
                SIGNAL(currentChanged(int)),
                this, SLOT(treeReload1()));
        //
        connect(this->m_tabWidget->tabBar(),
                SIGNAL(currentChanged(int)),
                this, SLOT(treeReload1()));
        //
        connect(this->m_tabWidget->tabBar(),
                SIGNAL(
                    customContextMenuRequested(const QPoint&)), this,
                SLOT(
                    tabMenu(const QPoint&)));

        //void BrowserWindow::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
        this->m_tabWidget->show();
        // ********************************************
    };

    _log::log("BrowserWindow::mousePressEvent() 4");
    if(this->m_treeWidget->isHidden()){
        // ********************************************
        connect(this->m_treeView,
                SIGNAL(
                    clicked(const QModelIndex&)), this,
                SLOT(
                    treeReload2(const QModelIndex&)));
        connect(this->m_treeView2,
                SIGNAL(
                    clicked(const QModelIndex&)), this,
                SLOT(
                    treeReload3(const QModelIndex&)));
        this->m_treeWidget->show();
        //this->m_tabWidget->setStyleSheet("border: 1px solid grey;");
        // ********************************************
    };
    //

    if(0<this->m_tabWidget->tabBar()->count()){
        _log::log("BrowserWindow::mousePressEvent() 5");
        this->m_tabWidget->setCurrentIndex(0);
        this->m_tabWidget->currentWebView()->setStyleSheet("border-width: 2px; border-color: gray; border-style: solid; border-radius: 7;");
        //        this->m_tabWidget->tabBar()->setTabText(0,
        //                    cfg2str(this->icfg,"ukmName"));
        this->treeReload1();
    };

    //        this->model = json2mod(
    //                objdir(this->m_tabWidget->currentWebView()->toolTip())+
    //                cfg2str(this->icfg,"descName"));
    //    this->m_treeView->setModel(this->model);setTracking
    //    this->m_treeView->hideColumn(1);
    // ****************************************

    _log::log("BrowserWindow::mousePressEvent() 6");
    //this->showMaximized();
    this->show();
    this->update();
}

//    //***************** tabs menu engine 20190916 **********
//    this->m_tabWidget->setTabsClosable(false);
//    //
//    this->m_tabCloseAction = new QAction(this);
//    this->m_tabCloseAction->setText(
//                this->tr("&Закрыть вкладку"));
//    this->m_tabCloseAction->setToolTip(
//                this->m_tabCloseAction->text());
//    this->m_tabCloseAction->setIcon(
//                myIcon("tab-close"));
//    connect(this->m_tabCloseAction, &QAction::triggered, [this]
//            () {
////        this->m_tabWidget->currentWebView()->close();
////        int index = this->m_tabWidget->currentIndex();
//        this->m_tabWidget->tabBar()->removeTab(index);
//    });
//    menu.addAction(this->m_tabCloseAction);

//    this->m_tabWidget->setTabsClosable(false);
//    //
//    this->m_tabReloadAction = new QAction(this);
//    this->m_tabReloadAction->setText(
//                this->tr("&Закрыть вкладку"));
//    this->m_tabReloadAction->setToolTip(
//                this->m_tabCloseAction->text());
//    this->m_tabReloadAction->setIcon(
//                myIcon("tab-close"));
//    connect(this->m_tabReloadAction, &QAction::triggered, [this]
//            () {
//        this->m_tabWidget->tabBar()->removeTab(index);
//    });
//    menu.addAction(this->m_tabCloseAction);

//    this->m_tabWidget->setTabsClosable(false);
//    //
//    this->m_tabReloadAllAction = new QAction(this);
//    this->m_tabReloadAllAction->setText(
//                this->tr("&Закрыть вкладку"));
//    this->m_tabReloadAllAction->setToolTip(
//                this->m_tabCloseAction->text());
//    this->m_tabReloadAllAction->setIcon(
//                myIcon("tab-close"));
//    connect(this->m_tabReloadAllAction, &QAction::triggered, [this]
//            () {
//        this->m_tabWidget->tabBar()->removeTab(index);
//    });
//    menu.addAction(this->m_tabReloadAllAction);

//menu.addAction(tr("Новая &вкладка"), this->m_tabWidget, &TabWidget::createTab, QKeySequence::AddTab);


void BrowserWindow::javaScriptConsoleMessage(QWebEnginePage::JavaScriptConsoleMessageLevel level, const QString &message, int lineNumber, const QString &sourceID)
{
    _log::log("BrowserWindow::javaScriptConsoleMessage "+message);
}

QString BrowserWindow::fMenu(const QPoint& pos)
{
    //*********** fav menu engine 20200317 *************
    QStringList _cat = cfg2str(this->icfg,"favCat").split(",");
    int _cs=_cat.size();
    if(0>=_cs)
        return "";
    //
    QMenu* menu;
    menu = new QMenu();
    //
    for (int i=1; i<_cs; i++)
    {
        QAction* action = new QAction(
                    _cat.value(i));
        action->setIconText(
                    QString::number(i));
        menu->addAction(action);
    };
    QAction* action = menu->exec(
                QCursor::pos());
    if(nullptr!=action)
        return action->iconText();
    return "";
    //**************************************************
}

void BrowserWindow::tabMenu(const QPoint& pos)
{
    int index = this->m_tabWidget->tabBar()->tabAt(pos);
    if(index<0)
        return;

    QMenu menu;
    QAction* action = menu.addAction(
                tr("Перезагрузить вкладку"));
    action->setShortcut(QKeySequence::Refresh);
    connect(action, &QAction::triggered, this->m_tabWidget, [this,index]() {
        this->m_tabWidget->currentWebView()->reload();
    });

    menu.addAction(
                tr("Перезагрузить все вкладки"), this->m_tabWidget, &TabWidget::reloadAllTabs);

    if(index > 3){
        action = menu.addAction(tr("Закрыть вкладку"));
        connect(action, &QAction::triggered, this->m_tabWidget, [this,index]() {
            this->m_tabWidget->removeTab(index);
        });
    };

    menu.exec(
                QCursor::pos());
}

void BrowserWindow::stateOps(unsigned int _step, QTreeView* tree)
{
    _log::log("BrowserWindow::stateOps() "+QString::number(_step));
#ifdef QT_DEBUG
    this->dLabel->setText("Шаг: "+
                          QString::number(this->step));
#endif
    _log::log("stateOp 1");
    //************************************************************
    //************* tree state machine operator 190910 ***********
    //************************************************************
    if (nullptr!=tree){
        _log::log("stateOp 2");
        if(!this->state.contains(_step))
            this->state[_step] = new TreeOp(tree);

        for(int i=1; i<this->state.size(); i++)
        {
            if(this->state.contains(_step-i))
                if(tree->statusTip()==this->state.value(_step-i)->src){
                    this->state[_step]->model = this->state.value(_step-i)->model;
                    this->state[_step]->src = this->state.value(_step-i)->src;
                    i=this->state.size();
                    _log::log("stateOp 3");
                };

            if(this->state.contains(_step+i))
                if(tree->statusTip()==this->state.value(_step+i)->src){
                    this->state[_step]->model = this->state.value(_step+i)->model;
                    this->state[_step]->src = this->state.value(_step+i)->src;
                    i=this->state.size();
                    _log::log("stateOp 4");
                };
        };

        if(nullptr==tree->model() | tree->statusTip()!=this->state.value(_step)->src)
        {
            _log::log("stateOp 5");
            _log::log(tree->statusTip());
            _log::log(this->state.value(_step)->src);
            QString _suf = "*."+QFileInfo(
                        cfg2str(this->icfg,"descName")).completeSuffix();
            _log::log("stateOp 6");
            this->state[_step]->model = json2mod(
                        dir2data(
                            tree->statusTip(),_suf), tree, this->clist.log->toolTip());
            _log::log("stateOp 7");
            tree->setModel(
                        this->state.value(_step)->model);
            this->state[_step]->src = tree->statusTip();
        };

        _log::log("stateOp 8");
        this->state[_step]->url = tree->toolTip();

        _log::log("stateOp 9");
        this->state[_step]->index = tree->selectionModel()->currentIndex();
        return;
    };
    //
    _log::log("stateOp 10");
    if (!this->state.contains(_step))
        return;

    _log::log("stateOp 11");
    if (nullptr==this->state.value(_step)->tree)
        return;

    _log::log("stateOp 12");
    QTreeView* _tree = this->state.value(_step)->tree;

    _log::log("stateOp 13");
    if (nullptr!=this->state.value(_step)->model)
        _tree->setModel(
                    this->state.value(_step)->model);

    _log::log("stateOp 14");
    if (!this->state.value(_step)->index.isValid())
        return;

    _log::log("stateOp 15");
    _tree->selectionModel()->clearSelection();
    _tree->selectionModel()->setCurrentIndex(
                this->state.value(_step)
                ->index, QItemSelectionModel::Select);
    //************************************************************
    //************************************************************
    //************************************************************
}

void BrowserWindow::stateOp(unsigned int _step, QTreeView* t, QString _url)
{
    _log::log("stateOp() STEP :: "+QString::number(_step));
    _log::log("stateOp() URL :: "+_url);
    t->setStatusTip(
                objdir(_url));
    t->setToolTip(_url);
    this->stateOps(_step, t);
}

void BrowserWindow::treeReload1(){
    _log::log("treeReload1() STEP :: "+QString::number(this->step));
    this->m_treeWidget->hide();
    this->v_splitter_->hide();
    //
    QString _url = this->m_tabWidget->currentWebView()->statusTip();
    if (_url.isEmpty() || _url.isNull())
        return;
    //this->step++;
    this->stateOp(this->step, this->m_treeView, _url);
    //
    if(nullptr!=this->m_treeView->model())
    {
        treeFormat1(this->m_treeView);
        //

        //
        this->m_treeWidget->show();
        this->v_splitter_->show();
        //this->m_treeView->setFocus();
    };
    //
    if(0==this->m_tabWidget->currentIndex())
        this->m_tabWidget->currentWebView()->setZoomFactor(1.5);
    this->urlProc(
                this->state.value(this->step)->url);
    this->m_tabWidget->currentWebView()->update();
}

QString BrowserWindow::urlProc(QString _sel){

    // ************** url processing 20190913 **************
    QString _url = objpath(this->lib,_sel);
    if(_url.isEmpty() | _url.isNull())
        _url = _sel;
    _log::log("urlProc() URL :: " + _url);
    if(0<_url.toLower().indexOf(".htm"))
        this->m_tabWidget->currentWebView()->setUrl(
                QUrl::fromLocalFile(_url));
    // *****************************************************
    return _url;
}

void BrowserWindow::treeReload2(const QModelIndex& index){
    _log::log("treeReload2() STEP :: "+QString::number(this->step));
    if(!this->m_treeView->selectionModel()->hasSelection())
        return;
    QString _url = this->urlProc(
                treeC1Text(index));
    //
    //index.s
    //
    this->step++;
    this->m_treeView->setToolTip(_url);
    this->stateOps(this->step, this->m_treeView);
    //
    this->stateOp(0, this->m_treeView2, _url);
    treeFormat1(this->m_treeView2);
    //this->m_treeView->setFocus();
}

void BrowserWindow::treeReload3(const QModelIndex& index){
    _log::log("treeReload3() STEP :: "+QString::number(this->step));
    if(!this->m_treeView2->selectionModel()->hasSelection())
        return;
    //
    QString _url = this->urlProc(
                treeC1Text(index));
    //
    //this->m_favoritesAddAction->setDisabled(false);
    //index.model()->i
    //
    this->step++;
    this->stateOp(this->step, this->m_treeView2, _url);
    treeFormat1(this->m_treeView2);
    //this->m_treeView2->setFocus();
}

QSize BrowserWindow::sizeHint() const
{
    _log::log("BrowserWindow::sizeHint()");
    //
    QRect desktopRect = QApplication::desktop()->screenGeometry();
    QSize size = desktopRect.size() * qreal(0.9);
    return size;
}

//QMenu *BrowserWindow::createWindowMenu(TabWidget *tabWidget)
//{
//    QMenu *menu = new QMenu(tr("&Окно"));

//    QAction *nextTabAction = new QAction(tr("Показать след.вкладку"), this);
//    QList<QKeySequence> shortcuts;
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceRight));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageDown));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BracketRight));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Less));
//    nextTabAction->setShortcuts(shortcuts);
//    connect(nextTabAction, &QAction::triggered, tabWidget, &TabWidget::nextTab);

//    QAction *previousTabAction = new QAction(tr("Показать пред.вкладку"), this);
//    shortcuts.clear();
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceLeft));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageUp));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BracketLeft));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Greater));
//    previousTabAction->setShortcuts(shortcuts);
//    connect(previousTabAction, &QAction::triggered, tabWidget, &TabWidget::previousTab);

//    connect(menu, &QMenu::aboutToShow, [this, menu, nextTabAction, previousTabAction]() {
//        menu->clear();
//        menu->addAction(nextTabAction);
//        menu->addAction(previousTabAction);
//        menu->addSeparator();

//        QVector<BrowserWindow*> windows = this->m_browser->windows();
//        int index(-1);
//        for (auto window : windows) {
//            QAction *action = menu->addAction(window->windowTitle(), this, &BrowserWindow::handleShowWindowTriggered);
//            action->setData(++index);
//            action->setCheckable(true);
//            if (window == this)
//                action->setChecked(true);
//        }
//    });
//    return menu;
//}

//void BrowserWindow::maximize(){
//    _log::log("BrowserWindow::maximize()";
//    //
//    this->showMaximized();
//};

//void BrowserWindow::handleFileOpenTriggered()
//{
//    QUrl url = QFileDialog::getOpenFileUrl(this, tr("Открыть веб-ресурс"), QString(),
//                                           tr("Веб-ресурсы (*.html *.htm *.svg *.png *.gif *.svgz);;All files (*.*)"));
//    if (url.isEmpty())
//        return;
//    currentTab()->setUrl(url);
//}

QToolBar *BrowserWindow::createTToolBar()
{
    _log::log("BrowserWindow::createLToolBar()");
    //
    QToolBar *navigationBar = new QToolBar(
                tr("Навигация"));
    navigationBar->setMovable(false);
    navigationBar->toggleViewAction()->setEnabled(false);
    navigationBar->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    //navigationBar->setStyleSheet("border: 1px solid grey;");
    //
    // **** drive optical template 20190705 ***
    //    this->m_driveOpticalAction = new QAction(this);
    //    this->m_driveOpticalAction->setIcon(
    //                myIcon("drive-optical"));
    //    this->m_driveOpticalAction->setToolTip(
    //                tr("Выбор библиотеки"));
    //    connect(this->m_driveOpticalAction, &QAction::triggered, [this]
    //            () {
    //        QString _lib = QFileDialog::getExistingDirectory(this,"Выберете каталог библиотеки","/");
    //        if (_lib.isNull() || _lib.isEmpty() || !QDir::setCurrent(_lib))
    //            return;
    //        //
    //        this->m_tabWidget->setTabsClosable(true);
    //        // remove all tabs after current
    //        for (int i = this->m_tabWidget->count() - 1; i > this->m_tabWidget->currentIndex(); --i) {
    //           this->m_tabWidget->removeTab(i);
    //        };

    //        // current tab is now the last, therefore remove all but the last
    //        for (int i = this->m_tabWidget->count(); i > 1; --i) {
    //           this->m_tabWidget->removeTab(0);
    //        } ;       //
    //        this->m_tabWidget->removeTab(0);

    //        this->lib = _lib;
    //        _log::log("BrowserWindow::createTToolBar() TABS :: " + this->m_tabWidget->tabBar()->count();
    //        _log::log("BrowserWindow::createTToolBar() LIB :: " + _lib;
    //        this->mousePressEvent(nullptr);
    //        //this->m_tabWidget->show();
    //    });
    //    navigationBar->addAction(this->m_driveOpticalAction);
    //******************************************

    return navigationBar;
}

QToolBar *BrowserWindow::createLToolBar()
{
    _log::log("BrowserWindow::createLToolBar()");
    //
    QToolBar *navigationBar = new QToolBar(
                tr("Навигация"));
    navigationBar->setMovable(false);
    navigationBar->toggleViewAction()->setEnabled(false);
    navigationBar->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    //navigationBar->setStyleSheet("border: 1px solid grey;");

    //******* new tab action 20190602 *********
    this->m_tabNewBackgroundAction = new QAction(this);
    this->m_tabNewBackgroundAction->setText(
                this->tr("Открыть в новой вкладке"));
    this->m_tabNewBackgroundAction->setToolTip(
                this->m_tabNewBackgroundAction->text());
    this->m_tabNewBackgroundAction->setIcon(
                myIcon("tab-new-background"));
    //    this->tabNewBackgroundAction->setShortcut(
    //                QKeySequence(Qt::MiddleButton));
    connect(this->m_tabNewBackgroundAction, &QAction::triggered, [this]
            () {
        int r = this->m_treeView2->selectionModel()->currentIndex().row();
        QString _url = objpath(this->lib,
                               this->m_treeView2->selectionModel()->currentIndex().sibling(r,1).data().toString());
        this->m_tabWidget->createTab();
        this->m_tabWidget->currentWebView()->setStatusTip(_url);
        this->urlProc(_url);
        //
    });

    this->m_treeView2->setContextMenuPolicy(Qt::ActionsContextMenu);
    this->m_treeView2->addAction(this->m_tabNewBackgroundAction);
    //*****************************************

    //******* view pane detailed action 20190828 *********
    this->m_viewPaneDetailedAction = new QAction(this);
    this->m_viewPaneDetailedAction->setText(
                this->tr("Развернуть все"));
    this->m_viewPaneDetailedAction->setToolTip(
                this->m_viewPaneDetailedAction->text());
    this->m_viewPaneDetailedAction->setIcon(
                myIcon("view-pane-detailed"));
    connect(this->m_viewPaneDetailedAction, &QAction::triggered, [this]
            () {
        this->m_treeView->expandAll();
    });

    this->m_treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    this->m_treeView->addAction(this->m_viewPaneDetailedAction);
    //*****************************************************

    // **** view pane tree template 20190705 ***
    this->m_viewPaneTreeAction = new QAction(this);
    this->m_viewPaneTreeAction->setText(
                this->tr("Свернуть все"));
    this->m_viewPaneTreeAction->setToolTip(
                this->m_viewPaneTreeAction->text());
    this->m_viewPaneTreeAction->setIcon(
                myIcon("view-pane-tree"));
    connect(this->m_viewPaneTreeAction, &QAction::triggered, [this]
            () {
        this->m_treeView->collapseAll();
    });
    this->m_treeView->addAction(this->m_viewPaneTreeAction);
    //******************************************

    // **** view side tree template 20190705 ***
    this->m_viewSideTreeAction = new QAction(this);
    this->m_viewSideTreeAction->setIcon(
                myIcon("view-sidetree"));
    this->m_viewSideTreeAction->setToolTip(
                tr("Показать/спрятать дерево переходов"));
    connect(this->m_viewSideTreeAction, &QAction::triggered, [this]
            () {
        if(this->h_splitter_->count() && this->h_splitter_->sizes()[0]) {
            this->h_splitter_->setSizes({0, width()});
        } else {
            this->h_splitter_->setSizes({100, width()});
        }
    });
    navigationBar->addAction(this->m_viewSideTreeAction);
    //*****************************************

    // **** history back template 20190604 ***
    this->m_historyBackAction = new QAction(this);
    QList<QKeySequence> backShortcuts = QKeySequence::keyBindings(QKeySequence::Back);
    for (auto it = backShortcuts.begin(); it != backShortcuts.end();) {
        if ((*it)[0] == Qt::Key_Backspace)
            it = backShortcuts.erase(it);
        else
            ++it;
    };
    backShortcuts.append(QKeySequence(Qt::Key_Back));
    this->m_historyBackAction->setShortcuts(backShortcuts);
    //this->m_historyBackAction->setIconVisibleInMenu(false);
    this->m_historyBackAction->setIcon(
                myIcon("go-previous"));
    this->m_historyBackAction->setToolTip(
                tr("Назад"));
    connect(this->m_historyBackAction, &QAction::triggered, [this]() {
        if(this->m_treeWidget->isHidden()){
            this->m_tabWidget->currentWebView()->triggerPageAction(QWebEnginePage::Back);
            return;
        };
        //
        if(this->step<=1)
            return;
        if(!this->state.contains(this->step-1))
            return;
        //
        _log::log("BrowserWindow::createLToolBar() STEP :: "+QString::number(this->step));
        this->step--;
        this->stateOps(this->step, nullptr);
        //
        QString _url = this->urlProc(
                    this->state.value(this->step)->url);
        //
        treeFormat1(this->state.value(this->step)->tree);
        if(this->state.value(this->step)->src!=this->m_treeView2->statusTip())
            if(this->step>1)
            {
                this->stateOp(0, this->m_treeView2, _url);
                this->state.remove(0);
                this->stateOps(this->step, nullptr);
                treeFormat1(this->m_treeView2);
            }
    });
    navigationBar->addAction(this->m_historyBackAction);
    //****************************************

    // **** go up template 20190909 ***
    this->m_goUpAction = new QAction(this);
    QList<QKeySequence> goUpShortcuts = QKeySequence::keyBindings(QKeySequence::MoveToPreviousLine);
    goUpShortcuts.append(
                QKeySequence(Qt::ALT + Qt::UpArrow));
    this->m_goUpAction->setShortcuts(goUpShortcuts);
    this->m_goUpAction->setText(
                this->tr("Вверх по содержимому"));
    this->m_goUpAction->setToolTip(
                this->m_goUpAction->text());
    this->m_goUpAction->setIcon(
                myIcon("go-up"));
    connect(this->m_goUpAction, &QAction::triggered, [this]
            () {
        if(!this->state.contains(this->step))
            return;
        //
        QTreeView* _tree = this->state.value(this->step)->tree;
        QModelIndex _i = prevC0Index(_tree);
        this->state[this->step]->index = _i;
        this->stateOps(this->step, nullptr);
        //
        //        QString _url = this->urlProc(
        //                    treeC1Text(_i));
        if (_tree == this->m_treeView)
            this->treeReload2(_i);
        if (_tree == this->m_treeView2)
            this->treeReload3(_i);
    });
    navigationBar->addAction(this->m_goUpAction);
    //******************************************

    // **** go down template 20190909 ***
    this->m_goDownAction = new QAction(this);
    QList<QKeySequence> goDownShortcuts = QKeySequence::keyBindings(QKeySequence::MoveToPreviousLine);
    goDownShortcuts.append(
                QKeySequence(Qt::ALT + Qt::DownArrow));
    this->m_goDownAction->setShortcuts(goDownShortcuts);
    this->m_goDownAction->setText(
                this->tr("Вниз по содержимому"));
    this->m_goDownAction->setToolTip(
                this->m_goDownAction->text());
    this->m_goDownAction->setIcon(
                myIcon("go-down"));
    connect(this->m_goDownAction, &QAction::triggered, [this]
            () {
        if(!this->state.contains(this->step))
            return;
        //
        QTreeView* _tree = this->state.value(this->step)->tree;
        QModelIndex _i = nextC0Index(_tree);
        this->state[this->step]->index = _i;
        this->stateOps(this->step, nullptr);
        //
        if (_tree == this->m_treeView)
            this->treeReload2(_i);
        if (_tree == this->m_treeView2)
            this->treeReload3(_i);
    });
    navigationBar->addAction(this->m_goDownAction);
    //******************************************

    // **** history forward template 20190604 ***
    this->m_historyForwardAction = new QAction(this);
    QList<QKeySequence> fwdShortcuts = QKeySequence::keyBindings(QKeySequence::Forward);
    //    for (auto it = fwdShortcuts.begin(); it != fwdShortcuts.end();) {
    //        if (((*it)[0] & Qt::Key_unknown) == Qt::Key_Backspace)
    //            it = fwdShortcuts.erase(it);
    //        else
    //            ++it;
    //    }
    fwdShortcuts.append(QKeySequence(Qt::Key_Forward));
    this->m_historyForwardAction->setShortcuts(fwdShortcuts);
    this->m_historyForwardAction->setIconVisibleInMenu(false);
    this->m_historyForwardAction->setIcon(
                myIcon("go-next"));
    this->m_historyForwardAction->setToolTip(
                tr("Вперед"));
    connect(this->m_historyForwardAction, &QAction::triggered, [this]() {
        if(this->m_treeWidget->isHidden()){
            this->m_tabWidget->currentWebView()->triggerPageAction(QWebEnginePage::Forward);
            return;
        };
        if(std::numeric_limits<unsigned int>::max()<=this->step+1)
            return;
        if(!this->state.contains(this->step+1))
            return;
        //
        _log::log("BrowserWindow::createLToolBar() STEP :: "+QString::number(this->step));
        this->step++;
        this->stateOps(this->step, nullptr);
        //
        QString _url = this->urlProc(
                    this->state.value(this->step)->url);
        //
        treeFormat1(this->state.value(this->step)->tree);
        if(this->state.value(this->step)->tree!=this->m_treeView2)
        {
            this->stateOp(0, this->m_treeView2, _url);
            this->state.remove(0);
            this->stateOps(this->step, nullptr);
            treeFormat1(this->m_treeView2);
        }
        //
    });
    navigationBar->addAction(this->m_historyForwardAction);
    //*******************************************

    // **** history home template 20190604 ***
    this->m_historyHomeAction = new QAction(this);
    this->m_historyHomeAction->setIcon(
                myIcon("go-home"));
    this->m_historyHomeAction->setShortcut(Qt::CTRL + Qt::Key_Home);
    this->m_historyHomeAction->setToolTip(
                tr("К индексу каталога"));
    connect(this->m_historyHomeAction, &QAction::triggered, [this]
            () {
        // *******************************************
        // **** Home engine Ib 20190909  *************
        // *******************************************
        QString _url = this->urlProc(
                    this->m_tabWidget->currentWebView()->statusTip());
        _log::log("BrowserWindow::createLToolBar() URL :: " + _url);
        //
        if(this->m_treeWidget->isHidden())
            return;
        this->step=1;
        this->stateOps(this->step, nullptr);
        this->m_treeView2->setModel(nullptr);
        // *******************************************
        // *******************************************
        // *******************************************
    });
    navigationBar->addAction(this->m_historyHomeAction);
    //****************************************

    // **** zoom in action 20190710 *********
    this->m_zoomInAction = new QAction(this);
    this->m_zoomInAction->setIcon(
                myIcon("zoom-in"));
    this->m_zoomInAction->setShortcut(Qt::CTRL + Qt::Key_Plus);
    this->m_zoomInAction->setToolTip("Увеличить");
    connect(this->m_zoomInAction, &QAction::triggered, [this]
            () {
        qreal zoom = this->m_tabWidget->currentWebView()->zoomFactor();
        zoom+=0.1;
        if(10>zoom)
            this->m_tabWidget->currentWebView()->setZoomFactor(zoom);
    });
    navigationBar->addAction(this->m_zoomInAction);
    //****************************************

    // **** zoom out action 20190710 *********
    this->m_zoomOutAction = new QAction(this);
    this->m_zoomOutAction->setIcon(
                myIcon("zoom-out"));
    this->m_zoomOutAction->setShortcut(Qt::CTRL + Qt::Key_Minus);
    this->m_zoomOutAction->setToolTip("Уменьшить");
    connect(this->m_zoomOutAction, &QAction::triggered, [this]
            () {
        qreal zoom = this->m_tabWidget->currentWebView()->zoomFactor();
        zoom-=0.1;
        if(0<zoom)
            this->m_tabWidget->currentWebView()->setZoomFactor(zoom);
    });
    navigationBar->addAction(this->m_zoomOutAction);
    //****************************************

    // **** zoom original action 190828 *****
    this->m_zoomOriginalAction = new QAction(this);
    this->m_zoomOriginalAction->setIcon(
                myIcon("zoom-original"));
    this->m_zoomOriginalAction->setShortcut(Qt::CTRL + Qt::Key_Equal);
    this->m_zoomOriginalAction->setToolTip("Сбросить масштаб");
    connect(this->m_zoomOriginalAction, &QAction::triggered, [this] () {

        this->m_tabWidget->currentWebView()->setZoomFactor(1);
        if(0==this->m_tabWidget->currentIndex())
            this->m_tabWidget->currentWebView()->setZoomFactor(1.5);
    });
    navigationBar->addAction(this->m_zoomOriginalAction);
    //****************************************

#ifdef QT_DEBUG
    // **** view refresh template 20190705 ***
    this->m_viewRefreshAction = new QAction(this);
    this->m_viewRefreshAction->setIcon(
                myIcon("view-refresh"));
    this->m_viewRefreshAction->setToolTip(
                tr("Перезагрузить содержимое дерева переходов"));
    connect(this->m_viewRefreshAction, &QAction::triggered, [this]
            () {
        this->treeReload1();
    });
    navigationBar->addAction(this->m_viewRefreshAction);
    //****************************************
#endif

    // **** bookmark-new template 20200310 ***
    this->m_favoritesAddAction = new QAction(this);
    this->m_favoritesAddAction->setIcon(
                myIcon("bookmark-new"));
    this->m_favoritesAddAction->setToolTip(
                tr("Добавить в избранное"));
    connect(this->m_favoritesAddAction, &QAction::triggered, [this]
            () {
        if(!this->m_treeView2->selectionModel()->hasSelection())
            return;
        //
        QString _fcat = this->fMenu(
                    currMousePos(this));
        if(""==_fcat)
            return;
        //**************** fav algo 20200318 ***************
        QString _frl = treeC1Text(
                    this->m_treeView2->selectionModel()->currentIndex());
        QString _frec = treeC0Text(
                    this->m_treeView2->selectionModel()->currentIndex())
                +"|"+_frl+"\n";
        QString _ffile = this->ffile;
        _ffile.replace(
                    objname(_ffile)+".",
                    objname(_ffile)+_fcat+".");
        //
        _log::log("ffile = "+this->ffile);
        _log::log("ffile = "+_ffile);
        _log::log(" frec = "+_frec);
        _log::log("  frl = "+_frl);
        //
        if(""==objchk(_ffile))
            return;
        //
        if(utfchk(_ffile, _frl))
        {
            utfrm(_ffile, _frec);
        } else {
            utfy(_ffile, _frec);
        };
        //**************************************************
        this->m_treeView2->setModel(nullptr);
        this->stateOp(this->step, this->m_treeView2,
                      QUrl(
                          objpath(this->lib,_frl)).toLocalFile());
        treeFormat1(this->m_treeView2);
        //this->m_favoritesAddAction->setDisabled(true);
        //
        this->flist->setToolTip(_ffile);
        this->favListUpdate();
    });
    navigationBar->addAction(this->m_favoritesAddAction);
    //this->m_favoritesAddAction->setDisabled(true);
    //****************************************


    //    this->m_stopReloadAction = new QAction(this);
    //    connect(this->m_stopReloadAction, &QAction::triggered, [this]() {
    //        this->m_tabWidget->triggerWebPageAction(QWebEnginePage::WebAction(this->m_stopReloadAction->data().toInt()));
    //    });
    //navigationBar->addAction(this->m_stopReloadAction);

    //    this->m_urlLineEdit = new QLineEdit(this);
    //    this->m_favAction = new QAction(this);
    //    this->m_urlLineEdit->addAction(this->m_favAction, QLineEdit::LeadingPosition);
    //    this->m_urlLineEdit->setClearButtonEnabled(true);
    //    navigationBar->addWidget(this->m_urlLineEdit);

    //    auto downloadsAction = new QAction(this);
    //    downloadsAction->setIcon(QIcon(QStringLiteral(":go-bottom.png")));
    //    downloadsAction->setToolTip(tr("Показать загрузки"));
    //navigationBar->addAction(downloadsAction);
    //    connect(downloadsAction, &QAction::triggered, [this]() {
    //        this->m_browser->downloadManagerWidget().show();
    //    });

    return navigationBar;
}

void BrowserWindow::favUrlOpen(int r, int c)
{
    _log::log("BrowserWindow::favUrlOpen() row "
              +QString::number(r));
    //
    if (""==this->flist->toolTip())
        return;
    //
    //
    QStringList fl = objdata(
                this->flist->toolTip(),"\n");
    //
    QString _sel = "";
    if(0<fl.value(r).indexOf("|"))
        _sel = fl.value(r).split("|").at(1);
    //
    _log::log("BrowserWindow::favUrlOpen() sel "+_sel);
    //
    if(""!=_sel)
        this->urlProc(_sel);
}

void BrowserWindow::favList()
{
    QString _fcat = this->fMenu(
                currMousePos(this));
    if(""==_fcat)
        return;
    //
    QStringList _cat = cfg2str(this->icfg,"favCat").split(",");
    this->flist->setWindowTitle(
                this->tr("Избранное")
                +" :: "
                +_cat.value(
                    _fcat.toInt()));
    //
    this->flist->setStatusTip(
                this->flist->windowTitle());
    //
    QString _ffile = this->ffile;
    _ffile.replace(
                objname(_ffile)+".",
                objname(_ffile)+_fcat+".");
    this->flist->setToolTip(_ffile);
    //
    connect(this->clist.log, &QTableWidget::cellDoubleClicked, this, &BrowserWindow::favUrlOpen);
    this->favListUpdate();
    //this->m_favoritesListShow->setDisabled(false);
    this->flist->show();
}

void BrowserWindow::favListUpdate(){
    QString _ffile = this->flist->toolTip();
    QStringList fl = objdata(_ffile,"\n");
    //
    int fs=fl.size();
    if(0>=fs)
        return;
    //
    //**************** fav list 20200319 ***************
    this->clist.log->clearContents();
    obj2table(this->clist.log,fl,"|");
    this->clist.log->setToolTip(_ffile);
    //
    if(nullptr==this->clist.log->item(0,2))
        return;
    //
    QString _frl = this->clist.log->item(0,2)->text();
    this->step++;
    this->m_treeView2->setModel(nullptr);
    this->stateOp(this->step, this->m_treeView2, _frl);
    treeFormat1(this->m_treeView2);
    //
    //this->m_favoritesAddAction->setDisabled(false);
    //***************************************************
}

QToolBar* BrowserWindow::createRToolBar()
{
    _log::log("BrowserWindow::createRToolBar()");
    //
    QToolBar *navigationBar = new QToolBar(
                tr("Навигация 2"));
    navigationBar->setMovable(false);
    navigationBar->toggleViewAction()->setEnabled(false);
    navigationBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    search_widget_ = new SearchWidget(navigationBar);
    search_action_input_ = navigationBar->addWidget(search_widget_);
    search_action_input_->setVisible(false);
    connect(search_widget_, &SearchWidget::textChanged, this, &BrowserWindow::findText);
    connect(search_widget_, &SearchWidget::previousClicked, this, &BrowserWindow::findPrevious);
    connect(search_widget_, &SearchWidget::nextClicked, this, &BrowserWindow::findNext);
    search_action_ = new QAction(navigationBar);
    search_action_->setIcon(
                myIcon("edit-find"));
    search_action_->setShortcut(Qt::CTRL + Qt::Key_F);
    search_action_->setToolTip("Найти на странице");
    search_action_->setCheckable(true);
    connect(search_action_, &QAction::triggered, [this] () {
        if(search_action_->isChecked()) {
            search_action_input_->setVisible(true);
            search_widget_->setFocus();
            search_widget_->selectAll();
        } else {
            search_action_input_->setVisible(false);
            search_widget_->clear();
        }
    });
    navigationBar->addAction(search_action_);
    //******************************************

    //********************************************
    //********  GEOM ENGINE 20190604  ************
    //********************************************
    this->m_toggleFullScreenAction = new QAction(this);
    //this->m_enterFullScreenAction->setShortcuts(fsShortcuts);
    //this->m_toggleFullScreenAction->setIconVisibleInMenu(false);
    this->m_toggleFullScreenAction->setIcon(
                myIcon("view-fullscreen"));
    this->m_toggleFullScreenAction->setShortcut(Qt::Key_F11);
    this->m_toggleFullScreenAction->setToolTip(
                tr("Переключение полноэкранного режима"));
    connect(this->m_toggleFullScreenAction, &QAction::triggered, [this]
            () {
        if (this->isFullScreen()) {
            //            this->centralWidget()->showNormal();
            this->showNormal();
            _log::log("BrowserWindow::createRToolBar() showNormal()");
            this->setGeometry(this->m_geometry);
            //this->setGeometry();
            return;
        } else {
            this->m_geometry = this->geometry();
            this->showFullScreen();
            _log::log("BrowserWindow::createRToolBar() showFullScreen()");
        };
    });
    navigationBar->addAction(this->m_toggleFullScreenAction);
    //*******************************************
    //*******************************************
    //*******************************************

    // **** help template 20190820 ***
    this->m_requestHelpAction = new QAction(this);
    this->m_requestHelpAction->setIcon(
                myIcon("dialog-question"));
    this->m_requestHelpAction->setToolTip(
                tr("Справка"));
    connect(this->m_requestHelpAction, &QAction::triggered, [this]
            () {
        this->m_tabWidget->createTab();
        QString _url = this->urlProc(
                    objpath(this->hlp,
                            cfg2str(this->icfg,"indexName")));
        this->m_tabWidget->currentWebView()->setToolTip(_url);
        this->m_tabWidget->currentWebView()->setToolTipDuration(1);
    });
    navigationBar->addAction(this->m_requestHelpAction);
    //**********************************

    // **** about template 20190604 ***
    this->m_requestAboutAction = new QAction(this);
    this->m_requestAboutAction->setIcon(
                myIcon("dialog-information"));
    this->m_requestAboutAction->setToolTip(
                tr("О программе"));
    connect(this->m_requestAboutAction, &QAction::triggered, [this]
            () {
        this->about.show();
    });
    navigationBar->addAction(this->m_requestAboutAction);
    //**********************************

    // **** bookmarks list template 20200319 ***
    this->m_favoritesListShow = new QAction(this);
    this->m_favoritesListShow->setIcon(
                myIcon("bookmark-new-list"));
    this->m_favoritesListShow->setToolTip(
                tr("Список избранного"));
    connect(this->m_favoritesListShow, &QAction::triggered, this, &BrowserWindow::favList);
    navigationBar->addAction(this->m_favoritesListShow);
    this->m_favoritesListShow->setDisabled(true);
    this->clist.setupUi(this->flist);
    //
    QStringList hHeaders;
    hHeaders << "№" << "Наименование раздела" << "Ссылка на файл документа";
    this->clist.log->setHorizontalHeaderLabels(hHeaders);
    //
    connect(this->flist, &QDialog::finished, this, &BrowserWindow::favListClosed);
    this->flist->setWindowFlag(Qt::WindowStaysOnTopHint,true);
    //
    //******************************************
    this->m_favoritesListUp = new QAction(this);
    this->m_favoritesListUp->setShortcut(Qt::CTRL+Qt::Key_Up);
    connect(this->m_favoritesListUp, &QAction::triggered, [this]
            () {
        QString _ffile = this->clist.log->toolTip();
        //
        if(""==_ffile)
            return;
        if(!this->clist.log->selectionModel()->hasSelection())
            return;
        //
        int r = this->clist.log->selectionModel()->currentIndex().row();
        if(0>=r)
            return;
        //
        QStringList fl = objdata(_ffile,"\n");
        QString s = fl.value(r-1);
        fl[r-1]=fl.value(r);
        fl[r]=s;
        //
        if(utfy(_ffile,fl,"\n"))
        {
            this->flist->setToolTip(_ffile);
            this->favListUpdate();
            this->clist.log->setFocus();
            this->clist.log->selectRow(r-1);
            //this->clist.log->setToolTip("");
        };
        //
    });
    this->clist.log->addAction(
                this->m_favoritesListUp);
    //******************************************

    //******************************************
    this->m_favoritesListRm = new QAction(this);
    this->m_favoritesListRm->setShortcut(Qt::CTRL+Qt::Key_Delete);
    connect(this->m_favoritesListRm, &QAction::triggered, [this]
            () {
        //
        if(""==this->clist.log->toolTip())
            return;
        if(!this->clist.log->selectionModel()->hasSelection())
            return;
        //
        int r = this->clist.log->selectionModel()->currentIndex().row();
        //
        QString _ffile = this->clist.log->toolTip();
        QStringList fl = objdata(
                    this->clist.log->toolTip(),"\n");
        fl.removeAt(r);
        //
        if(utfy(_ffile,fl,"\n"))
        {
            this->flist->setToolTip(_ffile);
            this->favListUpdate();
            this->clist.log->setFocus();
            r = this->clist.log->rowCount();
            if(r>0)
                this->clist.log->selectRow(r-1);
            //this->clist.log->setToolTip("");
        };
        //
    });
    this->clist.log->addAction(
                this->m_favoritesListRm);
    //******************************************

    return navigationBar;
}

void BrowserWindow::favListClosed(int r){
    this->clist.log->toolTip()="";
    //
    QString _frl = this->m_tabWidget->currentWebView()->url().toLocalFile();
    //
    this->m_treeView2->setModel(nullptr);
    this->stateOp(this->step, this->m_treeView2, _frl);
    treeFormat1(this->m_treeView2);
}

//QMenu *BrowserWindow::createFileMenu(TabWidget *tabWidget)
//{
//    QMenu *fileMenu = new QMenu(tr("&Файл"));
//    fileMenu->addAction(tr("&Новое окно"), this, &BrowserWindow::handleNewWindowTriggered, QKeySequence::New);
//    fileMenu->addAction(tr("Новое &Приватное окно"), this, &BrowserWindow::handleNewIncognitoWindowTriggered);

//    QAction *newTabAction = new QAction(tr("Новая &вкладка"), this);
//    newTabAction->setShortcuts(QKeySequence::AddTab);
//    connect(newTabAction, &QAction::triggered, this, [this]() {
//        this->m_tabWidget->createTab();
//        this->m_urlLineEdit->setFocus();
//    });
//    fileMenu->addAction(newTabAction);

//    fileMenu->addAction(tr("&Открыть файл ..."), this, &BrowserWindow::handleFileOpenTriggered, QKeySequence::Open);
//    fileMenu->addSeparator();

//    QAction *closeTabAction = new QAction(tr("&Закрыть вкладку"), this);
//    closeTabAction->setShortcuts(QKeySequence::Close);
//    connect(closeTabAction, &QAction::triggered, [tabWidget]() {
//        tabWidget->closeTab(tabWidget->currentIndex());
//    });
//    fileMenu->addAction(closeTabAction);

//    QAction *closeAction = new QAction(tr("&Выход"),this);
//    closeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
//    connect(closeAction, &QAction::triggered, this, &QWidget::close);
//    fileMenu->addAction(closeAction);

//    connect(fileMenu, &QMenu::aboutToShow, [this, closeAction]() {
//        if (this->m_browser->windows().count() == 1)
//            closeAction->setText(tr("&Выход"));
//        else
//            closeAction->setText(tr("&Закрыть окно"));
//    });
//    return fileMenu;
//}

void BrowserWindow::handleFindActionTriggered()
{
    _log::log("BrowserWindow::handleFindActionTriggered()");
    //
    if (!currentTab())
        return;
    bool ok = false;
    QString search = QInputDialog::getText(this,
                                           tr("Поиск"),
                                           tr("Поиск:"), QLineEdit::Normal, this->m_lastSearch, &ok);
    if (ok && !search.isEmpty()) {
        this->m_lastSearch = search;
        currentTab()->findText(this->m_lastSearch, 0, [this](bool found) {
            //            if (!found)
            //                statusBar()->showMessage(tr("\"%1\" не найдено.").arg(this->m_lastSearch));
        });
    }
}

//QMenu *BrowserWindow::createEditMenu()
//{
//    QMenu *editMenu = new QMenu(tr("&Редактировать"));
//    QAction *findAction = editMenu->addAction(tr("&Найти"));
//    findAction->setShortcuts(QKeySequence::Find);
//    connect(findAction, &QAction::triggered, this, &BrowserWindow::handleFindActionTriggered);

//    QAction *findNextAction = editMenu->addAction(tr("Найти &след."));
//    findNextAction->setShortcut(QKeySequence::FindNext);
//    connect(findNextAction, &QAction::triggered, [this]() {
//        if (!currentTab() || this->m_lastSearch.isEmpty())
//            return;
//        currentTab()->findText(this->m_lastSearch);
//    });

//    QAction *findPreviousAction = editMenu->addAction(tr("Найти &пред."));
//    findPreviousAction->setShortcut(QKeySequence::FindPrevious);
//    connect(findPreviousAction, &QAction::triggered, [this]() {
//        if (!currentTab() || this->m_lastSearch.isEmpty())
//            return;
//        currentTab()->findText(this->m_lastSearch, QWebEnginePage::FindBackward);
//    });

//    return editMenu;
//}

//QMenu *BrowserWindow::createViewMenu(QToolBar *toolbar)
//{
//    QMenu *viewMenu = new QMenu(tr("&Просмотр"));
//    this->m_stopAction = viewMenu->addAction(tr("&Стоп"));
//    QList<QKeySequence> shortcuts;
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Period));
//    shortcuts.append(Qt::Key_Escape);
//    this->m_stopAction->setShortcuts(shortcuts);
//    connect(this->m_stopAction, &QAction::triggered, [this]() {
//        this->m_tabWidget->triggerWebPageAction(QWebEnginePage::Stop);
//    });

//    this->m_reloadAction = viewMenu->addAction(tr("Перезагрузить страницу"));
//    this->m_reloadAction->setShortcuts(QKeySequence::Refresh);
//    connect(this->m_reloadAction, &QAction::triggered, [this]() {
//        this->m_tabWidget->triggerWebPageAction(QWebEnginePage::Reload);
//    });

//    QAction *zoomIn = viewMenu->addAction(tr("&Увеличить"));
//    zoomIn->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Plus));
//    connect(zoomIn, &QAction::triggered, [this]() {
//        if (currentTab())
//            currentTab()->setZoomFactor(currentTab()->zoomFactor() + 0.1);
//    });

//    QAction *zoomOut = viewMenu->addAction(tr("Ум&еньшить"));
//    zoomOut->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus));
//    connect(zoomOut, &QAction::triggered, [this]() {
//        if (currentTab())
//            currentTab()->setZoomFactor(currentTab()->zoomFactor() - 0.1);
//    });

//    QAction *resetZoom = viewMenu->addAction(tr("Сбросить увеличение"));
//    resetZoom->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
//    connect(resetZoom, &QAction::triggered, [this]() {
//        if (currentTab())
//            currentTab()->setZoomFactor(1.0);
//    });

//    viewMenu->addSeparator();
//    QAction *viewToolbarAction = new QAction(tr("Спрятать панель инстр."),this);
//    viewToolbarAction->setShortcut(tr("Ctrl+|"));
//    connect(viewToolbarAction, &QAction::triggered, [toolbar,viewToolbarAction]() {
//        if (toolbar->isVisible()) {
//            viewToolbarAction->setText(tr("Показать панель инстр."));
//            toolbar->close();
//        } else {
//            viewToolbarAction->setText(tr("Спрятать панель инстр."));
//            toolbar->show();
//        }
//    });
//    viewMenu->addAction(viewToolbarAction);

//    QAction *viewStatusbarAction = new QAction(tr("Спрятать строку статуса."), this);
//    viewStatusbarAction->setShortcut(tr("Ctrl+/"));
//    connect(viewStatusbarAction, &QAction::triggered, [this, viewStatusbarAction]() {
//        if (statusBar()->isVisible()) {
//            viewStatusbarAction->setText(tr("Показать строку статуса."));
//            statusBar()->close();
//        } else {
//            viewStatusbarAction->setText(tr("Спрятать строку статуса."));
//            statusBar()->show();
//        }
//    });
//    viewMenu->addAction(viewStatusbarAction);
//    return viewMenu;
//}

//QMenu *BrowserWindow::createWindowMenu(TabWidget *tabWidget)
//{
//    QMenu *menu = new QMenu(tr("&Окно"));

//    QAction *nextTabAction = new QAction(tr("Показать след.вкладку"), this);
//    QList<QKeySequence> shortcuts;
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceRight));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageDown));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BracketRight));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Less));
//    nextTabAction->setShortcuts(shortcuts);
//    connect(nextTabAction, &QAction::triggered, tabWidget, &TabWidget::nextTab);

//    QAction *previousTabAction = new QAction(tr("Показать пред.вкладку"), this);
//    shortcuts.clear();
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BraceLeft));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_PageUp));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_BracketLeft));
//    shortcuts.append(QKeySequence(Qt::CTRL | Qt::Key_Greater));
//    previousTabAction->setShortcuts(shortcuts);
//    connect(previousTabAction, &QAction::triggered, tabWidget, &TabWidget::previousTab);

//    connect(menu, &QMenu::aboutToShow, [this, menu, nextTabAction, previousTabAction]() {
//        menu->clear();
//        menu->addAction(nextTabAction);
//        menu->addAction(previousTabAction);
//        menu->addSeparator();

//        QVector<BrowserWindow*> windows = this->m_browser->windows();
//        int index(-1);
//        for (auto window : windows) {
//            QAction *action = menu->addAction(window->windowTitle(), this, &BrowserWindow::handleShowWindowTriggered);
//            action->setData(++index);
//            action->setCheckable(true);
//            if (window == this)
//                action->setChecked(true);
//        }
//    });
//    return menu;
//}

//QMenu *BrowserWindow::createHelpMenu()
//{
//    QMenu *helpMenu = new QMenu(tr("&Помощь"));
//    QAction* hAction = helpMenu->addAction(tr("О &программе"));
//    connect(hAction,
//            SIGNAL(triggered()),
//            this,
//            SLOT(about()));
//    return helpMenu;
//}

void BrowserWindow::handleWebActionEnabledChanged(QWebEnginePage::WebAction action, bool enabled)
{
    _log::log("BrowserWindow::handleWebActionEnabledChanged()");
    //
    switch (action) {
    case QWebEnginePage::Back:
        this->m_historyBackAction->setEnabled(enabled);
        break;
    case QWebEnginePage::Forward:
        this->m_historyForwardAction->setEnabled(enabled);
        break;
    case QWebEnginePage::Reload:
        this->m_reloadAction->setEnabled(enabled);
        break;
    case QWebEnginePage::Stop:
        this->m_stopAction->setEnabled(enabled);
        break;
    default:
        qWarning() << "Неизвестный сигнал веб-приложения";
    }
}

void BrowserWindow::handleWebViewTitleChanged(const QString &title)
{
    _log::log("BrowserWindow::handleWebViewTitleChanged()");
    //
    QString suffix = this->m_profile->isOffTheRecord()
            ? tr("Библиотека (Приватное)")
            : tr("Библиотека ");
    if (title.isEmpty())
        setWindowTitle(suffix);
    else
        setWindowTitle(title + " - " + suffix);
}

void BrowserWindow::handleNewWindowTriggered()
{
    _log::log("BrowserWindow::handleNewWindowTriggered()");
    //
    //BrowserWindow *window = this->m_browser->createWindow(false);
    //    window->setWindowTitle(
    //                tr("ОБУЧАЮЩИЕ ПРОГРАММЫ МНСК"));
}

void BrowserWindow::handleNewIncognitoWindowTriggered()
{
    _log::log("BrowserWindow::handleNewIncognitoWindowTriggered()");
    //
    //BrowserWindow *window = this->m_browser->createWindow(/* offTheRecord: */ true);
    //    window->this->m_urlLineEdit->setFocus();
}

void BrowserWindow::closeEvent(QCloseEvent *event)
{
    //    _log::log("BrowserWindow::closeEvent(";
    //
    if (this->m_tabWidget->count() > 1) {
        int ret = QMessageBox::warning(this, "Подтверждение завершения",
                                       "Вы действительно хотите завершить работу?",
                                       QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
        if (ret != QMessageBox::Yes) {
            event->ignore();
            return;
        }
    }
    event->accept();
    QApplication::quit();
}

TabWidget *BrowserWindow::tabWidget() const
{
    return this->m_tabWidget;
}

WebView *BrowserWindow::currentTab() const
{
    return this->m_tabWidget->currentWebView();
}

void BrowserWindow::handleWebViewLoadProgress(int progress)
{
    _log::log("BrowserWindow::handleWebViewLoadProgress(");
    //
    if (0 < progress && progress < 100) {
        this->m_stopReloadAction->setData(QWebEnginePage::Stop);
        this->m_stopReloadAction->setIcon(
                    myIcon("process-stop"));
        this->m_stopReloadAction->setToolTip(tr("Остановить загрузку страницы"));
        //this->m_progressBar->setValue(progress);
    } else {
        this->m_stopReloadAction->setData(QWebEnginePage::Reload);
        this->m_stopReloadAction->setIcon(
                    myIcon("view-refresh"));
        this->m_stopReloadAction->setToolTip(tr("Перезагрузить страницу"));
        //this->m_progressBar->setValue(0);
    }
}

void BrowserWindow::handleShowWindowTriggered()
{
    _log::log("BrowserWindow::handleShowWindowTriggered(");
    //
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        int offset = action->data().toInt();
        QVector<BrowserWindow*> windows = this->m_browser->windows();
        windows.at(offset)->activateWindow();
        windows.at(offset)->currentTab()->setFocus();
    }
}

void BrowserWindow::handleDevToolsRequested(QWebEnginePage *source)
{
    _log::log("BrowserWindow::handleDevToolsRequested()");
    //
    //source->setDevToolsPage(this->m_browser->createDevToolsWindow()->currentTab()->page());
    source->triggerAction(QWebEnginePage::InspectElement);
}

void BrowserWindow::adjustTree1(const QModelIndex& index)
{
    this->m_treeView->resizeColumnToContents(0);
}

void BrowserWindow::adjustTree2(const QModelIndex& index)
{
    this->m_treeView2->resizeColumnToContents(0);
}

void BrowserWindow::findText(const QString& text)
{
    text_to_find_ = text;
    this->m_tabWidget->currentWebView()->findText(text_to_find_);
}

void BrowserWindow::findPrevious()
{
    this->m_tabWidget->currentWebView()->findText(text_to_find_, QWebEnginePage::FindBackward);
}

void BrowserWindow::findNext()
{
    this->m_tabWidget->currentWebView()->findText(text_to_find_);
}
