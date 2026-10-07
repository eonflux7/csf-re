#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"
#include "types/auto_classes.h"

// CGuiPanel helper: returns a pointer to the character buffer of the
// small-buffer-optimized string stored at this+0x15C (m_str15C).
// The capacity word lives at this+0x174: if it is below 16 (unsigned compare,
// jb) the characters are stored inline at this+0x160, otherwise the heap
// pointer kept at this+0x160 is followed.
static char* __fastcall CGuiPanel_GetStringDataPtr(CGuiPanel* self, void* /*edx*/)
{
  if (CSF_AT(uint32_t, self, 0x174) < 0x10u)
    return reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(self) + 0x160);
  return CSF_AT(char*, self, 0x160);
}

HOOK(0x72BD10, CGuiPanel_GetStringDataPtr, "auto");
