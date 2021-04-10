#ifndef SURFACEBOX_H
#define SURFACEBOX_H
#pragma once
//#include <QWidget>
//#include <QtGui>
//#include <QOpenGLFunctions>
//#include <QtOpenGL/QOpenGLBuffer>
//#include <QtOpenGLWidgets/QOpenGLWidget>
//#include <QDebug>
//#include <QOpenGLShader>
//#include <QOpenGLShaderProgram>

//#include "Surface.h"
//class SurfaceBox : public QOpenGLWidget,protected QOpenGLFunctions
//{
//    Q_OBJECT

//public:
//    Surface* surface;
//    GLuint texture;
//    QOpenGLShaderProgram *program;

//    SurfaceBox(QWidget *parent = nullptr){
//        surface=new Surface(800,600);
//        surface->clearCheckboard();
//    };
//    void loadTexture(){
//        glGenTextures(1,&texture);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glTexImage2D(GL_TEXTURE_2D,0,GL_BGRA,surface->width,surface->height,0,GL_BGRA,GL_UNSIGNED_BYTE,surface->getPointAdress(0,0));
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glBindTexture( GL_TEXTURE_2D, 0);
//    }
//    void initializeGL() override{
//        initializeOpenGLFunctions();
//        glClear(GL_COLOR_BUFFER_BIT);
//        glEnable(GL_TEXTURE_2D);
//        loadTexture();
//        QOpenGLShader * vsh=new QOpenGLShader(QOpenGLShader::Vertex, this);
//        const char *vsrc =
//            "#version 150 core\n"
//            "in vec2 in_Vertex;\n"
//            "in vec2 vertTexCoord;\n"
//            "out vec2 fragTexCoord;\n"
//            "void main(void)\n"
//            "{\n"
//            "    gl_Position = vec4(in_Vertex, 0.0, 1.0);\n"
//            "    fragTexCoord = vertTexCoord;\n"
//            "}\n";
//        vsh->compileSourceCode(vsrc);
//        QOpenGLShader *fsh = new QOpenGLShader(QOpenGLShader::Fragment, this);
//        const char *fsrc =
//            "#version 150 core\n"
//            "uniform sampler2D tex;\n"
//            "in vec2 fragTexCoord;\n"
//            "void main(void)\n"
//            "{\n"
//            "    gl_FragColor = texture2D(tex,fragTexCoord);\n"
//            "}\n";
//        fsh->compileSourceCode(fsrc);
//        program = new QOpenGLShaderProgram;
//        program->addShader(vsh);
//        program->addShader(fsh);
//        program->link();
//        program->bind();
//        glActiveTexture(GL_TEXTURE0);
//        program->setUniformValue("tex", 0);
//        //  glDebugMessageCallback(debugCallback, nullptr);
//        //  QOpenGLFunctions* f= QOpenGLContext::currentContext()->functions();
//        //glClearColor( 0, 0, 0, 1 );

//    }
//    void resizeGL(int w, int h) override{
//        glViewport(0, 0, (GLint)w, (GLint)h);
//    }
//    void paintGL() override{
//        // QOpenGLFunctions* f= QOpenGLContext::currentContext()->functions();
//        glClear(GL_COLOR_BUFFER_BIT);
//        program->bind();
//        {
//            float vertices[] = {-1.0,-1.0,  1.0,-1.0,  1.0,1.0,  -1.0,1.0};
//            float coordTexture[] = {0.0,0.0,  1.0,0.0,  1.0,1.0,  0.0,1.0};
//            glVertexAttribPointer(program->attributeLocation("in_Vertex"), 2, GL_FLOAT, GL_FALSE, 0, vertices);
//            glEnableVertexAttribArray(program->attributeLocation("in_Vertex"));
//            glVertexAttribPointer(program->attributeLocation("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, coordTexture);
//            glEnableVertexAttribArray(program->attributeLocation("vertTexCoord"));
//            glBindTexture(GL_TEXTURE_2D, texture);
//            glDrawArrays(GL_QUADS, 0, 4);
//            glBindTexture(GL_TEXTURE_2D, 0);
//            glDisableVertexAttribArray(2);
//            glDisableVertexAttribArray(0);
//        }
//        program->release();
//    //    glDrawPixels(surface->width,surface->height,GL_BGRA,GL_UNSIGNED_BYTE,surface->getPointAdress(0,0));

