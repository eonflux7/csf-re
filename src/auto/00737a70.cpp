#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));
CSF_FUNC(C_0085e65c*, __fastcall, ConstructFontBase, 0x00692390, (C_0085e65c* self, void* edx));

static std::uint32_t* __fastcall C_00867260_InitializeResourceWrapper(
    C_00867260* self, void* /*edx*/, int resource)
{
    auto* fontBase = reinterpret_cast<C_0085e65c*>(
        reinterpret_cast<std::uint8_t*>(self) + 4);

    self->vtbl = CSF_VTBL(0x008670CC);
    ConstructFontBase(fontBase, nullptr);
    self->vtbl = CSF_VTBL(0x00867260);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x0086725C);

    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(float, self, 0x2C) = 0.0f;
    CSF_AT(float, self, 0x30) = 0.0f;
    CSF_AT(float, self, 0x34) = 0.0f;
    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0x40) = 0;
    CSF_AT(float, self, 0x44) = 0.0f;
    CSF_AT(float, self, 0x48) = 0.0f;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint32_t, self, 0x50) = 0;
    CSF_AT(float, self, 0x58) = 0.0f;
    CSF_AT(float, self, 0x5C) = 0.0f;
    CSF_AT(float, self, 0x60) = 0.0f;
    CSF_AT(float, self, 0x64) = 0.0f;
    CSF_AT(float, self, 0x68) = 0.0f;
    CSF_AT(float, self, 0x6C) = 0.0f;
    CSF_AT(float, self, 0x70) = 0.0f;
    CSF_AT(float, self, 0x74) = 1.0f;
    CSF_AT(std::uint32_t, self, 0x7C) = 0;
    CSF_AT(std::uint32_t, self, 0x80) = 0;

    CSF_AT(std::uint32_t*, self, 0x84) = RwMatrixCreate();
    CSF_AT(std::uint8_t, self, 0x88) = 1;
    CSF_AT(int, self, 0x0C) = resource;
    CSF_AT(std::uint32_t, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = AllocateAlignedBuffer(0x70u, 64, 0, 0);

    CSF_AT(float, self, 0x68) = 255.0f;
    CSF_AT(float, self, 0x6C) = 255.0f;
    CSF_AT(float, self, 0x70) = 255.0f;
    CSF_AT(float, self, 0x74) = 255.0f;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x54) = 0;
    CSF_AT(std::uint32_t, self, 0x78) = 0;
    CSF_AT(std::uint8_t, self, 0x8C) = 0;
    CSF_AT(std::uint8_t, self, 0x20) = 1;

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00737A70, C_00867260_InitializeResourceWrapper, "auto");
