#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// DestroyRecord: destroys the embedded record object at this+0x6C.
CSF_FUNC(unsigned int, __fastcall, DestroyRecord_43E300, 0x0043E300, (std::uint32_t* self, void* edx));
// Game heap free (cdecl, one argument).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets the save-game record: releases the buffers it owns and restores defaults.
static unsigned int __fastcall SaveGame_DestroyRecord(std::uint32_t* self, void* /*edx*/)
{
    // Embedded sub-object at +0x6C: its address is passed, not its contents.
    DestroyRecord_43E300(reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 0x6C), nullptr);

    std::uint32_t result = CSF_AT(std::uint32_t, self, 0x68);
    if (result >= 0x10) {
        // Large-buffer mode: the pointer at +0x54 is owned by this record.
        result = CSF_AT(std::uint32_t, self, 0x54);
        if (result != 0) {
            result = static_cast<std::uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x54))));
        }
    }

    CSF_AT(std::uint32_t, self, 0x68) = 0xF;
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint8_t, self, 0x54) = 0;

    if (CSF_AT(std::uint32_t, self, 0x18) >= 8) {
        result = CSF_AT(std::uint32_t, self, 0x04);
        if (result != 0) {
            result = static_cast<std::uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x04))));
        }
    }

    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 7;
    CSF_AT(std::uint16_t, self, 0x04) = 0;
    return result;
}

HOOK(0x00500A80, SaveGame_DestroyRecord, "auto");

