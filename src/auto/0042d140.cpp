#include "globals.h"
#include "hook.h"

// Sets the rotation angle (degrees) of the drawable at +0x08.
CSF_FUNC(int, __cdecl, Hud_SetDrawableRotation, 0x0042E460, (int a1, float a2));

// Size source at this+0x04: virtual slot 15 returns a pointer to its {width, height} pair.
static int* Hud_QuerySizePair(void* sizeSource)
{
    return ((int* (__fastcall*)(void*, void*))CSF_AT(void**, sizeSource, 0)[15])(sizeSource, nullptr);
}

// Drawable at this+0x08: virtual slot 10 sets its position from an {x, y} pair.
static void Hud_SetDrawablePosition(void* drawable, int* position)
{
    ((void (__fastcall*)(void*, void*, int*))CSF_AT(void**, drawable, 0)[10])(drawable, nullptr, position);
}

// Drawable at this+0x08: virtual slot 14 sets its size from a {width, height} pair.
static void Hud_SetDrawableSize(void* drawable, int* size)
{
    ((void (__fastcall*)(void*, void*, int*))CSF_AT(void**, drawable, 0)[14])(drawable, nullptr, size);
}

// Drawable at this+0x08: virtual slot 2 commits the pending changes.
static int Hud_CommitDrawable(void* drawable)
{
    return ((int (__fastcall*)(void*, void*))CSF_AT(void**, drawable, 0)[2])(drawable, nullptr);
}

// Lays out four rotated copies of the drawable as a border around the rect
// (top 0 deg, bottom 180 deg, left -90 deg, right 90 deg) and commits the last one.
// Rect: left @0x10, top @0x14, right @0x18, bottom @0x1C.
static int __fastcall Hud_LayoutRectBorderFrame(void* self, void* /*edx*/)
{
    int pos[2];   // v22 (x), v23 (y)
    int size[2];  // v20 (w), v21 (h)

    int* sizeInfo = Hud_QuerySizePair(CSF_AT(void*, self, 0x04));
    pos[0] = sizeInfo[0];
    pos[1] = 0;

    int rectWidth = CSF_AT(int, self, 0x18) - CSF_AT(int, self, 0x10);
    int sourceHeight = Hud_QuerySizePair(CSF_AT(void*, self, 0x04))[1];
    size[0] = rectWidth - 2 * Hud_QuerySizePair(CSF_AT(void*, self, 0x04))[0];
    size[1] = sourceHeight;

    Hud_SetDrawablePosition(CSF_AT(void*, self, 0x08), pos);
    Hud_SetDrawableSize(CSF_AT(void*, self, 0x08), size);
    Hud_SetDrawableRotation(reinterpret_cast<int>(CSF_AT(void*, self, 0x08)), 0.0f);
    Hud_CommitDrawable(CSF_AT(void*, self, 0x08));

    pos[1] = CSF_AT(int, self, 0x1C) - CSF_AT(int, self, 0x14) - size[1];
    Hud_SetDrawablePosition(CSF_AT(void*, self, 0x08), pos);
    Hud_SetDrawableRotation(reinterpret_cast<int>(CSF_AT(void*, self, 0x08)), 180.0f);
    Hud_CommitDrawable(CSF_AT(void*, self, 0x08));

    int rectHeight = CSF_AT(int, self, 0x1C) - CSF_AT(int, self, 0x14);
    int* sizeInfoHeight = Hud_QuerySizePair(CSF_AT(void*, self, 0x04));
    size[0] = rectHeight - 2 * sizeInfoHeight[1];
    size[1] = Hud_QuerySizePair(CSF_AT(void*, self, 0x04))[0];

    pos[0] = size[1] / 2 - size[0] / 2;
    pos[1] = (CSF_AT(int, self, 0x1C) - CSF_AT(int, self, 0x14)) / 2 - size[1] / 2;
    Hud_SetDrawablePosition(CSF_AT(void*, self, 0x08), pos);
    Hud_SetDrawableSize(CSF_AT(void*, self, 0x08), size);
    Hud_SetDrawableRotation(reinterpret_cast<int>(CSF_AT(void*, self, 0x08)), -90.0f);
    Hud_CommitDrawable(CSF_AT(void*, self, 0x08));

    int rectWidthRight = CSF_AT(int, self, 0x18) - CSF_AT(int, self, 0x10);
    int* sizeInfoRight = Hud_QuerySizePair(CSF_AT(void*, self, 0x04));
    pos[0] += rectWidthRight - sizeInfoRight[0];
    Hud_SetDrawablePosition(CSF_AT(void*, self, 0x08), pos);
    Hud_SetDrawableRotation(reinterpret_cast<int>(CSF_AT(void*, self, 0x08)), 90.0f);
    return Hud_CommitDrawable(CSF_AT(void*, self, 0x08));
}

HOOK(0x42D140, Hud_LayoutRectBorderFrame, "auto");
