#include <stdio.h>
#include <libbraxel.h>

i32 main(i32 argc, char * argv[])
{
    BrxPicture * deception = NULL;
    BrxInit();
    BrxPictureFromPNG(&deception, argv[1]);
    BrxPictureDraw(deception, stdout);
    BrxPictureDestroy(&deception);

    return 0;
}
