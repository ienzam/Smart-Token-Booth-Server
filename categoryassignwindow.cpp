#include "categoryassignwindow.h"
#include "ui_categoryassignwindow.h"

#include "config.h"
#include "servicecategory.h"

#include <QComboBox>
#include <QSettings>
#include <QDebug>

CategoryAssignWindow::CategoryAssignWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryAssignWindow)
{
    ui->setupUi(this);

    QList< QPair<QString, char> > categories = ServiceCategory::getCategoryList();
    qDebug() << categories ;

    QSettings settings;
    settings.beginReadArray("service_booth");

    for(int i = 1; i <= NUM_SERVICE_BOOTH; i++) {
        QComboBox *comboBox = new QComboBox(this);
        settings.setArrayIndex(i);
        char catID = settings.value("category", "A").toInt();

        for(int j = 0; j < categories.size(); j++) {
            comboBox->addItem(categories[j].first, categories[j].second);
            if(catID == categories[j].second) comboBox->setCurrentIndex(j);
            //qDebug() << comboBox->itemData(i).toString();
        }
        ui->formLayout->addRow(QString("Service Booth %1").arg(i), comboBox);
        boxes << comboBox;
    }
    settings.endArray();
}

CategoryAssignWindow::~CategoryAssignWindow()
{
    delete ui;
}

void CategoryAssignWindow::on_buttonBox_accepted()
{
    QSettings settings;
    settings.beginWriteArray("service_booth");
    for(int i = 1; i <= NUM_SERVICE_BOOTH; i++) {
        QComboBox *comboBox = boxes[i-1];
        char catID = comboBox->itemData(comboBox->currentIndex()).toChar().toAscii();
        settings.setArrayIndex(i);
        settings.setValue("category", catID);

        qDebug() << "Assigning " << i << " to category " << catID;

    }
    settings.endArray();
    this->accept();
}
