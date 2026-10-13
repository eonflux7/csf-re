#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, ObjectProps_ReadEntryFirstDword, 0x006B8430, (int entryIndex));

// Reads the first dword of an object-property entry. If it is nonzero,
// sets the byte flag at offset 0x27 of the target object, then returns the entry index unchanged.
static int __cdecl GfxWorld_FlagObjectIfPropEntryNonZero(int entryIndex, int targetObject)
{
    if (ObjectProps_ReadEntryFirstDword(entryIndex))
    {
        auto* object = reinterpret_cast<uint8_t*>(targetObject);
        CSF_AT(uint8_t, object, 0x27) = 1;
    }
    return entryIndex;
}

HOOK(0x006B6B50, GfxWorld_FlagObjectIfPropEntryNonZero, "auto");
