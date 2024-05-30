#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "surakarta_common.h"
#include "surakarta_piece.h"
#include "result1.h"
#include "result2.h"
#include "result3.h"
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    for(int i=0;i<24;i++)
    {
        p[i].init(i);
    }
    game=new SurakartaGame(ui->centralwidget,6);
    game->StartGame();
    selectid=-1;
    time=10;
    ti = new QTimer(this);
    ui->setupUi(this);
    this->page3 = new class result1;
    this->page4 = new class result2;
    this->page2 = new class result3;
    QFont font;
    font.setPointSize(24);
    ui->label_2->setFont(font);
    font.setPointSize(12);
    ui->label_3->setFont(font);
    ui->ip_edit->setText(ip);
    ui->port_edit->setText(QString::number(port));
    ui->move_button->setEnabled(false);
    ui->giveup_button->setEnabled(false);
    ui->ai_button->setEnabled(false);
    ui->disconnect_button->setEnabled(false);
    ui->receive_edit->setReadOnly(true);

    socket = new NetworkSocket(new QTcpSocket(this), this);

    connect(socket->base(), &QTcpSocket::connected, this, &MainWindow::connected_successfully);
    connect(ui->connect_button, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(ui->disconnect_button, &QPushButton::clicked, this, &MainWindow::disconnectFromServer);
    connect(ui->move_button, &QPushButton::clicked, this, &MainWindow::move);
    connect(socket, &NetworkSocket::receive, this, &MainWindow::receiveMessage);
    connect(ti,&QTimer::timeout,this,&MainWindow::TimeOut);
    connect(ui->giveup_button,&QPushButton::clicked,[=](){
        socket->send(NetworkData(OPCODE::GIVEUP_OP, player1, "", ""));
    });
    connect(ui->ai_button,&QPushButton::clicked,[=](){
        isAI=!isAI;
    });
    this->socket->base()->waitForConnected(2000);
    file.setFileName("D:\\Qt\\Qthomework\\QtNetWorkDemo\\ClientDemo\\recoadc.txt");
}

void MainWindow::connected_successfully() {
    ui->connect_button->setEnabled(false);
    ui->disconnect_button->setEnabled(true);
    ui->move_button->setEnabled(true);
    ui->giveup_button->setEnabled(true);
    ui->ai_button->setEnabled(true);
    ui->port_edit->setReadOnly(true);
    ui->ip_edit->setText("Connected");
    ui->ip_edit->setReadOnly(true);
    ready();
}

void MainWindow::connectToServer() {
    this->ip = ui->ip_edit->text();
    this->port = ui->port_edit->text().toInt();
    socket->hello(ip, port);
    this->socket->base()->waitForConnected(2000);
}

void MainWindow::disconnectFromServer() {
    socket->send(NetworkData(OPCODE::LEAVE_OP, "", "", ""));
    socket->bye();
    ui->connect_button->setEnabled(true);
    ui->disconnect_button->setEnabled(false);
    ui->move_button->setEnabled(false);
    ui->giveup_button->setEnabled(false);
    ui->port_edit->setReadOnly(false);
    ui->ip_edit->setReadOnly(false);
    ui->ai_button->setEnabled(false);
    ui->ip_edit->setText(ip);
    ti->stop();
    ui->label_2->setText("倒计时");
    ui->label_3->setText("玩家信息");
    time=10;
    for(int i=0;i<24;i++){
        p[i].init(i);
        selectid=-1;
        update();
        time=-1;
        QPainter painter(this);
        for(int i=0;i<24;i++){
            drawpiece(painter,i);
        }
    }
}

