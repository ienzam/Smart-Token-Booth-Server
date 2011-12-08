#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class QIODevice;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QIODevice *ioDev, QWidget *parent = 0);
    ~MainWindow();

private:
    QIODevice *ioDev;
    Ui::MainWindow *ui;
    QTimer* readWaitTimer;
    QString save;
    QTimer* readFormatedTimer;

private:

    enum ChatterType {Recieve = 0, System, Send};
    void printChat(ChatterType type, QString data);
    void processData(QString str);
    void sendData(QString str);

private slots:
    void on_sendPushButton_clicked();
    void on_disconPushButton_clicked();
    void onReadyRead();
    void onBytesWritten(qint64);
    void sendTextboxData();
};

#endif // MAINWINDOW_H
