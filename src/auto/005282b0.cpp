#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, InitSoundSlot, 0x00527DC0, (void* self, void* edx));

static C_00841090** __fastcall S_005282b0_InitPool(S_005282b0* self, void* /*edx*/, unsigned int requestedCount, C_00841090* owner)
{
    self->field_000c = requestedCount;
    self->field_0014 = (uint32_t)(uintptr_t)owner;
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0010 = 0;
    self->field_0008 = 0;

    if (requestedCount != 0)
    {
        if (requestedCount < 0x40u)
            self->field_000c = 64;

        uint32_t count = self->field_000c;
        uint32_t* block = static_cast<uint32_t*>(Mem_Alloc((std::size_t)(112u * count + 4u)));

        uint32_t dataStart = 0;
        if (block)
        {
            block[0] = count;
            dataStart = (uint32_t)(uintptr_t)(block + 1);

            uint8_t* cursor = reinterpret_cast<uint8_t*>(block + 1);
            for (uint32_t i = 0; i < count; ++i)
            {
                InitSoundSlot(cursor, nullptr);
                cursor += 112;
            }
        }

        self->field_0000 = dataStart;
        self->field_0004 = 112u * self->field_000c + dataStart - 112u;
    }

    return reinterpret_cast<C_00841090**>(self);
}

HOOK(0x005282B0, S_005282b0_InitPool, "auto");
