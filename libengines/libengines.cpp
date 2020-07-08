#include "libengines.h"
//
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QTextCodec>
//
// ******* flags 20190919 *******
const QDir::Filters stdDirFilters1 = QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable;
const QDir::Filters stdDirFilters2 = QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable;
// ******************************

//
// As nouns the difference between directory and catalogue
// is that directory is a list of names, addresses etc, of specific classes of people or organizations,
// often in alphabetical order or in some classification while
// catalogue is a systematic list of names, books, pictures etc.
//

bool basechk(QString f, QString s){
    _log::log("basechk() " + QString::number(
                  s.size()) + " symbols");
    //
    QFile _f(f);
    if(!_f.open(QIODevice::ReadOnly))
        return false;
    QByteArray _data = _f.readAll();
    _f.close();
    if(0<=_data.indexOf(
                s.toUtf8()
                .toBase64(QByteArray::OmitTrailingEquals)))
        return true;
    //
    return false;
}

bool basefy(QString f, QString s){
    _log::log("basefy() " + QString::number(
                  s.size()) + " symbols");
    QFile* jfile = new QFile(f);
    bool op = jfile->open(QIODevice::Append);
    if(!op)
        return false;
    //
    //****** basefy engine 20191129 *******
    QByteArray data = s.toUtf8().toBase64(QByteArray::OmitTrailingEquals);
    jfile->write(data);
    //    ,
    //                 data.size());
    //jfile->flush();

    //    if(op){
    //        QTextStream jstream(jfile);
    //        jstream.setGenerateByteOrderMark(false);
    //        jstream << data;
    //        jstream.flush();
    //    };
    //*************************************
    //
    _log::log("basefy() out " + QString::number(
                  data.size()) + " bytes");
    jfile->close();
    delete jfile;
    return op;
}

bool baserm(QString f, QString s){
    _log::log("baserm() " + QString::number(
                  s.size()) + " symbols");
    //
    if(!basechk(f,s))
        return false;
    //
    QFile _f(f);
    if(!_f.open(QIODevice::ReadOnly))
        return false;
    QByteArray _data = _f.readAll();
    _f.close();
    //
    _data.replace(
                s.toUtf8()
                .toBase64(QByteArray::OmitTrailingEquals),
                QByteArray());
    if(_f.open(QIODevice::WriteOnly))
        _f.write(_data);
    _f.close();
    //
    return false;
}

QStringList* biblio(QString wd, QString index){
    _log::log("biblio() " + wd + " :: " + index);
    // ********************************************
    // ****Library engine II 20190704  ************
    // ********************************************
    QStringList* _lib = dirstuff(wd,stdDirFilters2,"*");
    if(nullptr!=_lib)
        for (int i=0; i<_lib->size(); i++)
            _lib->replace(i,
                          objpath(
                              _lib->at(i),index));
    return _lib;
    // ********************************************
    // ********************************************
    // ********************************************
}

QString cdUp(QString wd){
    _log::log("cdUp() "+wd);
    // *** cdUp 20191203 ***
    QDir::setCurrent(wd);
    QDir* _wd = new QDir(wd);
    if(_wd->cdUp())
        //        if(!_wd->isRoot())
        return
                QDir::toNativeSeparators(
                    _wd->path());
    // *********************
    return
            QDir::toNativeSeparators(wd);
}

QString cfg2str(QSettings* cfg, QString p){
    _log::log("cfg2str() "+cfg->fileName()+" :: "+p);
    QTextCodec* tc = QTextCodec::codecForName("UTF-8");
    cfg->setIniCodec(tc);
    // *** string value from settings 20200317 *********
#ifdef Q_WS_WIN32
    //_log::log("cfg2str() WIN32 :: " + p + "=" + v.toLatin1();
    //return cfg->value(p,"").toString().toLatin1();
#endif
    //
#ifdef Q_OS_WIN32
    //QTextCodec* tc = QTextCodec::codecForName("UTF-8");
    //_log::log("cfg2str() WIN32 :: " + p + "=" + v.toLocal8Bit();
    //return cfg->value(p,"").toString().toLatin1();
    //    v=tc->toUnicode(
    //                v.toLatin1());
#endif
    //
#ifdef Q_OS_LINUX
    //_log::log("cfg2str() LINUX :: " + p + "=" + cfg->value(p,"").toString().toUtf8();
//    v=tc->toUnicode(
//                v.toUtf8());
#endif
    QString v = cfg->value(p,"").toString();
    //**************************************************

    //    log( tc->fromUnicode(
    //                    cfg->value(p,"").toByteArray());
    // log( QString::fromUtf8(cfg->value(p,"").toString());
    //v = QString::fromUtf8(cfg->value(p,"").);
    _log::log("cfg2str() got "
              +QString::number(
                  v.size())+" symbols");
    return v;
}

