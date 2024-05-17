#ifndef GLHANDLER_H
#define GLHANDLER_H

#include <QOpenGLContext>
#include <QOffscreenSurface>

#include <QSharedPointer>
#include <QVector>

#include "GLFunction.hpp"
#include "DataHandler.hpp"
#include "GLData.hpp"


typedef QSharedPointer<GLFunction> PGLFunction;

class GLHandler
{
public:
    enum FunctionType {
        RasterJoinFn = 0,
    };

private:
    GLHandler();
    ~GLHandler();
    static GLHandler* instance;

public:
    static GLHandler* getInstance();

public:
    void setupContext();
    void initializeGL();

    QVector<int> executeFunction(FunctionType fn);
    void setDataHandler(DataHandler *dataHandler);

protected:
    void initFunctions();

protected:
    QOpenGLContext * context;
    QOffscreenSurface * surface;
    QMap<FunctionType, PGLFunction> functions;

public:
    DataHandler *dataHandler;
};

#endif // GLHANDLER_H
