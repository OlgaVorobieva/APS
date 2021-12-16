#include "ProjectAPS.h"
#include "ui_ProjectAPS.h"

#include "StepStruct.h"
#include "request.h"
#include "source.h"
#include "buffer.h"
#include "compare.h"

#include <float.h>

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  currentStep(0),
  statistic(0, 0, 0, 0, 0.0, 0.0, 0.0)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::runSimulation()
{
  Statistic statistic(ui-requestAmount->text().toInt(),
    ui->sourceAmount->text().toInt(),
    ui->bufferSize->text().toInt(),
    ui->deviceAmount->text().toInt(),
    ui->lambda->text().toDouble(),
    ui->alpha->text().toDouble(),
    ui->beta->text().toDouble());

  std::vector<Source> sourceVector;
  std::vector<Device> deviceVector;
  std::vector<std::shared_ptr<Request>> requestVector;

  Buffer buffer(statistic.getBufferSize());

  for (int i = 0; i < statistic.getSourceAmount(); i++)
  {
    Source source(i, statistic.getAlpha(), statistic.getBeta());
    sourceVector.push_back(source);
  }
  for (int i = 0; i < statistic.getDeviceAmount(); i++)
  {
    Device device(i, statistic.getLambda());
    deviceVector.push_back(device);
  }
  for (int i = 0; i < statistic.getRequestAmount(); i++)
  {
    if (sourceVector[0].getRequestTime() > sourceVector[1].getRequestTime())
    {
      std::sort(sourceVector.begin(), sourceVector.end(), Comparator());
    }
    requestVector.push_back(sourceVector[0].generateRequest());
  }
  std::sort(requestVector.begin(), requestVector.end(), Comparator());

  int requestNumber = 0, deviceNumber = 0, place = 0;
  bool toReject = false;

  Step step;
  step.rejected = 0;
  step.processed = 0;
  step.devicePointer = 0;
  step.packageStateVector = std::vector<std::shared_ptr<Request>>(statistic.getBufferSize(), nullptr);
  step.sourceStateVector = std::vector<std::shared_ptr<Request>>(statistic.getSourceAmount(), nullptr);
  step.bufferStateVector = std::vector<std::shared_ptr<Request>>(statistic.getBufferSize(), nullptr);
  step.deviceStateVector = std::vector<std::shared_ptr<Request>>(statistic.getDeviceAmount(), nullptr);

  while (statistic.getProcessed() + statistic.getRejected() != statistic.getRequestAmount())
  {
    double nextRequestTime = DBL_MAX;

    if (requestNumber != statistic.getRequestAmount())
    {
      step.sourceStateVector[requestVector[requestNumber]->getSourceNumber()] = requestVector[requestNumber];
      step.action = step.action = "Request " + std::to_string(requestVector[requestNumber]->getSourceNumber() + 1) +
        "." + std::to_string(requestVector[requestNumber]->getRequestNumber()) + " generated";
      step.time = requestVector[requestNumber]->getStartTime();
      statistic.addStep(step);

      if (!buffer.isFull())
      {
        buffer.addRequest(requestVector[requestNumber], step);
        statistic.addStep(step);
      }
      else
      {
        toReject = true;
      }
      int count = 0;
      double time = 0.0;

      while (count < statistic.getDeviceAmount())
      {
        if (deviceVector[deviceNumber].isAvailable(requestVector[requestNumber]->getStartTime()))
        {
          std::vector<int> vector = getEarlierDevice(deviceVector, deviceVector[deviceNumber].getStopTime());

          for (int i : vector)
          {
            deviceVector[i].setAvailable(statistic, step);
          }
          deviceVector[deviceNumber].setAvailable(statistic, step);

          if (!buffer.isEmpty())
          {
            if (deviceVector[deviceNumber].getStopTime() < time)
            {
              deviceVector[deviceNumber].setStartTime(time);
            }
            deviceVector[deviceNumber].addRequest(buffer.getRequest(statistic, statistic.getSourceAmount(), step), step);
            step.devicePointer = deviceNumber + 1;
            statistic.addStep(step);
            place = deviceNumber;
            time = deviceVector[deviceNumber].getStartTime();
          }
          if (toReject == true)
          {
            buffer.addRequest(requestVector[requestNumber], step);
            statistic.addStep(step);
            toReject = false;
          }
        }
        if (deviceNumber != statistic.getDeviceAmount() - 1)
        {
          deviceNumber++;
        }
        else
        {
          deviceNumber = 0;
        }
        count++;
      }
      if (toReject == true)
      {
        step.time = requestVector[requestNumber]->getStartTime();
        buffer.removeRequest(statistic, step);
        buffer.addRequest(requestVector[requestNumber], step);
        statistic.addStep(step);
        toReject = false;
      }
      deviceNumber = place;

      if (requestNumber != statistic.getRequestAmount() - 1)
      {
        nextRequestTime = requestVector[requestNumber + 1]->getStartTime();
      }
      count = 0;
      time = 0.0;

      while (count < statistic.getDeviceAmount())
      {
        if (deviceVector[deviceNumber].isAvailable(nextRequestTime))
        {
          std::vector<int> vector = getEarlierDevice(deviceVector, deviceVector[deviceNumber].getStopTime());

          for (int i : vector)
          {
            deviceVector[i].setAvailable(statistic, step);
          }
          deviceVector[deviceNumber].setAvailable(statistic, step);

          if (!buffer.isEmpty())
          {
            if (deviceVector[deviceNumber].getStopTime() < time)
            {
              deviceVector[deviceNumber].setStartTime(time);
            }
            deviceVector[deviceNumber].addRequest(buffer.getRequest(statistic, statistic.getSourceAmount(), step), step);
            step.devicePointer = deviceNumber + 1;
            statistic.addStep(step);
            place = deviceNumber;
            time = deviceVector[deviceNumber].getStartTime();
          }
        }
        if (deviceNumber != statistic.getDeviceAmount() - 1)
        {
          deviceNumber++;
        }
        else
        {
          deviceNumber = 0;
        }
        count++;
      }
      deviceNumber = place;
      requestNumber++;
    }
    else
    {
      deviceVector[deviceNumber].setAvailable(statistic, step);

      if (!buffer.isEmpty())
      {
        deviceVector[deviceNumber].addRequest(buffer.getRequest(statistic, statistic.getSourceAmount(), step), step);
        statistic.addStep(step);
      }
      if (deviceNumber != statistic.getDeviceAmount() - 1)
      {
        deviceNumber++;
      }
      else
      {
        deviceNumber = 0;
      }
    }
  }
  this->statistic = statistic;
}

