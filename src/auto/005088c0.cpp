#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, S_00508240_GrowEntries, 0x00508240, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// The packet also lists the growth routine's address as a vtable reference.
static const auto g_ListedEntryArrayVtable = CSF_VTBL(0x00508240);

static int __fastcall S_005080e0_AppendDefaultEntries(
    S_005080e0* self, void* /*edx*/)
{
    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* object = LookupObjectById(globalObject, nullptr);
    void* countObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    const std::uint32_t savedProvider =
        CSF_AT(std::uint32_t, self, 0x1C);

    alignas(8) std::uint8_t stringBytes[0x1C];
    CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;
    CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, stringBytes, 0x04) = 0;
    TextInputSubobject_Init(stringBytes, nullptr, &g_byte_82E65C);

    void* provider = CSF_AT(void*, self, 0x1C);
    const int divisor =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, provider, 0)[0x18 / 4])(provider, nullptr, 1);
    int result = CSF_AT(int, countObject, 0x04) / divisor;

    if (result != 0)
    {
        auto* entries = reinterpret_cast<S_00508240*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x54);
        std::uint32_t remaining = static_cast<std::uint32_t>(result);

        do
        {
            if (static_cast<std::uint32_t>(entries->field_0008) >=
                static_cast<std::uint32_t>(entries->field_0004))
            {
                S_00508240_GrowEntries(
                    entries, nullptr, static_cast<int>(entries->field_000c));
            }

            const std::uint32_t index = entries->field_0008;
            const std::uint32_t entryAddress =
                entries->field_0000 + 56 * index;
            entries->field_0008 = index + 1;

            auto* entry = reinterpret_cast<std::uint8_t*>(entryAddress);
            CSF_AT(int, entry, 0x00) = 1;
            CSF_AT(std::uint32_t, entry, 0x04) = savedProvider;
            CSF_AT(int, entry, 0x08) = 0;
            CSF_AT(int, entry, 0x0C) = 0;
            CSF_AT(int, entry, 0x10) = 0;
            CSF_AT(float, entry, 0x14) = 1.0f;
            CSF_AT(int, entry, 0x18) = 0; // Original uninitialized local.

            result = static_cast<int>(reinterpret_cast<std::intptr_t>(
                AssignStringRange_488830(
                    entry + 0x1C, nullptr, stringBytes, 0, 0xFFFFFFFF)));
            --remaining;
        } while (remaining != 0);
    }

    if (CSF_AT(std::uint32_t, stringBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, stringBytes, 0x04);
        result = static_cast<int>(reinterpret_cast<std::intptr_t>(block));
        if (block)
            return Mem_FreeDebug(block);
    }
    return result;
}

HOOK(0x005088C0, S_005080e0_AppendDefaultEntries, "auto");
