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

#include "../alphabet/brxalphabet.h"

u32 brx_alphabet[BRX_BRAILLE_COUNT] = { 0 };

BrxError BrxAlphabetInit(void)
{
    u32 x;
    for (x = 0; x < BRX_BRAILLE_COUNT; x++)
        brx_alphabet[x] =
            0xE2 | ((0xA0 + (x >> 6)) << 8) |
            (((x & 0xF) + ((x % 0x40) & 0xF0) + 0x80) << 16);

    return BRX_ERROR_SUCCESS;
}
