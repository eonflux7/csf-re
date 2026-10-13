#include <cstdint>
#include "globals.h"
#include "hook.h"

// Configures the object from an entry record: forwards two entry fields
// (+0x5C then +0x58) to the object's virtual slot 5 (offset 0x14).
static char __fastcall C_00861b58_ConfigureFromEntry(void* self, void* /*edx*/, void* entry)
{
    uint32_t secondField = CSF_AT(uint32_t, entry, 0x58);
    uint32_t firstField = CSF_AT(uint32_t, entry, 0x5C);

    ((void(__fastcall*)(void*, void*, uint32_t, uint32_t))CSF_AT(void**, self, 0)[0x14 / 4])(
        self, nullptr, firstField, secondField);
    return static_cast<char>(1);
}

HOOK(0x6E41B0, C_00861b58_ConfigureFromEntry, "auto");
