#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4116B0, 0x004116B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_40FDF0, 0x0040FDF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the object's vtable, releases the counted array held in field_014c
// (calling its slot-0 destructor if the element count is nonzero, otherwise
// freeing the raw block), points two fields at the 0x82D410 vtable, then
// tail-forwards to the base cleanup routine.
static int __fastcall C_0082dfd0_ReleaseResources(C_0082dfd0* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x0082DFD0);
    sub_4116B0(self, nullptr);

    void* elements = reinterpret_cast<void*>(self->field_014c);
    if (elements != nullptr) {
        void* block = static_cast<uint8_t*>(elements) - 4;
        uint32_t elementCount = CSF_AT(uint32_t, block, 0);
        if (elementCount != 0) {
            void* obj = elements;
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0])(obj, nullptr, 3);
        } else {
            Mem_Free(block);
        }
        self->field_014c = 0;
    }

    self->field_0140 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_0118 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    return sub_40FDF0(self, nullptr);
}

HOOK(0x412000, C_0082dfd0_ReleaseResources, "auto");
