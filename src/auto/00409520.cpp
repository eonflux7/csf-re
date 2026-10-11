#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Negative keys are resolved through the owner's registry and sub-table; on success the
// matched node is asked to fill the output slot at a2. Otherwise a new matrix is created.
static int __fastcall C_0082d844_ResolveMatrixByKey(C_0082d844* self, void* /*edx*/, int a2, int a3)
{
    if (a3 < 0) {
        void* owner = reinterpret_cast<void*>(self->field_0008);
        // owner->vtbl[10]()
        void* registry = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[10])(owner, nullptr);
        // registry->vtbl[26]()
        void* subTable = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, registry, 0)[26])(registry, nullptr);
        if (subTable != nullptr) {
            // subTable->vtbl[26](a3 & 0x7FFFFFFF)
            int keyIndex = ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, subTable, 0)[26])(
                subTable, nullptr, a3 & 0x7FFFFFFF);
            // subTable->vtbl[27](keyIndex)
            void* node = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, subTable, 0)[27])(
                subTable, nullptr, keyIndex);
            if (node != nullptr) {
                // node->vtbl[7](a2)
                ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, node, 0)[7])(node, nullptr, a2);
                return a2;
            }
        }
    }

    void* out = reinterpret_cast<void*>(a2);
    CSF_AT(std::uint32_t*, out, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, out, 4) = 1;
    return a2;
}

HOOK(0x00409520, C_0082d844_ResolveMatrixByKey, "auto");
