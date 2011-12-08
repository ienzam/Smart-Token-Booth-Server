#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QMainWindow>
class AbstractSerial;
namespace Ui {
    class ConnectWindow;
}

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = 0);
    ~ConnectWindow();

private:
    Ui::ConnectWindow *ui;
    AbstractSerial *device;

    void populateBaudRates();
    void populateDataBits();
    void populateParity();
    void populateStopBits();
    void populateFlowControl();

private slots:
    void on_exitPushButton_clicked();
    void on_connectPushButton_clicked();
    void updateDeviceList(const QStringList& list);
    void on_categoryButton_clicked();
};

#endif // CONNECTWINDOW_H
