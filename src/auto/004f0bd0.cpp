#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, C_Unknown_StoreValueAt0058, 0x004D6B20, (void* self, void* , int value));

static void __cdecl SceneLights_ApplyObjectDefinition(int a1, int a2)
{
    if (a1 == 0)
        return;

    uint32_t objectHandle = CSF_AT(uint32_t, a1, 0x34);
    if (objectHandle == 0)
        return;

    void* obj = reinterpret_cast<void*>(objectHandle);
    void* typeFn = CSF_AT(void**, obj, 0)[38];
    int objectType = ((int(__fastcall*)(void*, void*))typeFn)(obj, nullptr);
    if (objectType != 7)
        return;

    uint8_t* globalState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    if (CSF_AT(uint8_t, globalState, 0xD0) == 0)
        return;

    uint8_t* globalsBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalDword_73C020()));
    if (CSF_AT(uint8_t, globalsBlock, 0xFC) == 0)
        return;

    int storedValue = static_cast<int>(CSF_AT(uint32_t, a2, 0x14C));
    void* self = reinterpret_cast<void*>(CSF_AT(uint32_t, a1, 0x34));
    C_Unknown_StoreValueAt0058(self, nullptr, storedValue);
}

HOOK(0x004F0BD0, SceneLights_ApplyObjectDefinition, "auto");
