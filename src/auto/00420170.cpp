#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 0x24 on the object at this->field_0014 + 0x48 (slot 9): returns bool, takes one int argument.
// Virtual slot 0x8C on the character task (slot 35): takes four int arguments and returns int.

static int __fastcall C_0082e7c8_AdvanceStateWhenAnimationDone(C_0082e7c8* self, void* /*edx*/, uint32_t* a2)
{
    int result = (int)(uintptr_t)a2;

    if (a2[1] == 0 && a2[0] == 0) {
        C_0082e598* charTask = reinterpret_cast<C_0082e598*>(self->field_0014);
        result = (int)(uintptr_t)charTask;

        void* animObj = CSF_AT(void*, charTask, 0x48);
        if (animObj != nullptr) {
            bool animDone = ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, animObj, 0)[9])(animObj, nullptr, 0);
            result = animDone ? 1 : 0;

            if (animDone) {
                if (self->field_0020 == 1) {
                    charTask = reinterpret_cast<C_0082e598*>(self->field_0014);
                    return ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, charTask, 0)[35])(
                        charTask, nullptr, 0, 0, -1, 0);
                }

                result = (int)self->field_0020 - 2;
                if (self->field_0020 == 2) {
                    charTask = reinterpret_cast<C_0082e598*>(self->field_0014);
                    if (CSF_AT(int, charTask, 0x1E0) == 1) {
                        return ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, charTask, 0)[35])(
                            charTask, nullptr, 3, 0, -1, 0);
                    }
                    return ((int(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, charTask, 0)[35])(
                        charTask, nullptr, 1, 0, -1, 0);
                }
            }
        }
    }

    return result;
}

HOOK(0x420170, C_0082e7c8_AdvanceStateWhenAnimationDone, "auto");
