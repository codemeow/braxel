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

#ifndef BRXBASE_H
#define BRXBASE_H

#include "../definitions/brxtypes.h"

EXPORT BrxError BrxInit(void);

EXPORT BrxError BrxPictureCreate  (BrxPicture ** picture, BrxSize *size);
EXPORT BrxError BrxPictureDestroy (BrxPicture ** picture);

/* BrxError BrxPictureCopy (BrxPicture * src, BrxPicture ** dst); */
/* BrxError BrxPictureResize(BrxPicture * src, BrxSize * newsize); */

#endif /* BRXBASE_H */

