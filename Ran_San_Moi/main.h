#pragma once

#include<thread> // chạy chữ liên tục
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<ctime>
#include<conio.h>
#include<Windows.h> 
#include<string.h>
#include"resource.h"
using namespace std;

#pragma comment(lib, "winmm.lib")
#define AmThanh(fileName) (PlaySound(MAKEINTRESOURCE(fileName), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC ));
#define AmThanhnen(fileName) (PlaySound(MAKEINTRESOURCE(fileName), GetModuleHandle(NULL),SND_RESOURCE | SND_LOOP | SND_ASYNC ));

#define ConsoleWidth 100   // chiều rộng của màn hình console
#define ConsoleHeight 30  // chiều dài của màn hình console

#define ChieuRong_Khung 70   // chiều rộng của khung chơi game
#define ChieuDai_Khung 22   // chiều dài của khung chơi game


struct huongdi {
	bool Ben_Trai;
	bool Ben_Phai;
	bool Huong_Len;
	bool Huong_Xuong;
};
typedef struct huongdi HUONGDI;

// khai báo cấu trúc của 1 con rắn
struct dauran {
	short sVT_Dong;
	short sVT_Cot;
	HUONGDI huong_di;
};
typedef struct dauran DAURAN;

struct thanran {
	short sVT_Dong;
	short sVT_Cot;
};
typedef struct thanran THANRAN;

struct conran {
	DAURAN dau_Ran;  // đầu rắn chỉ có một ô
	vector<THANRAN> than_Ran; // thân rắn thì có nhiều ô
};
typedef struct conran CONRAN;

//khai báo kiểu dữ liểu để lưu điểm và tên của 1 người chơi
struct LuuDiem {
	short sSTT;
	string sTen_NguoiChoi;
	short sDiem;
};
typedef struct LuuDiem LUUDIEM;