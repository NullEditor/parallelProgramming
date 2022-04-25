#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>
#include<sstream>
#include<Windows.h>
#include<pthread.h>
using namespace std;
#pragma comment(lib, "pthreadVC2.lib")

//一个基本行数据结构,用于存放一行数据(稀疏矩阵原形式)
class BinArray {
public:
	int* binArr;//数组
	int length;//长度
	//初始化
	BinArray(int* arr, int n) {
		length = n;
		binArr = new int[n];
		for (int i = 0; i < n; i++) {
			binArr[i] = arr[i];
		}
	}
	BinArray() {
		length = -1;
	}
	//与消元子异或,arr为减数,n为减数的size,对排序好的数组,复杂度为O(n)
	void xorArr(BinArray beSub) {
		int thisMax = 0;//被减数最大指针
		int arrMax = 0;
		int* newArr = new int[beSub.length + length];//目标数组
		int newLength = 0;//新数组的元素真个数
		int targetI = 0;//目标数组当前应该存入哪个位置,可以指示length
		int count = 0;//计算的数目,当为两个数组之和时结束循环
		while (count < beSub.length + length) {
			if (binArr[thisMax] > beSub.binArr[arrMax]) {
				newArr[targetI++] = binArr[thisMax];
				newLength++;
				thisMax++;
				count++;
			}
			else if (binArr[thisMax] < beSub.binArr[arrMax]) {
				newArr[targetI++] = beSub.binArr[arrMax];
				newLength++;
				arrMax++;
				count++;

			}
			else if (binArr[thisMax] == beSub.binArr[arrMax]) {//异或了
				thisMax++;
				arrMax++;
				count += 2;
			}

			if (thisMax == length) {
				for (int j = arrMax; j < beSub.length; j++) {//如果被减数全部读完,那剩下的照抄就可以了
					newArr[targetI++] = beSub.binArr[j];
					newLength++;
					count++;
				}
			}
			if (arrMax == beSub.length) {
				for (int j = thisMax; j < length; j++) {//如果减数全部读完,那剩下的照抄就可以了
					newArr[targetI++] = binArr[j];
					newLength++;
					count++;
				}
			}
		}
		delete this->binArr;
		binArr = newArr;
		length = newLength;
	}

};

//读入数据
const int singleInput = 100000;
//对每行数据读入elMatrix中
void inputData(BinArray* elMatrix, int elMatrixLength, char* line) {
	if (line[0] == 0)
		return;
	stringstream ss(line);//读取一行
	int* arr = new int[10000]{ -1 };//临时数组
	int currentPos = 0;//存到哪个位置
	int currentSum = 0;//有多少个length
	while (ss >> arr[currentPos++]) {
		currentSum++;
	}
	//目前为止,处理完了一个数组
	BinArray temp(arr, currentSum);
	elMatrix[elMatrixLength] = temp;
}
string txt_0 = "C:\\资料\\大二下\\并行程序设计\\GroData\\7_8399_6375_4535\\0.txt";
string txt_1 = "C:\\资料\\大二下\\并行程序设计\\GroData\\7_8399_6375_4535\\1.txt";
string txt_2 = "C:\\资料\\大二下\\并行程序设计\\GroData\\7_8399_6375_4535\\2.txt";
string txt_res = "C:\\资料\\大二下\\并行程序设计\\GroData\\7_8399_6375_4535\\myResult.txt";

typedef struct {
	int t_id; //线程 id
	
}threadParam_t;
const int NUM_THREADS = 10;
pthread_barrier_t barrier_open_elMatrix;//打开被消元行文件
pthread_barrier_t barrier_open_eMatrix;//打开消元子
//pthread_barrier_t barrier_elfeof;//被消元行读完了吗
//pthread_barrier_t barrier_efeof;//消元子读完了吗
pthread_barrier_t barrier_readIn_elMatrix;//id=0读入被消元行的barrier同步
pthread_barrier_t barrier_readIn_eMatrix;//id=0读入消元子的barrier同步
pthread_barrier_t barrier_canSubAndRange;//id=0划分可行域和范围
pthread_barrier_t barrier_elimilate;//各线程做完工作,同步
pthread_barrier_t barrier_update;//id=0进行真判断哪个可以升级
pthread_barrier_t barrier_saveTo;//id=0存入另一个消元子文件


