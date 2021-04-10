//#include "SurfaceBox.h"
//SurfaceBox::SurfaceBox(QWidget *parent){
//    temp=new Surface(800,600);
//    temp->clearCheckboard();
//}
//void SurfaceBox::initializeGL(){
//    initializeOpenGLFunctions();
//    glEnable(GL_TEXTURE_2D);
//    //  glDebugMessageCallback(debugCallback, nullptr);
//    //  QOpenGLFunctions* f= QOpenGLContext::currentContext()->functions();
//    glClearColor( 0, 0, 0, 1 );
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}
//void SurfaceBox::resizeGL(int w, int h){
//    glViewport(0, 0, (GLint)w, (GLint)h);
//}
//void SurfaceBox::paintGL(){
//    // QOpenGLFunctions* f= QOpenGLContext::currentContext()->functions();
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glDrawPixels(temp->width,temp->height,GL_BGRA,GL_UNSIGNED_INT,temp->getData());
//    //QOpenGLFunctions_3_2_Core*f=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
//}