QString objchk(QString wd, QString objname)
{
    _log::log("objchk() " + objname +" in "+ wd);
    // ***********************************************
    // ** objchk canonical engine I 20191121 *********
    // ***********************************************
    _log::log("objchk() "+wd+" "+objname);
    QFileInfo* fi = new QFileInfo(wd);
    if (!fi->isWritable())
        return "";
    //
    QString obj = wd+QDir::separator()+objname;
    return objchk(obj);
    // ***********************************************
    // ***********************************************
    // ***********************************************
}

QByteArray dir2data(QString wd, QString stuff){
    _log::log("dir2data() " + wd+" "+stuff);
    QStringList* _obj = dirstuff(wd, stdDirFilters1, stuff);
    _log::log("dir2data() " + QString::number(_obj->size()));
    //***** data join by suffix 20190718 *****
    QByteArray data;
    if(nullptr==_obj)
        return data;
    QFile* jfile;
    for (int i=0; i<_obj->size(); i++)
    {
        jfile = new QFile(
                    _obj->at(i));
        if (!jfile->open(QFile::ReadOnly))
            continue;
        _log::log("dir2data() append " + _obj->at(i));
        data.append(
                    jfile->readAll());
        jfile->close();
    };
    //****************************************
    data = QString(
                format1(data).
                replace("\n]\n[\n",",")).
            toUtf8();
    _log::log("dir2data() returned " + QString::number(
                  data.size()) + " symbols");
    return data;
}

QStringList* dirstuff(QString wd, QDir::Filters objtype, QString obj)
{
    _log::log("dirstuff() "+wd+" "+obj);
    // ********************************************
    // **** List engine II canonical 20190919  ****
    // ********************************************
    QDir* _wd = new QDir(wd,obj,QDir::Name,objtype);
    QStringList* result = new QStringList(
                _wd->entryList());
    //
    for (int i=0; i<result->size(); i++)
        result->replace(i,
                        objpath(wd,result->at(i)));
    // ********************************************
    // ********************************************
    // ********************************************
    return result;
}

//QPalette welcome(QString wpic){
//    _log::log("welcome() " + wpic);
//    // ********************************************
//    // **** Welcome engine I 20190604  ************
//    // ********************************************
//    QPixmap bp = myPixmap(wpic).scaled(
//                QApplication::desktop()->screenGeometry().size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    QPalette palette;
//    palette.setBrush(QPalette::Background, bp);
//    // ********************************************
//    // ********************************************
//    // ********************************************
//    return palette;
//}

QByteArray format1(QByteArray data){
    _log::log("format1() " + QString::number(
                  data.size()) + " bytes");
    //
    //********** data strip 20190718 **********
    QByteArray _data=data.replace("\n ","\n").
            replace("\n\n","\n").
            replace("  ","").
            replace("\t","");
    //*****************************************
    _log::log("format1() returned " + QString::number(
                  _data.size()) + " bytes");
    return _data;
}

//QStandardItemModel* json2mod(QString index){
//    _log::log("json2mod() " + index;
//    // ********************************************
//    // ****** json2model engine Ib 20190716  ******
//    // ********************************************
//    QFile jfile(index);
//    if (!jfile.open(QFile::ReadOnly))
//        return new QStandardItemModel();
//    //
//    QByteArray data = jfile.readAll();
//    jfile.close();
//    _log::log("json2mod() processed " + QString::number(
//                    data.size()) + " bytes";
//    return json2mod(data);
//    // ********************************************
//    // ********************************************
//    // ********************************************
//}

