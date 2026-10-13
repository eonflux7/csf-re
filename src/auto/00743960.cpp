#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Stores a one-byte option flag in the global options byte and mirrors it into
// the currently attached net object's byte at +0xAC, if one exists.
static char __cdecl OptionsFile_ApplyFlagChange(char flagValue) {
    CSF_GLOBAL(char, g_OptionsFlagByte, 0x008A8926);
    CSF_GLOBAL(uint32_t, g_AttachedNetObject, 0x008BC140);

    char result = flagValue;
    g_OptionsFlagByte = flagValue;

    uint8_t* attachedObject = reinterpret_cast<uint8_t*>(g_AttachedNetObject);
    if (attachedObject) {
        CSF_AT(char, attachedObject, 0xAC) = flagValue;
    }
    return result;
}

HOOK(0x00743960, OptionsFile_ApplyFlagChange, "auto");
