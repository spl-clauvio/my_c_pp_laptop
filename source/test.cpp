#include <graphics.h>
#include <conio.h>
#include <Windows.h>
void pause()
{
    system("pause");
}
int main()
{
    initgraph(640, 480);
    setbkcolor(WHITE);
    // 用背景色清空屏幕
    cleardevice();

    // 设置绘图色为红色
    setcolor(RED);
    circle(320, 240, 100);

    pause();
    return 0;
}
