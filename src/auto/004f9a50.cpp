#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0083f1f8_ActivateCheatOnObject, 0x004F98A0, (C_0083f1f8* self, void* , int activationMode));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SetConsoleControlState, 0x00712CE0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, MenuControls_ApplyChildIndex, 0x00712D90, (void* self, void* edx, int index));

CSF_GLOBAL(int, g_Value8B055C, 0x008B055C);

static uint32_t* __fastcall C_0083f1f8_ApplyState(
    C_0083f1f8* self, void* /*edx*/, int firstValue, int secondValue)
{
    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    uint32_t* result = CSF_AT(uint32_t*, globalObject, 0x1C);
    if (!result)
        return result;

    int activationMode;
    if (CSF_AT(int, result, 0x248) == firstValue &&
        CSF_AT(int, result, 0x24C) == secondValue)
    {
        activationMode = 2;
    }
    else if (CSF_AT(int, result, 0x25C) == firstValue &&
             CSF_AT(int, result, 0x260) == secondValue)
    {
        activationMode = 0;
    }
    else if (CSF_AT(int, result, 0x270) == firstValue &&
             CSF_AT(int, result, 0x274) == secondValue)
    {
        activationMode = 1;
    }
    else
    {
        return result;
    }

    int controlState = static_cast<int>(self->field_000c);
    uint32_t previousMode = self->field_0100;

    void* referencedObject = GetReferencedObject(self, nullptr);
    SetConsoleControlState(referencedObject, nullptr, controlState);

    int childIndex = g_Value8B055C;
    referencedObject = GetReferencedObject(self, nullptr);
    MenuControls_ApplyChildIndex(referencedObject, nullptr, childIndex);

    self->field_000c = 0;
    self->field_0010 = 0;

    if (previousMode != static_cast<uint32_t>(activationMode))
        return C_0083f1f8_ActivateCheatOnObject(self, nullptr, activationMode);

    return result;
}

HOOK(0x004F9A50, C_0083f1f8_ApplyState, "auto");
