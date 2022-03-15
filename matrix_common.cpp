#include<iostream>
#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
using namespace std;

int main()
{
timeval tv_begin,tv_end;//record start and end time
gettimeofday(&tv_begin,NULL);
const int x = 100,y=100;//x,y present cols and rows
double sum[x];
double a[y];
double b[y][x];

int temp = 0;//base number
for(int i = 0;i<x;i++){//initialize
        for(int j=0;j<y;j++){
        a[j] = ++temp%1000;
        b[j][i] = temp++%1000;
        }
}
//////////////////////////////////////////////////////////////////////////////
const int howMany = 100;//howmany times will you test
for(int ptime = howMany;ptime>0;ptime--){
for(int i = 0; i < x; i++){
        sum[i] = 0.0;
        for(int j = 0; j < y; j++)
                sum[i] += b[j][i] * a[j];
        }
}
gettimeofday(&tv_end,NULL);
 cout<<"time:"<<tv_end.tv_usec - tv_begin.tv_usec<<endl;
 }
