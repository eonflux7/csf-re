#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00840cf8_ProcessEntry, 0x00519770, (void* self, void* edx));

static unsigned int __fastcall C_00840cf8_CallHandlersForKey(C_00840cf8* self, void* /*edx*/, int key)
{
    unsigned int result = self->field_001c;
    unsigned int index = 0;
    if (result != 0)
    {
        uint32_t offset = 0;
        do
        {
            uint8_t* table = reinterpret_cast<uint8_t*>(self->field_0014);
            uint8_t* entry = table + offset;
            int entryKey = CSF_AT(int, entry, 4);
            if (entryKey == key)
            {
                void* entryPayload = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entry, 8)));
                C_00840cf8_ProcessEntry(entryPayload, nullptr);
            }
            result = self->field_001c;
            ++index;
            offset += 16;
        } while (index < result);
    }
    return result;
}

HOOK(0x00519510, C_00840cf8_CallHandlersForKey, "auto");
