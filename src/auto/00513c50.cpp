#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns a pointer to the save-slot data word of a CSaveGameManager.
// If the counter at +0xAC is below 0x10 (unsigned), the address of the inline
// field at +0x98 is returned; otherwise the pointer stored at +0x98 is returned.
static uint32_t* __fastcall CSaveGameManager_GetSaveBufferPtr(CSaveGameManager* self, void* /*edx*/)
{
  if (self->field_00ac < 0x10u)
    return &self->field_0098;
  return reinterpret_cast<uint32_t*>(self->field_0098);
}

HOOK(0x513C50, CSaveGameManager_GetSaveBufferPtr, "auto");
