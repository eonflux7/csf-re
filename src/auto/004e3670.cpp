#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083e248_RefreshAfterCheck, 0x004E2E70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e248_ApplyScaledValue, 0x004E30B0, (void* self, void* edx, int a3));

static char __fastcall C_0083e248_TryUpdateOnFailedCheck(
    C_0083e248* self, void* /*edx*/, int argument)
{
    char result = static_cast<char>(SceneObject_PassesFirstCheck(
        reinterpret_cast<void*>(self->field_000c), nullptr));

    if (!result)
    {
        void* sceneObject = reinterpret_cast<void*>(self->field_000c);
        double queriedValue =
            reinterpret_cast<double(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, sceneObject, 0)[0x60 / 4])(
                    sceneObject, nullptr, argument);

        // Unresolved: this callee requires queriedValue in ST0.
        // Its ordinary binding preserves ret 4 but cannot deliver that input.
        (void)queriedValue;
        C_0083e248_ApplyScaledValue(self, nullptr, 2);
        return C_0083e248_RefreshAfterCheck(self, nullptr);
    }

    return result;
}

HOOK(0x004E3670, C_0083e248_TryUpdateOnFailedCheck, "auto");
