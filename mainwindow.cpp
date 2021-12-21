#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      statistic_(0,0,0,0,0,0,0)
{
    ui->setupUi(this);
    ui->tab_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void minTimeSource(std::vector<Source> &sources)
{
    std::sort(sources.begin(), sources.end(), cmp());
}

void MainWindow::on_startButton_clicked()
{
    runSession();
    buildTable();
}

void MainWindow::on_startStepModeButton_clicked()
{
    runSession();
    step_ = 0;

    QString printable;
    QStringList headers = {"State", "Request"};
    QStringList headersVertical;
    QStringList headersVertical2;

    printable = QStringLiteral("%1").arg(statistic_.getSteps().size());
    ui->allSteps->setText(printable);

    ui->bufferStateTable->setColumnCount(2);
    ui->bufferStateTable->setRowCount(statistic_.getBufferSize());
    ui->bufferStateTable->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < statistic_.getBufferSize(); i++)
    {
        printable = QStringLiteral("Place %1").arg(i+1);
        headersVertical.push_back(printable);
    }
    ui->bufferStateTable->setVerticalHeaderLabels(headersVertical);

    ui->deviceStateTable->setColumnCount(2);
    ui->deviceStateTable->setRowCount(statistic_.getDevicesQuantity());
    ui->deviceStateTable->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < statistic_.getDevicesQuantity(); i++)
    {
        printable = QStringLiteral("Device %1").arg(i+1);
        headersVertical2.push_back(printable);
    }
    ui->deviceStateTable->setVerticalHeaderLabels(headersVertical2);

    ui->tabWidget->setCurrentIndex(2);
    ui->backButton->setEnabled(false);
    
    stepMode();
}

void MainWindow::on_backButton_clicked()
{
  if (step_ > 0) {
    step_ -= 1;
    stepMode();
  }
}

void MainWindow::on_nextButton_clicked()
{
    step_ += 1;
    stepMode();
}

void MainWindow::on_goToStep_clicked()
{
    step_ = ui->enterStep->text().toInt() - 1;
    stepMode();
}

