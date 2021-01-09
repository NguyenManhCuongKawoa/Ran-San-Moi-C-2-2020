#include"main.h"
#include"ham.h"
#include"Console.h"


// Ham an hien con tro.
void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
// Hàm xóa màn hình.
void XoaManHinh()
{
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

fstream fileLuuDiem;
bool DoAn[31][112];
CONRAN Con_Ran;
short sTrang, sVT_Chon = 1;
short sDiemSo = 0, sCapDo = 1;
// nếu sCapDo = 1 là cấp độ dễ
// nếu sCapDo = 2 là cấp độ khó
short sChuChay = 5;


// TRANG 1: TRANG MENU CHÍNH
// TRANG 2: TRANG CHƠI GAME
// TRANG 3: TRANG MENU THUA
// TRANG 4: TRANG MENU THANG //chưa có cập nhật chế độ thắng
// TRANG 5: TRANG TẠM DỪNG
// TRANG 6: TRANG THÔNG TIN
// TRANG 7: TRANG CẤP ĐỘ
// TRANG 8: TRANG LƯU ĐIỂM
// TRANG 9: TRANG XEM ĐIỂM ĐÃ LƯU
// TRANG 10: TRANG CHỌN NHẠC NỀN

void KhoiTaoGame()
{
	sTrang = 2;  // trang chơi game
	KhoiTaoViTriConRanBanDau();
	KhoiTaoDoAnBanDau();
	
	//mới vào game thì cứ chạy
    ChayLienTuc();
}

void KhoiTaoViTriConRanBanDau()
{
	//xuất phát từ vị trí có tọa đọ {0, 0}
	Con_Ran.dau_Ran.sVT_Dong = 1;
	Con_Ran.dau_Ran.sVT_Cot = 2;
	//khởi tạo hướng đi ban đầu
	Con_Ran.dau_Ran.huong_di.Ben_Phai = true;
	Con_Ran.dau_Ran.huong_di.Ben_Trai = false;
	Con_Ran.dau_Ran.huong_di.Huong_Len = false;
	Con_Ran.dau_Ran.huong_di.Huong_Xuong = false;
}
void KhoiTaoDoAnBanDau()
{
	for (short i = 0; i < ChieuDai_Khung; i++)
	{
		for (short j = 0; j < ChieuRong_Khung; j++)
    		DoAn[i][j] = false; // vị trí hàng i, côt j chưa có đồ ăn
	}
	srand(time(NULL));
	for (short i = 0; i < 3; i++)
		TaoDoAnNgauNhien();
}
void TaoDoAnNgauNhien()
{
	short sDong, sCot;
	sDong = rand() % (ChieuDai_Khung - 2) + 1;
	sCot = rand() % (ChieuRong_Khung - 4) + 2;
	DoAn[sDong][sCot] = true; 
}

void Ve_Bang()
{
	XoaManHinh();
	for (short i = 0; i < ChieuDai_Khung; i++)
	{
		for (int j = 0; j < ChieuRong_Khung; j++)
		{
			if (i == 0 || i == ChieuDai_Khung - 1)
                printf("%c", 219);
			else if (j == ChieuRong_Khung - 1 || j == 0 || j == ChieuRong_Khung - 2 || j == 1)
				printf("%c", 219);
			else Ve_O(i, j);
		}
		if(i< ChieuDai_Khung - 1) cout << endl;
	}
	VeBangDiemSo(sCapDo);
}

void Ve_O(short sVT_Dong, short sVT_Cot)
{
	 // giữ nút atl+ mã trong đó :█ 219, ░ 176
	if (DoAn[sVT_Dong][sVT_Cot] == true) // ô đồ ăn
	{
		printf("*");
	}
	else if (sVT_Dong == Con_Ran.dau_Ran.sVT_Dong && sVT_Cot == Con_Ran.dau_Ran.sVT_Cot) // ô đầu rắn
	{
		//cout << (char)219;
		printf("%c", 219);
	}
	else // 1 là ô trống 2 là ô thân rắn
	{
		if (!Con_Ran.than_Ran.empty()) // có thân rắn
		{
			bool lock = true;
			for (short i = 0; i < Con_Ran.than_Ran.size(); i++)
			{
				if ((sVT_Dong == Con_Ran.than_Ran[i].sVT_Dong) && (sVT_Cot == Con_Ran.than_Ran[i].sVT_Cot)) // là thân rắn
				{
					//cout << (char)176;
					printf("%c", 176);
					lock = false;
					break;
				}
			}
			if (lock) printf(" ");
		}
		else printf(" ");
	}
}

// hàm rắn ăn đồ ăn
void Ran_An_DoAn(short x, short y)
{
	if (DoAn[x][y] == true) // đầu đã chạm đồ ăn
	{
		DoAn[x][y] = false; //đã đc ăn
		TaoDoAnNgauNhien();
		sDiemSo++;

		THANRAN k; // khai báo 1 cái thân của con rắn
		k.sVT_Dong = x;
		k.sVT_Cot = y;
		Con_Ran.than_Ran.push_back(k); // thêm 1 thân rắn vào con rắn

		AmThanh(IDR_WAVE2);
	}
}
// cập nhật lại vị trí con rắn gồm thân và đầu
void CapNhatLaiViTri_ThanRan()
{
	if (!Con_Ran.than_Ran.empty())
	{
		for (short i = Con_Ran.than_Ran.size() - 1; i > 0; i--)
		{
			Con_Ran.than_Ran[i].sVT_Dong = Con_Ran.than_Ran[i - 1].sVT_Dong;
			Con_Ran.than_Ran[i].sVT_Cot = Con_Ran.than_Ran[i - 1].sVT_Cot;
		}
		Con_Ran.than_Ran[0].sVT_Dong = Con_Ran.dau_Ran.sVT_Dong;
		Con_Ran.than_Ran[0].sVT_Cot = Con_Ran.dau_Ran.sVT_Cot;
	}
}
// kiểm tra rắn. nếu đầu rắn chạm thân rắn thì kết thúc trò chơi 
bool KT_ConRanDaChamVaoThanNoChua()
{
	if (!Con_Ran.than_Ran.empty())
	{
		for (short i = Con_Ran.than_Ran.size() - 1; i >= 0; i--)
		{
			if (Con_Ran.than_Ran[i].sVT_Dong == Con_Ran.dau_Ran.sVT_Dong && Con_Ran.than_Ran[i].sVT_Cot == Con_Ran.dau_Ran.sVT_Cot)
			{
				sTrang = 3; // trang thua
				VeMenuThua(1);
				AmThanh(IDR_WAVE1);
				return 1;
			}
		}
		return 0;
	}
	else return 0;
}

// vẽ bảng chạy liên tục
void ChayLienTuc()
{
	if (Con_Ran.dau_Ran.huong_di.Ben_Phai == true) // con rắn đang hướng sang bên phải
	{
		while (sTrang == 2) // đang ở trang thái chơi game
 		{
			CapNhatLaiViTri_ThanRan();

		    Con_Ran.dau_Ran.sVT_Cot++;
			if (Con_Ran.dau_Ran.sVT_Cot == ChieuRong_Khung - 2) 
			{
				if(sCapDo == 1) Con_Ran.dau_Ran.sVT_Cot = 2;
				else { // chế độ khó
					sTrang = 3; // trang thua
					VeMenuThua(1);
					AmThanh(IDR_WAVE1);
					return;
				}
			}
			if(KT_ConRanDaChamVaoThanNoChua()) return;
			Ran_An_DoAn(Con_Ran.dau_Ran.sVT_Dong, Con_Ran.dau_Ran.sVT_Cot);
			
			Ve_Bang();
			if (_kbhit()) break; // nếu có phím bấm vào thì thoát và phím đó sẽ đc lưu
		}
	}
	else if (Con_Ran.dau_Ran.huong_di.Ben_Trai == true) // con rắn đang hướng sang bên trái
	{
		while (sTrang == 2) // dang ở trang thái chơi game
		{
			CapNhatLaiViTri_ThanRan();
            
		    Con_Ran.dau_Ran.sVT_Cot--;
			if (Con_Ran.dau_Ran.sVT_Cot < 2)
			{
				if(sCapDo == 1) Con_Ran.dau_Ran.sVT_Cot = ChieuRong_Khung - 3;
				else {
					sTrang = 3; // trang thua
					VeMenuThua(1);
					AmThanh(IDR_WAVE1);
					return;
				}
			}
			if (KT_ConRanDaChamVaoThanNoChua()) return;
			Ran_An_DoAn(Con_Ran.dau_Ran.sVT_Dong, Con_Ran.dau_Ran.sVT_Cot);
		
			Ve_Bang();
			if (_kbhit()) break; // nếu có phím bấm vào thì thoát và phím đó sẽ đc lưu
		}
	}
	else if (Con_Ran.dau_Ran.huong_di.Huong_Len == true) // con rắn đang hướng lên
	{
		while (sTrang == 2)  // đang ở trạng thái chơi game
		{
			CapNhatLaiViTri_ThanRan();

		    Con_Ran.dau_Ran.sVT_Dong--;
			if (Con_Ran.dau_Ran.sVT_Dong < 1)
			{
				if (sCapDo == 1) Con_Ran.dau_Ran.sVT_Dong = ChieuDai_Khung - 2;
				else {
					sTrang = 3; // trang thua
					VeMenuThua(1);
					AmThanh(IDR_WAVE1);
					return;
				}
			}
			if (KT_ConRanDaChamVaoThanNoChua()) return;
			Ran_An_DoAn(Con_Ran.dau_Ran.sVT_Dong, Con_Ran.dau_Ran.sVT_Cot);
			
			Ve_Bang();
			if (_kbhit()) break; // nếu có phím bấm vào thì thoát và phím đó sẽ đc lưu
		}
	}
	else  // con rắn đang hướng xuống
	{
		while (sTrang == 2) // đang ở trạng thái chơi game
		{
			CapNhatLaiViTri_ThanRan();

			Con_Ran.dau_Ran.sVT_Dong++;
			if (Con_Ran.dau_Ran.sVT_Dong == ChieuDai_Khung - 1)
			{
				if(sCapDo == 1) Con_Ran.dau_Ran.sVT_Dong = 1;
				else
				{
					sTrang = 3; // trang thua
					VeMenuThua(1);
					AmThanh(IDR_WAVE1);
					return;
				}
	    	}
			if (KT_ConRanDaChamVaoThanNoChua()) return;
			Ran_An_DoAn(Con_Ran.dau_Ran.sVT_Dong, Con_Ran.dau_Ran.sVT_Cot);

			 Ve_Bang();
			if (_kbhit()) break; // nếu có phím bấm vào thì thoát và phím đó sẽ đc lưu
		}
	}
}

//xử lý sự kiện
void Xu_Li_Phim(KEY_EVENT_RECORD key)
{
	if (key.bKeyDown) { //co nhan phim
		switch (key.wVirtualKeyCode) 
		{
		    case VK_UP: //MUI TEN LEN
				if (sTrang == 1) // đang ở menu chính
				{
					sVT_Chon--;
					if (sVT_Chon < 1) sVT_Chon = 6;
					VeMenuChinh(sVT_Chon);
				}
				else if (sTrang == 2)  // đang chơi game
				{
					if (Con_Ran.dau_Ran.huong_di.Ben_Trai == true)
					{
						Con_Ran.dau_Ran.huong_di.Ben_Trai = false;
						Con_Ran.dau_Ran.huong_di.Huong_Len = true;
						ChayLienTuc();
					}
					else if (Con_Ran.dau_Ran.huong_di.Ben_Phai == true)
					{
						Con_Ran.dau_Ran.huong_di.Ben_Phai = false;
						Con_Ran.dau_Ran.huong_di.Huong_Len = true;
						ChayLienTuc();
					}
					else ChayLienTuc(); // nếu khác các phím kia thì không thay đỏi hướng
				}
				else if (sTrang == 3) // trang thua
				{
					sVT_Chon--;
					if (sVT_Chon < 1) sVT_Chon = 3;
					VeMenuThua(sVT_Chon);
				}
				else if (sTrang == 5) // trang TẠM DỪNG
				{
					sVT_Chon == 1 ? sVT_Chon = 2 : sVT_Chon = 1;
					VeMenuTamDung(sVT_Chon);
				}
				else if (sTrang == 7) // trang chọn cấp độ
				{
					sVT_Chon == 1 ? sVT_Chon = 2 : sVT_Chon = 1;
					VeTrangCapDo(sVT_Chon);
				}
				else if (sTrang == 10) // trang chọn nhạc
				{
					sVT_Chon--;
					if (sVT_Chon < 1) sVT_Chon = 4;
					TrangChonNhac(sVT_Chon);
				}
				else;

			     break;
			case VK_DOWN: //MUI TEN XUONG
				if (sTrang == 1) // đang ở menu chính
				{
					sVT_Chon++;
					if (sVT_Chon > 6) sVT_Chon = 1;
					VeMenuChinh(sVT_Chon);
				}
				else if (sTrang == 2)  // đang chơi game
				{
					if (Con_Ran.dau_Ran.huong_di.Ben_Trai == true)
					{
						Con_Ran.dau_Ran.huong_di.Ben_Trai = false;
						Con_Ran.dau_Ran.huong_di.Huong_Xuong = true;
						ChayLienTuc();
					}
					else if (Con_Ran.dau_Ran.huong_di.Ben_Phai == true)
					{
						Con_Ran.dau_Ran.huong_di.Ben_Phai = false;
						Con_Ran.dau_Ran.huong_di.Huong_Xuong = true;
						ChayLienTuc();
					}
					else ChayLienTuc(); // nếu khác các phím kia thì không thay đỏi hướng
				}
				else if (sTrang == 3) // trang thua
				{
					sVT_Chon++;
					if (sVT_Chon > 3) sVT_Chon = 1;
					VeMenuThua(sVT_Chon);
				}
				else if (sTrang == 5) // trang TẠM DỪNG
				{
					sVT_Chon == 1 ? sVT_Chon = 2 : sVT_Chon = 1;
					VeMenuTamDung(sVT_Chon);
				}
				else if (sTrang == 7) // trang chọn cấp độ
				{
					sVT_Chon == 1 ? sVT_Chon = 2 : sVT_Chon = 1;
					VeTrangCapDo(sVT_Chon);
				}
				else if (sTrang == 10) // trang chọn nhạc
				{
					sVT_Chon++;
					if (sVT_Chon > 4) sVT_Chon = 1;
					TrangChonNhac(sVT_Chon);
				}
				break;
			case VK_LEFT: //MUI TEN TRAI
			    if (sTrang == 2)  // đang chơi game
				{
					if (Con_Ran.dau_Ran.huong_di.Huong_Len == true)
					{
						Con_Ran.dau_Ran.huong_di.Huong_Len = false;
						Con_Ran.dau_Ran.huong_di.Ben_Trai = true;
						ChayLienTuc();
					}
					else if (Con_Ran.dau_Ran.huong_di.Huong_Xuong == true)
					{
						Con_Ran.dau_Ran.huong_di.Huong_Xuong = false;
						Con_Ran.dau_Ran.huong_di.Ben_Trai = true;
						ChayLienTuc();
					}
					else ChayLienTuc(); // nếu khác các phím kia thì không thay đỏi hướng
				}

				
				break;
			case VK_RIGHT: //MUI TEN PHAI
				 if (sTrang == 2)  // đang chơi game
				 {
					if (Con_Ran.dau_Ran.huong_di.Huong_Len == true)
					{
						Con_Ran.dau_Ran.huong_di.Huong_Len = false;
						Con_Ran.dau_Ran.huong_di.Ben_Phai = true;
						ChayLienTuc();
					}
					else if (Con_Ran.dau_Ran.huong_di.Huong_Xuong == true)
					{
						Con_Ran.dau_Ran.huong_di.Huong_Xuong = false;
						Con_Ran.dau_Ran.huong_di.Ben_Phai = true;
						ChayLienTuc();
					}
					else ChayLienTuc(); // nếu khác các phím kia thì không thay đỏi hướng
				 }

		
				break;
			case VK_ESCAPE: // phím ESC
				AmThanh(IDR_WAVE3);
				if (sTrang == 1) // đang ở menu chính
					exit(0);
				else if (sTrang == 2)  // đang chơi game
				{
					sVT_Chon = 1;
					VeMenuTamDung(sVT_Chon);
				}
				else  // trang thua , tạm dừng, thông tin, chon cấp độ, lưu điểm, xem điểm đề về màn hình chính
				{
					AmThanhnen(IDR_WAVE4);
					sVT_Chon = 1;
					VeMenuChinh(sVT_Chon);
				}
				break;
			case VK_RETURN:  // phím ENTER
				AmThanh(IDR_WAVE3);
				if (sTrang == 1) // đang ở menu chính
				{
					if (sVT_Chon == 1)  // vô game mới
					{
						deleteRow(0, ConsoleHeight);
						Con_Ran.than_Ran.clear(); // xóa tất cả thân rắn
						sDiemSo = 0;
						KhoiTaoGame();
					}
					else if (sVT_Chon == 2) // vô trang thông tin
					{
						VeTrangThongTin();
					}
					else if (sVT_Chon == 3) // vô trang chọn mức độ chơi
					{
						sVT_Chon = 1;
						VeTrangCapDo(sVT_Chon);
					}
					else if (sVT_Chon == 4) // vô trang XEM ĐIỂM
					{
						XemDiemDaLuu();
					}
					else if (sVT_Chon == 5) // vô trang CHỌN NHẠC
					{
						sVT_Chon = 1;
						TrangChonNhac(sVT_Chon);
					}
					else  // chọn thoát
					{
						exit(0);
					}
				}
				else if (sTrang == 3)  // trang thua
				{
					if (sVT_Chon == 1)  // vô game mới
					{
						Con_Ran.than_Ran.clear(); // xóa tất cả thân rắn
						sDiemSo = 0;
						KhoiTaoGame();
					}
					else if (sVT_Chon == 2)  // lưu game
					{
						TrangLuuDiem();
					}
					else  // chọn về menu chính
					{
						AmThanhnen(IDR_WAVE4);
						sVT_Chon = 1;
						VeMenuChinh(sVT_Chon);
					}
				}
				else if (sTrang == 5) // trang TẠM DỪNG
				{
					if (sVT_Chon == 1)  // tiếp tục chơi
					{
						deleteRow(0, ConsoleHeight);
						sTrang = 2;
						ChayLienTuc();
					}
					else  // chọn về menu chính
					{
						AmThanhnen(IDR_WAVE4);
						sVT_Chon = 1;
						VeMenuChinh(sVT_Chon);
					}
				}
				else if (sTrang == 7) // trang chọn cấp độ
				{
					if (sVT_Chon == 1)  // chọn mức độ dễ
					{
						AmThanhnen(IDR_WAVE4);
						sVT_Chon = 1;
						sCapDo = 1;
						VeMenuChinh(sVT_Chon);
					}
					else if (sVT_Chon == 2)  // chọn mức độ khó
					{
						sVT_Chon = 1;
						AmThanhnen(IDR_WAVE4);
						sCapDo = 2;
						VeMenuChinh(sVT_Chon);
					}
				}
				else if (sTrang == 10) // trang chọn nhạc
				{
					 
					if (sVT_Chon == 1)  // chọn 1 triệu khả năng
					{
						AmThanhnen(IDR_WAVE4);
						//TrangChonNhac(1);
					}
					else if (sVT_Chon == 2)  // chọn youaremycrush
					{
						AmThanhnen(IDR_WAVE5);
						//TrangChonNhac(2);
					}
					else if (sVT_Chon == 3) // chọn yêu 5
					{
						AmThanhnen(IDR_WAVE6);
						//TrangChonNhac(3);
					}
				    else //THOAT
				    {
						AmThanhnen(IDR_WAVE4);
						sVT_Chon = 1;
						VeMenuChinh(sVT_Chon);
				    }
				}
				break;
		}
	}
}

void Xu_Li_Su_Kien() {
	while (true) {
		DWORD DW_Number_OF_Enents = 0; //luu lai su kien hien tai
		DWORD DW_Number_OF_Enents_Read = 0; // luu lai su kien da dc lap

		HANDLE HConsoleInput = GetStdHandle(STD_INPUT_HANDLE);//THIET BI DAU VAO
		GetNumberOfConsoleInputEvents(HConsoleInput, &DW_Number_OF_Enents);//dat su kien dau vao cua gia dien cho bien DW_Number_OF_Enents

		if (DW_Number_OF_Enents) {
			INPUT_RECORD* IREventBuffer = new INPUT_RECORD[DW_Number_OF_Enents]; //khai bao con tro EventBuffer.
			ReadConsoleInput(HConsoleInput, IREventBuffer, DW_Number_OF_Enents, &DW_Number_OF_Enents_Read); // dat cac su kien dc luu vao con tro EventBuffe.

			//chay vong lap de doc du kien
			for (DWORD i = 0; i < DW_Number_OF_Enents; i++) {
				if (IREventBuffer[i].EventType == KEY_EVENT) { //NEU LA SU KIEN PHIM
					Xu_Li_Phim(IREventBuffer[i].Event.KeyEvent);
				}
				//else if (IREventBuffer[i].EventType == MOUSE_EVENT) { //NEU LA SU KIEN CHUOT

				//}
			}
		}
	}
}


// hàm MENU
void VeMenuChinh(short sVT_Chon)
{
	deleteRow(0, ConsoleHeight);
	char a[100];
	sTrang = 1; // cập nhật trang

	if (sVT_Chon == 1) //chọn NEW GAME
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 7, 7, 15, 0, a);
    }
	else if (sVT_Chon == 2) //chọn INFORMATION
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 7, 8, 15, 0, a);
	}
	else if (sVT_Chon == 3) //chọn LEVEL
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 7, 9, 15, 0, a);
	}
	else if (sVT_Chon == 4) // xem điểm đã lưu
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 7, 10, 15, 0, a);
	}
	else if (sVT_Chon == 5) // bật nhạc
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 7, 11, 15, 0, a);
	}
	else  //chọn EXIT
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 7, 12, 15, 0, a);
	}

	strcpy_s(a, " GAME SNAKE ");
	setColorBGTextXY((ConsoleWidth / 2) - strlen(a) / 2, 3, 12, 14, a);

	strcpy_s(a, "  NEW GAME ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 7, 15, 9, a);
	strcpy_s(a, "INFORMATION");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 8, 15, 9, a);
	strcpy_s(a, "   LEVEL   ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 9, 15, 9, a);
	strcpy_s(a, "CHECK POINT");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 10, 15, 9, a);
	strcpy_s(a, "   MUSIC   ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 11, 15, 9, a);
	strcpy_s(a, "   EXIT    ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 12, 15, 9, a);

	strcpy_s(a, "|=> Created by: <NGUYEN MANH CUONG> <=|");
	setColorBGTextXY(sChuChay, 25, 9, 0, a);
	setBackgroundColor(0);
}

// vẽ MENU Thua
void VeMenuThua(short sVT_Chon)
{
	deleteRow(0, ConsoleHeight);
	char a[100];
	sTrang = 3; // cập nhật trang

	if (sVT_Chon == 1) //chọn NEW GAME
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 10, 7, 15, 0, a);
	}
	else if (sVT_Chon == 2) //chọn lưu điểm
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 10, 8, 15, 0, a);
	}
	else  //chọn EXIT
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 10, 9, 15, 0, a);
	}

	strcpy_s(a, " => GAME OVER <= ");
	setColorBGTextXY((ConsoleWidth / 2) - strlen(a) / 2, 3, 12, 14, a);

	strcpy_s(a, "   PLAY AGAIN   ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 7, 15, 9, a);
	strcpy_s(a, " SAVE THE POINT ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 8, 15, 9, a);
	strcpy_s(a, "      EXIT      ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 9, 15, 9, a);

	strcpy_s(a, "|=> Created by: <NGUYEN MANH CUONG> <=|");
	setColorBGTextXY(sChuChay, 25, 9, 0, a);
	setBackgroundColor(0);
}

// MENU tạm dừng
void VeMenuTamDung(short sVT_Chon)
{
	deleteRow(0, ConsoleHeight);
	char a[100];
	sTrang = 5; // cập nhật trang

	if (sVT_Chon == 1) //chọn tiếp tục
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 8, 7, 15, 0, a);
	}
	else  //chọn EXIT
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 8, 8, 15, 0, a);
	}

	strcpy_s(a, " => GAME SNAKE <= ");
	setColorBGTextXY((ConsoleWidth / 2) - strlen(a) / 2, 3, 12, 14, a);

	strcpy_s(a, "  CONTINUE  ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 7, 15, 9, a);
	strcpy_s(a, "    EXIT    ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 8, 15, 9, a);

	strcpy_s(a, "|=> Created by: <NGUYEN MANH CUONG> <=|");
	setColorBGTextXY(sChuChay, 25, 9, 0, a);
	setBackgroundColor(0);
}

// vẽ trang thông tin
void VeTrangThongTin()
{
	deleteRow(0, ConsoleHeight);
	sTrang = 6; // cập nhật trang
	cout << "\n\t\t\t\t==> Created by: <NGUYEN MANH CUONG>\n";

	cout << "\n\n\n\t\t\t";
	for (short i = 0; i < 55; i++)
		cout << (char)176;
	cout << endl;
	cout << "\t\t\t" << (char)176 << "                   => GAME SNAKE <=                  " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "                                                     " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "  _Cach choi:                                        " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "       + Cac phim mui ten de di chuyen.              " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "       + Phim Enter de chon chuc nang.               " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "       + Phim Ese de thoat, dung choi, tro ve Menu.  " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "  _Cap Do:                                           " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "       + EYSY: Dam DAU vao tuong khong CHET          " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "       + DIFFICULT: Dam DAU vao tuong thi CHET       " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "  _ HAI CAP DO: Dam DAU vao THAN RAN thi deu CHET    " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "                                                     " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "  ==> VUI LONG NHAN ESC DE RA MAN HINH CHINH <==     " << (char)176 << endl <<"\t\t\t";
	for (short i = 0; i < 55; i++)
		cout << (char)176;
}

// vẽ trang cấp độ
void VeTrangCapDo(short sVT_Chon)
{
	deleteRow(0, ConsoleHeight);
	char a[100];
	sTrang = 7; // cập nhật trang

	if (sVT_Chon == 1) //chọn mức dễ
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 9, 7, 15, 0, a);
	}
	else if (sVT_Chon == 2) //chọn khó
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 9, 8, 15, 0, a);
	}

	strcpy_s(a, " GAME SNAKE ");
	setColorBGTextXY((ConsoleWidth / 2) - strlen(a) / 2, 3, 12, 14, a);

	strcpy_s(a, "  LEVEL : EASY  ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 7, 15, 9, a);
	strcpy_s(a, "LEVEL: DIFFICULT");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 8, 15, 9, a);

	strcpy_s(a, "|=> Created by: <NGUYEN MANH CUONG> <=|");

	deleteRow(25, 3);
	setColorBGTextXY(sChuChay, 25, 9, 0, a);
	setBackgroundColor(0);
}

