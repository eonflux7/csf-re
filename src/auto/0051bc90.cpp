#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MissionScript_GetEntryCount_46BEA0, 0x0046BEA0, (S_0046bea0* self, void*));
CSF_FUNC(uint8_t*, __fastcall, BddTables_ResizeSoundStates, 0x0051B330, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(int, __fastcall, C_00840e38_GetSoundCount, 0x0051BA20, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetOptionsField100, 0x00697380, ());
CSF_FUNC(char, __fastcall, MissionScript_HasPendingState, 0x0069AF10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_GetSoundDescriptor, 0x0075A480, (void* self, void* edx, int a2));

static int __fastcall C_00840e38_ProcessSoundEntry(
    C_00840e38* self, void* /*edx*/, int soundId)
{
    (void)CSF_VTBL(0x0046BEA0);

    if (soundId == -1 || soundId == -2)
        return 0;

    const int index = self->vSlotC(soundId);
    if (index == -1)
        return 0;

    const uint32_t statesBegin = self->field_000c;
    const uint32_t stateCount = statesBegin
        ? CSF_AT(uint32_t, self, 0x10) - statesBegin
        : 0;

    void* table = reinterpret_cast<void*>(self->field_0004);
    const uint32_t entriesBegin = CSF_AT(uint32_t, table, 0x04);
    uint32_t entryCount = 0;
    if (entriesBegin)
    {
        const int32_t byteCount = static_cast<int32_t>(
            CSF_AT(uint32_t, table, 0x08) - entriesBegin);
        const int32_t scaledCount = static_cast<int32_t>(
            (int64_t{0x78787879} * byteCount) >> 32) >> 5;
        entryCount = static_cast<uint32_t>(scaledCount)
            + (static_cast<uint32_t>(scaledCount) >> 31);
    }

    if (stateCount != entryCount)
    {
        const int count = C_00840e38_GetSoundCount(self, nullptr);
        BddTables_ResizeSoundStates(
            reinterpret_cast<uint8_t*>(self) + 0x08,
            nullptr, static_cast<unsigned int>(count), 0);
    }

    uint8_t* state = reinterpret_cast<uint8_t*>(self->field_000c) + index;
    if (*state)
        return *state == 1;

    *state = 1;
    void* descriptor = reinterpret_cast<void*>(
        AudioSoundManager_GetSoundDescriptor(self, nullptr, index));
    auto* entries = reinterpret_cast<S_0046bea0*>(
        reinterpret_cast<uint8_t*>(descriptor) + 0x04);

    if (CSF_AT(uint8_t, descriptor, 0x22))
    {
        uint32_t remaining = static_cast<uint32_t>(
            MissionScript_GetEntryCount_46BEA0(entries, nullptr));
        uint32_t offset = 0;
        while (remaining)
        {
            uint8_t* entry = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, descriptor, 0x08)) + offset;
            const char* name = CSF_AT(uint32_t, entry, 0x18) < 0x10
                ? reinterpret_cast<const char*>(entry + 0x04)
                : reinterpret_cast<const char*>(
                      CSF_AT(uint32_t, entry, 0x04));

            void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
            (reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, manager, 0)[0xA0 / 4]))(
                    manager, nullptr, name);

            void* options = reinterpret_cast<void*>(GetOptionsField100());
            if (options && MissionScript_HasPendingState(options, nullptr))
            {
                (reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, options, 0)[0x04 / 4]))(options, nullptr);
                (reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, options, 0)[0x04 / 4]))(options, nullptr);
            }

            offset += 28;
            --remaining;
        }
        return 1;
    }

    uint32_t remaining = static_cast<uint32_t>(
        MissionScript_GetEntryCount_46BEA0(entries, nullptr));
    uint32_t offset = 0;
    int success = 1;
    while (remaining)
    {
        const int specialMode = CSF_AT(uint8_t, descriptor, 0x25) == 1;
        void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());

        uint8_t* entry = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, descriptor, 0x08)) + offset;
        const char* name = CSF_AT(uint32_t, entry, 0x18) < 0x10
            ? reinterpret_cast<const char*>(entry + 0x04)
            : reinterpret_cast<const char*>(CSF_AT(uint32_t, entry, 0x04));

        auto process = reinterpret_cast<bool(__fastcall*)(
            void*, void*, const char*, unsigned int, int)>(
                CSF_AT(void**, manager, 0)[0x30 / 4]);
        const unsigned int mode = CSF_AT(uint8_t, descriptor, 0x21);
        if (!process(manager, nullptr, name, mode, specialMode))
        {
            reinterpret_cast<uint8_t*>(self->field_000c)[index] = 0xFF;
            success = 0;
        }

        offset += 28;
        --remaining;
    }
    return success;
}

HOOK(0x0051BC90, C_00840e38_ProcessSoundEntry, "auto");
