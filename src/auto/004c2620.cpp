#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_ProcessRecord, 0x004C2380, (void* self, void* edx, int a2));

static int __fastcall S_004c2380_ProcessAttackFallbacks(
    S_004c2380* self, void* /*edx*/, int recordAddress)
{
    // The packet also catalogs this direct callee address as a vtable reference.
    (void)CSF_VTBL(0x004C2380);

    void* record = reinterpret_cast<void*>(recordAddress);
    const int savedContext = CSF_AT(int, record, 0x1C);

    bool blocked = false;
    if (CSF_AT(uint8_t, record, 0) == 0)
    {
        void* handler = CSF_AT(void*, record, 0x04);
        blocked = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, handler, 0)[0x80 / 4])(
                handler, nullptr, 0, recordAddress);
    }
    CSF_AT(uint8_t, record, 0) = blocked;

    CoverAttack_ProcessRecord(self, nullptr, recordAddress);

    blocked = CSF_AT(uint8_t, record, 0) != 0;
    CSF_AT(int, record, 0x1C) = 0;
    if (!blocked)
    {
        void* handler = CSF_AT(void*, record, 0x04);
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, handler, 0)[0x48 / 4])(
                    handler, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x14,
                    recordAddress);
        CSF_AT(uint8_t, record, 0) = result == -1;
    }

    blocked = CSF_AT(uint8_t, record, 0) != 0;
    CSF_AT(int, record, 0x1C) = 0;
    if (!blocked)
    {
        void* handler = CSF_AT(void*, record, 0x04);
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, handler, 0)[0x48 / 4])(
                    handler, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x18,
                    recordAddress);
        CSF_AT(uint8_t, record, 0) = result == -1;
    }

    blocked = CSF_AT(uint8_t, record, 0) != 0;
    CSF_AT(int, record, 0x1C) = savedContext;
    if (!blocked)
    {
        void* handler = CSF_AT(void*, record, 0x04);
        // Original: call [edx+7Ch]; test al, al; je ...; ... ret 4 with the full eax.
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, handler, 0)[0x7C / 4])(
                    handler, nullptr, recordAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, record, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, record, 0) = 1;
    return 1;
}

HOOK(0x004C2620, S_004c2380_ProcessAttackFallbacks, "auto");