// vẽ bảng điểm số
void VeBangDiemSo(short sCapDo)
{
	setColor(12);
	gotoXY(ChieuRong_Khung + 5, 2);
	for (short i = 0; i < 22; i++)
		cout << (char)176;
	cout << endl;
	gotoXY(ChieuRong_Khung + 5, 3);	cout << (char)176 << "  => GAME SNAKE <=  " << (char)176 << endl;
	gotoXY(ChieuRong_Khung + 5, 4); cout << (char)176 << "                    " << (char)176 << endl;
	gotoXY(ChieuRong_Khung + 5, 5);	cout << (char)176 << "                    " << (char)176 << endl;
	gotoXY(ChieuRong_Khung + 5, 6);	cout << (char)176 << "                    " << (char)176 << endl;
	gotoXY(ChieuRong_Khung + 5, 7);	cout << (char)176 << "                    " << (char)176 << endl;
	gotoXY(ChieuRong_Khung + 5, 8);	for (short i = 0; i < 22; i++) cout << (char)176;

	char a[50];
	strcpy_s(a, "POINT: ");
	setColorBGTextXY(ChieuRong_Khung + 7, 5, 12, 0, a); cout << sDiemSo;
	if (sCapDo == 1)
	{
		strcpy_s(a, "LEVEL: EASY");
		setColorBGTextXY(ChieuRong_Khung + 7, 6, 12, 0, a);
	}
	else {
		strcpy_s(a, "LEVEL: DIFFICULT");
		setColorBGTextXY(ChieuRong_Khung + 7, 6, 12, 0, a);
	}

	deleteRow(25, 3);

	strcpy_s(a, "|=> Created by: <NGUYEN MANH CUONG> <=|");
	setColorBGTextXY(sChuChay, 25, 9, 0, a);

	setColor(15);
}

