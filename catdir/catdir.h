#ifndef CATDIR_H
#define CATDIR_H
//
#include <QDialog>

namespace Ui {
class clist;
}

class catdir : public QDialog
{
    Q_OBJECT

public:
    explicit catdir(QWidget *parent = 0);
    ~catdir();
    //
    bool init(QStringList p);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::clist *ui;
    //
    bool check(QStringList exclude);
    bool checkfile();
    void dump();
    void edit(QStringList exclude);
    //
    QAction* removeAction = nullptr;
    QString sumfile = "";

};

#endif // CATDIR_H
