#include<iostream>
#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>
using namespace std;
const int n=1000;
timeval tv_begin,tv_end;//record start and end time
int sum[n],a[n],b[n][n];

void init(int n){
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++){
			a[j] = j;
			b[i][j] = i+j;
		}
}

int main(){
	init(n);
	gettimeofday(&tv_begin,NULL);
		for(int i = 0; i < n; i++){
			sum[i] = 0.0;
			for(int j = 0; j < n; j++)
				sum[i] += b[j][i] * a[j];
				}
	gettimeofday(&tv_end,NULL);
	cout<<"time:"<<(tv_end.tv_usec - tv_begin.tv_usec)<<endl;
}
