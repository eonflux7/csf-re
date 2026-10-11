#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_BeginBatch12, 0x006C8390, (void* self, void* edx, int a2));

static char __fastcall GfxSkin_UpdateAndCommitSkins(S_006c5cd0* self, void* /*edx*/) {
    uint32_t objectCount = self->field_00b8;
    self->field_0120 += objectCount;

    if (objectCount == 0) {
        return 0;
    }

    GfxSkin_BeginBatch12(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004)), nullptr, 12);

    for (uint32_t i = 0; i < self->field_00b8; ++i) {
        void** objects = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_00b0));
        void* obj = objects[i];
        // slot 0x14 / 4 = 5
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[5])(obj, nullptr);
    }

    // tail jump: ecx = this->field_0004, callee result is returned as-is
    return GlobalHud_Commit_6C7C30(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004)), nullptr);
}

HOOK(0x006C6190, GfxSkin_UpdateAndCommitSkins, "auto");
