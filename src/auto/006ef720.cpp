#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResetStreamingObject, 0x006EF440, (void* self, void* edx));

// Virtual slot +0x4 (index 1) of the object stored at this->field_0040.
using ReleaseSlotFn = void(__fastcall*)(void* self, void* edx, int freeFlag);

static void __fastcall C_008621c8_ReleaseResources(C_008621c8* self, void* /*edx*/)
{
    uint8_t streamingActive = CSF_AT(uint8_t, self, 0x3C);

    // Reset the vtable to C_008621c8 before any cleanup calls.
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x008621C8);

    if (streamingActive)
        GfxScene_ResetStreamingObject(self, nullptr);

    uint32_t* attachedObject = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x40));
    if (attachedObject)
    {
        void* attached = reinterpret_cast<void*>(attachedObject);
        reinterpret_cast<ReleaseSlotFn>(CSF_AT(void**, attached, 0)[1])(attached, nullptr, 1);
        CSF_AT(uint32_t, self, 0x40) = 0;
    }

    if (CSF_AT(uint32_t, self, 0x1C) >= 0x10 && CSF_AT(uint32_t, self, 0x08))
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08)));

    CSF_AT(uint32_t, self, 0x1C) = 15;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint8_t, self, 0x08) = 0;

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006EF720, C_008621c8_ReleaseResources, "auto");
