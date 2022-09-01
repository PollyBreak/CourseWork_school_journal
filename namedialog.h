#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>

namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameDialog(QWidget *parent = nullptr);
    ~NameDialog();
public slots:
    void receiveData(QString Name,QString Surn);
signals:
    void sendData(QString name,QString surn);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::NameDialog *nui;
};

#endif // NAMEDIALOG_H
