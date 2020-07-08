#include "searchwidget.h"

#include <QDebug>
#include <QFontMetrics>
#include <QFrame>
#include <QHBoxLayout>
#include <QLayout>
#include <QLineEdit>
#include <QToolButton>

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent)
{
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(5 ,0 ,5 ,0);
    layout->setSpacing(0);

    search_input_ = new QLineEdit(this);
//    search_input_->setFrame(false);
    search_input_->setClearButtonEnabled(true);
    search_input_->setPlaceholderText("Найти на странице");
    QFontMetrics font_metrics(search_input_->font());
    search_input_->setMinimumWidth(font_metrics.width(search_input_->placeholderText()) + 50);

    connect(search_input_, &QLineEdit::textChanged, this, &SearchWidget::textChanged);
    layout->addWidget(search_input_);

    search_previous_ = new QToolButton(this);
    search_previous_->setAutoRaise(true);
    search_previous_->setIcon(QIcon::fromTheme("go-up"));
    search_previous_->setShortcut(Qt::SHIFT + Qt::Key_F3);
    connect(search_previous_, &QToolButton::clicked, this, &SearchWidget::previousClicked);
    layout->addWidget(search_previous_);

    search_next_ = new QToolButton(this);
    search_next_->setAutoRaise(true);
    search_next_->setIcon(QIcon::fromTheme("go-down"));
    search_next_->setShortcut(Qt::Key_F3);
    connect(search_next_, &QToolButton::clicked, this, &SearchWidget::nextClicked);
    layout->addWidget(search_next_);

    auto vline = new QFrame(this);
    vline->setFrameShape(QFrame::VLine);
    layout->addWidget(vline);

    setLayout(layout);
}

SearchWidget::~SearchWidget()
{
    //Qt само управляет объектами.
}

void SearchWidget::setFocus()
{
    search_input_->setFocus();
}

void SearchWidget::selectAll()
{
    search_input_->selectAll();
}

void SearchWidget::clear()
{
    search_input_->clear();
}
