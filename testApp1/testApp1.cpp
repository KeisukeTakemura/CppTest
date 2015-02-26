#include"stdafx.h"
#include<iostream>
#include<fstream>

using namespace std;
const char gStageData[] ="\
	########\n\
	# .. p #\n\
	# oo   #\n\
	#      #\n\
	########";

const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN
};

//�֐��v���g�^�C�v�錾
void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);


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

//-------------�����܂Ńt�@�C���ǂݍ���
	//koui
	Object* state = new Object[gStageWidth * gStageHeight]; //��Ԕz��̊m��
	
	initialize(state, gStageWidth, gStageHeight, fileImage); //�X�e�[�W�̏�����

	//main Loop
	while (true){
		//draw
		draw(state, gStageWidth, gStageHeight);
		//check clear
		if (checkClear(state, gStageWidth, gStageHeight)){
			break;
		}

		//���͎擾
		cout << "a:left s:right w:up z:down. command?" << endl;
		char input;
		cin >> input;

		//�X�V
		update(state, input, gStageWidth, gStageHeight);
	}

	//�܎^�̌��t
	cout << "Congraturation! you win." << endl;
	//��n��
	delete[] state;	//�z��ɑ΂��Ă�new�ł������̂�delete[]
	state = 0;
}


//�ȉ��֐���`
void initialize(Object* state, int width ,int height, const char* stageData){
	const char* d = gStageData; //read pointer
	int x = 0;
	int y = 0;

	while (*d != '\0'){  //untill input NULL 
		Object t;
		switch (*d){
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //���s����
		default: t = OBJ_UNKNOWN; break;
		}
		++d;

		if (t != OBJ_UNKNOWN){
			state[y*width + x] = t; //��������
			x++;
		}

	}
}

//draw
void draw(const Object* state, int width, int height){
	const char font[] = { ' ', '#', 'o', 'O', 'p', 'P' }; //Object enum 
	
	for (int y = 0; y < height; ++y){
		for (int x = 0; x < width; ++x){
			Object o = state[y*width + x];
			switch (o){
			case OBJ_WALL: cout << '#'; break;
			case OBJ_SPACE: cout << ' '; break;
			case OBJ_BLOCK: cout << 'o'; break;
			case OBJ_BLOCK_ON_GOAL: cout << 'O'; break;
			case OBJ_GOAL: cout << '.'; break;
			case OBJ_MAN: cout << 'p';  break;
			case OBJ_MAN_ON_GOAL: cout << 'P'; break;
			}
		}	cout << endl;
	}
}


//update
void update(Object* s, char input, int w, int h){
	int dx = 0;
	int dy = 0;

	switch (input){
		case 'a': dx = -1; break;
		case 's': dx = 1; break;
		case 'w': dy = -1; break;
		case 'z': dy = 1; break;
	}

//�l���W������
	int i = -1;
	for (i = 0; i < w*h; ++i){
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL){
			break;
		}
	}
//���W�m��
	int x = i % w;
	int y = i / w;

//�ړ�
	int new_x = x + dx;
	int new_y = y + dy;

	if (new_x < 0 || new_y < 0 || new_x >= w || new_y >= h){
		return;
	}

	int p = y*w + x;
	int new_p = new_y*w + new_x;

	if (s[new_p] == OBJ_SPACE || s[new_p] == OBJ_GOAL){
		s[new_p] = (s[new_p] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //�S�[����Ȃ�S�[���̐l�֕ϊ�
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //�����Ƃ����S�[���Ȃ炽���̃S�[����
	} else if (s[new_p] == OBJ_BLOCK || s[new_p] == OBJ_BLOCK_ON_GOAL){
		//2�}�X�悪�i�߂邩�ǂ������肷��
		int new_x2 = new_x + dx;
		int new_y2 = new_y + dy;
		if (new_x2 < 0 || new_y2 < 0 || new_x2 >= w || new_y2 >= h){
			return;
		}

		int new_p2 = (new_y + dy)*w + (new_x + dx); //2�}�X��
		if (s[new_p2] == OBJ_SPACE || s[new_p2] == OBJ_GOAL){
			//change
			s[new_p2] = (s[new_p2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[new_p] = (s[new_p] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}


//clearcheck
bool checkClear(const Object* s, int w, int h){
	for (int i = 0; i < w*h; ++i){
		if (s[i] == OBJ_BLOCK){
			return false;
		}
	}
	return true;
}
