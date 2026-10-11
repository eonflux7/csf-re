#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __fastcall, Ai_GetTargetStatusFlagPtr, 0x004D1510, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, Ai_GetEmbeddedObjectAt, 0x004DB190, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Ai_ApplyTimedFlagUpdate, 0x00421B30, (void* self, void* edx, float a2, char a3));

// The object pointed to by this->field_0008 (the AI state owner).
static uint8_t* GetAiObject(S_004223f0* self)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
}

static int __fastcall S_004223f0_UpdateFlagAndDispatch(S_004223f0* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, GetAiObject(self), 0x205) != 0)
    {
        void* owner = CSF_AT(void*, GetAiObject(self), 0x14);
        void* target = CSF_AT(void*, owner, 0x34);

        if (*Ai_GetTargetStatusFlagPtr(target, nullptr) != 0)
        {
            CSF_AT(uint8_t, GetAiObject(self), 0x205) = 0;
        }
        else
        {
            void* subObject = Ai_GetEmbeddedObjectAt(self, nullptr,
                static_cast<int>(self->field_0008 + 0x1A8));
            Ai_ApplyTimedFlagUpdate(subObject, nullptr,
                CSF_AT(float, GetAiObject(self), 0x208), 1);
        }
    }

    if (CSF_AT(uint8_t, GetAiObject(self), 0x205) == 0)
    {
        void* result = Ai_GetEmbeddedObjectAt(self, nullptr,
            static_cast<int>(self->field_0008 + 0x194));
        return static_cast<int>(reinterpret_cast<uintptr_t>(result));
    }

    return static_cast<int>(self->field_0008);
}

HOOK(0x422570, S_004223f0_UpdateFlagAndDispatch, "auto");
