#ifndef SURFACEBOXBASERENDERER_H
#define SURFACEBOXBASERENDERER_H
#include "SurfaceBoxRenderer.h"
#include "SurfaceBoxRendererList.h"
#include "Surface.h"
class SurfaceBoxBaseRenderer:public SurfaceBoxRenderer{
public:
    Surface* source;
    SurfaceBoxBaseRenderer(SurfaceBoxRendererList* parent,Surface* src):SurfaceBoxRenderer(parent){
        this->source=src;
    }
    void onVisibleChanged() override{
        invalidate();
    }
    void onDestinationSizeChanged() override{
        this->parent->invalidateLookups();

    }
    void onSourceSizeChanged() override{
         this->parent->invalidateLookups();
    }
    static void renderOneToOne(Surface *dst,Surface* src,QPoint offset){
        QRect srcR(offset,QSize(dst->width,dst->height));
        srcR=srcR.intersected(src->bounds);
        for(int dstRow=0;dstRow<srcR.height();++dstRow){
            bgra *dstrowptr=dst->getRowAdress(dstRow);
            bgra *srcrowptr=src->getPointAdress(offset.x(),dstRow+offset.y());
            int dstcol=offset.x();
            int dstcolend=offset.x()+srcR.width();
            int checkery=dstRow+offset.y();
            while(dstcol<dstcolend){
                int r=srcrowptr->R;
                int g=srcrowptr->G;
                int b=srcrowptr->B;
                int a=srcrowptr->A;
                int v=(((dstcol^checkery)&8)<<3)+191;
                a=a+(a>>7);
                int vmia=v*(256-a);
                r=((r*a)+vmia)>>8;
                g=((g*a)+vmia)>>8;
                b=((b*a)+vmia)>>8;
                dstrowptr->R=r;
                dstrowptr->G=g;
                dstrowptr->B=b;
                dstrowptr->A=255;
               // dstrowptr->color=b+(g<<8)+(r<<16)+(255<<24);
                ++dstrowptr;
                ++srcrowptr;
                ++dstcol;
            }
        }
    }
    void renderOneToOne(Surface *dst,QPoint offset){
        renderOneToOne(dst,this->source,offset);
    }
    void renderZoomInNearestNeighbor(Surface* dst,QPoint offset){
        parent->createD2SLookupX();
        auto d2sLookupX=parent->d2sLookupX;
        parent->createD2SLookupY();
        auto d2sLookupY=parent->d2sLookupY;
        for(int dstrow=0;dstrow<dst->height;++dstrow){
            int nny=dstrow+offset.y();
            int srcy=d2sLookupY[nny];
            bgra *dstptr=dst->getRowAdress(dstrow);
            bgra *srcrow=this->source->getRowAdress(srcy);
            for(int dstcol=0;dstcol<dst->width;++dstcol){
                int nnx=dstcol+offset.x();
                int srcx=d2sLookupX[nnx];
                bgra src=*(srcrow+srcx);
                int r=src.R;
                int g=src.G;
                int b=src.B;
                int a=src.A;
                int v=(((dstcol+offset.x())^(dstrow+offset.y()))&8)*8+191;
                a=a+(a>>7);
                int vmia=v*(256-a);
                r=((r*a)+vmia)>>8;
                g=((g*a)+vmia)>>8;
                b=((b*a)+vmia)>>8;
                dstptr->R=r;
                dstptr->G=g;
                dstptr->B=b;
                dstptr->A=255;
               // dstptr->color=b+(g<<8)+(r<<16)+(255<<24);
                ++dstptr;
            }
        }
    }
    void renderZoomOutGridMiltisampling(Surface* dst,Surface* src,QPoint offset,QSize dstSize){
        const int fpShift=12;
        const int fpFactor=(1<<fpShift);
        QSize srcSize(src->width,src->height);
        long fdstleftlong=(offset.x()*fpFactor*srcSize.width())/dstSize.width();
        long fdsttoplong=(offset.y()*fpFactor*srcSize.height())/dstSize.height();
        long fdstrightlong=((offset.x()+dst->width)*fpFactor*srcSize.width())/dstSize.width();
        long fdstbottomlong=((offset.y()+dst->height)*fpFactor*srcSize.height())/dstSize.height();
        int dx=(fdstrightlong-fdstleftlong)/dst->width;
        int dy=(fdstbottomlong-fdsttoplong)/dst->height;
        for(int dstrow=0,fdsty=fdsttoplong; dstrow<dst->height&&fdsty<fdstbottomlong;++dstrow,fdsty+=dy){
            int srcy1=fdsty>>fpShift;
            int srcy2=(fdsty+(dy>>2))>>fpShift;
            int srcy3=(fdsty+(dy>>1))>>fpShift;
            int srcy4=(fdsty+(dy>>1)+(dy>>2))>>fpShift;
            bgra* src1=src->getRowAdress(srcy1);
            bgra* src2=src->getRowAdress(srcy2);
            bgra* src3=src->getRowAdress(srcy3);
            bgra* src4=src->getRowAdress(srcy4);
            bgra* dstptr=dst->getRowAdress(dstrow);
            int checkery=dstrow+offset.y();
            int checkerx=offset.x();
            int maxcheckerx=checkerx+dst->width;
            for(int fdstx=fdstleftlong;checkerx<maxcheckerx&&fdstx<fdstrightlong;++checkerx,fdstx+=dx){
                int srcx1 = (fdstx + (dx >> 2)) >> fpShift;
                int srcx2 = (fdstx + (dx >> 1) + (dx >> 2)) >> fpShift;
                int srcx3 = fdstx >> fpShift;
                int srcx4 = (fdstx + (dx >> 1)) >> fpShift;
                bgra* p1 = src1 + srcx1;
                bgra* p2 = src2 + srcx2;
                bgra* p3 = src3 + srcx3;
                bgra* p4 = src4 + srcx4;
                int r=(2 + p1->R + p2->R + p3->R + p4->R) >> 2;
                int g = (2 + p1->G + p2->G + p3->G + p4->G) >> 2;
                int b = (2 + p1->B + p2->B + p3->B + p4->B) >> 2;
                int a = (2 + p1->A + p2->A + p3->A + p4->A) >> 2;
                int v = ((checkerx ^ checkery) & 8) * 8 + 191;
                a = a + (a >> 7);
                int vmia = v * (256 - a);
                r = ((r * a) + vmia) >> 8;
                g = ((g * a) + vmia) >> 8;
                b = ((b * a) + vmia) >> 8;
                dstptr->R=r;
                dstptr->G=g;
                dstptr->B=b;
                dstptr->A=255;
           //     dstptr->color = (uint)b + ((uint)g << 8) + ((uint)r << 16) + 0xff000000;
                ++dstptr;
            }

        }
    }
    void renderZoomOutGridMiltisampling(Surface* dst,QPoint offset){
        renderZoomOutGridMiltisampling(dst,this->source,offset,this->getDestinationSize());
    }
    void render(Surface *dst, QPoint offset) override{
        if(getSourceSize().width()>getDestinationSize().width()){
            renderZoomOutGridMiltisampling(dst,offset);
        }
        else if(getSourceSize().width()==getDestinationSize().width()){
            renderOneToOne(dst,offset);
        }
        else if(getSourceSize().width()<getDestinationSize().width()){
            renderZoomInNearestNeighbor(dst,offset);
        }
    }

};

#endif // SURFACEBOXBASERENDERER_H
