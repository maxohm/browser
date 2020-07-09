#include "catdir.h"
#include "ui_clist.h"
//
#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QElapsedTimer>
//
#include "../libengines/libengines.h"

QIcon myIcon(QString iname)         // freedesktop unsupport workaround ...
{
    _log::_log::log("myIcon() " + iname);
    //
    QIcon icon = QIcon::fromTheme(iname);
    if (icon.isNull())
        icon = QIcon(
                    QStringLiteral(":")+iname);
    if (icon.isNull())
        return QIcon(
                    QStringLiteral(":")+"no");
    return icon;
}

catdir::catdir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::clist)
{
    this->ui->setupUi(this);
}

catdir::~catdir()
{
    delete ui;
}

bool catdir::checkfile()
{
    if (""==this->sumfile)
        return false;
    if (this->sumfile.isEmpty())
        return false;
    //
    return true;
}

void catdir::dump(){
    _log::log("catdir::dump()");
    int rc = this->ui->log->rowCount();
    if(1>rc)
        return;
    //
    QString sums = "";
    while(rc>0)
    {
        rc--;
        if(nullptr!=this->ui->log->item(rc,1))
            if(""!=this->ui->log->item(rc,1)->text())
                sums+=this->ui->log->item(rc,1)->text()+this->ui->log->item(rc,2)->text();
    };
    _log::log("catdir::dump() "+QString::number(
                  sums.size())+" symbols");
    _log::log("sums = "+sums);
    utfy(this->sumfile,sums);
}

bool catdir::check(QStringList ex) // CATDir MODE 2
{
    // ***** catdir mode2 20200131 ********
    _log::log("catdir::check() "+this->sumfile);
    return md5chks(this->sumfile, ex);
    // ************************************
}

void catdir::edit(QStringList ex) // CATDir MODE 1
{
    _log::log("catdir::edit() "+this->sumfile);
    // ************************************
    // ***** catdir mode1 20200131 ********
    // ************************************
    QStringList p;
    QElapsedTimer timer;
    timer.start();
    qint64 nanoSec = timer.nsecsElapsed();
    if(0<ex.size()){
        p = md5sums(
                    objdir(this->sumfile),ex);
    } else {
        p = md5sums(
                    objdir(this->sumfile));
    };
    //
    qDebug() << "catdir::edit() "+
                QString::number((
                                    timer.nsecsElapsed()-nanoSec)/1000000)+"ms.";
    //
    QString _wd = cdUp(
                objdir(this->sumfile));
    //
    connect(
                this->ui->log->model(),
                SIGNAL(
                    rowsInserted(const QModelIndex&, int, int)), this->ui->log,
                SLOT(
                    scrollToBottom()));
    //
    int i = p.size();
    while(i>0){
        i--;
        QString v = p.at(i);
        if(32>v.size())
            continue;
        if(0<v.indexOf(".cat"))
            continue;
        //
        QString s = v.left(32);
        v = v.replace(s,"");
        QString _s = md5sum(
                    objname(v)
                    .toUtf8());
        int r = this->ui->log->rowCount();
        this->ui->log->insertRow(r);
        this->ui->log->setItem(r,0,
                               new QTableWidgetItem(
                                   v.replace(_wd,"")));
        this->ui->log->setItem(r,1,
                               new QTableWidgetItem(s));
        this->ui->log->setItem(r,2,
                               new QTableWidgetItem(_s));
    };
    //
    this->ui->log->resizeColumnsToContents();
    this->ui->log->resizeRowsToContents();
    this->ui->log->hideColumn(2);
    //
    this->removeAction = new QAction(this);
    this->removeAction->setShortcut(Qt::CTRL+Qt::Key_Delete);
    connect(this->removeAction, &QAction::triggered, [this]
            () {
        if(!this->ui->log->hasFocus())
            return;
        //
        if(!this->checkfile())
            return;
        //
        this->ui->log->removeRow(
                    this->ui->log->currentRow());
        //this->dump();
    });
    this->addAction(this->removeAction);
    // ************************************
    // ************************************
    // ************************************
    this->setToolTip(this->sumfile);
    this->setWindowTitle(this->sumfile);
    this->setWindowIcon(
                myIcon("catdir"));
    this->show();
    //this->dump();
}

void catdir::closeEvent(QCloseEvent *event)
{
    _log::log("catdir::closeEvent");
    //
    this->dump();
    //
    event->accept();
    QApplication::quit();
}


bool catdir::init(QStringList p)
{
    _log::log("catdir::init() "
              +p.join(" "));
    //
    QString arg0 = p.at(0);
    p.removeAt(0);
    //this->ui->log->setHorizontalHeader();
    // ******************************
    // *** cat engine III 20200130 **
    // ******************************
    this->sumfile = objdir(arg0)
            +QDir::separator()
            +objname(arg0)+".cat";
    //
    _log::log("catdir::init() "+this->sumfile);
    QFile f(this->sumfile);
    //
    if (f.exists())
    {
        _log::log("catdir::init() 1");
        if(!this->check(p))
            qDebug() << "catdir::init() 3 fail ...";
        return false;
    } else {
        _log::log("catdir::init() 2");
        objchk(this->sumfile);
        _log::log("catdir::init() 3");
        this->edit(p);
    };
    // ******************************
    // ******************************
    // ******************************
    return true;
}
