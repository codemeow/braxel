#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <png.h>

#define BRAILLE_WIDTH   (2)
#define BRAILLE_HEIGHT  (4)
#define BRAILLE_COUNT (256)

int32_t max(int32_t a, int32_t b)
{
    if (a > b) return a;
          else return b;
}

int32_t min(int32_t a, int32_t b)
{
    if (a < b) return a;
          else return b;
}

typedef struct
{
    int32_t   width;
    int32_t   height;
    uint8_t * data;
} PData;

void data_set(PData * pdata, uint32_t x, uint32_t y, uint8_t bit);

uint8_t image_pixel(png_bytep * data, png_byte colort, uint32_t x, uint32_t y)
{
    uint8_t r, g, b, a;
    switch (colort)
    {
    case PNG_COLOR_TYPE_GRAY:
        break;
    case PNG_COLOR_TYPE_PALETTE:
        break;
    case PNG_COLOR_TYPE_RGB:
        a = 255;
        r = data[y][x * 3 + 0];
        g = data[y][x * 3 + 1];
        b = data[y][x * 3 + 2];
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        a = data[y][x * 4 + 3];
        r = data[y][x * 4 + 0];
        g = data[y][x * 4 + 1];
        b = data[y][x * 4 + 2];
        break;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        break;
    default:
        exit(1);
    }

    return (uint32_t)round((0.21 * r + 0.72 * g + 0.07 * b) * (255.0 / a)) & 0xFF;
}

PData * from_image(char * filename)
{ /* No memory freeing, just for demonstaration */
    png_byte header[8];
    FILE * file = fopen(filename, "rb");
    if (!file) exit(1);
    if (fread(header, 1, 8, file) != 8) exit(1);
    if (png_sig_cmp(header, 0, 8)) exit(1);
    png_structp handler = png_create_read_struct( PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);
    if (!handler) exit(1);
    png_infop info = png_create_info_struct(handler);
    if (!info) exit(1);
    if (setjmp(png_jmpbuf(handler))) exit(1);
    png_init_io(handler, file);
    png_set_sig_bytes(handler, 8);
    png_read_info(handler, info);

    PData * result = malloc(sizeof(PData));
    if (!result) exit(1);
    uint32_t width = png_get_image_width(handler, info);
    uint32_t height = png_get_image_height(handler, info);
    png_byte colort = png_get_color_type(handler, info);

    result->width  = width  + BRAILLE_WIDTH  - width  % BRAILLE_WIDTH;
    result->height = height + BRAILLE_HEIGHT - height % BRAILLE_HEIGHT;

    if (setjmp(png_jmpbuf(handler))) exit(1);
    result->data = calloc((result->width * result->height) / 8, 1);
    if (!result->data) exit(1);
    png_bytep * data = malloc(sizeof(png_bytep) * result->height);
    if (!data) exit(1);

    png_uint_32 y;
    for (y = 0; y < height; y++)
    {
        data[y] = malloc(png_get_rowbytes(handler, info));
        if (!data[y]) exit(1);
    }

    png_read_image(handler, data);
    if (fclose(file)) exit(1);

    png_uint_32 x;
    for (y = 0; y < height; y++)
    for (x = 0; x < width;  x++)
    {
        data_set(result, x, y, rand() % 255 < image_pixel(data, colort, x, y));
    }

    for (y = 0; y < height; y++)
        free(data[y]);

    free(data);

    /* Also free png_infop & png_structp */

    return result;
}

PData * data_read(char * filename)
{
    PData * result = malloc(sizeof(PData));
    if (!result) return NULL;

    FILE * file = fopen(filename, "rb");
    if (!file) goto error;

    if (!fread(&result->width,  sizeof(result->width ), 1, file)) goto error;
    if (!fread(&result->height, sizeof(result->height), 1, file)) goto error;
    if (result->width * result->height)
    {
        result->data = malloc((result->width * result->height) / 8);
        if (!result->data) goto error;
        if (!fread(result->data, sizeof(*result->data),
                   (result->width * result->height) / 8, file)) goto error;
    }

    fclose(file);
    return result;

error:
    if (file) fclose(file);
    free(result);

    return NULL;
}

