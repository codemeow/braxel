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

#ifndef BRXTYPES_H
#define BRXTYPES_H

#include "../definitions/brxexport.h"
#include "../xmacro/brxxerrors.h"
#include "../xmacro/brxxmodes.h"

EXPORT_FROM
#define BRX_BRAILLE_WIDTH   (2)
#define BRX_BRAILLE_HEIGHT  (4)
#define BRX_BRAILLE_COUNT (256)
#define BRX_BRAILLE_BITS (BRX_BRAILLE_WIDTH * BRX_BRAILLE_HEIGHT)
EXPORT_TO

EXPORT typedef   signed      char  i8;
EXPORT typedef unsigned      char  u8;
EXPORT typedef   signed short int i16;
EXPORT typedef unsigned short int u16;
EXPORT typedef   signed       int i32;
EXPORT typedef unsigned       int u32;

EXPORT_FROM
#define  U8_MIN (0)
#define U16_MIN (0)
#define U32_MIN (0)

#define  U8_MAX (0xFFU)
#define U16_MAX (0xFFFFU)
#define U32_MAX (0xFFFFFFFFU)

#define  I8_MIN (-128)
#define I16_MIN (-32768)
#define I32_MIN (-2147483648)

#define  I8_MAX (127)
#define I16_MAX (32767)
#define I32_MAX (2147483647)
EXPORT_TO

EXPORT typedef struct
{
    i32 x;
    i32 y;
} BrxPoint;

EXPORT typedef struct
{
    u32 w;
    u32 h;
} BrxSize;

EXPORT typedef struct
{
    BrxSize size;
    u8 *    data;
} BrxPicture;

EXPORT_FROM
#define BRX(NAME, CODE, DESC) BRX_ERROR_ ## NAME = (CODE),
typedef enum
{
    BRX_X_ERRORS_LIST
    BRX_ERROR_LAST
} BrxError;
#undef BRX
EXPORT_TO

EXPORT_FROM
#define BRX(NAME, FUNC) BRX_MODE_ ## NAME,
typedef enum
{
    BRX_X_MODES_LIST
    BRX_MODE_LAST
} BrxMode;
#undef BRX
EXPORT_TO

#endif /* BRXTYPES_H */

