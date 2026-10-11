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

// Bound original callees (by address).
CSF_FUNC(bool, __fastcall, NetSession_IsDuplicate, 0x0064B5C0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, NetCogible_EnqueueRecord, 0x00741DE0, (void* self, void* edx, uint32_t* a2));

// Queues a 40-byte network record (9 stack args, callee pops 36 bytes).
static char __fastcall C_00858180_EnqueueNetRecord(C_00858180* self, void* /*edx*/,
                                                   int a2, int a3, int a4, int a5,
                                                   int a6, int a7, int a8, char a9, int a10)
{
    int globalDword = GetGlobalDword_73C020();
    int result = globalDword;

    if (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(globalDword), 0x104))
    {
        result = NetSession_IsDuplicate(self, nullptr);
        if (static_cast<char>(result))
        {
            void* mem = Mem_Alloc(40);
            uint32_t entryHandle = reinterpret_cast<uint32_t>(mem);
            result = static_cast<int>(entryHandle);
            if (mem)
            {
                uint8_t* entry = reinterpret_cast<uint8_t*>(mem);
                CSF_AT(uint32_t, entry, 0x18) = static_cast<uint32_t>(a2);
                CSF_AT(uint32_t, entry, 0x0C) = static_cast<uint32_t>(a6);
                CSF_AT(uint32_t, entry, 0x10) = static_cast<uint32_t>(a7);
                CSF_AT(uint32_t, entry, 0x14) = static_cast<uint32_t>(a8);
                CSF_AT(uint32_t, entry, 0x00) = static_cast<uint32_t>(a3);
                CSF_AT(uint32_t, entry, 0x04) = static_cast<uint32_t>(a4);
                CSF_AT(uint32_t, entry, 0x08) = static_cast<uint32_t>(a5);
                CSF_AT(uint8_t, entry, 0x1C) = 1;
                CSF_AT(uint8_t, entry, 0x1D) = static_cast<uint8_t>(a9);
                CSF_AT(uint32_t, entry, 0x20) = static_cast<uint32_t>(a10);
                CSF_AT(uint8_t, entry, 0x24) = 0;

                result = NetCogible_EnqueueRecord(
                    reinterpret_cast<uint8_t*>(self) + 0x6D0, nullptr, &entryHandle);
            }
        }
    }
    return static_cast<char>(result);
}

HOOK(0x741EB0, C_00858180_EnqueueNetRecord, "auto");