void MainWindow::move() {
    QString message = ui->send_edit->text();
    QString whole_block = message;
    auto blocks = whole_block.split(' ');
    QString move[3];
    int i=0;
    for (const auto& block : blocks) {
        move[i++]=block;
    }
    socket->send(NetworkData(OPCODE::MOVE_OP, move[0], move[1],move[2]));
    file.write((move[0]+"-"+move[1]+" ").toUtf8());
    file.flush();
    ui->send_edit->clear();
    ti->stop();
    ui->label_2->setText("倒计时");
    time=10;
}
void MainWindow::ready() {
    QString message = ui->send_edit->text();
    QString whole_block = message;
    auto blocks = whole_block.split(' ');
    QString move[3];
    int i=0;
    for (const auto& block : blocks) {
        move[i++]=block;
    }
    player1=move[0];
    socket->send(NetworkData(OPCODE::READY_OP, move[0], move[1], move[2]));
    ui->send_edit->clear();
}
void MainWindow::receiveMessage(NetworkData data) {
    if(data.op==OPCODE::REJECT_OP)
    {
        ui->label_3->setText("请重新输入");
    }
    if(data.op==OPCODE::READY_OP)
    {
        player2=data.data1;
        if(data.data2=="1")
        {
            game->game_info_->current_player_=SurakartaPlayer::BLACK;
            playercolor2=piececolor::WHITE;
            playercolor1=piececolor::BLACK;
            QString content=player1;
            content += " ";
            content +="BLACK<br>";
            content +=player2;
            content += " ";
            content +="WHITE";
            ui->label_3->setText(content);
            //isAI=true;
            file.open(QIODevice::WriteOnly);
            //agent=new SurakartaAgentMine(game->GetBoard(),game->GetGameInfo(),game->GetRuleManager());
            ti->start(1000);
            //SurakartaMove best_move=agent->CalculateMove();
            //game->Move(best_move);
            //QString str=QString::number(best_move.from.x);
            //QString str1=QString::number(best_move.from.y);
            //str=change(str);
            //str+=str1;
            //str1=QString::number(best_move.to.x);
            //QString str2=QString::number(best_move.to.y);
           //str1=change(str1);
            //str1+=str2;
            //file.write((str+"-"+str1+" ").toUtf8());
            //file.flush();
            //socket->send(NetworkData(OPCODE::MOVE_OP,str,str1, ""));
        }
        if(data.data2=="2")
        {
            game->game_info_->current_player_=SurakartaPlayer::WHITE;
            playercolor2=piececolor::BLACK;
            playercolor1=piececolor::WHITE;
            QString content=player1;
            content += " ";
            content +="WHITE<br>";
            content +=player2;
            content += " ";
            content +="BLACK";
            ui->label_3->setText(content);
            //isAI=true;
        }
    }
    if(data.op==OPCODE::END_OP)
    {
        if(data.data3=="1")
        {
            if(playercolor1==piececolor::BLACK)
            {
                this->page3->show();
            }
            if(playercolor1==piececolor::WHITE)
            {
                this->page4->show();
            }
        }
        else if(data.data3=="2")
        {
            if(playercolor1==piececolor::BLACK)
            {
                this->page4->show();
            }
            if(playercolor1==piececolor::WHITE)
            {
                this->page3->show();
            }
        }
    }
    if(data.op==OPCODE::MOVE_OP)
    {
        step++;
        QString content=data.data1;
        content += "->";
        content +=data.data2;
        ui->receive_edit->setText(content);
        int fromto[4]={0};
        fromto[0]=data.data1[0].unicode()-65;
        fromto[1]=data.data1[1].unicode()-48;
        fromto[2]=data.data2[0].unicode()-65;
        fromto[3]=data.data2[1].unicode()-48;
        for (int i = 0; i <24; ++i) {
            if(p[i]._row==fromto[2]&&p[i]._col==fromto[3]&&p[i]._dead==false)
            {
                p[i]._dead=true;
            }
        }
        for (int i = 0; i <24; ++i){
            if(p[i]._row==fromto[0]&&p[i]._col==fromto[1]&&p[i]._dead==false)
            {
                selectid=i;
                p[i]._row=fromto[2];
                p[i]._col=fromto[3];
                break;
            }
        }
        QPainter painter(this);
        for(int x=0;x<24;x++){
            drawpiece(painter,x);
        }
        update();
        if((step%2==0&&playercolor1==piececolor::WHITE)||(step%2!=0&&playercolor1==piececolor::BLACK))
        {
            SurakartaMove move;
            move.from.y=fromto[0];
            move.from.x=fromto[1];
            move.to.y=fromto[2];
            move.to.x=fromto[3];
            if(game->game_info_->current_player_==PieceColor::BLACK)
                move.player=PieceColor::WHITE;
            if(game->game_info_->current_player_==PieceColor::WHITE)
                move.player=PieceColor::BLACK;
            game->Move(move);
        }
        if((step%2!=0&&playercolor1==piececolor::WHITE)||(step%2==0&&playercolor1==piececolor::BLACK))
        {
         ti->start(1000);
        //if(isAI==true)
        //{
           // file.open(QIODevice::WriteOnly);
            //agent=new SurakartaAgentMine(game->GetBoard(),game->GetGameInfo(),game->GetRuleManager());
            //SurakartaMove best_move=agent->CalculateMove();
            //game->Move(best_move);
            //QString str=QString::number(best_move.from.y);
            //QString str1=QString::number(best_move.from.x);
            //str=change(str);
            //str+=str1;
            //str1=QString::number(best_move.to.y);
            //QString str2=QString::number(best_move.to.x);
            //str1=change(str1);
            //str1+=str2;
            //socket->send(NetworkData(OPCODE::MOVE_OP,str,str1, ""));
            //ti->stop();
            //ui->label_2->setText("倒计时");
            //time=10;
        //}
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
    delete socket;
}

QPoint MainWindow::center(int row,int col){
    QPoint ret;
    ret.rx()=100+col*50;
    ret.ry()=100+row*50;
    return ret;
}

QPoint MainWindow::center(int id){
    return center(p[id]._row,p[id]._col);
}

QString MainWindow::change(QString a)
{
    if(a=="0")
    {
        return "A";
    }
    if(a=="1")
    {
        return "B";
    }
    if(a=="2")
    {
        return "C";
    }
    if(a=="3")
    {
        return "D";
    }
    if(a=="4")
    {
        return "E";
    }
    if(a=="5")
    {
        return "F";
    }
    return 0;
}

void MainWindow::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);


    int d=50;
    _r=d/2;

    //画竖线
    for(int i=50;i<=300;i+=50){
        painter.drawLine(QPoint(i+50,100),QPoint(i+50,350));
    }
    //画横线
    for(int i=50;i<=300;i+=50){
        painter.drawLine(QPoint(100,i+50),QPoint(350,i+50));
    }
    //画外旋线
    painter.drawArc(50,50,100,100,0,270*16);
    painter.drawArc(0,0,200,200,0,270*16);
    painter.drawArc(300,50,100,100,270*16,270*16);
    painter.drawArc(250,0,200,200,270*16,270*16);
    painter.drawArc(50,300,100,100,90*16,270*16);
    painter.drawArc(0,250,200,200,90*16,270*16);
    painter.drawArc(300,300,100,100,180*16,270*16);
    painter.drawArc(250,250,200,200,180*16,270*16);
    //画棋子
    for(int i=0;i<24;i++){
        drawpiece(painter,i);
    }
}

void MainWindow::drawpiece(QPainter&painter,int id){
    if(p[id]._dead){
        return;
    }
    if(id==selectid){
        painter.setBrush(Qt::red);
        painter.drawEllipse(center(id),_r,_r);

    }
    else{
        if(!p[id]._black){
            painter.setBrush(Qt::white);
            painter.drawEllipse(center(id),_r,_r);
        }

        if(p[id]._black){
            painter.setBrush(Qt::black);
            painter.drawEllipse(center(id),_r,_r);

        }
    }
}

void MainWindow::TimeOut()
{
    QString qstime= QString::number(time);
    if(time>=0)
    {
        ui->label_2->setText(qstime);
        time--;
        update();
    }
    else
    {
        //socket->send(NetworkData(OPCODE::TIMEOUT_END_OP, "","", ""));
        ui->label_2->setText(qstime);
        ti->stop();
        ui->label_2->setText("倒计时");
        time=10;
    }
}

