#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x706E80: __thiscall, one stack arg, ret 4.
CSF_FUNC(int, __fastcall, ReinitFromRecord, 0x00706E80, (void* self, void* edx, uint32_t* a2));

// Clears every slot of the 16-byte-stride entry table at +0x28 (count at +0x30),
// releasing each non-null entry via its vtable slot 0 with argument 1, then
// resets count and +0x44 and forwards the record at +0x50 to ReinitFromRecord.
static int __fastcall C_008632c0_ReleaseEntriesAndReinit(C_008632c0* self, void* /*edx*/)
{
    uint32_t entryIndex = 0;
    int byteOffset = 0;

    if (CSF_AT(uint32_t, self, 0x30) != 0)
    {
        do
        {
            uint8_t* table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x28)));
            void* entryObj = *reinterpret_cast<void**>(table + byteOffset);
            if (entryObj != nullptr)
            {
                // (**v4)(v4, 1): vtable slot 0, one stack argument.
                void* slot0 = CSF_AT(void**, entryObj, 0)[0];
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(slot0)(entryObj, nullptr, 1);

                // The table pointer is reloaded after the call.
                uint8_t* tableAfter = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x28)));
                *reinterpret_cast<uint32_t*>(tableAfter + byteOffset) = 0;
            }
            ++entryIndex;
            byteOffset += 16;
        } while (entryIndex < CSF_AT(uint32_t, self, 0x30));
    }

    uint32_t* record = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0050));
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    return ReinitFromRecord(self, nullptr, record);
}

HOOK(0x00707140, C_008632c0_ReleaseEntriesAndReinit, "auto");
