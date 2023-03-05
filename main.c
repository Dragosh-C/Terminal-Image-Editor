#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./bmp_header.h"
#define u_sh unsigned short
#define u_char unsigned char
#define u_int unsigned int
#define maxloc 50
#define rez 300

struct gbr
{ // matrix pixel colors
    u_char B[2], G[2], R[2];
};
// loading image into memory
struct gbr **editfile(char *img_name, struct gbr **img, struct bmp_header *file_h, struct info_header *info_h)
{
    FILE *file = fopen(img_name, "rb");

    fread(file_h, sizeof(struct bmp_header), 1, file);

    fread(info_h, sizeof(struct info_header), 1, file);

    fseek(file, file_h->imageDataOffset, SEEK_SET);

    int height = info_h->height;
    int width = info_h->width;

    char padd[4];
    img = (struct gbr **)malloc(height * sizeof(struct gbr *));
    int i = 0, j = 0, sw = 1;
    int padding = 1;
    if (width > rez)
    {
        padding = width * 3 % 4;
    }
    for (i = height - 1; i >= 0;)
    {
        if (sw)
        {
            img[i] = (struct gbr *)malloc(width * sizeof(struct gbr));
        }
        sw = 0;
        fread(&img[i][j].B, sizeof(u_char), 1, file);
        fread(&img[i][j].G, sizeof(u_char), 1, file);
        fread(&img[i][j].R, sizeof(u_char), 1, file);
        if (j == (width - 1))
        {
            i--;
            j = -1;
            sw = 1;
            fread(&padd, sizeof(u_char), padding, file);
        }
        j++;
    }

    fclose(file);

