#include "ProjectAPS.h"

#include <stdio.h>
#include <QApplication>
#include <iostream>
#include <cmath>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    /*srand(time(NULL));
    double alpha = 0.1;
    double beta = 1;
    double lambda = 0.1;
    for (int i = 0; i < 30; i++)
    {
        //std::cout <<(((double)qrand() / (double)RAND_MAX) * (beta - alpha) + alpha) << std::endl;
        std::cout <<"LAMBDA:"<<(-1 / lambda * std::log((double)qrand() / (double)RAND_MAX))<<std::endl;
    }*/
}
