#ifndef BITMAP_H
#define BITMAP_H
#pragma once
#include "Bgra.h"
#include <cstring>
#include <QRect>
typedef unsigned char byte;
typedef unsigned int uint;

struct memoryBlock{
    memoryBlock* parent=nullptr;
    byte* ptr=nullptr;
    long length;
    int width;
    int height;
    bool valid=false;
    memoryBlock(){
    }
    memoryBlock(long bytes){
        this->length=bytes;
        this->parent=nullptr;
        this->ptr=(byte*)malloc(bytes);
        memset(this->ptr,0,length);
        this->valid=true;
    }
    memoryBlock(int width,int height){
        this->length=width*height*sizeof(bgra);
        this->parent=nullptr;
        this->ptr=(byte*)malloc(length);
        memset(this->ptr,0,length);
        this->width=width;
        this->height=height;
        this->valid=true;
    }
    memoryBlock(memoryBlock* parent,long offset,long length){
        this->parent=parent;
        this->ptr=(parent->ptr);
        this->ptr+=offset;
        this->length=length;
        this->valid=true;
    }
    memoryBlock(byte* data, long length){
        this->length=length;
        this->ptr=data;
        this->valid=true;
    }
    byte& operator[](long index){
        return ((ptr)[index]);
        //return &(((byte*)ptr)[index]);
    }
    ~memoryBlock(){
        if (this->parent==nullptr)
            free(ptr);
    }

};


class Bitmap{
protected:
    memoryBlock* block;
public:
  long bytes=0;
  int stride=0;
  QRect bounds;
  int width=0,height=0;
  const memoryBlock* getData(){
      return block;
  }
    Bitmap(int width,int height){
        if(width*sizeof(bgra)>INT32_MAX ||
           height*stride>LONG_MAX)
            throw "Dimensions are too large - not enough memory";
        this->stride=width*sizeof (bgra);
        this->bytes=height*stride;
        this->block=new memoryBlock(width,height);
        this->width=width;
        this->height=height;
        this->bounds={0,0,width,height};
    }
    Bitmap(byte *data,int width,int height){
        this->block=new memoryBlock(data,width*height*sizeof(bgra));
        this->stride=width*sizeof (bgra);
        this->width=width;
        this->height=height;
        this->bounds={0,0,width,height};
    }
    Bitmap(int width,int height,int stride,memoryBlock *scan){
        this->width=width;
        this->height=height;
        this->stride=stride;
        this->block=scan;
       // this->bytes=height*stride;
        this->bounds={0,0,width,height};
    }
    ~Bitmap(){
        delete block;
    }
    Bitmap* createSubBitmap(int x,int y,int w,int h){
        QRect sub(x,y,w,h);
        QRect clip=this->bounds.intersected(sub);
        if(clip!=sub){
            throw "bounds parameters must be a subset of this Surface's bounds";
        }

        long offset=this->getPointByteOffset(x,y);
        long length=(h-1)*stride+(w*sizeof(bgra));
        memoryBlock* bloc=new memoryBlock(this->block,offset,length);
        return new Bitmap(w,h,this->stride,bloc);
    }
    Bitmap* createSubBitmap(QRect rect){
        return createSubBitmap(rect.x(),rect.y(),rect.width(),rect.height());
    }
    void copyBitmap(Bitmap* source){
        int cw=std::min(width,source->width);
        int ch=std::min(height,source->height);
        if (this->stride==source->stride && this->width*sizeof(bgra)==this->stride
            && this->width == source->width && this->height == source->height)
            memcpy(this->block->ptr,source->block->ptr,((height-1)*stride)+(width*sizeof(bgra)));
        else
            for(int y=0;y<ch;++y)
                memcpy(getRowAdress(y),source->getRowAdress(y),cw*sizeof(bgra));
    }
    void copyBitmap(Bitmap* source,QPoint dst){
        QRect dstRect(dst,QSize(source->width,source->height));
        dstRect=dstRect.intersected(this->bounds);
        if(dstRect.width()==0||dstRect.height()==0) return;
        QPoint srcPoint(dstRect.x()-dst.x(),dstRect.y()-dst.y());
        QRect srcRect(srcPoint,dstRect.size());
        Bitmap* srcWin=source->createSubBitmap(srcRect);
        Bitmap* dstWin=this->createSubBitmap(dstRect);
        dstWin->copyBitmap(srcWin);
        delete srcWin;
        delete dstWin;
    }
    void copyBitmap(Bitmap* source,QRect rect){
        rect = rect.intersected(source->bounds);
        int cw=std::min(width,rect.width());
        int ch=std::min(height,rect.height());
        if(cw==0||ch==0) return;
        Bitmap* srcWin=source->createSubBitmap(rect);
        this->copyBitmap(srcWin);
        delete srcWin;
    }
    void copyBitmap(Bitmap* source,QRect* region){
        QRect* i=region;
        while (i!=nullptr) {
            QRect intr=i->intersected(this->bounds);
            intr=intr.intersected(source->bounds);
            if(intr.width()==0||intr.height()==0) continue;
            for(int y=intr.top();y<intr.bottom();++y){
                bgra *dst=this->getPointAdress(intr.left(),y);
                bgra *src=source->getPointAdress(intr.left(),y);
                memcpy(dst,src,intr.width()*sizeof(bgra));
            }
            i++;
        }
    }
    void copyBitmap(Bitmap* source,QRect* region,int startindex,int length){
        for(int i=startindex;i<startindex+length;++i){
            QRect intr=region[i];
            intr=intr.intersected(this->bounds);
            intr=intr.intersected(source->bounds);
            if(intr.width()==0||intr.height()==0) continue;
            for(int y=intr.top();y<intr.bottom();++y){
                bgra *dst=this->getPointAdress(intr.left(),y);
                bgra *src=source->getPointAdress(intr.left(),y);
                memcpy(dst,src,intr.width()*sizeof(bgra));
            }
        }
    }
    long getRowByteOffset(int y){
        return y*stride;
    }
    bgra* getRowAdress(int y){
        return (bgra*)(block->ptr+getRowByteOffset(y));
    }
    long getColumnByteOffset(int x){
        return x*sizeof(bgra);
    }
    long getPointByteOffset(QPoint p){
        return getPointByteOffset(p.x(),p.y());
    }
    long getPointByteOffset(int x,int y){
        return getRowByteOffset(y)+getColumnByteOffset(x);
    }
    bgra getPoint(QPoint p){
        return getPoint(p.x(),p.y());
    }
    bgra getPoint(int x,int y){
        return *(x+(bgra*)(this->block->ptr+(y*stride)));
    }
    bgra* getPointAdress(QPoint p){
        return getPointAdress(p.x(),p.y());
    }
    bgra* getPointAdress(int x,int y){
        return (x+(bgra*)(this->block->ptr+(y*stride)));
    }
    memoryBlock getRow(int y){
        return memoryBlock(this->block,getRowByteOffset(y),this->width*sizeof(bgra));
    }
    bool isVisible(QPoint p){
        return isVisible(p.x(),p.y());
    }
    bool isVisible(int x,int y){
        return x>=0&&x<width&&y>=0&&y<height;
    }
    bool isRowVisible(int y){
        return y>=0&&y<height;
    }
    bool isColumnVisible(int x){
        return x>=0&&x<width;
    }
};

#endif // BITMAP_H
