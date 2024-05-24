#include <GL/glew.h>
#include "GLData.hpp"

#include <cassert>
#include <iostream>

void GLBuffer::generate(GLenum tgt, bool mapped) {
    glGenBuffers(1, &this->id);
    this->target = tgt;
    this->size = 0;
    this->mapped = mapped;
}

void GLBuffer::resize(GLenum usage, GLsizeiptr dataSize)
{
    if (this->size<dataSize) {
        glBindBuffer(this->target, this->id);
        glBufferData(this->target, dataSize, 0, usage);
        this->size = dataSize;
        GLenum err = glGetError();
        if(err != 0) {
            std::cout << "Memory error: " << err << " " << dataSize << "\n";
            exit(0);
        }

    }
}
// this->handler->attrBuf->setData(GL_DYNAMIC_DRAW,attribTypes.data(),3 * sizeof(GLfloat),0);
// this->polyBuffer->setData(GL_DYNAMIC_DRAW,data->verts.data(),psize*2,0);
void GLBuffer::setData(GLenum usage, const GLvoid* data, GLsizeiptr dataSize, GLsizeiptr offset)
{
    if (dataSize==0) return;
    this->resize(usage, offset+dataSize);
    glBindBuffer(this->target, this->id);
    if(mapped) {
        void *dest = glMapBufferRange(this->target, offset, dataSize,
                                      GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT |
                                      (GL_MAP_INVALIDATE_RANGE_BIT));
        memcpy(dest, data, dataSize);
        glUnmapBuffer(this->target);
    } else {
        glBufferSubData(this->target, offset, dataSize, data);
    }
}

//  this->pbuffer->setData1(GL_DYNAMIC_DRAW, myId.data(), result_size * 4, result_size * 8, false);
void GLBuffer::setData1(GLenum usage, const GLvoid* data, GLsizeiptr dataSize, GLsizeiptr offset, bool flag)
{
    if (dataSize==0) return;
    // if(flag){this->resize(usage, dataSize*4); flag = false;}
    glBindBuffer(this->target, this->id);
    if(mapped) {
        void *dest = glMapBufferRange(this->target, offset, dataSize,
                                      GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT |
                                      (GL_MAP_INVALIDATE_RANGE_BIT));
        memcpy(dest, data, dataSize);
        glUnmapBuffer(this->target);
    } else {
        glBufferSubData(this->target, offset, dataSize, data);
    }
}

void GLBuffer::setData(GLenum usage, const QByteArray *data, int numData)
{
    int totalMem = 0;
    for (int i=0; i<numData; i++)
        totalMem += data[i].size();

    uint8_t *arrayData = (uint8_t*)this->map(GL_STREAM_DRAW, totalMem);
    for (int i=0; i<numData; i++) {
        if (data[i].size()>0) {
            memcpy(arrayData, data[i].constData(), data[i].size());
            arrayData += data[i].size();
        }
    }
    this->unmap();
}

void * GLBuffer::map(GLenum usage, GLsizeiptr dataSize)
{
    if (dataSize==0) return 0;
    this->resize(usage, dataSize);
    return glMapBufferRange(this->target, 0, dataSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
}

void GLBuffer::unmap()
{
    glUnmapBuffer(this->target);
}

void GLBuffer::bind()
{
    glBindBuffer(this->target, this->id);
}

void GLBuffer::release()
{
    glBindBuffer(this->target, 0);
}

void GLTexture::bind()
{
    if (!this->size.isValid()) {
        glGenTextures(1, &this->id);
        glBindTexture(GL_TEXTURE_2D, this->id);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
        glBindTexture(GL_TEXTURE_2D, this->id);
}

void GLTexture::release()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::ensureSize(const QSize &newSize)
{
    {
        this->size = QSize(std::max(this->size.width(), newSize.width()),
                           std::max(this->size.height(), newSize.height()));
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     this->size.width(), this->size.height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
}

void GLTexture::setImage(const QImage &img)
{
    QImage texImg = QGLWidget::convertToGLFormat(img);
    this->bind();
    this->ensureSize(texImg.size());
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                    texImg.width(), texImg.height(),
                    GL_RGBA, GL_UNSIGNED_BYTE, texImg.bits());
}


void GLTextureBuffer::create(int size, GLenum format, void* data)
{
    this->size = size;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
}

QVector<int> GLTextureBuffer::getBuffer(uint32_t result_size)
{
    QVector<int> polysid(result_size);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, result_size * sizeof(float), polysid.data());
    GLenum err = glGetError();
    if( err > 0 ){
        QString strError;
        strError.sprintf("%s", glewGetErrorString(err));
        qDebug() << "getBuffer error: " << strError;
    }
    return polysid;
}


void GLTextureBuffer::destroy()
{

    if( texId > 0 )
        glDeleteTextures( 1, &texId); //delete previously created texture
    if( ssbo > 0 )
        glDeleteBuffers( 1, &ssbo );  //delete previously created tbo

    texId = 0;
    ssbo = 0;
}


