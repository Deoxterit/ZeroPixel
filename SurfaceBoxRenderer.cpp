#include "SurfaceBoxRendererList.h"
#include "SurfaceBoxRenderer.h"

SurfaceBoxRenderer::SurfaceBoxRenderer(SurfaceBoxRendererList* parent)
{
    this->parent=parent;
    this->visible=true;

}

void SurfaceBoxRenderer::onInvalidate(QRect rect)
{
    this->parent->invalidate(rect);

}

void SurfaceBoxRenderer::invalidate()
{
    invalidate(maxBounds());
}

void SurfaceBoxRenderer::invalidate(QRect rect)
{
    onInvalidate(rect);
}

void SurfaceBoxRenderer::invalidate(QRectF rect)
{
    invalidate(QRect(rect.left(),rect.top(),rect.width(),rect.height()));
}

QRect SurfaceBoxRenderer::maxBounds()
{
    return QRect(minXCoord,minYCoord,maxXCoord+1,maxYCoord+1);
}

bool SurfaceBoxRenderer::getVisible()
{
    return visible;
}

void SurfaceBoxRenderer::setVisible(bool value)
{
    if(this->visible!=value){
        onVisibleChanging();
        this->visible=value;
        onVisibleChanged();
    }
}

QSize SurfaceBoxRenderer::getSourceSize(){
    return this->parent->getSourceSize();
}

QSize SurfaceBoxRenderer::getDestinationSize()
{
    return this->parent->getDestinationSize();
}

