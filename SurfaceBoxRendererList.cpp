#include "SurfaceBoxRendererList.h"
#include "SurfaceBoxRenderer.h"
SurfaceBoxRendererList::SurfaceBoxRendererList(QSize srcSize,QSize dstSize)
{
    this->sourceSize=srcSize;
    this->destinationSize=dstSize;
    computeScaleFactor();
}

QVector<QVector<SurfaceBoxRenderer*>>SurfaceBoxRendererList::Renderers(){
    return QVector<QVector<SurfaceBoxRenderer*>>{list,topList};
}
void SurfaceBoxRendererList::computeScaleFactor(){
    factor=new ScaleFactor(this->destinationSize.width(),this->sourceSize.width());
}
QPoint SurfaceBoxRendererList::SrcToDst(QPoint p){
    return this->factor->scalePoint(p);
}
QPoint SurfaceBoxRendererList::DstToSrc(QPoint p){
    return this->factor->unscalePoint(p);
}
QRectF SurfaceBoxRendererList::SrcToDst(QRectF p){
    return this->factor->scaleRectangle(p);
}
QRectF SurfaceBoxRendererList::DstToSrc(QRectF p){
    return this->factor->unscaleRectangle(p);
}
void SurfaceBoxRendererList::add(SurfaceBoxRenderer* renderer,bool alwaystop){
    if(alwaystop)
        topList.push_back(renderer);
    else
        topList.push_back(renderer);
}
void SurfaceBoxRendererList::remove(SurfaceBoxRenderer* renderer){

    list.removeOne(renderer);
    topList.removeOne(renderer);
}
void SurfaceBoxRendererList::invalidateLookups(){
    s2dLookupY.clear();
    s2dLookupX.clear();
    d2sLookupY.clear();
    d2sLookupX.clear();
}
QSize SurfaceBoxRendererList::getSourceSize(){
    return this->sourceSize;
}
void SurfaceBoxRendererList::setSourceSize(QSize value){
    if(this->sourceSize!=value){
        this->sourceSize=value;
        onSourceSizeChanged();
    }
}
QSize SurfaceBoxRendererList::getDestinationSize(){
    return this->destinationSize;
}
void SurfaceBoxRendererList::setDestinationSize(QSize value){
    if(this->destinationSize!=value){
        this->destinationSize=value;
        onSourceSizeChanged();
    }
}
void SurfaceBoxRendererList::onDestinationSizeChanged(){
    invalidateLookups();
    if(this->destinationSize.width()!=0&&this->sourceSize.width()!=0){
        computeScaleFactor();
        for(auto x:this->list)
            x->onDestinationSizeChanged();
        for(auto x:this->topList)
            x->onDestinationSizeChanged();
    }
}
void SurfaceBoxRendererList::onSourceSizeChanged(){
    invalidateLookups();
    if(this->destinationSize.width()!=0&&this->sourceSize.width()!=0){
        computeScaleFactor();
        for(auto x:this->list)
            x->onSourceSizeChanged();
        for(auto x:this->topList)
            x->onSourceSizeChanged();
    }
}

void SurfaceBoxRendererList::createD2SLookupX(){
    if (this->d2sLookupX.length() == 0 || this->d2sLookupX.length() != this->destinationSize.width() + 1)
    {
        d2sLookupX=QVector<int>(this->destinationSize.width() + 1);
        for(int i=0;i<d2sLookupX.length();++i){
            QPoint p(i,0);
            QPoint cp=DstToSrc(p);
            d2sLookupX[i]=Clamp<int>(cp.x(),0,this->sourceSize.width()-1);
        }
    }
}
void SurfaceBoxRendererList::createD2SLookupY(){
    if (this->d2sLookupY.length() == 0 || this->d2sLookupY.length() != this->destinationSize.height() + 1)
    {
        d2sLookupY=QVector<int>(this->destinationSize.height() + 1);
        for(int i=0;i<d2sLookupY.length();++i){
            QPoint p(0,i);
            QPoint cp=DstToSrc(p);
            d2sLookupY[i]=Clamp<int>(cp.y(),0,this->sourceSize.height()-1);
        }
    }
}

void SurfaceBoxRendererList::createS2DLookupX(){
    if (this->s2dLookupX.length() == 0 || this->s2dLookupX.length() != this->sourceSize.width() + 1)
    {
        s2dLookupX=QVector<int>(this->sourceSize.width() + 1);
        for(int i=0;i<s2dLookupX.length();++i){
            QPoint p(i,0);
            QPoint cp=SrcToDst(p);
            s2dLookupX[i]=Clamp<int>(cp.x(),0,this->destinationSize.width()-1);
        }
    }
}
void SurfaceBoxRendererList::createS2DLookupY(){
    if (this->s2dLookupY.length() == 0 || this->s2dLookupY.length() != this->sourceSize.height() + 1)
    {
        s2dLookupY=QVector<int>(this->sourceSize.height() + 1);
        for(int i=0;i<s2dLookupY.length();++i){
            QPoint p(0,i);
            QPoint cp=SrcToDst(p);
            s2dLookupY[i]=Clamp<int>(cp.y(),0,this->destinationSize.height()-1);
        }
    }
}

void SurfaceBoxRendererList::render(Surface* dst, QPoint offset)
{
    for(auto i:this->list){
        if(i->getVisible()){
            i->render(dst,offset);
        }
    }
    for(auto i:this->topList){
        if(i->getVisible()){
            i->render(dst,offset);
        }
    }
}


void SurfaceBoxRendererList::invalidate(QRect rect)
{
    emit invalidated(rect);
}

void SurfaceBoxRendererList::invalidate()
{
    invalidate(SurfaceBoxRenderer::maxBounds());
}
