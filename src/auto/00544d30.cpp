#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// std::string assign helper (sub_4109E0), __thiscall, 3 stack args (ret 12).
CSF_FUNC(void*, __fastcall, StringBaseAssign, 0x4109E0, (void* self, void* edx, void* source, int offset, unsigned int count));

// CRT strncpy (0x7AFEC0), cdecl.
CSF_FUNC(char*, __cdecl, CrtStrncpy, 0x7AFEC0, (char* dest, const char* src, size_t count));

// Resets a save-slot string entry to empty, assigns the name from a2 (full string),
// and stores the first 4 characters of Source as the slot tag.
static void* __fastcall MenuSaveGame_InitEntryFromString(void* self, void* /*edx*/, void* a2, const char* Source)
{
    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x04) = 0;

    StringBaseAssign(self, nullptr, a2, 0, 0xFFFFFFFFu);

    CrtStrncpy(reinterpret_cast<char*>(static_cast<uint8_t*>(self) + 0x1C), Source, 4u);

    CSF_AT(uint8_t, self, 0x20) = 0;
    return self;
}

HOOK(0x00544D30, MenuSaveGame_InitEntryFromString, "auto");
