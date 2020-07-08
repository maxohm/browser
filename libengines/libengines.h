#ifndef LIBENGINES_H
#define LIBENGINES_H
//
#include <QDir>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTranslator>
//
#include "libengines_global.h"
#include "logger.h"
//B
LIBENGINESSHARED_EXPORT bool                        basechk(QString f, QString s);
LIBENGINESSHARED_EXPORT bool                        basefy(QString f, QString s);
LIBENGINESSHARED_EXPORT bool                        baserm(QString f, QString s);
LIBENGINESSHARED_EXPORT QStringList*                biblio(QString wd, QString index);
//C
LIBENGINESSHARED_EXPORT QString                     cdUp(QString wd);
LIBENGINESSHARED_EXPORT QString                     cfg2str(QSettings* cfg, QString p);
//D
LIBENGINESSHARED_EXPORT QByteArray                  dir2data(QString wd, QString stuff);
QStringList*                                        dirstuff(QString wd, QDir::Filters objtype, QString obj);
//F
QByteArray                                          format1(QByteArray data);
//L
class LIBENGINESSHARED_EXPORT                       _log;
//M
LIBENGINESSHARED_EXPORT bool                        md5chks();
LIBENGINESSHARED_EXPORT bool                        md5chks(QString sumf);
LIBENGINESSHARED_EXPORT bool                        md5chks(QString sumf, QString ex);
LIBENGINESSHARED_EXPORT bool                        md5chks(QString sumf, QStringList ex);
LIBENGINESSHARED_EXPORT QString                     md5sum(QString obj);
LIBENGINESSHARED_EXPORT QStringList                 md5sums(QString wd);
LIBENGINESSHARED_EXPORT QStringList                 md5sums(QString wd, QString ex);
LIBENGINESSHARED_EXPORT QStringList                 md5sums(QString wd, QStringList ex);
//O
LIBENGINESSHARED_EXPORT QStringList                 objdata(QString obj, QString s);
LIBENGINESSHARED_EXPORT QString                     objdir(QString url);
LIBENGINESSHARED_EXPORT QString                     objchk(QString obj);
QString                                             objlist(QString wd);
LIBENGINESSHARED_EXPORT QString                     objname(QString url);
LIBENGINESSHARED_EXPORT QString                     objpath(QString wd, QString url);
LIBENGINESSHARED_EXPORT QString*                    objrel1(QString wd, QString obj);
//R
LIBENGINESSHARED_EXPORT QHash<QString, QString>*    rhash1(QString rpath, QString rs, QString re);
//S
//T
LIBENGINESSHARED_EXPORT QList<QTranslator*>         transinst(QString wd, QObject* parent = nullptr);
//U
LIBENGINESSHARED_EXPORT bool                        utfchk(QString f, QString s);
LIBENGINESSHARED_EXPORT bool                        utfrm(QString f, QString s);
LIBENGINESSHARED_EXPORT bool                        utfy(QString f, QString s);
LIBENGINESSHARED_EXPORT bool                        utfy(QString f, QStringList l, QString s);

#endif // LIBENGINES_H
