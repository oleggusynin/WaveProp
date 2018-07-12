#ifndef _MBW_C
#define _MBW_C
#include "list.h"

extern int memColor;
extern int fo[4100];

typedef unsigned char unchar;
typedef unsigned long unlong;



int O_BIN=0,/*S_MOD=S_IWRITE|S_IREAD,*/O_WBC;

mode_t S_MOD=S_IWRITE|S_IREAD;
//mode_t S_MOD=S_IREAD;
unlong   MAXX,MAXY,X_LAST,Y_LAST,BytInLin,BytInMem;
unchar   *SCR,MASKA[]={128,64,32,16,8,4,2,1};

// MemInitGraph  -------------------------------------
int meminitgraph(int X,int Y)
{ unlong i;
  i=(X+7)/8;  BytInLin=i; MAXY=Y-1;  MAXX=BytInLin*8-1;
  BytInMem=BytInLin*(MAXY+1);
  SCR=(unsigned char *)calloc(1,BytInMem);
  if(SCR==NULL)return(-1); else return(1);
}

// MemCloseGraph  -----------------------------------
void memclosegraph( void ){ free(SCR); }

// MemMoveTo      -----------------------------------
void memmoveto(int X,int Y){ X_LAST=X; Y_LAST=Y; }

// MemPutPixel    -----------------------------------
void memputpixel(int X,int Y,int color)
{ unlong  bit,byt;  unchar M;
 if((X<0)||(X>MAXX)||(Y<0)||(Y>MAXY)) {memmoveto(X,Y);return;}
 byt=X/8+Y*BytInLin; bit=X%8;  M=MASKA[bit];
 if(color==15) SCR[byt]|=M;  else  SCR[byt]&=255-M;
 memmoveto(X,Y);
}

// MemGetPixel    -----------------------------------
int  memgetpixel(int X,int Y)
{ int color;  unlong  bit,byt;  unchar M;
  if((X<0)||(X>MAXX)||(Y<0)||(Y>MAXY)) return(0);
  byt=X/8+Y*BytInLin;  bit=X%8;   M=MASKA[bit];
  if(SCR[byt]&M==M)color=15;  else color=0;
  return(color);
}

// MemScreen 1    -----------------------------------
void memscreen1()
{ unlong LinScrN,LinMemN,BytOut,i,j,x,y; char u;
  BytOut=(getmaxx()+1)/8; if(BytOut>BytInLin)BytOut=BytInLin;
  LinScrN=LinMemN=0;
  while((LinScrN <= getmaxy())&&(LinMemN<=MAXY))
  { for(i=0;i<BytOut;i++)
    { u=*(SCR+BytInLin*LinMemN+i);
      for(j=0;j<=7;j++)
      { x=8*i+j; y=LinScrN;
	if(u&128>>j)putpixel(x,y,WHITE);  else putpixel(x,y,BLACK);
      }
    }
    LinScrN++;  LinMemN++;
  }
}

// MemScreen 2    -----------------------------------
void memscreen2(int Xplace,int Yplace,int Masht)
{ unlong LinScrN,LinMemN,BytOut,i,j,x,y; char u;

  setcolor(15);  x=(MAXX-1)/Masht; y=(MAXY-1)/Masht;
 setfillstyle(1,WHITE);
  bar(Xplace,Yplace,Xplace+x,Yplace+y);

  BytOut=((getmaxx()+1)/8)*Masht; if(BytOut>BytInLin)BytOut=BytInLin;
  LinScrN=LinMemN=0;
  while((LinScrN < getmaxy()*Masht)&& LinMemN<MAXY)
  { for(i=0;i<BytOut;i++)
    { u=*(SCR+BytInLin*LinMemN+i);
      for(j=0;j<=7;j++)
      { if(u&128>>j)continue;
        x=Xplace+(8*i+j)/Masht; y=Yplace+LinScrN/Masht;
	if(x<getmaxx()&&y<getmaxy())putpixel(x,y,BLACK);
      }
    }
    LinScrN++;  LinMemN++;
  }
}

