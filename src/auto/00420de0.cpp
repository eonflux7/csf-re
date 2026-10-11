#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class routine that finalizes the task exit (bound as in packet).
CSF_FUNC(int, __fastcall, C_00834fa8_ApplyTaskExit, 0x004DB440, (void* self, void* edx, int a2));

// Leaves the free-move task state: optionally stops the owner's animation, notifies the
// owner's embedded sub-object, sets the 0x04 flag on the task record, then applies the base exit.
static int __fastcall TFreeMove_ExitStateAndNotify(void* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    // this->field_0008 : task record pointer
    uint8_t* taskRecord = CSF_AT(uint8_t*, self, 0x08);
    // [taskRecord+0x14] : owning character object
    uint8_t* character = CSF_AT(uint8_t*, taskRecord, 0x14);
    // [character+0x48] : character's state/animation controller
    uint8_t* controller = CSF_AT(uint8_t*, character, 0x48);

    // controller->vtbl[0x24/4](1): if the controller reports active, stop the character's motion
    if (((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, controller, 0)[0x24 / 4])(controller, nullptr, 1))
    {
        ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, character, 0)[0x8C / 4])(
            character, nullptr, 0, 0, -1, 0);
    }

    // Notify the character's embedded sub-object (character + 4) via its vtbl[0x10/4].
    uint8_t* embedded = character + 4;
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, embedded, 0)[0x10 / 4])(embedded, nullptr);

    // taskRecord->flags |= 4
    CSF_AT(uint32_t, taskRecord, 0x18) |= 4u;

    // Return ApplyTaskExit(this, this->field_000c + 0x20)
    uint32_t argument = CSF_AT(uint32_t, self, 0x0C) + 0x20u;
    return C_00834fa8_ApplyTaskExit(self, nullptr, static_cast<int>(argument));
}

HOOK(0x00420DE0, TFreeMove_ExitStateAndNotify, "auto");
