#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Character_MoveByVectorStandard, 0x00498A40, (void* self, void* edx, float* a3));
CSF_FUNC(void, __fastcall, Character_MoveByVectorFlagged, 0x00498C90, (void* self, void* edx, float* a3));

// Applies a direction vector to the character when the scene object is
// eligible for movement. Flag 0x2000 in the word at +0x2E0 selects the
// alternate movement path. Then calls the object's vtable slot 0xC4.
static int __fastcall Character_ApplyVectorInput(C_00833b88* self, void* /*edx*/, float* a2)
{
    int result = SceneObject_PassesFirstCheck(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
    {
        result = SceneObj_IsInactive(self, nullptr);
        if (static_cast<uint8_t>(result) == 0)
        {
            uint32_t movementFlags = CSF_AT(uint32_t, self, 0x2E0);
            if ((movementFlags & 0x2000) != 0)
                Character_MoveByVectorFlagged(self, nullptr, a2);
            else
                Character_MoveByVectorStandard(self, nullptr, a2);

            return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xC4 / 4])(self, nullptr);
        }
    }
    return result;
}

HOOK(0x4998E0, Character_ApplyVectorInput, "auto");
