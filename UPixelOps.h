#ifndef UPIXELOPS_H
#define UPIXELOPS_H
#pragma once
#include "UPixelOp.h"
class UPO_Identity:public UPixelOp{
public:
    using UPixelOp::apply;
    bgra apply(bgra c)override{

        return c;
    }
    void apply(bgra *dst, bgra *src, int length) override{
        memcpy(dst,src,length*sizeof(bgra));
    }
    void apply(bgra*, int) override{
        return;
    }
};

class UPO_Constant:public UPixelOp{
private:
    bgra setColor;
public:
    using UPixelOp::apply;
    UPO_Constant(bgra color):setColor(color){

    }
    bgra apply(bgra)override{
        return setColor;
    }
    void apply(bgra *dst, bgra *src, int length) override{
        while(length>0){
            *dst=setColor;
            ++dst;
            --length;
        }
    }
    void apply(bgra *ptr, int length) override{
        while(length>0){
            *ptr=setColor;
            ++ptr;
            --length;
        }
    }
};
class UPO_BlendConstant:public UPixelOp{
private:
    bgra blendColor;
public:
    using UPixelOp::apply;
    UPO_BlendConstant(bgra c):blendColor(c){

    }

    bgra apply(bgra c) override{
        int a=blendColor.A;
        int inva=255-a;
        int r=((c.R*inva)+(blendColor.R*a))/256;
        int g=((c.G*inva)+(blendColor.G*a))/256;
        int b=((c.B*inva)+(blendColor.B*a))/256;
        byte a2=computeAlpha(c.A,blendColor.A);
        return bgra(r,g,b,a);
    }
};
class UPO_Invert:public UPixelOp{
public:
    using UPixelOp::apply;
    bgra apply(bgra c)override{
        return bgra(255-c.R,255-c.G,255-c.B,c.A);
    }
};
class UPO_SetChannel:public UPixelOp{
private:
    byte schannel;
    byte svalue;
public:
    using UPixelOp::apply;
    UPO_SetChannel(byte channel,byte value):schannel(channel),svalue(value){}

    bgra apply(bgra c)override{
        c[schannel]=svalue;
        return c;
    }
    void apply(bgra *dst, bgra *src, int length) override{
        while(length>0){
            *dst=*src;
            (*dst)[schannel]=svalue;
            ++dst;
            ++src;
            --length;
        }
    }
    void apply(bgra *ptr, int length) override{
        while(length>0){
            (*ptr)[schannel]=svalue;
            ++ptr;
            --length;
        }
    }
};
class UPO_AverageChannels:public UPixelOp{
public:
    using UPixelOp::apply;
    bgra apply(bgra c) override{
        byte a = (c.R+c.G+c.B)/3;
        return bgra(a,a,a,c.A);
    }
};

class UPO_Desaturate:public UPixelOp{
public:
    using UPixelOp::apply;
    bgra apply(bgra c) override{
        byte a = c.getIntensity();
        return bgra(a,a,a,c.A);
    }
    void apply(bgra *ptr, int length) override{
        while(length>0){
            byte i = ptr->getIntensity();
            ptr->R=i;
            ptr->G=i;
            ptr->B=i;
            ++ptr;
            --length;
        }
    }
    void apply(bgra *dst, bgra *src, int length) override{
        while(length>0){
            byte i = src->getIntensity();
            dst->R=i;
            dst->G=i;
            dst->B=i;
            dst->A=src->A;
            ++dst;
            ++src;
            --length;
        }
    }
};
class UPO_LuminocityCurve:public UPixelOp{
public:
     using UPixelOp::apply;
    byte *curve=new byte[256];
    UPO_LuminocityCurve(){
        for(int i=0;i<256;i++){
            curve[i]=i;
        }
    }
    bgra apply(bgra c) override{
        byte lum=c.getIntensity();
        int diff=curve[lum]-lum;
        return bgra((byte)c.R+diff,(byte)c.G+diff,(byte)c.B+diff,c.A);
    }
};

class UPO_ChannelCurve:public UPixelOp{
public:
     using UPixelOp::apply;
    byte *curveR=new byte[256];
    byte *curveG=new byte[256];
    byte *curveB=new byte[256];
    UPO_ChannelCurve(){
        for(int i=0;i<256;i++){
            curveR[i]=i;
            curveG[i]=i;
            curveB[i]=i;
        }
    }
    void apply(bgra *dst, bgra *src, int length) override{
        while (--length>=0) {
            dst->R=curveR[src->R];
            dst->G=curveG[src->G];
            dst->B=curveB[src->B];
            dst->A=src->A;
            ++dst;
            ++src;
        }
    }
    void apply(bgra *ptr, int length) override{
        while (--length>=0) {
            ptr->R=curveR[ptr->R];
            ptr->G=curveR[ptr->G];
            ptr->B=curveR[ptr->B];
            ++ptr;
        }
    }
    bgra apply(bgra c) override{
        return bgra(curveR[c.R],curveG[c.G],curveB[c.B],c.A);
    }
    void apply(Bitmap *dst, QPoint dstoff, Bitmap *src, QPoint srcoff, int scanlength) override{
        apply(dst,dstoff,src,srcoff,scanlength);
    }
};

