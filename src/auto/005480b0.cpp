#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Helper that resolves a companion object from the menu subobject (__thiscall, ecx = self, no stack args).
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x6683F0, (void* self, void* edx));

// Finalizes the menu state when the input-disabled flag is set (__thiscall on CMenuExtras, ecx = self - 4, no stack args).
CSF_FUNC(int, __fastcall, CMenuExtras_ResetActiveExtra, 0x5477E0, (void* self, void* edx));

// Selects the thumbnail matching a control index (__thiscall on CMenuExtras, one stack arg, ret 4).
CSF_FUNC(void, __fastcall, CMenuExtras_SelectThumbnailFromControl, 0x5464E0, (void* self, void* edx, int control));

// Handles a command message sent to the menu subobject (__thiscall, ret 8 -> two stack args).
static void __fastcall CMenuExtras_HandleCommandMessage(void* self, void* /*edx*/, int command, int argument)
{
    if (CSF_AT(uint8_t, self, 0x43))
    {
        CMenuExtras_ResetActiveExtra(reinterpret_cast<uint8_t*>(self) - 4, nullptr);
        return;
    }

    switch (command)
    {
    case 1:
        CSF_AT(uint8_t, self, 0x42) = 1;
        break;

    case 50:
    {
        void* argumentObject = reinterpret_cast<void*>(static_cast<uintptr_t>(argument));
        if (CSF_AT(uint8_t, argumentObject, 0x38))
        {
            void* companion = GetReferencedObject(self, nullptr);
            void** companionVtbl = CSF_AT(void**, companion, 0);
            // Virtual slot at byte offset 0x1C.
            reinterpret_cast<void (__fastcall*)(void*, void*)>(companionVtbl[0x1C / 4])(companion, nullptr);
        }
        break;
    }

    case 200:
    case 203:
    case 205:
    case 208:
    {
        void* owner = CSF_AT(void*, self, 0x14);
        void** ownerVtbl = CSF_AT(void**, owner, 0);
        // Virtual slot at byte offset 0x104 returns the selected control index.
        int controlIndex = reinterpret_cast<int (__fastcall*)(void*, void*)>(ownerVtbl[0x104 / 4])(owner, nullptr);
        CMenuExtras_SelectThumbnailFromControl(reinterpret_cast<uint8_t*>(self) - 4, nullptr, controlIndex);
        break;
    }

    default:
        return;
    }
}

HOOK(0x5480B0, CMenuExtras_HandleCommandMessage, "auto");
