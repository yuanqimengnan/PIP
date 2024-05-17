#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

#include <QVector>
#include <QPolygonF>
#include <QString>
#include <vector>
#include "clipper/clipper.hpp"
#include "poly2tri/poly2tri.h"
#include "clip2tri/clip2tri.h"

using namespace c2t;

typedef vector<Point> TPolygon;
typedef QVector<TPolygon> PolygonCollection;

class DataHandler
{
public:
    DataHandler();
    ~DataHandler();

    void initData(QString polyPath, QString pointPath);

public:
    PolygonCollection polys;
    vector<float> verts, ids; // to store the result of trianglulate
    vector<float> point;

public:
    void readPolygons(QString polyPath);
    void getTriangulation(vector<float> &verts, vector<float> &ids);
    int getNoPolys();
    void readPoints(QString pointPath);

};

#endif // DATAHANDLER_HPP
