#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_startCount_clicked();

    void on_stopCount_clicked();

    void on_clear_clicked();

    void on_isInfch_stateChanged(int arg1);

    void paintGraph(int numbColumnX,int numbColumnY, int N, int numbGraph);



    void on_exit_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
