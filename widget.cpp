#include "widget.h"
#include "ui_widget.h"
#include  <QInputDialog>
#include   <QMessageBox>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QFile>

const int subj_num = 14;
const int stud_num = 30;
const int grup_num = 11;
const int lesn_num = 14;
class Student{
private:
    QString name = "";
    QString surname ="";
public:
    Student (){};
    Student(QString Name){
        this->name = Name;
    }
    Student(QString Name,QString Surn){
        this->name=Name;
        this->surname=Surn;
    }
    QString getName(){
        return this->name;
    }
    void setName(QString Name){
        this->name=Name;
    }
    QString getSurn(){
        return this->surname;
    }
    void setSurn(QString sName){
        this->surname=sName;
    }
    double middleGrade[subj_num]={6,6,6,6,6,6,6,6,6,6,6,6,6,6};
    double absence=0;
    bool isViable = true;
    //these are tmp
    int gradeSum = 0;
    int gradeNum = 0;
    int gradeNumTotal=0;
    int missNum = 0;
    //
    QString grades[subj_num][lesn_num];
    void updateQuality(){
        this->isViable=true;
        this->missNum=0;
        this->gradeNumTotal=0;
        for(int i=0;i<subj_num;i++){
            this->gradeSum=0;
            this->gradeNum=0;
                for(int  j=0;j<lesn_num;j++)
                {
                    if (this->grades[i][j][0].isDigit())
                    {
                        this->gradeSum+=this->grades[i][j].toInt();
                        this->gradeNum+=1;
                    } else if (this->grades[i][j]=="н"&&this->grades[i][j]!="")
                        this->missNum+=1;
                }
                this->gradeNumTotal+=this->gradeNum;
                if (this->gradeNum!=0)
                this->middleGrade[i]=(double)this->gradeSum/this->gradeNum;
                if(this->middleGrade[i]<2.5)
                    this->isViable=false;
        }
        if (this->missNum+this->gradeNumTotal!=0)
        this->absence=(double)(1-(double)((double)this->missNum/(double)(this->missNum+this->gradeNumTotal)));
        if (this->absence<0.5)
            this->isViable=false;
    }
};
class Group{
public:
    int name;
    Student students[stud_num];
};
Group groups[grup_num];
int currentGroup=-1;
int currentSubject=-1;
QStringList one =  {"Русский язык","Чтение","Физ-ра","Музыка","Математика","ИЗО","Окр. мир"};
QStringList twotofour =  {"Русский язык","Чтение","Физ-ра","Музыка","Математика","ИЗО","Окр. мир","Англ. язык"};
QStringList fivetosix =  {"Русский язык","Литература","Физ-ра","Математика","ИЗО","География","История","Обществозн.","Биология","Англ. язык"};
QStringList seventoeleven =  {"Русский язык","Литература","Физ-ра","Алгебра","Геометрия","ИЗО","География","История","Обществозн.","Биология","Англ. язык","Химия","Информатика","Физика"};



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //loadData();
    ui->tableWidget->hideColumn(0);
}
Widget::~Widget()
{
    delete ui;
}