    return img;
}
// overlap images
void insertfile(char *path, int y, int x, struct gbr **img, int height, int width)
{
    struct bmp_header insert_file;
    struct info_header insert_info;
    struct gbr **insert_img = NULL;
    insert_img = editfile(path, insert_img, &insert_file, &insert_info);
    int insert_height = insert_info.height;
    int insert_width = insert_info.width;
    int insert_i = insert_height - 1;
    int insert_j = 0;
    for (int i = height - 1 - x; i >= 0; i--)
    {
        for (int j = 0; j < insert_width; j++)
        {
            if (j + y < width)
            {
                strncpy(img[i][j + y].B, insert_img[insert_i][j].B, 1);
            }
            if (j + y < width)
            {
                strncpy(img[i][j + y].G, insert_img[insert_i][j].G, 1);
            }
            if (j + y < width)
            {
                strncpy(img[i][j + y].R, insert_img[insert_i][j].R, 1);
            }
            insert_j++;
        }
        if (insert_i > 0)
        {
            insert_i--;
        }
        else
        {
            break;
        }
    }
    for (int i = insert_height - 1; i >= 0; i--)
        free(insert_img[i]);
    free(insert_img);
}
// draw line
void draw_line(int y1, int x1, int y2, int x2, u_char *R, u_char *G, u_char *B,
               u_sh l_width, struct gbr **img, int height, int width)
{
    int x = 0, y = 0;

    for (int i = 0; i < l_width; i++)
        for (int j = 0; j < l_width; j++)
        {
            if ((height - 1 - x1 + i - l_width / 2) >= 0 && (height - 1 - x1 + i - l_width / 2) < height 
            && (y1 - l_width / 2 + j) >= 0 && (y1 - l_width / 2 + j) < width)
            {
                strncpy(img[height - 1 - x1 + i - l_width / 2][y1 - l_width / 2 + j].B, B, 1);
                strncpy(img[height - 1 - x1 + i - l_width / 2][y1 - l_width / 2 + j].G, G, 1);
                strncpy(img[height - 1 - x1 + i - l_width / 2][y1 - l_width / 2 + j].R, R, 1);
            }
        }
    for (int i = 0; i < l_width; i++)
        for (int j = 0; j < l_width; j++)
        {
            if ((height - 1 - x2 + i - l_width / 2) >= 0 && (height - 1 - x2 + i - l_width / 2) < height 
            && (y2 - l_width / 2 + j) >= 0 && (y2 - l_width / 2 + j) < width)
            {
                strncpy(img[height - 1 - x2 + i - l_width / 2][y2 - l_width / 2 + j].B, B, 1);
                strncpy(img[height - 1 - x2 + i - l_width / 2][y2 - l_width / 2 + j].G, G, 1);
                strncpy(img[height - 1 - x2 + i - l_width / 2][y2 - l_width / 2 + j].R, R, 1);
            }
        }

    if (abs(x1 - x2) >= abs(y1 - y2))
    {
        if (x1 < x2)
        {
            for (x = x1 + 1; x <= x2; x++)
            {
                y = ((x - x1) * (y2 - y1) + (x2 - x1) * y1) / (x2 - x1);

                for (int i = 0; i < l_width; i++)
                    for (int j = 0; j < l_width; j++)
                    {
                        if ((height - 1 - x + i - l_width / 2) >= 0 && (height - 1 - x + i - l_width / 2) < 
                        height && (y - l_width / 2 + j) >= 0 && (y - l_width / 2 + j) < width)
                        {
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].B, B, 1);
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].G, G, 1);
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].R, R, 1);
                        }
                    }
            }
        }
        else
        {
            for (x = x2; x <= x1; x++)
            {
                y = ((x - x1) * (y2 - y1) + (x2 - x1) * y1) / (x2 - x1);

                for (int i = 0; i < l_width; i++)
                    for (int j = 0; j < l_width; j++)
                    {
                        if ((height - 1 - x + i - l_width / 2) >= 0 && (height - 1 - x + i - l_width / 2) < height &&
                            (y - l_width / 2 + j) >= 0 && (y - l_width / 2 + j) < width)
                        {
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].B, B, 1);
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].G, G, 1);
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].R, R, 1);
                        }
                    }
            }
        }
    }

    if (abs(x1 - x2) < abs(y1 - y2))
    {
        if (y1 < y2)
        {
            for (y = y1; y <= y2; y++)
            {
                x = ((y - y1) * (x2 - x1) + x1 * (y2 - y1)) / (y2 - y1);

                for (int i = 0; i < l_width; i++)
                {
                    for (int j = 0; j < l_width; j++)
                    {
                        if ((height - 1 - x + i - l_width / 2) >= 0 && (height - 1 - x + i - l_width / 2) < 
                        height && (y - l_width / 2 + j) >= 0 && (y - l_width / 2 + j) < width)
                        {
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].B, B, 1);
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].G, G, 1);
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].R, R, 1);
                        }
                    }
                }
            }
        }
        else
        {
            for (y = y2; y <= y1; y++)
            {
                x = ((y - y1) * (x2 - x1) + (y2 - y1) * x1) / (y2 - y1);

                for (int i = 0; i < l_width; i++)
                {
                    for (int j = 0; j < l_width; j++)
                    {
                        if ((height - 1 - x + i - l_width / 2) >= 0 && (height - 1 - x + i - l_width / 2) < 
                        height && (y - l_width / 2 + j) >= 0 && (y - l_width / 2 + j) < width)
                        {
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].B, B, 1);
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].G, G, 1);
                            strncpy(img[height - 1 - x + i - l_width / 2][y + j - l_width / 2].R, R, 1);
                        }
                    }
                }
            }
        }
    }
}

