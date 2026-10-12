#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860288_UpdateClumpState, 0x006B3B40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_StoreEntryTableValue, 0x006B88A0, (int offset, int value));
CSF_FUNC(int, __cdecl, GfxObjectProps_VisitAtomicsThenProcessMaterials, 0x006BBF30, (int clumpHandle));
CSF_FUNC(uint32_t*, __fastcall, GfxObject_InitializeFrameData, 0x006BF1E0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxObject_SetFrameDataSource, 0x006BF380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));

static int __fastcall C_00860288_InitializeClumpResources(
    C_00860288* self, void* /*edx*/)
{
    if (self->field_000c)
    {
        if (self->field_0010 == static_cast<uint32_t>(-1))
        {
            GfxObjectProps_VisitAtomicsThenProcessMaterials(
                static_cast<int>(self->field_000c));
        }

        GfxObject_SetFrameDataSource(
            &self->field_0030, nullptr,
            CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x04));
        C_00860288_UpdateClumpState(self, nullptr);
    }
    else
    {
        GfxObject_InitializeFrameData(&self->field_0030, nullptr);
    }

    GfxObjectProps_StoreEntryTableValue(
        static_cast<int>(self->field_003c), reinterpret_cast<int>(self));

    if (self->field_000c)
    {
        RpClumpForAllAtomics(
            static_cast<int>(self->field_000c),
            reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006B2910)),
            0);
    }

    self->field_0060 = 0;

    void* helper = Mem_Alloc(28);
    if (helper)
    {
        C_00860178_Construct(reinterpret_cast<C_00860178*>(helper), nullptr);
        CSF_AT(void**, helper, 0x00) = CSF_VTBL(0x00860340);
        C_00860894_Init(static_cast<uint8_t*>(helper) + 8, nullptr, -1, 0);
        CSF_AT(C_00860288*, helper, 0x04) = self;
    }

    self->field_0068 = reinterpret_cast<uint32_t>(helper);
    self->field_0078 = static_cast<uint32_t>(-1082130432);

    void* bounds = Mem_Alloc(16);
    if (bounds)
    {
        CSF_AT(uint8_t, bounds, 0x01) = static_cast<uint8_t>(-1);
        CSF_AT(uint8_t, bounds, 0x02) = static_cast<uint8_t>(-1);
        CSF_AT(uint8_t, bounds, 0x00) = 1;
        CSF_AT(uint32_t, bounds, 0x04) = 1287568416;
        CSF_AT(uint32_t, bounds, 0x08) = 1287568416;
        CSF_AT(uint32_t, bounds, 0x0C) = 1287568416;
        self->field_0084 = reinterpret_cast<uint32_t>(bounds);
    }
    else
    {
        self->field_0084 = 0;
    }

    self->field_0088 = 0;
    return reinterpret_cast<int>(bounds);
}

HOOK(0x006B3FE0, C_00860288_InitializeClumpResources, "auto");