bool md5chk(QString obj, QString sum){
    _log::log("md5chk() "+sum+" "+obj);
    QString s = md5sum(obj);
    //
    QString _sum = sum;
    int ps=0;
    if (""!=s)
        for(int i=0; i<s.size(); i+=2){
            ps = _sum.indexOf(
                        s.at(i));
            if(0>ps)
                return false;
            _sum = sum.right(
                        sum.length()-ps);
        };
    return true;
}

bool md5chks()
{
    _log::log("md5chks()");
    QString arg0 = QCoreApplication::arguments().at(0);
    QString cwd = objdir(arg0);
    QString cat = objpath(cwd,
                          objname(arg0)+".cat");
    return md5chks(cat);
}

bool md5chks(QString sumf){
    _log::log("md5chks() "+sumf);
    //
    QFile f(sumf);
    if(!f.open(QIODevice::ReadOnly))
        return false;
    QString _sums = QString::fromUtf8(
                QByteArray::fromBase64(
                    f.readAll(),QByteArray::OmitTrailingEquals));
    f.close();
    //
    //*** md5 checkS engine 20191203 ****
    qDebug() << cdUp(
                    objdir(sumf));
    return true;

    QStringList p = md5sums(
                cdUp(
                    objdir(sumf)));
    return true;
    int i = p.size();
    while(i>0)
    {
        i--;
        QString v = p.at(i);
        QString s = v.left(32);
        _sums = _sums.replace(s, "");
        //qDebug() << " _sums = " + _sums;
        v = v.replace(s,"");
        s = md5sum(
                    objname(v)
                    .toUtf8());
        _sums = _sums.replace(s,"");
    };
    //**********************************
    //
    if(""!=_sums)
        return false;
    //
    qDebug() << "md5chks() ok";
    return true;
}

bool md5chks(QString sumf, QString ex){
    _log::log("md5chks() "+sumf+" exclude "+ex);
    //
    QFile f(sumf);
    if(!f.open(QIODevice::ReadOnly))
        return false;
    QString _sums = QString::fromUtf8(
                QByteArray::fromBase64(
                    f.readAll(),QByteArray::OmitTrailingEquals));
    f.close();
    //
    //*** md5 checkS engine 20191203 ****
    QStringList p = md5sums(
                cdUp(
                    objdir(sumf)),ex);
    int i = p.size();
    while(i>0)
    {
        i--;
        QString v = p.at(i);
        QString s = v.left(32);
        _sums = _sums.replace(s, "");
        //qDebug() << " _sums = " + _sums;
        v = v.replace(s,"");
        s = md5sum(
                    objname(v)
                    .toUtf8());
        _sums = _sums.replace(s,"");
    };
    //**********************************
    //
    if(""!=_sums)
        return false;
    //
    qDebug() << "md5chks() ok";
    return true;
}

bool md5chks(QString sumf, QStringList ex){
    _log::log("md5chks() "+sumf+" exclude "+ex.join(" "));
    //
    QFile f(sumf);
    if(!f.open(QIODevice::ReadOnly))
        return false;
    QString _sums = QString::fromUtf8(
                QByteArray::fromBase64(
                    f.readAll(),QByteArray::OmitTrailingEquals));
    f.close();
    //
    //*** md5 checkS engine 20191203 ****
    QStringList p = md5sums(
                cdUp(
                    objdir(sumf)),ex);
    int i = p.size();
    while(i>0)
    {
        i--;
        QString v = p.at(i);
        QString s = v.left(32);
        _sums = _sums.replace(s, "");
        //qDebug() << " _sums = " + _sums;
        v = v.replace(s,"");
        s = md5sum(
                    objname(v)
                    .toUtf8());
        _sums = _sums.replace(s,"");
    };
    //**********************************
    //
    if(""!=_sums)
        return false;
    //
    qDebug() << "md5chks() ok";
    return true;
}

QString md5sum(QByteArray data){
    _log::log("md5sum() "
              +QString::number(
                  data.size())+" bytes");
    //***** md5sum engine 20191122 *****
    QCryptographicHash c(QCryptographicHash::Md5);
    QString sum = c.hash(data,QCryptographicHash::Md5)
            .toHex(0);
    //**********************************
    return sum;
}