// MemClearDevice   ---------------------------------
void memcleardevice(int color)
{ unlong k;  unchar c;  if(color==15) c=255;  else c=0;
  memmoveto(0,0);
  for(k=0;k<BytInMem;k++)SCR[k]=c;
}

// MemInverse       ---------------------------------
void meminverse()
{ unlong k; for(k=0;k<BytInMem;k++)SCR[k]^=255;
}

// SMALL FUNCTIONS  ---------------------------------
int  memgetx(void){ return(X_LAST); }
int  memgety(void){ return(Y_LAST); }
int  memgetmaxx(void){ return(MAXX); }
int  memgetmaxy(void){ return(MAXY); }

// MemLine         ----------------------------------
inline void memline(int x0,int y0,int x1,int y1)
{ int t,d,xt,yt,dx,dy,incx=0,incy=0;

  dx=x1-x0; dy=y1-y0;
  if(dx>0){incx=1;}  if(dx<0){incx=-1; dx=-dx;}
  if(dy>0){incy=1;}  if(dy<0){incy=-1; dy=-dy;}
  if(dx>dy)d=dx; else d=dy;
  xt=d/2; yt=d/2;
  for(t=0;t<=d;t++)
  { memputpixel(x0,y0,memColor);  xt+=dx; yt+=dy;
    if(xt>d){ xt=xt-d; x0+=incx; }
    if(yt>d){ yt=yt-d; y0+=incy; }
  }
  memputpixel(x1,y1,memColor);
}
// MemCircle  ---------------------------------------
void memcircle(int x,int y,double r)
{ int i,x1,y1,s,p; double t;
  s=(int)(r+1.);
  if(x>=0 && x<MAXX && y>=0 && y<MAXY)
  for(i=0;i<8*s;i++)
  { t=i*3.14159265/(4*s);
    p=(int)(r*cos(t)); x1=x+p; p=(int)(r*sin(t)); y1=y+p;
    if(x1>=0 && x1<MAXX && y1>=0 && y1<MAXY)memline(x,y,x1,y1);
  }
}

// MemLineTo  ---------------------------------------
void memlineto(int X,int Y){ memline(X_LAST,Y_LAST,X,Y);}

// MemTiff   ----------------------------------------
void memtiff(char *fname,int I)
{  int  i,j,h,LenHead=342;
   unchar Head[342]=
   {0x49,0x49,0x2A,0x00, 0x08,0x00,0x00,0x00,
    0x12,0x00,0xFF,0x00, 0x03,0x00,0x01,0x00,
    0x00,0x00,0x01,0x00, 0x00,0x00,0x00,0x01,
    0x03,0x00,0x01,0x00, 0x00,0x00,0x80,0x02,
    0x00,0x00,0x01,0x01, 0x03,0x00,0x01,0x00,
    0x00,0x00,0xDD,0x01, 0x00,0x00,0x02,0x01,
    0x03,0x00,0x01,0x00, 0x00,0x00,0x01,0x00,
    0x00,0x00,0x03,0x01, 0x03,0x00,0x01,0x00,
    0x00,0x00,0x01,0x00, 0x00,0x00,0x06,0x01,
    0x03,0x00,0x01,0x00, 0x00,0x00,0x01,0x00,
    0x00,0x00,0x07,0x01, 0x03,0x00,0x01,0x00,
    0x00,0x00,0x01,0x00, 0x00,0x00,0x0A,0x01,
    0x03,0x00,0x01,0x00, 0x00,0x00,0x01,0x00,
    0x00,0x00,0x11,0x01, 0x04,0x00,0x01,0x00,
    0x00,0x00,0x56,0x01, 0x00,0x00,0x12,0x01,
    0x03,0x00,0x01,0x00, 0x00,0x00,0x01,0x00,
    0x00,0x00,0x15,0x01, 0x03,0x00,0x01,0x00,
    0x00,0x00,0x01,0x00, 0x00,0x00,0x16,0x01,
    0x04,0x00,0x01,0x00, 0x00,0x00,0x00,0x02,
    0x00,0x00,0x17,0x01, 0x04,0x00,0x01,0x00,
    0x00,0x00,0x10,0x95, 0x00,0x00,0x18,0x01,
    0x03,0x00,0x01,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x19,0x01, 0x03,0x00,0x01,0x00,
    0x00,0x00,0x01,0x00, 0x00,0x00,0x1A,0x01,
    0x05,0x00,0x01,0x00, 0x00,0x00,0x46,0x01,
    0x00,0x00,0x1B,0x01, 0x05,0x00,0x01,0x00,
    0x00,0x00,0x4E,0x01, 0x00,0x00,0x1C,0x01,
    0x03,0x00,0x01,0x00, 0x00,0x00,0x01,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00, 0x00,0x00,0x2C,0x01,
    0x00,0x00,0x01,0x00, 0x00,0x00,0x2C,0x01,
    0x00,0x00,0x01,0x00, 0x00,0x00
   };
    ///O_WBC=O_WRONLY|O_BIN|O_CREAT //for Linux
O_WBC=O_WRONLY|O_BINARY|O_CREAT;  ///for Windows


   i=(MAXX+6)&0xFFF8;
   j=i%256; i=i/256;  Head[0x1e]=j; Head[0x1f]=i;

   i=MAXY+1; j=i%256;  i=i/256;
   Head[0x2a]=Head[0x96]=j;  Head[0x2b]=Head[0x97]=i;

   i=((MAXX+6)>>3)*(MAXY+1);
   j=i%256; i=i/256;  Head[0xa2]=j;
   j=i%256; i=i/256;  Head[0xa3]=j;
   j=i%256; i=i/256;  Head[0xa4]=j; Head[0xa5]=i;

   if(I==1)h=open(fname,O_WBC|O_APPEND,S_MOD);
   else    h=open(fname,O_WBC|O_TRUNC,S_MOD);
   if(h<0)exit(-1);
   write(h,Head,LenHead); write(h,SCR,BytInMem); close(h);
}