//    }
//};
#include <QtGui>
#include <QtWidgets>
#include "Surface.h"
#include "SurfaceBoxBaseRenderer.h"
#include "SurfaceBoxRendererList.h"
#include "ScaleFactor.h"
#include "UPixelOps.h"
class SurfaceBox:public QWidget{
private:
    Surface *surface;
    ScaleFactor *factor;
    QImage* image;
    SurfaceBoxBaseRenderer* renderer;

    //SurfaceBoxRender
    //SurfaceBoxRenderList
public:
    const int maxSideLength=32767;
     SurfaceBoxRendererList* rendererList;
    SurfaceBox(){
//        surface=new Surface(800,600);
//        image=new QImage(reinterpret_cast<byte*>(surface->getPointAdress(0,0)),surface->width,surface->height,QImage::Format_ARGB32);
//        surface->clearCheckboard();
        auto t=ScaleFactor::oneToOne();
        factor=&t;
        rendererList=new SurfaceBoxRendererList(this->size(),this->size());
        renderer=new SurfaceBoxBaseRenderer(rendererList,nullptr);
        //TODO Connect invalidate
        rendererList->add(renderer,false);
    }
    void setSurface(Surface* value){
        this->surface=value;
        image=new QImage(reinterpret_cast<byte*>(surface->getPointAdress(0,0)),surface->width,surface->height,QImage::Format_ARGB32);

        this->renderer->source=value;
        if(this->surface!=nullptr){
            this->resize(this->factor->scaleSize(QSize(surface->width,surface->height)));
           // this->setFixedSize(this->factor->scaleSize(QSize(surface->width,surface->height)));
            this->rendererList->setSourceSize(QSize(surface->width,surface->height));
            this->rendererList->setDestinationSize(this->size());
        }

       // this->renderer->
    }
    void fitToSize(QSize fit){
        ScaleFactor newsf=ScaleFactor::min(fit.width(),surface->width,fit.height(),surface->height,ScaleFactor::minValue());
        this->factor=&newsf;
       // this->setFixedSize(this->factor->scaleSize(QSize(surface->width,surface->height)));
        this->resize(this->factor->scaleSize(QSize(surface->width,surface->height)));
    }
    void renderTo(Surface* dst){
        dst->clear(bgra(0,0,0,0));
        if(this->surface!=nullptr){
            SurfaceBoxRendererList sbrl(QSize(surface->width,surface->height),QSize(dst->width,dst->height));
            SurfaceBoxBaseRenderer sbbr(&sbrl,this->surface);
            sbrl.add(&sbbr,true);
            sbrl.render(dst,QPoint(0,0));
            sbrl.remove(&sbbr);
        }
    }
    void resizeEvent(QResizeEvent *event) override{
        QSize ms=this->size();
        if(this->width()==maxSideLength&&surface!=nullptr){
            ms.setHeight((int)(((long)(maxSideLength + 1) * (long)surface->height) / (long)surface->width));
        }else if(ms.width()==0){
            ms.setWidth(1);
        }
        if(this->width()==maxSideLength&&surface!=nullptr){
            ms.setHeight((int)(((long)(maxSideLength + 1) * (long)surface->width) / (long)surface->height));

        }else if(ms.height()==0){
            ms.setHeight(1);
        }
        if(ms!=this->size()){
            this->resize(ms);
           // this->setFixedSize(ms);
        }
        if(surface==nullptr){
            *this->factor=ScaleFactor::oneToOne();
        }else{
            ScaleFactor newsf=ScaleFactor::min(this->width(),surface->width,this->height(),surface->height,ScaleFactor::oneToOne());
            this->factor=&newsf;
        }
        this->rendererList->setDestinationSize(this->size());
    }
    void paintEvent(QPaintEvent *event) override{
        QPainter qp(this);
        Surface temp(this->size().width(),this->size().height());
        renderTo(&temp);
        QImage* imfage=new QImage(reinterpret_cast<byte*>(temp.getPointAdress(0,0)),temp.width,temp.height,QImage::Format_ARGB32);
        qp.drawImage(0,0,*imfage);
    }


};

#endif // SURFACEBOX_H
