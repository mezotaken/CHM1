#include <QMainWindow>

//Структура функция
struct func
{
    double m;   //Масса
    double a1;  //Пар-р a1
    double a3;  //Пар-р a2

    func() {}   //Конструктор
    double count(double x,double v);            //Подсчёт значения
    QString print();                            //Вывод строки с функцией
};

struct model:public func
{
public:
    bool isRun;             //Запущен ли счёт
    bool isInf;             //Считать ли на установление
    bool isStepFixed;       //Фиксирован ли шаг
    double x0,u0;           //Начальные условия задачи Коши
    int stDwn,stUp;         //Число удваиваний и делений шага
    double x,v;             //Текущая точка численной траектории
    double v2;              //Точка посчитанная с вдвое меньшим шагом
    double v1;              //Точка посчитанная обычным шагом
    double u;               //Текущая точка точной траектории
    double E;               //Глобальная погрешность
    double h;               //Текущий шаг
    double hprev;           //Предыдущий шаг
    double s, avgS;         //S оценка ЛП, Среднее |S|
    double minS,maxS;       //Макс. и Мин. |S|
    double xminS,xmaxS;     //Точки в которых |S| максимально(минимально)
    double minH,maxH;       //Макс. и Мин. H
    double xminH,xmaxH;     //
    double minE,maxE;       //Макс. и Мин. E
    double xminE,xmaxE;     //
    double end;             //Граничная точка
    double epsBrd,epsCtrl;  //Параметры контроля выхода на границу и контроля погрешности
    int maxStep;            //Максимальное число шагов
    int usage;              //Схема использования

    model() {}                                    //Констуктор
    void set(double i_m, double i_a1,double i_a3, //Установить параметры
          bool i_isInf, double i_x0,double i_u0,
          double i_h, double i_end, double i_epsBrd,
          double i_epsCtrl, int i_maxStep, bool i_isStepFixed,
          int i_usage);
    void start();   //Начать счёт
    void stop();    //Остановить счёт
    double countNext(double th, double tx, double tv);          //Посчитать следующую точку
    double halfStepCountNext(double th, double tx, double tv);  //Посчитать следующую точку с вдвое меньшим шагом
    int ctrlLocalErr();   //Контроль локальной погрешности
    void halveStep();     //Поделить шаг пополам
    void doubleStep();    //Удвоить шаг
    double countCorrect(double tx);//Посчитать точное решение
    void iterate();

};