//trang lưu điểm 
void TrangLuuDiem()
{
	deleteRow(0, ConsoleHeight);
	sTrang = 8;
	string Ten;
	short sSTT = 0;
	fileLuuDiem.open("LuuDiem.txt", ios_base::in); 
	if (!fileLuuDiem.is_open())
	{	
		fileLuuDiem.open("LuuDiem.txt", ios_base::out);
		fileLuuDiem.close();
	}
	short sRac;
	while (!fileLuuDiem.eof())
	{
		getline(fileLuuDiem, Ten);
		sSTT++;
	}
	fileLuuDiem.close();
	cout << "\n\t\t\t\t==> Created by: <NGUYEN MANH CUONG>\n";

	cout << "\n\n\n\t\t\t";
	for (short i = 0; i < 55; i++)
		cout << (char)176;
	cout << endl;
	cout << "\t\t\t" << (char)176 << "                   => GAME SNAKE <=                  " << (char)176 << endl;
	cout << "\t\t\t" << (char)176 << "                                                     " << (char)176 << endl;
	cout << "\t\t\t"  << "        POINT: " << sDiemSo << "                                    "  << endl;
	cout << "\t\t\t" << (char)176 << "  Vui long nhap ten khong dau                        " << (char)176 << endl << "\t\t\t";;
	for (short i = 0; i < 55; i++)
		cout << (char)176;
	cout << "\n\n\t\t\t" << "ENTER NAME: "; getline(cin, Ten);
	cout << "\t" << "SAVE SUCCESS < LUU THANH CONG >\n";
	cout << "\t\t\t"  << "  ==> VUI LONG NHAN ESC DE QUAY LAI. <==";

	fileLuuDiem.open("LuuDiem.txt", ios_base::app);
	fileLuuDiem << sSTT << " " << Ten << ": " << sDiemSo << endl;
	fileLuuDiem.close();
}

