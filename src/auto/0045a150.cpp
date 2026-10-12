#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudStatusIcons_ProcessEntry, 0x00458C60, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, HudStatusIcons_ClearEntries, 0x00458E40, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, HudStatusIcons_ReserveEntries, 0x00459340, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __fastcall, HudStatusIcons_AppendEntry, 0x00459FA0, (void* self, void* edx, char* a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __stdcall HudStatusIcons_ProcessEntries(S_00459e50* entries, int a2)
{
    void* context = reinterpret_cast<void*>(a2);
    const uint8_t existingEntries = CSF_AT(uint8_t, context, 0x14);
    const uint8_t entryMode = CSF_AT(uint8_t, context, 0x15);

    alignas(8) uint8_t countBytes[0x4];
    uint32_t& count = *reinterpret_cast<uint32_t*>(countBytes);
    count = 0;

    if (existingEntries)
    {
        uint32_t limit = entries->field_0004;
        if (limit)
            limit = static_cast<int32_t>(entries->field_0008 - limit) / 40;
        count = limit;

        bool failed = false;
        if (CSF_AT(uint8_t, context, 0) == 0)
        {
            void* stream = CSF_AT(void*, context, 0x4);
            const bool success =
                reinterpret_cast<bool (__fastcall*)(void*, void*, uint32_t*, void*)>(
                    CSF_AT(void**, stream, 0)[0x78 / 4])(
                        stream, nullptr, &count, context);
            limit = count;
            failed = !success;
        }
        CSF_AT(uint8_t, context, 0) = failed;
        if (failed)
        {
            CSF_AT(uint8_t, context, 0) = 1;
            return 1;
        }

        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < limit)
        {
            void* entry = reinterpret_cast<void*>(entries->field_0004 + offset);
            const bool canProcess = CSF_AT(uint8_t, context, 0) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canProcess)
            {
                CSF_AT(uint8_t, context, 0x15) = entryMode;
                CSF_AT(uint8_t, context, 0x16) = 0;
                HudStatusIcons_ProcessEntry(entry, nullptr, context);
                limit = count;
            }
            ++index;
            offset += 40;
        }
    }
    else
    {
        bool failed = false;
        if (CSF_AT(uint8_t, context, 0) == 0)
        {
            void* stream = CSF_AT(void*, context, 0x4);
            failed =
                !reinterpret_cast<bool (__fastcall*)(void*, void*, uint32_t*, void*)>(
                    CSF_AT(void**, stream, 0)[0x78 / 4])(
                        stream, nullptr, &count, context);
        }
        CSF_AT(uint8_t, context, 0) = failed;
        if (failed)
        {
            CSF_AT(uint8_t, context, 0) = 1;
            return 1;
        }

        if (entryMode)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            while (index < count)
            {
                void* entry = reinterpret_cast<void*>(entries->field_0004 + offset);
                const bool canProcess = CSF_AT(uint8_t, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = entryMode;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    HudStatusIcons_ProcessEntry(entry, nullptr, context);
                }
                ++index;
                offset += 40;
            }
        }
        else
        {
            HudStatusIcons_ClearEntries(entries, nullptr);
            HudStatusIcons_ReserveEntries(entries, nullptr, count);

            alignas(8) uint8_t entryBytes[0x28];
            uint32_t index = 0;
            uint32_t offset = 0;
            while (index < count)
            {
                CSF_AT(int32_t, entryBytes, 0x00) = -1;
                CSF_AT(float, entryBytes, 0x10) = -1.0f;
                CSF_AT(float, entryBytes, 0x08) = -1.0f;
                CSF_AT(float, entryBytes, 0x14) = 0.0f;
                CSF_AT(float, entryBytes, 0x0C) = 0.0f;
                CSF_AT(float, entryBytes, 0x04) = 255.0f;
                CSF_AT(uint32_t, entryBytes, 0x18) =
                    CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1D98);
                CSF_AT(void**, entryBytes, 0x1C) = CSF_VTBL(0x0082D448);
                CSF_AT(int32_t, entryBytes, 0x20) = -1;
                CSF_AT(int32_t, entryBytes, 0x24) = -1;
                HudStatusIcons_AppendEntry(
                    entries, nullptr, reinterpret_cast<char*>(entryBytes));

                void* entry = reinterpret_cast<void*>(entries->field_0004 + offset);
                const bool canProcess = CSF_AT(uint8_t, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    HudStatusIcons_ProcessEntry(entry, nullptr, context);
                }

                ++index;
                offset += 40;
                const bool moreEntries = index < count;
                CSF_AT(void**, entryBytes, 0x1C) = CSF_VTBL(0x0082D410);
                if (!moreEntries)
                    break;
            }
        }
    }

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* stream = CSF_AT(void*, context, 0x4);
        if (reinterpret_cast<bool (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, context))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x0045A150, HudStatusIcons_ProcessEntries, "auto");
