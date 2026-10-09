#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, sub_469710, 0x00469710, (int a1, int a2));
CSF_FUNC(int, __cdecl, sub_46C190, 0x0046C190, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, sub_46C620, 0x0046C620, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, sub_4714E0, 0x004714E0, (void* Block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static uint32_t* __fastcall MissionScript_ClearOwnedData(void* self, void* /*edx*/)
{
    auto* bytes = static_cast<uint8_t*>(self);
    int lastChild = 0;

    for (uint32_t index = 0;; ++index)
    {
        const uint32_t begin = CSF_AT(uint32_t, self, 0x0C);
        if (!begin)
            break;

        const uint32_t count = static_cast<uint32_t>(
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x10) - begin) >> 3);
        if (index >= count)
            break;

        auto* entry = CSF_AT(S_0069b1c0*,
                            CSF_AT(void*, self, 0x0C), index * 8 + 4);
        if (entry)
        {
            auto* child = reinterpret_cast<S_0069b1c0*>(entry->field_0004);
            lastChild = static_cast<int>(reinterpret_cast<uintptr_t>(child));
            if (child)
            {
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, child, 0)[0])(child, nullptr, 0);
                sub_4714E0(child);
            }
            entry->field_0004 = 0;
            sub_4714E0(entry);
        }

        CSF_AT(uint32_t, CSF_AT(void*, self, 0x0C), index * 8 + 4) = 0;
    }

    auto* owned = CSF_AT(S_0069b1c0*, self, 0x28);
    if (owned)
    {
        if (owned->field_0008)
        {
            sub_469710(static_cast<int>(owned->field_0008),
                       static_cast<int>(owned->field_000c));
            if (owned->field_0008)
                Mem_FreeDebug(reinterpret_cast<void*>(owned->field_0008));
        }
        owned->field_0008 = 0;
        owned->field_000c = 0;
        CSF_AT(uint32_t, owned, 0x10) = 0;
        sub_4714E0(owned);
    }
    CSF_AT(uint32_t, self, 0x28) = 0;

    if (CSF_AT(void*, self, 0x74))
        Mem_FreeDebug(CSF_AT(void*, self, 0x74));
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;

    if (CSF_AT(void*, self, 0x60))
        Mem_FreeDebug(CSF_AT(void*, self, 0x60));
    CSF_AT(uint32_t, self, 0x60) = 0;
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint32_t, self, 0x68) = 0;

    if (CSF_AT(void*, self, 0x50))
        Mem_FreeDebug(CSF_AT(void*, self, 0x50));
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;

    if (CSF_AT(void*, self, 0x40))
        Mem_FreeDebug(CSF_AT(void*, self, 0x40));
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;

    if (CSF_AT(int, self, 0x30))
    {
        sub_46C190(CSF_AT(int, self, 0x30),
                   CSF_AT(int, self, 0x34),
                   static_cast<int>(reinterpret_cast<uintptr_t>(bytes + 0x2C)),
                   lastChild);
        if (CSF_AT(void*, self, 0x30))
            Mem_FreeDebug(CSF_AT(void*, self, 0x30));
    }
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;

    if (CSF_AT(int, self, 0x1C))
    {
        sub_46C190(CSF_AT(int, self, 0x1C),
                   CSF_AT(int, self, 0x20),
                   static_cast<int>(reinterpret_cast<uintptr_t>(bytes + 0x18)),
                   lastChild);
        if (CSF_AT(void*, self, 0x1C))
            Mem_FreeDebug(CSF_AT(void*, self, 0x1C));
    }
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;

    if (CSF_AT(int, self, 0x0C))
    {
        sub_46C620(CSF_AT(int, self, 0x0C),
                   CSF_AT(int, self, 0x10),
                   static_cast<int>(reinterpret_cast<uintptr_t>(bytes + 8)),
                   lastChild);
        if (CSF_AT(void*, self, 0x0C))
            Mem_FreeDebug(CSF_AT(void*, self, 0x0C));
    }
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;

    return reinterpret_cast<uint32_t*>(bytes + 8);
}

HOOK(0x00471130, MissionScript_ClearOwnedData, "auto");
