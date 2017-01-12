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

#include <stdlib.h>

#include "../base/brxbase.h"
#include "../definitions/brxcheck.h"
#include "../alphabet/brxalphabet.h"

BrxError BrxInit(void)
{
    BRX_CHECKERR(BrxAlphabetInit());

    return BRX_ERROR_SUCCESS;
}

BrxError BrxPictureCreate(BrxPicture ** picture, BrxSize * size)
{
    BrxPicture * brx;

    BRX_CHECKPTR(picture);

    brx = calloc(sizeof(BrxPicture), 1);
    brx->size.w = size->w + size->w % BRX_BRAILLE_WIDTH;
    brx->size.h = size->h + size->h % BRX_BRAILLE_HEIGHT;
    brx->data = calloc((brx->size.w * brx->size.h) / BRX_BRAILLE_BITS, 1);

    if (!brx->data)
        return BRX_ERROR_USER_BADALLOC;

    *picture = brx;

    return BRX_ERROR_SUCCESS;
}

BrxError BrxPictureDestroy(BrxPicture ** picture)
{
    BrxPicture * brx;

    BRX_CHECKPTR(picture);
    BRX_CHECKPTR(*picture);

    brx = *picture;

    free(brx->data);
    free(brx);
    *picture = NULL;

    return BRX_ERROR_SUCCESS;
}
