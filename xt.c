/************************  x-t diagram T - Tn  && t1 - t2    ***************/
#include "list.h"

#define Zz(j,i) *(Zz+N3*(j)+(i))

  extern int J,Virt,Real,IX0,IXD,IY0,IYD;
  extern double T[2200][4],Tn[2200][4],Xs[2200];

  double Xkl=0.,Ykl=0.;

inline void lin(int x1, int y1, int x2, int y2)
{
  if(Virt!=0){ if(x2>=x1)memline(x1,y1,x2,y2); else memline(x2,y2,x1,y1);}
  if(Real!=0){ line(x1,y1,x2,y2);}
}

inline void pole1(double Z,double HL,double t1,double t2,int COLOR=15)
{ int j,k,L,KX1,KY1,KX2,KY2,L1,L2,KT,IX00,IY00,ID;
  double A,B,Q,FMIN,FMAX,SL,X[7],Y[7],DF[7],F[7],SF[7],XSS[9],YSS[9];
  int K1[5]={0,1,2,1,4},K2[5]={0,2,3,4,3};
/*----------------------------------------------------*/
  IX00=(int)(IX0-Xkl*IXD);  IY00=(int)(IY0+Ykl*IYD*Z);  ID=(int)(Z*IYD);

 for(j=1;j<=J-2;j++)
 { X[1]=Xs[j];     Y[1]=t1;     F[1]=T[j][0];
   X[2]=Xs[j+1];   Y[2]=t1;     F[2]=T[j+1][0];
   X[3]=Xs[j+1];   Y[3]=t2;     F[3]=Tn[j+1][0];
   X[4]=Xs[j];     Y[4]=t2;     F[4]=Tn[j][0];
   X[5]=X[1];      Y[5]=Y[1];   F[5]=F[1];

if(IY00<ID*t2)return;

   FMIN=FMAX=F[1]; for(k=2;k<=4;k++){ A=F[k]; if(A<FMIN)FMIN=A;  if(A>FMAX)FMAX=A; }

   L1=(int)(FMIN/HL+1); L2=(int)(FMAX/HL+1); if(L1>=L2)continue;

   for(k=1;k<=4;k++) { DF[k]=F[K2[k]]-F[K1[k]]; }
   setcolor(COLOR);
//------------------------
   for(L=L1;L<=L2;L++)
   { SL=L*HL;
     for(k=1;k<=4;k++) { SF[k]=SL-F[k]; } KT=0;
     for(k=1;k<=4;k++)
     { A=SF[K1[k]]; B=SF[K2[k]];
       if(A*B<0.)
       { KT=KT+1; Q=SF[K1[k]]/DF[k];
         XSS[KT]=X[K1[k]]+(X[K2[k]]-X[K1[k]])*Q;
         YSS[KT]=Y[K1[k]]+(Y[K2[k]]-Y[K1[k]])*Q;
       }
     }
     for(k=KT;k>=2;k=k-2)
     { KX1=(int)(IX00+IXD*XSS[k-1]); KY1=(int)(IY00-ID*YSS[k-1]);
       KX2=(int)(IX00+IXD*XSS[k]);   KY2=(int)(IY00-ID*YSS[k]);
       lin(KX1,KY1,KX2,KY2);
     }
   }/* L */
}/* j */

 setcolor(COLOR);
// Left-Right Borders
 KX1=(int)(IX00+IXD*(Xs[0]+Xs[1])/2.);  KY1=(int)(IY00-ID*t1);
 KX2=KX1;  KY2=(int)(IY00-ID*t2);   lin(KX1,KY1,KX2,KY2);

 KX1=(int)(IX00+IXD*(Xs[J-1]+Xs[J])/2.);  KY1=(int)(IY00-ID*t1);
 KX2=KX1;  KY2=(int)(IY00-ID*t2); lin(KX1,KY1,KX2,KY2);

}/****** End of pole1.c ******/


