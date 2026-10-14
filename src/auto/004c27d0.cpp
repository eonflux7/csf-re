#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, CoverAttack_ProcessRecord, 0x004C2380, (void* self, void* edx, int a2));

static const auto kProcessRecordVtable = CSF_VTBL(0x004C2380);

static int __fastcall S_004c2380_SerializeCoverAttackRecord(
    S_004c2380* self, void* /*edx*/, int recordAddress)
{
    void* record = reinterpret_cast<void*>(recordAddress);
    const uint32_t savedContext = CSF_AT(uint32_t, record, 0x1C);

    bool ready = false;
    if (CSF_AT(uint8_t, record, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, record, 0x04);
        ready = !((bool(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, recordAddress);
    }
    CSF_AT(uint8_t, record, 0x00) = ready;
    CoverAttack_ProcessRecord(self, nullptr, recordAddress);

    bool failed = CSF_AT(uint8_t, record, 0x00) != 0;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x14),
            reinterpret_cast<S_00691860*>(record));
    }

    failed = CSF_AT(uint8_t, record, 0x00) != 0;
    CSF_AT(uint32_t, record, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, record, 0x04);
        CSF_AT(uint8_t, record, 0x15) = 0;
        CSF_AT(uint8_t, record, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, backend, 0)[0x38 / 4])(
                backend, nullptr,
                reinterpret_cast<uint8_t*>(self) + 0x20,
                recordAddress);
        CSF_AT(uint8_t, record, 0x00) = result == -1;
    }

    CSF_AT(uint32_t, record, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, record, 0x00) != 0)
    {
        CSF_AT(uint8_t, record, 0x00) = 1;
        return 1;
    }

    void* backend = CSF_AT(void*, record, 0x04);
    // Original: call [edx+7Ch]; test al, al; je ...; ... ret 4 with the full eax.
    const int result = ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, backend, 0)[0x7C / 4])(
            backend, nullptr, recordAddress);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, record, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, record, 0x00) = 0;
    return result;
}

HOOK(0x004C27D0, S_004c2380_SerializeCoverAttackRecord, "auto");
