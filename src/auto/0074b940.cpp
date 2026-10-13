#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, ObjectBase_Initialize, 0x0064EBA0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnDescriptor_Initialize, 0x00749A60, (C_008310b8* self, void* edx));
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_NetGhostClassId, 0x008BC174);

static C_008310b8* SubObjectAt(void* base, std::size_t offset)
{
    return reinterpret_cast<C_008310b8*>(reinterpret_cast<uint8_t*>(base) + offset);
}

static int __fastcall C_00867578_Construct(C_00867578* self, void* /*edx*/)
{
    ObjectBase_Initialize(self, nullptr);
    self->vtbl = CSF_VTBL(0x00867578);

    C_008310b8_GetSelf(SubObjectAt(self, 0x44), nullptr);
    CSF_AT(uint8_t, self, 0x45) = 0;
    CSF_AT(uint8_t, self, 0x44) = 0;
    CSF_AT(uint8_t, self, 0x46) = 0;

    C_008310b8_GetSelf(SubObjectAt(self, 0x47), nullptr);
    CSF_AT(uint8_t, self, 0x48) = 0;
    CSF_AT(uint8_t, self, 0x47) = 0;
    CSF_AT(uint8_t, self, 0x49) = 0;

    C_008310b8_GetSelf(SubObjectAt(self, 0x4A), nullptr);
    CSF_AT(uint8_t, self, 0x4B) = 0;
    CSF_AT(uint8_t, self, 0x4A) = 0;
    CSF_AT(uint8_t, self, 0x4C) = 0;

    SpawnDescriptor_Initialize(SubObjectAt(self, 0x50), nullptr);

    C_008310b8_GetSelf(SubObjectAt(self, 0x5C), nullptr);
    CSF_AT(uint8_t, self, 0x60) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x64) = 0;

    C_008310b8_GetSelf(SubObjectAt(self, 0x68), nullptr);
    CSF_AT(uint8_t, self, 0x70) = 0;
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;

    Registry_Attach(reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr, static_cast<int>(g_NetGhostClassId));

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x0074B940, C_00867578_Construct, "auto");
