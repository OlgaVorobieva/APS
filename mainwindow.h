#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "request.h"
#include "source.h"
#include "Buffer.h"
#include "Device.h"
#include "Statistic.h"
#include "StepStruct.h"
#include "Compare.h"

#include <QMainWindow>
#include <QtDebug>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void runSession();
    void buildTable();
    void stepMode();

private:
    Ui::MainWindow *ui;
    int step_ = 0;
    double p_ = 0.0;
    Statistic statistic_;

private slots:

    void on_startButton_clicked();
    void on_backButton_clicked();
    void on_nextButton_clicked();
    void on_startStepModeButton_clicked();
    void on_goToStep_clicked();
};

#endif // MAINWINDOW_H