//常量们
bool whichFile = true;//中间会进行反复从2个不同文件中读取消元子,所以true代表从1.txt,false代表从0中读取

bool elfeof = false;//被消元行读完了吗
bool efeof = false;//消元子读完了吗

ifstream Eliminated_line_file;
BinArray* elMatrix = new BinArray[10];//被消元行的二维矩阵
int elMatrixLength = 0;//被消元行有多少行(理论上是10,但是尾部可能不是10)

ifstream Eliminator_file;
BinArray* eMatrix = new BinArray[20];//消元子的二维矩阵
int eMatrixLength = 0;//消元子有多少行(理论上是20,但是尾部可能不是20)

int* canSub = new int[singleInput];//可行域,特点是可以以O(1)级别查找是否有首项对应消去元,但是牺牲了空间
int upperBound = 0;//当前组的上下界,利用有序性
int lowerBound = 0;

bool mayBeUpdate[10] = { false };//可能升级为消元子吗,默认为false,当检测到可能时根据对应t_id变成true


bool ifReallyUpdate(int currentLine) {
	/*
	args:currentLine:当前在elMatrix中的第几行
	*/
	//如果在可行域内,直接异或
	
	while (true) {
		int item = elMatrix[currentLine].binArr[0];
		if (canSub[item] == 1) {//这一步是确定被消元行首项在不在当前组消元子可行域内
			//在的话,找到那个消元子并异或
			for (int k = 0; k < eMatrixLength; k++) {
				if (eMatrix[k].binArr[0] == item) {
					elMatrix[currentLine].xorArr(eMatrix[k]);
					break;
				}
			}
			continue;//再次扫描这个被消元行,因为一个被消元行可能被多个消元子消元
		}
		//如果不在可行域内,看看是否是空行
		if (elMatrix[currentLine].length == 0)
			return false;//跳过
		//如果在范围内,此时只有一种可能,它可以被升级成消元子,特别的,我们只需要保证下界即可,因为可能有被夹在中间的被消元行
		if (item >= lowerBound) {
			//首先,这个特殊的被消元行是我们想要的结果矩阵的一部分,把他直接存进去
			ofstream outfile;
			outfile.open(txt_res, ios::app);
			for (int k = 0; k < elMatrix[currentLine].length; k++) {
				outfile << elMatrix[currentLine].binArr[k] << " ";
			}
			outfile << endl;
			outfile.close();
			//第二步,存入消元子数组内,并更新可用域和上域
				//先判断应该存入哪里,不破坏它的顺序性
				if (item > upperBound) {//如果比第一个消元子还大,那么放在第一个
					for (int p = eMatrixLength; p >= 1; p--) {
						eMatrix[p] = eMatrix[p - 1];
					}
					eMatrix[0] = elMatrix[currentLine];
					eMatrixLength++;
					upperBound = item;
					canSub[item] = 1;
				}
				if (item > lowerBound && item < upperBound) {//介于中间,则找到对应的位置
					int p = eMatrixLength - 1;
					for (p; p >= 1; p--) {
						if (item > eMatrix[p].binArr[0] && item < eMatrix[p - 1].binArr[0]) {//说明它介于p和p-1之间
							for (int l = eMatrixLength; l >= p + 1; l--) {
								eMatrix[l] = eMatrix[l - 1];
							}
							eMatrix[p] = elMatrix[currentLine];
							eMatrixLength++;
							canSub[item] = 1;
							break;
						}
					}
				}
			//第三步,返回,删除操作留给外面做
			
			return true;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		break;
	}
}

void* threadFunc(void* param) {
	threadParam_t* p = (threadParam_t*)param;
	int t_id = p -> t_id;//赋id



	//step1:读入
	if (t_id == 0) {
		Eliminated_line_file.open(txt_2);//读取被消元行内容
		if (!Eliminated_line_file) {
			cout << "Unable to open Eliminated_line_file";
			pthread_exit(NULL); // terminate with error  
		}
	}

	pthread_barrier_wait(&barrier_open_elMatrix);


	while (true) {
		//由id=0开始读入被消元行
 		if (t_id == 0) {
			if (Eliminated_line_file.eof()) {
				elfeof = true;
			}
			else {
				elfeof = false;
				delete elMatrix;
				elMatrix = new BinArray[10];//被消元行的二维矩阵
				elMatrixLength = 0;//被消元行有多少行(理论上是10,但是尾部可能不是10)
				for (elMatrixLength; elMatrixLength < 10 && !Eliminated_line_file.eof(); elMatrixLength++) {//每次读10个被消元行
					char line[singleInput];
					Eliminated_line_file.getline(line, singleInput);
					if (line[0] != 0)
						inputData(elMatrix, elMatrixLength, line);
					else
						break;
				}
			}
		}
		pthread_barrier_wait(&barrier_readIn_elMatrix);//同步打开
		if (elfeof == true) {
			break;
		}
		//判断消元子文件情况
		if (t_id == 0) {
			if (whichFile == true)
				Eliminator_file.open(txt_1);//读取消元子内容1
			else
				Eliminator_file.open(txt_0);//读取消元子内容0
			if (!Eliminator_file) {
				cout << "Unable to open Eliminator_file";
				pthread_exit(NULL); // terminate with error  
			}
		}
		pthread_barrier_wait(&barrier_open_eMatrix);//同步消元子文件打开
		while (true) {
			//读入消元子
			if (t_id == 0) {
				if (Eliminator_file.eof()) {
					efeof = true;
				}
				else {
					efeof = false;
					delete eMatrix;
					eMatrix = new BinArray[20];//消元子的二维矩阵
					eMatrixLength = 0;//消元子有多少行(理论上是20,但是尾部可能不是20)
					for (eMatrixLength; eMatrixLength < 10 && !Eliminator_file.eof(); eMatrixLength++) {//每次读10个被消元行
						char line[singleInput];
						Eliminator_file.getline(line, singleInput);
						if (line[0] != 0)
							inputData(eMatrix, eMatrixLength, line);
						else
							break;
					}
				}
			}
			pthread_barrier_wait(&barrier_readIn_eMatrix);//同步消元子读取
			//划分可行域
			if (efeof == true) {
				break;
			}
			if (t_id == 0) {
				canSub = new int[singleInput];//可行域,特点是可以以O(1)级别查找是否有首项对应消去元,但是牺牲了空间
				upperBound = 0;//当前组的上下界,利用有序性
				lowerBound = 0;
				for (int i = 0; i < eMatrixLength; i++) {
					if (i == 0)
						upperBound = eMatrix[i].binArr[0];
					if (i == eMatrixLength - 1)
						lowerBound = eMatrix[i].binArr[0];
					canSub[eMatrix[i].binArr[0]] = 1;//canSub[x] = 1代表第x为有消去元

				}
			}
			pthread_barrier_wait(&barrier_canSubAndRange);//划分可行域和范围
			//现在各个线程开始各司其职,按照t_id对应的行进行消去

			//开始对每个被消元行进行计算
			//如果在可行域内,直接异或
			int currentLine = t_id;//当前线程处理的行就是t_id对应的行

			while(currentLine<elMatrixLength){//当确实存在这一行
				int item = elMatrix[currentLine].binArr[0];
				if (canSub[item] == 1) {//这一步是确定被消元行首项在不在当前组消元子可行域内
					//在的话,找到那个消元子并异或
					for (int k = 0; k < eMatrixLength; k++) {
						if (eMatrix[k].binArr[0] == item) {
							elMatrix[currentLine].xorArr(eMatrix[k]);//最后空行处理问题留给t_id=0统一处理
							break;
						}
					}
					if (elMatrix[currentLine].length == 0) {
						break;
					}
					else
						continue;//再次扫描这个被消元行,因为一个被消元行可能被多个消元子消元
				}
				//如果不在可行域内,看看是否是空行
				if (elMatrix[currentLine].length == 0)
					break;//跳过
				//如果在范围内,此时只有一种可能,它可以被升级成消元子,特别的,我们只需要保证下界即可,因为可能有被夹在中间的被消元行
				if (item >= lowerBound) {
					mayBeUpdate[currentLine] = true;//标记这一行可能变成消元子
					break;
				}
				break;
				//////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			pthread_barrier_wait(&barrier_elimilate);//各线程计算完毕,等待同步
			//由id=0开始清算每个可能升级的被消元行是不是真的可以升级
			//还是要完成以下步骤:
			/*
			1.遍历刚刚的mayBeUpdate列表,找到为true的元素
			2.再一次判断它是不是可以被消去(步骤跟上面一模一样),这一步是确保在它之前有被消元行升级了,能够减去它的情况
			3.如果是真的能升级成消元子,则执行升级流程(这里记得更新elmatrixLength和elmatrix数组,否则跳过
			4.重置mayBeUpdate数组
			*/
			if (t_id == 0) {
				for (int i = 0; i < elMatrixLength; i++) {
					//如果可能升级
					if (mayBeUpdate[i] == true) {
						//这个函数完成了消元子升级的全过程,我们现在只需要清点length=0的elmatrix数组元素
						if (ifReallyUpdate(i) == true) {
							for (int k = i; k < elMatrixLength - 1; k++) {
								elMatrix[k] = elMatrix[k + 1];
							}
							for (int k = i; k < 10; k++) {
								mayBeUpdate[k] = mayBeUpdate[k + 1];
							}
							elMatrixLength--;
							i--;
						}
					}
				}
				//开始压缩elMatrix数组
				for (int i = 0; i < elMatrixLength; i++) {
					if (elMatrix[i].length == 0) {
						for (int k = i; k < elMatrixLength - 1; k++) {
							elMatrix[k] = elMatrix[k + 1];
						}
						elMatrixLength--;
						i--;
					}
				}
				//重置mayBeUpdate
				for (int i = 0; i < 10; i++) {
					mayBeUpdate[i] = false;
				}
				
			}
			pthread_barrier_wait(&barrier_update);
			//最后由id=0负责存入消元子
			if (t_id == 0) {
				ofstream anotherEfile;
				if (whichFile == true)
					anotherEfile.open(txt_0, ios::app);
				else
					anotherEfile.open(txt_1, ios::app);
				for (int k = 0; k < eMatrixLength; k++) {
					for (int l = 0; l < eMatrix[k].length; l++) {
						anotherEfile << eMatrix[k].binArr[l] << " ";
					}
					anotherEfile << endl;
				}
			}
		}
		//现在这一组被消元行已经处理完毕,由id=0的线程进行读取文件的切换
		if (t_id == 0) {
			//当前消元子处理完了,清除当前消元子文本内容,并且改变消元子源文件指向
			Eliminator_file.close();
			ofstream emptyFile;
			if (whichFile == true) {
				emptyFile.open(txt_1, ios::trunc);
				whichFile = false;
			}
			else {
				emptyFile.open(txt_0, ios::trunc);
				whichFile = true;
			}
		}
		pthread_barrier_wait(&barrier_saveTo);
	}
	pthread_exit(NULL);
	return 0;
}
int main() {
	LARGE_INTEGER t1, t2, tc;
	double time0;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	pthread_barrier_init(&barrier_canSubAndRange, NULL, NUM_THREADS);
	pthread_barrier_init(&barrier_elimilate, NULL, NUM_THREADS);
	pthread_barrier_init(&barrier_open_elMatrix, NULL, NUM_THREADS);
	pthread_barrier_init(&barrier_open_eMatrix, NULL, NUM_THREADS);
	pthread_barrier_init(&barrier_readIn_elMatrix, NULL, NUM_THREADS);
	pthread_barrier_init(&barrier_readIn_eMatrix, NULL, NUM_THREADS);
	pthread_barrier_init(&barrier_saveTo, NULL, NUM_THREADS);
	pthread_barrier_init(&barrier_update, NULL, NUM_THREADS);
	pthread_t handles[NUM_THREADS];// 创建对应的 Handle
	threadParam_t param[NUM_THREADS];// 创建对应的线程数据结构
	for(int t_id = 0; t_id < NUM_THREADS; t_id++) {
		param[t_id].t_id = t_id;
		pthread_create(&handles[t_id], NULL, threadFunc, &param[t_id]);
	}
	for (int t_id = 0; t_id < NUM_THREADS; t_id++) {
		pthread_join(handles[t_id], NULL);
	}
	pthread_barrier_destroy(&barrier_canSubAndRange);
	pthread_barrier_destroy(&barrier_elimilate);
	pthread_barrier_destroy(&barrier_open_elMatrix);
	pthread_barrier_destroy(&barrier_open_eMatrix);
	pthread_barrier_destroy(&barrier_readIn_elMatrix);
	pthread_barrier_destroy(&barrier_readIn_eMatrix);
	pthread_barrier_destroy(&barrier_saveTo);
	pthread_barrier_destroy(&barrier_update);

	QueryPerformanceCounter(&t2);
	std::cout << "time = " << (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart * 1000 << "ms" << std::endl;
	return 0;
}