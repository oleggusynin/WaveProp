/********************** ALL functions in project ***********/
extern int meminitgraph(int X,int Y);
extern void memclosegraph(void);
extern void memputpixel(int,int,int);
extern int memgetmaxx(void);
extern int memgetmaxy(void);
extern void memscreen1(void);
extern void memscreen2(int Xplace,int Yplace,int Masht);
extern void memcleardevice(int color);
extern void meminverse(void);
extern void memline(int startx,int starty,int endx,int endy);
extern void memouttextxy(int X,int Y,char *textstring);
extern void memcircle(int x,int y,double r);
extern void memtiff(char *fname,int I);
extern int  tiffmem(char *fname,int I);
extern double fi(double x);
extern double sign(double a);
extern void Rim(double *UL, double *UR, double *U);
extern void flux( double *UL, double *UR, double *F);
extern double mms(double a, double b);
extern double mhs(double a, double b);
extern double mhs1(double a, double b);
extern void correct(double * UL, double * UC,
                    double * UR, double * U);

extern void Ris (int par, int COLOR, double koef, double up);
extern void UinW(double *U, double *W);
extern void WinU(double *U, double *W);



