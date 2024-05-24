#include <GL/glew.h>
#include "GLHandler.hpp"

#include <QDebug>
#include <cassert>
#include <iostream>

#include "RasterJoin.hpp"
#include <algorithm>

#if defined (__linux__)
#include <GL/glx.h>
#endif


GLHandler* GLHandler::instance = NULL;

GLHandler::GLHandler() {

    this->initializeGL();
}

GLHandler::~GLHandler() {
    delete surface;
    delete context;
    GLHandler::instance = NULL;
}

GLHandler *GLHandler::getInstance() {
    if(instance == NULL) {
        instance = new GLHandler();
    }
    return instance;
}

void GLHandler::setDataHandler(DataHandler * dataHandler) {
    this->dataHandler = dataHandler;
}

void GLHandler::setupContext() {
#ifndef USE_EGL
    context->makeCurrent(surface);
#endif
}

void GLHandler::initializeGL() {

    surface = new QOffscreenSurface();
    surface->setFormat(QSurfaceFormat::defaultFormat());
    surface->create();
    assert(surface->isValid());


    context = new QOpenGLContext();
    context->setFormat(QSurfaceFormat::defaultFormat());
    context->create();
    assert(context->isValid());

    context->makeCurrent(surface);

    // setup glew
    glewExperimental = GL_TRUE;
    glewInit();

    this->initFunctions();
}

void GLHandler::initFunctions() {
    // setup and initialize the different functions
    {
        PGLFunction function = PGLFunction(new RasterJoin(this));
        function->initGL();
        this->functions[RasterJoinFn] = function;
    }
}

QVector<int> GLHandler::executeFunction(FunctionType fn) {
        if (dataHandler == NULL) {
            qDebug() << "DataHandler not set.";
            return QVector<int>();
        }

        this->setupContext();
        if(this->functions.contains(fn)) {
            return this->functions[fn]->execute();
        } else {
            qDebug() << "Function type" << fn << "not yet supported";
        }
    return QVector<int>();
}


