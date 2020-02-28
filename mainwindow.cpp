// (c) 2019 Dzoka
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString confFile = QCoreApplication::applicationFilePath();
    confFile.append(".conf");
    QFile file(confFile);
    ui->statusBar->showMessage(confFile, 3000);
    if(file.exists())                   // read configuration file, if exists
    {
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while(!in.atEnd())
            {
                QString line = in.readLine(50);
                QStringList list = line.split("=");
                if(list[0] == "Server")
                {
                    server = list[1];
                }
                else if(list[0] == "Port")
                {
                    port = list[1].toUInt();
                }
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendDataToServer(QString dataOut)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), SLOT(on_readyRead()));                             // on reply
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(on_socketError()));   // on socket error

    ui->statusBar->showMessage(tr("Connecting..."));
    socket->connectToHost(server, port);

    if(socket->waitForConnected())
    {
        ui->statusBar->showMessage(tr("Connected."));
        qint64 i;
        i = socket->write(QString(dataOut.left(dataOut.size())).toUtf8());
        if(i < dataOut.size())
        {
            ui->textEditResponse->setText(tr("Could not send all data"));

        }
    }
    else
    {
        ui->textEditResponse->setText(tr("Server timeout"));
    }
    ui->statusBar->clearMessage();
}

void MainWindow::on_readyRead()
{
    QString dataIn = socket->readAll();
    ui->textEditResponse->setText(dataIn.toUtf8());
    ui->statusBar->showMessage(tr("Disconnect"), 1000);
    socket->close();
    delete socket;
}

void MainWindow::on_socketError()
{
    switch(socket->error())
    {
        case QAbstractSocket::HostNotFoundError:
            ui->statusBar->showMessage(tr("Server not found"));
            ui->textEditResponse->setText(tr("Server not found"));
            break;
        default:
            ui->statusBar->showMessage(tr("Unknown socket error"));
            ui->textEditResponse->setText(tr("Unknown socket error"));
            break;
    }
}

void MainWindow::on_pushButtonSubmit_clicked()
{
    this->setCursor(Qt::WaitCursor);
    MainWindow::blockSignals(true);

    ui->textEditResponse->setText(tr("Sending..."));
    QString data;
    data.append("URFL:");
    data.append(ui->lineEditProduction->text());
    data.append(',');
    data.append(ui->lineEditItem->text());
    data.append(',');
    data.append(ui->lineEditBatch->text());
    data.append(',');
    data.append(ui->lineEditSerial->text());
    data.append(',');
    data.append(ui->lineEditQuantity->text());
    sendDataToServer(data);

    MainWindow::blockSignals(false);
    this->unsetCursor();
}

void MainWindow::on_pushButtonKey0_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("0"));
}

void MainWindow::on_pushButtonKey1_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("1"));
}

void MainWindow::on_pushButtonKey2_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("2"));
}

void MainWindow::on_pushButtonKey3_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("3"));
}

void MainWindow::on_pushButtonKey4_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("4"));
}

void MainWindow::on_pushButtonKey5_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("5"));
}

void MainWindow::on_pushButtonKey6_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("6"));
}

void MainWindow::on_pushButtonKey7_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("7"));
}

void MainWindow::on_pushButtonKey8_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("8"));
}

void MainWindow::on_pushButtonKey9_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("9"));
}

void MainWindow::on_pushButtonDot_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().append("."));
}

void MainWindow::on_pushButtonKeyClear_clicked()
{
    ui->lineEditQuantity->setText(ui->lineEditQuantity->text().left(ui->lineEditQuantity->text().length()-1));
}
