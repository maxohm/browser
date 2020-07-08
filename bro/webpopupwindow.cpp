#include "webpage.h"
#include "webpopupwindow.h"
#include "webview.h"
//
#include <QAction>
//#include <QIcon>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWindow>
#include <QToolBar>

WebPopupWindow::WebPopupWindow(QWebEngineProfile *profile) :
//      m_urlLineEdit(new QLineEdit(this)),
    m_favAction(new QAction(this)),
    m_view(new WebView(this))
{
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout_ = new QVBoxLayout;
    //layout->setMargin(0);
    layout_->setContentsMargins(1,1,1,1);

    setLayout(layout_);
//    layout_->addWidget(m_urlLineEdit);
    toolbar_ = new QToolBar(this);
    m_zoomInAction = new QAction(this);
    m_zoomInAction->setIcon(QIcon::fromTheme("zoom-in"));
    m_zoomInAction->setShortcut(Qt::CTRL + Qt::Key_Plus);
    m_zoomInAction->setToolTip("Увеличить");
    connect(m_zoomInAction, &QAction::triggered, [this] () {
        qreal zoom = this->m_view->zoomFactor();
        zoom += 0.1;
            m_view->setZoomFactor(zoom);
    });
    toolbar_->addAction(m_zoomInAction);
    m_zoomOutAction = new QAction(this);
    m_zoomOutAction->setIcon(QIcon::fromTheme("zoom-out"));
    m_zoomOutAction->setShortcut(Qt::CTRL + Qt::Key_Minus);
    m_zoomOutAction->setToolTip("Уменьшить");
    connect(m_zoomOutAction, &QAction::triggered, [this] () {
        qreal zoom = this->m_view->zoomFactor();
        zoom -= 0.1;
            m_view->setZoomFactor(zoom);
    });
    toolbar_->addAction(m_zoomOutAction);
    m_zoomReset = new QAction(this);
    m_zoomReset->setIcon(QIcon::fromTheme("zoom-original"));
    m_zoomReset->setShortcut(Qt::CTRL + Qt::Key_Equal);
    m_zoomReset->setToolTip("Сбросить масштаб");
    connect(m_zoomReset, &QAction::triggered, [this] () {
        m_view->setZoomFactor(1.0);
    });
    toolbar_->addAction(m_zoomReset);

    layout_->addWidget(toolbar_);
    layout_->addWidget(m_view);

    m_view->setPage(new WebPage(profile, m_view));
    m_view->setFocus();
//    m_urlLineEdit->setReadOnly(true);
//    m_urlLineEdit->addAction(m_favAction, QLineEdit::LeadingPosition);

    connect(m_view, &WebView::titleChanged, this, &QWidget::setWindowTitle);
//    connect(m_view, &WebView::urlChanged, [this](const QUrl &url) {
//        m_urlLineEdit->setText(url.toDisplayString());
//    });
    connect(m_view, &WebView::favIconChanged, m_favAction, &QAction::setIcon);
    connect(m_view->page(), &WebPage::geometryChangeRequested, this, &WebPopupWindow::handleGeometryChangeRequested);
    connect(m_view->page(), &WebPage::windowCloseRequested, this, &QWidget::close);
}

WebPopupWindow::~WebPopupWindow()
{
    delete m_zoomOutAction;
    delete m_zoomInAction;
    delete m_favAction;
    delete toolbar_;
//    delete m_urlLineEdit;
    delete m_view;
    delete layout_;
}

WebView *WebPopupWindow::view() const
{
    return m_view;
}

void WebPopupWindow::handleGeometryChangeRequested(const QRect &newGeometry)
{
    if (QWindow *window = windowHandle())
        setGeometry(newGeometry.marginsRemoved(window->frameMargins()));
    show();
    m_view->setFocus();
}
