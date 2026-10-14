#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall InputDirectInput_ApplyMenuStateFlag(
    void* self, void* /*edx*/, char applyFlag)
{
    uint32_t count = CSF_AT(uint32_t, self, 0x10);
    uint32_t index = 0;
    const uint32_t savedState = CSF_AT(uint32_t, self, 0x18);

    if (count != 0)
    {
        do
        {
            const uint32_t currentState = CSF_AT(uint32_t, self, 0x18);
            const uint32_t entry = CSF_AT(uint32_t*, self, 0x08)[index];

            if (savedState != currentState)
                break;

            if (savedState == 0)
            {
                ((void(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, self, 0)[0x74 / 4])(
                        self, nullptr, entry);
            }
            else if (savedState == entry)
            {
                ((void(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, self, 0)[0x74 / 4])(
                        self, nullptr, entry);
                break;
            }

            count = CSF_AT(uint32_t, self, 0x10);
            ++index;
        } while (index < count);
    }

    if (applyFlag != 0)
    {
        return static_cast<char>(
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0x78 / 4])(self, nullptr));
    }

    return applyFlag;
}

HOOK(0x712DF0, InputDirectInput_ApplyMenuStateFlag, "auto");
