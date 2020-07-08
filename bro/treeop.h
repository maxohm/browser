#ifndef TREEOP_H
#define TREEOP_H
//
#include <QStandardItemModel>
#include <QTreeView>
#include <QTableWidget>
//
QStandardItemModel* json2mod(QByteArray data, QObject* parent, QString obj);
QModelIndex nextC0Index(QTreeView* _tree);
QModelIndex prevC0Index(QTreeView* _tree);
void obj2table(QTableWidget* t, QStringList l, QString s);
QString treeC0Text(QModelIndex i);
QString treeC1Text(QModelIndex i);
void treeFormat1(QTreeView* t);
//
// ******* flags 20190819 *******
const QFlags<Qt::ItemFlag> stdItemFlags1 = Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
// ******************************
//
QPalette welcome(QString wpic, QString wtext);
QPixmap myPixmap(QString iname);
QImage myImage(QString iname);
QIcon myIcon(QString iname);
//
QPoint currMousePos(QWidget* w);

class TreeOp
{
public:
    explicit TreeOp(QTreeView* t);
    ~TreeOp();
    //
    QAbstractItemModel* model = nullptr;
    QItemSelectionModel* selector = nullptr;
    QModelIndex index;
    QString src;
    QString url;
    QTreeView* tree = nullptr;
};

#endif // TREEOP_H
