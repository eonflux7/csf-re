#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_406610, 0x00406610, (void* self, void* edx, int a2, char a3, char a4));

// Sets the byte flag at +0x38 to the requested state, notifies the helper at 0x406610,
// then forwards to the attached object's vtable: slot 10 (enable/start) when a2 is set,
// slot 11 (disable/stop with a3) otherwise.
static char __fastcall SetStateAndDispatchToTarget(S_004054a0* self, void* /*edx*/, char a2, int a3)
{
    char result = 0;

    if (!a2 || (result = self->field_0039) == 0)
    {
        self->field_0038 = static_cast<uint8_t>(a2);
        sub_406610(self, nullptr, 513, a2, static_cast<char>(a3));

        void* target = reinterpret_cast<void*>(self->field_0040);
        if (target)
        {
            if (a2)
            {
                return static_cast<char>(
                    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, target, 0)[10])(target, nullptr, 0));
            }
            else
            {
                return static_cast<char>(
                    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, target, 0)[11])(target, nullptr, a3));
            }
        }
    }
    return result;
}

HOOK(0x004066D0, SetStateAndDispatchToTarget, "auto");