// draw reclangle
void draw_rect(int y, int x, int rect_width, int rect_height, u_char *R,
               u_char *G, u_char *B, u_sh l_width, struct gbr **img, int height, int width)
{
    // l_width - line width
    for (int i = 0; i < l_width; i++)
        for (int j = 0; j < l_width; j++)
        {
            if ((y - l_width / 2 + j) < width && (height - 1 - x + i - l_width / 2) >= 0)
            {
                strncpy(img[height - 1 - x + i - l_width / 2][y - l_width / 2 + j].B, B, 1);
                strncpy(img[height - 1 - x + i - l_width / 2][y - l_width / 2 + j].G, G, 1);
                strncpy(img[height - 1 - x + i - l_width / 2][y - l_width / 2 + j].R, R, 1);
            }
        }
    for (int g = 0; g <= rect_height; g++)
        for (int i = 0; i < l_width; i++)
            for (int j = 0; j < l_width; j++)
            {
                if ((y - l_width / 2 + j) < width && (height - 1 - x + i - l_width / 2 - g) >= 0)
                {
                    strncpy(img[height - 1 - x + i - l_width / 2 - g][y - l_width / 2 + j].B, B, 1);
                    strncpy(img[height - 1 - x + i - l_width / 2 - g][y - l_width / 2 + j].G, G, 1);
                    strncpy(img[height - 1 - x + i - l_width / 2 - g][y - l_width / 2 + j].R, R, 1);
                }
            }

    for (int g = 0; g <= rect_height; g++)
        for (int i = 0; i < l_width; i++)
            for (int j = 0; j < l_width; j++)
            {
                if ((y - l_width / 2 + j + rect_width) < width && (height - 1 - x + i - l_width / 2 - g) >= 0)
                {
                    strncpy(img[height - 1 - x + i - l_width / 2 - g][y - l_width / 2 + j + rect_width].B, B, 1);
                    strncpy(img[height - 1 - x + i - l_width / 2 - g][y - l_width / 2 + j + rect_width].G, G, 1);
                    strncpy(img[height - 1 - x + i - l_width / 2 - g][y - l_width / 2 + j + rect_width].R, R, 1);
                }
            }

    for (int g = 0; g <= rect_width; g++)
        for (int i = 0; i < l_width; i++)
            for (int j = 0; j < l_width; j++)
            {
                if ((y - l_width / 2 + j + g) < width && (height - 1 - x + i - l_width / 2 - rect_height) >= 0)
                {
                    strncpy(img[height - 1 - x + i - l_width / 2 - rect_height][y - l_width / 2 + j + g].B, B, 1);
                    strncpy(img[height - 1 - x + i - l_width / 2 - rect_height][y - l_width / 2 + j + g].G, G, 1);
                    strncpy(img[height - 1 - x + i - l_width / 2 - rect_height][y - l_width / 2 + j + g].R, R, 1);
                }
            }
    for (int g = 0; g <= rect_width; g++)
        for (int i = 0; i < l_width; i++)
            for (int j = 0; j < l_width; j++)
            {
                if ((y - l_width / 2 + j + g) < width && (height - 1 - x + i - l_width / 2) >= 0)
                {
                    strncpy(img[height - 1 - x + i - l_width / 2][y - l_width / 2 + j + g].B, B, 1);
                    strncpy(img[height - 1 - x + i - l_width / 2][y - l_width / 2 + j + g].G, G, 1);
                    strncpy(img[height - 1 - x + i - l_width / 2][y - l_width / 2 + j + g].R, R, 1);
                }
            }
}

// image filling

void fill4(struct gbr(**img), int height, int width, int y,
           int x, int i, int j, u_char *R, u_char *G, u_char *B)
{
    if ((i > 0) && (i < height - 1) && (j >= 0) && (j <= width - 1))
    {
        if ((strcmp(img[x][y].B, img[i + 1][j].B) == 0) &&
            (strcmp(img[x][y].R, img[i + 1][j].R) == 0) &&
            (strcmp(img[x][y].G, img[i + 1][j].G) == 0))
        {
            strncpy(img[i + 1][j].B, B, 1);
            strncpy(img[i + 1][j].G, G, 1);
            strncpy(img[i + 1][j].R, R, 1);
            fill4(img, height, width, y, x, i + 1, j, R, G, B);
        }
        if ((strcmp(img[x][y].B, img[i][j - 1].B) == 0) &&
            (strcmp(img[x][y].R, img[i][j - 1].R) == 0) &&
            (strcmp(img[x][y].G, img[i][j - 1].G) == 0))
        {
            strncpy(img[i][j - 1].B, B, 1);
            strncpy(img[i][j - 1].G, G, 1);
            strncpy(img[i][j - 1].R, R, 1);
            fill4(img, height, width, y, x, i, j - 1, R, G, B);
        }
    }
}

void fill3(struct gbr(**img), int height, int width, int y,
           int x, int i, int j, u_char *R, u_char *G, u_char *B)
{
    if ((i > 0) && (i < height - 1) && (j >= 0) && (j <= width - 1))
    {
        if ((i > 0) && (i < height - 1) && (j >= 0) && (j <= width - 1))
        {
            if ((strcmp(img[x][y].B, img[i + 1][j].B) == 0) &&
                (strcmp(img[x][y].R, img[i + 1][j].R) == 0) &&
                (strcmp(img[x][y].G, img[i + 1][j].G) == 0))
            {
                strncpy(img[i + 1][j].B, B, 1);
                strncpy(img[i + 1][j].G, G, 1);
                strncpy(img[i + 1][j].R, R, 1);
                fill3(img, height, width, y, x, i + 1, j, R, G, B);
            }
        }
        if ((strcmp(img[x][y].B, img[i][j + 1].B) == 0) &&
            (strcmp(img[x][y].R, img[i][j + 1].R) == 0) &&
            (strcmp(img[x][y].G, img[i][j + 1].G) == 0))
        {
            strncpy(img[i][j + 1].B, B, 1);
            strncpy(img[i][j + 1].G, G, 1);
            strncpy(img[i][j + 1].R, R, 1);
            fill3(img, height, width, y, x, i, j + 1, R, G, B);
        }
    }
    fill4(img, height, width, y, x, x, y, R, G, B);
}

