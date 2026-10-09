#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_48E100, 0x0048E100, (void* self, void* edx));

// __stdcall, 3 stack args, callee pops 12 bytes.
// a3 -> pointer to a record; record+4 -> entry; entry+12 = signed table index; entry+4 = handle.
// a1+4 -> output object; writes a success flag byte at +1.
static int __stdcall Scene_SetResolvedHandleFlag(int a1, int /*a2*/, int a3)
{
    const uint32_t record = CSF_AT(uint32_t, reinterpret_cast<void*>(a3), 0);
    const uint32_t entry = CSF_AT(uint32_t, reinterpret_cast<void*>(record), 4);
    const int32_t entryIndex = CSF_AT(int32_t, reinterpret_cast<void*>(entry), 12);
    const uint32_t handleValue = entry + 4;

    if (entryIndex < 0)
    {
        const uint32_t flagOwner = CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4);
        CSF_AT(uint8_t, reinterpret_cast<void*>(flagOwner), 1) = 0;
        return static_cast<int>(flagOwner);
    }

    const int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleValue));
    if (resolved == 0 || (sub_48E100(reinterpret_cast<void*>(resolved), nullptr) & 0xFF) != 0)
    {
        const uint32_t flagOwner = CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4);
        CSF_AT(uint8_t, reinterpret_cast<void*>(flagOwner), 1) = 0;
        return 0;
    }

    const uint32_t flagOwner = CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 4);
    CSF_AT(uint8_t, reinterpret_cast<void*>(flagOwner), 1) = 1;
    return 1;
}

HOOK(0x45CF30, Scene_SetResolvedHandleFlag, "auto");
