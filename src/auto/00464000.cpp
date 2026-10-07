#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pGlobalObject_8AFC90, 0x008AFC90);

static int __fastcall GetIndexedRecordAddress(
    void* self, void* /*edx*/, std::uint32_t index)
{
    void* records = CSF_AT(void* volatile, self, 0x20);

    if ((index & 0x00800000u) != 0)
    {
        void* sharedObject = CSF_AT(void*, g_pGlobalObject_8AFC90, 0xBC);
        records = CSF_AT(void*, sharedObject, 0x38);
        index &= 0xFF7FFFFFu;
    }

    const std::uint32_t baseAddress = CSF_AT(std::uint32_t, records, 0x08);
    return static_cast<int>(baseAddress + 12u * index);
}

HOOK(0x00464000, GetIndexedRecordAddress, "auto");