void fill2(struct gbr(**img), int height, int width, int y,
           int x, int i, int j, u_char *R, u_char *G, u_char *B)
{
    if ((i > 0) && (i < height - 1) && (j >= 0) && (j <= width - 1))
    {
        if ((strcmp(img[x][y].B, img[i - 1][j].B) == 0) &&
            (strcmp(img[x][y].R, img[i - 1][j].R) == 0) && (strcmp(img[x][y].G, img[i - 1][j].G) == 0))
        {
            strncpy(img[i - 1][j].B, B, 1);
            strncpy(img[i - 1][j].G, G, 1);
            strncpy(img[i - 1][j].R, R, 1);
            fill2(img, height, width, y, x, i - 1, j, R, G, B);
        }

        if ((strcmp(img[x][y].B, img[i][j + 1].B) == 0) &&
            (strcmp(img[x][y].R, img[i][j + 1].R) == 0) &&
            (strcmp(img[x][y].G, img[i][j + 1].G) == 0))
        {
            strncpy(img[i][j + 1].B, B, 1);
            strncpy(img[i][j + 1].G, G, 1);
            strncpy(img[i][j + 1].R, R, 1);
            fill2(img, height, width, y, x, i, j + 1, R, G, B);
        }
    }
    fill3(img, height, width, y, x, x, y, R, G, B);
}

void fill1(struct gbr(**img), int height, int width, int y, int x,
           int i, int j, u_char *R, u_char *G, u_char *B)
{
    if ((i > 0) && (i < height - 1) && (j >= 0) && (j <= width - 1))
    {
        if ((strcmp(img[x][y].B, img[i - 1][j].B) == 0) &&
            (strcmp(img[x][y].R, img[i - 1][j].R) == 0) && (strcmp(img[x][y].G, img[i - 1][j].G) == 0))
        {
            strncpy(img[i - 1][j].B, B, 1);
            strncpy(img[i - 1][j].G, G, 1);
            strncpy(img[i - 1][j].R, R, 1);
            fill1(img, height, width, y, x, i - 1, j, R, G, B);
        }

        if ((strcmp(img[x][y].B, img[i][j - 1].B) == 0) &&
            (strcmp(img[x][y].R, img[i][j - 1].R) == 0) &&
            (strcmp(img[x][y].G, img[i][j - 1].G) == 0))
        {
            strncpy(img[i][j - 1].B, B, 1);
            strncpy(img[i][j - 1].G, G, 1);
            strncpy(img[i][j - 1].R, R, 1);
            fill1(img, height, width, y, x, i, j - 1, R, G, B);
        }
    }
    fill2(img, height, width, y, x, x, y, R, G, B);
}

// saves the image to the path
void savefile(char *img_name, struct gbr **img, struct bmp_header file_h,
              struct info_header info_h)
{
    FILE *wfile = fopen(img_name, "wb");

    fwrite(&file_h.fileMarker1, sizeof(u_char), 1, wfile);
    fwrite(&file_h.fileMarker2, sizeof(u_char), 1, wfile);
    fwrite(&file_h.bfSize, sizeof(u_int), 1, wfile);
    fwrite(&file_h.unused1, sizeof(u_sh), 1, wfile);
    fwrite(&file_h.unused2, sizeof(u_sh), 1, wfile);
    fwrite(&file_h.imageDataOffset, sizeof(u_int), 1, wfile);

