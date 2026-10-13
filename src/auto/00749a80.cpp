#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Copies a bounded wide string into a buffer, bound by address.
CSF_FUNC(void, __cdecl, CoreVariantValues_CopyWideStringBounded, 0x006956C0, (int destBase, uint16_t* srcStr, int maxCount));

// Copies a wide-character source string (up to 127 characters) into the
// descriptor-name buffer embedded at offset +0x10C of the object.
// __thiscall: ecx = this, one stack argument (source string), callee pops 4 bytes.
static int __fastcall MpSpawnPoints_SetDescriptorName(char* self, void* /*edx*/, int sourceString)
{
    char* descriptorNameBuffer = self + 0x10C;
    CoreVariantValues_CopyWideStringBounded(reinterpret_cast<int>(descriptorNameBuffer),
                                            reinterpret_cast<uint16_t*>(sourceString),
                                            127);
    return 0;
}

HOOK(0x00749A80, MpSpawnPoints_SetDescriptorName, "auto");
