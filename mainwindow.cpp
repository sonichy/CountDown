//https://ddrk.me/vjs-plugins/10.m3u8
#include "mainwindow.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QTimer>
#include <QFontDatabase>
#include <QDebug>
#include <QShortcut>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    showFullScreen();
    text = "看到此倒计时是因为您屏蔽了本站广告\n关闭广告屏蔽软件就不需要等待了";
    int fontId = QFontDatabase::addApplicationFont(":/FZCCHJW.TTF");
    FZCCH = QFontDatabase::applicationFontFamilies(fontId).at(0);
    qDebug() << fontId << FZCCH;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(itv);

    connect(new QShortcut(QKeySequence(Qt::Key_Space), this), &QShortcut::activated, [=]{
        isPaused = !isPaused;
        if (isPaused) {
             timer->stop();
        } else {
            timer->start(itv);
        }
    });

    connect(new QShortcut(QKeySequence(Qt::Key_R), this), &QShortcut::activated, [=]{ //restart
        a = 0;
        t= 10;
        timer->start(itv);
    });

    connect(new QShortcut(QKeySequence(Qt::Key_Escape), this), &QShortcut::activated, []{
        qApp->quit();
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), QBrush(Qt::black));
    QPen pen(QColor(0,162,244));
    pen.setWidth(10);
    painter.setPen(pen);
    int d = qMin(width(), height()) / 3;
    QRect rect(QApplication::desktop()->width() / 2 - d, QApplication::desktop()->height() / 2 - d, 2 * d, 2 * d);

    painter.drawArc(rect, 90*16, a);
    //painter.setPen(Qt::NoPen);
    painter.setPen(QColor(0,162,244,50));
    painter.setBrush(QColor(0,162,244,50));
    if (a > -40*16)
        painter.drawPie(rect, a + 90*16, -a);//出扇
    else
        painter.drawPie(rect, a + 90*16, 40*16);
    painter.setPen(Qt::white);
    QFont font(FZCCH, 300);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString::number(t));
    font = qApp->font();
    font.setPointSize(30);
    painter.setFont(font);
    painter.drawText(this->rect(), Qt::AlignCenter | Qt::AlignBottom, text);
    if (a < -360*16) {
        a = 0;
        if (t > 0) {
            t--;
        } else {
            timer->stop();
            //补一帧清除扇形
            painter.fillRect(this->rect(), QBrush(Qt::black));
            QPen pen(QColor(0,162,244));
            pen.setWidth(10);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(rect);
            painter.setPen(Qt::white);
            QFont font(FZCCH, 300);
            painter.setFont(font);
            painter.drawText(rect, Qt::AlignCenter, "0");
            font = qApp->font();
            font.setPointSize(30);
            painter.setFont(font);
            painter.drawText(this->rect(), Qt::AlignCenter | Qt::AlignBottom, text);
        }
    }
    a -= 360 * 16 / itv / 10;
}