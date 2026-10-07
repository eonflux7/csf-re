#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall ProcessObjectsAtOrBeyondThreshold(int* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x48);
    std::uint32_t count = 0;

    if (result != 0)
    {
        const std::uint32_t byteDifference =
            CSF_AT(std::uint32_t, self, 0x4C) -
            static_cast<std::uint32_t>(result);
        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDifference) >> 2);
    }

    for (std::uint32_t index = 0; index < count; ++index)
    {
        if (index >= CSF_AT(std::uint32_t, self, 0x6C))
        {
            void* entries = CSF_AT(void*, self, 0x48);
            void* object = CSF_AT(void*, entries, index * 4);

            result = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[8])(object, nullptr);

            if (result != 0)
            {
                void* innerObject = reinterpret_cast<void*>(
                    static_cast<std::uint32_t>(result));

                result = ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, innerObject, 0)[76])(innerObject, nullptr);
            }
        }
    }

    return result;
}

HOOK(0x0071AB00, ProcessObjectsAtOrBeyondThreshold, "auto");
