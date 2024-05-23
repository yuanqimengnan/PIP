#include <GL/glew.h>
#include "GLFunction.hpp"
#include "GLHandler.hpp"

#include <iostream>
#include <iomanip>
#include <QElapsedTimer>
using namespace std;
GLFunction::GLFunction(GLHandler *handler): handler(handler), dirty(true)
{

}

GLFunction::~GLFunction() {}

void GLFunction::setupForRender() {
    if(dirty) {
        this->updateBuffers();
    }
    dirty = false;
}

QVector<int> GLFunction::execute() {

    polySize = this->handler->dataHandler->getNoPolys();
    // result = QVector<int>(polySize * 3,0);
    // result = QVector<int>(result_size*4);
    this->setupForRender();
    return this->executeFunction();
}

void GLFunction::initGL() {
}

void GLFunction::updateBuffers() {
}


void GLFunction::setupPolygons() {
    DataHandler * data = this->handler->dataHandler;
    nopolys = data->getNoPolys();

    // loading polygon into memory
    psize = data->verts.size() / 2;
    this->polyBuffer->resize(GL_DYNAMIC_DRAW,psize*3* sizeof(float));
    this->polyBuffer->setData(GL_DYNAMIC_DRAW,data->verts.data(),psize*2* sizeof(float),0);// vertex of polygons
    this->polyBuffer->setData(GL_DYNAMIC_DRAW,data->ids.data(),psize* sizeof(float),psize* sizeof(float)*2);// id of polygons
}

void GLFunction::setupPoints() {
    DataHandler * data = this->handler->dataHandler;
    pointsize = data->point.size() / 2;
    this->pbuffer->resize(GL_DYNAMIC_DRAW, (pointsize * 2 * sizeof(float)));
    this->pbuffer->setData1(GL_DYNAMIC_DRAW, data->point.data(), pointsize * 2 * sizeof(float), 0, false);
    uint32_t result_size = pointsize;
    result = QVector<int>(result_size, 9999);
    texBuf.create((result_size)  * sizeof(GLint), GL_R32I, result.data());

}
