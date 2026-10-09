#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Calls the virtual update slot (vtable[0x24 / 4] = slot 9) on every
// 0x70-byte element of the instance array. The array base is re-read each
// iteration and the count is re-read after each call, since a callee may
// change either one.
static uint32_t __fastcall SoundManager_UpdateAllInstances(uint32_t* self, void* /*edx*/)
{
    uint32_t instanceCount = CSF_AT(uint32_t, self, 0xC);
    uint32_t instanceIndex = 0;

    if (instanceCount != 0)
    {
        uint32_t elementOffset = 0;
        do
        {
            uint8_t* instance = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0)) + elementOffset;
            void** instanceVtable = CSF_AT(void**, instance, 0);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(instanceVtable[9])(instance, nullptr);

            instanceCount = CSF_AT(uint32_t, self, 0xC);
            ++instanceIndex;
            elementOffset += 0x70;
        } while (instanceIndex < instanceCount);
    }

    return instanceCount;
}

HOOK(0x00526F70, SoundManager_UpdateAllInstances, "auto");