void Widget::saveData(){
    QFile file("data.xml");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter written(&file);
    written.writeStartDocument();
     written.setAutoFormatting(true);
    for (int grup =0;grup<grup_num;grup++)
    {
        written.writeStartElement("grup");
        written.writeAttribute("id",QString::number(grup));
        for(int stud=0;stud<stud_num;stud++){
            written.writeStartElement("stud");
            written.writeAttribute("id",QString::number(stud));
            written.writeAttribute("name",groups[grup].students[stud].getName());
            written.writeAttribute("sname",groups[grup].students[stud].getSurn());
            written.writeAttribute("viability",groups[grup].students[stud].isViable?"true":"false");
            written.writeAttribute("absence",QString::number(groups[grup].students[stud].absence,'f',1));
            written.writeStartElement("averages");
            for (int i=0;i<subj_num;i++)
            {
                written.writeAttribute(QString::number(i),QString::number(groups[grup].students[stud].middleGrade[i]));
            }
            written.writeEndElement();
            written.writeStartElement("grades");
            for (int i=0;i<subj_num;i++)
                for(int  j=0;j<lesn_num;j++)
                    written.writeAttribute(QString::number(i),groups[grup].students[stud].grades[i][j]);
            written.writeEndElement();
            written.writeEndElement();
    }
        written.writeEndElement();
    }
    written.writeEndDocument();
    file.close();
}
void Widget::loadData(){
QFile file("data.xml");
if (!file.open(QIODevice::ReadOnly))
   {
       createmsgSignal("Сохраненные данные не обнаружены!");
   } else {
    QXmlStreamReader readen(&file);
    int grup=0;
    int stud=0;
    while(!readen.atEnd()&&!readen.hasError())
    {
         QXmlStreamReader::TokenType token = readen.readNext();
         if (token == QXmlStreamReader::StartDocument)
                    continue;
         if (token == QXmlStreamReader::StartElement)
                {
                    if (readen.name() == "grup"){
                        QXmlStreamAttributes attributes = readen.attributes();
                        if (attributes.hasAttribute("id"))
                                grup = attributes.value("id").toInt();
                            readen.readNext();
                            while (!(readen.tokenType() == QXmlStreamReader::EndElement && readen.name() == "grup"))
                               {
                                   if (readen.tokenType() == QXmlStreamReader::StartElement)
                                   {
                                       if (readen.name() == "stud"){
                                           foreach(const QXmlStreamAttribute  &attr,readen.attributes()){
                                               if(attr.name()=="id")
                                                   stud=attr.value().toInt();
                                               if(attr.name()=="name")
                                                   groups[grup].students[stud].setName(attr.value().toString());
                                               if(attr.name()=="sname")
                                                   groups[grup].students[stud].setSurn(attr.value().toString());
                                               if(attr.name()=="viability")
                                                   groups[grup].students[stud].isViable=QString::compare(attr.value().toString(),"true")?true:false;
                                               if(attr.name()=="absence")
                                                    groups[grup].students[stud].absence=attr.value().toDouble();
                                           }
                                           readen.readNext();
                                           while (!(readen.tokenType() == QXmlStreamReader::EndElement && readen.name() == "stud"))
                                              {
                                                  if (readen.tokenType() == QXmlStreamReader::StartElement){
                                                      if (readen.name()=="averages")
                                                      {
                                                          foreach(const QXmlStreamAttribute  &attr,readen.attributes()){
                                                               groups[grup].students[stud].middleGrade[attr.name().toInt()]=attr.value().toDouble();
                                                          }
                                                          readen.readNext();
                                                      }
                                                      if (readen.name()=="grades"){
                                                          int  i =0;
                                                          foreach(const QXmlStreamAttribute  &attr,readen.attributes()){
                                                               groups[grup].students[stud].grades[attr.name().toInt()][i]=attr.value().toString();
                                                               i++;
                                                               if(i==14)i=0;
                                                          }
                                                          readen.readNext();
                                                      }
                                                  }
                                           }
                                   }

                            }
                    }
                               }
                               readen.readNext();
                           }

                    }
     file.close();
}
}
        /*if(readen.isStartElement()){
            grup = readen.name().toInt();
            readen.readNext();
           if(readen.isStartElement()){
                stud=readen.name().toInt();
                foreach(const QXmlStreamAttribute  &attr,readen.attributes()){
                    if(attr.name().toString()=="name")
                        groups[grup].students[stud].setName(attr.value().toString());
                    if(attr.name().toString()=="sname")
                        groups[grup].students[stud].setSurn(attr.value().toString());
                    if(attr.name().toString()=="viability")
                        groups[grup].students[stud].isViable=QString::compare(attr.value().toString(),"true")?true:false;
                    if(attr.name().toString()=="absence")
                         groups[grup].students[stud].absence=attr.value().toDouble();
                }
                readen.readNext();
                if(readen.isStartElement()){
                    foreach(const QXmlStreamAttribute  &attr,readen.attributes()){
                         groups[grup].students[stud].middleGrade[attr.name().toInt()]=attr.value().toDouble();
                    }
                    readen.readNext();
                    if  (readen.isStartElement()){
                        int i=0;
                        foreach(const QXmlStreamAttribute  &attr,readen.attributes()){
                             groups[grup].students[stud].grades[attr.name().toInt()][i]=attr.value().toString();
                             i++;
                             if(i==14)i=0;
                        }
                    }
                }
           }
        }*/
