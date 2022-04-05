#include <iostream>
#include <iomanip>
#include <fstream>
#include<sys/time.h>
#include<unistd.h>
#include <algorithm>
#include <string>
#include <unistd.h> //对系统API操作
#include<sstream>
using namespace std;
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
		binArr = newArr;
		length = newLength;
	}

};

//读入数据
const int singleInput = 100000;
//对每行数据读入elMatrix中
void inputData(BinArray* elMatrix, int elMatrixLength,char* line) {
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


int main() {
timeval tv_begin,tv_end;//record start and end time
gettimeofday(&tv_begin,NULL);
 char *path = get_current_dir_name(); //调用系统API获取当前路径
    string currentPath = path;
    free(path);  //释放指针
    path = NULL; //指针重新赋值
 
    int indexParent = currentPath.find_last_of("/");
    string parentPath = currentPath.substr(0, indexParent);
    string txt_0 = parentPath+ "/guass_grobner/data/0.txt";
    string txt_1 = parentPath+ "/guass_grobner/data/1.txt";
    string txt_2 = parentPath+ "/guass_grobner/data/2.txt";
    string txt_res = parentPath+ "/guass_grobner/data/MyResult.txt";
	bool whichFile = true;//中间会进行反复从2个不同文件中读取消元子,所以true代表从1.txt,false代表从0中读取
	ifstream Eliminated_line_file(txt_2, ios::in);//读取被消元行内容
	if (!Eliminated_line_file) {
		cout << "Unable to open Eliminated_line_file";
		exit(1); // terminate with error  

	}
	while (!Eliminated_line_file.eof()) {
		// 读入被消元行//////////////////////////////////////////////////////////////////////////////////////////////////
		BinArray* elMatrix = new BinArray[10];//被消元行的二维矩阵
		int elMatrixLength = 0;//被消元行有多少行(理论上是10,但是尾部可能不是10)
		for (elMatrixLength; elMatrixLength < 10 && !Eliminated_line_file.eof(); elMatrixLength++) {//每次读10个被消元行
			char line[singleInput];
			Eliminated_line_file.getline(line, singleInput);
			if (line[0] != 0)
				inputData(elMatrix, elMatrixLength, line);
			else
				break;
		}
		// 读入消元子//////////////////////////////////////////////////////////////////////////////////////////////////
		ifstream Eliminator_file;
		if (whichFile == true)
			Eliminator_file.open(txt_1);//读取消元子内容1
		else
			Eliminator_file.open(txt_0);//读取消元子内容0
		if (!Eliminator_file) {
			cout << "Unable to open Eliminator_file";
			exit(1); // terminate with error  
		}
		while (!Eliminator_file.eof()) {
			BinArray* eMatrix = new BinArray[20];//消元子的二维矩阵
			int eMatrixLength = 0;//消元子有多少行(理论上是20,但是尾部可能不是20)
			for (eMatrixLength; eMatrixLength < 10 && !Eliminator_file.eof(); eMatrixLength++) {//每次读10个被消元行
				char line[singleInput];
				Eliminator_file.getline(line, singleInput);
				if (line[0] != 0)
					inputData(eMatrix, eMatrixLength, line);
				else
					break;
			}
			//划分可行域与范围,注意可行域指的是可以异或的散点值,范围指的是这一组消元子覆盖的上下界,是一个区间
			int* canSub = new int[singleInput];//可行域,特点是可以以O(1)级别查找是否有首项对应消去元,但是牺牲了空间
			int upperBound = 0;//当前组的上下界,利用有序性
			int lowerBound = 0;
			for (int i = 0; i < eMatrixLength; i++) {
				if (i == 0)
					upperBound = eMatrix[i].binArr[0];
				if (i == eMatrixLength-1)
					lowerBound = eMatrix[i].binArr[0];
				canSub[eMatrix[i].binArr[0]] = 1;//canSub[x] = 1代表第x为有消去元

			}
			//开始对每个被消元行进行计算
			for (int currentLine = 0; currentLine < elMatrixLength; currentLine++) {
				//如果在可行域内,直接异或
				int item = elMatrix[currentLine].binArr[0];
				if (canSub[item] == 1) {//这一步是确定被消元行首项在不在当前组消元子可行域内
					//在的话,找到那个消元子并异或
					for (int k = 0; k < eMatrixLength; k++) {
						if (eMatrix[k].binArr[0] == item) {
							elMatrix[currentLine].xorArr(eMatrix[k]);
							if (elMatrix[currentLine].length == 0) {//如果删掉以后为空行
								for (int a = currentLine; a < elMatrixLength; a++) {
									elMatrix[a] = elMatrix[a + 1];
								}
								elMatrixLength--;
							}
							break;
						}
					}
					currentLine--;
					continue;//再次扫描这个被消元行,因为一个被消元行可能被多个消元子消元
				}
				//如果不在可行域内,看看是否是空行
				if (elMatrix[currentLine].length == 0)
					continue;//跳过
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
					for (int k = 0; k < eMatrixLength; k++) {
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
							int p = eMatrixLength-1;
							for (p; p >= 1; p--) {
								if (item > eMatrix[p].binArr[0] && item < eMatrix[p - 1].binArr[0]) {//说明它介于p和p-1之间
									for (int l = eMatrixLength; l >= p+1; l--) {
										eMatrix[l] = eMatrix[l - 1];
									}
									eMatrix[p] = elMatrix[currentLine];
									eMatrixLength++;
									canSub[item] = 1;
									break;
								}
							}
						}
					}
						//第三步,从被消元行中删除
					for (int k = currentLine; k < elMatrixLength - 1; k++) {
						elMatrix[k] = elMatrix[k + 1];
					}
					elMatrixLength--;
						//第四步,由于删掉了那一行,所以currentLine--
					currentLine--;
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
		//最后把新的消元子存入另一个消元子文件中
			ofstream anotherEfile;
			if(whichFile==true)
				anotherEfile.open(txt_0, ios::app);
			else
				anotherEfile.open(txt_1, ios::app);
			for (int k = 0; k <eMatrixLength; k++) {
				for (int l = 0; l < eMatrix[k].length; l++) {
					anotherEfile << eMatrix[k].binArr[l] << " ";
				}
				anotherEfile << endl;
			}
		}
		//当所有消元子处理完以后,如果被消元行矩阵还有元素,则也加入到消元子序列中,此时一定是比最低消元行还要小的元素
		ofstream anotherEfile;
		if (whichFile == true)
			anotherEfile.open(txt_0, ios::app);
		else
			anotherEfile.open(txt_1, ios::app);
		for (int k = 0; k < elMatrixLength; k++) {
			for (int l = 0; l < elMatrix[k].length; l++) {
				anotherEfile << elMatrix[k].binArr[l] << " ";
			}
			anotherEfile << endl;
		}

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
	gettimeofday(&tv_end,NULL);
cout<<"time:"<<(tv_end.tv_usec - tv_begin.tv_usec)<<endl;  
}