    fwrite(&info_h.biSize, sizeof(u_int), 1, wfile);
    fwrite(&info_h.width, sizeof(signed int), 1, wfile);
    fwrite(&info_h.height, sizeof(signed int), 1, wfile);
    fwrite(&info_h.planes, sizeof(u_sh), 1, wfile);
    fwrite(&info_h.bitPix, sizeof(u_sh), 1, wfile);
    fwrite(&info_h.biCompression, sizeof(u_int), 1, wfile);
    fwrite(&info_h.biSizeImage, sizeof(u_int), 1, wfile);
    fwrite(&info_h.biXPelsPerMeter, sizeof(int), 1, wfile);
    fwrite(&info_h.biYPelsPerMeter, sizeof(int), 1, wfile);
    fwrite(&info_h.biClrUsed, sizeof(u_int), 1, wfile);
    fwrite(&info_h.biClrImportant, sizeof(u_int), 1, wfile);
    int height = info_h.height;
    int width = info_h.width;
    int padding = 1;
    if (width > rez)
    {
        padding = width * 3 % 4;
    }
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            fwrite(img[i][j].B, sizeof(u_char), 1, wfile);
            fwrite(img[i][j].G, sizeof(u_char), 1, wfile);
            fwrite(img[i][j].R, sizeof(u_char), 1, wfile);
            if (j == (width - 1))
            {
                fwrite("00", sizeof(u_char), padding, wfile);
            }
        }
    }
    fclose(wfile);
}

// frees up allocated memory
void freemem(struct gbr **img, int height)
{
    for (int i = height - 1; i >= 0; i--)
        free(img[i]);
    free(img);
}

int main(int argc, char *argv[])
{
    char *path = NULL;
    path = malloc(maxloc);
    struct bmp_header file_h;
    struct info_header info_h;
    struct gbr **img = NULL;
    char *v = NULL;
    v = malloc(maxloc);

    u_char R[2] = {'0'}, G[2] = {'0'}, B[2] = {'0'};
    u_sh l_width_x = 1;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0, height = 0, width = 0;

    scanf("%s", v);

    while (strcmp(v, "quit"))
    {
        if (strcmp(v, "edit") == 0)
        {
            scanf("%s", v);
            img = editfile(v, img, &file_h, &info_h);
        }
        if (strcmp(v, "insert") == 0)
        {
            int y = 0, x = 0;
            scanf("%s", path);
            scanf("%d", &y);
            scanf("%d", &x);
            insertfile(path, y, x, img, info_h.height, info_h.width);
        }
        // Set color or line width
        if (strcmp(v, "set") == 0)
        {
            scanf("%s", v);
            if (strcmp(v, "draw_color") == 0)
            {
                scanf("%hhu %hhu %hhu", R, G, B);
            }
            if (strcmp(v, "line_width") == 0)
            {
                scanf("%hu", &l_width_x);
            }
        }
        // Draw a line, a rectangle or a triangle
        if (strcmp(v, "draw") == 0)
        {
            scanf("%s", v);
            if (strcmp(v, "line") == 0)
            {
                scanf("%d %d %d %d", &y1, &x1, &y2, &x2);
                draw_line(y1, x1, y2, x2, R, G, B, l_width_x, img, info_h.height, info_h.width);
            }
            if (strcmp(v, "rectangle") == 0)
            {
                scanf("%d %d %d %d", &y1, &x1, &width, &height);
                draw_rect(y1, x1, width, height, R, G, B, l_width_x, img, info_h.height, info_h.width);
            }
            if (strcmp(v, "triangle") == 0)
            {
                scanf("%d %d %d %d %d %d", &y1, &x1, &y2, &x2, &y3, &x3);
                draw_line(y1, x1, y2, x2, R, G, B, l_width_x, img, info_h.height, info_h.width);
                draw_line(y1, x1, y3, x3, R, G, B, l_width_x, img, info_h.height, info_h.width);
                draw_line(y2, x2, y3, x3, R, G, B, l_width_x, img, info_h.height, info_h.width);
            }
        }

        // Fill de form
        if (strcmp(v, "fill") == 0)
        {
            scanf("%d %d", &y1, &x1);
            fill1(img, info_h.height, info_h.width, y1, info_h.height - x1 - 1,
                  info_h.height - x1 - 1, y1, R, G, B);

            strncpy(img[info_h.height - 1 - x1][y1].B, B, 1);
            strncpy(img[info_h.height - 1 - x1][y1].G, G, 1);
            strncpy(img[info_h.height - 1 - x1][y1].R, R, 1);
        }
        // Save image
        if (strcmp(v, "save") == 0)
        {
            scanf("%s", v);
            savefile(v, img, file_h, info_h);
        }
        scanf("%s", v);
    }

    if (strcmp(v, "quit") == 0)
    {
        freemem(img, info_h.height);
    }

    free(path);
    free(v);

    return 0;
}
