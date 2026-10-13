#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

CSF_GLOBAL(int, g_Value_8B066C, 0x008B066C);

// Lazily creates the singleton C_00830438 (36-byte object) and returns it.
static C_00830438* C_00830438_GetSingleton() {
    C_00830438* result = reinterpret_cast<C_00830438*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(g_Value_8B066C)));
    if (result == nullptr) {
        int allocated = CoreMemory_AllocUntagged(36);
        std::uint8_t* obj = reinterpret_cast<std::uint8_t*>(
            static_cast<std::uintptr_t>(static_cast<std::uint32_t>(allocated)));
        if (obj != nullptr) {
            result = reinterpret_cast<C_00830438*>(C_00830438_Construct(obj, nullptr));
            *reinterpret_cast<void***>(obj) = CSF_VTBL(0x00840CF8);
            CSF_AT(int, obj, 0x14) = 0;
            CSF_AT(int, obj, 0x18) = 0;
            CSF_AT(int, obj, 0x1C) = 0;
            CSF_AT(int, obj, 0x20) = 10;
            CSF_AT(std::uint8_t, obj, 0x0C) = 0;
            g_Value_8B066C = static_cast<int>(reinterpret_cast<std::uintptr_t>(obj));
        } else {
            g_Value_8B066C = 0;
        }
    }
    return result;
}

HOOK(0x00519550, C_00830438_GetSingleton, "auto");
