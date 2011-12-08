#ifndef CATEGORYASSIGNWINDOW_H
#define CATEGORYASSIGNWINDOW_H

#include <QDialog>

namespace Ui {
    class CategoryAssignWindow;
}
class QComboBox;

class CategoryAssignWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryAssignWindow(QWidget *parent = 0);
    ~CategoryAssignWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CategoryAssignWindow *ui;
    QList<QComboBox*> boxes;
};

#endif // CATEGORYASSIGNWINDOW_H
