#include "result3.h"
#include "ui_result3.h"

result3::result3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::result3)
{
    ui->setupUi(this);
    ui->label->setText("平局");
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        this->hide();
    });
}

result3::~result3()
{
    delete ui;
}
