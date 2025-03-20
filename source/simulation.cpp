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

void clear_input()
{
    while ('\n' != getchar())
        ;
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

void draw_a_well1(int origin_x, int origin_y, int width, int height, int row, int column, int flag)
{
    int frame_coordinate[2][2];
    int i = 0;

    {
        if (flag == 1 || flag == 3)
        {
            for (i = 0; i < row; i++)
            {
                frame_coordinate[0][0] = origin_x;
                frame_coordinate[0][1] = origin_y - (int)round((1 + i) * height / row);

                frame_coordinate[1][0] = width + origin_x;
                frame_coordinate[1][1] = frame_coordinate[0][1];

                line(frame_coordinate[0][0], frame_coordinate[0][1], frame_coordinate[1][0], frame_coordinate[1][1]);
            }
        }

        if (flag == 2 || flag == 3)
        {
            for (i = 0; i < column; i++)
            {
                frame_coordinate[0][0] = origin_x + (int)round((1 + i) * width / column);
                frame_coordinate[0][1] = origin_y;

                frame_coordinate[1][0] = frame_coordinate[0][0];
                frame_coordinate[1][1] = origin_y - height;

                line(frame_coordinate[0][0], frame_coordinate[0][1], frame_coordinate[1][0], frame_coordinate[1][1]);
            }
        }
    }
}

void draw_table_1(double last_point[2], int width, int height, double show_number, int y_gap, double max_num[20], double info[2][500][256], int sim_times, double y_ratio, double *y_scale_ratio)
{
    int i = 0, j = 0, RRound, tries, RRound_1, tries_1;
    double xpercent, xlength, linear_ratio, x_origin, y_origin;
    double new_point[2];
    int old_coordinate[2], new_coordinate[2];
    x_origin = last_point[0];
    y_origin = last_point[1];

    for (i = 0; i < 2; i++)
    {
        last_point[0] = 0;
        last_point[1] = 0;
        printf("%lf %lf\n", last_point[0], last_point[1]);

        y_ratio = y_scale_ratio[i] / max_num[i] * height;

        for (j = 0; j < show_number; j++)
        {
            xpercent = (j + 1) / (show_number);
            xlength = xpercent * sim_times;

            new_point[0] = xpercent * width;
            RRound = (int)floor(xlength) / 256;
            tries = (int)floor(xlength) % 256;
            linear_ratio = (xlength) - (256 * RRound + tries);

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

            old_coordinate[0] = (int)round(last_point[0] + x_origin);
            old_coordinate[1] = (int)round(y_convert((int)floor(last_point[1] * y_ratio), height + 2 * y_gap) - y_origin);
            new_coordinate[0] = (int)round(new_point[0] + x_origin);
            new_coordinate[1] = (int)round(y_convert((int)floor(new_point[1] * y_ratio), height + 2 * y_gap) - y_origin);

            line(old_coordinate[0], old_coordinate[1], new_coordinate[0], new_coordinate[1]);
            printf("%d %lf (%d,%d) , (%d,%d)\n", j, y_ratio, old_coordinate[0], old_coordinate[1], new_coordinate[0], new_coordinate[1]);

            last_point[0] = new_point[0];
            last_point[1] = new_point[1];
            Sleep(50);
        }
    }
}

void draw_a_coordination(int origin_x, int origin_y, int x_length, int y_length)
{
    line(origin_x, origin_y, origin_x + x_length, origin_y);
    line(origin_x, origin_y, origin_x, origin_y - y_length);
}

int x_fix(int x, int origin_x, double scale_ratio)
{
    int xx = 0;
    xx = floor(scale_ratio * x + origin_x);
    return xx;
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
    double draw_time = 1;
    double draw_wait_time = 0;
    double time_used = 0;
    double step_sim = 0;
    double step_time = 0;
    double xpercent = 0;
    double xlength = 0;
    double ypercent = 0;
    double max_y_ratio = 1;
    double linear_ratio = 1;
    double y_ratio = 1;
    double show_number = 20.0;

    double info[2][500][256] = {0};
    double last_point[2];
    double new_point[2];
    double max_num[20] = {0};
    double y_scale_ratio[100] = {0};

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
    int coordinate_check[4] = {0};
    int origin_point[2] = {0};

    clock_t start, end;

    for (i = 0; i < 100; i++)
    {
        y_scale_ratio[i] = 1;
    }

    printf("Select mod :\nMod 0 : NEW file\nMod 1 : Test file\n");
    scanf("%d", &mod);
    clear();
    printf("You have chosen Mod %d\n", mod);
    Sleep(200);
    f = 100;

    if (mod)
    {
        end_time = 120;
        width = 1200;
        height = 800;
        frame_row = 8;
        frame_column = 12;
        show_number = 40;
        y_scale_ratio[0] = 0.3;
        y_scale_ratio[1] = 0.9;
    }
    else
    {
        printf("Enter simulation time (s)\n");
        scanf("%lf", &end_time);
        printf("Enter screen width height (pixel)\n");
        scanf("%d %d", &width, &height);
        printf("Enter frame row column (int)\n");
        scanf("%d %d", &frame_row, &frame_column);
        printf("Enter show acc (int)\n");
        scanf("%lf", &show_number);
        for (i = 0; i < 2; i++)
        {
            printf("Enter max y scale ratio for NO.%d number\n", i + 1);
            scanf("%lf", &y_scale_ratio[i]);
        }
    }

    gap_time = 1000 / f;
    clear();

    start = clock();

    for (sim_time = 0, sim_times = 0, RRound = 0, tries = 0; sim_time / 1000 < end_time + gap_time / 1000; sim_time += gap_time, tries++, sim_times++)
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
        // RRound = sim_times / 256;
        // tries = sim_times % 256;

        if (tries > 255)
        {
            RRound = RRound + 1;
            tries = 0;
        }

        info[0][RRound][tries] = v;
        info[1][RRound][tries] = x;

        // printf("%d %d %d %lf %lf\n", sim_times, RRound, tries, v, x);
    }
    sim_times -= 1;

    end = clock();

    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    per_sim_time = time_used * 1000 / sim_times;
    per_refresh_time = (time_used * 1000) / sim_times - sleep_time;

    if (mod)
    {
        width = 1200;
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
        draw_a_coordination(x_gap, y_convert(y_gap, height + 2 * y_gap), width, height);
    }
    // 框架

    step_sim = sim_times / show_number;
    draw_wait_time = draw_time / show_number * 1000;

    /*
        for (i = 0; i < 2; i++)
        {
            last_point[0] = x_gap, last_point[1] = 0;
            y_ratio = y_scale_ratio[i] / max_num[i] * height;

            for (j = 0; j < show_number; j++)
            {
                xpercent = (j + 1) / (show_number);
                xlength = xpercent * sim_times;

                new_point[0] = xpercent * width + x_gap;
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
                old_coordinate[1] = (int)round(y_convert((int)floor(last_point[1] * y_ratio), height + 2 * y_gap) - y_gap);
                new_coordinate[0] = (int)round(new_point[0]);
                new_coordinate[1] = (int)round(y_convert((int)floor(new_point[1] * y_ratio), height + 2 * y_gap) - y_gap);

                {
                    coordinate_check[0] = old_coordinate[0];
                    coordinate_check[1] = old_coordinate[1];
                    coordinate_check[2] = new_coordinate[0];
                    coordinate_check[3] = new_coordinate[1];
                }

                // printf("x percent : %4.3lf RRound : %d tries : %d\n", xpercent, RRound, tries);

                line(old_coordinate[0], old_coordinate[1], new_coordinate[0], new_coordinate[1]);
                printf("%d %lf (%d,%d) , (%d,%d)\n", j, y_ratio, old_coordinate[0], old_coordinate[1], new_coordinate[0], new_coordinate[1]);
                //  printf("%d %lf\n", j, show_number);

                last_point[0] = new_point[0];
                last_point[1] = new_point[1];
                // Sleep(draw_wait_time);
            }
        }
        clear_input();
        getchar();
        cleardevice();

    cleardevice();
    setlinestyle(PS_SOLID, 1);
    draw_a_well1(0, y_convert(0, height + 2 * y_gap), width, height, frame_row, frame_column, 3);
    setlinestyle(PS_SOLID, 3);
    draw_a_coordination(0, y_convert(0, height + 2 * y_gap), width, height);

    for (i = 0; i < 2; i++)
    {
        last_point[0] = 0, last_point[1] = 0;
        y_ratio = y_scale_ratio[i] / max_num[i] * height;

        for (j = 0; j < show_number; j++)
        {
            xpercent = (j + 1) / (show_number);
            xlength = xpercent * sim_times;

            new_point[0] = xpercent * width;
            RRound = (int)floor(xlength) / 256;
            tries = (int)floor(xlength) % 256;
            linear_ratio = (xlength) - (256 * RRound + tries);

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
            old_coordinate[1] = (int)round(y_convert((int)floor(last_point[1] * y_ratio), height + 2 * y_gap));
            new_coordinate[0] = (int)round(new_point[0]);
            new_coordinate[1] = (int)round(y_convert((int)floor(new_point[1] * y_ratio), height + 2 * y_gap));

            {
                coordinate_check[0] = old_coordinate[0];
                coordinate_check[1] = old_coordinate[1];
                coordinate_check[2] = new_coordinate[0];
                coordinate_check[3] = new_coordinate[1];
            }

            line(old_coordinate[0], old_coordinate[1], new_coordinate[0], new_coordinate[1]);
            printf("%d %lf (%d,%d) , (%d,%d)\n", j, y_ratio, old_coordinate[0], old_coordinate[1], new_coordinate[0], new_coordinate[1]);

            last_point[0] = new_point[0];
            last_point[1] = new_point[1];
            // Sleep(draw_wait_time);
        }
    }
        */

    cleardevice();
    setlinestyle(PS_SOLID, 1);
    draw_a_well1(0, y_convert(0, height + 2 * y_gap), width, height, frame_row, frame_column, 3);
    setlinestyle(PS_SOLID, 3);
    draw_a_coordination(0, y_convert(0, height + 2 * y_gap), width, height);

    last_point[0] = 500, last_point[1] = 500;

    draw_table_1(last_point, width, height, show_number, y_gap, max_num, info, sim_times, y_ratio, y_scale_ratio);

    {
        printf("Sim_times : %d\n", sim_times);
        printf("Time cost : %lf\n", time_used);
        printf("Time cost per simulation : %lf\n", per_sim_time);
        printf("Gap time : %lf\n", gap_time);
    }

    pause();
    return 0;
}