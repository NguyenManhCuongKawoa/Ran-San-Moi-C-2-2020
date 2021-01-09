#pragma once

// khởi tạo game mới
void KhoiTaoGame();

// hàm trên mạng
void Nocursortype(); // ẩn hiện con trỏ
void XoaManHinh(); // xóa màn hình

//tạo vị trí xuất hiện ban đầu
void KhoiTaoViTriConRanBanDau();

//tạo đồ ăn
void KhoiTaoDoAnBanDau();
void TaoDoAnNgauNhien();

// vẽ bản đồ của con rắn có thể di chuyển trong đó
void Ve_Bang();
void Ve_O(short sVT_Dong, short sVT_Cot);

// Xử lý Phím
void Xu_Li_Su_Kien();
void Xu_Li_Phim(KEY_EVENT_RECORD key);

// Xử lý game
void ChayLienTuc();
void Ran_An_DoAn(short x, short y);
void CapNhatLaiViTri_ThanRan();
bool KT_ConRanDaChamVaoThanNoChua();

// hàm MENU
void VeMenuChinh(short sVT_Chon);
void VeMenuThua(short sVT_Chon);
void VeMenuTamDung(short sVT_Chon);
void VeTrangThongTin();
void VeTrangCapDo(short sVT_Chon);
void VeBangDiemSo(short sCapDo);
void TrangLuuDiem();
void XemDiemDaLuu();
void TrangChonNhac(short sVT_Chon);

//hàm chạy nền trong game
void ChuChay(); // chữ chạy