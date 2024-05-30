#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new NetworkServer(this); // 创建一个服务器对象，其父对象为当前窗口
    file.setFileName("file:///D://Qt//Qthomework//QtServerDemo//records.txt");
    ui->ShowClient1->setReadOnly(true);
    ui->ShowClient2->setReadOnly(true);

    connect(ui->PortButton, &QPushButton::clicked, this, &MainWindow::listen_port); // 开启服务器监听
    connect(ui->restart, &QPushButton::clicked, this, &MainWindow::restart_server); // 重启服务器
    connect(server, &NetworkServer::receive, this, &MainWindow::receive_from_client);
    // receive 是服务端收到消息后发出的信号，receive_from_client 是处理这个信号的槽函数

    for(int i=0;i<24;i++)
    {
        p[i].init(i);
    }
    step=0;
}
int MainWindow::findId(myPoint mypoint)
{
    for(int i=0;i<24;i++)
    {
        if(p[i]._row==mypoint.x&&p[i]._col==mypoint.y&&!p[i]._dead)
        {
            return i;
        }
    }
    return -1;
}
int MainWindow::getBlackNum()
{
    int count=0;
    for(int i=0;i<12;i++)
    {
        if(!p[i]._dead)
        {
            count++;
        }
    }
    return count;
}
int MainWindow::getWhiteNum()
{
    int count=0;
    for(int i=12;i<=24;i++)
    {
        if(!p[i]._dead)
        {
            count++;
        }
    }
    return count;

}
SurakartaEndReason MainWindow::whyEnd()
{
    SurakartaEndReason end_reason;
    if(getWhiteNum()==0||getBlackNum()==0)
    {
        end_reason=SurakartaEndReason::CHECKMATE;
    }
    else
    {
        end_reason=SurakartaEndReason::NONE;
    }
    return end_reason;
}
bool MainWindow::judgeEnd()
{
    if(getWhiteNum()==0||getBlackNum()==0)
    {
        return true;
    }
    return false;
}
MainWindow::myPoint MainWindow::change(QString str)
{
    myPoint point;
    if(str[0]=='A')
    {
        point.x=0;
    }
    if(str[0]=='B')
    {
        point.x=1;
    }
    if(str[0]=='C')
    {
        point.x=2;
    }
    if(str[0]=='D')
    {
        point.x=3;
    }
    if(str[0]=='E')
    {
        point.x=4;
    }
    if(str[0]=='F')
    {
        point.x=5;
    }
    if(str[1]=='1')
    {
        point.y=1;
    }
    if(str[1]=='2')
    {
        point.y=2;
    }
    if(str[1]=='3')
    {
        point.y=3;
    }
    if(str[1]=='4')
    {
        point.y=4;
    }
    if(str[1]=='5')
    {
        point.y=5;
    }
    if(str[1]=='0')
    {
        point.y=0;
    }
    return point;
}
void MainWindow::listen_port() {
    this->port = ui->PortEdit->text().toInt();
    server->listen(QHostAddress::Any, this->port);
    // 一行代码搞定监听，它会在所有 ip 地址上监听指定端口 port
    // 所谓监听，就是在网络上服务器要开始盯住某个端口，此时客户端可以通过这个端口和服务器取得联系
    // QHostAddress::Any 表示监听所有 ip 地址，在不考虑安全的情况下，这是一个比较方便的选择，你不用纠结为什么
    ui->PortButton->setEnabled(false);
    ui->PortEdit->setReadOnly(true);
    ui->PortEdit->setText("Listening...");
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
bool MainWindow::judgeCatch(int moveid,int row,int col,int killid)
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
        int count1=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(a1[i].x==p[moveid]._row&&a1[i].y==p[moveid]._col)
            {
                count1++;
                if(count1==1)
                {
                    tmp1=i;
                }
                if(count1==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count1==1)
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
                for(int i=(tmp1+1)%24;a1[i].x!=row||a1[i].y!=col;i++)
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
        if(count1==2)
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
                for(int i=(tmp1+1)%24;a1[i].x!=row||a1[i].y!=col;i++)
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
                for(int i=(tmp2+1)%24;a1[i].x!=row||a1[i].y!=col;i++)
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
        count1=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(a2[i].x==p[moveid]._row&&a2[i].y==p[moveid]._col)
            {
                count1++;
                if(count1==1)
                {
                    tmp1=i;
                }
                if(count1==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count1==1)
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
                for(int i=(tmp1+1)%24;a2[i].x!=row||a2[i].y!=col;i++)
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
        if(count1==2)
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
                for(int i=(tmp1+1)%24;a2[i].x!=row||a2[i].y!=col;i++)
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
        int count1=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(b1[i].x==p[moveid]._row&&b1[i].y==p[moveid]._col)
            {
                count1++;
                if(count1==1)
                {
                    tmp1=i;
                }
                if(count1==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count1==1)
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
                for(int i=(tmp1+1)%24;b1[i].x!=row||b1[i].y!=col;i++)
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
        if(count1==2)
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
                for(int i=(tmp1+1)%24;b1[i].x!=row||b1[i].y!=col;i++)
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
                for(int i=(tmp2+1)%24;b1[i].x!=row||b1[i].y!=col;i++)
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
        count1=0, tmp1=0,tmp2=0;
        for(int i=0;i<24;i++)
        {
            if(b2[i].x==p[moveid]._row&&b2[i].y==p[moveid]._col)
            {
                count1++;
                if(count1==1)
                {
                    tmp1=i;
                }
                if(count1==2)
                {
                    tmp2=i;
                    break;
                }
            }
        }
        if(count1==1)
        {
            int sign1=0,sign2=0;
            if(b2[(tmp1+1)%24].x==row&&b2[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag22=1;
                }
            }
            for(int i=(tmp1+1)%24;b2[i].x!=row||b2[i].y!=col;i++)
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
        if(count1==2){
            int sign1=0,sign2=0;
            if(b2[(tmp1+1)%24].x==row&&b2[(tmp1+1)%24].y==col)
            {
                if(p[moveid]._row!=row&&p[moveid]._col!=col)
                {
                    flag22=1;
                }
            }
            else{
                for(int i=(tmp1+1)%24;b2[i].x!=row||b2[i].y!=col;i++)
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
                for(int i=(tmp2+1)%24;b2[i].x!=row||b2[i].y!=col;i++)
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

void MainWindow::remove_client(QTcpSocket* client) {
    if (client == client1) {
        client1 = nullptr;
        ui->ShowClient1->setText("");
    }
    else if (client == client2) {
        client2 = nullptr;
        ui->ShowClient2->setText("");
    }
    clients.remove(client);
}

void MainWindow::receive_from_client(QTcpSocket* client, NetworkData data) {
    // client 是发送消息的客户端，data 是消息内容
    // 以下代码首先处理客户端的连接和断开，然后处理从客户端收到的消息，显示在界面上并转发给另一个客户端
    // 你们在游戏里要做类似的事情，只不过处理不仅仅是显示，而是对应的游戏逻辑
    if(!client1)
    {
        client1=client;
        player1_Name=data.data1;
        if(data.data2=="1")
        {
            player_color1=piececolor::BLACK;
        }
        if(data.data2=="2")
        {
            player_color1=piececolor::WHITE;
        }
        if(data.data2==" ")
        {
            player_color1=piececolor::NONE;
        }
    }
    else if(!client2)
    {
        client2=client;
        player2_Name=data.data1;
        if(data.data2=="1")
        {
            player_color2=piececolor::BLACK;
        }
        if(data.data2=="2")
        {
            player_color2=piececolor::WHITE;
        }
        if(data.data2=="")
        {
            player_color2=piececolor::NONE;
        }
    }
    if(client1&&client2&&data.op==OPCODE::READY_OP)
        {
         if(player_color1==piececolor::NONE&&player_color2==piececolor::WHITE)
         {
            player_color1=piececolor::BLACK;
            send_to_another_client(client1,NetworkData(OPCODE::READY_OP,player2_Name,"2",data.data3));
            send_to_another_client(client2,NetworkData(OPCODE::READY_OP,player1_Name,"1",data.data3));
         }
         if(player_color1==piececolor::NONE&&player_color2==piececolor::BLACK)
         {
            player_color1=piececolor::WHITE;
            send_to_another_client(client1,NetworkData(OPCODE::READY_OP,player2_Name,"1",data.data3));
            send_to_another_client(client2,NetworkData(OPCODE::READY_OP,player1_Name,"2",data.data3));
         }
         if(player_color1==piececolor::NONE&&player_color2==piececolor::NONE)
         {
            player_color1=piececolor::BLACK;
            player_color2=piececolor::WHITE;
            send_to_another_client(client1,NetworkData(OPCODE::READY_OP,player2_Name,"2",data.data3));
            send_to_another_client(client2,NetworkData(OPCODE::READY_OP,player1_Name,"1",data.data3));
         }
         else if(player_color1==player_color2)
         {
             send_to_another_client(client1,NetworkData(OPCODE::REJECT_OP,"","",data.data3));
             send_to_another_client(client2,NetworkData(OPCODE::REJECT_OP,"","",data.data3));
         }
         if(player_color1==piececolor::WHITE&&player_color2==piececolor::NONE)
         {
            player_color2=piececolor::BLACK;
            send_to_another_client(client1,NetworkData(OPCODE::READY_OP,player2_Name,"1",data.data3));
            send_to_another_client(client2,NetworkData(OPCODE::READY_OP,player1_Name,"2",data.data3));
         }
         if(player_color1==piececolor::BLACK&&player_color2==piececolor::NONE)
         {
            player_color2=piececolor::WHITE;
            send_to_another_client(client1,NetworkData(OPCODE::READY_OP,player2_Name,"2",data.data3));
            send_to_another_client(client2,NetworkData(OPCODE::READY_OP,player1_Name,"1",data.data3));
         }
         if(player_color1==piececolor::BLACK&&player_color2==piececolor::WHITE)
         {
             send_to_another_client(client1,NetworkData(OPCODE::READY_OP,player2_Name,"1",data.data3));
             send_to_another_client(client2,NetworkData(OPCODE::READY_OP,player1_Name,"2",data.data3));
         }
         if(player_color1==piececolor::WHITE&&player_color2==piececolor::BLACK)
         {
             send_to_another_client(client1,NetworkData(OPCODE::READY_OP,player2_Name,"2",data.data3));
             send_to_another_client(client2,NetworkData(OPCODE::READY_OP,player1_Name,"1",data.data3));
         }
        }
    SurakartaIllegalMoveReason move_reason;
    if(data.op==OPCODE:: MOVE_OP)
    {
        step++;
        if(getBlackNum()==0)
        {
            send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","3","2"));
            send_to_another_client(client2,NetworkData(OPCODE::END_OP,"","3","2"));
        }
        if(getWhiteNum()==0)
        {
            send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","3","1"));
            send_to_another_client(client2,NetworkData(OPCODE::END_OP,"","3","1"));
        }
        if(step>maxstep&&getBlackNum()>getWhiteNum())
        {
            send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","2","1"));
            send_to_another_client(client2,NetworkData(OPCODE::END_OP,"","2","1"));
        }
        if(step>maxstep&&getBlackNum()<getWhiteNum())
        {
            send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","2","2"));
            send_to_another_client(client2,NetworkData(OPCODE::END_OP,"","2","2"));
        }
        QString str1,str2;
        str1.push_back(data.data1[0]);
        str1.push_back(data.data1[1]);
        str2.push_back(data.data2[0]);
        str2.push_back(data.data2[1]);
        myPoint from_point=change(str1);
        myPoint to_point=change(str2);
        int from_id=findId(from_point);
        int to_id=findId(to_point);

        if(client==client1)
        {

            if(from_id==-1)
            {
                move_reason=SurakartaIllegalMoveReason::NOT_PIECE;
                if(player_color1==piececolor::BLACK)
                {
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"7","7","2"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"7","7","2"));
                }
                else if(player_color1==piececolor::WHITE)
                {
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"7","7","1"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"7","7","1"));
                }
            }
            else if(player_color1==piececolor::BLACK&&from_id>=12&&from_id<24)
            {
                move_reason=SurakartaIllegalMoveReason::NOT_PLAYER_PIECE;
            send_to_another_client(client1,NetworkData(OPCODE::END_OP,"8","7","2"));
            send_to_another_client(client2,NetworkData(OPCODE::END_OP,"8","7","2"));
            }
            else if(player_color1==piececolor::WHITE&&from_id<12&&from_id>=0)
                {
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"8","7","1"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"8","7","1"));
                }
            else if((step%2==0&&player_color1==piececolor::BLACK)||(step%2!=0&&player_color1==piececolor::WHITE))
            {
                move_reason=SurakartaIllegalMoveReason::NOT_PLAYER_TURN;
                if(player_color1==piececolor::BLACK)
                {
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"5","7","2"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"5","7","2"));
                }
                if(player_color1==piececolor::WHITE)
                {
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"5","7","1"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"5","7","1"));
                }
            }
            else if(to_id==-1)
            {
                int res=abs(from_point.x-to_point.x)+10*abs(from_point.y-to_point.y);
                if(res==10||res==1||res==11)
                {
                    move_reason=SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE;
                    p[from_id]._row=from_point.x;
                    p[from_id]._col=from_point.y;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                }
                else
                {
                    move_reason=SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
                    if(player_color1==piececolor::BLACK)
                    {
                        send_to_another_client(client1,NetworkData(OPCODE::END_OP,"10","7","2"));
                        send_to_another_client(client2,NetworkData(OPCODE::END_OP,"10","7","2"));
                    }
                    else if(player_color1==piececolor::WHITE)
                    {
                        send_to_another_client(client1,NetworkData(OPCODE::END_OP,"10","7","1"));
                        send_to_another_client(client2,NetworkData(OPCODE::END_OP,"10","7","1"));
                    }
                }
            }
            else if(player_color1==piececolor::BLACK&&to_id>=12&&to_id<24)
            {
                if(judgeCatch(from_id,to_point.x,to_point.y,to_id))
                {
                    move_reason=SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
                    p[to_id]._dead=true;
                    p[from_id]._row=p[to_id]._row;
                    p[from_id]._col=p[to_id]._col;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                }
                else
                {
                    move_reason=SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"9","7","2"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"9","7","2"));
                }
            }
            else if(player_color1==piececolor::WHITE&&to_id>=0&&to_id<12)
            {
                if(judgeCatch(from_id,to_point.x,to_point.y,to_id))
                {
                    move_reason=SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
                    p[to_id]._dead=true;
                    p[from_id]._row=p[to_id]._row;
                    p[from_id]._col=p[to_id]._col;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                }
                else
                {
                    move_reason=SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"9","7","1"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"9","7","1"));
                }
            }
        }
        if(client==client2)
        {

            if(from_id==-1)
            {
                move_reason=SurakartaIllegalMoveReason::NOT_PIECE;
                if(player_color2==piececolor::BLACK)
                {
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"7","7","2"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"7","7","2"));
                }
                else if(player_color2==piececolor::WHITE)
                {
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"7","7","1"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"7","7","1"));
                }
            }
            else if(player_color2==piececolor::BLACK&&from_id>=12&&from_id<24)
            {
                move_reason=SurakartaIllegalMoveReason::NOT_PLAYER_PIECE;
                send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                send_to_another_client(client1,NetworkData(OPCODE::END_OP,"8","7","2"));
                send_to_another_client(client2,NetworkData(OPCODE::END_OP,"8","7","2"));
            }
            else if(player_color2==piececolor::WHITE&&from_id<12&&from_id>=0)
            {
                  move_reason=SurakartaIllegalMoveReason::NOT_PLAYER_PIECE;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"8","7","1"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"8","7","1"));
            }
            else if((step%2==0&&player_color2==piececolor::BLACK)||(step%2!=0&&player_color2==piececolor::WHITE))
            {
                move_reason=SurakartaIllegalMoveReason::NOT_PLAYER_TURN;
                if(player_color2==piececolor::BLACK)
                {
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"5","7","2"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"5","7","2"));
                }
                if(player_color2==piececolor::WHITE)
                {
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"5","7","1"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"5","7","1"));
                }
            }
            else if(to_id==-1)
            {
                int res=abs(from_point.x-to_point.x)+10*abs(from_point.y-to_point.y);
                if(res==10||res==1||res==11)
                {
                    move_reason=SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE;
                    p[from_id]._row=from_point.x;
                    p[from_id]._col=from_point.y;
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                }
                else
                {
                    move_reason=SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
                    if(player_color2==piececolor::BLACK)
                    {
                        send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                        send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                        send_to_another_client(client1,NetworkData(OPCODE::END_OP,"10","7","2"));
                        send_to_another_client(client2,NetworkData(OPCODE::END_OP,"10","7","2"));
                    }
                    else if(player_color2==piececolor::WHITE)
                    {
                        send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                        send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                        send_to_another_client(client1,NetworkData(OPCODE::END_OP,"10","7","1"));
                        send_to_another_client(client2,NetworkData(OPCODE::END_OP,"10","7","1"));
                    }
                }
            }
            else if(player_color2==piececolor::BLACK&&to_id>=12&&to_id<24)
            {
                if(judgeCatch(from_id,to_point.x,to_point.y,to_id))
                {
                    move_reason=SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
                    p[to_id]._dead=true;
                    p[from_id]._row=p[to_id]._row;
                    p[from_id]._col=p[to_id]._col;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                }
                else
                {
                    move_reason=SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"9","7","2"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"9","7","2"));
                }
            }
            else if(player_color2==piececolor::WHITE&&to_id>=0&&to_id<12)
            {
                if(judgeCatch(from_id,to_point.x,to_point.y,to_id))
                {
                    move_reason=SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
                    p[to_id]._dead=true;
                    p[from_id]._row=p[to_id]._row;
                    p[from_id]._col=p[to_id]._col;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                }
                else
                {
                    move_reason=SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
                    send_to_another_client(client2,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::MOVE_OP,data.data1,data.data2,""));
                    send_to_another_client(client1,NetworkData(OPCODE::END_OP,"9","7","1"));
                    send_to_another_client(client2,NetworkData(OPCODE::END_OP,"9","7","1"));
                }
            }
        }
    }
    if (data.op == OPCODE::LEAVE_OP)
    {
        remove_client(client);
        return;
    }
    if(data.op==OPCODE::GIVEUP_OP)
    {
        if(client==client1)
        {
            if(player_color1==piececolor::WHITE)
            {
             send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","5","1"));
            }
            if(player_color1==piececolor::BLACK)
            {
                send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","5","2"));
            }
        }
        if(client==client2)
        {
            if(player_color2==piececolor::WHITE)
            {
                send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","5","1"));
            }
            if(player_color2==piececolor::BLACK)
            {
                send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","5","2"));
            }
        }
        file.open(QIODevice::Append);
        QString str="R#";
        file.write((str).toUtf8());
        file.flush();
        file.close();
    }
    if(data.op==OPCODE::TIMEOUT_END_OP)
    {
        if(client==client1)
        {
            if(player_color1==piececolor::WHITE)
            {
                send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","6","1"));
            }
            if(player_color1==piececolor::BLACK)
            {
                send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","6","2"));
            }
        }
        if(client==client2)
        {
            if(player_color2==piececolor::WHITE)
            {
                send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","6","1"));
            }
            if(player_color2==piececolor::BLACK)
            {
                send_to_another_client(client1,NetworkData(OPCODE::END_OP,"","6","2"));
            }
        }
        file.open(QIODevice::Append);
        QString str="R#";
        file.write((str).toUtf8());
        file.flush();
        file.close();
    }
    //if (!clients.contains(client))
   // {
   //    if (clients.size() >= max_clients)
   //  {
  //        QMessageBox::warning(this, "Warning", "The server is full!");
  //          return;
  //      }
   //     clients.insert(client);
   //     if (!client1)
   //         client1 = client;
   //     else if (!client2)
   //         client2 = client;
   // }
    if (client == client1)
    {
        this->ui->ShowClient1->setText(data.data1);
        if (client2 && data.op == OPCODE::CHAT_OP)
            send_to_another_client(client2, data);
    }
    else if (client == client2)
    {
        this->ui->ShowClient2->setText(data.data1);
        if (client1 && data.op == OPCODE::CHAT_OP)
            send_to_another_client(client1, data);
    }
    else
        QMessageBox::warning(this, "Warning", "Unknown client!");
}

void MainWindow::send_to_another_client(QTcpSocket* another, NetworkData data)
{
    // 发送消息给一个客户端，这个程序里实际上做的事情是转发消息
    this->server->send(another, data);
}

void MainWindow::restart_server()
{
    // 如果你要重启服务器，就像其他 QObject 一样，你需要 disconnect 再 connect 回来一些信号槽
    server->close();
    clients.clear();
    client1 = nullptr;
    client2 = nullptr;
    disconnect(server, &NetworkServer::receive, this, &MainWindow::receive_from_client);
    delete server;
    server = new NetworkServer(this);
    ui->PortButton->setEnabled(true);
    ui->PortEdit->setReadOnly(false);
    ui->PortEdit->setText(QString::number(this->port));
    ui->ShowClient1->setText("");
    ui->ShowClient2->setText("");
    connect(server, &NetworkServer::receive, this, &MainWindow::receive_from_client);
}

MainWindow::~MainWindow() {
    delete ui;
    delete server;
}

