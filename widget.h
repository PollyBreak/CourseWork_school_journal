#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include  "namedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void receiveData(QString Name,QString Surn);
signals:
    void sendData(QString name,QString surn);
private slots:
    void on_pushButton_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButton_2_clicked();

    void on_comboBox_activated(int index);

    void on_comboBox_2_activated(int index);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::Widget *ui;
    NameDialog *ndlg;
    void setupTables();
    void setupGrades();
    int createmsgSignal(QString mesg);
    void formateTable();
    bool checkGrade(QString Check);
    void loadData();
    void saveData();
    void restrictEdit();
    void enableEdit();
};

#endif // WIDGET_H
