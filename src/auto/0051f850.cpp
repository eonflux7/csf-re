#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall ParticleManager_DispatchMatchingRecords(void* self, void* /*edx*/, int recordId)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x34);
    int result = static_cast<int>(count);

    if (count != 0)
    {
        std::uint32_t offset = 0;
        std::uint32_t remaining = count;
        do
        {
            auto* record = CSF_AT(std::uint8_t*, self, 0x2C) + offset;
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(record));
            if (CSF_AT(int, record, 0x4) == recordId)
            {
                void* object = CSF_AT(void*, record, 0x8);
                result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr, 0);
            }
            offset += 20;
            --remaining;
        } while (remaining != 0);
    }

    return result;
}

HOOK(0x0051F850, ParticleManager_DispatchMatchingRecords, "auto");
