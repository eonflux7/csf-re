#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);
CSF_FUNC(char*, __fastcall, MpSpawn_GetCharacterSelectStateRecord, 0x006B29A0, (void* self, void* edx));

// Returns true when the character-select record linked from the global state
// has a state byte (at +6) equal to 1, 2 or 3.
static bool MpSpawn_IsCharacterSelectStateActive()
{
    // The global holds a pointer; its +4 dword links to the record (record base = link - 776).
    uint32_t linkedRecordAddress = *reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(g_CharacterSelectState8B0694) + 4);
    bool isActive = false;
    if (linkedRecordAddress != 0)
    {
        C_00860288* recordObject = reinterpret_cast<C_00860288*>(
            static_cast<uintptr_t>(linkedRecordAddress - 776u));
        if (recordObject != nullptr)
        {
            char* stateRecord = MpSpawn_GetCharacterSelectStateRecord(recordObject, nullptr);
            uint8_t stateByte = static_cast<uint8_t>(stateRecord[6]);
            return stateByte == 1 || stateByte == 2 || stateByte == 3;
        }
    }
    return isActive;
}

HOOK(0x5515A0, MpSpawn_IsCharacterSelectStateActive, "auto");
