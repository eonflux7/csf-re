#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0,
         (void* self, void* edx));

static int __fastcall HandleObjectNotification(
    void* self, void* /*edx*/, int notification, int payload)
{
    std::uint32_t result = static_cast<std::uint32_t>(notification) - 1u;
    if (result == 0)
    {
        CSF_AT(std::uint8_t, self, 0x24) = 1;
    }
    else
    {
        result -= 0x31u;
        if (result == 0)
        {
            result = static_cast<std::uint32_t>(payload);
            void* payloadObject = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(result));

            if (CSF_AT(std::uint8_t, payloadObject, 0x38) != 0)
            {
                void* relatedObject = GetReferencedObject(self, nullptr);
                return ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, relatedObject, 0)[7])(
                        relatedObject, nullptr);
            }
        }
    }

    return static_cast<int>(result);
}

HOOK(0x005656A0, HandleObjectNotification, "auto");
