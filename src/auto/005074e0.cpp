#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(uint32_t, g_value_8B056C, 0x008B056C);

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GameHeap_Initialize, 0x006912A0, ());

// Lazily creates the 52-byte singleton state object (once), stores it in the
// global slot and returns the slot value (object pointer, or 0 on allocation failure).
static int SceneLoader_GetOrCreateStateObject()
{
    if (g_value_8B056C != 0)
        return static_cast<int>(g_value_8B056C);

    GameHeap_Initialize();

    void* obj = Mem_Alloc(52);
    if (obj != nullptr)
    {
        CSF_AT(uint32_t, obj, 0x28) = 0;
        CSF_AT(uint32_t, obj, 0x2C) = 0;
        CSF_AT(uint32_t, obj, 0x30) = 0;
        CSF_AT(uint32_t, obj, 0x00) = 0;
        CSF_AT(uint32_t, obj, 0x04) = 0;
        CSF_AT(uint32_t, obj, 0x08) = 0;
        CSF_AT(uint32_t, obj, 0x14) = 0;
        CSF_AT(uint32_t, obj, 0x18) = 0;
        CSF_AT(uint32_t, obj, 0x1C) = 0;
        CSF_AT(uint8_t, obj, 0x20) = 0;
        CSF_AT(uint8_t, obj, 0x21) = 0;
        CSF_AT(uint32_t, obj, 0x10) = 0x88EE5500u;
        g_value_8B056C = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(obj));
    }
    else
    {
        g_value_8B056C = 0;
    }

    SceneLoader_NoOp();
    return static_cast<int>(g_value_8B056C);
}

HOOK(0x005074E0, SceneLoader_GetOrCreateStateObject, "auto");
