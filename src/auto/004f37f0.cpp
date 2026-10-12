#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, HudStringShow_DestroyEntry, 0x004F2240, (void* self, void* edx));
CSF_FUNC(int, __fastcall, HudStringShow_InitializeEntry, 0x004F2570, (void* a1, int a2));
CSF_FUNC(int, __fastcall, HudStringShow_ClearEntries, 0x004F2B70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, HudStringShow_ProcessEntry, 0x004F2C90, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, HudStringShow_ReserveEntries, 0x004F3070, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, HudStringShow_AppendEntry, 0x004F3760, (void* self, void* edx, int a3));

static int __stdcall HudStringShow_ProcessEntries(S_004f36f0* entries, int contextAddress)
{
    auto* context = reinterpret_cast<uint8_t*>(contextAddress);
    const uint8_t reuseEntries = CSF_AT(uint8_t, context, 0x14);
    const uint8_t entryMode = CSF_AT(uint8_t, context, 0x15);
    unsigned int count = 0;

    if (reuseEntries)
    {
        uint32_t limit = entries->field_0004;
        if (limit)
            limit = static_cast<int32_t>(entries->field_0008 - limit) / 124;
        count = limit;

        bool failed = false;
        if (CSF_AT(uint8_t, context, 0) == 0)
        {
            void* handler = CSF_AT(void*, context, 0x4);
            failed = !((bool(__fastcall*)(void*, void*, unsigned int*, int))
                CSF_AT(void**, handler, 0)[0x78 / 4])(
                    handler, nullptr, &count, contextAddress);
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
        if (limit)
        {
            do
            {
                void* entry = reinterpret_cast<void*>(entries->field_0004 + offset);
                const bool inactive = CSF_AT(uint8_t, context, 0) != 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (!inactive)
                {
                    CSF_AT(uint8_t, context, 0x15) = entryMode;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    HudStringShow_ProcessEntry(entry, nullptr, contextAddress);
                    limit = count;
                }
                ++index;
                offset += 124;
            } while (index < limit);
        }
    }
    else
    {
        bool failed = false;
        if (CSF_AT(uint8_t, context, 0) == 0)
        {
            void* handler = CSF_AT(void*, context, 0x4);
            failed = !((bool(__fastcall*)(void*, void*, unsigned int*, int))
                CSF_AT(void**, handler, 0)[0x78 / 4])(
                    handler, nullptr, &count, contextAddress);
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
            if (count)
            {
                do
                {
                    void* entry = reinterpret_cast<void*>(entries->field_0004 + offset);
                    const bool inactive = CSF_AT(uint8_t, context, 0) != 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (!inactive)
                    {
                        CSF_AT(uint8_t, context, 0x15) = entryMode;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        HudStringShow_ProcessEntry(entry, nullptr, contextAddress);
                    }
                    ++index;
                    offset += 124;
                } while (index < count);
            }
        }
        else
        {
            HudStringShow_ClearEntries(entries, nullptr);
            HudStringShow_ReserveEntries(entries, nullptr, count);

            alignas(8) uint8_t entryBytes[0x7C];
            uint32_t index = 0;
            uint32_t offset = 0;
            if (count)
            {
                do
                {
                    HudStringShow_InitializeEntry(entryBytes, 0);
                    HudStringShow_AppendEntry(
                        entries, nullptr, reinterpret_cast<int>(entryBytes));

                    void* entry = reinterpret_cast<void*>(entries->field_0004 + offset);
                    const bool inactive = CSF_AT(uint8_t, context, 0) != 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (!inactive)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        HudStringShow_ProcessEntry(entry, nullptr, contextAddress);
                    }
                    HudStringShow_DestroyEntry(entryBytes, nullptr);
                    ++index;
                    offset += 124;
                } while (index < count);
            }
        }
    }

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        if (((bool(__fastcall*)(void*, void*, int))
            CSF_AT(void**, handler, 0)[0x7C / 4])(
                handler, nullptr, contextAddress))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004F37F0, HudStringShow_ProcessEntries, "auto");
