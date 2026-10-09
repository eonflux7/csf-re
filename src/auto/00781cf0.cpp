#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, DestroyArrayWithCount_775200, 0x00775200, (int* a1, int* a2, int a3, int a4));
CSF_FUNC(int, __cdecl, DestroyArrayWithCount_7752E0, 0x007752E0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, DestroyArrayWithCount_7754B0, 0x007754B0, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, ResetInlineContainer_776A00, 0x00776A00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResetSubobjectHeader_77D520, 0x0077D520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseBaseSubobject_77D5F0, 0x0077D5F0, (void* self, void* edx));

// Releases every buffer owned by a CLevelData instance and resets its members
// to the empty state. Original body of the level-data teardown (0x00781CF0).
static int __fastcall CLevelData_ReleaseAll(CLevelData* self, void* /*edx*/) {
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008723F8);

    ReleaseBaseSubobject_77D5F0(self, nullptr);

    // String-like member at +0x1F74: free its heap buffer if it was allocated
    // (capacity >= 16 means the inline buffer is not in use), then mark it empty.
    if (self->field_1f88 >= 0x10 && self->field_1f74 != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_1f74));
    }
    self->field_1f88 = 15;
    self->field_1f84 = 0;
    CSF_AT(uint8_t, self, 0x1F74) = 0;

    // Same handling for the member at +0x1F58.
    if (self->field_1f6c >= 0x10 && self->field_1f58 != 0) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_1f58));
    }
    self->field_1f6c = 15;
    self->field_1f68 = 0;
    CSF_AT(uint8_t, self, 0x1F58) = 0;
    ResetInlineContainer_776A00(reinterpret_cast<uint8_t*>(self) + 0x54, nullptr);

    // Array at +0x48 with its header at +0x44 (count at +0x4C).
    uint32_t blockHandle48 = self->field_0048;
    if (blockHandle48 != 0) {
        DestroyArrayWithCount_7752E0(
            static_cast<int>(blockHandle48),
            static_cast<int>(CSF_AT(uint32_t, self, 0x4C)),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x44),
            reinterpret_cast<int>(self));
        if (self->field_0048 != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0048));
        }
    }
    self->field_0048 = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;

    // Array at +0x38 with its header at +0x34 (count at +0x3C).
    uint32_t blockHandle38 = self->field_0038;
    if (blockHandle38 != 0) {
        DestroyArrayWithCount_7754B0(
            static_cast<int>(blockHandle38),
            static_cast<int>(CSF_AT(uint32_t, self, 0x3C)),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x34),
            reinterpret_cast<int>(self));
        if (self->field_0038 != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0038));
        }
    }
    self->field_0038 = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;

    ResetSubobjectHeader_77D520(reinterpret_cast<uint8_t*>(self) + 0x24, nullptr);

    // Array at +0x18 with its header at +0x14 (count at +0x1C).
    uint32_t blockHandle18 = self->field_0018;
    if (blockHandle18 != 0) {
        DestroyArrayWithCount_775200(
            reinterpret_cast<int*>(blockHandle18),
            reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x1C)),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x14),
            reinterpret_cast<int>(self));
        if (self->field_0018 != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
        }
    }
    self->field_0018 = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;

    // Final owned block at +0x04 is released with Mem_Free (not the debug variant).
    int result = static_cast<int>(self->field_0004);
    if (result != 0) {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x00781CF0, CLevelData_ReleaseAll, "auto");
