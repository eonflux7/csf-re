#include <cstdint>
#include "globals.h"
#include "hook.h"

// CDummyMesh constructor: installs the CDummyMesh vtable, clears the three
// DUMMIES list pointers (begin, end, capacity) and returns the object address.
// The class type is not resolvable in this translation unit, so the fields are
// accessed by their original offsets through CSF_AT.
static uint32_t* __fastcall CDummyMesh_Construct(void* self, void* /*edx*/) {
    CSF_AT(void*, self, 0x0) = reinterpret_cast<void*>(CSF_VTBL(0x0086F15C));
    CSF_AT(uint32_t, self, 0x008) = 0;  // m_dummiesBegin
    CSF_AT(uint32_t, self, 0x00C) = 0;  // m_dummiesEnd
    CSF_AT(uint32_t, self, 0x010) = 0;  // m_dummiesCapacity
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00768E10, CDummyMesh_Construct, "auto");
