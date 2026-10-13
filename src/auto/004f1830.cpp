#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, SceneLights_ParseSubEntry, 0x004F17B0, (void* self, void* edx, uint32_t* a2, int a3));

// Dispatches a scene-lights parse record by its leading tag dword (0, 1 or 2)
// to one of three embedded sub-parsers at this+0x2C, this+0x1C and this+0x0C.
// Any other tag returns the record pointer unchanged.
static uint32_t* __fastcall SceneLights_DispatchParseByTag(C_0083ef30* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t* result = a2;
    switch (*a2) {
    case 0:
        return SceneLights_ParseSubEntry(reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr, a2, 0);
    case 1:
        return SceneLights_ParseSubEntry(reinterpret_cast<uint8_t*>(self) + 0x1C, nullptr, a2, 0);
    case 2:
        return SceneLights_ParseSubEntry(reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr, a2, 0);
    }
    return result;
}

HOOK(0x4F1830, SceneLights_DispatchParseByTag, "auto");