QString md5sum(QString obj){
    _log::log("md5sum() "+obj);
    //***** md5sum engine 20191122 *****
    if(1>obj.size())
        return "";
    QByteArray data = obj.toUtf8();
    QFile f(obj);
    if(f.open(QIODevice::ReadOnly))
        data = f.readAll();
    QString sum = md5sum(data);
    //**********************************
    f.close();
    return sum;
}

QStringList md5sums(QString wd){
    QString _wd = cdUp(wd);
    _log::log("md5sums "+_wd);
    //
    QStringList _p;
    _p.clear();
    //***** md5sums engine 20191203 *****
    QDirIterator it(_wd, stdDirFilters1, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString v = QDir::toNativeSeparators(
                    it.next());
        _p.push_back(
                    md5sum(v)+v);
    }
    //***********************************
    return _p;
}

//QStringList md5sums(QString wd, QString ex){
//    QString _wd = cdUp(wd);
//    _log::log("md5sums() "+_wd);
//    QStringList p = objlist(_wd)
//            .replace("||","|")
//            .split("|");
//    QElapsedTimer timer;
//    qint64 nanoSec;
//    timer.start();
//    nanoSec = timer.nsecsElapsed();

//    //***** md5sums engine 20191125 *****
//    QStringList _p;
//    //
//    int i = p.size();
//    while(i>0)
//    {
//        i--;
//        QString v = p.at(i);
//        if(v.isEmpty())
//            continue;
//        if(0<v.indexOf(ex))
//            continue;
//        if(""!=v)
//            v = md5sum(
//                        p.at(i));
//        //
//        _p.push_back(v+p[i]);
//    };
//    //***********************************
//    _log::log("md5sums() "+
//        QString::number((timer.nsecsElapsed()-nanoSec)/1000000)+"ms.");
//    return _p;
//}

QStringList md5sums(QString wd, QString ex)
{
    _log::log("md5sums "+wd);
    //
    QStringList _p;
    _p.clear();
    //***** md5sums engine 20200130 *****
    QDirIterator it(wd, stdDirFilters1, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString v = QDir::toNativeSeparators(
                    it.next());
        if (v.contains(ex
                       +QDir::separator()))
            continue;
        _p.push_back(
                    md5sum(v)+v);
    }
    //***********************************
    return _p;
}

QStringList md5sums(QString wd, QStringList ex)
{
    _log::log("md5sums "+wd);
    //
    QStringList _p;
    _p.clear();
    QStringListIterator l(ex);
    //***** md5sums engine 20200130 w/exlusions *********
    QDirIterator it(wd, stdDirFilters1, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString v = QDir::toNativeSeparators(
                    it.next());
        //
        bool f=false;
        l.toFront();
        while(!f && l.hasNext())
            if(!v.contains(
                        l.next()+QDir::separator()))
                f=true;
        //
        if(!f)
            _p.push_back(
                        md5sum(v)+v);
    };
    //***************************************************
    return _p;
}

//QIcon myIcon(QString iname)         // freedesktop unsupport workaround ...
//{
//    _log::log("myIcon() " + iname);
//    //
//    QIcon icon = QIcon::fromTheme(iname);
//    if (icon.isNull())
//        icon = QIcon(
//                    QStringLiteral(":")+iname);
//    if (icon.isNull())
//        return QIcon(
//                    QStringLiteral(":")+"no");
//    return icon;
//}

//QImage myImage(QString iname)
//{
//    _log::log("myIcon() " + iname);
//    //
//    QImage img = QImage(
//                QStringLiteral(":")+iname);
//    if (img.isNull())
//        return QImage(
//                    QStringLiteral(":no"));
//    return img;
//}


//QPixmap myPixmap(QString iname)
//{
//    _log::log("myPixmap() " + iname);
//    //
//    QPixmap pix = QPixmap(
//                QStringLiteral(":")+iname);
//    if (pix.isNull())
//        return myPixmap("no");
//    return pix;
//}

QString objdir(char* arg){
    return objdir(
                QString(arg));
}

QString objdir(QString arg){
    _log::log("objdir() " + arg);
    // ********************************************
    // *** objdir canonical engine II 20190916  ***
    // ********************************************
    QFileInfo* fi = new QFileInfo(arg);
    QString result = "";
    if (fi->isReadable())
        result = QDir::toNativeSeparators(
                    fi->canonicalPath());
    // ********************************************
    // ********************************************
    // ********************************************
    _log::log("objdir() returned "+result);
    return result;
}

