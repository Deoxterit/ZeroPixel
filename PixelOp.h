#ifndef PIXELOP_H
#define PIXELOP_H
#pragma once
#include "Bitmap.h"
#include <QRect>
#include "Bgra.h"
#include <math.h>
class PixelOp{
public:
    static double log(double a,double b){

        return std::log(b)/std::log(a);
    }
    static byte computeAlpha(byte la,byte ra){
        return ((la*(256-(ra+(ra>>7))))>>8)+ra;
    }
    void apply(Bitmap* dst,Bitmap*src,QRect* rects ,int start,int length){
        for (int i = start; i < start + length; ++i){
            applyBase(dst, QPoint(rects[i].x(),rects[i].y()), src, QPoint(rects[i].x(),rects[i].y()), rects[i].size());
        }
    }
    void apply(Bitmap *dst,QPoint dstoff,Bitmap *src,QPoint srcoff,QSize roisize){
        applyBase(dst,dstoff,src,srcoff,roisize);
    }
    void applyBase(Bitmap *dst,QPoint dstoff,Bitmap *src,QPoint srcoff,QSize roisize){
        QRect dstrect(dstoff,roisize);
        QRect srcrect(srcoff,roisize);
        QRect dstclip=dstrect.intersected(dst->bounds);
        QRect srcclip=srcrect.intersected(src->bounds);
        int w=roisize.width();
        int h=roisize.height();
        if(dstclip!=dstrect || srcclip!=srcrect) return;
        for(int row=0;row<h;++row){
            bgra *dstp=dst->getPointAdress(dstoff.x(),dstoff.y()+row);
            bgra *srcp=src->getPointAdress(srcoff.x(),srcoff.y()+row);
            apply(dstp,srcp,w);
        }

    }
    virtual void apply(Bitmap *dst,QPoint dstoff,Bitmap*src,QPoint srcoff,int scanlength){
        apply(dst->getPointAdress(dstoff),src->getPointAdress(srcoff),scanlength);
    }
    virtual void apply(bgra* dst,bgra* src,int length){
        throw "Not Implemented";
    }
    PixelOp(){}
};
#endif // PIXELOP_H
