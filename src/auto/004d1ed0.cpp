#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(char, __fastcall, GfxLights_ClearLightFlag, 0x006FD0C0, (void* self, void* edx, char a2));

static bool __fastcall S_004d1e30_ApplyParamsAndQuery(S_004d1e30* self, void* /*edx*/, int a2, float a3, int a4)
{
    if (a3 != g_fZero) {
        uint8_t* inner = reinterpret_cast<uint8_t*>(self->field_0014);
        uint8_t* sub = CSF_AT(uint8_t*, inner, 0x48);
        CSF_AT(float, sub, 0x7C) = a3;
    }

    if (a4 != 0) {
        uint8_t* inner = reinterpret_cast<uint8_t*>(self->field_0014);
        uint8_t* sub = CSF_AT(uint8_t*, inner, 0x48);
        char lightFlag = (a4 == 1) ? 1 : 0;
        GfxLights_ClearLightFlag(sub, nullptr, lightFlag);
    }

    uint8_t* target = reinterpret_cast<uint8_t*>(self->field_0014);
    int queryResult = ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, target, 0)[35])(
        target, nullptr, a2, 0, -1, 0);
    return queryResult != -1;
}

HOOK(0x4D1ED0, S_004d1e30_ApplyParamsAndQuery, "auto");
