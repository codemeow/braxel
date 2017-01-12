#include <stdio.h>
#include <libbraxel.h>

#define TEST(X) printf("%60.60s : %s\n", #X, BrxErrorDescription(X));

void dump(BrxPicture * picture)
{
    u32 x, y;
    for (y = 0; y < picture->size.h; y++, printf("\n"))
    for (x = 0; x < picture->size.w; x++, printf("%02X", picture->data[x + y * picture->size.w]));
}

void goto00(void)
{
    printf("\033[3;0H");
}

void draw(BrxPicture * picture)
{
    goto00();
    BrxPictureDraw(picture, stdout);
}

void invert(BrxPicture * picture, BrxPoint * point)
{
    u8 pix;
    BrxPicturePixelGet(picture, point, &pix);
    pix = !pix;
    BrxPicturePixelSet(picture, point, pix);
}

void animate(BrxPicture * picture, i32 count)
{
    while (count--)
    {
        BrxPoint p;

        for (p.y = 0; p.y < (i32)picture->size.h; p.y++, draw(picture))
            for (p.x = 0; p.x < (i32)picture->size.w; p.x++)
                invert(picture, &p);

        for (p.x = 0; p.x < (i32)picture->size.w; p.x++, draw(picture))
            for (p.y = 0; p.y < (i32)picture->size.h; p.y++)
                invert(picture, &p);

        for (p.y = (i32)picture->size.h - 1; p.y >= 0 ; p.y--, draw(picture))
            for (p.x = (i32)picture->size.w - 1; p.x >= 0; p.x--)
                invert(picture, &p);

        for (p.x = (i32)picture->size.w - 1; p.x >= 0; p.x--, draw(picture))
            for (p.y = (i32)picture->size.h - 1; p.y >= 0; p.y--)
                invert(picture, &p);
    }
}

i32 main(void)
{
    BrxPicture * tiger = NULL;
    BrxPicture * earth = NULL;

    BrxPoint p = { 0 };

    TEST(BrxInit());
    TEST(BrxPictureFromPNG(&tiger, "resources/tiger.png"));
    TEST(BrxPictureDraw(tiger, stdout));

    animate(tiger, 1);

    TEST(BrxPictureFromPNG(&earth, "resources/earth.png"));
    TEST(BrxPictureDraw(earth, stdout));
    TEST(BrxPictureMap(tiger, earth, &p, BRX_MODE_OR));
    TEST(BrxPictureDraw(tiger, stdout));

    TEST(BrxPictureDestroy(&tiger));
    TEST(BrxPictureDestroy(&earth));

    return 0;
}
