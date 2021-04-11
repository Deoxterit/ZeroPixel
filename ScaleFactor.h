#ifndef SCALEFACTOR_H
#define SCALEFACTOR_H
#pragma once
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <math.h>
class ScaleFactor{
private:
    int numerator;
    int denominator;

    void clamp(){
        if (this->numerator*100 < 1*this->denominator )
        {
            this->numerator=1;
            this->denominator=100;
        }
        else if (this->numerator*1 > 32*this->denominator )
        {
            this->numerator=32;
            this->denominator=1;
        }
    }
public:
    int getNumerator(){
        return numerator;
    }
    int getDenominator(){
        return denominator;
    }
 bool operator==(ScaleFactor rhs)
    {
        return (this->numerator * rhs.denominator) == (rhs.numerator * this->denominator);
    }

 bool operator!=(ScaleFactor rhs)
    {
        return !(*this == rhs);
    }

 bool operator<(ScaleFactor rhs)
    {
        return (this->numerator * rhs.denominator) < (rhs.numerator * this->denominator);
    }

 bool operator<=(ScaleFactor rhs)
    {
        return (this->numerator * rhs.denominator) <= (rhs.numerator * this->denominator);
    }

  bool operator>(ScaleFactor rhs)
    {
        return (this->numerator * rhs.denominator) > (rhs.numerator * this->denominator);
    }

  bool operator>=(ScaleFactor rhs)
    {
        return (this->numerator * rhs.denominator) >= (rhs.numerator * this->denominator);
    }
    float getRatio(){
        return (float) numerator/(float)denominator;
    }
    static ScaleFactor oneToOne() {
        return ScaleFactor(1,1);
    }
    static ScaleFactor minValue(){
        return ScaleFactor(1,100);
    }
    static ScaleFactor maxValue(){
        return ScaleFactor(32,1);
    }
    static ScaleFactor useIfValid(int num,int denom,ScaleFactor lastResort){
        if (num <= 0 || denom <= 0)
        {
            return lastResort;
        }
        else
        {
            return ScaleFactor(num, denom);
        }
    }
    static ScaleFactor min(int n1,int d1,int n2,int d2,ScaleFactor last){
        ScaleFactor a = useIfValid(n1, d1, last);
        ScaleFactor b = useIfValid(n2, d2, last);
        return min(a, b);
    }
    static ScaleFactor max(int n1,int d1,int n2,int d2,ScaleFactor last){
        ScaleFactor a = useIfValid(n1, d1, last);
        ScaleFactor b = useIfValid(n2, d2, last);
        return max(a, b);
    }
    static ScaleFactor min(ScaleFactor l,ScaleFactor r){
        if (l < r)
        {
            return l;
        }
        else
        {
            return r;
        }
    }
    static ScaleFactor max(ScaleFactor l,ScaleFactor r){
        if (l > r)
        {
            return l;
        }
        else
        {
            return r;
        }
    }

    int scaleScalar(int x){
        return (int)(((long)x * numerator) / denominator);
    }
    int unscaleScalar(int x){
        return (int)(((long)x * denominator) / numerator);
    }
    double scaleScalar(double x){
        return (x * (float)numerator) / (float)denominator;
    }
    double unscaleScalar(double x){
        return (x * (float)denominator) / (float)numerator;
    }
    QPoint scalePoint(QPoint x){
        return QPoint(scaleScalar(x.x()),scaleScalar(x.y()));
    }
    QSize scaleSize(QSize x){
        return QSize(scaleScalar(x.width()),scaleScalar(x.height()));
    }
    QPoint scalePointX(QPoint x){
        return QPoint(scaleScalar(x.x()),(x.y()));
    }
    QPoint scalePointY(QPoint x){
        return QPoint((x.x()),scaleScalar(x.y()));
    }
    QPointF scalePoint(QPointF x){
        return QPointF(scaleScalar(x.x()),scaleScalar(x.y()));
    }
    QPointF scalePointX(QPointF x){
         return QPointF(scaleScalar(x.x()),(x.y()));
    }
    QPointF scalePointY(QPointF x){
        return QPointF((x.x()),scaleScalar(x.y()));
    }
    QPoint unscalePoint(QPoint x){
        return QPoint(unscaleScalar(x.x()),unscaleScalar(x.y()));
    }
    QPoint unscalePointX(QPoint x){
        return QPoint(unscaleScalar(x.x()),(x.y()));
    }
    QPoint unscalePointY(QPoint x){
        return QPoint((x.x()),unscaleScalar(x.y()));
    }
    QPointF unscalePoint(QPointF x){
        return QPointF(unscaleScalar(x.x()),unscaleScalar(x.y()));
    }
    QPointF unscalePointX(QPointF x){
        return QPointF(unscaleScalar(x.x()),(x.y()));
    }
    QPointF unscalePointY(QPointF x){
        return QPointF((x.x()),unscaleScalar(x.y()));
    }
    QRect scaleRectangle(QRect x){
        return QRect(scalePoint(x.topLeft()),scalePoint(x.bottomRight()));
    }
    QRect unscaleRectangle(QRect x){
        return QRect(unscalePoint(x.topLeft()),unscalePoint(x.bottomRight()));
    }

    QRectF scaleRectangle(QRectF x){
        return QRectF(scalePoint(x.topLeft()),scalePoint(x.bottomRight()));
    }
    QRectF unscaleRectangle(QRectF x){
        return QRectF(unscalePoint(x.topLeft()),unscalePoint(x.bottomRight()));
    }

    static ScaleFactor reduce(int numerator,int denominator){
        int factor = 2;
        while (factor < denominator && factor < numerator)
        {
            if ((numerator % factor) == 0 && (denominator % factor) == 0)
            {
                numerator /= factor;
                denominator /= factor;
            }
            else
                ++factor;
        }
        return ScaleFactor(numerator, denominator);
    }
    static ScaleFactor fromDouble(double x){
        int numerator = (int)(floor(x * 1000.0));
        int denominator = 1000;
        return reduce(numerator, denominator);
    }
    ScaleFactor(int n,int d){
        if(d<=0) throw "reger";
        if(n<0) throw "ehrth";
        this->numerator=n;
        this->denominator=d;
        this->clamp();
    }

};

#endif // SCALEFACTOR_H