QString objchk(QString obj)
{
    _log::log("objchk() "+obj);
    // ***********************************************
    // ** objchk canonical engine I 20200312 *********
    // ***********************************************
    QFile* f = new QFile(obj);
    if(!f->open(QIODevice::ReadWrite))
        return "";
    f->flush();
    f->close();
    //
    QFileInfo* fi = new QFileInfo(obj);
    if (fi->isWritable())
        return QDir::toNativeSeparators(
                    fi->canonicalFilePath());
    return "";
    // ***********************************************
    // ***********************************************
    // ***********************************************
}


QByteArray objdata(QString obj){
    _log::log("objdata() "+obj);
    // ** object data engine I 20200317 **************
    QFile* f = new QFile(obj);
    QByteArray b;
    if(f->open(QIODevice::ReadOnly))
         b=f->readAll();
    f->close();
    // ***********************************************
    return b;
}

QStringList objdata(QString obj, QString s){
    return QString::fromUtf8(
                objdata(obj)).split(s);
}

QString objlist(QString wd){
    // ************************************************
    // **** relative objects finder 20191119  *********
    // ************************************************
    _log::log("objlist() " + wd);
    QString s="";
    int i=0;
    //
    QDir* _wd = new QDir(wd,"",QDir::Name,stdDirFilters2);
    i=_wd->entryInfoList().size();
    while(i>0)
    {
        i--;
        s+="|"+objlist(
                    QDir::toNativeSeparators(
                        _wd->entryInfoList().at(i).canonicalFilePath()));
    };
    //
    QDir* _wf = new QDir(wd,"",QDir::Name,stdDirFilters1);
    i=_wf->entryInfoList().size();
    while(i>0)
    {
        i--;
        s+="|"+QDir::toNativeSeparators(
                    _wf->entryInfoList().at(i).canonicalFilePath());
    };
    return s;
    // ************************************************
    // ************************************************
    // ************************************************
}

QString objlist(QString wd, QString ex){
    // ************************************************
    // **** relative objects finder 20191119  *********
    // ************************************************
    _log::log("objlist() " + wd +" exclude "+ex);

    //    QDirIterator it(wd, stdDirFilters1, QDirIterator::Subdirectories);
    //    while (it.hasNext()) {
    //        QFile f(it.next());
    //        f.open(QIODevice::ReadOnly);
    //        qDebug() << f.fileName() << f.readAll().trimmed().toDouble() / 1000 << "MHz";
    //    }


    QString s="";
    int i=0;
    //
    QDir* _wd = new QDir(wd,"",QDir::Name,stdDirFilters2);
    i=_wd->entryInfoList().size();
    while(i>0)
    {
        i--;
        s+="|"+objlist(
                    QDir::toNativeSeparators(
                        _wd->entryInfoList().at(i).canonicalFilePath()));
    };
    //
    QDir* _wf = new QDir(wd,"",QDir::Name,stdDirFilters1);
    i=_wf->entryInfoList().size();
    while(i>0)
    {
        i--;
        s+="|"+QDir::toNativeSeparators(
                    _wf->entryInfoList().at(i).canonicalFilePath());
    };
    return s;
    // ************************************************
    // ************************************************
    // ************************************************
}

QString objname(char* arg){
    _log::log("objname() " + QString(arg));
    return objname(
                QString(arg));
}

QString objname(QString arg){
    _log::log("objname() " + arg);
    // ********************************************
    // *** objname complete engine II 20190916  ***
    // ********************************************
    QFileInfo* fi = new QFileInfo(arg);
    QString result = "";
    if (fi->isReadable())
        result = fi->baseName();
    // ********************************************
    // ********************************************
    // ********************************************
    _log::log("objname() returned "+result);
    return result;
}

QString objpath(QString wd, QString relpath){
    _log::log("objpath() "+wd+" "+relpath);
    // ***********************************************
    // ** objpath canonical engine II 20190916 *******
    // ***********************************************
    QFileInfo* fi = new QFileInfo(wd, relpath);
    QString result = "";
    if (fi->isReadable())
        result = QDir::toNativeSeparators(
                    fi->canonicalFilePath());
    _log::log("objpath() returned "+result);
    return result;
    // ***********************************************
    // ***********************************************
    // ***********************************************
}

