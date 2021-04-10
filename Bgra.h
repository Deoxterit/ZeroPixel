#ifndef BGRA_H
#define BGRA_H
#pragma once
#include <cmath>
typedef unsigned char byte;
typedef unsigned int uint;

struct hsv{
    short H;
    byte S;
    byte V;
    hsv(short h,byte s,byte v):H(h),V(v),S(s){

    }
};

struct bgra{
    union{
        uint color;
        struct{
            byte B;
            byte G;
            byte R;
            byte A;
        };
    };
    bgra(){
        bgra(0,0,0,0);
    }
    bgra(byte r,byte g, byte b,byte a=255):R(r),G(g),B(b),A(a){

    }
    bgra(uint clr):color(clr){

    }
    void fromRgba(byte r,byte g,byte b,byte a=255){
        this->R=r;
        this->G=g;
        this->B=b;
        this->A=a;
    }

    byte& operator[](byte channel){
        channel&=0x7;
        byte &ptr=*(&B+sizeof (byte)*channel);
        return ptr;
    }
    static bgra alphaBlend(bgra src,bgra dst){
        uint not_a = 256 - (src.A);
           return  bgra(src.color + (((not_a * (dst.B*256))>>16) |
                          (((not_a * (dst.G*256))>>8) & 0xff00) |
                          (((not_a * (dst.R*256))) & 0xff0000) |
                          (((not_a * (dst.A*256))<<8) & 0xff000000)));
    }
    static bgra mix(bgra c1,bgra c2,byte alpha){
        uint rb=c1.color&0xff00ff;
        uint g=c1.color&0x00ff00;
        rb += ((c2.color & 0xff00ff) - rb) * alpha >> 8;
        g += ((c2.color & 0x00ff00) - g) * alpha >> 8;
        uint a = (alpha << 24);
        return bgra((rb & 0xff00ff) | (g & 0xff00) | (a));
    }
    static bgra lerp(bgra from, bgra to, float frac) {
            //(from + frac * (to - from));
            return bgra(
                (from.R + frac * (to.R - from.R)),
                (from.G + frac * (to.G - from.G)),
                (from.B + frac * (to.B - from.B)),
                (from.A + frac * (to.A - from.A))
            );
        }
    static bgra blerp(bgra c1,bgra c2,bgra c3,bgra c4,float frac,float frac2){
        return lerp(lerp(c1,c2,frac),lerp(c3,c4,frac),frac2);
    }
    static bgra mix4w16(bgra c1,uint w1,bgra c2,uint w2,bgra c3,uint w3,bgra c4,uint w4){
        uint ww=32768;
        uint af=c1.A*w1 + c2.A*w2 + c3.A*w3 + c4.A*w4;
        uint a=(af+ww)>>16;
        if(a==0) return bgra(0,0,0,0);
        uint b=((c1.A*c1.B*w1)+(c2.A*c2.B*w2)+(c3.A*c3.B*w3)+(c4.A*c4.B*w4))/af;
        uint g=((c1.A*c1.G*w1)+(c2.A*c2.G*w2)+(c3.A*c3.G*w3)+(c4.A*c4.G*w4))/af;
        uint r=((c1.A*c1.R*w1)+(c2.A*c2.R*w2)+(c3.A*c3.R*w3)+(c4.A*c4.R*w4))/af;
        return bgra(r,g,b,a);

    }
    static bgra toRGB(hsv from) {
            float r, g, b;
            float h = from.H % 360;
            float s = from.S / 100;
            float v = from.V / 100;
            if (s == 0) {
                r = v;
                g = v;
                b = v;
            }
            else {
                float p, q, t;
                int numsectr;
                float factr, possectr;
                possectr = h / 60;
                numsectr = floor(possectr);
                factr = possectr - numsectr;
                p = v * (1 - s);
                q = v * (1 - (s * factr));
                t = v * (1 - (s * (1 - factr)));
                switch (numsectr)
                {
                case 0:
                    r = v;
                    g = t;
                    b = p;
                    break;
                case 1:
                    r = 1;
                    g = v;
                    b = p;
                    break;
                case 2:
                    r = p;
                    g = v;
                    b = t;
                    break;
                case 3:
                    r = p;
                    g = q;
                    b = v;
                    break;
                case 4:
                    r = t;
                    g = p;
                    b = v;
                    break;
                case 5:
                    r = v;
                    g = p;
                    b = q;
                    break;
                }
            }
        }
    static hsv toHSV(bgra from) {
            float min, max, delta;
            float h, s, v;
            float r = from.R / 255;
            float g = from.G / 255;
            float b = from.B / 255;
            min = std::min(std::min(from.R, from.G), from.B);
            max = std::max(std::max(from.R, from.G), from.B);
            delta = max - min;
            v = max;
            if (max == 0 || delta == 0) {
                s = 0;
                h = 0;
            }
            else {
                s = delta / max;
                if (r == max)
                    h = (g - b) / delta;
                else if (g == max)
                    h = 2 + (b - r) / delta;
                else
                    h = 4 + (r - g) / delta;
            }
            h *= 60;
            if (h < 0)
                h += 360;
            return hsv(h, s * 100, v * 100);
        }
    byte getIntensity(){
        return(byte)((7471*B+38470*G+19595*R)>>16);
    }
};

#endif // BGRA_H
