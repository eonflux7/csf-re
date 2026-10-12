#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, SceneMissionSetup_InitializeNamedEntry, 0x006CFC10, (void* self, void* edx, int a2, unsigned int a3));
CSF_FUNC(char, __fastcall, SceneMissionSetup_InsertNamedEntry, 0x006D0110, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, sub_6E0FD0, 0x006E0FD0, (void* self, void* edx, uint32_t* a2));

static int __fastcall SceneMissionSetup_FindNamedEntry(
    char* self, void* /*edx*/, const char* name)
{
    alignas(8) std::uint8_t nameBytes[0x20];
    CSF_AT(void*, nameBytes, 0x1C) = self;
    CSF_AT(std::uint32_t, nameBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, nameBytes, 0x14) = 0;
    CSF_AT(std::uint8_t, nameBytes, 0x04) = 0;

    AssignStringRange_429B60(
        nameBytes, nullptr, const_cast<char*>(name),
        static_cast<unsigned int>(std::strlen(name)));

    void* entries = self + 4;
    int entry = sub_6E0FD0(
        entries, nullptr, reinterpret_cast<uint32_t*>(nameBytes));

    if (!entry)
    {
        void* block = Mem_Alloc(96);
        if (block)
        {
            CSF_AT(std::uint32_t, block, 0x14) = 0;
            CSF_AT(std::uint32_t, block, 0x18) = 15;
            CSF_AT(std::uint8_t, block, 0x04) = 0;
            CSF_AT(std::uint32_t, block, 0x34) = 15;
            CSF_AT(std::uint32_t, block, 0x30) = 0;
            CSF_AT(std::uint8_t, block, 0x20) = 0;
            CSF_AT(std::uint32_t, block, 0x3C) = 0;
            CSF_AT(std::uint32_t, block, 0x40) = 0;
            CSF_AT(std::uint32_t, block, 0x44) = 0;
            CSF_AT(std::uint32_t, block, 0x48) = 10;
            CSF_AT(std::uint32_t, block, 0x4C) = 0;
            CSF_AT(std::uint32_t, block, 0x50) = 0;
            CSF_AT(std::uint32_t, block, 0x54) = 0;
            CSF_AT(std::uint32_t, block, 0x58) = 10;
            CSF_AT(std::uint8_t, block, 0x38) = 0;
            CSF_AT(std::uint32_t, block, 0x5C) = 0;
            entry = static_cast<int>(reinterpret_cast<std::uintptr_t>(block));
        }
        else
        {
            entry = 0;
        }

        SceneMissionSetup_InitializeNamedEntry(
            CSF_AT(void*, nameBytes, 0x1C), nullptr,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(name)),
            static_cast<unsigned int>(entry));
        SceneMissionSetup_InsertNamedEntry(
            entries, nullptr, nameBytes, entry);
    }

    if (CSF_AT(std::uint32_t, nameBytes, 0x18) >= 0x10)
    {
        void* storage = CSF_AT(void*, nameBytes, 0x04);
        if (storage)
            Mem_FreeDebug(storage);
    }

    return entry;
}

HOOK(0x006D03B0, SceneMissionSetup_FindNamedEntry, "auto");
