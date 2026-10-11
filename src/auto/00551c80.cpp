#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(bool, __fastcall, Character_748380, 0x00748380, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, ConsumeFlagByteAndClear, 0x007483D0, (void* self, void* edx));

static uint32_t SpawnPoints_ProcessCharacterSelectState()
{
    uint32_t state = g_CharacterSelectState8B0694;
    if (state == 0)
        return 0;

    uint32_t result = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(state), 4);
    if (result == 0)
        return 0;

    // The original subtracts 776 from the stored pointer to reach the owning
    // object; the vtable call receives the stored pointer itself as `this`.
    uint32_t objectAddress = result - 776;
    if (result == 776)
        return result;

    if (objectAddress != 0)
    {
        void* selectedObject = reinterpret_cast<void*>(result);
        bool needsReset = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, selectedObject, 0)[2])(selectedObject, nullptr);
        if (needsReset)
            Character_748380(reinterpret_cast<void*>(objectAddress), nullptr, 0);
    }

    return static_cast<uint32_t>(
        static_cast<uint8_t>(ConsumeFlagByteAndClear(reinterpret_cast<void*>(objectAddress), nullptr)));
}

HOOK(0x00551C80, SpawnPoints_ProcessCharacterSelectState, "auto");