void MainWindow::fillTable()
{
  QString printable;
  QStringList headers = { " Generated ", " Processed ", " Rejected ", " Reject probability ",
                         " Waiting Time ", " Servicing Time ", " Total time ", " Waiting dispersion ",
                         " Servicing dispersion " };

  QStringList headersVertical;

  ui->tableWidget->setColumnCount(9);
  ui->tableWidget->setRowCount(statistic.getSourceAmount());
  ui->tableWidget->setHorizontalHeaderLabels(headers);
  ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

  QTableWidgetItem* protoitem = new QTableWidgetItem();
  protoitem->setTextAlignment(Qt::AlignCenter);

  QString printableKK;
  double rejected = statistic.getRejected(), requestAmount = statistic.getRequestAmount();
  printableKK = QStringLiteral("%1").arg(rejected / requestAmount);
  ui->AverageReject->setText(printableKK);

  double temp = 0.0;
  for (int i = 0; i < statistic.getDeviceAmount(); i++)
  {
    temp += statistic.getDeviceUsage(i);
  }
  printableKK = QStringLiteral("%1").arg(temp / statistic.getDeviceAmount());
  ui->AverageReject_2->setText(printableKK);

  temp = 0.0;
  for (int i = 0; i < statistic.getSourceAmount(); i++)
  {
    temp += statistic.getSimulationTime(i);
  }
  printableKK = QStringLiteral("%1").arg(temp / statistic.getSourceAmount());
  ui->AverageReject_3->setText(printableKK);

  for (int i = 0; i < statistic.getSourceAmount(); i++)
  {
    printable = QStringLiteral("Source %1").arg(i + 1);
    headersVertical.push_back(printable);

    //Generated
    QTableWidgetItem* item1 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getProcessed(i) + statistic.getRejected(i));
    item1->setText(printable);
    ui->tableWidget->setItem(i, 0, item1);

    //Processed
    QTableWidgetItem* item2 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getProcessed(i));
    item2->setText(printable);
    ui->tableWidget->setItem(i, 1, item2);

    //Rejected
    QTableWidgetItem* item3 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getRejected(i));
    item3->setText(printable);
    ui->tableWidget->setItem(i, 2, item3);

    //Rejected probality
    QTableWidgetItem* item4 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getRejectProbality(i));
    item4->setText(printable);
    ui->tableWidget->setItem(i, 3, item4);

    //Waiting time
    QTableWidgetItem* item5 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getWaitingTime(i));
    item5->setText(printable);
    ui->tableWidget->setItem(i, 4, item5);

    //Servicing Time
    QTableWidgetItem* item6 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getServicingTime(i));
    item6->setText(printable);
    ui->tableWidget->setItem(i, 5, item6);

    //Total time
    QTableWidgetItem* item7 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getSimulationTime(i));
    item7->setText(printable);
    ui->tableWidget->setItem(i, 6, item7);

    //Waiting dispersion
    QTableWidgetItem* item8 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getWaitingTimeDispersion(i));
    item8->setText(printable);
    ui->tableWidget->setItem(i, 7, item8);

    //Servicing Dispersion
    QTableWidgetItem* item9 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getServicingTimeDispersion(i));
    item9->setText(printable);
    ui->tableWidget->setItem(i, 8, item9);
  }
  ui->tableWidget->setVerticalHeaderLabels(headersVertical);

  QStringList header = { "Devise usage" };
  QStringList headerVertical;

  ui->tableWidget_2->setColumnCount(1);
  ui->tableWidget_2->setRowCount(statistic.getDeviceAmount());
  ui->tableWidget_2->setHorizontalHeaderLabels(header);

  for (int i = 0; i < statistic.getDeviceAmount(); i++)
  {
    printable = QStringLiteral("Device %1").arg(i + 1);
    headerVertical.push_back(printable);
    QTableWidgetItem* item1 = protoitem->clone();
    printable = QStringLiteral("%1").arg(statistic.getDeviceUsage(i));
    item1->setText(printable);
    ui->tableWidget_2->setItem(i, 0, item1);
  }
  ui->tableWidget_2->setVerticalHeaderLabels(headerVertical);
  ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::stepMode()
{
  QString printable1;
  QString printable2;
  Step step = statistic.getStepVector()[currentStep];

  printable1 = QString::fromStdString(step.action);
  ui->stepText->setText(printable1);

  printable1 = QStringLiteral("%1").arg(step.time);
  ui->timeValue->setText(printable1);

  printable1 = QStringLiteral("%1").arg(currentStep + 1);
  ui->currentStep->setText(printable1);

  printable1 = QStringLiteral("%1").arg(step.processed);
  ui->enterProcessed->setText(printable1);

  printable1 = QStringLiteral("%1").arg(step.rejected);
  ui->enterRejected->setText(printable1);

  printable1 = QStringLiteral("%1").arg(step.devicePointer);
  ui->devicePointer->setText(printable1);

  QTableWidgetItem* protoitem = new QTableWidgetItem();
  protoitem->setTextAlignment(Qt::AlignCenter);

  for (int i = 0; i < statistic.getSourceAmount(); i++)
  {
    QTableWidgetItem* item1 = protoitem->clone();

    if (step.sourceStateVector[i] == nullptr)
    {
      printable2 = QStringLiteral("-");
    }
    else
    {
      printable2 = QStringLiteral("%1").arg(step.sourceStateVector[i]->getSourceNumber() + 1);
      printable2 += QStringLiteral(".%1").arg(step.sourceStateVector[i]->getRequestNumber());
    }
    item1->setText(printable2);
    ui->sourceStateTable->setItem(i, 0, item1);
  }
  for (int i = 0; i < statistic.getBufferSize(); i++)
  {
    QTableWidgetItem* item1 = protoitem->clone();
    QTableWidgetItem* item2 = protoitem->clone();

    if (step.bufferStateVector[i] == nullptr)
    {
      printable1 = QStringLiteral("Free");
      printable2 = QStringLiteral("-");
    }
    else
    {
      printable1 = QStringLiteral("Busy");
      printable2 = QStringLiteral("%1").arg(step.bufferStateVector[i]->getSourceNumber() + 1);
      printable2 += QStringLiteral(".%1").arg(step.bufferStateVector[i]->getRequestNumber());
    }
    item1->setText(printable1);
    ui->bufferStateTable->setItem(i, 0, item1);

    item2->setText(printable2);
    ui->bufferStateTable->setItem(i, 1, item2);
  }
  for (int i = 0; i < statistic.getBufferSize(); i++)
  {
    QTableWidgetItem* item1 = protoitem->clone();

    if (step.packageStateVector[i] == nullptr)
    {
      printable2 = QStringLiteral("-");
    }
    else
    {
      printable2 = QStringLiteral("%1").arg(step.packageStateVector[i]->getSourceNumber() + 1);
      printable2 += QStringLiteral(".%1").arg(step.packageStateVector[i]->getRequestNumber());
    }
    item1->setText(printable2);
    ui->packageStateVector->setItem(i, 0, item1);
  }
  for (int i = 0; i < statistic.getDeviceAmount(); i++)
  {
    QTableWidgetItem* item1 = protoitem->clone();
    QTableWidgetItem* item2 = protoitem->clone();

    if (step.deviceStateVector[i] == nullptr)
    {
      printable1 = QStringLiteral("Free");
      printable2 = QStringLiteral("-");
    }
    else
    {
      printable1 = QStringLiteral("Busy");
      printable2 = QStringLiteral("%1").arg(step.deviceStateVector[i]->getSourceNumber() + 1);
      printable2 += QStringLiteral(".%1").arg(step.deviceStateVector[i]->getRequestNumber());
    }
    item1->setText(printable1);
    ui->deviceStateTable->setItem(i, 0, item1);

    item2->setText(printable2);
    ui->deviceStateTable->setItem(i, 1, item2);
  }
  if (currentStep == (int)statistic.getStepVector().size() - 1)
  {
    ui->nextButton->setEnabled(false);
  }
  else
  {
    ui->nextButton->setEnabled(true);
  }
  if (currentStep == 0)
  {
    ui->backButton->setEnabled(false);
  }
  else
  {
    ui->backButton->setEnabled(true);
  }
}

