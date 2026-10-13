#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (addresses from the packet).
CSF_FUNC(void, __fastcall, GfxCamera_UpdateFieldOfView, 0x006D2F60, (void* self, void* , float deltaTime));

// Walks the child list at field_0044 (count field_004c). For each child whose
// virtual slot 0x30 returns true, calls its virtual slot 0x10 with a2.
static int __fastcall C_00862e88_UpdateChildrenWithDeltaTime(C_00862e88* self, void* /*edx*/, float a2)
{
    if (self->field_0010)
    {
        GfxCamera_UpdateFieldOfView(reinterpret_cast<void*>(self->field_0010), nullptr, a2);
    }

    int result = static_cast<int>(self->field_004c);
    for (int i = 0; i < static_cast<int>(self->field_004c); ++i)
    {
        void* childObject = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_0044)[i]);

        // Slot 0x30 / 4 = 12: query whether this child wants the update.
        bool wantsUpdate = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, childObject, 0)[12])(childObject, nullptr);
        if (wantsUpdate)
        {
            // Slot 0x10 / 4 = 4: forward a2 to the child.
            ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, childObject, 0)[4])(childObject, nullptr, a2);
        }

        result = static_cast<int>(self->field_004c);
    }
    return result;
}

HOOK(0x006FC600, C_00862e88_UpdateChildrenWithDeltaTime, "auto");
