#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudStatusIcons_ProcessEntry, 0x00458AF0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, HudStatusIcons_ReserveEntries, 0x00459460, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __fastcall, HudStatusIcons_AppendEntry, 0x0045A030, (void* self, void* edx, void* a3));
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __stdcall HudStatusIcons_ProcessEntries(S_0047ea40* entries, int a2)
{
    auto* context = reinterpret_cast<uint8_t*>(a2);
    const uint8_t updateExisting = CSF_AT(uint8_t, context, 0x14);
    const uint8_t savedMode = CSF_AT(uint8_t, context, 0x15);
    uint32_t count = 0;

    if (updateExisting)
    {
        uint32_t limit = entries->field_0004;
        if (limit)
            limit = static_cast<int32_t>(entries->field_0008 - limit) / 40;
        count = limit;

        bool failed = false;
        if (CSF_AT(uint8_t, context, 0) == 0)
        {
            void* processor = CSF_AT(void*, context, 0x4);
            failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, processor, 0)[120 / 4])(
                    processor, nullptr, &count, a2);
            limit = count;
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
            const bool ready = CSF_AT(uint8_t, context, 0) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (ready)
            {
                CSF_AT(uint8_t, context, 0x15) = savedMode;
                CSF_AT(uint8_t, context, 0x16) = 0;
                HudStatusIcons_ProcessEntry(entry, nullptr, a2);
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
            void* processor = CSF_AT(void*, context, 0x4);
            failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, processor, 0)[120 / 4])(
                    processor, nullptr, &count, a2);
        }
        CSF_AT(uint8_t, context, 0) = failed;
        if (failed)
        {
            CSF_AT(uint8_t, context, 0) = 1;
            return 1;
        }

        if (savedMode)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            while (index < count)
            {
                void* entry = reinterpret_cast<void*>(entries->field_0004 + offset);
                const bool ready = CSF_AT(uint8_t, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (ready)
                {
                    CSF_AT(uint8_t, context, 0x15) = savedMode;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    HudStatusIcons_ProcessEntry(entry, nullptr, a2);
                }
                ++index;
                offset += 40;
            }
        }
        else
        {
            S_0047ea40_DestroyPointerStorage(entries, nullptr);
            HudStatusIcons_ReserveEntries(entries, nullptr, count);
            if (count)
            {
                alignas(8) uint8_t entryBytes[0x28];
                CSF_AT(int32_t, entryBytes, 0x00) = -1;
                CSF_AT(float, entryBytes, 0x10) = -1.0f;
                CSF_AT(float, entryBytes, 0x08) = -1.0f;
                CSF_AT(uint32_t, entryBytes, 0x14) = 0;
                CSF_AT(uint32_t, entryBytes, 0x0C) = 0;
                CSF_AT(float, entryBytes, 0x04) = 255.0f;
                CSF_AT(uint32_t, entryBytes, 0x1C) = 0;
                CSF_AT(uint32_t, entryBytes, 0x20) = 0;
                CSF_AT(uint32_t, entryBytes, 0x24) = 0;

                uint32_t index = 0;
                uint32_t offset = 0;
                do
                {
                    void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                    CSF_AT(uint32_t, entryBytes, 0x18) =
                        CSF_AT(uint32_t, globalState, 0x1D98);
                    HudStatusIcons_AppendEntry(entries, nullptr, entryBytes);

                    void* entry = reinterpret_cast<void*>(entries->field_0004 + offset);
                    const bool ready = CSF_AT(uint8_t, context, 0) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (ready)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        HudStatusIcons_ProcessEntry(entry, nullptr, a2);
                    }
                    ++index;
                    offset += 40;
                } while (index < count);
            }
        }
    }

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* processor = CSF_AT(void*, context, 0x4);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, processor, 0)[124 / 4])(processor, nullptr, a2))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }
    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x0045A360, HudStatusIcons_ProcessEntries, "auto");
