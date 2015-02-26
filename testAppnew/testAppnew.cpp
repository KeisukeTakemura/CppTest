// testAppnew.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<fstream>

using namespace std;

//関数プロトタイプ
void readFile(char** buffer, int* size, const char* filename);

int main(){
	ifstream inputFile("stageData.txt", fstream::binary);
	inputFile.seekg(0, ifstream::end);
//最後までいったので現在の位置＝ファイルサイズ
	int fileSize = static_cast<int>(inputFile.tellg());
	inputFile.seekg(0, ifstream::beg);	//retrun first point

	char* fileImage = new char[fileSize];
	inputFile.read(fileImage, fileSize);

	cout.write(fileImage, fileSize);
	while (true){}

}