//dọc dữ liệu 1 người chơi
void Doc1NguoiChuoi(fstream& fileLuuDiem, LUUDIEM &NguoiChoi)
{
	string a;
	fileLuuDiem >> NguoiChoi.sSTT;
	getline(fileLuuDiem, NguoiChoi.sTen_NguoiChoi, ':');
	NguoiChoi.sTen_NguoiChoi.erase(NguoiChoi.sTen_NguoiChoi.begin()); // xóa đi khoảng trắng ở đầu
	fileLuuDiem >> NguoiChoi.sDiem;
	getline(fileLuuDiem, a);
}
// xem điểm đã lưu
void XemDiemDaLuu()
{
	deleteRow(0, ConsoleHeight);
	sTrang = 9; // cập nhật trang
	cout << "\n\t\t\t\t==> Created by: <NGUYEN MANH CUONG>\n\n";

	fileLuuDiem.open("LuuDiem.txt", ios_base::in);
	if (!fileLuuDiem.is_open()) // k thể mở file
	{
		cout << "CHUA CO NGUOI CHOI NAO LUU!";
		cout << "\n\t\t\t" << "  ==> VUI LONG NHAN ESC DE QUAY LAI. <==";
		fileLuuDiem.close();
	}
	else
	{
		LUUDIEM NguoiChoi;
		vector<LUUDIEM> arr;
		while (!fileLuuDiem.eof())
		{
			Doc1NguoiChuoi(fileLuuDiem, NguoiChoi);
			arr.push_back(NguoiChoi);
		}
		arr.pop_back();
		fileLuuDiem.close();
		//sắp xếp
		for (short i = 0; i < arr.size(); i++)
		{
			for (short j = i + 1; j < arr.size(); j++)
			{
				if (arr[i].sDiem < arr[j].sDiem)
				{
					LUUDIEM x = arr[i];
					arr[i] = arr[j];
					arr[j] = x;
				}
			}
		}
		fileLuuDiem.open("LuuDiem.txt", ios_base::out);
		for (short i = 0; i < arr.size(); i++)
		{
			arr[i].sSTT = i + 1;
			cout << arr[i].sSTT << " " << arr[i].sTen_NguoiChoi << ": " << arr[i].sDiem << endl;
			fileLuuDiem << arr[i].sSTT << " " << arr[i].sTen_NguoiChoi << ": " << arr[i].sDiem << endl;
		}

		fileLuuDiem.close();
		cout << "\t\t\t" << "  ==> VUI LONG NHAN ESC DE QUAY LAI. <==";
	}
}

