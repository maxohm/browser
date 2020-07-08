#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>

class QLineEdit;
class QToolButton;

class SearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();

signals:
    void textChanged(const QString& text);
    void previousClicked();
    void nextClicked();

public slots:
    void setFocus();
    void selectAll();
    void clear();

private:
    QLineEdit* search_input_;
    QToolButton* search_previous_;
    QToolButton* search_next_;
};

#endif // SEARCHWIDGET_H
