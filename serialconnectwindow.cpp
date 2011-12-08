#include "serialconnectwindow.h"
#include "ui_connectwindow.h"
#include "serialdeviceenumerator.h"
#include "abstractserial.h"
#include "mainwindow.h"
#include <QSettings>
#include "boothserver.h"

ConnectWindow::ConnectWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::ConnectWindow),
        device(new AbstractSerial(this))
{
    ui->setupUi(this);

    SerialDeviceEnumerator* serDevEnum = SerialDeviceEnumerator::instance();
    connect(serDevEnum, SIGNAL(hasChanged(QStringList)),
            this, SLOT(updateDeviceList(QStringList)));

    connect(device, SIGNAL(aboutToClose()), this, SLOT(show()));

    updateDeviceList(serDevEnum->devicesAvailable());
    populateBaudRates();
    populateDataBits();
    populateParity();
    populateStopBits();
    populateFlowControl();
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::updateDeviceList(const QStringList &list)
{
    ui->deviceNameComboBox->clear();
    SerialDeviceEnumerator* serDevEnum = SerialDeviceEnumerator::instance();
    QSettings settings;
    foreach (QString s, list) {
        if(!s.toLower().contains("usb")) continue;
        serDevEnum->setDeviceName(s);
        ui->deviceNameComboBox->addItem(serDevEnum->description()+" (" + s + ")", s);
        if(settings.value("deviceName") == s) {
            ui->deviceNameComboBox->setCurrentIndex(ui->deviceNameComboBox->count()-1);
        }
    }
}

#define ADD_ITEM(comboBox,enumName) comboBox->addItem(#enumName, AbstractSerial::enumName)
#define DIM(a,b) __typeof(b) a = b

void ConnectWindow::populateBaudRates()
{
    QSettings settings;
    DIM(baudRateMap, device->baudRateMap());
    for(DIM(it, baudRateMap.constBegin()); it != baudRateMap.constEnd(); it++) {
        ui->baudRateComboBox->addItem(it.value(), it.key());
        if(it.key() == settings.value("baudRate")) {
            ui->baudRateComboBox->setCurrentIndex(ui->baudRateComboBox->count()-1);
        }
    }
}

void ConnectWindow::populateDataBits()
{
    QSettings settings;
    DIM(dataBitsMap, device->dataBitsMap());
    for(DIM(it, dataBitsMap.constBegin()); it != dataBitsMap.constEnd(); it++) {
        ui->dataBitsComboBox->addItem(it.value(), it.key());
        if(it.key() == settings.value("dataBits")) {
            ui->dataBitsComboBox->setCurrentIndex(ui->dataBitsComboBox->count()-1);
        }
    }
}

void ConnectWindow::populateParity()
{
    QSettings settings;
    DIM(parityMap, device->parityMap());
    for(DIM(it, parityMap.constBegin()); it != parityMap.constEnd(); it++) {
        ui->parityComboBox->addItem(it.value(), it.key());
        if(it.key() == settings.value("parity")) {
            ui->parityComboBox->setCurrentIndex(ui->parityComboBox->count()-1);
        }
    }
}

void ConnectWindow::populateStopBits()
{
    QSettings settings;
    DIM(stopBitsMap, device->stopBitsMap());
    for(DIM(it, stopBitsMap.constBegin()); it != stopBitsMap.constEnd(); it++) {
        ui->stopBitsComboBox->addItem(it.value(), it.key());
        if(it.key() == settings.value("stopBits")) {
            ui->stopBitsComboBox->setCurrentIndex(ui->stopBitsComboBox->count()-1);
        }
    }
}

void ConnectWindow::populateFlowControl()
{
    QSettings settings;
    DIM(flowControlMap, device->flowControlMap());
    for(DIM(it, flowControlMap.constBegin()); it != flowControlMap.constEnd(); it++) {
        ui->flowControlComboBox->addItem(it.value(), it.key());
        if(it.key() == settings.value("flowControl")) {
            ui->flowControlComboBox->setCurrentIndex(ui->flowControlComboBox->count()-1);
        }
    }
}

#define COMBO_DATA_STR(comboBox) comboBox->itemData(comboBox->currentIndex()).toString()
#define COMBO_DATA_INT(comboBox) comboBox->itemData(comboBox->currentIndex()).toInt()

void ConnectWindow::on_connectPushButton_clicked()
{
    QSettings settings;
    device->setDeviceName(COMBO_DATA_STR(ui->deviceNameComboBox));

    if(!device->open(AbstractSerial::ReadWrite)) {
        qDebug() << "Connection error.";
        statusBar()->showMessage(tr("Can't Connect to device!!! :-("));
        return;
    }
    settings.setValue("deviceName", COMBO_DATA_STR(ui->deviceNameComboBox));

    if (!device->setBaudRate((AbstractSerial::BaudRate)COMBO_DATA_INT(ui->baudRateComboBox))) {
        qDebug() << "Set baud rate error.";
        statusBar()->showMessage(tr("Set baud rate error."));
        goto label;
    };
    settings.setValue("baudRate", COMBO_DATA_INT(ui->baudRateComboBox));

    if (!device->setDataBits((AbstractSerial::DataBits)COMBO_DATA_INT(ui->dataBitsComboBox))) {
        qDebug() << "Set data bits error.";
        statusBar()->showMessage(tr("Set data bits error."));
        goto label;
    }
    settings.setValue("dataBits", COMBO_DATA_INT(ui->dataBitsComboBox));

    if (!device->setParity((AbstractSerial::Parity)COMBO_DATA_INT(ui->parityComboBox))) {
        qDebug() << "Set parity error.";
        statusBar()->showMessage(tr("Set parity error."));
        goto label;
    }
    settings.setValue("parity", COMBO_DATA_INT(ui->parityComboBox));

    if (!device->setStopBits((AbstractSerial::StopBits)COMBO_DATA_INT(ui->stopBitsComboBox))) {
        qDebug() << "Set stop bits error.";
        statusBar()->showMessage(tr("Set stop bits error."));
        goto label;
    }
    settings.setValue("stopBits", COMBO_DATA_INT(ui->stopBitsComboBox));

    if (!device->setFlowControl((AbstractSerial::Flow)COMBO_DATA_INT(ui->flowControlComboBox))) {
        qDebug() << "Set flow control error.";
        statusBar()->showMessage(tr("Set flow control error."));
        goto label;
    }
    settings.setValue("flowControl", COMBO_DATA_INT(ui->flowControlComboBox));
    goto showMain;

    label:
    device->close();
    return;

    showMain:

    qDebug() << "= Default parameters =";
    qDebug() << "Device name            : " << device->deviceName();
    qDebug() << "Baud rate              : " << device->baudRate();
    qDebug() << "Data bits              : " << device->dataBits();
    qDebug() << "Parity                 : " << device->parity();
    qDebug() << "Stop bits              : " << device->stopBits();
    qDebug() << "Flow                   : " << device->flowControl();
    qDebug() << "Char timeout, msec     : " << device->charIntervalTimeout();


    MainWindow *mainWin = new MainWindow(device);
    mainWin->show();
    this->hide();

    BoothServer *boothServer = new BoothServer(device);

    connect(device, SIGNAL(aboutToClose()), mainWin, SLOT(deleteLater()));
    connect(device, SIGNAL(aboutToClose()), boothServer, SLOT(deleteLater()));
}

void ConnectWindow::on_exitPushButton_clicked()
{
    this->close();
}

#include "categoryassignwindow.h"
void ConnectWindow::on_categoryButton_clicked()
{
    (new CategoryAssignWindow(this))->exec();
}
