#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, sub_6A95D0, 0x006A95D0, (void* self, void* edx, int a2, uint32_t* a3, uint32_t* a4));
CSF_FUNC(void*, __fastcall, sub_6A9F10, 0x006A9F10, (void* self, void* edx, void* a2, int a3));

static void __fastcall NetGatheringDdl_CreateGatheringEntries(void* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x8) != 2)
        return;

    bool createdEntry = false;
    CSF_AT(void**, self, 0x328) =
        static_cast<void**>(Mem_Alloc(40));

    for (int index = 0; index < 10; ++index)
    {
        void* head = CSF_AT(void*, self, 0x44);
        void* node = CSF_AT(void*, head, 0);
        int position = 0;
        uint8_t* entry = nullptr;

        if (node != head)
        {
            do
            {
                if (position == index)
                    break;
                node = CSF_AT(void*, node, 0);
                ++position;
            } while (node != CSF_AT(void*, self, 0x44));

            if (node != head)
                entry = static_cast<uint8_t*>(node) + 8;
        }

        auto* gathering = static_cast<uint8_t*>(
            reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, self, 0)[0xC8 / 4])(self, nullptr, index));

        CSF_AT(void**, self, 0x328)[index] = nullptr;

        if (entry && gathering)
        {
            createdEntry = true;
            void* storage = Mem_Alloc(68);
            void* object = nullptr;

            if (storage)
            {
                object = sub_6A9F10(
                    storage, nullptr,
                    CSF_AT(void*, self, 0x38),
                    CSF_AT(int, self, 0x2C));
            }

            CSF_AT(void**, self, 0x328)[index] = object;

            sub_6A95D0(
                CSF_AT(void**, self, 0x328)[index], nullptr,
                static_cast<int>(reinterpret_cast<std::intptr_t>(entry)),
                reinterpret_cast<uint32_t*>(gathering + 0x2AC),
                reinterpret_cast<uint32_t*>(gathering + 0x2A4));

            CSF_AT(uint32_t, gathering, 0x2A8) =
                CSF_AT(uint16_t, CSF_AT(void*, entry, 0x4), 0x12);
        }
    }

    if (createdEntry)
    {
        CSF_AT(int, self, 0x8) = 3;
        CSF_AT(int, self, 0xC) = 7;
    }
}

HOOK(0x006AA570, NetGatheringDdl_CreateGatheringEntries, "auto");
