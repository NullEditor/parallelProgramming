#include <arm_neon.h>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include<sys/time.h>
#include<unistd.h>
using namespace std;
timeval tv_begin,tv_end;//record start and end time
const int n = 1000;//执行程序前手动改变方程数
///////////////////////////////////////////////////////////////////////////////////////////
void gauss(float matrix[n][n], float result[n][n]){
gettimeofday(&tv_begin,NULL);
for(int k=0;k<=n-1;k++){
    int j=k+1;
    float32x4_t vt = vdupq_n_f32(matrix[k][k]);
    for(j;j+4<=n;j+=4){
        float32x4_t va=vld1q_f32(*(matrix+k)+j);
        float32x4_t reciprocal = vrecpeq_f32(vt);
        reciprocal = vmulq_f32(vrecpsq_f32(vt, reciprocal), reciprocal);//高精度倒数
        
        va = vmulq_f32(va,reciprocal);
        vst1q_f32(*(matrix+k)+j, va);
    }
    for(j;j<n;j++){
        matrix[k][j] = matrix[k][j]/matrix[k][k];
    }
    matrix[k][k] = 1.0;
    
    for(int i=k+1;i<=n-1;i++){
        float32x4_t vaik = vdupq_n_f32(matrix[i][k]);
        int j=k+1;
        for(j;j+4<=n;j+=4){
       	    float32x4_t vakj=vld1q_f32(*(matrix+k)+j);
       	    float32x4_t vaij=vld1q_f32(*(matrix+i)+j);
       	    float32x4_t vx = vmulq_f32(vakj,vaik);
       	    float32x4_t neg = {-1,-1,-1,-1};
       	    vaij = vmlaq_f32(vaij,neg,vx);
       	    
       	    vst1q_f32(*(matrix+i)+j, vaij);
        }
        for(j;j<n;j++){
            matrix[i][j] = matrix[i][j]-matrix[k][j]*matrix[i][k];
        }
        matrix[i][k] = 0;
    }
}
for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++)
            cout<<matrix[i][j]<<" ";
        cout<<endl;}
gettimeofday(&tv_end,NULL);
cout<<"time:"<<(tv_end.tv_sec - tv_begin.tv_sec)<<endl;  
}
///////////////////////////////////////////////////////////////////////////////////////
int main(){
    float matrix[n][n];
    float result[n][n];
    cout<<"当前n= "<<n<<"需要输入"<<n<<"行"<<n<<"列"<<endl;
    for(int i = 0;i < n;i++)
        for(int j = 0;j < n;j++)
            cin >> matrix[i][j];
    gauss(matrix,result);

    
}


