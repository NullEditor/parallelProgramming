//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <algorithm>
//#include <string>
//#include<sstream>
//#include<pthread.h>
//#include<Windows.h>
//using namespace std;
////һ�����������ݽṹ,���ڴ��һ������(ϡ�����ԭ��ʽ)
//class BinArray {
//public:
//	int* binArr;//����
//	int length;//����
//	//��ʼ��
//	BinArray(int* arr, int n) {
//		length = n;
//		binArr = new int[n];
//		for (int i = 0; i < n; i++) {
//			binArr[i] = arr[i];
//		}
//	}
//	BinArray() {
//		length = -1;
//	}
//	//����Ԫ�����,arrΪ����,nΪ������size,������õ�����,���Ӷ�ΪO(n)
//	void xorArr(BinArray beSub) {
//		int thisMax = 0;//���������ָ��
//		int arrMax = 0;
//		int* newArr = new int[beSub.length + length];//Ŀ������
//		int newLength = 0;//�������Ԫ�������
//		int targetI = 0;//Ŀ�����鵱ǰӦ�ô����ĸ�λ��,����ָʾlength
//		int count = 0;//�������Ŀ,��Ϊ��������֮��ʱ����ѭ��
//		while (count < beSub.length + length) {
//			if (binArr[thisMax] > beSub.binArr[arrMax]) {
//				newArr[targetI++] = binArr[thisMax];
//				newLength++;
//				thisMax++;
//				count++;
//			}
//			else if (binArr[thisMax] < beSub.binArr[arrMax]) {
//				newArr[targetI++] = beSub.binArr[arrMax];
//				newLength++;
//				arrMax++;
//				count++;
//
//			}
//			else if (binArr[thisMax] == beSub.binArr[arrMax]) {//�����
//				thisMax++;
//				arrMax++;
//				count += 2;
//			}
//
//			if (thisMax == length) {
//				for (int j = arrMax; j < beSub.length; j++) {//���������ȫ������,��ʣ�µ��ճ��Ϳ�����
//					newArr[targetI++] = beSub.binArr[j];
//					newLength++;
//					count++;
//				}
//			}
//			if (arrMax == beSub.length) {
//				for (int j = thisMax; j < length; j++) {//�������ȫ������,��ʣ�µ��ճ��Ϳ�����
//					newArr[targetI++] = binArr[j];
//					newLength++;
//					count++;
//				}
//			}
//		}
//		binArr = newArr;
//		length = newLength;
//	}
//
//};
//
////��������
//const int singleInput = 100000;
////��ÿ�����ݶ���elMatrix��
//void inputData(BinArray* elMatrix, int elMatrixLength,char* line) {
//	if (line[0] == 0)
//		return;
//	stringstream ss(line);//��ȡһ��
//	int* arr = new int[10000]{ -1 };//��ʱ����
//	int currentPos = 0;//�浽�ĸ�λ��
//	int currentSum = 0;//�ж��ٸ�length
//	while (ss >> arr[currentPos++]) {
//		currentSum++;
//	}
//	//ĿǰΪֹ,��������һ������
//	BinArray temp(arr, currentSum);
//	elMatrix[elMatrixLength] = temp;
//}
//string txt_0 = "C:\\����\\�����\\���г������\\GroData\\7_8399_6375_4535\\0.txt";
//string txt_1 = "C:\\����\\�����\\���г������\\GroData\\7_8399_6375_4535\\1.txt";
//string txt_2 = "C:\\����\\�����\\���г������\\GroData\\7_8399_6375_4535\\2.txt";
//string txt_res = "C:\\����\\�����\\���г������\\GroData\\7_8399_6375_4535\\myResult.txt";
//int main() {
//	LARGE_INTEGER t1, t2, tc;
//	double time0;
//	QueryPerformanceFrequency(&tc);
//	QueryPerformanceCounter(&t1);
//
//	bool whichFile = true;//�м����з�����2����ͬ�ļ��ж�ȡ��Ԫ��,����true�����1.txt,false�����0�ж�ȡ
//	ifstream Eliminated_line_file(txt_2);//��ȡ����Ԫ������
//	if (!Eliminated_line_file) {
//		cout << "Unable to open Eliminated_line_file";
//		exit(1); // terminate with error  
//
//	}
//	while (!Eliminated_line_file.eof()) {
//		// ���뱻��Ԫ��//////////////////////////////////////////////////////////////////////////////////////////////////
//		BinArray* elMatrix = new BinArray[10];//����Ԫ�еĶ�ά����
//		int elMatrixLength = 0;//����Ԫ���ж�����(��������10,����β�����ܲ���10)
//		for (elMatrixLength; elMatrixLength < 10 && !Eliminated_line_file.eof(); elMatrixLength++) {//ÿ�ζ�10������Ԫ��
//			char line[singleInput];
//			Eliminated_line_file.getline(line, singleInput);
//			if (line[0] != 0)
//				inputData(elMatrix, elMatrixLength, line);
//			else
//				break;
//		}
//		// ������Ԫ��//////////////////////////////////////////////////////////////////////////////////////////////////
//		ifstream Eliminator_file;
//		if (whichFile == true)
//			Eliminator_file.open(txt_1);//��ȡ��Ԫ������1
//		else
//			Eliminator_file.open(txt_0);//��ȡ��Ԫ������0
//		if (!Eliminator_file) {
//			cout << "Unable to open Eliminator_file";
//			exit(1); // terminate with error  
//		}
//		while (!Eliminator_file.eof()) {
//			BinArray* eMatrix = new BinArray[20];//��Ԫ�ӵĶ�ά����
//			int eMatrixLength = 0;//��Ԫ���ж�����(��������20,����β�����ܲ���20)
//			for (eMatrixLength; eMatrixLength < 10 && !Eliminator_file.eof(); eMatrixLength++) {//ÿ�ζ�10������Ԫ��
//				char line[singleInput];
//				Eliminator_file.getline(line, singleInput);
//				if (line[0] != 0)
//					inputData(eMatrix, eMatrixLength, line);
//				else
//					break;
//			}
//			//���ֿ������뷶Χ,ע�������ָ���ǿ�������ɢ��ֵ,��Χָ������һ����Ԫ�Ӹ��ǵ����½�,��һ������
//			int* canSub = new int[singleInput];//������,�ص��ǿ�����O(1)��������Ƿ��������Ӧ��ȥԪ,���������˿ռ�
//			int upperBound = 0;//��ǰ������½�,����������
//			int lowerBound = 0;
//			for (int i = 0; i < eMatrixLength; i++) {
//				if (i == 0)
//					upperBound = eMatrix[i].binArr[0];
//				if (i == eMatrixLength-1)
//					lowerBound = eMatrix[i].binArr[0];
//				canSub[eMatrix[i].binArr[0]] = 1;//canSub[x] = 1�����xΪ����ȥԪ
//
//			}
//			//��ʼ��ÿ������Ԫ�н��м���
//			for (int currentLine = 0; currentLine < elMatrixLength; currentLine++) {
//				//����ڿ�������,ֱ�����
//				int item = elMatrix[currentLine].binArr[0];
//				if (canSub[item] == 1) {//��һ����ȷ������Ԫ�������ڲ��ڵ�ǰ����Ԫ�ӿ�������
//					//�ڵĻ�,�ҵ��Ǹ���Ԫ�Ӳ����
//					for (int k = 0; k < eMatrixLength; k++) {
//						if (eMatrix[k].binArr[0] == item) {
//							elMatrix[currentLine].xorArr(eMatrix[k]);
//							if (elMatrix[currentLine].length == 0) {//���ɾ���Ժ�Ϊ����
//								for (int a = currentLine; a < elMatrixLength; a++) {
//									elMatrix[a] = elMatrix[a + 1];
//								}
//								elMatrixLength--;
//							}
//							break;
//						}
//					}
//					currentLine--;
//					continue;//�ٴ�ɨ���������Ԫ��,��Ϊһ������Ԫ�п��ܱ������Ԫ����Ԫ
//				}
//				//������ڿ�������,�����Ƿ��ǿ���
//				if (elMatrix[currentLine].length == 0)
//					continue;//����
//				//����ڷ�Χ��,��ʱֻ��һ�ֿ���,�����Ա���������Ԫ��,�ر��,����ֻ��Ҫ��֤�½缴��,��Ϊ�����б������м�ı���Ԫ��
//				if (item >= lowerBound) {
//					//����,�������ı���Ԫ����������Ҫ�Ľ�������һ����,����ֱ�Ӵ��ȥ
//					ofstream outfile;
//					outfile.open(txt_res, ios::app);
//					for (int k = 0; k < elMatrix[currentLine].length; k++) {
//						outfile << elMatrix[currentLine].binArr[k] << " ";
//					}
//					outfile << endl;
//					outfile.close();
//					//�ڶ���,������Ԫ��������,�����¿����������
//						//���ж�Ӧ�ô�������,���ƻ�����˳����
//					for (int k = 0; k < eMatrixLength; k++) {
//						if (item > upperBound) {//����ȵ�һ����Ԫ�ӻ���,��ô���ڵ�һ��
//							for (int p = eMatrixLength; p >= 1; p--) {
//								eMatrix[p] = eMatrix[p - 1];
//							}
//							eMatrix[0] = elMatrix[currentLine];
//							eMatrixLength++;
//							upperBound = item;
//							canSub[item] = 1;
//						}
//						if (item > lowerBound && item < upperBound) {//�����м�,���ҵ���Ӧ��λ��
//							int p = eMatrixLength-1;
//							for (p; p >= 1; p--) {
//								if (item > eMatrix[p].binArr[0] && item < eMatrix[p - 1].binArr[0]) {//˵��������p��p-1֮��
//									for (int l = eMatrixLength; l >= p+1; l--) {
//										eMatrix[l] = eMatrix[l - 1];
//									}
//									eMatrix[p] = elMatrix[currentLine];
//									eMatrixLength++;
//									canSub[item] = 1;
//									break;
//								}
//							}
//						}
//					}
//						//������,�ӱ���Ԫ����ɾ��
//					for (int k = currentLine; k < elMatrixLength - 1; k++) {
//						elMatrix[k] = elMatrix[k + 1];
//					}
//					elMatrixLength--;
//						//���Ĳ�,����ɾ������һ��,����currentLine--
//					currentLine--;
//				}
//				//////////////////////////////////////////////////////////////////////////////////////////////////////////
//			}
//		//�����µ���Ԫ�Ӵ�����һ����Ԫ���ļ���
//			ofstream anotherEfile;
//			if(whichFile==true)
//				anotherEfile.open(txt_0, ios::app);
//			else
//				anotherEfile.open(txt_1, ios::app);
//			for (int k = 0; k <eMatrixLength; k++) {
//				for (int l = 0; l < eMatrix[k].length; l++) {
//					anotherEfile << eMatrix[k].binArr[l] << " ";
//				}
//				anotherEfile << endl;
//			}
//		}
//		//��������Ԫ�Ӵ������Ժ�,�������Ԫ�о�����Ԫ��,��Ҳ���뵽��Ԫ��������,��ʱһ���Ǳ������Ԫ�л�ҪС��Ԫ��
//		ofstream anotherEfile;
//		if (whichFile == true)
//			anotherEfile.open(txt_0, ios::app);
//		else
//			anotherEfile.open(txt_1, ios::app);
//		for (int k = 0; k < elMatrixLength; k++) {
//			for (int l = 0; l < elMatrix[k].length; l++) {
//				anotherEfile << elMatrix[k].binArr[l] << " ";
//			}
//			anotherEfile << endl;
//		}
//
//		//��ǰ��Ԫ�Ӵ�������,�����ǰ��Ԫ���ı�����,���Ҹı���Ԫ��Դ�ļ�ָ��
//		Eliminator_file.close();
//		ofstream emptyFile;
//		if (whichFile == true) {
//			emptyFile.open(txt_1, ios::trunc);
//			whichFile = false;
//		}
//		else {
//			emptyFile.open(txt_0, ios::trunc);
//			whichFile = true;
//		}
//
//	}
//	QueryPerformanceCounter(&t2);
//	std::cout << "time = " << (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart * 1000 << "ms" << std::endl;
//}