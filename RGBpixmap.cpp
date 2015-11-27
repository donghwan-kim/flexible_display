//RGBpixmap.cpp  
 
#include "RGBpixmap.h"  
typedef unsigned short ushort;  
typedef unsigned long ulong;  
fstream inf;  
  
ushort getShort()                                       //  
{  
    //BMP  
    //2  
    char ic;  
    ushort ip;  
    inf.get(ic);  
    ip=ic;                                              //  
    inf.get(ic);  
    ip|=((ushort)ic<<8);                                //  
    return ip;  
}  
  
ulong getLong()  
{  
    ulong ip=0;  
    char ic=0;  
    unsigned char uc=ic;  
    inf.get(ic);uc=ic;ip=uc;  
    inf.get(ic);uc=ic;ip|=((ulong)uc<<8);  
    inf.get(ic);uc=ic;ip|=((ulong)uc<<16);  
    inf.get(ic);uc=ic;ip|=((ulong)uc<<24);  
    return ip;  
}  
  
int RGBpixmap::readBMPFile(string fname)  
{   
    //BMPmRGB  
    //0?1  
    inf.open(fname.c_str(),ios::in|ios::binary);                    //  
    if(!inf){cout<<"Can't open file:"<<fname<<endl;return 0;}  
    int k,row,col,numPadBytes,nBytesInRow;  
    //  
    char ch1,ch2;  
    inf.get(ch1);inf.get(ch2);                                     //?'BM'  
    ulong fileSize=getLong();  
    ushort reserved1=getShort();                                   //0  
    ushort reserved2=getShort();                                   //0  
    ulong offBits=getLong();                                       //-  
    ulong headerSize=getLong();                                    //40  
    ulong numCols=getLong();                                       //  
    ulong numRows=getLong();                                       //  
    ushort planes=getShort();                                      //1  
    ushort bitsPerPixel=getShort();                                //824?24  
    ulong compression=getLong();                                   //0  
    ulong imageSize=getLong();                                     //  
    ulong xpels=getLong();                                         //0  
    ulong ypels=getLong();                                         //0  
    ulong numLUTentries=getLong();                                 //8256?0  
    ulong impColors=getLong();                                     //0  
    if(bitsPerPixel!=24)  
    {  
        //24  
        cout<<"not a 24 bit/pixelimage,or is compressed!/n";  
        inf.close();return 0;  
    }  
  
    //4  
    //3*numCols4  
    nBytesInRow=((3*numCols+3)/4)*4;  
    numPadBytes=nBytesInRow-3*numCols;                             //  
    nRows=numRows;                                                 //  
    nCols=numCols;  
    pixel=new mRGB[nRows*nCols];                                   //  
    if(!pixel)return 0;                                            //  
    long count=0;  
    char dum;     
    char r,g,b;  
    for(row=0;row<nRows;row++)                                     //  
    {  
        for(col=0;col<nCols;col++)  
        {  
          
            inf.get(b);inf.get(g);inf.get(r);                      //  
            pixel[count].r=r;                                      //  
            pixel[count].g=g;  
            pixel[count++].b=b;  
        }  
        for(k=0;k<numPadBytes;k++)                                 //  
            inf>>dum;  
    }  
    inf.close();  
    return 1;                                                      //  
}  
  
void RGBpixmap::makeCheckerboard()  
{  
    nRows=nCols=64;                              //24*24  
    pixel=new mRGB[3*nRows*nCols];               //  
    if(!pixel){cout<<"out of memory!";return;}  
    long count=0;  
    for(int i=0;i<nRows;i++)  
        for(int j=0;j<nCols;j++)  
        {  
            int c=(((i/8)+(j/8))%2)*255;  
            pixel[count].r=c;  
            pixel[count].g=c;  
            pixel[count++].b=0;  
        }  
}  
  
void RGBpixmap::setTexture(GLint textureName)  
{  
    glBindTexture(GL_TEXTURE_2D,textureName);  
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);  
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);  
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,nCols,nRows,0,GL_RGB,GL_UNSIGNED_BYTE,pixel);//  
}  