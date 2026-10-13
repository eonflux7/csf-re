#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(int, __fastcall, PTank_GetPluginFieldPlus216, 0x006EED00, (S_006ada10* self, void*));
CSF_FUNC(int, __fastcall, GfxRenderEngine_SetResourceRectangle, 0x006EECB0, (S_006ada10* self, void*, uint32_t* a2));

// Computes the source rectangle (left, top, right, bottom as floats) of tile tileIndex
// in a tile sheet whose tile width is at +0x74 and tile height at +0x78, then hands it
// to the render engine.
static int __fastcall C_00862640_SetResourceRectForTileIndex(C_00862640* self, void* /*edx*/, int tileIndex)
{
    uint8_t* tileSheet = reinterpret_cast<uint8_t*>(self->field_0018);
    int tileWidth = CSF_AT(int, tileSheet, 0x74);
    int rowIndex = tileIndex / tileWidth;
    int columnIndex = tileIndex % tileWidth;

    float originX = (g_TransitionOne / (float)tileWidth) * (float)columnIndex;
    int tileHeight = CSF_AT(int, tileSheet, 0x78);
    float originY = (g_TransitionOne / (float)tileHeight) * (float)rowIndex;

    float rect[4];
    rect[0] = originX;
    rect[1] = originY;

    PTank_GetPluginFieldPlus216(reinterpret_cast<S_006ada10*>(self->field_0020), nullptr);

    tileSheet = reinterpret_cast<uint8_t*>(self->field_0018);
    rect[2] = (g_TransitionOne / (float)CSF_AT(int, tileSheet, 0x74)) + originX;
    rect[3] = (g_TransitionOne / (float)CSF_AT(int, tileSheet, 0x78)) + originY;

    return GfxRenderEngine_SetResourceRectangle(
        reinterpret_cast<S_006ada10*>(self->field_0020), nullptr,
        reinterpret_cast<uint32_t*>(rect));
}

HOOK(0x006F2CC0, C_00862640_SetResourceRectForTileIndex, "auto");
