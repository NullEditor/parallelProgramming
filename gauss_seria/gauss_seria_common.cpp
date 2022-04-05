#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include<sys/time.h>
#include<unistd.h>
using namespace std;
//采用列主元高斯消去
const int n = 300;//执行程序前手动改变方程数
timeval tv_begin,tv_end;//record start and end time
void ColPivot(double c[n][n+1],double x[])
{
gettimeofday(&tv_begin,NULL);
    int i,j,k;
    double p,maxs;
    double t[n];
    for(i=0;i<=n-2;i++)
    {
        maxs=0;
        k=i;
        for(j=i+1;j<n;j++)
            if(fabs(c[j][i])>maxs)
            {
                k=j;
                maxs=fabs(c[j][i]);
            }
        if(k!=i)
            for(j=i;j<=n;j++)
                swap(c[i][j], c[k][j]);
        for(j=i+1;j<n;j++)
        {
            p=c[j][i]/c[i][i];
            for(k=i;k<=n;k++)
                c[j][k]-=p*c[i][k];
        }
    }
        for(i=0;i<n;i++)
            t[i]=c[i][n];
        for(i=n-1;i>=0;i--)
        {
            for(j=n-1;j>i;j--)
                t[i]-=c[i][j]*x[j];
            x[i]=t[i]/c[i][i];
        }
        gettimeofday(&tv_end,NULL);
 
}

int main()
{
    double temp[n][n+1];
    double x[n];
    cout<<"当前n= "<<n<<"需要输入"<<n<<"行"<<n+1<<"列"<<endl;
    for(int i = 0;i < n;i++)
        for(int j = 0;j <= n;j++)
            cin >> temp[i][j];
    ColPivot(temp, x);
    for(int i = 0;i < n;i++)
        cout << "X" << i << "=:" << x[i] << endl;
    cout<<"time:"<<(tv_end.tv_usec - tv_begin.tv_usec)<<endl; 
    return 0;
}
