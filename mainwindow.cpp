#include "mainwindow.h"
#include "ui_mainwindow.h"

class func
{
private:
    double m;
    double a1;
    double a3;
public:
    func()
    {
    m=0;a1=0;a3=0;
    }
    double count(double x,double v)
    {
        return -(a1*v+a3*v*v*v)/m;
    }
    QString print()
    {
     return QString("u' = (" + QString::number(a1) + "*u + " + QString::number(a3) + "*u^3)/"+ QString::number(m));
    }
    void setm(double i_m) {m = i_m;}
    void seta1(double i_a1) {a1 = i_a1;}
    void seta3(double i_a3) {a3 = i_a3;}
    double getm() {return m;}
    double geta1() {return a1;}
    double geta3() {return a3;}
};

class model : public func
{
private:
    int cond;
    bool isInf;
    double x0,u0;
    double xprev,vprev;
    int stDwn,stUp;
    double x,v;
    double v2;
    double h;
    double s;
    double end;
    double epsBrd,epsCtrl;
    int maxStep;
public:
    model():func()
    {
        cond = 2;
        isInf = false;
        stDwn = stUp = s = xprev = vprev = end = epsBrd = epsCtrl = v2 = x = v = maxStep = x0 = u0 = h = 0;
    }
    void setx0(double i_x0) {x0 = i_x0;}
    void setu0(double i_u0) {u0 = i_u0;}
    void seth(double i_h) {h = i_h;}
    void setend(double i_end) {end = i_end;}
    void setepsBrd (double i_eps) {epsBrd = i_eps;}
    void setepsCtrl(double i_eps) {epsCtrl = i_eps;}
    void setmaxStep(int i_maxStep) {maxStep = i_maxStep;}
    void setisInf(bool i_inf) {isInf = i_inf;}
    void setstChzero() {stDwn = stUp = 0;}
    double getx0() {return x0;}
    int getstUp() {return stUp;}
    int getstDown() {return stDwn;}
    double getu0() {return u0;}
    double getx() {return x;}
    double getv() {return v;}
    double geth() {return h;}
    double getv2(){return v2;}
    double getend() {return end;}
    double gets() {return s;}
    bool getisInf() {return isInf;}
    int getmaxStep() {return maxStep;}
    double getepsBrd () {return epsBrd;}
    double getepsCtrl() {return epsCtrl;}
    int getcond() {return cond;}

    void start()
    {
        if(cond == 2)
        {
            x = x0;
            v = u0;
        }
        cond = 0;
    }
    void halvestep() {h/=2; stDwn++;}
    void doublestep() {h*=2; stUp++;}
    void getbackaccurate() {v = vprev; x=xprev; halvestep();}
    void countS() {s = (v2-v)/15;}
    void countNext()
    {
        double k1,k2,k3,k4;
        vprev = v; xprev = x;
        k1 = count(x,v);
        k2 = count(x+h/2,v+h/2*k1);
        k3 = count(x+h/2,v+h/2*k2);
        k4 = count(x+h,v+h*k3);
        v = v+h/6*(k1+2*k2+2*k3+k4);
        x = x+h;
    }
    void halfstepcount()
    {
        double xprev = x, vprev = v, hprev = h;

        h /=2;
        countNext();
        countNext();
        v2 = v;
        v= vprev;
        x= xprev;
        h = hprev;
    }
    int ctrle()
    {
        int res;
        if(std::abs(s)<epsCtrl/32)
            res = -1;
        else if(std::abs(s)<=epsCtrl)
            res = 0;
        else res = 1;
        return res;
    }
    void stop() {cond = 2;}
    void pause() {cond = 1;}
};


static model md;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "v"<<"h"<<"v2"<<"S");
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startCount_clicked()
{
    if(md.getcond() == 2)
    {
        ui->tableWidget->clear();
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "v"<<"h"<<"v2"<<"S");
        md.setx0(ui->x0fd->text().toDouble());
        md.setu0(ui->u0fd->text().toDouble());
        md.seth(ui->stepfd->text().toDouble());
        md.setm(ui->mfd->text().toDouble());
        md.seta1(ui->a1fd->text().toDouble());
        md.seta3(ui->a3fd->text().toDouble());
        md.setend(ui->endfd->text().toDouble());
        md.setisInf(ui->isInfch->isChecked());
        md.setmaxStep(ui->maxStepfd->text().toInt());
        md.setepsBrd(ui->epsBrdfd->text().toDouble());
        md.setepsCtrl(ui->epsCtrlfd->text().toDouble());
        ui->task->setText(md.print());
    }
    md.start();
    ui->tableWidget->setRowCount(10);
    ui->countParam->setDisabled(true);
    ui->taskParam->setDisabled(true);
    int i = 1;
    while(md.getcond() == 0 && i<md.getmaxStep() && (md.getend()-md.getx() > md.getepsBrd() || md.getisInf() ))
    {
        if(ui->tableWidget->rowCount() == i)
            ui->tableWidget->setRowCount(2*ui->tableWidget->rowCount());
        QTableWidgetItem *tbl;
        md.halfstepcount();
        md.countNext();
        md.countS();
        int ctrl = md.ctrle();
        while(ctrl == 1)
        {
            md.getbackaccurate();
            md.halfstepcount();
            md.countNext();
            md.countS();
            ctrl = md.ctrle();
        }

        tbl = new QTableWidgetItem(QString::number(md.getx()));
        ui->tableWidget->setItem(i,0,tbl);
        tbl = new QTableWidgetItem(QString::number(md.getv()));
        ui->tableWidget->setItem(i,1,tbl);
        tbl = new QTableWidgetItem(QString::number(md.geth()));
        ui->tableWidget->setItem(i,2,tbl);
        tbl = new QTableWidgetItem(QString::number(md.getv2()));
        ui->tableWidget->setItem(i,3,tbl);
        tbl = new QTableWidgetItem(QString::number(md.gets()));
        ui->tableWidget->setItem(i,4,tbl);
        ui->stepUp->setText(QString::number(md.getstUp()));
        ui->stepDwn->setText(QString::number(md.getstDown()));
        if(ctrl == -1)
        {
            md.doublestep();
        }
        QCoreApplication::processEvents();
        i++;
    }
    if(md.getcond() != 1)
    {
        ui->countParam->setDisabled(false);
        ui->taskParam->setDisabled(false);
        md.setstChzero();
        md.stop();
    }
}

void MainWindow::on_stopCount_clicked()
{
    md.stop();
}

void MainWindow::on_pauseCount_clicked()
{
    md.pause();
}

void MainWindow::on_clear_clicked()
{
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "v"<<"h"<<"v2"<<"S");
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
