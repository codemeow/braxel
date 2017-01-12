/* «Braxel» - Braille-based console drawer
 *
 * Copyrigth (c) Alexey Shishkin 2016-2017
 *
 *  This file is part of Project Braxel.
 *
 *  Project Braxel is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Project Braxel is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Project Braxel. If not, see <http://www.gnu.org/licenses/>.
 */

#include <png.h>
#include <stdlib.h>

#include "../graph/brxpng.h"
#include "../base/brxbase.h"
#include "../definitions/brxcheck.h"
#include "../graph/brxgraph.h"

#define PNG_HEADER (8)

i32 round(double x)
{
    if (x < 0.0)
        return (i32)(x - 0.5);
    else
        return (i32)(x + 0.5);
}

u8 brx_image_pixel(png_bytep * data, png_byte colort, u32 x, u32 y)
{
    u8 r,  g,  b,  a;
       r = g = b = a = 0;
    switch (colort)
    {
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
    case PNG_COLOR_TYPE_PALETTE   :
    case PNG_COLOR_TYPE_GRAY      :
    default:
        printf("Wrong PNG type\n");
        exit(1);
    }

    return (u8)round((0.21 * r + 0.72 * g + 0.07 * b) * (255.0 / a)) & 0xFF;
}

BrxError BrxPictureFromPNG(BrxPicture ** picture, char * filename)
{
    BrxError exit_code = BRX_ERROR_SUCCESS;
    BrxPicture * result;
    png_byte header[PNG_HEADER];
    png_structp handler;
    png_infop info;
    FILE * file;
    BrxSize size, rsize;
    png_byte colort;
    png_bytep * data = NULL;
    png_uint_32 datah = 0;
    png_uint_32 y, x;

    BRX_CHECKPTR(picture);
    BRX_CHECKPTR(filename);

    file = fopen(filename, "rb");
    if (!file)
        return BRX_ERROR_USER_BADFILE;

    if (fread(header, 1, PNG_HEADER, file) != PNG_HEADER)
    {
       exit_code = BRX_ERROR_USER_BADPNGFILE;
       goto error;
    }

    if (png_sig_cmp(header, 0, PNG_HEADER))
    {
        exit_code = BRX_ERROR_USER_BADPNGFILE;
        goto error;
    }

    handler = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!handler)
    {
        exit_code = BRX_ERROR_USER_BADPNGFILE;
        goto error;
    }

    info = png_create_info_struct(handler);
    if (!info)
    {
        exit_code = BRX_ERROR_USER_BADPNGFILE;
        goto error;
    }

    if (setjmp(png_jmpbuf(handler)))
    {
        exit_code = BRX_ERROR_USER_BADPNGFILE;
        goto error;
    }

    png_init_io(handler, file);
    png_set_sig_bytes(handler, PNG_HEADER);
    png_read_info(handler, info);

    size.w = png_get_image_width(handler, info);
    size.h = png_get_image_height(handler, info);
    colort = png_get_color_type(handler, info);

    rsize.w = size.w + size.w % BRX_BRAILLE_WIDTH;
    rsize.h = size.h + size.h % BRX_BRAILLE_HEIGHT;

    exit_code = BrxPictureCreate(&result, &rsize);
    if (exit_code)
        goto error;

    if (setjmp(png_jmpbuf(handler)))
    {
        exit_code = BRX_ERROR_USER_BADPNGFILE;
        goto error;
    }

    data = calloc(sizeof(png_bytep), size.h);
    if (!data)
    {
        exit_code = BRX_ERROR_USER_BADALLOC;
        goto error;
    }

    datah = size.h;

    for (y = 0; y < size.h; y++)
    {
        data[y] = calloc(png_get_rowbytes(handler, info), 1);
        if (!data[y])
        {
            exit_code = BRX_ERROR_USER_BADALLOC;
            goto error;
        }
    }

    png_read_image(handler, data);

    for (y = 0; y < size.h; y++)
    for (x = 0; x < size.w; x++)
        brx_data_set(result, x, y,
                     rand() % 255 < brx_image_pixel(data, colort, x, y));

    *picture = result;

    error:
        png_destroy_read_struct(&handler, &info, NULL);
        fclose(file);
        while (datah--)
            free(data[datah]);
        free(data);
        return exit_code;
}
