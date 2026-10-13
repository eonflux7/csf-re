#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global data referenced by the original (text-input default source buffer).
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Callee: text-input subobject initializer (__thiscall, one stack arg, ret 4).
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130,
         (S_004f21d0* self, void* /*edx*/, uint8_t* a2));

// Resets the 30-entry DDL object table embedded in the manager and clears the two trailing counters.
static uint32_t* __fastcall DdlObjectManager_ResetEntries(S_0074a3e0* self, void* /*edx*/)
{
    // Entries start at offset 0x2C with a stride of 0x30 bytes; 30 entries total.
    int entryOffset = 0x2C;
    int remaining = 30;
    do
    {
        CSF_AT(int32_t, self, entryOffset - 0x28) = -1;
        TextInputSubobject_Init(
            reinterpret_cast<S_004f21d0*>(reinterpret_cast<uint8_t*>(self) + entryOffset - 0x24),
            nullptr,
            &g_byte_82E65C);

        CSF_AT(uint8_t, self, entryOffset + 0) = 0;
        CSF_AT(uint8_t, self, entryOffset + 1) = 0;
        CSF_AT(int32_t, self, entryOffset - 8) = -1;
        CSF_AT(uint8_t, self, entryOffset + 2) = 0;
        CSF_AT(uint8_t, self, entryOffset + 3) = 0;
        CSF_AT(uint8_t, self, entryOffset + 4) = 0xFF;
        CSF_AT(int32_t, self, entryOffset - 4) = 0;

        entryOffset += 0x30;
        --remaining;
    } while (remaining != 0);

    CSF_AT(int32_t, self, 0x5A4) = 0;
    CSF_AT(int32_t, self, 0x5A8) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0074A3E0, DdlObjectManager_ResetEntries, "auto");