uint8_t data_write(char * filename, PData * data)
{
    uint8_t result = 0;
    FILE * file = fopen(filename, "wb");
    if (!file) return 0;

    if (!fwrite(&data->width , sizeof(data->width ), 1, file)) goto error;
    if (!(data->width % BRAILLE_WIDTH)) goto error;
    if (!fwrite(&data->height, sizeof(data->height), 1, file)) goto error;
    if (!(data->height % BRAILLE_HEIGHT)) goto error;
    if (data->width * data->height)
        if (!fwrite( data->data  , sizeof(*data->data),
                    (data->width * data->height) / 8, file)) goto error;

    result = 1;

error:
    fclose(file);
    return result;
}

PData * data_copy(PData * data)
{
    PData * copy = malloc(sizeof(PData));
    if (!data) return NULL;
    if (!copy) return NULL;

    copy->width  = data->width;
    copy->height = data->height;
    copy->data = malloc((copy->height * copy->width) / 8);
    if (!copy->data) goto error;
    memcpy(copy->data, data->data, (copy->height * copy->width) / 8);
    return copy;

error:
    free(copy);
    return NULL;
}

void data_free(PData * data)
{
    if (data->height && data->width) free(data->data);
    free(data);
}

uint32_t prnt[BRAILLE_COUNT];

void init_utf8(void)
{   /* UTF-8 Braille */
    uint32_t x;

    for (x = 0; x < BRAILLE_COUNT; x++)
        prnt[x] = 0xE2 | ((0xA0 + (x >> 6)) << 8) |
                  (((x & 0xF) + ((x % 0x40) & 0xF0) + 0x80) << 16);
}

void data_set(PData * pdata, uint32_t x, uint32_t y, uint8_t bit)
{
    uint32_t Bi, bi;
    Bi = (x + y * pdata->width) / (8 * sizeof(*pdata->data));
    bi = (x + y * pdata->width) % (8 * sizeof(*pdata->data));

    if (bit) pdata->data[Bi] |=  (1 << bi);
        else pdata->data[Bi] &= ~(1 << bi);
}

uint8_t data_get(PData * pdata, uint32_t x, uint32_t y)
{
    uint32_t Bi, bi;
    Bi = (x + y * pdata->width) / (8 * sizeof(*pdata->data));
    bi = (x + y * pdata->width) % (8 * sizeof(*pdata->data));
    return 1 & (pdata->data[Bi] >> bi);
}

uint32_t get_braille(PData * pdata, uint32_t x, uint32_t y)
{
    return (data_get(pdata, x + 0, y + 0) << 0) |
           (data_get(pdata, x + 0, y + 1) << 1) |
           (data_get(pdata, x + 0, y + 2) << 2) |
           (data_get(pdata, x + 1, y + 0) << 3) |
           (data_get(pdata, x + 1, y + 1) << 4) |
           (data_get(pdata, x + 1, y + 2) << 5) |
           (data_get(pdata, x + 0, y + 3) << 6) |
           (data_get(pdata, x + 1, y + 3) << 7) ;
}

void data_print(PData * pdata)
{
    int32_t x, y;
    for (y = 0; y < pdata->height; y += BRAILLE_HEIGHT, printf("\n"))
    for (x = 0; x < pdata->width ; x += BRAILLE_WIDTH)
        printf("%s", (char *)&prnt[get_braille(pdata, x, y)]);
}

void data_draw(PData * dst, PData * src, int32_t x, int32_t y)
{
    int32_t ystart = y < 0 ? 0 : y;
    int32_t ystop  = src->height + y > dst->height ?
                     dst->height : src->height + y;
    int32_t dstx = x < 0 ?  0 : x;
    int32_t srcx = x < 0 ? -x : 0;
    int32_t count;
    if (x < 0)
    {
        if ((x + src->width) > dst->width) count = dst->width;
                                      else count = src->width + x;
    }
    else
    {
        if ((x + src->width) > dst->width) count = dst->width - x;
                                      else count = src->width;
    }

    int32_t ly, lx;
    for (ly = ystart; ly < ystop; ly++)
    for (lx = 0;      lx < count; lx++)
        data_set(dst, lx + dstx, ly, data_get(src, lx + srcx, ly - ystart));
}

#define CHECK(FILE)                            \
if (!FILE)                                     \
{                                              \
    printf("Can't read file \"%s\"\n", #FILE); \
    exit(1);                                   \
}

int main(void)
{
    PData * tiger = from_image("image.png");
    CHECK(tiger);

    init_utf8();
    data_print(tiger);

    data_free(tiger);

    return 0;
}