//trang chọn nhạc nền
void TrangChonNhac(short sVT_Chon)
{
	deleteRow(0, ConsoleHeight);
	char a[100];
	sTrang = 10; // cập nhật trang

	if (sVT_Chon == 1) //1 TRIỆU KHẢ NĂNG
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 17, 7, 15, 0, a);
	}
	else if (sVT_Chon == 2) //YOU ARE MY CRUSH
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 17, 8, 15, 0, a);
	}
	else  if (sVT_Chon == 3) //YÊU 5
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 17, 9, 15, 0, a);
	}
	else  //THOÁT
	{
		strcpy_s(a, "<=");
		setColorBGTextXY((ConsoleWidth / 2) + 17, 10, 15, 0, a);
	}

	strcpy_s(a, " GAME SNAKE ");
	setColorBGTextXY((ConsoleWidth / 2) - strlen(a) / 2, 3, 12, 14, a);

	strcpy_s(a, " 1 TRIEU KHA NANG <FENG TIMO> ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 7, 15, 9, a);
	strcpy_s(a, "       You Are My Crush       ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 8, 15, 9, a);
	strcpy_s(a, "            YEU 5             ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 9, 15, 9, a);
	strcpy_s(a, "            EXIT              ");
	setColorBGTextXY((ConsoleWidth / 2) - (strlen(a) / 2), 10, 15, 9, a);

	strcpy_s(a, " ==> VUI LONG NHAN ESC DE QUAY LAI. <== ");
	setColorBGTextXY((ConsoleWidth / 2) - strlen(a), 12, 15, 0, a);

	strcpy_s(a, "|=> Created by: <NGUYEN MANH CUONG> <=|");
	setColorBGTextXY(sChuChay, 25, 9, 0, a);
	setBackgroundColor(0);
}

//chữ chạy
void ChuChay()
{
	char a[100];
	for (short i = 5; i <= ConsoleWidth - 5; i++)
	{
		Sleep(50);
		sChuChay = i;
		if (i == ConsoleWidth - 5) i = 5;
		if (sTrang == 2 || sTrang == 8);
		else
		{
			deleteRow(25, 3);
			strcpy_s(a, "|=> Created by: <NGUYEN MANH CUONG> <=|");
			setColorBGTextXY(sChuChay, 25, 9, 0, a);
			setColor(15);
		}
	}
}


