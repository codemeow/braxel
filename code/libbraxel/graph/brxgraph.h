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

#ifndef BRXGRAPH_H
#define BRXGRAPH_H

#include <stdio.h>

#include "../definitions/brxexport.h"
#include "../definitions/brxtypes.h"

EXPORT BrxError BrxPictureDraw    (BrxPicture  * picture, FILE * handler);
EXPORT BrxError BrxPicturePixelSet(BrxPicture  * picture, BrxPoint * point, u8   value);
EXPORT BrxError BrxPicturePixelGet(BrxPicture  * picture, BrxPoint * point, u8 * value);

EXPORT BrxError BrxPictureMap     (BrxPicture  * dst, BrxPicture * src, BrxPoint * point, BrxMode mode);

void brx_data_set(BrxPicture * picture, u32 x, u32 y, u8 value);

#endif /* BRXGRAPH_H */

