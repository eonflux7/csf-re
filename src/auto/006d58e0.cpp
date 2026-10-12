#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxTextureLoader_FindEntryByName, 0x006D5560, (void* self, void* edx, char* String2, int a3));

// Looks up a texture/model entry by name, then invokes virtual slot 4 (+0x10) on the
// loader with the entry's record address (base + index * 60). Returns the found index or -1.
static int __fastcall GfxTextureLoader_FindModelIndex(S_006d4450* self, void* /*edx*/, char* String2, int /*unusedA3*/)
{
    int entryIndex = GfxTextureLoader_FindEntryByName(self, nullptr, String2, -1);
    if (entryIndex != -1)
    {
        uint32_t entryAddress = self->field_0008 + 60u * (uint32_t)entryIndex;
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, self, 0)[4])(self, nullptr, entryAddress);
        return entryIndex;
    }
    return entryIndex;
}

HOOK(0x006D58E0, GfxTextureLoader_FindModelIndex, "auto");