int Widget::createmsgSignal(QString mesg){
    QMessageBox msg;
    msg.setWindowTitle("Журнал");
    msg.setText("Внимание");
    msg.setInformativeText(mesg);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);
    return msg.exec();
}
void Widget::setupTables ()
{
    if (currentGroup!=-1)
    {
        formateTable();
int i=0;
ui->tableWidget->clearContents();
ui->tableWidget->setRowCount(0);
while (groups[currentGroup].students[i].getName() != ""){
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
         QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(groups[currentGroup].students[i].getSurn()+" "+groups[currentGroup].students[i].getName());
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,item);
        if(currentSubject==-1){
            for(int j =2;j<ui->tableWidget->columnCount()-1;j++){
                if  (groups[currentGroup].students[i].middleGrade[j-2]!=6)
                  ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString::number(groups[currentGroup].students[i].middleGrade[j-2],'f',1)));
            }
            ui->tableWidget->setItem(i,ui->tableWidget->columnCount()-1,new QTableWidgetItem(QString("%1 %").arg(QString::number(groups[currentGroup].students[i].absence*100,'f',1))));
        }
        i++;
}
restrictEdit();
    }        
    }
void Widget::setupGrades()
{
    if (currentSubject!=-1){
        formateTable();
    for (int j=0; j<ui->tableWidget->rowCount();j++){
    for (int i = 2;i<ui->tableWidget->columnCount()-1;i++)
    {
ui->tableWidget->setItem(j,i,new QTableWidgetItem(groups[currentGroup].students[j].grades[currentSubject][i-2]));
        if (ui->tableWidget->item(j,1)->background()==QBrush(Qt::yellow))
   {
            if (!ui->tableWidget->item(j,i))
              ui->tableWidget->setItem(j,i,new QTableWidgetItem);
        ui->tableWidget->item(j,i)->setBackground(QBrush(Qt::yellow));
    }
}
    }
    enableEdit();
}
}
void Widget::formateTable(){
    if (currentGroup!=-1&&currentSubject==-1){
        int i=0;
        ui->tableWidget->setColumnCount(2);
        QStringList list;
        switch(currentGroup){
   case 0 : {list=one;break;}
   case 1: case 2: case 3:  {list=twotofour;break;}
   case 4: case 5: {list=fivetosix;break;}
   case 6: case 7: case 8: case 9: case 10:{list=seventoeleven;break;}
        }
while (list.size()>i)
{
    if (ui->tableWidget->columnCount()<=i+2)
        ui->tableWidget->setColumnCount(ui->tableWidget->columnCount()+1);
    ui->tableWidget->setHorizontalHeaderItem(2+i, new QTableWidgetItem(QString("Ср. оценка\n%1").arg(list[i])));
     ui->tableWidget->setColumnWidth(i+2,85);
    i++;
}
if (ui->tableWidget->columnCount()<=i+2)
    ui->tableWidget->setColumnCount(ui->tableWidget->columnCount()+1);
 ui->tableWidget->setHorizontalHeaderItem(2+i, new QTableWidgetItem("Посещаемость"));
  ui->tableWidget->setColumnWidth(i+2,100);
    ui->tableWidget->setColumnWidth(1,200);
    }
    if (currentSubject!=-1){
        ui->tableWidget->setColumnCount(2);
        for (int i =1;i<16;i++){
            if(i!=15){
                if (ui->tableWidget->columnCount()<=i+1)
                    ui->tableWidget->setColumnCount(ui->tableWidget->columnCount()+1);
                ui->tableWidget->setHorizontalHeaderItem(1+i, new QTableWidgetItem(QString::number(i)));
            }
            else {
                if (ui->tableWidget->columnCount()<=i+1)
                    ui->tableWidget->setColumnCount(ui->tableWidget->columnCount()+1);
                ui->tableWidget->setHorizontalHeaderItem(1+i, new QTableWidgetItem("Ср. оценка"));
            }
        }
    }
if (currentGroup==-1&&currentSubject==-1)
    ui->tableWidget->setColumnCount(2);
}
bool Widget::checkGrade(QString Check){
    if ((Check[0]>49&&Check[0]<54)||Check=="н")
        return true;
    else return false;
}
QString receivedname="";
QString receivedsurn ="";
void Widget::receiveData(QString Name,QString Surn){
    if (Name!=""&&Surn!=""){
        receivedname=Name;
        receivedsurn=Surn;
    }
    else {receivedname="";
        receivedsurn="";}
}
void Widget::restrictEdit(){
    if(ui->tableWidget->rowCount()>0&&ui->tableWidget->columnCount()>0){
        for (int i =0;i<ui->tableWidget->rowCount();i++)
            for (int j=2;j<ui->tableWidget->columnCount();j++)
            {
                if (!ui->tableWidget->item(i,j))
                  ui->tableWidget->setItem(i,j,new QTableWidgetItem);
            ui->tableWidget->item(i,j)->setFlags(ui->tableWidget->item(i,j)->flags() & ~Qt::ItemIsEditable);
            }
    }
}
void Widget::enableEdit(){
    if(ui->tableWidget->rowCount()>0&&ui->tableWidget->columnCount()>0){
        for (int i =0;i<ui->tableWidget->rowCount();i++)
            for (int j=2;j<ui->tableWidget->columnCount()-1;j++)
            {
                if (!ui->tableWidget->item(i,j))
                  ui->tableWidget->setItem(i,j,new QTableWidgetItem);
            ui->tableWidget->item(i,j)->setFlags(ui->tableWidget->item(i,j)->flags() | Qt::ItemIsEditable);
            }
    }
}
void Widget::on_pushButton_clicked()
{
    //QString text  = QInputDialog::getText(0, "Input dialog","Новый ученик", QLineEdit::Normal,"");
    ndlg = new NameDialog();
    connect(ndlg,SIGNAL(sendData(QString,QString)),this,SLOT(receiveData(QString,QString)));
    ndlg->exec();
    if (receivedname!="")
    {       if(ui->tableWidget->rowCount()<30){
        QString text=receivedsurn+" "+receivedname;
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
   QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(text);
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,item);
        }
        else createmsgSignal("Вы ввели максимальное количество учеников!");
    }
}
void Widget::on_tableWidget_cellChanged(int row, int column)
{
    if (column==1 && currentGroup!=-1){
        if  (groups[currentGroup].students[row].getName()==""){
     groups[currentGroup].students[row]=Student(receivedname,receivedsurn);
     receivedname="";
     receivedsurn="";
        }
    }
     else if (column>1 && column!=16 && currentSubject !=-1&&ui->tableWidget->item(row,column)->text().size()<2&&checkGrade(ui->tableWidget->item(row,column)->text())){
     groups[currentGroup].students[row].grades[currentSubject][column-2]=ui->tableWidget->item(row,column)->text();
     groups[currentGroup].students[row].updateQuality();
     if (groups[currentGroup].students[row].middleGrade[currentSubject]!=6){
     ui->tableWidget->setItem(row,16,new QTableWidgetItem(QString::number(groups[currentGroup].students[row].middleGrade[currentSubject],'f',1)));
     ui->tableWidget->item(row,16)->setFlags(ui->tableWidget->item(row,16)->flags() & ~Qt::ItemIsEditable);
     }
    }
    else  if(column!=16&&column>1&&currentSubject!=-1) ui->tableWidget->item(row,column)->setText("");
    //EDIT  VVV
    if(ui->tableWidget->item(row,column)->checkState()==Qt::Checked && ui->pushButton_2->text()!="Подтвердить/отменить")
    {
        //QString text = QInputDialog::getText(0,"Input dialog","Введите имя заново",QLineEdit::Normal,ui->tableWidget->item(row,1)->text());
        ndlg = new NameDialog();
        connect(ndlg,SIGNAL(sendData(QString,QString)),this,SLOT(receiveData(QString,QString)));
        connect(this,SIGNAL(sendData(QString,QString)),ndlg,SLOT(receiveData(QString,QString)));
        emit sendData(groups[currentGroup].students[row].getName(),groups[currentGroup].students[row].getSurn());
        ndlg->exec();
        if (receivedname != ""||receivedsurn!="")
        {
            groups[currentGroup].students[row].setName(receivedname);
             groups[currentGroup].students[row].setSurn(receivedsurn);
             receivedname="";
             receivedsurn="";
            ui->tableWidget->hideColumn(0);
            if (ui->comboBox_2->currentIndex()==-1){
                setupTables();
            }
            else{
                setupTables();
                setupGrades();
            }
            ui->pushButton->setEnabled(true);
               ui->pushButton_3->setEnabled(true);
               ui->pushButton_4->setEnabled(true);
               ui->pushButton_5->setEnabled(true);
            ui->comboBox->setEnabled(true);
            ui->comboBox_2->setEnabled(true);
        }
}
    //SET  COLOR  VVV
   /*if (ui->tableWidget->item(row,1)->background()==QBrush(Qt::yellow))
    {
        for (int i = 1; i<ui->tableWidget->columnCount();i++){
             if (!ui->tableWidget->item(row,i))
               ui->tableWidget->setItem(row,i,new QTableWidgetItem);
         ui->tableWidget->item(row,i)->setBackground(QBrush(Qt::yellow));
        }
     }*/
 }
