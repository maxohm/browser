#ifndef WEBPOPUPWINDOW_H
#define WEBPOPUPWINDOW_H

#include <QWidget>

class QLineEdit;
class QWebEngineProfile;
class QWebEngineView;
class QToolBar;
class QVBoxLayout;
class WebView;

class WebPopupWindow : public QWidget
{
    Q_OBJECT

public:
    WebPopupWindow(QWebEngineProfile *profile);
    ~WebPopupWindow() override;
    WebView *view() const;

private slots:
    void handleGeometryChangeRequested(const QRect &newGeometry);

private:
//    QLineEdit *m_urlLineEdit;
    QAction *m_favAction;
    WebView *m_view;

    //
    QToolBar* toolbar_;
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    QAction* m_zoomReset;
    QVBoxLayout *layout_;
};
#endif // WEBPOPUPWINDOW_H
