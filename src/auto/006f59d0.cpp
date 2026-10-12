#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: GfxCollision_MergeSortedIntersections (0x006ED600), __thiscall, ret 4.
CSF_FUNC(int, __fastcall, GfxCollision_MergeSortedIntersections, 0x006ED600, (uint8_t* self, void* , uint8_t* other));

// Calls slot 1 (reset/begin) on the embedded sub-object at +0x14, then runs slot 15 on every
// element of the list at +0x04 (count at +0x0C), merging each non-null intersection result into the
// embedded sub-object. Finally returns the embedded sub-object if slot 2 reports false, else null.
// Stack args: 4 (ret 0x10). The fourth stack argument is unused by the original.
static uint32_t* __fastcall C_008627d8_CollectClipIntersections(C_008627d8* self, void* /*edx*/,
                                                               uint32_t a3, uint32_t a4, uint32_t a5, uint32_t /*a6*/)
{
    uint8_t* embedded = reinterpret_cast<uint8_t*>(self) + 0x14;

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, embedded, 0)[1])(embedded, nullptr);

    uint32_t index = 0;
    if (self->field_000c != 0) {
        do {
            uint32_t* elements = reinterpret_cast<uint32_t*>(self->field_0004);
            uint8_t* element = reinterpret_cast<uint8_t*>(elements[index]);

            int intersection = ((int(__fastcall*)(void*, void*, uint32_t, uint32_t, uint32_t, uint32_t))
                                    CSF_AT(void**, element, 0)[15])(element, nullptr, a3, a4, a5, 3u);
            if (intersection != 0) {
                GfxCollision_MergeSortedIntersections(
                    embedded, nullptr, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(intersection)));
            }

            ++index;
        } while (index < self->field_000c);
    }

    unsigned char isDone = ((unsigned char(__fastcall*)(void*, void*))CSF_AT(void**, embedded, 0)[2])(embedded, nullptr);
    return isDone == 0 ? reinterpret_cast<uint32_t*>(embedded) : nullptr;
}

HOOK(0x006F59D0, C_008627d8_CollectClipIntersections, "auto");
