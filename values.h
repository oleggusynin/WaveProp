#ifndef _VALUES_H
#define _VALUES_H

#define Hr(j,i)   *(Hr+(I+3)*(j)+(i))
#define Hu(j,i)   *(Hu+(I+3)*(j)+(i))
#define Hv(j,i)   *(Hv+(I+3)*(j)+(i))
#define Ht(j,i)   *(Ht+(I+3)*(j)+(i))

double T[1200][4], Tn[1200][4], Ta[1200][4], Taa[1200][4];

double Xs[1200];
int fo[4100];
int Body[1200];

double dx,dy, Lx=4., Ly=4.;

double tau=0.,h=0.,Time=0.;

const double a=1.;

int memColor=15,KRIP=0;




///see the config.ini file

//[Filename]

char filename[40];
int numSShots;

//[function Fi]

double Ystart, Yend, Ax, Bx;

//[Window size]

int Height;
int Width;


//[X\Y position]

int IX0, IXD, IY0, IYD;


//[Order]
int order;
//[Gravitation]

double g;

//[Net]

int J, I;

//[Drow]

int Virt;
int Real;
int xtd;

//- - - - - - - - - - - - - - - - - - - - - - -

  double* Hr,* Hu,* Hv;
  int* Ht;

  void TakeMem()
  {
      int n=(I+3)*(J+3);
      Hr=(double*)calloc(n,8);
      Hu=(double*)calloc(n,8);
      Hv=(double*)calloc(n,8);
      Ht=(int*)calloc(n,4);
  }


#endif //_VALUES_H
