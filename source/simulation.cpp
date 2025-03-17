#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <my_math.h>
#include <Windows.h>
#include <string.h>
#include <my_sort.h>
#include <time.h>
#include <easyx.h>

void pause()
{
    system("pause");
}

void clear()
{
    COORD coord = {0, 0};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hConsole, coord);
}

int y_convert(int y, int h)
{
    return h - y;
}

void draw_a_well(int x_gap, int y_gap, int width, int height, int row, int column, int flag)
{
    int frame_coordinate[2][2];
    int i = 0;

    {
        if (flag == 1 || flag == 3)
        {
            for (i = 0; i < row; i++)
            {
                frame_coordinate[0][0] = x_gap;
                frame_coordinate[0][1] = height + y_gap - (int)round((1 + i) * height / row);

                frame_coordinate[1][0] = width + x_gap;
                frame_coordinate[1][1] = frame_coordinate[0][1];

                line(frame_coordinate[0][0], frame_coordinate[0][1], frame_coordinate[1][0], frame_coordinate[1][1]);
            }
        }

        if (flag == 2 || flag == 3)
        {
            for (i = 0; i < column; i++)
            {
                frame_coordinate[0][0] = x_gap + (int)round((1 + i) * width / column);
                frame_coordinate[0][1] = y_gap;

                frame_coordinate[1][0] = frame_coordinate[0][0];
                frame_coordinate[1][1] = height + y_gap;

                line(frame_coordinate[0][0], frame_coordinate[0][1], frame_coordinate[1][0], frame_coordinate[1][1]);
            }
        }
    }
}

int main()
{

    double a = 0, g = 9.8, x = 0, m = 0, v = 0, f = 0;
    double end_time = 1.0;
    double sim_time = 0.0;
    double gap_time = 0.0;
    double fix_time = 0.0;
    double sleep_time = 0.0;
    double per_sim_time = 0.0;
    double per_refresh_time = 0.0;
    double time_used = 0;
    double step_sim = 0;
    double step_time = 0;
    double xpercent = 0;
    double xlength = 0;
    double ypercent = 0;
    double max_y_ratio = 1;
    double linear_ratio = 1;

    double info[2][256][256] = {0};
    double last_point[2];
    double new_point[2];
    double max_num[20] = {0};
    double y_ratio = 1;

    int sim_times = 0;
    int RRound = 0;
    int RRound_1 = 0;
    int tries = 0;
    int tries_1 = 0;
    int mod = 0;
    int width = 0;
    int height = 0;
    int x_gap = 50;
    int y_gap = 50;
    int frame_row = 10;
    int frame_column = 10;

    int i, j, k = 0;

    int old_coordinate[2] = {0};
    int new_coordinate[2] = {0};
    int frame_coordinate[100][2] = {0};

    clock_t start, end;

    printf("Select mod :\nMod 0 : NEW file\nMod 1 : Test file\n");
    scanf("%d", &mod);
    clear();
    printf("You have chosen Mod %d\n", mod);
    Sleep(200);

    if (mod)
    {
        end_time = 10;
        f = 60;
    }
    else
    {
        printf("Enter simulation time (s)\n");
        scanf("%lf", &end_time);
        printf("Enter f (hz)\n");
        scanf("%lf", &f);
        printf("Enter screen width height (pixel)\n");
        scanf("%d %d", &width, &height);
        printf("Enter frame row column (int)\n");
        scanf("%d %d", &frame_row, &frame_column);
    }

    gap_time = 1000 / f;
    clear();

    start = clock();

    for (sim_time = 0, sim_times = 0; sim_time < end_time * 1000 + gap_time; sim_time += gap_time, sim_times++)
    {
        v = g * sim_time / 1000;
        if (v > max_num[0])
        {
            max_num[0] = v;
        }
        x = 0.5 * g * sim_time / 1000 * sim_time / 1000;
        if (x > max_num[1])
        {
            max_num[1] = x;
        }
        RRound = sim_times / 256;
        tries = sim_times % 256;
        info[0][RRound][tries] = v;
        info[1][RRound][tries] = x;
    }
    sim_times -= 1;

    end = clock();

    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    per_sim_time = time_used * 1000 / sim_times;
    per_refresh_time = (time_used * 1000) / sim_times - sleep_time;

    if (mod)
    {
        width = sim_times * 2;
        height = 800;
        x_gap = 50;
        y_gap = 50;
    }

    initgraph(width + 2 * x_gap, height + 2 * y_gap, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();

    {
        setlinecolor(BLACK);
        draw_a_well(x_gap, y_gap, width, height, frame_row, frame_column, 3);
        setlinestyle(PS_SOLID, 3);

        line(x_gap, height + y_gap, width + x_gap, height + y_gap);
        line(x_gap, height + y_gap, x_gap, y_gap);
        line(width + x_gap, height + y_gap, width + x_gap, y_gap);
    }
    // 框架

    step_sim = sim_times / 22.0;

    for (i = 0; i < 2; i++)
    {
        last_point[0] = 50, last_point[1] = 0;
        y_ratio = 800 / max_num[i] * 0.8;

        for (j = 0; j < 22; j++)
        {
            xpercent = (j + 1) / 22.0;
            xlength = xpercent * sim_times;

            new_point[0] = xpercent * width + 50;
            RRound = (int)floor(xlength) / 256;
            tries = (int)floor(xlength) % 256;
            linear_ratio = (xlength) - (256 * RRound + tries);

            // printf("%lf %d %lf \n", xpercent * sim_times, 256 * RRound + tries, linear_ratio);

            if (tries == 255)
            {
                RRound_1 = RRound + 1;
                tries_1 = 0;
            }
            else
            {
                RRound_1 = RRound;
                tries_1 = tries + 1;
            }

            new_point[1] = info[i][RRound][tries] + linear_ratio * (info[i][RRound_1][tries_1] - info[i][RRound][tries]);

            old_coordinate[0] = (int)round(last_point[0]);
            old_coordinate[1] = (int)round(y_convert((int)floor(last_point[1] * y_ratio), 900) - 50);
            new_coordinate[0] = (int)round(new_point[0]);
            new_coordinate[1] = (int)round(y_convert((int)floor(new_point[1] * y_ratio), 900) - 50);

            // printf("x percent : %lf RRound : %d tries : %d\n", xpercent, RRound, tries);
            // printf("%d %d \n", new_coordinate[0], new_coordinate[1]);

            line(old_coordinate[0], old_coordinate[1], new_coordinate[0], new_coordinate[1]);

            last_point[0] = new_point[0];
            last_point[1] = new_point[1];
        }
    }

    {
        printf("Sim_times : %d\n", sim_times);
        printf("Time cost : %lf\n", time_used);
        printf("Time cost per simulation : %lf\n", per_sim_time);
        printf("Gap time : %lf\n", gap_time);
    }

    pause();
    return 0;
}