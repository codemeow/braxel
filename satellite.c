#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

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
    PData * earth = data_read("earth.bin");
    CHECK(earth);

    PData * point = data_read("point.bin");
    CHECK(point);

    init_utf8();

    int i;
    for (i = 0; i < earth->width; i++)
    {
        PData * ecopy = data_copy(earth);
        CHECK(ecopy);
        data_draw(ecopy, point, i, round(sin(i / 20.0) * 40) + ecopy->height / 2);
        data_print(ecopy);
        printf("\033[%uA", ecopy->height / BRAILLE_HEIGHT);
        usleep(100 * 1000);
        data_free(ecopy);
    }
    printf("\033[%uB", earth->height / BRAILLE_HEIGHT);
    data_free(earth);
    data_free(point);

    return 0;
}
