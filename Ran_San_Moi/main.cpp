#include"main.h"
#include"ham.h"
#include"Console.h"


int main()
{
	resizeConsole(ConsoleWidth, ConsoleHeight);
	SetConsoleTitle(TEXT("Ran San Moi"));
	thread thr1(ChuChay);
	thr1.detach();
	VeMenuChinh(1);
	AmThanhnen(IDR_WAVE7);
	Nocursortype();
	Xu_Li_Su_Kien();
	return 0;
}