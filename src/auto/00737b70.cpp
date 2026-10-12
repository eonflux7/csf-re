#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, InitializeResourceWrapper, 0x00737A70, (void* self, void* edx, C_008655e8* owner));

static std::uint32_t* __fastcall C_00867260_CloneResourceWrapper(
    C_00867260* self, void* /*edx*/)
{
    void* storage = Mem_Alloc(144);
    void* clone = storage
        ? InitializeResourceWrapper(
              storage, nullptr, CSF_AT(C_008655e8*, self, 0x0C))
        : nullptr;

    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, clone, 0)[7])(
            clone, nullptr, CSF_AT(std::uint32_t, self, 0x18));

    ((void(__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, clone, 0)[8])(
            clone, nullptr, CSF_AT(std::uint32_t, self, 0x1C));

    const void* sourceData = CSF_AT(void*, self, 0x84);
    void* destinationData = CSF_AT(void*, clone, 0x84);
    std::memcpy(destinationData, sourceData, 0x40u);

    CSF_AT(std::uint32_t, clone, 0x10) = CSF_AT(std::uint32_t, self, 0x10);
    CSF_AT(std::uint32_t, clone, 0x24) = CSF_AT(std::uint32_t, self, 0x24);
    CSF_AT(std::uint32_t, clone, 0x28) = CSF_AT(std::uint32_t, self, 0x28);
    CSF_AT(std::uint32_t, clone, 0x2C) = CSF_AT(std::uint32_t, self, 0x2C);
    CSF_AT(std::uint32_t, clone, 0x30) = CSF_AT(std::uint32_t, self, 0x30);
    CSF_AT(std::uint32_t, clone, 0x34) = CSF_AT(std::uint32_t, self, 0x34);
    CSF_AT(std::uint32_t, clone, 0x38) = CSF_AT(std::uint32_t, self, 0x38);
    CSF_AT(std::uint32_t, clone, 0x3C) = CSF_AT(std::uint32_t, self, 0x3C);
    CSF_AT(std::uint32_t, clone, 0x40) = CSF_AT(std::uint32_t, self, 0x40);
    CSF_AT(std::uint32_t, clone, 0x44) = CSF_AT(std::uint32_t, self, 0x44);
    CSF_AT(std::uint32_t, clone, 0x48) = CSF_AT(std::uint32_t, self, 0x48);
    CSF_AT(std::uint32_t, clone, 0x4C) = CSF_AT(std::uint32_t, self, 0x4C);
    CSF_AT(std::uint32_t, clone, 0x50) = CSF_AT(std::uint32_t, self, 0x50);
    CSF_AT(std::uint32_t, clone, 0x54) = CSF_AT(std::uint32_t, self, 0x54);
    CSF_AT(std::uint32_t, clone, 0x58) = CSF_AT(std::uint32_t, self, 0x58);
    CSF_AT(std::uint32_t, clone, 0x5C) = CSF_AT(std::uint32_t, self, 0x5C);
    CSF_AT(std::uint32_t, clone, 0x60) = CSF_AT(std::uint32_t, self, 0x60);
    CSF_AT(std::uint32_t, clone, 0x64) = CSF_AT(std::uint32_t, self, 0x64);
    CSF_AT(std::uint32_t, clone, 0x68) = CSF_AT(std::uint32_t, self, 0x68);
    CSF_AT(std::uint32_t, clone, 0x6C) = CSF_AT(std::uint32_t, self, 0x6C);
    CSF_AT(std::uint32_t, clone, 0x70) = CSF_AT(std::uint32_t, self, 0x70);
    CSF_AT(std::uint32_t, clone, 0x74) = CSF_AT(std::uint32_t, self, 0x74);
    CSF_AT(std::uint32_t, clone, 0x78) = CSF_AT(std::uint32_t, self, 0x78);
    CSF_AT(std::uint32_t, clone, 0x7C) = CSF_AT(std::uint32_t, self, 0x7C);
    CSF_AT(std::uint32_t, clone, 0x80) = CSF_AT(std::uint32_t, self, 0x80);
    CSF_AT(std::uint8_t, clone, 0x8C) = CSF_AT(std::uint8_t, self, 0x8C);

    return reinterpret_cast<std::uint32_t*>(clone);
}

HOOK(0x00737B70, C_00867260_CloneResourceWrapper, "auto");
