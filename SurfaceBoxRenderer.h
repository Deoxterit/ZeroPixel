#ifndef SURFACEBOXRENDERER_H
#define SURFACEBOXRENDERER_H
#include <QtCore>
#include "ScaleFactor.h"
#include "Surface.h"
template <class T>
T Clamp(T a, T min, T max) {
    if (a < min) return min;
    if (a > max) return max;
    return a;
}
class SurfaceBoxRendererList;
class SurfaceBoxRenderer:public QObject{
    Q_OBJECT
protected:
    bool visible;
    SurfaceBoxRendererList* parent;
public:
    static const int minXCoord=-131072;
    static const int minYCoord=-131072;
    static const int maxXCoord= 131072;
    static const int maxYCoord= 131072;
public:
    SurfaceBoxRenderer(SurfaceBoxRendererList*);
    virtual void onSourceSizeChanged(){}
    virtual void onDestinationSizeChanged(){}
    virtual void onVisibleChanging(){}
    virtual void onVisibleChanged(){}
    virtual void render(Surface* dst,QPoint offset){}
    virtual void onInvalidate(QRect);
    void invalidate();
    void invalidate(QRect);
    void invalidate(QRectF);
    static QRect maxBounds();
    bool getVisible();
    void setVisible(bool);
    QSize getSourceSize();
    QSize getDestinationSize();
    bool operator==(SurfaceBoxRenderer rhs){
        if(parent==rhs.parent && visible==rhs.visible)
            return true;
        return false;
    }
};


#endif // SURFACEBOXRENDERER_H
