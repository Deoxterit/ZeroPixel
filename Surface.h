#ifndef SURFACE_H
#define SURFACE_H
#pragma once
#include "Bitmap.h"
#include "UPixelOps.h"

#include <QDebug>
class Surface:public Bitmap{
public:
    Surface(int width,int height):Bitmap(width,height){}
    Surface(int width,int height,int stride,memoryBlock *scan):Bitmap(width,height,stride,scan){}
    Surface(byte *data,int width,int height):Bitmap(data,width,height){}
    void clear(bgra color){
        UPO_Constant p(color);
        p.apply(this);
    }
    void clearCheckboard(){
        for(int y=0;y<this->height;++y){
            bgra* ptr=getRowAdress(y);
            for(int x=0;x<this->width;++x){
                byte v=(((x^y)&8)*8+191);
                *ptr=bgra(v,v,v,255);
                ++ptr;
            }

        }
    }


    void superSamplingFitSurface(Surface* source,QRect dst){
        if(source->width==width&&source->height==height){
            copyBitmap(source);
        }
        else if(source->width<=width||source->height<=height){
                if(source->width<2||source->height<2||this->width<2||this->height<2)
                    {nearestNeighborFitSurface(source,dst);}
                else
                    {bicubicFitSurface(source,dst);}
        }
        else
        {
            QRect dstRoi2 = dst.intersected(this->bounds);

            for (int dstY = dstRoi2.top(); dstY < dstRoi2.bottom(); ++dstY)
            {
                double srcTop = (double)(dstY * source->height) / (double)height;
                double srcTopFloor = floor(srcTop);
                double srcTopWeight = 1 - (srcTop - srcTopFloor);
                int srcTopInt = (int)srcTopFloor;

                double srcBottom = (double)((dstY + 1) * source->height) / (double)height;
                double srcBottomFloor = floor(srcBottom - 0.00001);
                double srcBottomWeight = srcBottom - srcBottomFloor;
                int srcBottomInt = (int)srcBottomFloor;

                bgra *dstPtr = this->getPointAdress(dstRoi2.left(), dstY);

                for (int dstX = dstRoi2.left(); dstX < dstRoi2.right(); ++dstX)
                {
                    double srcLeft = (double)(dstX * source->width) / (double)width;
                    double srcLeftFloor = floor(srcLeft);
                    double srcLeftWeight = 1 - (srcLeft - srcLeftFloor);
                    int srcLeftInt = (int)srcLeftFloor;
                    double srcRight = (double)((dstX + 1) * source->width) / (double)width;
                    double srcRightFloor = floor(srcRight - 0.00001);
                    double srcRightWeight = srcRight - srcRightFloor;
                    int srcRightInt = (int)srcRightFloor;
                    double blueSum = 0;
                    double greenSum = 0;
                    double redSum = 0;
                    double alphaSum = 0;
                    bgra *srcLeftPtr = source->getPointAdress(srcLeftInt, srcTopInt + 1);

                    for (int srcY = srcTopInt + 1; srcY < srcBottomInt; ++srcY)
                    {
                        double a = srcLeftPtr->A;
                        blueSum += srcLeftPtr->B * srcLeftWeight * a;
                        greenSum += srcLeftPtr->G * srcLeftWeight * a;
                        redSum += srcLeftPtr->R * srcLeftWeight * a;
                        alphaSum += srcLeftPtr->A * srcLeftWeight;
                        srcLeftPtr = (bgra*)((byte*)srcLeftPtr + source->stride);
                    }
                    bgra *srcRightPtr = source->getPointAdress(srcRightInt, srcTopInt + 1);
                    for (int srcY = srcTopInt + 1; srcY < srcBottomInt; ++srcY)
                    {
                        double a = srcRightPtr->A;
                        blueSum += srcRightPtr->B * srcRightWeight * a;
                        greenSum += srcRightPtr->G * srcRightWeight * a;
                        redSum += srcRightPtr->R * srcRightWeight * a;
                        alphaSum += srcRightPtr->A * srcRightWeight;
                        srcRightPtr = (bgra*)((byte*)srcRightPtr + source->stride);
                    }
                    bgra *srcTopPtr = source->getPointAdress(srcLeftInt + 1, srcTopInt);
                    for (int srcX = srcLeftInt + 1; srcX < srcRightInt; ++srcX)
                    {
                        double a = srcTopPtr->A;
                        blueSum += srcTopPtr->B * srcTopWeight * a;
                        greenSum += srcTopPtr->G * srcTopWeight * a;
                        redSum += srcTopPtr->R * srcTopWeight * a;
                        alphaSum += srcTopPtr->A * srcTopWeight;
                        ++srcTopPtr;
                    }
                    bgra *srcBottomPtr = source->getPointAdress(srcLeftInt + 1, srcBottomInt);
                    for (int srcX = srcLeftInt + 1; srcX < srcRightInt; ++srcX)
                    {
                        double a = srcBottomPtr->A;
                        blueSum += srcBottomPtr->B * srcBottomWeight * a;
                        greenSum += srcBottomPtr->G * srcBottomWeight * a;
                        redSum += srcBottomPtr->R * srcBottomWeight * a;
                        alphaSum += srcBottomPtr->A * srcBottomWeight;
                        ++srcBottomPtr;
                    }

                    for (int srcY = srcTopInt + 1; srcY < srcBottomInt; ++srcY)
                    {
                        bgra *srcPtr = source->getPointAdress(srcLeftInt + 1, srcY);

                        for (int srcX = srcLeftInt + 1; srcX < srcRightInt; ++srcX)
                        {
                            double a = srcPtr->A;
                            blueSum += (double)srcPtr->B * a;
                            greenSum += (double)srcPtr->G * a;
                            redSum += (double)srcPtr->R * a;
                            alphaSum += (double)srcPtr->A;
                            ++srcPtr;
                        }
                    }

                    bgra srcTL = source->getPoint(srcLeftInt, srcTopInt);
                    double srcTLA = srcTL.A;
                    blueSum += srcTL.B * (srcTopWeight * srcLeftWeight) * srcTLA;
                    greenSum += srcTL.G * (srcTopWeight * srcLeftWeight) * srcTLA;
                    redSum += srcTL.R * (srcTopWeight * srcLeftWeight) * srcTLA;
                    alphaSum += srcTL.A * (srcTopWeight * srcLeftWeight);

                    bgra srcTR = source->getPoint(srcRightInt, srcTopInt);
                    double srcTRA = srcTR.A;
                    blueSum += srcTR.B * (srcTopWeight * srcRightWeight) * srcTRA;
                    greenSum += srcTR.G * (srcTopWeight * srcRightWeight) * srcTRA;
                    redSum += srcTR.R * (srcTopWeight * srcRightWeight) * srcTRA;
                    alphaSum += srcTR.A * (srcTopWeight * srcRightWeight);

                    bgra srcBL = source->getPoint(srcLeftInt, srcBottomInt);
                    double srcBLA = srcBL.A;
                    blueSum += srcBL.B * (srcBottomWeight * srcLeftWeight) * srcBLA;
                    greenSum += srcBL.G * (srcBottomWeight * srcLeftWeight) * srcBLA;
                    redSum += srcBL.R * (srcBottomWeight * srcLeftWeight) * srcBLA;
                    alphaSum += srcBL.A * (srcBottomWeight * srcLeftWeight);

                    bgra srcBR = source->getPoint(srcRightInt, srcBottomInt);
                    double srcBRA = srcBR.A;
                    blueSum += srcBR.B * (srcBottomWeight * srcRightWeight) * srcBRA;
                    greenSum += srcBR.G * (srcBottomWeight * srcRightWeight) * srcBRA;
                    redSum += srcBR.R * (srcBottomWeight * srcRightWeight) * srcBRA;
                    alphaSum += srcBR.A * (srcBottomWeight * srcRightWeight);

                    double area = (srcRight - srcLeft) * (srcBottom - srcTop);

                    double alpha = alphaSum / area;
                    double blue;
                    double green;
                    double red;

                    if (alpha == 0)
                    {
                        blue = 0;
                        green = 0;
                        red = 0;
                    }
                    else
                    {
                        blue = blueSum / alphaSum;
                        green = greenSum / alphaSum;
                        red = redSum / alphaSum;
                    }

                    blue += 0.5;
                    green += 0.5;
                    red += 0.5;
                    alpha += 0.5;
                    dstPtr->R=red;
                    dstPtr->G=green;
                    dstPtr->B=blue;
                    dstPtr->A=alpha;
                  //  dstPtr->Bgra = (uint)blue + ((uint)green << 8) + ((uint)red << 16) + ((uint)alpha << 24);
                    ++dstPtr;
                }
            }
        }
    }
    void nearestNeighborFitSurface(Surface* source){
        nearestNeighborFitSurface(source,this->bounds);
    }
    void nearestNeighborFitSurface(Surface* source,QRect dst){

        QRect roi=this->bounds.intersected(dst);
        for(int dsy=roi.top();dsy<roi.bottom();++dsy)
        {
            int srcY=(dsy*source->height)/height;
            bgra *srcRow=source->getRowAdress(srcY);
            bgra *dsPtr=this->getPointAdress(roi.left(),dsy);
            for(int dsx=roi.left();dsx<roi.right();++dsx){
                int srcX=(dsx*source->width)/width;
                *dsPtr=*(srcRow+srcX);
                ++dsPtr;
            }
        }
    }
    void bicubicFitSurface(Surface* src,QRect dst){
        float leftF = (1 * (float)(width - 1)) / (float)(src->width - 1);
        float topF = (1 * (height - 1)) / (float)(src->height - 1);
        float rightF = ((float)(src->width - 3) * (float)(width - 1)) / (float)(src->width - 1);
        float bottomF = ((float)(src->height - 3) * (float)(height - 1)) / (float)(src->height - 1);
        int left = (int)ceil((double)leftF);
        int top = (int)ceil((double)topF);
        int right = (int)floor((double)rightF);
        int bottom = (int)floor((double)bottomF);
        QRect rois[]={
            QRect(left,top,right,bottom),
            QRect(0,0,width,top),
            QRect(0, top, left, height - top),
            QRect(right, top, width - right, height - top),
            QRect(left, bottom, right - left, height - bottom)
        };
        for(int i=0;i<5;i++){
            rois[i].intersected(dst);
            if(rois[i].width()>0&&rois[i].height()>0){
                if(i==0)
                    bicubicFitSurfaceU(src,rois[i]);
                else
                    bicubicFitSurfaceC(src,rois[i]);
            }
        }
    }
    double cubeClamped(double x){
        if(x>=0) return x*x*x;
        return 0;
    }
    double R(double x){
        return (cubeClamped(x + 2) - (4 * cubeClamped(x + 1)) + (6 * cubeClamped(x)) - (4 * cubeClamped(x - 1))) / 6;
    }
    void bicubicFitSurfaceC(Surface* src,QRect dst){
        QRect roi=this->bounds.intersected(dst);
        double* rColCacheIp=new double[4*roi.width()];
        double* rColCache=rColCacheIp;
        for(int dstx=roi.left();dstx<roi.right();++dstx){
            double srcColumn = (double)(dstx * (src->width - 1)) / (double)(width - 1);
            double srcColumnFloor = floor(srcColumn);
            double srcColumnFrac = srcColumn - srcColumnFloor;
            int srcColumnInt = (int)srcColumn;

            for (int m = -1; m <= 2; ++m)
            {
                int index = (m + 1) + ((dstx - roi.left()) * 4);
                double x = m - srcColumnFrac;
                rColCache[index] = R(x);
            }
        }
        double* rRowCache=new double[4];
        for (int dstY = roi.top(); dstY < roi.bottom(); ++dstY)
        {
            double srcRow = (double)(dstY * (src->height - 1)) / (double)(height - 1);
            double srcRowFloor = floor(srcRow);
            double srcRowFrac = srcRow - srcRowFloor;
            int srcRowInt = (int)srcRow;
            bgra *dstPtr = this->getPointAdress(roi.left(), dstY);
            for (int n = -1; n <= 2; ++n)
            {
                double x = srcRowFrac - n;
                rRowCache[n + 1] = R(x);
            }

            bgra* srcrowptr=src->getRowAdress(srcRowInt-1);
            double* rColCache=rColCacheIp;
            for (int dstX = roi.left(); dstX < roi.right(); dstX++)
            {
                double srcColumn = (double)(dstX * (src->width - 1)) / (double)(width - 1);
                double srcColumnFloor = floor(srcColumn);
                double srcColumnFrac = srcColumn - srcColumnFloor;
                int srcColumnInt = (int)srcColumn;

                double blueSum = 0;
                double greenSum = 0;
                double redSum = 0;
                double alphaSum = 0;
                double totalWeight = 0;
                bgra *srcptr=srcrowptr+srcColumnInt-1;
                for (int n = -1; n <= 2; ++n)
                {
                    int srcY = srcRowInt + n;
                    for (int m = -1; m <= 2; ++m)
                    {
                        if(src->isVisible(srcColumnInt+m,srcY)){
                            double w0 = rColCache[(m+1)+(4*(dst.x()-roi.left()))];
                            double w1 = rRowCache[n+1];
                            double w = w0 * w1;
                            blueSum += srcptr->B * w * srcptr->A;
                            greenSum += srcptr->G * w * srcptr->A;
                            redSum += srcptr->R * w * srcptr->A;
                            alphaSum += srcptr->A * w;

                            totalWeight += w;
                        }
                        ++srcptr;
                    }
                    srcptr = (bgra*)( (byte *)(srcptr-4) + src->stride);
                }
                double alpha = alphaSum / totalWeight;
                double blue;
                double green;
                double red;
                if (alpha == 0)
                {
                    blue = 0;
                    green = 0;
                    red = 0;
                }
                else
                {
                    blue = blueSum / alphaSum;
                    green = greenSum / alphaSum;
                    red = redSum / alphaSum;
                    alpha += 0.5;
                    blue += 0.5;
                    green += 0.5;
                    red += 0.5;
                }
                dstPtr->A=alpha;
                dstPtr->R=red;
                dstPtr->B=blue;
                dstPtr->G=green;
                //  dstPtr->color = (uint)blue + ((uint)green << 8) + ((uint)red << 16) + ((uint)alpha << 24);
                ++dstPtr;
            }
        }

    }

