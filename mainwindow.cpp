#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTimer>
#include "abstractserial.h"
#include "config.h"
#include <limits>

MainWindow::MainWindow(QIODevice *ioDev, QWidget *parent) :
        QMainWindow(parent),
        ioDev(ioDev),
        ui(new Ui::MainWindow)
{
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    ui->setupUi(this);
    connect(ioDev, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(ioDev, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));

    readWaitTimer = new QTimer(this);
    readWaitTimer->setInterval(STB_NEXTCHAR_INTERVAL);
    readWaitTimer->setSingleShot(true);
    connect(readWaitTimer, SIGNAL(timeout()), this, SLOT(onReadyRead()));

    onReadyRead();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printChat(ChatterType type, QString data) {
    data = data.trimmed();
    if(!data.length()) return;
    char colors[][10] = { "blue", "red", "green" };
    char chatterName[][10] = {"Recieve", "System", "Send"};
    ui->chatBoxTextEdit->append(
            QString("<font color='%1'>[%2.%5] %3 : </font>%4\n")
            .arg(colors[type], QTime::currentTime().toString(), chatterName[type], data)
                .arg(QTime::currentTime().msec())
            );
}

void MainWindow::processData(QString data) {
    if(data.isEmpty()) return;
    printChat(Recieve, data);
    readWaitTimer->stop();
}


void MainWindow::onReadyRead() {
    static QByteArray buffer = "";
    QByteArray b = ioDev->peek(100);
    save+=b;
    buffer += b;
    if(b.length()) readWaitTimer->start();
    else {
        processData(buffer);
        buffer = "";
    }
}

void MainWindow::onBytesWritten(qint64 numBytes) {
    printChat(System, QString("Wrote %1 bytes").arg(numBytes));
}

void MainWindow::sendTextboxData() {
    QString data = ui->chatlineEdit->text()+'\n';
    sendData(data);
}

void MainWindow::sendData(QString data) {
    if(ioDev->write(data.toAscii()) == -1) {
         statusBar()->showMessage("Can't send data!");
         printChat(System, "Can't send data :o");
     } else {
         printChat(Send, data);
     }
}

void MainWindow::on_disconPushButton_clicked()
{
    ioDev->close();
}

void MainWindow::on_sendPushButton_clicked()
{
    sendTextboxData();
}
