#include "list.h"
#include "mbw.c"
#include "pole.c"
#define ksi(x) (int)((x)*dx/4.)
#define eta(y) (int)(dy-(y)*dy/2.-100)
#define cik4 for(k=0; k<=2; ++k)
#include "read_file.h"
#include "function.h"
#include <omp.h>
///===================== M A I N


int main()
{
    read_config("config.ini");
     show_config();
     printf("\n\n Press any key ...\n\n");
     getchar();
      int i,j,n,k,flag=0,numscreen=0;
      double x,y;
    double nu=0.1,v1, D1=0,v2=0,D2;
    TakeMem();
            initwindow(Height,Width);
if(Virt==1) meminitgraph(Height,Width);

    h=8./(J-1);


double TIMESTART,TIMEEND;
TIMESTART=omp_get_wtime();
     for(j=0; j<=J; ++j)
     {
         for(i=0; i<=I; ++i)
         {
             x=j*h-0.5*h; y=i*h-0.5*h;
             Hr(j,i)=1.; Hu(j,i)=Hv(j,i)=0.;
             if(x<=0.5) Hr(j,i)=2.;
             Ht(j,i)=0.;
             ///====== О
//             if((x-5)*(x-5)+(y-4.7)*(y-4.7)<=0.5) ///окружность
//             {
//               Ht(j,i)=1.;
//               Hr(j,i)=4.;
//             }
             if(((x-4.7)*(x-4.7))/2.+((y-4.3)*(y-4.3))/6.>0.1 && ((x-4.7)*(x-4.7))/2.+((y-4.3)*(y-4.3))/6.<=0.13) /// эллипс
             {
               Ht(j,i)=1.;
               Hr(j,i)=4.;
             }
//             if(((x-4.7)*(x-4.7))/2.+((y-4.3)*(y-4.3))/6.<=0.1) ///
//             {
//               Ht(j,i)=1.;
//               Hr(j,i)=4.;
//             }

            ///====== Г
             if(x>=2.5  && x<=2.6 && y>=3.6 && y<=5.1)
             {
                 Ht(j,i)=1.;
                 Hr(j,i)=4.;
             }

             if(x>=2.5 && x<=3.5 && y>=5. && y<=5.15)
             {
                 Ht(j,i)=1.;
                 Hr(j,i)=4.;
             }
              ///======= С
             if(x>=6.1 && x<=6.2 && y>=3.6 && y<=5.1)
             {
                 Ht(j,i)=1.;
                 Hr(j,i)=4.;
             }
             if(x>=6.1 && x<=6.8 && y>=3.6 && y<=3.75)
             {
                 Ht(j,i)=1.;
                 Hr(j,i)=4.;
             }
             if(x>=6.1 && x<=6.8 && y>=5. && y<=5.15)
             {
                 Ht(j,i)=1.;
                 Hr(j,i)=4.;
             }
         }
     }
TIMEEND=omp_get_wtime();
printf("\n\n TIME 1 = %f src\n\n",(TIMEEND-TIMESTART));
getchar();
/// - - - - - - - - - - - - - - - -


//        for(j=0; j<=J; ++j)
//        {
//             Xs[j] = j*h-0.5*h; // координата центра ячеек
//             T[j][0] = fi(Xs[j]); T[j][1] = 0; T[j][2]=0.;
//        }

    ///- - - - - - - - - -

    for(n=0; Time<=1000.0; ++n){///цикл по времени
///- - - - - вычисление шага по времени
        for(j=1; j<J; ++j){
            for(i=1; i<I; ++i){
            v1 = fabs(Hu(j,i))+sqrt(g*Hr(j,i));
            v2 = fabs(Hv(j,i))+sqrt(g*Hr(j,i));
            if(D1<v1)D1=v1;
            if(D2<v2)D2=v2;
        }
        }
        tau = 1.15*h/(D1+D2);
        if(n<10)tau/=10.;
        SW1D(J,tau/h);
///    
for(i=1; i<I; ++i)
    {
        //загрузка в массив
        for(j=0; j<=J; ++j){ T[j][0]=Hr(j,i);
                             T[j][1]=Hu(j,i);
                             T[j][2]=Hv(j,i);
                             Body[j]=Ht(j,i); }
        SW1D(J,tau/h);
        //выгрузка из массива
      for(j=1; j<=J; ++j){ Hr(j,i)=Tn[j][0];
                           Hu(j,i)=Tn[j][1];
                           Hv(j,i)=Tn[j][2];
                      }

    }

   BC();
//
for(j=1; j<J; ++j)
    {
        //загрузка в массив
        for(i=0; i<=I; ++i){ T[i][0]=Hr(j,i);
                             T[i][1]=Hv(j,i);
                             T[i][2]=Hu(j,i);
                             Body[i]=Ht(j,i); }
        SW1D(I,tau/h);
        //выгрузка из массива
      for(i=1; i<=I; ++i){ Hr(j,i)=Tn[i][0];
                           Hv(j,i)=Tn[i][1];
                           Hu(j,i)=Tn[i][2];
                        }

    }
BC();
   // Virt=0;
    //setcolor(BLUE);

 ///- - -  - - - - - - - - - - -



if(flag<=numSShots*2 && Virt==1){
    cleardevice();
    pole(1.,0.,10.,0.043,Hr);
    createfiles(filename,flag, numscreen);

     flag++;
     if(flag==numSShots*2) break;
        }
if(Virt==0 && Real==1)
{

    setactivepage(0);
    cleardevice();
    setvisualpage(1);
    pole(1.,0.,10.,0.043,Hr);
    setactivepage(1);
    cleardevice();
    setvisualpage(0);
    pole(1.,0.,10.,0.043,Hr);
}

getch();

for(j=0;j<=J;j++) cik4 T[j][k]=Tn[j][k];
           Time=Time+tau;
    }//Time

    closegraph();

   if(Virt==1) memclosegraph();

    return 0;
}

