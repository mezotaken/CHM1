#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"







static model md;    //Модель

int numbGraph = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("LabWork");
    //Инициализация таблицы
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "v"<<"h"<<"v2"<<"S"<<"e"<<"u"<<"E");

    //Инициализация основного графика
    ui->graph->xAxis->setLabel("x");
    ui->graph->yAxis->setLabel("V");
    ui->graph->setInteraction(QCP::iRangeZoom,true);
    ui->graph->setInteraction(QCP::iRangeDrag, true); // Включаем взаимодействие перетаскивания графика

    //Инициализация графика погрешностей
    ui->errGraph->xAxis->setLabel("x");
    ui->errGraph->yAxis->setLabel("E");
    ui->errGraph->setInteraction(QCP::iRangeZoom,true);
    ui->errGraph->setInteraction(QCP::iRangeDrag, true);
}
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::paintGraph(int numbColumnX, int numbColumnY, int N, int numbGraph){

    QVector<double> X(N);
    QVector<double> Y(N);
    QVector<double> E(N);
    QVariant check;
    QTableWidgetItem *itemX;
    QTableWidgetItem *itemY;
    QTableWidgetItem *itemE;


    QPen pen;



    pen.setColor(Qt::GlobalColor(numbGraph+7));


    for(int i = 0; i < N; i++){
        itemX = ui->tableWidget->item(i,numbColumnX);
        itemY = ui->tableWidget->item(i,numbColumnY);
        itemE = ui->tableWidget->item(i,7);
        check = itemX->text();
        X[i] = check.toDouble();
        check = itemY->text();
        Y[i] = check.toDouble();
        check = itemE->text();
        E[i] = check.toDouble();
        }



    ui->graph->addGraph();
    ui->errGraph->addGraph();
    ui->graph->graph(numbGraph)->setData(X,Y);
    ui->graph->graph(numbGraph)->setPen(pen);
    ui->errGraph->graph(numbGraph)->setData(X,E);
    ui->errGraph->graph(numbGraph)->setPen(pen);

    ui->graph->xAxis->setRange(X[0],X[N-1]);
    ui->errGraph->xAxis->setRange(X[0],X[N-1]);

    double minY = Y[0], maxY = Y[0], minE = E[0], maxE = E[0];
    for(int i = 1; i < N; i++){
        if(Y[i]<minY) minY = Y[i];
        if(Y[i]>maxY) maxY = Y[i];
        if(E[i]<minE) minE = E[i];
        if(E[i]>maxE) maxE = E[i];
    }

    ui->graph->yAxis->setRange(minY, maxY);
    ui->errGraph->yAxis->setRange(minE, maxE);
    ui->graph->replot();
    ui->errGraph->replot();
    ui->graph->legend->setVisible(true);
}



void MainWindow::on_startCount_clicked()
{
    QTableWidgetItem *tbl;
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "v"<<"h"<<"v2"<<"S"<<"e"<<"u"<<"E");
    md.set(ui->mfd->text().toDouble(), ui->a1fd->text().toDouble(),
           ui->a3fd->text().toDouble(), ui->isInfch->isChecked(),
           ui->x0fd->text().toDouble(), ui->u0fd->text().toDouble(),
           ui->stepfd->text().toDouble(), ui->endfd->text().toDouble(),
           ui->epsBrdfd->text().toDouble(), ui->epsCtrlfd->text().toDouble(),
           ui->maxStepfd->text().toInt(),ui->isStepFixedch->isChecked());
    ui->task->setText(md.print());
    md.start();
    ui->tableWidget->setRowCount(50);
    ui->countParam->setDisabled(true);
    ui->taskParam->setDisabled(true);
    tbl = new QTableWidgetItem(QString::number(md.x));
    ui->tableWidget->setItem(0,0,tbl);
    tbl = new QTableWidgetItem(QString::number(md.v));
    ui->tableWidget->setItem(0,1,tbl);
    tbl = new QTableWidgetItem("0");
    ui->tableWidget->setItem(0,7,tbl);
    int i = 1;
    while(md.isRun && i<md.maxStep && (md.end-md.x > md.epsBrd || md.isInf ))
    {
        if(ui->tableWidget->rowCount() == i)
            ui->tableWidget->setRowCount(2*ui->tableWidget->rowCount());
        md.iterate();
        tbl = new QTableWidgetItem(QString::number(md.x));
        ui->tableWidget->setItem(i,0,tbl);
        tbl = new QTableWidgetItem(QString::number(md.v));
        ui->tableWidget->setItem(i,1,tbl);
        tbl = new QTableWidgetItem(QString::number(md.hprev));
        ui->tableWidget->setItem(i,2,tbl);
        tbl = new QTableWidgetItem(QString::number(md.v2));
        ui->tableWidget->setItem(i,3,tbl);
        tbl = new QTableWidgetItem(QString::number(md.s));
        ui->tableWidget->setItem(i,4,tbl);
        tbl = new QTableWidgetItem(QString::number(md.s*16));
        ui->tableWidget->setItem(i,5,tbl);
        tbl = new QTableWidgetItem(QString::number(md.u));
        ui->tableWidget->setItem(i,6,tbl);
        tbl = new QTableWidgetItem(QString::number(md.E));
        ui->tableWidget->setItem(i,7,tbl);
        ui->stepUp->setText(QString::number(md.stUp));
        ui->stepDwn->setText(QString::number(md.stDwn));
        ui->maxS->setText(QString::number(md.maxS));
        ui->minS->setText(QString::number(md.minS));
        ui->xmaxS->setText(QString::number(md.xmaxS));
        ui->xminS->setText(QString::number(md.xminS));
        QCoreApplication::processEvents();
        i++;
    }
        md.avgS/=i;
        ui->avgS->setText(QString::number(md.avgS));
        ui->countParam->setDisabled(false);
        ui->taskParam->setDisabled(false);
        md.stop();


       paintGraph(0,1,i,numbGraph);
       numbGraph++;


}

void MainWindow::on_stopCount_clicked()
{
    md.stop();
}


void MainWindow::on_clear_clicked()
{
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "v"<<"h"<<"v2"<<"S");
    ui->stepUp->setText("");
    ui->stepDwn->setText("");
    ui->xmaxS->setText("");
    ui->maxS->setText("");
    ui->xminS->setText("");
    ui->minS->setText("");
    ui->avgS->setText("");
    ui->tableWidget->setRowCount(0); //устанавливаем количество строк в ноль
    ui->graph->clearGraphs();
    ui->graph->replot();
    ui->errGraph->clearGraphs();
    ui->errGraph->replot();
    numbGraph = 0;
}

void MainWindow::on_isInfch_stateChanged(int st)
{
    if(st)
    {
        ui->endfd->setDisabled(true);
        ui->epsBrdfd->setDisabled(true);
    }
    else
    {
        ui->endfd->setDisabled(false);
        ui->epsBrdfd->setDisabled(false);
    }

}
