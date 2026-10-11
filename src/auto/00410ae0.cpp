#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_410740, 0x00410740, (void* self, void* edx, float a3));
CSF_FUNC(char, __fastcall, sub_41E090, 0x0041E090, (void* self, void* edx, int a2));

// Forwards the notification to the child object at +0x10 (vtable slot 5), then
// runs the local refresh routine. The final update is skipped when the flag at +0x14 is set.
static void __fastcall C_0082deb8_NotifyChildAndRefresh(C_0082deb8* self, void* /*edx*/, int a2)
{
    if (self->field_000c)
    {
        uint32_t childObjectAddress = self->field_0010;
        void* childObject = reinterpret_cast<void*>(childObjectAddress);

        // Slot +0x14 of the child's vtable (index 5).
        using ChildNotifyFn = void(__fastcall*)(void* self, void* edx, int a2);
        ChildNotifyFn notifyChild = reinterpret_cast<ChildNotifyFn>(CSF_AT(void**, childObject, 0)[5]);
        notifyChild(childObject, nullptr, a2);

        sub_41E090(self, nullptr, a2);

        if (!self->field_0014)
        {
            // The original passes the stack dword a2 reinterpreted as a float.
            float a2AsFloat;
            std::memcpy(&a2AsFloat, &a2, sizeof(a2AsFloat));
            sub_410740(self, nullptr, a2AsFloat);
        }
    }
}

HOOK(0x00410AE0, C_0082deb8_NotifyChildAndRefresh, "auto");
