#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, sub_433FE0, 0x00433FE0, (void* a1, void* a2, void* a3));
CSF_FUNC(int, __cdecl, Hud_ProcessWidgetPair, 0x00433B90,
         (int* firstArgument, int* secondArgument, std::uintptr_t context, int* repeatedArgument));

static void** __fastcall SetHudWidgetValue(
    S_00434780* self,
    void* /*edx*/,
    void** outputSlot,
    void* currentValue,
    void* previousValue)
{
    if (currentValue != previousValue)
    {
        void* newWidget = sub_433FE0(
            previousValue,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
            currentValue);

        Hud_ProcessWidgetPair(
            reinterpret_cast<int*>(newWidget),
            reinterpret_cast<int*>(static_cast<std::uintptr_t>(self->field_0008)),
            reinterpret_cast<std::uintptr_t>(self),
            reinterpret_cast<int*>(outputSlot));

        self->field_0008 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(newWidget));
    }

    *outputSlot = currentValue;
    return outputSlot;
}

HOOK(0x00434780, SetHudWidgetValue, "auto");
