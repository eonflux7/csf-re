#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees bound by address (copied from the packet).
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_681FC0, 0x00681FC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(unsigned int, __fastcall, sub_6C4CF0, 0x006C4CF0, (void* self, void* edx, unsigned int a2));

// Original: unsigned int __thiscall sub_6C5820(C_0085e7b8 **this, int a2); ret 4
// Picks a scene region field from the object at a2 based on the low bit of its
// flags byte, then stores the result through the object returned by a2's vtable slot 7.
static unsigned int __fastcall GfxSkin_ApplyRegionFieldsByFlag(C_0085e7b8** self, void* /*edx*/, int a2)
{
    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));

    // v3 = sub_681FC0(a2)
    int dispatchTarget = sub_681FC0(obj, nullptr);

    // v4 = a2->vtbl[7](a2)  (offset 0x1C)
    uint8_t* flagsByte = reinterpret_cast<uint8_t*>(
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[7])(obj, nullptr));

    void* dispatchObj = reinterpret_cast<void*>(static_cast<uintptr_t>(dispatchTarget));

    if ((*flagsByte & 1) != 0) {
        int regionFor5 = GetSceneRegionAt(self[1], nullptr, 5);
        C_008670f0* fieldObj5 = reinterpret_cast<C_008670f0*>(
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, dispatchObj, 0)[7])(dispatchObj, nullptr));
        C_008670f0_SetField0C(fieldObj5, nullptr, regionFor5);

        unsigned int regionFor7 = static_cast<unsigned int>(GetSceneRegionAt(self[1], nullptr, 7));
        C_008670f0* fieldObj7 = reinterpret_cast<C_008670f0*>(
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, dispatchObj, 0)[7])(dispatchObj, nullptr));
        return sub_6C4CF0(fieldObj7, nullptr, regionFor7);
    }
    else {
        int regionFor7 = GetSceneRegionAt(self[1], nullptr, 7);
        C_008670f0* fieldObj = reinterpret_cast<C_008670f0*>(
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, dispatchObj, 0)[7])(dispatchObj, nullptr));
        return static_cast<unsigned int>(C_008670f0_SetField0C(fieldObj, nullptr, regionFor7));
    }
}

HOOK(0x006C5820, GfxSkin_ApplyRegionFieldsByFlag, "auto");
