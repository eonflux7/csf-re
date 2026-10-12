#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copies a 3-dword point (x, y, z) into the object, then stores the tag at +0x0C.
// Returns the third point dword (eax at exit in the original).
static int __fastcall MovilAPunto_AssignPointAndTag(void* self, void* /*edx*/, int tag, const int* point)
{
  CSF_AT(int, self, 0x0) = point[0];
  CSF_AT(int, self, 0x4) = point[1];
  int result = point[2];
  CSF_AT(int, self, 0x8) = result;
  CSF_AT(int, self, 0xC) = tag;
  return result;
}

HOOK(0x4A9F00, MovilAPunto_AssignPointAndTag, "auto");