    void bicubicFitSurfaceU(Surface* src,QRect dst){
        QRect roi=this->bounds.intersected(dst);
        double* rColCacheIp=new double[4*roi.width()];
        double* rColCache=rColCacheIp;
        for(int dstx=roi.left();dstx<roi.right();++dstx){
           double srcColumn = (double)(dstx * (src->width - 1)) / (double)(width - 1);
           double srcColumnFloor = floor(srcColumn);
           double srcColumnFrac = srcColumn - srcColumnFloor;
           int srcColumnInt = (int)srcColumn;

           for (int m = -1; m <= 2; ++m)
           {
               int index = (m + 1) + ((dstx - roi.left()) * 4);
               double x = m - srcColumnFrac;
               rColCache[index] = R(x);
           }
        }
        double* rRowCache=new double[4];
        for (int dstY = roi.top(); dstY < roi.bottom(); ++dstY)
        {
            double srcRow = (double)(dstY * (src->height - 1)) / (double)(height - 1);
            double srcRowFloor = floor(srcRow);
            double srcRowFrac = srcRow - srcRowFloor;
            int srcRowInt = (int)srcRow;
            bgra *dstPtr = this->getPointAdress(roi.left(), dstY);
            for (int n = -1; n <= 2; ++n)
            {
                double x = srcRowFrac - n;
                rRowCache[n + 1] = R(x);
            }

            bgra* srcrowptr=src->getRowAdress(srcRowInt-1);
            double* rColCache=rColCacheIp;
            for (int dstX = roi.left(); dstX < roi.right(); dstX++)
            {
                double srcColumn = (double)(dstX * (src->width - 1)) / (double)(width - 1);
                double srcColumnFloor = floor(srcColumn);
                double srcColumnFrac = srcColumn - srcColumnFloor;
                int srcColumnInt = (int)srcColumn;

                double blueSum = 0;
                double greenSum = 0;
                double redSum = 0;
                double alphaSum = 0;
                double totalWeight = 0;
                bgra *srcptr=srcrowptr+srcColumnInt-1;
                for (int n = 0; n <= 3; ++n)
                {
                    double w0 = rColCache[0] * rRowCache[n];
                    double w1 = rColCache[1] * rRowCache[n];
                    double w2 = rColCache[2] * rRowCache[n];
                    double w3 = rColCache[3] * rRowCache[n];
                    bgra b=*srcptr;
                    double a0 = srcptr[0].A;
                    double a1 = srcptr[1].A;
                    double a2 = srcptr[2].A;
                    double a3 = srcptr[3].A;

                    alphaSum += (a0 * w0) + (a1 * w1) + (a2 * w2) + (a3 * w3);
                    totalWeight += w0 + w1 + w2 + w3;

                    blueSum += (a0 * srcptr[0].B * w0) + (a1 * srcptr[1].B * w1) + (a2 * srcptr[2].B * w2) + (a3 * srcptr[3].B * w3);
                    greenSum += (a0 * srcptr[0].G * w0) + (a1 * srcptr[1].G * w1) + (a2 * srcptr[2].G * w2) + (a3 * srcptr[3].G * w3);
                    redSum += (a0 * srcptr[0].R * w0) + (a1 * srcptr[1].R * w1) + (a2 * srcptr[2].R * w2) + (a3 * srcptr[3].R * w3);

                    srcptr = (bgra*)( (byte *)srcptr + src->stride);
                }
                double alpha = alphaSum / totalWeight;
                double blue;
                double green;
                double red;
                if (alpha == 0)
                {
                    blue = 0;
                    green = 0;
                    red = 0;
                }
                else
                {
                    blue = blueSum / alphaSum;
                    green = greenSum / alphaSum;
                    red = redSum / alphaSum;
                    alpha += 0.5;
                    blue += 0.5;
                    green += 0.5;
                    red += 0.5;
                }
                dstPtr->A=alpha;
                dstPtr->R=red;
                dstPtr->B=blue;
                dstPtr->G=green;
              //  dstPtr->color = (uint)blue + ((uint)green << 8) + ((uint)red << 16) + ((uint)alpha << 24);
                ++dstPtr;
                rColCache += 4;
            }
        }
    }
    bgra getBilinearSample(float x,float y){
        float u=x;
        float v=y;
        //(u>=0&&v>=0&&u<width&&v<height){
            int iu=floor(u);
            uint sxfrac = (uint)(256 * (u - (float)iu));
            uint sxfracinv = 256 - sxfrac;

            int iv = (int)floor(v);
            uint syfrac = (uint)(256 * (v - (float)iv));
            uint syfracinv = 256 - syfrac;

            uint wul = (uint)(sxfracinv * syfracinv);
            uint wur = (uint)(sxfrac * syfracinv);
            uint wll = (uint)(sxfracinv * syfrac);
            uint wlr = (uint)(sxfrac * syfrac);

            int sx = iu;
            if(sx<0)
                sx=(width-1)+((sx+1)%width);
            else if(sx>(width-1))
                sx=sx%width;
            int sy = iv;
            if(sy<0)
                sy=(height-1)+((sy+1)%height);
            else if(sy>(height-1))
                sy=sy%height;

            int sleft = sx;
            int sright;
            if(sleft==(width-1))
                sright=0;
            else
                sright=sleft+1;
            int stop=sy;
            int sbottom;
            if(stop==(height-1))
                sbottom=0;
            else
                sbottom=stop+1;
            bgra cul=getPoint(sleft,stop);
            bgra cur=getPoint(sright,stop);
            bgra cll=getPoint(sleft,sbottom);
            bgra clr=getPoint(sright,sbottom);
            return bgra::mix4w16(cul,wul,cur,wur,cll,wll,clr,wlr);
      //  }
    }
    void bilinearFitSurface(Surface* src, QRect dst){
        {
            QRect roi=this->bounds.intersected(dst);
            for(int dsty=roi.top();dsty<roi.bottom();++dsty){
                bgra * dstrow=getRowAdress(dsty);
                float srcrow=(float)(dsty*(src->height-1))/(float)(height-1);
                for(int dstx=roi.left();dstx<roi.right();++dstx){
                    float srccol=(float)(dstx*(src->width-1))/(float)(width-1);
                    *dstrow=src->getBilinearSample(srccol,srcrow);
                    ++dstrow;
                }

            }
        }
    }


};

#endif // SURFACE_H
