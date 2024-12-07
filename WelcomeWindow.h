#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>

class WelcomeWindow : public QWidget {
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *startButton;

signals:
    void startGameClicked();
    void  startAgain();
};

#endif // WELCOMEWINDOW_H
