#include "DataHandler.hpp"
#include "GLHandler.hpp"
#include "forexperiment.h"

#include <QString>
#include <QApplication>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>

DataHandler* dataHandler;
QString polyPath, pointPath, experimentPath;
bool outputResult;
uint nIter;
GLHandler* handler;

QElapsedTimer timer, timer1;
float_t initpolygons_time, initpoints_time, initGL_time, passpolygons_time, passpoints_time, renderpolygons_time, renderpoints_time, getresult_time, total_time = 0.0;

bool parseArguments(const QMap<QString,QString> &args) {
    QList<QString> keys = args.keys();

    if(keys.contains("--help")) {
        return false;
    }
    if(keys.contains("--polyPath")) {
        polyPath = args["--polyPath"];
    } else {
        return false;
    }
    if(keys.contains("--pointPath")) {
        pointPath = args["--pointPath"];
    } else {
        return false;
    }
    if(keys.contains("--outputResult")) {
        outputResult = true;
        if(keys.contains("--experimentPath")) {
            experimentPath = args["--experimentPath"];
        } else {
            return false;
        }
    } else {
        outputResult = false;
    }
    if(keys.contains("--nIter")) {
        nIter = args["--nIter"].toInt();
    } else {
        return false;
    }

    return true;
}
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QStringList argString = app.arguments();
    QMap<QString, QString> args;
    for(int i = 1;i < argString.size();i ++) {
        QString arg1 = argString[i];
        if(arg1.startsWith("--")) {
            QString arg2 = " ";
            if((i + 1) < argString.length()) {
                arg2 = argString[i + 1];
            }
            if(arg2.startsWith("--")) {
                args[arg1] = " ";
            } else {
                args[arg1] = arg2;
            }
        }
    }
    if(!parseArguments(args)) {
        cout << "help message: see code!" << "\n";
        return 1;
    }

    timer1.start();

    //init data
    dataHandler = new DataHandler();
    dataHandler->initData(polyPath, pointPath);

    //init GL
    timer.restart();
    handler = GLHandler::getInstance();
    initGL_time = timer.nsecsElapsed();

    //execute query
    QVector<int> agg;
    GLHandler::FunctionType joinType = GLHandler::RasterJoinFn;
    handler->setDataHandler(dataHandler);

    for (uint i=0; i<nIter; i++) {
       agg = handler->executeFunction(joinType);
    }

    total_time = timer1.elapsed();

    //convert ns to ms
    initpolygons_time = std::round(initpolygons_time/10000.0f)/100.0f;
    initpoints_time = std::round(initpoints_time/10000.0f)/100.0f;
    initGL_time = std::round(initGL_time/10000.0f)/100.0f;
    passpolygons_time = (std::round(passpolygons_time/10000.0f)/100.0f ) / float_t(nIter);
    passpoints_time = (std::round(passpoints_time/10000.0f)/100.0f ) / float_t(nIter);
    renderpolygons_time = (std::round(renderpolygons_time/10000.0f)/100.0f ) / float_t(nIter);
    renderpoints_time = (std::round(renderpoints_time/10000.0f)/100.0f ) / float_t(nIter);
    getresult_time = (std::round(getresult_time/10000.0f)/100.0f ) / float_t(nIter);

    //output the result of experiment
    if (outputResult) {
        QFile file(experimentPath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            qDebug() << "Can't open the experimentPath: " << experimentPath << endl;
        }
        QTextStream out(&file);
        out << "initPolygons, initPoints, initGL, passPolygons, passPoints, renderPolygons, renderPoints, getResult, totalTime(nIter)\n";
        out << initpolygons_time << ',' << initpoints_time << ',' << initGL_time << ',' << passpolygons_time << ',' << passpoints_time << ',' << renderpolygons_time << ',' << renderpoints_time << ',' << getresult_time << ',' << total_time << '\n';
        file.close();
    }
    std::cout << "initPolygons: " << initpolygons_time << "ms" << endl;
    std::cout << "initPoints: " << initpoints_time << "ms" << endl;
    std::cout << "initGL: " << initGL_time << "ms" << endl;
    std::cout << "avg_passPolygons: " << passpolygons_time << "ms" << endl;
    std::cout << "avg_passPoints: " << passpoints_time << "ms" << endl;
    std::cout << "avg_renderPolygons: " << renderpolygons_time << "ms" << endl;
    std::cout << "avg_renderPoints: " << renderpoints_time << "ms" << endl;
    std::cout << "avg_getResult: " << getresult_time << "ms" << endl;
    std::cout << "totalTime(nIter): " << total_time << "ms" << endl;
}

