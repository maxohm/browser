#include <QDir>
#include <QTranslator>
#include <QStandardPaths>
//
#include "browser.h"
#include "browserwindow.h"
#include "ui_about.h"
#include "ui_passworddialog.h"
//
#include <QJsonParseError>
#include <QTextCodec>
#include <QMessageBox>
//
#include "treeop.h"
#include "../libengines/libengines.h"

//QUrl commandLineUrlArgument()
//{
////    const QStringList args = QCoreApplication::arguments();
////    for (const QString &arg : args.mid(1)) {
////        if (!arg.startsWith(QLatin1Char('-')))
////            return QUrl::fromUserInput(arg);
////    }
//    return QUrl(QStringLiteral(""));
//}

int main(int argc, char **argv)
{
    //AllocConsole();
    //
    _log::_log::log("main() 1");
    const auto chromiumFlags = qgetenv("QTWEBENGINE_CHROMIUM_FLAGS");

#ifdef QT_DEBUG
    //qInstallMessageHandler(_log::myMessageHandler);
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", chromiumFlags+"--flag-switches-begin --enable-logging=stderr --v --use-spdy=off --disable-http2 --disable-gl-error-limit --enable-es3-apis --enable-webgl-draft-extensions --flag-switches-end");
#else
    //QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    //QUrl url = commandLineUrlArgument();
    //    const auto chromiumFlags = qgetenv("QTWEBENGINE_CHROMIUM_FLAGS");
    //    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", chromiumFlags + " --enable-logging=stderr --v");

    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", chromiumFlags+"--flag-switches-begin --disable-logging --flag-switches-end");
#endif

    QApplication app(argc, argv);
    //********************************
    QString cwd = objdir(argv[0]);
    //    cwd.truncate(
    //                cwd.indexOf("bin"));
    QDir::setCurrent(cwd);
    _log::log(cwd);
    //********************************

    // ****** args engine I 20190708 ******
    QString args = app.arguments().join(" ").toLower();
    _log::log(" args :: " + args);
    // ************************************

    //    QCoreApplication::setOrganizationName("2019, IKAR, Krasnodar");
    //    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    //
    _log::log("QStandardPaths::DataLocation       : " + QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    _log::log("QStandardPaths::CacheLocation      : " + QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    _log::log("QStandardPaths::RuntimeLocation    : " + QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation));
    _log::log("QStandardPaths::GenericDataLocation: " + QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
    _log::log("QStandardPaths::AppDataLocation    : " + QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    //
    _log::log("main() 2");
    QString conf = objpath(cwd,
                           objname(argv[0])+".ini");
    //
    _log::log("main() 3 "+conf);
    QSettings* cfg = new QSettings(conf, QSettings::IniFormat);
    cfg->beginGroup("main");
    //
    QCoreApplication::setOrganizationName(
                cfg2str(cfg,"binaryName"));
    _log::log(
                cfg2str(cfg,"defaultUser"));
    //
    QDialog dialog;
    //
    QString sumf = objpath(cwd,
                           objname(argv[0])+".cat");
    QString ex = cfg2str(cfg,"libPath");
    //**** ultimate file verification engine *****
    if(!args.contains("nc"))
        if(!args.contains("no_check"))
            if(!md5chks())
            {
    // ********************************************
                QMessageBox::critical(&dialog,
                                      app.tr("Ошибка"),
                                      app.tr("Нарушена целостность файлов программы")+
                                      app.tr("\n- программа будет закрыта."),
                                      QMessageBox::Close);
                exit(1);
            };
    //
    _log::log("main() 4");
    // ********************************************
    // ****** Auth engine IIa 20190710  ***********
    // ********************************************

#ifdef QT_DEBUG
    if(!args.contains("na")) {
#endif
        const char* auths = "Авторизация";
        //
        Ui::PasswordDialog pwD;
        pwD.setupUi(&dialog);
        //pwD.m_iconLabel->setText(QString());
        //        pwD.m_iconLabel->setWindowIcon(
        //                        myIcon("wicon"));
        pwD.m_iconLabel->setPixmap(
                    myIcon("secure-card"
                           ).pixmap(32,32));
        pwD.m_infoLabel->setText(
                    app.tr(""));
        pwD.m_infoLabel->setWordWrap(true);
        //
        dialog.setWindowTitle(
                    app.tr(auths));
        dialog.setModal(true);
        dialog.setStatusTip(
                    app.tr(auths));
        //
        bool ok;
        int ret = cfg2str(cfg,"defaultTries").toInt(&ok, 10);
        //
        //
        while(0<ret){

            if(QDialog::Accepted!=dialog.exec())
                return 1;
            ret--;
            //

            //
            //            if(0==ret){
            //                QMessageBox::critical(&dialog,
            //                                      app.tr("Ошибка"),
            //                                      app.tr("Исчерпано количество попыток авторизации (")+
            //                                      cfg2str(cfg,"defaultTries")+
            //                                      app.tr(")\n- программа будет закрыта."),
            //                                      QMessageBox::Close);

            _log::log(" ========================== " + cfg2str(cfg,"defaultUser") + " ========================== " + pwD.m_userNameLineEdit->text());
            _log::log(" ========================== " + cfg2str(cfg,"defaultPass") + " ========================== " + pwD.m_passwordLineEdit->text());

            if (  cfg2str(cfg,"defaultUser") != pwD.m_userNameLineEdit->text() ||
                  cfg2str(cfg,"defaultPass") != pwD.m_passwordLineEdit->text() )
            {
                QMessageBox::critical(&dialog,
                                      app.tr("Ошибка"),
                                      app.tr("Неверные имя пользователя или пароль."),
                                      QMessageBox::Close);
                _log::log(QString::number(ret));
                //
                if(0==ret){
                    QMessageBox::critical(&dialog,
                                          app.tr("Ошибка"),
                                          app.tr("Исчерпано количество попыток авторизации (")+
                                          cfg2str(cfg,"defaultTries")+
                                          app.tr(")\n- программа будет закрыта."),
                                          QMessageBox::Close);
                    return 2;
                };
            } else {
                ret = 0;
            };
            //};
        };
#ifdef QT_DEBUG
    }
#endif
    // ********************************************
    // ********************************************
    // ********************************************

    //
    _log::log("main() 5");
    //
    Browser browser;
    BrowserWindow* window = browser.createWindow(conf);

    QList<QTranslator*> _tr = transinst(
                objpath(
                    cdUp(cwd),
                    cfg2str(cfg,"transPath")),window);
    for (int i=0; i<_tr.count(); i++)
        app.installTranslator(
                    _tr.at(i));

    window->setWindowTitle(
                cfg2str(cfg,"prodName"));

    _log::log("main() 6");
    // ********************************************
    // ****** About engine Ia 20190709 ************
    // ********************************************
    Ui::About aboutD;
    aboutD.setupUi(&window->about);
    //
    aboutD.m_iconLabel->setPixmap(
                myPixmap(
                    cfg2str(cfg,"prodLogo")
                    ).scaled(
                    aboutD.m_iconLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    aboutD.m_infoLabel->setText(
                window->wtext);
    aboutD.m_infoLabel->setWordWrap(true);
    // ********************************************
    // ********************************************
    // ********************************************

    _log::log("main() 7");
    app.setWindowIcon(
                myIcon("wicon"));
    //FreeConsole();
    return app.exec();
}