// TiffMem   ----------------------------------------
int tiffmem(char *fname,int I)
{  int  i,j,h,LenHead=342;    unchar Head[342];

   i=(MAXX+6)&0xFFF8;
   j=i%256; i=i/256;  Head[0x1e]=j; Head[0x1f]=i;

   i=MAXY+1; j=i%256;  i=i/256;
   Head[0x2a]=Head[0x96]=j;  Head[0x2b]=Head[0x97]=i;

   i=((MAXX+6)>>3)*(MAXY+1);
   j=i%256; i=i/256;  Head[0xa2]=j;
   j=i%256; i=i/256;  Head[0xa3]=j;
   j=i%256; i=i/256;  Head[0xa4]=j; Head[0xa5]=i;

   h=open(fname,O_RDONLY|O_BIN);  if(h<0)return(-1);
   lseek(h,(342+BytInMem)*I,SEEK_SET);
   read(h,Head,LenHead); read(h,SCR,BytInMem);
   close(h);

   return 0;
}


//  MemOutTextXY  ----------------------------------
void  memouttextxy(int X,int Y,char  *textstring)
{ int i,j,k,L,x,y; unlong byt;  L=strlen(textstring);
  for(i=0;i<L;i++)
  { x=X+8-X%8+i*8;  j=textstring[i]; if(j<0)j+=256; j=8*j;
    for(k=0;k<8;k++)
    { y=Y+k; if((x<0)||(x>MAXX)||(y<=0)||(y>=MAXY))continue;
      byt=x/8 + y*BytInLin;   SCR[byt]=fo[j+k];
    }
  }
}


void  memouttextxy16(int X,int Y,char  *textstring)
{ int i,j,k,L,x,y; unlong byt;  L=strlen(textstring);
  for(i=0;i<L;i++)
  { x=X+8-X%8+i*8;  j=textstring[i]; if(j<0)j+=256; j=16*j;
    for(k=0;k<16;k++)
    { y=Y+k; if((x<0)||(x>MAXX)||(y<=0)||(y>=MAXY))continue;
      byt=x/8 + y*BytInLin;   SCR[byt]=fo[j+k];
    }
  }
}


#endif //_MBW_C
