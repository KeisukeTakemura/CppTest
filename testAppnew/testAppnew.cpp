// testAppnew.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<fstream>

using namespace std;

//�֐��v���g�^�C�v
void readFile(char** buffer, int* size, const char* filename);

int main(){
	ifstream inputFile("stageData.txt", fstream::binary);
	inputFile.seekg(0, ifstream::end);
//�Ō�܂ł������̂Ō��݂̈ʒu���t�@�C���T�C�Y
	int fileSize = static_cast<int>(inputFile.tellg());
	inputFile.seekg(0, ifstream::beg);	//retrun first point

	char* fileImage = new char[fileSize];
	inputFile.read(fileImage, fileSize);

	cout.write(fileImage, fileSize);
	while (true){}

}