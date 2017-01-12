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

#include "../graph/brxgraph.h"
#include "../definitions/brxcheck.h"
#include "../alphabet/brxalphabet.h"

void brx_data_set(BrxPicture * picture, u32 x, u32 y, u8 value)
{
    u32 Bi, bi;
    Bi = (x + y * picture->size.w) / BRX_BRAILLE_BITS;
    bi = (x + y * picture->size.w) % BRX_BRAILLE_BITS;

    if (value) picture->data[Bi] |=  (1 << bi);
          else picture->data[Bi] &= ~(1 << bi);
}


u8 brx_data_get(BrxPicture * picture, u32 x, u32 y)
{
    u32 Bi, bi;
    Bi = (x + y * picture->size.w) / BRX_BRAILLE_BITS;
    bi = (x + y * picture->size.w) % BRX_BRAILLE_BITS;

    return 1 & (picture->data[Bi] >> bi);
}

u8 brx_character_get(BrxPicture * picture, u32 x, u32 y)
{
    return (brx_data_get(picture, x + 0, y + 0) << 0) |
           (brx_data_get(picture, x + 0, y + 1) << 1) |
           (brx_data_get(picture, x + 0, y + 2) << 2) |
           (brx_data_get(picture, x + 1, y + 0) << 3) |
           (brx_data_get(picture, x + 1, y + 1) << 4) |
           (brx_data_get(picture, x + 1, y + 2) << 5) |
           (brx_data_get(picture, x + 0, y + 3) << 6) |
           (brx_data_get(picture, x + 1, y + 3) << 7) ;
}

u8 brx_mode_dst (u8 src, u8 dst) { (void)src; return         dst;  }
u8 brx_mode_src (u8 src, u8 dst) { (void)dst; return   src;        }
u8 brx_mode_ndst(u8 src, u8 dst) { (void)src; return         !dst; }
u8 brx_mode_nsrc(u8 src, u8 dst) { (void)dst; return ! src;        }
u8 brx_mode_or  (u8 src, u8 dst) {            return   src | dst;  }
u8 brx_mode_and (u8 src, u8 dst) {            return   src & dst;  }
u8 brx_mode_xor (u8 src, u8 dst) {            return   src ^ dst;  }
u8 brx_mode_nor (u8 src, u8 dst) {            return !(src | dst); }
u8 brx_mode_nand(u8 src, u8 dst) {            return !(src & dst); }

typedef u8 (*brx_mode_func)(u8 src, u8 dst);

brx_mode_func brx_modes[BRX_MODE_LAST] =
{
#   define BRX(NAME, FUNC) FUNC,

    BRX_X_MODES_LIST
#   undef BRX
};

BrxError BrxPictureDraw(BrxPicture * picture, FILE * handler)
{
    u32 x, y;

    BRX_CHECKPTR(handler);
    BRX_CHECKPTR(picture);

    if (picture->size.w % BRX_BRAILLE_WIDTH)
        return BRX_ERROR_USER_BADWIDTH;
    if (picture->size.h % BRX_BRAILLE_HEIGHT)
        return BRX_ERROR_USER_BADHEIGHT;
    if (!picture->data)
        return BRX_ERROR_USER_BADPOINTER;

    for (y = 0; y < picture->size.h; y += BRX_BRAILLE_HEIGHT,
                                     fprintf(handler, "\n"))
    for (x = 0; x < picture->size.w; x += BRX_BRAILLE_WIDTH)
        fprintf(handler, "%s",
                (char *)&brx_alphabet[
                             brx_character_get(
                                 picture, x, y)]);

    return BRX_ERROR_SUCCESS;
}

BrxError BrxPicturePixelSet(BrxPicture * picture, BrxPoint * point, u8 value)
{
    BRX_CHECKPTR(point);
    BRX_CHECKPTR(picture);

    if (point->x >= (i32)picture->size.w)
        return BRX_ERROR_USER_BADX;
    if (point->y >= (i32)picture->size.h)
        return BRX_ERROR_USER_BADY;

    brx_data_set(picture, point->x, point->y, !!value);

    return BRX_ERROR_SUCCESS;
}

BrxError BrxPicturePixelGet(BrxPicture * picture, BrxPoint * point, u8 * value)
{
    BRX_CHECKPTR(point);
    BRX_CHECKPTR(picture);

    if (point->x >= (i32)picture->size.w)
        return BRX_ERROR_USER_BADX;
    if (point->y >= (i32)picture->size.h)
        return BRX_ERROR_USER_BADY;

    *value = brx_data_get(picture, point->x, point->y);

    return BRX_ERROR_SUCCESS;
}

BrxError BrxPictureMap(BrxPicture * dst, BrxPicture * src, BrxPoint * point, BrxMode mode)
{
    i32 ystart, ystop;
    i32 xdst, xsrc;
    i32 count;
    i32 lx, ly;

    BRX_CHECKPTR(dst);
    BRX_CHECKPTR(src);
    BRX_CHECKPTR(point);

    ystart = point->y < 0 ? 0 : point->y;
    ystop  = src->size.h + point->y > dst->size.h ?
             dst->size.h : point->y + src->size.h ;
    xdst   = point->x < 0 ? 0 : point->x;
    xsrc   = point->x < 0 ? -point->x : 0;

    if (point->x < 0)
    {
        if ((point->x + src->size.w) > dst->size.w)
                 count = dst->size.w;
            else count = src->size.w + point->x;
    }
    else
    {
        if ((point->x + src->size.w) > dst->size.w)
                 count = dst->size.w - point->x;
            else count = src->size.w;
    }

    for (ly = ystart; ly < ystop; ly++)
    for (lx = 0;      lx < count; lx++)
    {
        u8 vdst, vsrc;
        vsrc = brx_data_get(src, lx + xsrc, ly - ystart);
        vdst = brx_data_get(dst, lx + xdst, ly);
        brx_data_set(dst, lx + xdst, ly,
                     brx_modes[mode](vsrc, vdst));
    }

    return BRX_ERROR_SUCCESS;
}

