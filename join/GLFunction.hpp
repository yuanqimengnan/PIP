#ifndef GLFUNCTION_H
#define GLFUNCTION_H

#include <QVector>
#include <QPointF>
#include <QSize>

#include "GLData.hpp"

#define PROFILE_GL

//#define FULL_SUMMARY_GL
#define MAX_FBO_SIZE 8192

class GLHandler;
struct GLBuffer;

class GLFunction
{
public:
    GLFunction(GLHandler *handler);
    ~GLFunction();

public:
    void setupForRender();
    QVector<int> execute();

public:
    virtual void initGL();

protected:
    virtual void updateBuffers();
    virtual QVector<int> executeFunction() = 0;

// common functions
public:
    void setupPolygons();
    void setupPoints();

protected:
    GLuint gvao;
    GLHandler *handler;
    GLBuffer *buffer;
    GLBuffer *pbuffer;
    bool dirty;

protected:
    QSize size;
    double cellSize;

protected:
    GLBuffer *polyBuffer;
    GLTextureBuffer texBuf;
    uint32_t psize, pointsize;
    uint32_t nopolys;
    QVector<int> result;

// For index and hybrid joins
protected:
    GLTextureBuffer polyBufferGL;


public:
    uint32_t polySize;
};

#endif // GLFUNCTION_H
