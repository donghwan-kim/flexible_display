//RGBpixmap.h  
 
#ifndef _RGBPIXMAP  
#define _RGBPIXMAP  
 
#include <fstream>  
#include <iostream>  
#include <string>  
using namespace std;  
 
#include <windows.h>  
#include <assert.h>  
#include <math.h>  
#include <gl/GL.h>  
#include <gl/Glu.h>  
  
class IntPoint  
{  
public:  
    int x,y;  
    void set(int dx,int dy){x=dx,y=dy;}  
    void set(IntPoint& p){x=p.x;y=p.y;}  
    IntPoint(int xx,int yy){x=xx;y=yy;}  
    IntPoint(){x=y=0;}  
};  
  
class IntRect  
{  
public:  
    int left,top,right,bott;  
    IntRect(){left=top=right=bott=0;}  
    IntRect(int l,int t,int r,int b)  
    {left=l; top=t; right=r;bott=b;}  
    void set(int l,int t,int r,int b)  
    {left=l;top=t;right=r;bott=b;}  
    void set(IntRect& r){left=r.left;top=r.top;right=r.right;bott=r.bott;}  
};  
  
  
typedef unsigned char uchar;  
  
class mRGB  
{  
public:  
    uchar r,g,b;  
    mRGB(){r=g=b=0;}  
    mRGB(mRGB& p){r=p.r;g=p.g;b=p.b;}  
    mRGB(uchar rr,uchar gg,uchar bb){r=rr;g=gg;b=bb;}  
};  
  
class RGBpixmap  
{  
private:  
  
public:  
    mRGB* pixel;  
//  mRGB* numPixel;  
    int nRows,nCols;  
    void makeCheckerboard();  
    void setTexture(GLint textureName);  
    RGBpixmap(){nRows=nCols=0;pixel=0;}  
    RGBpixmap(int rows,int cols)  
    {  
        nRows=rows;  
        nCols=cols;  
        pixel=new mRGB[rows* cols];  
    }  
  
    int readBMPFile(string fname);  
  
    void freeIt()  
    {  
        delete []pixel;  
        nRows=nCols=0;  
    }  
  
    void copy(IntPoint from,IntPoint to,int x,int y,int width,int height)  
    {  
        if(nRows==0||nCols==0)return;  
        glCopyPixels(x,y,width,height,GL_COLOR);        //  
    }  
  
    void draw()  
    {  
        if(nRows==0||nCols==0)return;  
        //OpenGL?4  
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);  
        glDrawPixels(nCols,nRows,GL_RGB,GL_UNSIGNED_BYTE,pixel);         //framebuffer  
    }  
  
    int read(int x,int y,int wid,int ht)  
    {  
        nRows=ht;  
        nCols=wid;  
        pixel=new mRGB[nRows*nCols];  
        if(!pixel)return -1;  
        glPixelStorei(GL_PACK_ALIGNMENT,1);                              //  
        glReadPixels(x,y,nCols,nRows,GL_RGB,GL_UNSIGNED_BYTE,pixel);     // framebuffer  
        return 0;  
    }  
  
    int read(IntRect r)  
    {  
        nRows=r.top-r.bott;  
        nCols=r.right-r.left;  
        pixel=new mRGB[nRows*nCols];  
        if(!pixel)return -1;  
        glPixelStorei(GL_PACK_ALIGNMENT,1);  
        glReadPixels(r.left,r.bott,nCols,nRows,GL_RGB,GL_UNSIGNED_BYTE,pixel);  
        return 0;  
    }  
  
    void setPixel(int x,int y,mRGB color)  
    {  
        if(x>=0 && x<nCols && y>=0 &&y<nRows)  
            pixel[nCols*y+x]=color;  
    }  
  
    mRGB getPixel(int x,int y)  
    {  
        mRGB bad(255,255,255);  
        assert(x>=0 && x<nCols);  
        assert(y>=0 && y<nRows);  
        return pixel[nCols*y+x];  
    }  
};  
 
#endif  