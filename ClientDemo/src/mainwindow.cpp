#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "surakarta_common.h"
//#include "surakartapiece.h"
#include "result1.h"
#include "result2.h"
#include "result3.h"
#include "Step.h"
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
    step=0;
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



int MainWindow::getId(int row,int col)
{
    for(int i=0;i<24;i++)
    {
        if(p[i]._row==row&&p[i]._col==col&&!p[i]._dead)
        {
            return i;
        }
    }
    return -1;
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
    // if(data.op==OPCODE::REJECT_OP)
    // {
    //     ui->label_3->setText("请重新输入");
    // }
    if(data.op==OPCODE::READY_OP)
    {
        player2=data.data1;
        if(data.data2=="1")
        {
            // game->game_info_->currentplayer_=SurakartaPlayer::BLACK;
            playercolor2=piececolor::WHITE;
            playercolor1=piececolor::BLACK;
            QString content=player1;
            content += " ";
            content +="BLACK<br>";
            content +=player2;
            content += " ";
            content +="WHITE";
            ui->label_3->setText(content);
            isAI=true;
            black_computer_move();
            file.open(QIODevice::WriteOnly);
            // if(playercolor1==piececolor::BLACK)
            //     game->game_info_->currentplayer_=PieceColor::BLACK;
            // if(playercolor1==piececolor::WHITE)
            //     game->game_info_->currentplayer_=PieceColor::WHITE;
            // agent=new SurakartaAgentMine(game->GetBoard(),game->GetGameInfo(),game->GetRuleManager());
            ti->start(1000);
            // SurakartaMove best_move=agent->CalculateMove();
            // game->Move(best_move);
            // if(playercolor1==piececolor::BLACK)
            //     game->game_info_->currentplayer_=PieceColor::BLACK;
            // if(playercolor1==piececolor::WHITE)
            //     game->game_info_->currentplayer_=PieceColor::WHITE;
            Step*step=ai_steps.back();
            QString str=QString::number(step->_rowPre);
            QString str1=QString::number(step->_colPre);
            str=change(str);
            str+=str1;
            str1=QString::number(step->_rowNow);
            QString str2=QString::number(step->_colNow);
            str1=change(str1);
            str1+=str2;
            file.write((str+"-"+str1+" ").toUtf8());
            file.flush();
            socket->send(NetworkData(OPCODE::MOVE_OP,str,str1, ""));
        }
        if(data.data2=="2")
        {
            //game->game_info_->currentplayer_=SurakartaPlayer::WHITE;
            playercolor2=piececolor::BLACK;
            playercolor1=piececolor::WHITE;
            QString content=player1;
            content += " ";
            content +="WHITE<br>";
            content +=player2;
            content += " ";
            content +="BLACK";
            ui->label_3->setText(content);
            isAI=true;
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
        qDebug()<<"Step:"<<step;
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
        // QPainter painter(this);
        // for(int x=0;x<24;x++)
        // {
        //     drawpiece(painter,x);
        // }
        update();
        //if((step%2==0&&playercolor1==piececolor::WHITE)||(step%2!=0&&playercolor1==piececolor::BLACK))
        //{
        // SurakartaMove move;
        // move.from.y=fromto[0];
        // move.from.x=fromto[1];
        // move.to.y=fromto[2];
        // move.to.x=fromto[3];
        // if(game->game_info_->currentplayer_==PieceColor::BLACK)
        //     move.player=PieceColor::WHITE;
        // if(game->game_info_->currentplayer_==PieceColor::WHITE)
        //     move.player=PieceColor::BLACK;
        // game->Move(move);
        //}
        if((step%2==1&&playercolor1==piececolor::WHITE)||(step%2==0&&playercolor1==piececolor::BLACK))
        {
            ti->start(1000);
            if(isAI==true)
            {
                file.open(QIODevice::WriteOnly);
                // if(playercolor1==piececolor::BLACK)
                //     game->game_info_->currentplayer_=PieceColor::BLACK;
                // if(playercolor1==piececolor::WHITE)
                //     game->game_info_->currentplayer_=PieceColor::WHITE;
                // agent=new SurakartaAgentMine(game->GetBoard(),game->GetGameInfo(),game->GetRuleManager());
                // SurakartaMove best_move=agent->CalculateMove();
                // game->Move(best_move);
                if(playercolor1==piececolor::BLACK)
                {
                    black_computer_move();
                }
                else if(playercolor1==piececolor::WHITE)
                {
                    white_computer_move();
                }
                Step*step=ai_steps.back();
                QString str=QString::number(step->_rowPre);
                QString str1=QString::number(step->_colPre);
                str=change(str);
                str+=str1;
                str1=QString::number(step->_rowNow);
                QString str2=QString::number(step->_colNow);
                str1=change(str1);
                str1+=str2;
                file.write((str+"-"+str1+" ").toUtf8());
                file.flush();
                socket->send(NetworkData(OPCODE::MOVE_OP,str,str1, ""));
                // if(playercolor1==piececolor::BLACK)
                //     game->game_info_->currentplayer_=PieceColor::BLACK;
                // if(playercolor1==piececolor::WHITE)
                //     game->game_info_->currentplayer_=PieceColor::WHITE;
                ti->stop();
                ui->label_2->setText("倒计时");
                time=10;
            }
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
bool MainWindow::same_black(int row,int col)
{
    if(getId(row,col)==-1)
    {
        return false;
    }
    else
    {
        if(getId(row,col)<12&&getId(row,col)>=0)
        {
            return true;
        }
    }
    return false;
}
bool MainWindow::same_white(int row,int col)
{
    if(getId(row,col)==-1)
    {
        return false;
    }
    else
    {
        if(getId(row,col)<24&&getId(row,col)>=12)
        {
            return true;
        }
    }
    return false;
}
void MainWindow::getBlackStep(QVector<Step*>&steps)
{
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<6;j++)
        {
            for(int k=0;k<6;k++)
            {
                if(!p[i]._dead&&computer_canMove(i,j,k,getId(j,k))&&!same_black(j,k))
                {
                    saveStep(i,getId(j,k),j,k,steps);
                }
            }
        }
    }
}
void MainWindow::getWhiteStep(QVector<Step*>&steps)
{
    for(int i=12;i<24;i++)
    {
        for(int j=0;j<6;j++)
        {
            for(int k=0;k<6;k++)
            {
                if(!p[i]._dead&&computer_canMove(i,j,k,getId(j,k))&&!same_white(j,k))
                {
                    saveStep(i,getId(j,k),j,k,steps);
                }
            }
        }
    }
}
void MainWindow::black_computer_move()
{
    step++;
    qDebug()<<"Step:"<<step;
    int best_score=-100000;
    Step* best_step;
    getBlackStep(black_steps);
    for(auto i:black_steps)
    {
        int score=Calculate1(i);
        if(score>best_score)
        {
            best_score=score;
            best_step=i;

        }
    }
    saveStep(best_step->_moveid,best_step->_killid,best_step->_rowNow,best_step->_colNow,ai_steps);
    p[best_step->_moveid]._row=best_step->_rowNow;
    p[best_step->_moveid]._col=best_step->_colNow;
    if(best_step->_killid!=-1)
    {
        p[best_step->_killid]._dead=true;
    }
    qDebug()<<"Black ("<<best_step->_rowPre<<","<<best_step->_colPre<<") -> ("<<best_step->_rowNow<<","<<best_step->_colNow<<")";
    //blackturn=!blackturn;
    black_steps.clear();
    update();

}
void MainWindow::white_computer_move()
{
    step++;
    qDebug()<<"Step:"<<step;
    int best_score=-100000;
    Step* best_step;
    getWhiteStep(white_steps);
    for(auto i:white_steps)
    {
        int score=Calculate2(i);
        if(score>best_score)
        {
            best_score=score;
            best_step=i;

        }
    }
    saveStep(best_step->_moveid,best_step->_killid,best_step->_rowNow,best_step->_colNow,ai_steps);
    p[best_step->_moveid]._row=best_step->_rowNow;
    p[best_step->_moveid]._col=best_step->_colNow;
    if(best_step->_killid!=-1)
    {
        p[best_step->_killid]._dead=true;
    }
    qDebug()<<"White ("<<best_step->_rowPre<<","<<best_step->_colPre<<") -> ("<<best_step->_rowNow<<","<<best_step->_colNow<<")";
    //blackturn=!blackturn;
    white_steps.clear();
    update();
}
int MainWindow::Calculate1(Step*&step)
{
    int score=0;
    for(int i=12;i<24;i++)
    {
        if(!p[i]._dead&&computer_canMove(i,step->_rowNow,step->_colNow,step->_moveid))
        {
            score-=10;
        }
    }
    if(step->_killid!=-1)
    {
        score+=20;
    }
    else
    {
        if (step->_rowNow== 3&& step->_colNow ==3) {
            score += 5;
        }
        if (step->_rowNow == 2&& step->_colNow == 3) {
            score += 5;
        }
        if (step->_rowNow == 3&& step->_colNow == 2) {
            score += 5;
        }
        if (step->_rowNow == 2 && step->_colNow == 3) {
            score += 5;
        }
        if (step->_rowNow == 1 && step->_colNow == 1) {
            score += 5;
        }
        if (step->_rowNow == 1 && step->_colNow == 4) {
            score += 5;
        }
        if (step->_rowNow == 4 && step->_colNow == 1) {
            score += 5;
        }
        if (step->_rowNow == 4 && step->_colNow == (6 / 2 + 1)) {
            score += 5;
        }
        // 移动到边界的棋子分数+1
        if (step->_rowNow == 5 && step->_colNow == 5) {
            score += 1;
        }
        if (step->_rowNow == 0 && step->_colNow == 5) {
            score += 1;
        }
        if (step->_rowNow == 5 && step->_colNow == 0) {
            score += 1;
        }
        if (step->_rowNow == 0 && step->_colNow == 0) {
            score += 1;
        }
        // 落在边界线但不是角落的棋子分数+2；
        if (step->_rowNow == 0 && step->_colNow != 0 && step->_colNow != 5) {
            score += 2;
        }
        if (step->_rowNow == 5 && step->_colNow != 0 && step->_colNow != 5) {
            score += 2;
        }
        if (step->_colNow == 0 && step->_rowNow != 0 && step->_rowNow != 5) {
            score += 2;
        }
        if (step->_colNow == 5&& step->_rowNow != 5 && step->_rowNow != 0) {
            score += 2;
        }
        // 落在其他位置
        if (step->_colNow == 4 && (step->_rowNow == 1 || step->_rowNow == 4)) {
            score += 3;
        }
        if (step->_colNow == 1 && (step->_rowNow == 1 || step->_rowNow == 4)) {
            score += 3;
        }
        if (step->_rowNow == 3 && (step->_colNow == 1 || step->_colNow == 4)) {
            score += 3;
        }
        if (step->_rowNow == 1 && (step->_colNow == 1 || step->_colNow == 4)) {
            score += 3;
        }
    }
    return score;
    //随机算法
    // int score=0;
    // if(step->_killid!=-1)
    // {
    //     score+=20;
    // }
    // else
    // {
    //     score+=0;

    // }
    //  return score;
}
int MainWindow::Calculate2(Step*&step)
{
    int score=0;
    for(int i=0;i<12;i++)
    {
        if(!p[i]._dead&&computer_canMove(i,step->_rowNow,step->_colNow,step->_moveid))
        {
            score-=10;
        }
    }
    if(step->_killid!=-1)
    {
        score+=20;
    }
    else
    {
        if (step->_rowNow== 3&& step->_colNow ==3) {
            score += 5;
        }
        if (step->_rowNow == 2&& step->_colNow == 3) {
            score += 5;
        }
        if (step->_rowNow == 3&& step->_colNow == 2) {
            score += 5;
        }
        if (step->_rowNow == 2 && step->_colNow == 3) {
            score += 5;
        }
        if (step->_rowNow == 1 && step->_colNow == 1) {
            score += 5;
        }
        if (step->_rowNow == 1 && step->_colNow == 4) {
            score += 5;
        }
        if (step->_rowNow == 4 && step->_colNow == 1) {
            score += 5;
        }
        if (step->_rowNow == 4 && step->_colNow == (6 / 2 + 1)) {
            score += 5;
        }
        // 移动到边界的棋子分数+1
        if (step->_rowNow == 5 && step->_colNow == 5) {
            score += 1;
        }
        if (step->_rowNow == 0 && step->_colNow == 5) {
            score += 1;
        }
        if (step->_rowNow == 5 && step->_colNow == 0) {
            score += 1;
        }
        if (step->_rowNow == 0 && step->_colNow == 0) {
            score += 1;
        }
        // 落在边界线但不是角落的棋子分数+2；
        if (step->_rowNow == 0 && step->_colNow != 0 && step->_colNow != 5) {
            score += 2;
        }
        if (step->_rowNow == 5 && step->_colNow != 0 && step->_colNow != 5) {
            score += 2;
        }
        if (step->_colNow == 0 && step->_rowNow != 0 && step->_rowNow != 5) {
            score += 2;
        }
        if (step->_colNow == 5&& step->_rowNow != 5 && step->_rowNow != 0) {
            score += 2;
        }
        // 落在其他位置
        if (step->_colNow == 4 && (step->_rowNow == 1 || step->_rowNow == 4)) {
            score += 3;
        }
        if (step->_colNow == 1 && (step->_rowNow == 1 || step->_rowNow == 4)) {
            score += 3;
        }
        if (step->_rowNow == 3 && (step->_colNow == 1 || step->_colNow == 4)) {
            score += 3;
        }
        if (step->_rowNow == 1 && (step->_colNow == 1 || step->_colNow == 4)) {
            score += 3;
        }
    }
    return score;
    // int score=0;
    // if(step->_killid!=-1)
    //     {
    //         score+=20;
    //     }
    // else
    // {
    //     score+=0;

    // }
    //     return score;
}

bool MainWindow::judgeLoc(int moveid,int row,int col,int)
{
    int flag11=0,flag21=0,flag12=0,flag22=0;
    for(int i=0;i<24;i++){
        if(p[moveid]._row==a1[i].x&&p[moveid]._col==a1[i].y){
            flag11=1;
        }
        if(p[moveid]._row==b1[i].x&&p[moveid]._col==b1[i].y){
            flag12=1;
        }
        if(row==a1[i].x&&col==a1[i].y){
            flag21=1;
        }
        if(row==b1[i].x&&col==b1[i].y){
            flag22=1;
        }
    }
    if(flag11==1&&flag21==1)return true;
    if(flag12==1&&flag22==1)return true;
    return false;

}
bool MainWindow::computer_canMove(int moveid,int row,int col,int killid)
{
    //注意row,col是killid的row,col;
    //如果移动到己方棋子，则更新点击;
    if(killid!=-1)
    {
        if(p[moveid]._black==p[killid]._black)
        {
            return false;
        }
        if(p[moveid]._black!=p[killid]._black)
        {

            //处于角落的棋子不可能吃子或者被吃;
            if(col==0&&row==0)return false;
            if(col==0&&row==5)return false;
            if(col==5&&row==0)return false;
            if(col==5&&row==5)return false;
            if(p[moveid]._col==0&&p[moveid]._row==0)return false;
            if(p[moveid]._col==0&&p[moveid]._row==5)return false;
            if(p[moveid]._col==5&&p[moveid]._row==0)return false;
            if(p[moveid]._col==5&&p[moveid]._row==5)return false;

            //判断两者在同一条吃子路线上
            //判断两者中间是否有棋子;
            if(judgeLoc(moveid, row,col, killid))
            {
                if(judgeNum(moveid,row,col, killid))
                {
                    return true;
                }
            }
        }
    }
    else
    {
        int dr=p[moveid]._row-row;
        int dc=p[moveid]._col-col;
        int d=abs(dr)*10+abs(dc);
        if(d==10||d==1||d==11)
        {
            return true;
        }
    }
    return false;
}
int MainWindow::getLine(int moveid,int row,int col,int)
{
    int flag11=0,flag21=0,flag12=0,flag22=0;
    for(int i=0;i<24;i++)
    {
        if(p[moveid]._row==a1[i].x&&p[moveid]._col==a1[i].y)
        {
            flag11=1;
        }
        if(p[moveid]._row==b1[i].x&&p[moveid]._col==b1[i].y)
        {
            flag12=1;
        }
        if(row==a1[i].x&&col==a1[i].y)
        {
            flag21=1;
        }
        if(row==b1[i].x&&col==b1[i].y)
        {
            flag22=1;
        }
    }
    if(flag11==1&&flag21==1&&flag12==1&&flag22==1)
    {
        return 3;
    }
    else if(flag11==1&&flag21==1)
    {
        return 1;
    }
    else if(flag12==1&&flag22==1)
    {
        return 2;
    }
    return 0;
}
bool MainWindow::judgeNum(int moveid,int row,int col,int killid)
{
    int line=getLine(moveid,row,col,killid);
    if(line==0)
    {
        return false;
    }
    int flag11=0,flag12=0,flag21=0,flag22=0;
    if(line==1||line==3)
    {
        //得到移动的棋子在方向数组中的位置
        int count=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(a1[i].x==p[moveid]._row&&a1[i].y==p[moveid]._col)
            {
                count++;
                if(count==1)
                {
                    tmp1=i;
                }
                if(count==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count==1)
        {
            int sign1=0,sign2=0;//当sign1=0,sign2=1满足吃子条件
            if(a1[(tmp1+1)%24].x==row&&a1[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag11=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;a1[i%24].x!=row||a1[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        //注意要判断棋子是否存活;
                        if(p[j]._row==a1[i].x&&p[j]._col==a1[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(a1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(a1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag11=1;
                }
            }
        }
        if(count==2)
        {
            int sign1=0,sign2=0;//当sign1=0,sign2=1满足吃子条件
            if(a1[(tmp1+1)%24].x==row&&a1[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag11=1;
                }
            }
            else{
                for(int i=(tmp1+1)%24;a1[i%24].x!=row||a1[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++){
                        if(p[j]._row==a1[i].x&&p[j]._col==a1[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(a1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(a1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag11=1;
                }
            }
            sign1=0,sign2=0;
            if(a1[(tmp2+1)%24].x==row&&a1[(tmp2+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag11=1;
                }
            }
            else
            {
                for(int i=(tmp2+1)%24;a1[i%24].x!=row||a1[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(p[j]._row==a1[i].x&&p[j]._col==a1[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(a1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(a1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag11=1;
                }
            }
        }
        count=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(a2[i].x==p[moveid]._row&&a2[i].y==p[moveid]._col)
            {
                count++;
                if(count==1)
                {
                    tmp1=i;
                }
                if(count==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count==1)
        {
            int sign1=0,sign2=0;
            if(a2[(tmp1+1)%24].x==row&&a2[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag12=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;a2[i%24].x!=row||a2[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(p[j]._row==a2[i].x&&p[j]._col==a2[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(a2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(a2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag12=1;
                }
            }
        }
        if(count==2)
        {
            int sign1=0,sign2=0;
            if(a2[(tmp1+1)%24].x==row&&a2[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag12=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;a2[i%24].x!=row||a2[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(p[j]._row==a2[i].x&&p[j]._col==a2[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(a2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(a2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag12=1;
                }
            }
            sign1=0,sign2=0;
            if(a2[(tmp2+1)%24].x==row&&a2[(tmp2+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag12=1;
                }
            }
            else
            {
                for(int i=(tmp2+1)%24;a2[i].x!=row||a2[i].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(p[j]._row==a2[i].x&&p[j]._col==a2[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(a2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(a2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag12=1;
                }
            }

        }
        if(flag11||flag12)
        {
            return true;
        }
    }
    if(line==2||line==3)
    {
        int count=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(b1[i].x==p[moveid]._row&&b1[i].y==p[moveid]._col)
            {
                count++;
                if(count==1)
                {
                    tmp1=i;
                }
                if(count==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count==1)
        {
            int sign1=0,sign2=0;
            if(b1[(tmp1+1)%24].x==row&&b1[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag21=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;b1[i%24].x!=row||b1[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(p[j]._row==b1[i].x&&p[j]._col==b1[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(b1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(b1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag21=1;
                }
            }
        }
        if(count==2)
        {
            int sign1=0,sign2=0;
            if(b1[(tmp1+1)%24].x==row&&b1[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag21=1;
                }
            }
            else
            {
                for(int i=(tmp1+1)%24;b1[i%24].x!=row||b1[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(p[j]._row==b1[i].x&&p[j]._col==b1[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(b1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(b1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag21=1;
                }
            }
            sign1=0,sign2=0;
            if(b1[(tmp2+1)%24].x==row&&b1[(tmp2+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag21=1;
                }
            }
            else
            {
                for(int i=(tmp2+1)%24;b1[i%24].x!=row||b1[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(p[j]._row==b1[i].x&&p[j]._col==b1[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(b1[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(b1[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag21=1;
                }
            }
        }
        count=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(b2[i].x==p[moveid]._row&&b2[i].y==p[moveid]._col)
            {
                count++;
                if(count==1)
                {
                    tmp1=i;
                }
                if(count==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count==1)
        {
            int sign1=0,sign2=0;
            if(b2[(tmp1+1)%24].x==row&&b2[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag22=1;
                }
            }
            for(int i=(tmp1+1)%24;b2[i%24].x!=row||b2[i%24].y!=col;i++)
            {
                i%=24;
                for(int j=0;j<24;j++)
                {
                    if(p[j]._row==b2[i].x&&p[j]._col==b2[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                    {
                        sign1=1;
                        break;
                    }
                }
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    sign2=1;
                }
                if(p[moveid]._row==row)
                {
                    if(b2[i].x!=row)
                    {
                        sign2=1;
                    }
                }
                if(p[moveid]._col==col)
                {
                    if(b2[i].y!=col)
                    {
                        sign2=1;
                    }
                }
            }
            if(sign1==0&&sign2==1)
            {
                flag22=1;
            }
        }
        if(count==2){
            int sign1=0,sign2=0;
            if(b2[(tmp1+1)%24].x==row&&b2[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag22=1;
                }
            }
            else{
                for(int i=(tmp1+1)%24;b2[i%24].x!=row||b2[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++){
                        if(p[j]._row==b2[i].x&&p[j]._col==b2[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(b2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(b2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag22=1;
                }
            }
            sign1=0,sign2=0;
            if(b2[(tmp2+1)%24].x==row&&b2[(tmp2+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag22=1;
                }
            }
            else{
                for(int i=(tmp2+1)%24;b2[i%24].x!=row||b2[i%24].y!=col;i++)
                {
                    i%=24;
                    for(int j=0;j<24;j++)
                    {
                        if(p[j]._row==b2[i].x&&p[j]._col==b2[i].y&&j!=moveid&&j!=killid&&p[j]._dead==false)
                        {
                            sign1=1;
                            break;
                        }
                    }
                    if(p[moveid]._row!=row&&p[moveid]._col!=col)
                    {
                        sign2=1;
                    }
                    if(p[moveid]._row==row)
                    {
                        if(b2[i].x!=row)
                        {
                            sign2=1;
                        }
                    }
                    if(p[moveid]._col==col)
                    {
                        if(b2[i].y!=col)
                        {
                            sign2=1;
                        }
                    }
                }
                if(sign1==0&&sign2==1)
                {
                    flag22=1;
                }
            }
        }
        if(flag21||flag22)
        {
            return true;
        }
    }
    return false;
}

void MainWindow::saveStep(int moveid, int killid, int row, int col,QVector<Step*>&steps)
{
    Step*step=new Step;
    step->_rowPre=p[moveid]._row;
    step->_colPre=p[moveid]._col;
    step->_rowNow=row;
    step->_colNow=col;
    step->_killid=killid;
    step->_moveid=moveid;
    step->_blackTurnPre=blackturn;
    steps.append(step);
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