QString* objrel1(QString wd, QString obj){
    _log::log("objrel1() " + obj + " in " + wd);
    // ************************************************
    // **** relative object path finder Ia 20190919  **
    // ************************************************
    QDir* _wd = new QDir(wd,obj,QDir::Name,stdDirFilters1);
    if(_wd->isRoot())
        return nullptr;
    if (0<_wd->entryInfoList().size())
        return new QString(
                    QDir::toNativeSeparators(
                        _wd->entryInfoList().at(0).canonicalFilePath()));
    return objrel1(
                cdUp(
                    _wd->absolutePath()),obj);
    // ************************************************
    // ************************************************
    // ************************************************
}

QStringList* rfilter(QString* _rx, QString ps, QString pe){
    _log::log("rfilter() PS :: "+ps+" PE :: "+pe);
    //
    QStringList* flist = new QStringList();
    if (_rx->isNull() || _rx->isEmpty() || 0>=_rx->size()) return nullptr;
    //
    // ************************************************************
    // ***************** filter engine 20190911 *******************
    // ************************************************************
    //
    int _pst = 0;
    int _pse = 1;
    int _psl;
    //
    int i = _rx->count(ps);
    //
    while (0<i) {
        _pse = _rx->lastIndexOf(pe);
        _pst = _rx->lastIndexOf(ps);
        _psl = _pse-_pst+1;
        //
        flist->push_front(
                    _rx->mid(_pst,_psl-1)
                    .replace(ps,""));
        //                    );
        _rx->truncate(_pst);
        i--;
    };
    //
    // ************************************************************
    // ************************************************************
    // ************************************************************
    return flist;
}

QHash<QString, QString>* rhash1(QString rpath, QString rs, QString re){
    _log::log("rhash1() " + rpath);
    // ***************** reduc hash engine 20190912 ***************
    QFile* r = new QFile(rpath);
    if (!r->open(QIODevice::ReadOnly))
        return nullptr;
    //
    QByteArray* rdata = new QByteArray(
                r->readAll());
    QString* rstring = new QString(
                QString::fromUtf8(
                    rdata->data()));

    QStringList* rlist = rfilter(rstring, rs, re);

    QHash<QString, QString>* rhash = new QHash<QString, QString>();
    for(int i=0; i<rlist->length()-1; i+=2)
    {
        rhash->insert(
                    rlist->at(i),
                    rlist->at(i+1));
        //
        _log::log("RLIST :: VALUE AT ["+rlist->at(i)+"] = ["+rhash->value(
                      rlist->at(i))+"]");
    };
    //
    return rhash;
    // ************************************************************
}

QList<QTranslator*> transinst(QString wd, QObject* parent){
    _log::log("transinst() "+wd);
    //
    QString obj = objname(
                QCoreApplication::arguments().at(0));
    // ***********************************************
    // **  Translation engine Ia 20190919  ***********
    // ***********************************************
    QStringList* _trs = dirstuff(wd,stdDirFilters1,obj+"*.qm");
    QList<QTranslator*> _tr;
    if(nullptr!=_trs)
        for (int i=0; i<_trs->size(); i++){
            QTranslator* qtr = new QTranslator(parent);
            if(!qtr->load(
                        _trs->at(i)))
                continue;
            _tr.push_back(qtr);
            _log::log("transinst() loaded " + _trs->at(i));
        };
    // ***********************************************
    // ***********************************************
    // ***********************************************
    return _tr;
}

bool utfchk(QString f, QString s){
    QFile _f(f);
    if(!_f.open(QIODevice::ReadOnly))
        return false;
    QString _data = _f.readAll();
    _f.close();
    if(0<=_data.indexOf(
                s.toUtf8()))
        return true;
    //
    return false;
}

