#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseEmbeddedMember, 0x0044AFF0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MissionScript_ReserveStringEntries, 0x0046FB30, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, MissionScript_AppendStringEntry, 0x004709D0, (void* self, void* edx, int a3));
CSF_FUNC(char*, __fastcall, Object_PrepareForDestruction, 0x004734D0, (char** self, void* edx));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

static int __stdcall MissionScript_SerializeStringEntries(S_00470340* entries, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    const uint8_t writing = CSF_AT(uint8_t, archive, 0x14);
    const uint8_t updateExisting = CSF_AT(uint8_t, archive, 0x15);
    unsigned int count = 0;

    if (writing)
    {
        const uint32_t begin = entries->field_0004;
        if (begin)
            count = static_cast<int32_t>(entries->field_0008 - begin) / 12;
    }

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, unsigned int*, void*)>(
            CSF_AT(void**, stream, 0)[0x78 / 4])(stream, nullptr, &count, archive);
    }
    CSF_AT(uint8_t, archive, 0) = failed;

    if (!failed)
    {
        if (writing)
        {
            unsigned int limit = count;
            uint32_t offset = 0;
            for (unsigned int index = 0; index < limit; ++index, offset += 12)
            {
                const int entryAddress = static_cast<int>(entries->field_0004 + offset);
                const bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
                CSF_AT(uint32_t, archive, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(uint8_t, archive, 0x15) = updateExisting;
                    CSF_AT(uint8_t, archive, 0x16) = 0;
                    Archive_SerializeString(entryAddress, archive);
                    limit = count;
                }
            }
        }
        else if (updateExisting)
        {
            uint32_t offset = 0;
            for (unsigned int index = 0; index < count; ++index, offset += 12)
            {
                const int entryAddress = static_cast<int>(entries->field_0004 + offset);
                const bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
                CSF_AT(uint32_t, archive, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(uint8_t, archive, 0x15) = updateExisting;
                    CSF_AT(uint8_t, archive, 0x16) = 0;
                    Archive_SerializeString(entryAddress, archive);
                }
            }
        }
        else
        {
            ReleaseEmbeddedMember(entries, nullptr);
            MissionScript_ReserveStringEntries(entries, nullptr, count);

            uint32_t offset = 0;
            for (unsigned int index = 0; index < count; ++index, offset += 12)
            {
                C_008313f8 emptyString;
                emptyString.vtbl = CSF_VTBL(0x008313F8);
                emptyString.field_0004 = 0;
                emptyString.field_0008 = 0xFFFFFFFF;
                MissionScript_AppendStringEntry(
                    entries, nullptr, reinterpret_cast<int>(&emptyString));

                const int entryAddress = static_cast<int>(entries->field_0004 + offset);
                const bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
                CSF_AT(uint32_t, archive, 0x1C) = 0;
                if (canSerialize)
                {
                    CSF_AT(uint8_t, archive, 0x15) = 0;
                    CSF_AT(uint8_t, archive, 0x16) = 0;
                    Archive_SerializeString(entryAddress, archive);
                }
                Object_PrepareForDestruction(
                    reinterpret_cast<char**>(&emptyString), nullptr);
            }
        }

        if (CSF_AT(uint8_t, archive, 0) == 0)
        {
            void* stream = CSF_AT(void*, archive, 0x4);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive))
            {
                CSF_AT(uint8_t, archive, 0) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x00470B20, MissionScript_SerializeStringEntries, "auto");