class UPO_Level:UPO_ChannelCurve{
private:
    bgra colorInLow;
    bgra colorInHigh;
    bgra colorOutLow;
    bgra colorOutHigh;
    bool isValid=true;
    float *gamma=new float[3];
public:
    using UPixelOp::apply;
    UPO_Level(bgra inlo,bgra inhi,float* gamma,bgra outlo,bgra outhi){
        colorInLow=inlo;
        colorInHigh=inhi;
        colorOutLow=outlo;
        colorOutHigh=outhi;
        this->gamma=gamma;
        updateLookupTable();
    }
    bgra apply(byte r, byte g,byte b){
        bgra rt(0,0,0,255);
        byte in[]{r,g,b};
        for (int i = 0; i < 3; i++)
        {
            auto v=in[i]-colorInLow[i];
           if (v < 0)
           {
               rt[i] = colorOutLow[i];
           }
           else if (v + colorInLow[i] >= colorInHigh[i])
           {
               rt[i] = colorOutHigh[i];
           }
           else
           {
               rt[i] = (byte)(
                   colorOutLow[i] + (colorOutHigh[i] - colorOutLow[i]) * pow(v / (colorInHigh[i] - colorInLow[i]), gamma[i]),
                   0.0f,
                   255.0f);
           }
        }
        return rt;
    }
    void updateLookupTable(){
       for (int i = 0; i < 3; i++)
       {
           if (colorOutHigh[i] < colorOutLow[i] ||
               colorInHigh[i] <= colorInLow[i] ||
               gamma[i] < 0)
           {
               isValid = false;
               return;
           }

           for (int j = 0; j < 256; j++)
           {
               bgra col = apply(j, j, j);
               curveB[j] = col.B;
               curveG[j] = col.G;
               curveR[j] = col.R;
           }
       }
    }
    static UPO_Level autoFromLoMdHi(bgra  lo, bgra  md, bgra hi) {
        float *gamm=new float[3];
        for (int i = 0; i < 3; i++)
        {
            if (lo[i] < md[i] && md[i] < hi[i])
            {
                gamm[i]=(float)(log(0.5,(float)(md[i]-lo[i]) /(float)(hi[i]-lo[i])));
            }
            else
                gamm[i]=1.0f;
        }
        return UPO_Level(lo,hi,gamm,bgra(0,0,0),bgra(255,255,255));
    }
    float getGamma(int index){
        if(index<0||index>2) return -1.0;
        return gamma[index];
    }
    void setGamma(int index,float val){
        if(index<0||index>2) return ;
        if(val<0.1) val=0.1;
        if(val>10.0) val=10.0;
        gamma[index]=val;
        updateLookupTable();
    }
    void setColorInHigh(bgra c){
        if(c.R==0) c.R=1;
        if(c.G==0) c.G=1;
        if(c.B==0) c.B=1;
        if(colorInLow.R>c.R-1) colorInLow.R=c.R-1;
        if(colorInLow.G>c.G-1) colorInLow.G=c.R-1;
        if(colorInLow.B>c.B-1) colorInLow.B=c.R-1;
        colorInHigh=c;
        updateLookupTable();
    }
    void setColorInLow(bgra c){
        if(c.R==255) c.R=254;
        if(c.G==255) c.G=254;
        if(c.B==255) c.B=254;
        if(colorInHigh.R<c.R+1) colorInHigh.R=c.R+1;
        if(colorInHigh.G<c.G+1) colorInHigh.G=c.R+1;
        if(colorInHigh.B<c.B+1) colorInHigh.B=c.R+1;
        colorInLow=c;
        updateLookupTable();
    }
    void setColorOutLow(bgra c){
        if(c.R==255) c.R=254;
        if(c.G==255) c.G=254;
        if(c.B==255) c.B=254;
        if(colorOutHigh.R<c.R+1) colorOutHigh.R=c.R+1;
        if(colorOutHigh.G<c.G+1) colorOutHigh.G=c.G+1;
        if(colorOutHigh.B<c.B+1) colorOutHigh.B=c.B+1;
        colorOutLow=c;
        updateLookupTable();
    }
    void setColorOutHigh(bgra c){
        if(c.R==0) c.R=1;
        if(c.G==0) c.G=1;
        if(c.B==0) c.B=1;
        if(colorOutLow.R>c.R-1) colorOutLow.R=c.R-1;
        if(colorOutLow.G>c.G-1) colorOutLow.G=c.G-1;
        if(colorOutLow.B>c.B-1) colorOutLow.B=c.B-1;
        colorOutHigh=c;
        updateLookupTable();
    }
};

#endif // UPIXELOPS_H
