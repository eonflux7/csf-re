#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_unknown_GetLinkedDword, 0x00476080, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Character_RequestMovementAction, 0x00498FF0, (void* self, void* edx));

// Original: if the object at +0x44 yields a non-null linked object whose
// GetLinkedDword() == 2, it requests a movement action on this character.
static void __fastcall C_00833b88_RequestAlternateMovementAction(C_00833b88* self, void* /*edx*/)
{
    void* linkedObject = reinterpret_cast<void*>(self->field_0044);

    // Virtual call at vtable slot 0xD8 / 4 = 54 on the object at +0x44 (type unknown).
    using GetLinkedObjectFn = void* (__fastcall*)(void* obj, void* edx);
    void* resultObject = ((GetLinkedObjectFn)CSF_AT(void**, linkedObject, 0)[0xD8 / 4])(linkedObject, nullptr);

    if (resultObject != nullptr)
    {
        if (C_unknown_GetLinkedDword(resultObject, nullptr) == 2)
        {
            Character_RequestMovementAction(self, nullptr);
        }
    }
}

HOOK(0x499050, C_00833b88_RequestAlternateMovementAction, "auto");