void MainWindow::buildTable()
{
    double p = 0.0;

    QString printable;
    QStringList headers = {" Generated ", " Processed ", " Rejected ", " Reject probability ", " Time in system ", " Waiting dispersion ", " Servicing dispersion "};
    QStringList headersVertical;

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(statistic_.getSourceQantity());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

    auto * protoitem = new QTableWidgetItem();
    protoitem->setTextAlignment(Qt::AlignCenter);

    for (int i = 0; i < statistic_.getSourceQantity(); i++)
    {
        printable = QStringLiteral("Source %1").arg(i+1);
        headersVertical.push_back(printable);

        QTableWidgetItem *item1 = protoitem->clone();
        printable = QStringLiteral("%1").arg(statistic_.getProcessed(i)+statistic_.getRejected(i));
        item1->setText(printable);
        ui->tableWidget->setItem(i, 0, item1);

        QTableWidgetItem *item2 = protoitem->clone();
        printable = QStringLiteral("%1").arg(statistic_.getProcessed(i));
        item2->setText(printable);
        ui->tableWidget->setItem(i, 1, item2);

        QTableWidgetItem *item3 = protoitem->clone();
        p += statistic_.getRejected(i);
        printable = QStringLiteral("%1").arg(statistic_.getRejected(i));
        item3->setText(printable);
        ui->tableWidget->setItem(i, 2, item3);

        QTableWidgetItem *item4 = protoitem->clone();
        double rejectProb = static_cast<double>(statistic_.getRejected(i)) / static_cast<double>(statistic_.getProcessed(i)+statistic_.getRejected(i));
        printable = QStringLiteral("%1").arg(rejectProb);
        item4->setText(printable);
        ui->tableWidget->setItem(i, 3, item4);

        QTableWidgetItem *item5 = protoitem->clone();
        printable = QStringLiteral("%1").arg(statistic_.getTimeInSystem(i) / (statistic_.getProcessed(i)+statistic_.getRejected(i)));
        item5->setText(printable);
        ui->tableWidget->setItem(i, 4, item5);

        QTableWidgetItem *item6 = protoitem->clone();
        printable = QStringLiteral("%1").arg(statistic_.getWaitingTimeDisp(i));
        item6->setText(printable);
        ui->tableWidget->setItem(i, 5, item6);

        QTableWidgetItem *item7 = protoitem->clone();
        printable = QStringLiteral("%1").arg(statistic_.getServiceTimeDisp(i));
        item7->setText(printable);
        ui->tableWidget->setItem(i, 6, item7);
    }
    ui->tableWidget->setVerticalHeaderLabels(headersVertical);


    QStringList header = {"Devise usage"};
    QStringList headerVertical;
    ui->tableWidget_2->setColumnCount(1);
    ui->tableWidget_2->setRowCount(statistic_.getDevicesQuantity());
    ui->tableWidget_2->setHorizontalHeaderLabels(header);

    for (int i = 0; i < statistic_.getDevicesQuantity(); i++)
    {
        printable = QStringLiteral("Device %1").arg(i+1);
        headerVertical.push_back(printable);
        QTableWidgetItem *item1 = protoitem->clone();
        printable = QStringLiteral("%1").arg(statistic_.getDeviceWorkingTime(i) / statistic_.getSystemTime());
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
    step_t step = statistic_.getSteps()[step_];

    printable1 = QString::fromStdString(step.actionString);
    ui->stepText->setText(printable1);

    printable1 = QStringLiteral("%1").arg(step.time);
    ui->timeValue->setText(printable1);

    printable1 = QStringLiteral("%1").arg(step_ + 1);
    ui->currentStep ->setText(printable1);

    printable1 = QStringLiteral("%1").arg(step.addPointer+1);
    ui->addPtr->setText(printable1);

    printable1 = QStringLiteral("%1").arg(step.getPointer+1);
    ui->getPtr->setText(printable1);

    auto * protoitem = new QTableWidgetItem();
    protoitem->setTextAlignment(Qt::AlignCenter);

    for (int i = 0; i < statistic_.getBufferSize(); i++)
    {
        QTableWidgetItem *item1 = protoitem->clone();
        QTableWidgetItem *item2 = protoitem->clone();

        if (step.bufferStateVector[i] == nullptr)
        {
            printable1 = QStringLiteral("Free");
            printable2 = QStringLiteral("-");
        } else {
            printable1 = QStringLiteral("Busy");
            printable2 = QStringLiteral("%1").arg(step.bufferStateVector[i]->getSerialNumber());
        }

        item1->setText(printable1);
        ui->bufferStateTable->setItem(i, 0, item1);

        item2->setText(printable2);
        ui->bufferStateTable->setItem(i, 1, item2);
    }

    for (int i = 0; i < statistic_.getDevicesQuantity(); i++)
    {
        QTableWidgetItem *item1 = protoitem->clone();
        QTableWidgetItem *item2 = protoitem->clone();

        if (step.deviceStateVector[i] == nullptr)
        {
            printable1 = QStringLiteral("Free");
            printable2 = QStringLiteral("-");
        } else {
            printable1 = QStringLiteral("Busy");
            printable2 = QStringLiteral("%1").arg(step.deviceStateVector[i]->getSerialNumber());
        }

        item1->setText(printable1);
        ui->deviceStateTable->setItem(i, 0, item1);

        item2->setText(printable2);
        ui->deviceStateTable->setItem(i, 1, item2);
    }

    if (step_ == statistic_.getSteps().size() - 1)
    {
        ui->nextButton->setEnabled(false);
    } else {
        ui->nextButton->setEnabled(true);
    }

    if (step_ == 0)
    {
        ui->backButton->setEnabled(false);
    } else {
        ui->backButton->setEnabled(true);
    }
}

void MainWindow::runSession()
{

    Statistic statistic(ui->request_quantity->text().toInt(),
                        ui->source_quantity->text().toInt(),
                        ui->buffer_size->text().toInt(),
                        ui->device_quantity->text().toInt(),
                        ui->lambda->text().toDouble(),
                        ui->A->text().toDouble(),
                        ui->B->text().toDouble());

    std::vector<Source> soursesVector;
    std::vector<Device> devicesVector;
    std::vector<requestPointer> requestsVector;
    Buffer buffer(statistic.getBufferSize());

    for (int i = 0; i < statistic.getSourceQantity(); i++)
    {
        Source source(i, statistic.getLambda());
        soursesVector.push_back(source);
    }

    for (int i = 0; i < statistic.getDevicesQuantity(); i++)
    {
        Device device(i, statistic.getA_(),statistic.getB_());
        devicesVector.push_back(device);
    }

    for (int i = 0; i < statistic.getRequestQantity(); i++)
    {
        if (soursesVector[0].getRequestTime() > soursesVector[1].getRequestTime())
        {
            minTimeSource(soursesVector);
        }

        requestsVector.push_back(soursesVector[0].generation());
    }   
    std::sort(requestsVector.begin(), requestsVector.end(), cmp());

    int requestNumber = 0;
    int deviceNumber = 0;
    bool isRejected = false;
    step_t step;
    step.bufferStateVector = std::vector<requestPointer>(statistic.getBufferSize(), nullptr);
    step.deviceStateVector = std::vector<requestPointer>(statistic.getDevicesQuantity(), nullptr);
    step.addPointer = 0;
    step.getPointer = 0;

    while (statistic.getProcessed() + statistic.getRejected() != statistic.getRequestQantity())
    {
      if (requestNumber != statistic.getRequestQantity())
      {
        requestsVector[requestNumber]->setSerialNumber(requestNumber+1);
        bool isAdded = buffer.addRequest(requestsVector[requestNumber], step);
        if (isAdded)
        {
          statistic.addStep(step);
        }
        else {
          isRejected = true;
        }

        for (int i = 0; i < statistic.getDevicesQuantity(); i++)
        {
          if (devicesVector[i].isAvaliable(requestsVector[requestNumber]->getStartTime(), statistic, step))
          {
            devicesVector[i].getRequest(buffer.getRequest(step), step);
            statistic.addStep(step);

            if (isRejected)
            {
              buffer.addRequest(requestsVector[requestNumber], step);
              statistic.addStep(step);
              isRejected = false;
            }

            break;
          }

        }

        if (isRejected)
        {
          buffer.removeRequest(statistic, step);
          buffer.addRequest(requestsVector[requestNumber], step);
          statistic.addStep(step);

          isRejected = false;
        }

        requestNumber++;
      }
      else {
        for (int i = 0; i < statistic.getDevicesQuantity() && !buffer.isEmpty(); i++)
        {
          if (devicesVector[i].isAvaliable(statistic, step))
          {
            devicesVector[i].getRequest(buffer.getRequest(step), step);
            break;
          }
        }

        if (buffer.isEmpty())
        {
          devicesVector[deviceNumber].isAvaliable(statistic, step);
          deviceNumber = deviceNumber != statistic.getDevicesQuantity() - 1 ? deviceNumber + 1 : 0;
        }
      }
    }
    statistic_ = statistic;
}