std::vector<int> MainWindow::getEarlierDevice(std::vector<Device> vector, double deviceStopTime)
{
  std::vector<Device> deviceVector;

  for (int i = 0; i < (int)vector.size(); i++)
  {
    if (vector[i].getStopTime() < deviceStopTime)
    {
      deviceVector.push_back(vector[i]);
    }
  }
  std::sort(deviceVector.begin(), deviceVector.end(), Comparator());

  std::vector<int> intVector;

  for (int i = 0; i < (int)deviceVector.size(); i++)
  {
    intVector.push_back(deviceVector[i].getDeviceNumber());
  }
  return intVector;
}

void MainWindow::on_autoModeButton_clicked()
{
  runSimulation();
  fillTable();
}

void MainWindow::on_stepModeButton_clicked()
{
  currentStep = 0;

  QString printable;
  QStringList header = { "Request" };
  QStringList headers = { "State", "Request" };
  QStringList headersVertical;
  QStringList headersVertical2;
  QStringList headersVertical3;
  QStringList headersVertical4;

  printable = QStringLiteral("%1").arg(statistic.getStepVector().size());

  ui->allSteps->setText(printable);

  ui->sourceStateTable->setColumnCount(1);
  ui->sourceStateTable->setRowCount(statistic.getSourceAmount());
  ui->sourceStateTable->setHorizontalHeaderLabels(header);

  for (int i = 0; i < statistic.getSourceAmount(); i++)
  {
    printable = QStringLiteral("Source %1").arg(i + 1);
    headersVertical3.push_back(printable);
  }
  ui->sourceStateTable->setVerticalHeaderLabels(headersVertical3);

  ui->bufferStateTable->setColumnCount(2);
  ui->bufferStateTable->setRowCount(statistic.getBufferSize());
  ui->bufferStateTable->setHorizontalHeaderLabels(headers);

  for (int i = 0; i < statistic.getBufferSize(); i++)
  {
    printable = QStringLiteral("Buffer place %1").arg(i + 1);
    headersVertical.push_back(printable);
  }
  ui->bufferStateTable->setVerticalHeaderLabels(headersVertical);

  ui->packageStateVector->setColumnCount(1);
  ui->packageStateVector->setRowCount(statistic.getBufferSize());
  ui->packageStateVector->setHorizontalHeaderLabels(header);

  for (int i = 0; i < statistic.getBufferSize(); i++)
  {
    printable = QStringLiteral("Place %1").arg(i + 1);
    headersVertical4.push_back(printable);
  }
  ui->packageStateVector->setVerticalHeaderLabels(headersVertical4);

  ui->deviceStateTable->setColumnCount(2);
  ui->deviceStateTable->setRowCount(statistic.getDeviceAmount());
  ui->deviceStateTable->setHorizontalHeaderLabels(headers);

  for (int i = 0; i < statistic.getDeviceAmount(); i++)
  {
    printable = QStringLiteral("Device %1").arg(i + 1);
    headersVertical2.push_back(printable);
  }
  ui->deviceStateTable->setVerticalHeaderLabels(headersVertical2);
  ui->tabWidget->setCurrentIndex(2);
  ui->backButton->setEnabled(false);
  stepMode();
}

void MainWindow::on_backButton_clicked()
{
  currentStep--;
  stepMode();
}

void MainWindow::on_nextButton_clicked()
{
  currentStep++;
  stepMode();
}

void MainWindow::on_goToStep_clicked()
{
  currentStep = ui->enterStep->text().toInt() - 1;
  stepMode();
}


