#include "treeop.h"
//
#include <QFileIconProvider>
#include <QDebug>
//
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
//
#include "../libengines/libengines.h"
//
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QHeaderView>

QPoint currMousePos(QWidget* w){
    return QCursor::pos(
                QApplication::screens().value(
                    QApplication::desktop()->screenNumber(w)));
}

void obj2table(QTableWidget* t, QStringList l, QString s)
{
    if(nullptr==t)
        return;
    //
    int ls = l.size();
    _log::log("tableOp2() rows "+QString::number(
                  t->rowCount()));
    _log::log("tableOp2() list "+QString::number(ls));
    //
    int r=t->rowCount();
    while(r!=ls)
        if(r<ls) {
            t->insertRow(r);
            r++;
        } else {
            t->removeRow(r);
            r--;
        };
    //
    for(int i=0; i<ls; i++)
    {
        int j = l.value(i).count(s);
        if(0>=j)
            continue;
        //
        t->setItem(i,0,
                   new QTableWidgetItem(
                       QString::number(i+1)));
        //
        QStringList _l = l.value(i).split(s);
        while(0<=j){
            QTableWidgetItem* it = new QTableWidgetItem(
                        _l.value(j));
            it->setFlags(stdItemFlags1);
            t->setItem(i,j+1,it);
            j--;
        };
    };
    //
    t->resizeColumnsToContents();
}

QPixmap myPixmap(QString iname)
{
    _log::log("myPixmap() " + iname);
    //
    QPixmap pix = QPixmap(
                QStringLiteral(":")+iname);
    if (pix.isNull())
        return myPixmap("no");

    return pix;
}

QImage myImage(QString iname)
{
    _log::log("myIcon() " + iname);
    //
    QImage img = QImage(
                QStringLiteral(":")+iname);
    if (img.isNull())
        return QImage(
                    QStringLiteral(":no"));
    return img;
}

