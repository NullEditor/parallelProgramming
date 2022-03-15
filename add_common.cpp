#include<iostream>
#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
using namespace std;
const int n = 9000;
timeval tv_begin,tv_end;//record start and end time
int a[n] = {0};
int sum = 0;

void init(int n){
for(int i=0;i<n;i++){
a[i] = i+1;
}
}

int main(){
init(n);
gettimeofday(&tv_begin,NULL);
for(int loop = 100;loop>0;loop--)
for (int i = 0; i < n; i++)
 sum += a[i];
 gettimeofday(&tv_end,NULL);
cout<<"time:"<<(tv_end.tv_usec - tv_begin.tv_usec)<<endl;
}