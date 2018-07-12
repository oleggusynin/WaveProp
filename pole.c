/************************  POLE.C  for 2D GasDyn ******************/
#include <graphics.h>
#include <math.h>
//#include "values.h"
#define Zz(j,i) *(Zz+(I+3)*(j)+(i))
#define Hm(j,i) *(Hm+(I+3)*(j)+(i))

int pprintf(int,int,int,int,char*,...);
extern double Lx,Ly,*Hm;
extern int J,I,KRIP,Virt,Real;
extern int IX0, IXD, IY0, IYD;
       int jf[6]={0,0,1,1,0,0},kf[6]={0,0,0,1,1,0};
      // int IX0=20, IY0=600, IXD=400, IYD=400;
       int Nsq=0;


void lin(int x1, int y1, int x2, int y2)
{ int a;
  /* a=x1; x1=y1; y1=-a; a=x2; x2=y2; y2=-a;*/     /* for turn */
  if(Virt!=0)
     { if(x2>=x1)memline(x1,y1,x2,y2); else memline(x2,y2,x1,y1);}
  if(Real!=0)
     { line(x1,y1,x2,y2);}
}


void pole(double Z,double Ha,double Hb,double HL,double *Zz)
{ int i,j,k,m,ii,jj,L,KX1,KY1,KX2,KY2;
  double XJ[11],YJ[11],X0,X[7],Y0,Y[7],DF[7],F[7],SF[7],XSS[9],YSS[9];
  double T,A,B,C,D,e,U,V,x,y,hm,hx,hy,FMIN,FMAX,Q,SL;
  int L1,L2,LD,KT,IZ,ID;
  int K1[6]={0,1,2,1,4},K2[6]={0,2,3,4,3};
/*----------------------------------------------------*/
  ID=Z*IYD+0.1;  hm=fabs(HL);  hx=Lx/(J-1); hy=Ly/(I-1);

for(i=1;i<I;i++)
{ for(j=1;j<J;j++)
  {
    for(k=1;k<=5;k++)
    { jj=j-1+jf[k]; ii=i-1+kf[k]; XJ[k]=jj*hx; YJ[k]=ii*hy;}
    if(XJ[1]<0. || 2.*XJ[2]-XJ[1] > Lx)continue;

if(KRIP>0)
{  setcolor(KRIP);                          /**** Grid ****/
   for(k=1;k<=4;k++)
   {
     if((i%10==1 && k==1) || (j%10==1 && k==4) )setcolor(KRIP+8);
     else setcolor(KRIP);
     KX1=IX0+IXD*XJ[k]+0.0001;   KY1=IY0-ID*YJ[k]+0.0001;
     KX2=IX0+IXD*XJ[k+1]+0.0001; KY2=IY0-ID*YJ[k+1]+0.0001;
     lin(KX1,KY1,KX2,KY2);
   }
}

 for(k=1;k<=4;k++){ jj=j+jf[k];  ii=i+kf[k];  F[k]=Zz(jj,ii);  }
// if(Nsq)for(m=1;m<=Nsq;m++)for(k=1;k<=4;k++){F[k]=sqrt(fabs(F[k])); }

 FMIN=F[1]; FMAX=F[1];
 for(k=2;k<=4;k++) { if(F[k]<FMIN)FMIN=F[k];  if(F[k]>FMAX)FMAX=F[k]; }
 if(FMAX<Ha || FMIN>Hb)continue;


 L1=(FMIN-Ha)/hm+1; L2=(FMAX-Ha)/hm+1; if(L1>L2)continue;

/*
if(FMAX>FMIN)
{  pprintf(20,20,11,0,"%f %f %4i %4i j=%3i i=%3i ",FMIN,FMAX,L1,L2,j,i);
   pprintf(20,40,11,0,"hm %f   Ha %f ",hm,Ha);
   getch();
}
*/
  setcolor(15);
//  B=Hm(j,i); if( B>1.)setcolor(12);
//  if(B>0.49 && B<=1)setcolor(11);
//  if(B>0.25 && B<=0.49)setcolor(13);

for(k=1;k<=5;k++){ X[k]=XJ[k]+0.5*hx; Y[k]=YJ[k]+0.5*hy; }

     for(k=1;k<=4;k++) { DF[k]=F[K2[k]]-F[K1[k]]; }
/*-----------------*/
    for(L=L1;L<=L2;L++)
    {  SL=Ha+L*hm;  if(SL<Ha || SL>Hb)continue;
       for(k=1;k<=4;k++) { SF[k]=SL-F[k]; } KT=0;
       for(k=1;k<=4;k++)
       { A=SF[K1[k]]; B=SF[K2[k]];
	 if(A*B< 0.) { KT=KT+1; e=SF[K1[k]]/DF[k];
		       XSS[KT]=X[K1[k]]+(X[K2[k]]-X[K1[k]])*e;
		       YSS[KT]=Y[K1[k]]+(Y[K2[k]]-Y[K1[k]])*e;
		     }
       }
 for(k=KT;k>=2;k=k-2){ KX1=IX0+IXD*XSS[k-1]; KY1=IY0-ID*YSS[k-1];
		       KX2=IX0+IXD*XSS[k];   KY2=IY0-ID*YSS[k];
		       lin(KX1,KY1,KX2,KY2);
		     }
     }
  }
}

}

