#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static void __fastcall MenuAccounts_ResetConfiguration(
    void* self, void* /*edx*/)
{
    void* ownedObject = CSF_AT(void*, self, 0x14);

    CSF_AT(std::int32_t, self, 0x00) = -1;
    CSF_AT(std::int32_t, self, 0x04) = 3;
    CSF_AT(std::int32_t, self, 0x08) = 0;
    CSF_AT(std::int32_t, self, 0x0C) = 2;
    CSF_AT(std::int32_t, self, 0x10) = -1;

    if (ownedObject != nullptr)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0x00)[0])(
                ownedObject, nullptr, 1);
        CSF_AT(void*, self, 0x14) = nullptr;
    }
}

HOOK(0x00565FA0, MenuAccounts_ResetConfiguration, "auto");
