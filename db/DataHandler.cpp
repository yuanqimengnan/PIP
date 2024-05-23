#include "DataHandler.hpp"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QTextStream>
#include <omp.h>
#include <iostream>
DataHandler::DataHandler() {

}

void DataHandler::readPolygons(QString polyPath) {
    QFile fi(polyPath);
    if(!fi.open( QIODevice::ReadOnly | QIODevice::Text )) {
        qDebug() << "Could not open file" << polyPath;
        return;
    }

    QTextStream input(&fi);
    int polysize;

    input >> polysize;
    polys.resize(polysize);

    int currentID = 0;
    int nvertex = 0;
    QString sep;

    while(!input.atEnd()) {
        input >> sep;
        if (sep == "1") // new polygon
        {
            input >> currentID; // ID of polygon
            input >> nvertex; // number of polygon' vertex
            Point v;
            while (nvertex--) {
                input >> v.x >> v.y;
                polys[currentID].push_back(v);
            }
        }
    }

    fi.close();
}

void DataHandler::getTriangulation(std::vector<float> &verts, std::vector<float> &ids) {
    verts.clear();
    ids.clear();

    int mts = omp_get_max_threads();
    std::vector<std::vector<float>> tverts(mts), tids(mts);
    int polysize = this->getNoPolys();

#pragma omp parallel for
    for (size_t i = 0; i < polysize; i++) {
        int id = omp_get_thread_num();
        std::vector<std::vector<Point>> inputPolygons;
        std::vector<Point> outputTriangles; // Every 3 points is a triangle
        std::vector<Point> boundingPolygon;
        inputPolygons.push_back(polys[i]);
        clip2tri clip2tri;
        clip2tri.triangulate(inputPolygons, outputTriangles, boundingPolygon);

        for (size_t j = 0; j<outputTriangles.size(); j++) {
            double x = double(outputTriangles[j].x);
            double y = double(outputTriangles[j].y);
            tverts[id].push_back(x);
            tverts[id].push_back(y);
            tids[id].push_back(i);
        }
    }
    for(int i = 0;i < mts;i ++) {
        verts.insert(verts.end(),tverts[i].begin(),tverts[i].end());
        ids.insert(ids.end(),tids[i].begin(),tids[i].end());
    }


//    float precision = 0.001;
//    for(size_t i = 0; i < verts.size(); i++) {
//        if (verts[i] - 0 <= precision) {
//            verts[i] = 0.0;
//        } else if (1 - verts[i] <= precision) {
//            verts[i] = 1.0;
//        }
//    }
}

int DataHandler::getNoPolys() {
    return this->polys.size();
}

void DataHandler::readPoints(QString pointPath) {
    QFile fi(pointPath);
    if(!fi.open( QIODevice::ReadOnly | QIODevice::Text )) {
        qDebug() << "Could not open file" << pointPath;
        return;
    }
    QTextStream input(&fi);
    float point_x, point_y;
    while(!input.atEnd()) {
        QString line = input.readLine();
        if(line.trimmed().length() == 0) {
            continue;
        }
        QStringList list = line.split(" ");
        point_x = list[0].toFloat();
        point_y = list[1].toFloat();
        point.push_back(point_x);
        point.push_back(point_y);
    }
    fi.close();
}

void DataHandler::initData(QString polyPath, QString pointPath) {
    this->readPolygons(polyPath);
    this->getTriangulation(verts, ids);
    this->readPoints(pointPath);
}
