#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

// Returns the cached bone name "Hueso <index>" for this object, creating it on first use.
static int __fastcall C_00861dd8_GetOrCreateBoneName(C_00861dd8* self, void* /*edx*/)
{
    if (self->field_0008 == 0) {
        char* nameBuffer = reinterpret_cast<char*>(
            CoreMemory_AllocUntagged(std::strlen(CSF_STR(0x00861DD0, "Hueso ")) + 5));
        int boneIndex = static_cast<int>(self->field_0004);
        self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(nameBuffer));
        _sprintf(nameBuffer, CSF_STR(0x00861DC8, "%s%d"), CSF_STR(0x00861DD0, "Hueso "), boneIndex);
    }
    return static_cast<int>(self->field_0008);
}

HOOK(0x6E7E10, C_00861dd8_GetOrCreateBoneName, "auto");
