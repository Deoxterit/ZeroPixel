#ifndef UPIXELOP_H
#define UPIXELOP_H
#pragma once
#include "PixelOp.h"
class UPixelOp:public PixelOp{
protected:
    UPixelOp(){}
public:

    void apply(Bitmap *dst, QPoint dstoff, Bitmap *src, QPoint srcoff, int scanlength) override{
    }
    virtual bgra apply(bgra c){
        return bgra(0,0,0,0);
    }
    void apply(bgra*dst,bgra*src,int length) override{
        while(length>0){
            *dst=this->apply(*src);
            ++dst;
            ++src;
            --length;
        }
    }
    virtual void apply(bgra*ptr,int length){
        while(length>0){
            *ptr=this->apply(*ptr);
            ++ptr;
            --length;
        }
    }
    void apply(Bitmap* surface){
        this->applyRect(surface,surface->bounds);
    }
    void applyRect(Bitmap * surface,QRect rect){
        for(int y=rect.top();y<rect.bottom();++y){
            bgra*ptr=surface->getPointAdress(rect.left(),y);
            this->apply(ptr,rect.width());
        }
    }
    void apply(Bitmap*surface,QRect* rects,int start,int length){
        QRect regionBounds=rects[start];
        for(int i=1;i<length;i++){
            regionBounds=regionBounds.intersected(rects[i]);
        }
        if(regionBounds!=surface->bounds.intersected(regionBounds)) return;
        for(int x=start;x<start+length;++x){
            this->applyRect(surface,rects[x]);
        }
    }
    void apply(Bitmap * surface,QRect rect){
        this->applyRect(surface,rect);
    }
};
#endif // UPIXELOP_H
