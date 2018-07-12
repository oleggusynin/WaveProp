
#ifndef _READ_FILE_H
#define _READ_FILE_H
#include <stdio.h>
#include "values.h"


///=================== E N C O D E

inline void encode(const char* temp,char* buf )
{
    char str1[]="Height",str2[]="Width",str3[]="gravity",str4[]="PartitionX";
    char str5[]="VirtualScene",str6[]="RealScene";
    char str7[]="XtdDiagram",str8[]="IX0",str9[]="IXD",str10[]="IY0",str11[]="IYD";
    char str12[]="order", str13[]="Ystart",str14[]="Yend",str15[]="Ax",str16[]="Bx";
    char str17[]="Name",str18[]="PartitionY",str19[]="NumberOfScreenshots";
    char num[]="=",num2[]="\"";unsigned int i;
if(strstr(temp,str1))  Height   =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str2))  Width    =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str3))  g        =atof(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str4))  J        =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str5))  Virt     =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str6))  Real     =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str7))  xtd      =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str8))  IX0      =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str9))  IXD      =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str10)) IY0      =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str11)) IYD      =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str12)) order    =atoi(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str13)) Ystart   =atof(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str14)) Yend     =atof(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str15)) Ax       =atof(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str16)) Bx       =atof(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str17)) {
     strcpy(buf,(strstr(temp,num2)+1));
     for(i=0; i<strlen(buf)-2; ++i) filename[i]=buf[i];

}
if(strstr(temp,str18)) I               =atof(strcpy(buf,(strstr(temp,num)+1)));
if(strstr(temp,str19)) numSShots       =atof(strcpy(buf,(strstr(temp,num)+1)));
}




///================= R E A D  config.ini

inline void read_config(const char* filename)
{
    int len=40;

    FILE* file=fopen(filename,"rt");

    if((file=fopen(filename,"rt"))==NULL){
        printf("Can't open\\read file"); exit(-1);
    }

    char* temp=(char*)malloc(len*sizeof(char));
    char* buf=(char*)malloc(len*sizeof(char));

     while(!feof(file))
     {

         fgets(temp,len, file);
         encode(temp,buf);

     }

free(buf);
free(temp);
fclose(file);
}

///===================== S H O W  C O N F I G

void show_config()
{
    char str[4];if(Virt==0) strcpy(str,"NO");   else strcpy(str,"YES");
    char str1[4];if(Real==0) strcpy(str1,"NO"); else strcpy(str1,"YES");
    char str2[4];if(xtd==0) strcpy(str2,"NO");  else strcpy(str2,"YES");
printf("\n Filename:\n\n  name=%s\n  NumberOfScreenshots= %d \n",filename,numSShots);
printf("\n Function Fi:\n\n  Ystart= %.3f\n    Yend= %.3f\n\n      Ax= %.3f\n      Bx= %.3f\n\n",Ystart,Yend,Ax,Bx);
printf("\n Order:\n\n  order= %d\n\n Gravitation:\n\n  g= %.3f\n\n",order,g);
printf(" Partition X and Y:\n\n  J= %d\n  I=%d\n\n",J,I);
printf(" Draw:\n\n  Draw Virtual Scene = %s\n  Draw Real Scene = %s\n  Draw XTD Diagram = %s\n\n",str,str1,str2);
printf(" Window:\n\n  Height= %d\n   Width= %d\n\n",Height,Width);
printf(" Position:\n\n  IX0= %d\n  IXD= %d\n  IY0= %d\n  IYD= %d\n\n",IX0,IXD,IY0,IYD);

}

///============== C R E A T E  F I L E S

inline void createfiles(char* filename,int numShots, int &flag)
{
    meminverse();

    if(numShots%2==0){
        flag++;
    char str[10];
    char str2[20];
    strcpy(str2,filename);
    itoa(flag,str,10);
    strcat(str2,str);
    strcat(str2,".tiff");
    printf("Create file %d of %d\r",flag,numSShots);
    memtiff((str2),0);

    }
    memcleardevice(0);
}

#endif //_READ_FILE_H
