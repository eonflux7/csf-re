#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));

static char __fastcall CommandoAnims_UpdateLinkedEntries(void* self, void* /*edx*/)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x1C);
    char result = 1;

    for (uint32_t index = 0; index < entryCount; ++index)
    {
        if (!result)
            break;

        void* entry = CSF_AT(void**, self, 0x14)[index];
        const int32_t linkId = CSF_AT(int32_t, entry, 0x10);
        if (linkId != -1)
        {
            const uint32_t linkCount = CSF_AT(uint32_t, self, 0x2C);
            int32_t linkedIndex = -1;
            if (linkCount)
            {
                void** links = CSF_AT(void**, self, 0x24);
                for (uint32_t candidate = 0; candidate < linkCount; ++candidate)
                {
                    if (CSF_AT(int32_t, links[candidate], 0x0C) == linkId)
                    {
                        linkedIndex = static_cast<int32_t>(candidate);
                        break;
                    }
                }
            }

            void* resolved = reinterpret_cast<void*>(
                ResolveEntryLookupValue(entry, nullptr));
            void** linkedSlot = CSF_AT(void**, self, 0x24) + linkedIndex;
            void* source =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, resolved, 0)[0x2C / 4])(resolved, nullptr);

            void* destination = *linkedSlot;
            CSF_AT(uint32_t, destination, 0x10) = CSF_AT(uint32_t, source, 0);
            CSF_AT(uint32_t, destination, 0x14) = CSF_AT(uint32_t, source, 4);
            CSF_AT(uint32_t, destination, 0x18) = CSF_AT(uint32_t, source, 8);
            CSF_AT(uint32_t, destination, 0x1C) = CSF_AT(uint32_t, source, 0x0C);
        }

        void* resolved = reinterpret_cast<void*>(
            ResolveEntryLookupValue(CSF_AT(void**, self, 0x14)[index], nullptr));
        result = static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, resolved, 0)[0x68 / 4])(resolved, nullptr));
    }

    void* finalEntry = CSF_AT(void*, self, 0x34);
    if (finalEntry)
    {
        void* resolved = reinterpret_cast<void*>(
            ResolveEntryLookupValue(finalEntry, nullptr));
        return static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, resolved, 0)[0x68 / 4])(resolved, nullptr));
    }
    return result;
}

HOOK(0x004FC7E0, CommandoAnims_UpdateLinkedEntries, "auto");
