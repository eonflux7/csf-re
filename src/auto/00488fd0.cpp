#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x004812A0 - applies the integer argument to the action FSM (result unused by the caller).
CSF_FUNC(int, __fastcall, CharActionFsm_ApplyIntArg, 0x4812A0,
         (void* self, void* /*edx*/, int a2));

// 0x00488E70 - returns the wide-string label for the float argument.
CSF_FUNC(wchar_t*, __fastcall, CharActionFsm_GetFloatLabel, 0x488E70,
         (void* self, void* /*edx*/, float a2));

// Reinterprets the stack dword as a float, as the original does when it passes a2 on.
static float IntBitsToFloat(int bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

// 0x00488FD0 - applies the argument to the FSM, then returns the label for the same value.
static wchar_t* __fastcall CCharActionFsm_ApplyArgAndGetLabel(CCharActionFsm* self, void* /*edx*/, int a2)
{
    CharActionFsm_ApplyIntArg(self, nullptr, a2);
    return CharActionFsm_GetFloatLabel(self, nullptr, IntBitsToFloat(a2));
}

HOOK(0x488FD0, CCharActionFsm_ApplyArgAndGetLabel, "auto");
