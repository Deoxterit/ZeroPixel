#ifndef SURFACEBOXRENDERERLIST_H
#define SURFACEBOXRENDERERLIST_H
#include <QtCore>
#include <QObject>
#include <vector>
#include "Surface.h"
#include "ScaleFactor.h"
class SurfaceBoxRenderer;
class SurfaceBoxRendererList:public QObject{
    Q_OBJECT
private:
    QVector<SurfaceBoxRenderer*> list;
    QVector<SurfaceBoxRenderer*> topList;
    ScaleFactor* factor;
    QSize sourceSize;
    QSize destinationSize;
public:
    QVector<int> s2dLookupY,s2dLookupX,d2sLookupY,d2sLookupX;
    SurfaceBoxRendererList(QSize srcSize,QSize dstSize);
    QVector<QVector<SurfaceBoxRenderer*>> Renderers();
    void computeScaleFactor();
    QPoint SrcToDst(QPoint);
    QPoint DstToSrc(QPoint);
    QRectF SrcToDst(QRectF);
    QRectF DstToSrc(QRectF);
    void add(SurfaceBoxRenderer*,bool);
    void remove(SurfaceBoxRenderer*);
    void invalidateLookups();
    QSize getSourceSize();
    void setSourceSize(QSize);
    QSize getDestinationSize();
    void setDestinationSize(QSize);
    void onDestinationSizeChanged();
    void onSourceSizeChanged();
    void createD2SLookupX();
    void createD2SLookupY();
    void createS2DLookupX();
    void createS2DLookupY();
    void render(Surface* dst,QPoint offset);
    void invalidate(QRect rect);
    void invalidate();
signals:
    void invalidated(QRect rect);

};
#endif // SURFACEBOXRENDERERLIST_H
