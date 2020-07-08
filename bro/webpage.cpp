#include "browserwindow.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webview.h"
//
#include <QAuthenticator>
#include <QMessageBox>
#include <QStyle>
#include <QIcon>

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
//    connect(this, &QWebEnginePage::authenticationRequired, this, &WebPage::handleAuthenticationRequired);
    connect(this, &QWebEnginePage::featurePermissionRequested, this, &WebPage::handleFeaturePermissionRequested);
//    connect(this, &QWebEnginePage::proxyAuthenticationRequired, this, &WebPage::handleProxyAuthenticationRequired);
    //connect(this, &QWebEnginePage::registerProtocolHandlerRequested, this, &WebPage::handleRegisterProtocolHandlerRequested);
#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    //connect(this, &QWebEnginePage::selectClientCertificate, this, &WebPage::handleSelectClientCertificate);
#endif
}

//bool WebPage::certificateError(const QWebEngineCertificateError &error)
//{
//    QWidget *mainWindow = view()->window();
//    if (error.isOverridable()) {
//        QDialog dialog(mainWindow);
//        dialog.setModal(true);
//        dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
//        Ui::CertificateErrorDialog certificateDialog;
//        certificateDialog.setupUi(&dialog);
//        certificateDialog.m_iconLabel->setText(QString());
//        QIcon icon(mainWindow->style()->standardIcon(QStyle::SP_MessageBoxWarning, 0, mainWindow));
//        certificateDialog.m_iconLabel->setPixmap(icon.pixmap(32, 32));
//        certificateDialog.m_errorLabel->setText(error.errorDescription());
//        dialog.setWindowTitle(tr("Ошибка сертификата"));
//        return dialog.exec() == QDialog::Accepted;
//    }

//    QMessageBox::critical(mainWindow, tr("Ошибка сертификата"), error.errorDescription());
//    return false;
//}

//void WebPage::handleAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth)
//{
//    QWidget *mainWindow = view()->window();
//    QDialog dialog(mainWindow);
//    dialog.setModal(true);
//    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

//    Ui::PasswordDialog passwordDialog;
//    passwordDialog.setupUi(&dialog);

//    passwordDialog.m_iconLabel->setText(QString());
//    QIcon icon(mainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, mainWindow));
//    passwordDialog.m_iconLabel->setPixmap(icon.pixmap(32, 32));

//    QString introMessage(tr("Введите имя пользователя и пароль для \"%1\" на %2")
//                         .arg(auth->realm()).arg(requestUrl.toString().toHtmlEscaped()));
//    passwordDialog.m_infoLabel->setText(introMessage);
//    passwordDialog.m_infoLabel->setWordWrap(true);

//    if (dialog.exec() == QDialog::Accepted) {
//        auth->setUser(passwordDialog.m_userNameLineEdit->text());
//        auth->setPassword(passwordDialog.m_passwordLineEdit->text());
//    } else {
//        *auth = QAuthenticator();
//    }
//}

inline QString questionForFeature(QWebEnginePage::Feature feature)
{
    switch (feature) {
    case QWebEnginePage::Geolocation:
        return WebPage::tr("Разрешить %1 доступ к информации о местоположении?");
    case QWebEnginePage::MediaAudioCapture:
        return WebPage::tr("Разрешить %1 доступ к микрофону?");
    case QWebEnginePage::MediaVideoCapture:
        return WebPage::tr("Разрешить %1 доступ к вебкамере?");
    case QWebEnginePage::MediaAudioVideoCapture:
        return WebPage::tr("Разрешить %1 доступ к микрофону/вебкамере?");
    case QWebEnginePage::MouseLock:
        return WebPage::tr("Разрешить %1 захват курсора?");
//    case QWebEnginePage::DesktopVideoCapture:
//        return WebPage::tr("Allow %1 to capture video of your desktop?");
//    case QWebEnginePage::DesktopAudioVideoCapture:
//        return WebPage::tr("Allow %1 to capture audio and video of your desktop?");
    case QWebEnginePage::Notifications:
        return QString();
    }
    return QString();
}

void WebPage::handleFeaturePermissionRequested(const QUrl &securityOrigin, Feature feature)
{
    QString title = tr("Запрос разрешений");
    QString question = questionForFeature(feature).arg(securityOrigin.host());
    if (!question.isEmpty() && QMessageBox::question(view()->window(), title, question) == QMessageBox::Yes)
        setFeaturePermission(securityOrigin, feature, PermissionGrantedByUser);
    else
        setFeaturePermission(securityOrigin, feature, PermissionDeniedByUser);
}

//void WebPage::handleProxyAuthenticationRequired(const QUrl &, QAuthenticator *auth, const QString &proxyHost)
//{
//    QWidget *mainWindow = view()->window();
//    QDialog dialog(mainWindow);
//    dialog.setModal(true);
//    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

//    Ui::PasswordDialog passwordDialog;
//    passwordDialog.setupUi(&dialog);

//    passwordDialog.m_iconLabel->setText(QString());
//    QIcon icon(mainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion, 0, mainWindow));
//    passwordDialog.m_iconLabel->setPixmap(icon.pixmap(32, 32));

//    QString introMessage = tr("Подключение к прокси \"%1\" используя:");
//    introMessage = introMessage.arg(proxyHost.toHtmlEscaped());
//    passwordDialog.m_infoLabel->setText(introMessage);
//    passwordDialog.m_infoLabel->setWordWrap(true);

//    if (dialog.exec() == QDialog::Accepted) {
//        auth->setUser(passwordDialog.m_userNameLineEdit->text());
//        auth->setPassword(passwordDialog.m_passwordLineEdit->text());
//    } else {
//        *auth = QAuthenticator();
//    }
//}

//void WebPage::handleRegisterProtocolHandlerRequested(QWebEngineRegisterProtocolHandlerRequest request)
//{
//    auto answer = QMessageBox::question(
//        view()->window(),
//        tr("Запрос разрешений"),
//        tr("Разрешить %1 открывать все %2 ссылки?")
//        .arg(request.origin().host())
//        .arg(request.scheme()));
//    if (answer == QMessageBox::Yes)
//        request.accept();
//    else
//        request.reject();
//}

#if !defined(QT_NO_SSL) || QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
//void WebPage::handleSelectClientCertificate(QWebEngineClientCertificateSelection selection)
//{
//    // Just select one.
//    selection.select(selection.certificates().at(0));
//}
#endif
