#include <graphics.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define d 35  // d=distance
#define s 30  // s=size
#define f 200 // f=display coordinate factor
#define mx getmaxx()
#define my getmaxy()

char grid[3][3];

int getkey();                // To capture arrow keys pressed
void display(int, int);      // To display grid
int checkWin(int, int, int); // Check win; return 1 if any one of two players win else 0
int checkDraw(int, int);     // Check draw; return 1 if draw else 0
void end(char *);            // Game end
void bresenhamLine(int, int, int, int); // Custom line drawing function
void bresenhamCircle(int, int, int);    // Custom circle drawing function

int main()
{
    int gd, gm, sx = 0, sy = 0, i, j, k, count = 2, player;
    char str[25], ch;

    detectgraph(&gd, &gm);
    initgraph(&gd, &gm, "C:\\MinGW\\lib\\libbgi.a");

    for (i = 0; i < 3; i++) // Initialize grid with blank spaces
        for (j = 0; j < 3; j++)
            grid[i][j] = ' ';

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2); // Initialize text style

    while (1)
    {
        cleardevice();
        // Print TIC-TAC-T   OE on screen
        setcolor(BLUE);
        outtextxy(200, 150, "TIC-TAC-TOE");
        // Print current Player number
        player = count % 2;
        sprintf(str, "Player : %d (%c)", player + 1, player ? 'O' : 'X');
        setcolor(WHITE);
        outtextxy(350, 250, str);
        display(sx, sy);
        ch = getkey(); // Capture arrow key input
        switch (ch)
        {
        case 72: // up arrow
            if (sy != 0)
                sy--;
            break;
        case 80: // down arrow
            if (sy != 2)
                sy++;
            break;
        case 75: // left arrow
            if (sx != 0)
                sx--;
            break;
        case 77: // right arrow
            if (sx != 2)
                sx++;
            break;
        case ' ':                    // Space key
            if (grid[sy][sx] == ' ') // Mark the cell only if it is empty
            {
                if (player == 0)
                    grid[sy][sx] = 'X';
                else
                    grid[sy][sx] = 'O';
                count++;
            }
            break;
        case 'e':
        case 'E': //'e' or 'E' key
            cleardevice();
            closegraph();
            return 0;
        default:
            break;
        }
        if (checkWin(sx, sy, player) == 1 || checkDraw(sx, sy) == 1)
            break;
    }
    return 0;
}

int getkey()
{
    int ch;
    ch = getch();
    if (ch == 0)
    {
        ch = getch();
        return (ch);
    }
    return (ch);
}

void display(int sx, int sy)
{
    int i, j;
    char str[2];
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (j == sx && i == sy) // If the cell is selected cell
                setcolor(RED);
            else
                setcolor(WHITE); // For non-selected cell

            // Draw the rectangle using Bresenham's line algorithm
            bresenhamLine(j * d + f, i * d + f, j * d + s + f, i * d + f);
            bresenhamLine(j * d + s + f, i * d + f, j * d + s + f, i * d + s + f);
            bresenhamLine(j * d + s + f, i * d + s + f, j * d + f, i * d + s + f);
            bresenhamLine(j * d + f, i * d + s + f, j * d + f, i * d + f);

            sprintf(str, "%c", grid[i][j]); // To print Player's symbol in cell
            outtextxy(j * d + 8 + f, i * d + 8 + f, str);
        }
    }
}

int checkWin(int sx, int sy, int player)
{
    char str[25];
    int i, j;
    for (i = 0; i < 3; i++)
    {
        if ((grid[i][0] == grid[i][1] && grid[i][0] == grid[i][2] && grid[i][0] != ' ') || (grid[0][i] == grid[1][i] && grid[0][i] == grid[2][i] && grid[0][i] != ' '))
        {
            display(sx, sy);
            getch();
            sprintf(str, "Player %d (%c) You Won!!!", player + 1, player ? 'O' : 'X');
            end(str);
            return 1;
        }
        if ((grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2] && grid[2][2] != ' ') || (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0] && grid[2][0] != ' '))
        {
            display(sx, sy);
            getch();
            sprintf(str, "Player %d (%c) You Won!!!", player + 1, player ? 'O' : 'X');
            end(str);
            return 1;
        }
    }
    return 0;
}

int checkDraw(int sx, int sy)
{
    int i, j, k = 0;
    char str[25];
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (grid[i][j] != ' ')
                k++;
    if (k == 9) // All cells marked but yet no win i.e. Draw
    {
        display(sx, sy);
        getch();
        sprintf(str, "The game is draw!!!");
        end(str);
        return 1;
    }
    return 0;
}

void end(char *str)
{
    int i, j;
    delay(800);
    cleardevice();
    setcolor(WHITE);
    outtextxy(mx / 2 - 150, my / 2, str);
    for (i = 0, j = 0; i < 14; i++)
    {
        if (i > 7)
            j = 1;
        setcolor(j);

        // Draw the circle using Bresenham's circle algorithm
        bresenhamCircle(mx / 2, my / 2, i);

        delay(100);
    }
    outtextxy(mx / 2 - 150, my / 2, str);
    getch();
    closegraph();
}

void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        putpixel(x1, y1, WHITE); // Draw pixel at (x1, y1)

        if (x1 == x2 && y1 == y2)
            break;

        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void bresenhamCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        // Draw the eight octants
        putpixel(xc + x, yc + y, WHITE);
        putpixel(xc - x, yc + y, WHITE);
        putpixel(xc + x, yc - y, WHITE);
        putpixel(xc - x, yc - y, WHITE);
        putpixel(xc + y, yc + x, WHITE);
        putpixel(xc - y, yc + x, WHITE);
        putpixel(xc + y, yc - x, WHITE);
        putpixel(xc - y, yc - x, WHITE);

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}
