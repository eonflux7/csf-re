#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall S_004fa900_DrawTextLine(
    S_004fa900* self, void* /*edx*/, int x, int y, int* line)
{
    alignas(8) uint8_t colorBytes[0x18] = {};
    alignas(8) uint8_t geometryBytes[0x8];
    float* color = reinterpret_cast<float*>(colorBytes);
    int* geometry = reinterpret_cast<int*>(geometryBytes);

    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 170.0f;

    switch (*line)
    {
    case 0:
        color[4] = 255.0f;
        color[5] = 255.0f;
        color[0] = 255.0f;
        color[1] = 255.0f;
        color[2] = 255.0f;
        color[3] = 255.0f;
        break;
    case 1:
        color[4] = 255.0f;
        color[5] = 0.0f;
        color[0] = 255.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 255.0f;
        break;
    case 2:
        color[4] = 0.0f;
        color[5] = 255.0f;
        color[0] = 0.0f;
        color[1] = 255.0f;
        color[2] = 0.0f;
        color[3] = 255.0f;
        break;
    case 3:
        color[4] = 228.0f;
        color[5] = 236.0f;
        color[0] = 228.0f;
        color[1] = 236.0f;
        color[2] = 16.0f;
        color[3] = 255.0f;
        break;
    default:
        break;
    }

    geometry[0] = x;
    geometry[1] = y;
    void* textObject = reinterpret_cast<void*>(self->field_0190);
    ((void(__fastcall*)(void*, void*, int*))
        CSF_AT(void**, textObject, 0)[80 / 4])
        (textObject, nullptr, geometry);

    void* manager = reinterpret_cast<void*>(GetGlobalInt());
    void* display = LookupObjectById(manager, nullptr);
    void* dimensions = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, display, 0)[24 / 4])(display, nullptr);

    geometry[0] = CSF_AT(int, dimensions, 0) - 10;
    geometry[1] = 20;
    textObject = reinterpret_cast<void*>(self->field_0190);
    ((void(__fastcall*)(void*, void*, int*))
        CSF_AT(void**, textObject, 0)[88 / 4])
        (textObject, nullptr, geometry);

    textObject = reinterpret_cast<void*>(self->field_0190);
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, textObject, 0)[96 / 4])
        (textObject, nullptr, color);

    textObject = reinterpret_cast<void*>(self->field_0190);
    ((void(__fastcall*)(void*, void*, int*))
        CSF_AT(void**, textObject, 0)[64 / 4])
        (textObject, nullptr, line + 1);

    textObject = reinterpret_cast<void*>(self->field_0190);
    void* font = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, textObject, 0)[40 / 4])(textObject, nullptr);

    textObject = reinterpret_cast<void*>(self->field_0190);
    void** savedTextVtable = CSF_AT(void**, textObject, 0);
    int fontValue = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, font, 0)[44 / 4])(font, nullptr);

    textObject = reinterpret_cast<void*>(self->field_0190);
    ((void(__fastcall*)(void*, void*, int))savedTextVtable[152 / 4])
        (textObject, nullptr, fontValue);

    textObject = reinterpret_cast<void*>(self->field_0190);
    return ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, textObject, 0)[8 / 4])(textObject, nullptr);
}

HOOK(0x004FA900, S_004fa900_DrawTextLine, "auto");
