#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));

// Starts the weapon action of the linked character task for the alert state.
static int __stdcall AiAlert_StartWeaponAction(int a1, int a2)
{
    (void)a2;
    void* linkedSelf = reinterpret_cast<void*>(a1);

    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedSelf, nullptr);
    int targetObjectAddr = linkedValue ? linkedValue - 4 : 0;

    int ownerLinkedValue = GetLinkedObjectValue_4DB5C0(linkedSelf, nullptr);
    int ownerObjectAddr = ownerLinkedValue ? ownerLinkedValue - 4 : 0;

    uint8_t* ownerObject = reinterpret_cast<uint8_t*>(ownerObjectAddr);
    uint32_t characterData = CSF_AT(uint32_t, ownerObject, 0x2C);
    uint32_t characterTask = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(characterData), 0x204);

    return TaskCharAction_48CFE0(reinterpret_cast<void*>(characterTask), nullptr,
                                 targetObjectAddr, static_cast<char>(1), static_cast<char>(1));
}

HOOK(0x004AB990, AiAlert_StartWeaponAction, "auto");
