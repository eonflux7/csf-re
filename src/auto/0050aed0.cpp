#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(int, g_Value_8BB450, 0x008BB450);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));

static C_00860130* __fastcall OptionsFile_PushScaledFloatToObject(float* self, void* /*edx*/)
{
    C_00860130* result = reinterpret_cast<C_00860130*>(static_cast<intptr_t>(g_Value_8BB450));
    if (g_Value_8BB450) {
        int globalObjectId = GetGlobalInt();
        void* targetObject = LookupObjectById(reinterpret_cast<void*>(static_cast<intptr_t>(globalObjectId)), nullptr);

        float scaled = CSF_AT(float, self, 0x10) * g_Const200_0f;
        int64_t scaledValue = static_cast<int64_t>(scaled);
        uint32_t lowWord = static_cast<uint32_t>(scaledValue);

        // Slot 0x4C / 4 = 19 on the target object's vtable; one stack argument (low dword).
        using SlotFn = void*(__fastcall*)(void* self, void* edx, uint32_t value);
        SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, targetObject, 0)[19]);
        return reinterpret_cast<C_00860130*>(slot(targetObject, nullptr, lowWord));
    }
    return result;
}

HOOK(0x0050AED0, OptionsFile_PushScaledFloatToObject, "auto");
