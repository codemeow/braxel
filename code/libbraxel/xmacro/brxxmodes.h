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

#ifndef BRXXMODES_H
#define BRXXMODES_H

#include "../definitions/brxexport.h"

EXPORT_FROM
#define BRX_X_MODES_LIST     \
    BRX(DST,  brx_mode_dst ) \
    BRX(SRC,  brx_mode_src ) \
    BRX(NDST, brx_mode_ndst) \
    BRX(NSRC, brx_mode_nsrc) \
    BRX(OR,   brx_mode_or  ) \
    BRX(AND,  brx_mode_and ) \
    BRX(XOR,  brx_mode_xor ) \
    BRX(NOR,  brx_mode_nor ) \
    BRX(NAND, brx_mode_nand)
EXPORT_TO


#endif /* BRXXMODES_H */

