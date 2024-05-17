#include "DataHandler.hpp"
#include "GLHandler.hpp"

#include <QString>
#include <QApplication>
#include <iostream>
#include <QMap>

DataHandler* dataHandler;
QString polyPath, pointPath;
GLHandler* handler;

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

    //init data
    dataHandler = new DataHandler();
    dataHandler->initData(polyPath, pointPath);

    //init GL
    handler = GLHandler::getInstance();

    //execute query
    QVector<int> agg;
    GLHandler::FunctionType joinType = GLHandler::RasterJoinFn;
    handler->setDataHandler(dataHandler);
    agg = handler->executeFunction(joinType);
}

