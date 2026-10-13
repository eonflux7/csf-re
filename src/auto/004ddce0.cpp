#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
static CSF_FUNC(int, __fastcall, S_004dca10_UpdateEntryState, 0x004DDC10, (void* self, void* edx, int a2));

static int __fastcall C_0083e0d8_UpdateEntryState(
    C_0083e0d8* self, void* /*edx*/, int entryAddress)
{
    void* entry = reinterpret_cast<void*>(entryAddress);
    bool active = false;
    if (CSF_AT(uint8_t, entry, 0x00) == 0)
    {
        void* owner = CSF_AT(void*, entry, 0x04);
        active = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, owner, 0)[0x80 / 4])(
                owner, nullptr, 0, entryAddress);
    }

    const int savedState = CSF_AT(int, entry, 0x1C);
    CSF_AT(uint8_t, entry, 0x00) = active;
    AudioSoundManager_ProcessArchiveContext(entry);

    bool processMember = CSF_AT(uint8_t, entry, 0x00) == 0;
    CSF_AT(int, entry, 0x1C) = 0;
    if (processMember)
    {
        CSF_AT(uint8_t, entry, 0x15) = 0;
        CSF_AT(uint8_t, entry, 0x16) = 0;
        S_004dca10_UpdateEntryState(&self->field_0034, nullptr, entryAddress);
    }

    processMember = CSF_AT(uint8_t, entry, 0x00) == 0;
    CSF_AT(int, entry, 0x1C) = 0;
    if (processMember)
    {
        CSF_AT(uint8_t, entry, 0x15) = 0;
        CSF_AT(uint8_t, entry, 0x16) = 0;
        S_004dca10_UpdateEntryState(&self->field_0044, nullptr, entryAddress);
    }

    processMember = CSF_AT(uint8_t, entry, 0x00) == 0;
    CSF_AT(int, entry, 0x1C) = 0;
    if (processMember)
    {
        CSF_AT(uint8_t, entry, 0x15) = 0;
        CSF_AT(uint8_t, entry, 0x16) = 0;
        S_004dca10_UpdateEntryState(&self->field_0054, nullptr, entryAddress);
    }

    const bool finalize = CSF_AT(uint8_t, entry, 0x00) == 0;
    CSF_AT(int, entry, 0x1C) = savedState;
    if (finalize)
    {
        void* owner = CSF_AT(void*, entry, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, owner, 0)[0x7C / 4])(
                    owner, nullptr, entryAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, entry, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, entry, 0x00) = 1;
    return 1;
}

HOOK(0x004DDCE0, C_0083e0d8_UpdateEntryState, "auto");
