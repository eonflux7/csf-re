#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, SpawnPoints_ProcessEntries, 0x004079D0,
         (unsigned int a1, int a2));
CSF_FUNC(int, __stdcall, SpawnPoints_ProcessPointRecords, 0x00407BA0,
         (int a1, int a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10,
         (void* a3, bool* a2));

static int __fastcall SpawnPoints_ProcessCollection(
    void* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    auto* collection = static_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, processor, 0x00)[0x80 / 4])(
                processor, nullptr, 0, contextAddress);
    }

    const uint32_t savedReference = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0x00) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Archive_SerializeObjectRef(collection + 4,
                                   reinterpret_cast<bool*>(context));
    }

    bool canProcess = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canProcess)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SpawnPoints_ProcessPointRecords(
            reinterpret_cast<int>(collection + 8), contextAddress);
    }

    canProcess = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canProcess)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SpawnPoints_ProcessEntries(
            reinterpret_cast<unsigned int>(collection + 24), contextAddress);
    }

    canProcess = CSF_AT(uint8_t, context, 0x00) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedReference;
    if (canProcess)
    {
        void* processor = CSF_AT(void*, context, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, processor, 0x00)[0x7C / 4])(
                    processor, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00407DC0, SpawnPoints_ProcessCollection, "auto");
