#ifndef GLDATA_HPP
#define GLDATA_HPP

#include <qgl.h>
#include <QByteArray>
#include <QImage>
#include <QSharedPointer>
#include <QOpenGLFramebufferObject>
#include <stdint.h>

#ifndef USE_EGL
#include <QOpenGLShaderProgram>
typedef QOpenGLFramebufferObject FBOObject;
#else
#include <OpenGLShaderProgram.hpp>
#include <OpenGLFrameBufferObject.hpp>

typedef OpenGLShaderProgram QOpenGLShaderProgram;
typedef OpenGLShader QOpenGLShader;
typedef OpenGLFrameBufferObject FBOObject;
#endif
class QByteArray;

typedef QSharedPointer<FBOObject> PQOpenGLFramebufferObject;
typedef QSharedPointer<QOpenGLShaderProgram> PQOpenGLShaderProgram;
typedef std::vector<char> ByteArray;

struct GLBuffer
{
    GLBuffer(): id(-1), size(0), mapped(true) {}
    void       generate(GLenum target, bool mapped = true);
    void       resize(GLenum usage, GLsizeiptr datasize);
    void       setData(GLenum usage, const GLvoid* data, GLsizeiptr dataSize, GLsizeiptr offset=0);
    void       setData1(GLenum usage, const GLvoid* data, GLsizeiptr dataSize, GLsizeiptr offset, bool flag = true);
    void       bind();
    void       release();

    void *     map(GLenum usage, GLsizeiptr bufferSize);
    void       unmap();

    void       setData(GLenum usage, const QByteArray *data, int numData);


    GLenum     target;
    GLuint     id;
    GLsizeiptr size;
    bool       mapped;

    template<typename T> static
    QByteArray vec2ba(const QVector<T>& vec)
    {
        return QByteArray::fromRawData((const char*)vec.constData(), sizeof(T) * vec.size());
    }
};

struct GLTexture
{
    GLTexture(): id(0) {}
    GLTexture(const QImage &img) { this->setImage(img); }
    void   bind();
    void   release();
    void   ensureSize(const QSize &newSize);
    void   setImage(const QImage &img);

    GLuint id;
    QSize  size;
};

struct GLTextureBuffer {
    GLTextureBuffer(): texId(0) {}
    void create(int size, GLenum format, void *data);
    QVector<int> getBuffer(uint32_t result_size);
    void destroy();

    int size;
    GLuint texId;
    GLuint ssbo;
};


#endif // GLDATA_HPP

