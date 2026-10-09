#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_4460A0, 0x004460A0, (void* self, void* edx, int a2, int a3, uint32_t* a4, int a5));
CSF_FUNC(unsigned int*, __fastcall, SceneRequest_Submit, 0x00446F10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, IsFieldAt10Equal36, 0x00483360, (void* self, void*));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4968D0, 0x004968D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_496940, 0x00496940, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_499270, 0x00499270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_49A070, 0x0049A070, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_49B2C0, 0x0049B2C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Per-frame update of the character's action-state flags:
//   field_02e0 = current (active) flags
//   field_02e4 = requested flags
//   field_02e8 = filtered / pending flags
static unsigned int* __fastcall C_00833ee8_UpdateActionFlags(C_00833ee8* self, void* /*edx*/)
{
    uint32_t activeFlags = self->field_02e0;
    if ((activeFlags & 0x88000) != 0)
        return reinterpret_cast<unsigned int*>(activeFlags);

    // Both predicates return their result in al only; the upper bytes of eax are garbage.
    if ((activeFlags & 0x1000) != 0 ||
        (IsFieldAt10Equal36(reinterpret_cast<void*>(self->field_0044), nullptr) & 0xFF) != 0) {
        sub_49A070(self, nullptr);
    }

    sub_499270(self, nullptr);

    if ((sub_4968D0(self, nullptr, static_cast<int>(self->field_02e4)) & 0xFF) != 0) {
        // Requested flags differ from the active ones: filter them into the pending set.
        sub_496940(self, nullptr);
        if ((sub_4968D0(self, nullptr, static_cast<int>(self->field_02e8)) & 0xFF) != 0)
            sub_49B2C0(self, nullptr);
    }

    activeFlags = self->field_02e0;

    // Drop one-shot requests that did not become active.
    if ((activeFlags & 8) == 0)
        self->field_02e4 &= ~8u;
    if ((activeFlags & 0x100) == 0)
        self->field_02e4 &= ~0x100u;
    if ((activeFlags & 0x20000) == 0)
        self->field_02e4 &= ~0x20000u;
    if ((activeFlags & 0x80000) == 0)
        self->field_02e4 &= ~0x80000u;
    if ((activeFlags & 0x80) == 0)
        self->field_02e4 &= ~0x80u;
    if (self->field_03dd != 0)
        self->field_02e4 &= ~0x10u;

    if ((activeFlags & 2) != 0) {
        uint8_t sceneRequest[0x28] = {};

        const int requestParam = CSF_AT(int, GetGlobalValue_8BC1E0(), 0xA74);
        const int positionVector = GetObjectPositionVector(self, nullptr);

        sub_4460A0(sceneRequest, nullptr, 3,
                   reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x1C),
                   reinterpret_cast<uint32_t*>(positionVector),
                   requestParam);

        return SceneRequest_Submit(CSF_AT(void*, g_pCurrentScene, 0xB0), nullptr,
                                   reinterpret_cast<int>(sceneRequest));
    }

    return reinterpret_cast<unsigned int*>(activeFlags);
}

HOOK(0x0049BDF0, C_00833ee8_UpdateActionFlags, "auto");
