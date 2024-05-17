#ifndef RASTERJOIN_H
#define RASTERJOIN_H

#include "GLFunction.hpp"
// #include<unordered_set>
#include<set>
#include<vector>

class RasterJoin : public GLFunction
{
public:
    RasterJoin(GLHandler *handler);
    ~RasterJoin();

protected:
    void initGL();
    void updateBuffers();
    QVector<int> executeFunction();

    void renderPoints();
    void renderPolys();
    void performJoin();

    std::vector<int> pos1;

protected:
    PQOpenGLFramebufferObject pointsFbo;
    PQOpenGLFramebufferObject polyFbo;

    PQOpenGLShaderProgram pointsShader;
    PQOpenGLShaderProgram polyShader;

// Handling multiple resolutions
protected:
    int maxRes;
    int resx, resy, splitx,splity;
    int actualResX, actualResY;

public:
    GLuint query;
};

#endif // RASTERJOIN_H