bool utfy(QString f){
    _log::log("utfy() "+f);
    QFile* jfile = new QFile(f);
    //bool op = jfile->open(QIODevice::ReadWrite | QIODevice::Text);
    bool op = jfile->open(QIODevice::ReadWrite);
    //
    if(op){
        QByteArray data = jfile->readAll();
        //jstream.setCodec("UTF-8");
        QTextStream jstream(jfile);
        jstream.setAutoDetectUnicode(true);
        jstream.setCodec(
                    QTextCodec::codecForMib(106));
        jstream.setGenerateByteOrderMark(true);
        jstream << QString(
                       format1(data)).
                   toUtf8();
        jstream.flush();
    };
    //                            QTextStream stream(file);
    //                            stream.readLine();    // detect codec and possibly switch off generateByteOrderMark
    //                            int mib = stream.codec()->mibEnum();    // detected codec, or UTF-8 (default set above)
    //                            bool hasByteOrderMark = stream.generateByteOrderMark();
    _log::log("utfy() out " + QString::number(
                  jfile->readAll().size()) + " bytes");
    jfile->close();
    return op;
}

bool utfrm(QString f, QString s){
    _log::log("utfrm() " + QString::number(
                  s.size()) + " symbols");
    //
    if(!utfchk(f,s))
        return false;
    //
    QFile _f(f);
    if(!_f.open(QIODevice::ReadOnly))
        return false;
    QByteArray _data = _f.readAll();
    _f.close();
    //
    _data.replace(
                s.toUtf8(),
                QByteArray());
    if(_f.open(QIODevice::WriteOnly))
        _f.write(_data);
    _f.close();
    //
    return false;
}


bool utfy(QString f, QStringList l, QString s){
    s = l.join(s);
    //s=_s+s;
    _log::log("utfy() " + QString::number(
                  s.size()) + " symbols");
    QFile* jfile = new QFile(f);
    //bool op = jfile->open(QIODevice::WriteOnly | QIODevice::Text);
    bool op = jfile->open(QIODevice::WriteOnly);
    //
    //****** utfy engine 20191122 **********
    QByteArray data = s.toUtf8();
    if(op){
        //jstream.setCodec("UTF-8");
        QTextStream jstream(jfile);
        jstream.setAutoDetectUnicode(true);
        jstream.setCodec(
                    QTextCodec::codecForMib(106));
        jstream.setGenerateByteOrderMark(true);
        jstream << QString(data)
                   .toUtf8();
        jstream.flush();
    };
    //*************************************
    _log::log("utfy() out " + QString::number(
                  data.size()) + " bytes");
    jfile->close();
    return op;
}

bool utfy(QString f, QString s){
    _log::log("utfy() " + QString::number(
                  s.size()) + " symbols");
    QFile* jfile = new QFile(f);
    //bool op = jfile->open(QIODevice::Append | QIODevice::Text);
    bool op = jfile->open(QIODevice::Append);
    //
    //****** utfy engine 20191122 **********
    QByteArray data = s.toUtf8();
    if(op){
        //jstream.setCodec("UTF-8");
        QTextStream jstream(jfile);
        jstream.setAutoDetectUnicode(true);
        jstream.setCodec(
                    QTextCodec::codecForMib(106));
        jstream.setGenerateByteOrderMark(true);
        jstream << QString(data)
                   .toUtf8();
        jstream.flush();
    };
    //*************************************
    _log::log("utfy() out " + QString::number(
                  data.size()) + " bytes");
    jfile->close();
    return op;
}

//QPalette welcome(QString wpic, QString wtext){
//    _log::log("welcome()\npic :: " + wpic + "\ntext :: " + wtext);
//    // ********************************************
//    // **** Welcome engine Ia 20190709  ***********
//    // ********************************************
//    QImage img = myImage(wpic);
//    //
//    QPainter p(&img);
//    p.setPen(
//                QPen(Qt::white));
//    p.setFont(
//                QFont("Times", 36, QFont::Bold));
//    p.drawText(
//                img.rect(), Qt::AlignLeft, wtext);
//    //
//    QPixmap bp = QPixmap::fromImage(img).scaled(
//                QApplication::desktop()->screenGeometry().size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    QPalette palette;
//    palette.setBrush(QPalette::Background, bp);
//    // ********************************************
//    // ********************************************
//    // ********************************************
//    //    _log::log("welcome() data " + QString::number(
//    //                    img.sizeInBytes()) + " bytes";
//    return palette;
//}
