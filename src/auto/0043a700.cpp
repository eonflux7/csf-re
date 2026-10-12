#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Converts a signed int to float, adding the unsigned bias when negative (as the original does).
static float IntToFloatUnsignedBias(int value)
{
    float f = (float)value;
    if (value < 0)
        f += g_UnsignedIntegerBias;
    return f;
}

static int __fastcall MinimapRadar_EnsureRadarObject(C_00830d68* self, void* /*edx*/)
{
    int result = (int)self->field_001c;
    if (result != 0)
        return result;

    void* menuObj = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    void** vt = CSF_AT(void**, menuObj, 0);
    void* radarObj = ((void*(__fastcall*)(void*, void*, int))vt[0x5C / 4])(menuObj, nullptr, 0);
    self->field_001c = (uint32_t)(uintptr_t)radarObj;

    int size = (int)self->field_0020;
    int arg9 = size * 9;
    int arg2 = size * 2 + 1;
    ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, radarObj, 0)[0x1C / 4])(radarObj, nullptr, arg2, arg9);

    void* obj = reinterpret_cast<void*>(self->field_001c);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0x20 / 4])(obj, nullptr, 1);

    float color[4];
    int packed = CSF_AT(int, (void*)(intptr_t)GetGlobalValue_8BC1E0(), 0x1D90);
    CSF_AT(int, color, 12) = 0x3F800000;
    color[0] = IntToFloatUnsignedBias((packed >> 16) & 0xFF);
    color[1] = IntToFloatUnsignedBias((packed >> 8) & 0xFF);
    color[2] = IntToFloatUnsignedBias(packed & 0xFF);

    int alphaBits = CSF_AT(int, (void*)(intptr_t)GetGlobalValue_8BC1E0(), 0x1D94);
    obj = reinterpret_cast<void*>(self->field_001c);
    CSF_AT(int, color, 12) = alphaBits;

    return ((int(__fastcall*)(void*, void*, float*))CSF_AT(void**, obj, 0)[0x50 / 4])(obj, nullptr, color);
}

HOOK(0x0043A700, MinimapRadar_EnsureRadarObject, "auto");
