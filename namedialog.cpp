#include "namedialog.h"
#include "ui_namedialog.h"

NameDialog::NameDialog(QWidget *parent) :
    QDialog(parent),
    nui(new Ui::NameDialog)
{
    nui->setupUi(this);
}

NameDialog::~NameDialog()
{
    delete nui;
}

void NameDialog::receiveData(QString Name,QString Surn){
    nui->lineEdit->setText(Name);
    nui->lineEdit_2->setText(Surn);
    nui->label_3->setText("Изменение имени и фамилии.");
}
void NameDialog::on_pushButton_clicked()
{
    emit sendData(nui->lineEdit->text(),nui->lineEdit_2->text());
    this->close();
}


void NameDialog::on_pushButton_2_clicked()
{
    emit sendData("","");
    this->close();
}

