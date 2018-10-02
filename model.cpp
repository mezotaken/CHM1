#include "model.h"
//Реализация функций структуры func


double func::count(double x,double v)
{
    return -(a1*v+a3*v*v*v)/m;
}

QString func::print()
{
    return QString("u' = (" + QString::number(a1) + "*u + " + QString::number(a3) + "*u^3)/"+ QString::number(m));
}
//Реализация функций структуры model

void model::set(double i_m, double i_a1,double i_a3,
             bool i_isInf, double i_x0,double i_u0,
             double i_h, double i_end, double i_epsBrd,
             double i_epsCtrl, int i_maxStep, bool i_isStepFixed,
             int i_usage)
{
    m = i_m;
    a1 = i_a1;
    a3 = i_a3;
    isInf = i_isInf;
    x0 = i_x0;
    u0 = i_u0;
    h = i_h;
    end = i_end;
    epsBrd = i_epsBrd;
    epsCtrl = i_epsCtrl;
    maxStep = i_maxStep;
    isStepFixed = i_isStepFixed;
    usage = i_usage;
}

void model::start()
{
    x = x0;
    v = u0;
    isRun = true;
    hprev = h;
    maxS = 0;
    xmaxS = x0;
    minS =  1;
    xminS = x0;
    avgS = 0;
    maxH = 0;
    xmaxH = x0;
    minH = 10;
    xminH = x0;
    maxE = 0;
    xmaxE = x0;
    minE = 5;
    xminE = x0;
}

void model::stop()
{
    isRun = false;
    stDwn = stUp = 0;
}

double model::countNext(double th, double tx, double tv)
{
    double k1,k2,k3,k4;
    double res;
    k1 = count(tx,tv);
    k2 = count(tx+th/2,tv+th/2*k1);
    k3 = count(tx+th/2,tv+th/2*k2);
    k4 = count(tx+th,tv+th*k3);
    res = tv+th/6*(k1+2*k2+2*k3+k4);
    return res;
}

double model::halfStepCountNext(double th, double tx, double tv)
{
    return countNext(th/2,tx+th/2,countNext(th/2,tx,tv));
}

int model::ctrlLocalErr()
{
    int res;
    if(std::abs(s)<epsCtrl/32)
        res = -1;
    else if(std::abs(s)<=epsCtrl)
        res = 0;
    else res = 1;
        return res;
}

void model::halveStep()
{
    h/=2; stDwn++;
}

void model::doubleStep()
{
    h*=2; stUp++;
}

double model::countCorrect(double tx)
{

    double c = m*log(u0*u0*a1+a3)/(2*a1) - x0;
    return sqrt(a1)/sqrt(exp(2*a1*(c+tx)/m) - a3);

}

void model::iterate()
{
    v2 = halfStepCountNext(h,x,v);
    v1 = countNext(h,x,v);

    s = (v2-v1)/15;
    if(!isStepFixed)
    {
        int ctrl = ctrlLocalErr();

        while(ctrl == 1)
        {

            halveStep();
            v2 = halfStepCountNext(h,x,v);
            v1 = countNext(h,x,v);
            s = (v2-v1)/15;
            ctrl = ctrlLocalErr();
        }
         hprev = h;

        if(ctrl == -1)
            doubleStep();
    }

    x = x+hprev;
    switch(usage)
    {
        case 0:
        v = v1;
        break;
        case 1:
        v = v2;
        break;
        case 2:
        v = v1+s*16;
        break;
    {

    }
    }
    u = countCorrect(x);
    E = u-v;
    if(hprev>maxH)
    {
        maxH = hprev;
        xmaxH = x;
    }
    if(hprev<minH)
    {
        minH = hprev;
        xminH = x;
    }
    if(std::abs(s)>maxS)
    {
        maxS = std::abs(s);
        xmaxS = x;
    }
    if(std::abs(s)<minS)
    {
        minS = std::abs(s);
        xminS = x;
    }
    if(std::abs(E)>maxE)
    {
        maxE = std::abs(E);
        xmaxE = x;
    }
    if(std::abs(E)<minE)
    {
        minE = std::abs(E);
        xminE = x;
    }
    avgS+=std::abs(s);
}
