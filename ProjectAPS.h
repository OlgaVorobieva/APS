#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "statistic.h"
#include "device.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void runSimulation();
  void fillTable();
  void stepMode();

private:
  Ui::MainWindow* ui;
  int currentStep;
  Statistic statistic;

  std::vector<int> getEarlierDevice(std::vector<Device> vector, double deviceStopTime);

private slots:
  void on_autoModeButton_clicked();
  void on_stepModeButton_clicked();
  void on_backButton_clicked();
  void on_nextButton_clicked();
  void on_goToStep_clicked();
};

#endif