QIcon myIcon(QString iname)         // freedesktop unsupport workaround ...
{
    _log::log("myIcon() " + iname);
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

QPalette welcome(QString wpic, QString wtext){
    _log::log("welcome()\npic :: " + wpic + "\ntext :: " + wtext);
    // ********************************************
    // **** Welcome engine Ia 20190709  ***********
    // ********************************************
    QImage img = myImage(wpic);
    //
    QPainter p(&img);
    p.setPen(
                QPen(Qt::white));
    p.setFont(
                QFont("Times", 36, QFont::Bold));
    p.drawText(
                img.rect(), Qt::AlignLeft, wtext);
    //
    QPixmap bp = QPixmap::fromImage(img).scaled(
                QApplication::desktop()->screenGeometry().size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Background, bp);
    // ********************************************
    // ********************************************
    // ********************************************
    //    _log::log("welcome() data " + QString::number(
    //                    img.sizeInBytes()) + " bytes";
    return palette;
}

TreeOp::TreeOp(QTreeView* t)
{
    this->tree = t;
    //
    this->model = nullptr;
}

TreeOp::~TreeOp()
{
    delete this;
}

void treeFormat1(QTreeView* t){
    t->hideColumn(1);
    t->resizeColumnToContents(0);
    //
}

QString treeC0Text(QModelIndex i){
    int r = i.row();
    _log::log("treeC0Text() ["+QString::number(r)+",1]");
    //
    QString _sel = i.sibling(r,0).data().toString();
    _log::log("treeC0Text() SEL :: " + _sel);
    if (_sel.isEmpty() || _sel.isNull())
        _sel = "";
    return _sel;
}

QString treeC1Text(QModelIndex i){
    int r = i.row();
    _log::log("treeC1Text() ["+QString::number(r)+",1]");
    //
    QString _sel = i.sibling(r,1).data().toString();
    _log::log("treeC1Text() SEL :: " + _sel);
    if (_sel.isEmpty() || _sel.isNull())
        _sel = "";
    return _sel;
}

QModelIndex lastChild(QModelIndex i)
{
    _log::log("lastChild() ["+QString::number(i.row())+","+QString::number(i.column())+"]");
    int r=0;
    while(i.child(r,0).isValid())
        r++;
    return i.child(r-1,0);
}

QModelIndex prevC0Sibling(QModelIndex i)
{
    _log::log("prevC0Sibling() ["+QString::number(i.row())+","+QString::number(i.column())+"]");
    int r = i.row();
    if (!i.sibling(r-1,0).isValid())
        if (i.parent().isValid())
            return prevC0Sibling(i.parent());
    //
    if (i.sibling(r-1,0).isValid())
        return i.sibling(r-1,0);
    //
    return i;
}

QModelIndex prevC0Index(QModelIndex i)
{
    _log::log("prevC0Index() ["+QString::number(i.row())+","+QString::number(i.column())+"]");
    if (i.child(0,0).isValid())
        return lastChild(i);
    //
    return prevC0Sibling(i);
}

QModelIndex prevC0Index(QItemSelectionModel* m)
{
    return prevC0Index(m->currentIndex());
}

QModelIndex prevC0Index(QTreeView* _tree)
{
    return prevC0Index(_tree->selectionModel());
}

QModelIndex deepC0Sibling(QModelIndex i)
{
    _log::log("deepC0Sibling() ["+QString::number(i.row())+","+QString::number(i.column())+"]");
    int r = i.row();
    if (!i.sibling(r+1,0).isValid())
        if (i.parent().isValid())
            return deepC0Sibling(i.parent());
    //
    if (i.sibling(r+1,0).isValid())
        return i.sibling(r+1,0);
    //
    return i;
}

QModelIndex nextC0Index(QModelIndex i)
{
    _log::log("nextC0Index() ["+QString::number(i.row())+","+QString::number(i.column())+"]");
    QModelIndex _i = i.child(0,0);
    if(_i.isValid())
        return _i;
    //
    _i=deepC0Sibling(i);
    if(_i.isValid())
        return _i;
    //
    return i;
}

QModelIndex nextC0Index(QItemSelectionModel* m){
    return nextC0Index(m->currentIndex());
}

QModelIndex nextC0Index(QTreeView* _tree){
    return nextC0Index(_tree->selectionModel());
}

QStandardItem* findChildItem(QStandardItem* it, const QString &text){
    if(!it->hasChildren())
        return nullptr;
    for(int i=0; i< it->rowCount(); i++){
        if(it->child(i)->text() == text)
            return it->child(i);
    }
    return nullptr;
}

void appendToModel(QStandardItemModel* model, const QStringList &list, const QString &url, QString cobj){
    QStandardItem* parent = model->invisibleRootItem();
    QFileIconProvider provider;
    //
    for(QStringList::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        QStandardItem* item = findChildItem(parent, *it);
        if(item){
            parent = item;
            continue;
        }
        item = new QStandardItem(*it);
        // ******* flags 20190819 *******
        item->setFlags(stdItemFlags1);
        // ******************************
        item->setIcon(
                    provider.icon(QFileIconProvider::File));

        if(std::next(it) == list.end()){
            // ******* fav engine 20200318 ******
            if(utfchk(cobj, url))
                item->setIcon(
                            myIcon("bookmark-new"));
            // **********************************
            parent->appendRow({item, new QStandardItem(url)});
        }
        else{
            item->setIcon(
                        provider.icon(QFileIconProvider::Folder));
            parent->appendRow(item);
        }

        parent = item;
    }
}

void appendToModel(QStandardItemModel* model, const QStringList &list, const QString &url){
    QStandardItem* parent = model->invisibleRootItem();
    QFileIconProvider provider;
    //
    for(QStringList::const_iterator it = list.begin(); it != list.end(); ++it)
    {
        QStandardItem* item = findChildItem(parent, *it);
        if(item){
            parent = item;
            continue;
        }
        item = new QStandardItem(*it);
        // ******* flags 20190819 *******
        item->setFlags(stdItemFlags1);
        // ******************************
        item->setIcon(
                    provider.icon(QFileIconProvider::File));

        if(std::next(it) == list.end()){
            parent->appendRow({item, new QStandardItem(url)});
        }
        else{
            item->setIcon(
                        provider.icon(QFileIconProvider::Folder));
            parent->appendRow(item);
        }

        parent = item;
    }
}

QStandardItemModel* json2mod(QByteArray data, QObject* parent, QString cobj){
    _log::log("json2mod() " + QString::number(
                  data.size()) + " bytes");
    // ********************************************
    // ****** json2model engine Ib 20190906  ******
    // ********************************************
    QJsonParseError parse;
    QJsonDocument const& jdoc =  QJsonDocument::fromJson(data, &parse);
    _log::log("json2mod() " + parse.errorString());
    QStandardItemModel* m = new QStandardItemModel(parent);
    if(QJsonParseError::NoError!=parse.error)
        return nullptr;
    //
    //Q_ASSERT(parse.error == QJsonParseError::NoError);
    if(jdoc.isArray()){
        for(const QJsonValue &element : jdoc.array()){
            QJsonObject obj = element.toObject();
            QString name = obj["name"].toString();
            QString url = obj["url"].toString();
            appendToModel(m,
                          name.split("/", QString::SkipEmptyParts),url, cobj);
        };
    };
    return m;
    // ********************************************
    // ********************************************
    // ********************************************

}

QStandardItemModel* json2mod(QByteArray data, QObject* parent){

    _log::log("json2mod() " + QString::number(
                  data.size()) + " bytes");
    // ********************************************
    // ****** json2model engine Ib 20190906  ******
    // ********************************************
    QJsonParseError parse;
    QJsonDocument const& jdoc =  QJsonDocument::fromJson(data, &parse);
    _log::log("json2mod() " + parse.errorString());
    QStandardItemModel* m = new QStandardItemModel(parent);
    if(QJsonParseError::NoError!=parse.error)
        return nullptr;
    //
    //Q_ASSERT(parse.error == QJsonParseError::NoError);
    if(jdoc.isArray()){
        for(const QJsonValue &element : jdoc.array()){
            QJsonObject obj = element.toObject();
            QString name = obj["name"].toString();
            QString url = obj["url"].toString();
            appendToModel(m,
                          name.split("/", QString::SkipEmptyParts),url);
        };
    };
    return m;
    // ********************************************
    // ********************************************
    // ********************************************
}
