#ifndef _FUNCTION_H
#define _FUNCTION_H
#include "values.h"

///===================== FI

inline double fi(double x){
    double y=Ystart;
    if(x>Ax && x<Bx)y=Yend;
    return y;
}

///===================== S I G N

inline double sign(double a){ return (a<0)? -1 : 1; }

///======================= R I M

inline void Rim(double *UL, double *UR, double *U){

     double h,c,u,dh,du,z0,z1;

     h=0.5*(UL[0]+UR[0]);
     u=0.5*(UL[1]+UR[1]);
     c=sqrt(2.*g*h);
     dh=0.5*(UL[0]-UR[0]);
     du=0.5*(UL[1]-UR[1]);
     z0=0.5*(dh/h-du/c);
     z1=0.5*(dh/h+du/c);
     z0=sign(u-c)*z0;
     z1=sign(u+c)*z1;

     U[0]=h+h*(z0+z1);
     U[1]=u+c*(z1-z0);
     if(u>=0.) U[2]=UL[2]; else U[2]=UR[2];
}

///===================== F L U X

inline void flux( double *UL, double *UR, double *F){
    double U[4];
    Rim(UL,UR,U);
    F[0] = U[0]*U[1];
    F[1] = U[0]*U[1]*U[1]+0.5*g*U[0]*U[0];
    F[2] = U[0]*U[1]*U[2];
}

///====================== M M S

inline double mms(double a, double b){
    double c = a*b;
    if(c<=0.)return 0.;
    if(fabs(a)>fabs(b))return b;
    else return a;
}

///======================= M H S

inline double mhs(double a, double b){
    double c = a*b;
    return (c<=0.)? 0 : (c+c)/(b+b);
}

///================== M H S (1)

inline double mhs1(double a,double b)
{ double c,s,p,q,e,w; c=a*b; if(c<=0.)return 0.; s=a+b; c=(c+c)/s; p=fabs(a); q=fabs(b);
 if(p+q>=0.000001)
 { e=fabs(b-a)/(p+q); w=p; if(q>=w)w=q;  c=c*(1.+16.*e*e*(1.-e)*(1.-e)*(fabs(s)/w-1.));
 }
 return(c);
}

///=================== C O R R E C T

inline void correct(double * UL, double * UC, double * UR, double * U)
{
     double u,hhh,c,d0,d1,e0,e1,z,q;
     hhh=UC[0];
     u=UC[1];
     c=sqrt(2*g*hhh);
     d0=UC[0]-UL[0];
     d1=UC[1]-UL[1];
     e0=UR[0]-UC[0];
     e1=UR[1]-UC[1];
     //теперь каждый из них умножим на матрицу R-1
     z=0.5*(d0/hhh-d1/c);
     d1=0.5*(d0/hhh+d1/c);
     d0=z;
     z=0.5*(e0/hhh-e1/c);
     e1=0.5*(e0/hhh+e1/c);
     e0=z;
     d0=0.5*(sign(u-c)-tau*(u-c)/h)*mhs1(d0,e0);
     d1=0.5*(sign(u+c)-tau*(u+c)/h)*mhs1(d1,e1);
     U[0]=h*(d0+d1);
     U[1] = c*(d1-d0);
     U[2] = 0.5*((sign(u)-(tau/h)*u)*mhs(UC[2]-UL[2],UR[2]-UC[2]));
     q=fabs(U[0])/UC[0]; if(q>0.3){ U[0]/=q; U[1]/=q; U[2]/=q; }
}

///==================== R I S

inline void Ris (int par, int COLOR, double koef, double up){
    setcolor(COLOR);
    int j;
    for(j=1 ; j<J; ++j)
    line(ksi(Xs[j]),eta(koef*T[j][par]),ksi(Xs[j+1]),eta(koef*T[j+1][par]+up));
}

///===================U in W

inline void UinW(double *U, double *W){
    W[0] = U[0];
    W[1] = U[0]*U[1];
    W[2]=U[0]*U[2];
}

///=================== W in U

inline void WinU(double *U, double *W){
    U[0] = W[0];
    U[1] = W[1]/U[0];
    U[2]=W[2]/W[0];
}

///==================================================================////////////

void SW1D(int M, double dth)/// 1D представление воды
{
 int j, k;
    double VL[5], VR[5], W[5],U[5] ,*zL, *z,*zR;

    for(j=1; j<M; ++j)
    {
        if(Body[j]==1 || order==1) { Ta[j][0]=Ta[j][1]=Ta[j][2]=0.; continue;}
        zL=T[j-1]; z=T[j]; zR=T[j+1];
        if(Body[j-1]==1 && Body[j]==0) { U[0]=z[0]; U[1]=-z[1]; U[2]=z[2]; zL=U; }
        if(Body[j]==0 && Body[j+1]==1) { U[0]=z[0]; U[1]=-z[1]; U[2]=z[2]; zR=U; }
        correct(zL,z,zR,Ta[j]);
    }
    for(j=1; j<M; ++j) { if(Body[j]==1) continue; cik4{Taa[j][k]=Ta[j][k]+T[j][k];}  }

     cik4{Taa[0][k]=Taa[1][k]; Taa[M][k]=Taa[M-1][k];}
     Taa[0][1]=-Taa[1][1]; Taa[M][1]=-Taa[M-1][1];



     for(j=1; j<M; ++j)
     {
         if(Body[j]==1){ cik4{Tn[j][k]=T[j][k];} continue;}
         zL=Taa[j-1]; z=Taa[j]; zR=Taa[j+1];
         if(Body[j-1]==1 && Body[j]==0){ U[0]=z[0]; U[1]=-z[1]; U[2]=z[2]; zL=U;}
         if(Body[j]==0 && Body[j+1]==1){ U[0]=z[0]; U[1]=-z[1]; U[2]=z[2]; zR=U;}
         flux(zL,z,VL); flux(z,zR,VR);
         UinW(T[j],W);
         cik4{ W[k]=W[k]-(dth)*(VR[k]-VL[k]); }
         WinU(Tn[j],W);
     }

///- - - - - вычисления граничных значений ВС для решения
}

void BC()
{
     int i,j;
     for(j=1; j<J; ++j)
     {
         Hr(j,0)=Hr(j,1); Hu(j,0)=Hu(j,1); Hv(j,0)=-Hv(j,1);
         Hr(j,I)=Hr(j,I-1); Hu(j,I)=Hu(j,I-1); Hv(j,I)=-Hv(j,I-1);
     }
     for(i=1; i<J; ++i)
     {
         Hr(0,i)=Hr(1,i); Hu(0,i)=Hu(1,i); Hv(0,i)=-Hv(1,i);
         Hr(J,i)=Hr(J-1,i); Hu(J,i)=Hu(J-1,i); Hv(J,i)=-Hv(J-1,i);
     }
}

#endif //_FUNCTION_H
