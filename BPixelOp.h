#ifndef BPIXELOP_H
#define BPIXELOP_H
#pragma once
#include "PixelOp.h"
class BPixelOp:public PixelOp{
protected:
    BPixelOp(){}
public:
    virtual bgra apply(bgra lhs,bgra rhs){

    }
    virtual void apply(bgra *dst,bgra *lhs,bgra *rhs,int length){
        while(length>0){
            *dst=apply(*lhs,*rhs);
            ++dst;
            ++rhs;
            --length;
        }
    }
    void apply(Bitmap* dst, QPoint dstoff,
               Bitmap* lhs,QPoint lhsoff,
               Bitmap* rhs,QPoint rhsoff,
               QSize roisize){
        int w=roisize.width();
        int h=roisize.height();
        for(int row=0;row<h;++row){
            bgra *dstptr=dst->getPointAdress(dstoff.x(),dstoff.y()+row);
            bgra *lhsptr=lhs->getPointAdress(lhsoff.x(),lhsoff.y()+row);
            bgra *rhsptr=dst->getPointAdress(rhsoff.x(),rhsoff.y()+row);
            apply(dstptr,lhsptr,rhsptr,w);
        }
    }
    void apply(bgra *dst, bgra *src, int length) override{
        while(length>0){
            *dst=apply(*dst,*src);
            ++dst;
            ++src;
            --length;
        }
    }
    void apply(Bitmap *dst, QPoint dstoff, Bitmap *src, QPoint srcoff, int scanlength) override{
        apply(dst->getPointAdress(dstoff),
              src->getPointAdress(srcoff),
              scanlength);
    }
    void apply(Bitmap* dst,Bitmap* src){
        if(dst->width!=src->width || dst->height!=src->height) return;
        for(int y=0;y<dst->height;++y){
            bgra *dstptr=dst->getRowAdress(y);
            bgra *srcptr=src->getRowAdress(y);
            apply(dstptr,srcptr,dst->width);
        }
    }
    void apply(Bitmap*dst,Bitmap*lhs,Bitmap*rhs){
        if(dst->width!=lhs->width || dst->height!=lhs->height) return;
        if(dst->width!=rhs->width || dst->height!=rhs->height) return;
        for(int y=0;y<dst->height;++y){
            bgra *dstptr=dst->getRowAdress(y);
            bgra *lhsptr=lhs->getRowAdress(y);
            bgra *rhsptr=rhs->getRowAdress(y);
            apply(dstptr,lhsptr,rhsptr,dst->width);
        }
    }
};

#endif // BPIXELOP_H
