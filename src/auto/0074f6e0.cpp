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
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void, __stdcall, FinalizeSpawnUpdate, 0x0064AF90, (int a1, int a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(char, __fastcall, NetPlayerDdl_ApplyProperty, 0x0066CD80, (void* self, void* edx, int arg0, int a3, int a4, int a5, void* a6));

static int __fastcall NetProyectilDdl_ProcessField(void* self, void* /*edx*/, C_008622e8* a2, int a3)
{
    // Object index is the top 10 bits of the dword at +0x2C.
    uint32_t objectIndex = CSF_AT(uint32_t, self, 0x2C) >> 22;

    uint8_t propertyFlag = 1;
    // Output slot filled by NetCogibleDdl_FindProperty; the original does not
    // initialize it before the call, so neither do we.
    void* foundProperty;

    auto* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(
        LookupObjectByIndex(static_cast<int>(objectIndex))));

    NetCogibleDdl_FindProperty(owner + 0x0C, nullptr, &foundProperty, &propertyFlag);

    // The original loads [foundProperty+0x10] into ecx and uses it as the receiver.
    void* propertyValue = CSF_AT(void*, foundProperty, 0x10);
    NetPlayerDdl_ApplyProperty(
        propertyValue,
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(self) + 0x44)),
        1,
        a3,
        a2);

    FinalizeSpawnUpdate(static_cast<int>(reinterpret_cast<uintptr_t>(a2)), a3);
    return 0;
}

HOOK(0x0074F6E0, NetProyectilDdl_ProcessField, "auto");