void Widget::on_pushButton_2_clicked()
{
    //DELETE VVV
    bool flag = false;
    int  once;
    if (ui->pushButton_2->text()=="Подтвердить/отменить")
    {
        int deletesCount=0;
        for (int i = 0;i<ui->tableWidget->rowCount();i++)
        {
            if (ui->tableWidget->item(i,0)->checkState()==Qt::Checked)
            {
                if (!flag) once = createmsgSignal("Вы действительно хотите удалить выбранных учеников?");
                if (!flag && once  ==  QMessageBox::Ok) flag = true;
                if (flag){
                for (int j=i-deletesCount;j<ui->tableWidget->rowCount()-deletesCount;j++)
                {
                    if(j!=29)
                    groups[currentGroup].students[j]=groups[currentGroup].students[j+1];
                    else
                        groups[currentGroup].students[j]=Student("");
                }
                deletesCount++;
                }
            }
        }
        flag = true;
        ui->tableWidget->hideColumn(0);
        ui-> pushButton_2->setText("Удаление учеников");
         if (ui->comboBox_2->currentIndex()==-1){
             setupTables();
         ui->pushButton->setEnabled(true);
         ui->comboBox->setEnabled(true);
         ui->comboBox_2->setEnabled(true);
         }
         else{
             setupTables();
             setupGrades();
             ui->pushButton->setEnabled(true);
             ui->comboBox->setEnabled(true);
            ui-> comboBox_2->setEnabled(true);
         }
     }
    //CHECKBOXES  VVV
    if (ui->pushButton_2->text()=="Удаление учеников"&&ui->tableWidget->rowCount()!=0&&!flag){
            ui->tableWidget->showColumn(0);
        ui->pushButton->setEnabled(false);
        ui->comboBox->setEnabled(false);
       ui->comboBox_2->setEnabled(false);
    for (int i = 0; i<ui->tableWidget->rowCount();i++)
    {
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setData(Qt::CheckStateRole,Qt::Unchecked);
        check->setFlags(check->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(i,0,check);
    }
    ui->pushButton_2->setText("Подтвердить/отменить");
    }
    }
void Widget::on_comboBox_activated(int index)
{
  currentGroup = index;
ui->tableWidget->hideColumn(0);
  if (currentGroup!=-1){
      ui->pushButton->setEnabled(true);
      ui->pushButton->setEnabled(true);
      ui->pushButton_2->setEnabled(true);
      ui->pushButton_3->setEnabled(true);
      ui->pushButton_5->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
     ui->comboBox_2->clear();
     switch(currentGroup){
case 0 : {ui->comboBox_2->addItems(one);break;}
case 1: case 2: case 3:  {ui->comboBox_2->addItems(twotofour);break;}
case 4: case 5: {ui->comboBox_2->addItems(fivetosix);break;}
case 6: case 7: case 8: case 9: case 10:{ui->comboBox_2->addItems(seventoeleven);break;}
     }
     ui->comboBox_2->setCurrentIndex(-1);
     currentSubject=-1;
  }
  else{
      ui->pushButton->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
  }   
  setupTables();
}
void Widget::on_comboBox_2_activated(int index)
{
    if  (index!=-1){
    currentSubject=index;    
 setupGrades();
}}
void Widget::on_pushButton_3_clicked()
{
    if (ui->tableWidget->rowCount()!=0){
    ui->pushButton->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->comboBox->setEnabled(false);
   ui->comboBox_2->setEnabled(false);

for (int i = 0; i<ui->tableWidget->rowCount();i++)
{
    QTableWidgetItem *check = new QTableWidgetItem();
    check->setData(Qt::CheckStateRole,Qt::Unchecked);
    check->setFlags(check->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(i,0,check);
}
ui->tableWidget->showColumn(0);
    }
}


void Widget::on_pushButton_4_clicked()
{
    bool is_global = true;
    QString gotname = "";
    if (currentGroup!=-1)
        if (createmsgSignal("Вы хотите начать поиск только по выбранному классу?")==QMessageBox::Ok)
            is_global = false;
    QString text = QInputDialog::getText(0,"Input dialog","Поиск",QLineEdit::Normal,"");
    if (text != ""){
    if (is_global){
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        ui->comboBox->setCurrentIndex(-1);
        ui->comboBox_2->setCurrentIndex(-1);
        currentSubject=-1;
        currentGroup=-1;
        formateTable();
        for (int i = 0; i<grup_num;i++)
            for (int j = 0;j<stud_num;j++){
                gotname = groups[i].students[j].getName();
                int c=0;
                while (gotname[c].toLower()==text[c].toLower() && gotname.size()>=c && text.size()>=c)
                {
                    c++;
                }
                if (c>2 && gotname[c-1].toLower()==text[c-1].toLower())
                {
                    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                     ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(QString("%1 Класс").arg(QString::number(i+1))));
                    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(groups[i].students[j].getSurn()+" "+groups[i].students[j].getName()));
                    ui->tableWidget->item(ui->tableWidget->rowCount()-1,1)->setBackground(QBrush(Qt::white));
                }else {
                    gotname = groups[i].students[j].getSurn();
                    int c=0;
                    while (gotname[c].toLower()==text[c].toLower() && gotname.size()>=c && text.size()>=c)
                    {
                        c++;
                    }
                    if (c>2 && gotname[c-1].toLower()==text[c-1].toLower())
                    {
                        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
                         ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(QString("%1 Класс").arg(QString::number(i+1))));
                        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(groups[i].students[j].getSurn()+" "+groups[i].students[j].getName()));
                        ui->tableWidget->item(ui->tableWidget->rowCount()-1,1)->setBackground(QBrush(Qt::white));
                    }
                }
        }
        ui->tableWidget->showColumn(0);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
    }
            else {
            for (int j = 0;j<ui->tableWidget->rowCount();j++){
                gotname = groups[currentGroup].students[j].getName();
                int c=0;
                while (gotname[c].toLower()==text[c].toLower() && gotname.size()>c && text.size()>c)
                {
                    c++;
                }
                if (c>2 && gotname[c-1].toLower()==text[c-1].toLower())
                {
                    for (int i = 1; i<ui->tableWidget->columnCount();i++){
                         if (!ui->tableWidget->item(j,i))
                           ui->tableWidget->setItem(j,i,new QTableWidgetItem);
                     ui->tableWidget->item(j,i)->setBackground(QBrush(Qt::yellow));
                    }
                }else{
                    gotname = groups[currentGroup].students[j].getSurn();
                    int c=0;
                    while (gotname[c].toLower()==text[c].toLower() && gotname.size()>c && text.size()>c)
                    {
                        c++;
                    }
                    if (c>2 && gotname[c-1].toLower()==text[c-1].toLower())
                    {
                        for (int i = 1; i<ui->tableWidget->columnCount();i++){
                             if (!ui->tableWidget->item(j,i))
                               ui->tableWidget->setItem(j,i,new QTableWidgetItem);
                         ui->tableWidget->item(j,i)->setBackground(QBrush(Qt::yellow));
                        }
                    }
                }
        }
    }
    }
}
void Widget::on_tableWidget_cellClicked(int row, int column)
{
    if (row!=-1&&column!=-1){
    if (ui->tableWidget->item(row,1)->background()==QBrush(Qt::yellow))
    {
        for (int i = 1; i<ui->tableWidget->columnCount();i++){
            if (!ui->tableWidget->item(row,i))
              ui->tableWidget->setItem(row,i,new QTableWidgetItem);
         ui->tableWidget->item(row,i)->setBackground(QBrush(Qt::white));
        }
     }
    if (ui->tableWidget->columnCount()==2){
        QStringList klass=ui->tableWidget->item(row,0)->text().split(" ");
        QStringList surn=ui->tableWidget->item(row,1)->text().split(" ");
        ui->comboBox->setCurrentIndex(klass[0].toInt()-1);
        currentGroup=klass[0].toInt()-1;
        Widget::on_comboBox_activated(currentGroup);
        for(int j=0;j<ui->tableWidget->rowCount();j++){
            if(groups[currentGroup].students[j].getSurn()==surn[0] && groups[currentGroup].students[j].getName()==surn[1]){
        for (int i = 1; i<ui->tableWidget->columnCount();i++){
             if (!ui->tableWidget->item(j,i))
               ui->tableWidget->setItem(j,i,new QTableWidgetItem);
         ui->tableWidget->item(j,i)->setBackground(QBrush(Qt::yellow));
    }
        break;
            }
        }
    }
}
}
void Widget::on_pushButton_5_clicked()
{
    currentGroup=-1;
    ui->comboBox->setCurrentIndex(-1);
    currentSubject=-1;
    ui->comboBox_2->setCurrentIndex(-1);
    ui->pushButton->setEnabled(false);
if (createmsgSignal("Процедура смены учебного года полностью автоматическая!\nУченики, у которых нет половины посещений или минимум среднего балла 2.5 по каждому из предметов, остаются на второй год.\nВы хотите начать новый учебный год?")==QMessageBox::Ok){
    int shift =0;
    int newshift=0;
    int currentshift =0;
    for (int i=grup_num-1;i>=0;i--){
        shift=newshift;
        newshift=0;
        currentshift=0;
    for (int j=0;j<stud_num;j++){
        if (i==grup_num-1){
            if(groups[i].students[j].isViable){
                groups[i].students[j] = Student("");
            }else if (groups[i].students[j].getName()!=""){
                groups[i].students[newshift]=Student(groups[i].students[j].getName(),groups[i].students[j].getSurn());
                if (newshift!=j)
                groups[i].students[j]=Student("");
                newshift++;
            }
        } else {
            if (groups[i].students[j].isViable&&groups[i].students[j].getName()!=""){
                groups[i+1].students[currentshift+shift]=Student(groups[i].students[j].getName(),groups[i].students[j].getSurn());
                groups[i].students[j]=Student("");
                currentshift++;
            } else if(groups[i].students[j].getName()!=""){
                groups[i].students[newshift]=Student(groups[i].students[j].getName(),groups[i].students[j].getSurn());
                if (newshift!=j)
                groups[i].students[j]=Student("");
                newshift++;
            }
        }
    }
    }
}
ui->tableWidget->clearContents();
ui->tableWidget->setRowCount(0);
}
void Widget::on_pushButton_6_clicked()
{
    saveData();
}

