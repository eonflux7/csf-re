#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(uint32_t*, __fastcall, GfxHAnim_InitializeHierarchyStorage, 0x006E8E30, (void* self, void* edx, int a2, int a3));

CSF_GLOBAL(std::uint32_t*, g_HAnimSharedMatrixA, 0x008BBABC);
CSF_GLOBAL(std::uint32_t*, g_HAnimSharedMatrixB, 0x008BBAC0);
CSF_GLOBAL(std::uint32_t*, g_HAnimSharedMatrixC, 0x008BBAC4);
CSF_GLOBAL(std::uint32_t, g_HAnimInstanceCount, 0x008BBAC8);

static int* __fastcall C_00861e98_Construct(C_00861e98* self, void* /*edx*/)
{
    C_00860178_Construct(static_cast<C_00860178*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00861E98);
    GfxHAnim_InitializeHierarchyStorage(
        reinterpret_cast<std::uint8_t*>(self) + 4, nullptr, 1, 1);

    CSF_AT(int, self, 0x18) = -1;
    CSF_AT(int, self, 0x30) = -1;
    CSF_AT(int, self, 0x34) = -1;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0x40) = 0;
    CSF_AT(float, self, 0x44) = 1.0f;
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(float, self, 0x54) = 1.0f;
    CSF_AT(int, self, 0x58) = 10;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint32_t, self, 0x50) = 0;

    CSF_AT(std::uint32_t, self, 0x60) = 0;
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint32_t, self, 0x68) = 0;
    CSF_AT(std::uint32_t, self, 0x6C) = 0;
    CSF_AT(std::uint32_t, self, 0x70) = 0;
    CSF_AT(std::uint32_t, self, 0x74) = 0;
    CSF_AT(std::uint32_t, self, 0x78) = 0;
    CSF_AT(std::uint32_t, self, 0x7C) = 0;
    CSF_AT(std::uint32_t, self, 0x80) = 0;
    CSF_AT(std::uint32_t, self, 0x84) = 0;
    CSF_AT(std::uint32_t, self, 0x88) = 0;
    CSF_AT(std::uint32_t, self, 0x8C) = 0;
    CSF_AT(std::uint32_t, self, 0x90) = 0;
    CSF_AT(std::uint32_t, self, 0x94) = 0;
    CSF_AT(std::uint32_t, self, 0x98) = 0;
    CSF_AT(std::uint32_t, self, 0x9C) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;

    CSF_AT(std::uint32_t*, self, 0x48) = RwMatrixCreate();
    if (g_HAnimInstanceCount == 0 && g_HAnimSharedMatrixA == nullptr)
    {
        g_HAnimSharedMatrixA = RwMatrixCreate();
        g_HAnimSharedMatrixB = RwMatrixCreate();
        g_HAnimSharedMatrixC = RwMatrixCreate();
    }
    ++g_HAnimInstanceCount;
    return reinterpret_cast<int*>(self);
}

HOOK(0x006E8F50, C_00861e98_Construct, "auto");
