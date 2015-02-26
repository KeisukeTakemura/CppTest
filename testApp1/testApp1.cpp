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

//関数プロトタイプ宣言
void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);


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

//-------------ここまでファイル読み込み
	//koui
	Object* state = new Object[gStageWidth * gStageHeight]; //状態配列の確保
	
	initialize(state, gStageWidth, gStageHeight, fileImage); //ステージの初期化

	//main Loop
	while (true){
		//draw
		draw(state, gStageWidth, gStageHeight);
		//check clear
		if (checkClear(state, gStageWidth, gStageHeight)){
			break;
		}

		//入力取得
		cout << "a:left s:right w:up z:down. command?" << endl;
		char input;
		cin >> input;

		//更新
		update(state, input, gStageWidth, gStageHeight);
	}

	//賞賛の言葉
	cout << "Congraturation! you win." << endl;
	//後始末
	delete[] state;	//配列に対してのnewであったのでdelete[]
	state = 0;
}


//以下関数定義
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
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //改行処理
		default: t = OBJ_UNKNOWN; break;
		}
		++d;

		if (t != OBJ_UNKNOWN){
			state[y*width + x] = t; //書き込み
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

//人座標を検索
	int i = -1;
	for (i = 0; i < w*h; ++i){
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL){
			break;
		}
	}
//座標確定
	int x = i % w;
	int y = i / w;

//移動
	int new_x = x + dx;
	int new_y = y + dy;

	if (new_x < 0 || new_y < 0 || new_x >= w || new_y >= h){
		return;
	}

	int p = y*w + x;
	int new_p = new_y*w + new_x;

	if (s[new_p] == OBJ_SPACE || s[new_p] == OBJ_GOAL){
		s[new_p] = (s[new_p] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //ゴール上ならゴールの人へ変換
		s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //いたとこがゴールならただのゴールへ
	} else if (s[new_p] == OBJ_BLOCK || s[new_p] == OBJ_BLOCK_ON_GOAL){
		//2マス先が進めるかどうか判定する
		int new_x2 = new_x + dx;
		int new_y2 = new_y + dy;
		if (new_x2 < 0 || new_y2 < 0 || new_x2 >= w || new_y2 >= h){
			return;
		}

		int new_p2 = (new_y + dy)*w + (new_x + dx); //2マス先
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
