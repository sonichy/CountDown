#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    int a=0, itv=10, t=10;
    QTimer *timer;
    QString FZCCH, text;
    bool isPaused=true;
};

#endif // MAINWINDOW_H