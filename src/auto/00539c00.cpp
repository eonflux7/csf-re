#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00539A80: base constructor that initializes the CMenuLoadGame / CMenuScreen
// portion of the object and forwards the construction argument.
CSF_FUNC(CMenuLoadGame*, __fastcall, CMenuLoadGame_Construct, 0x539A80,
         (CMenuLoadGame* self, void* /*edx*/, int a2));

// CMenuLoadGameInGame constructor (0x00539C00):
// runs the base constructor, then installs the derived class vtable at +0 and
// the secondary base vtable at +4, finally returning the object pointer.
static CMenuLoadGameInGame* __fastcall CMenuLoadGameInGame_Construct(
    CMenuLoadGameInGame* self, void* /*edx*/, int a2)
{
  CMenuLoadGame_Construct(self, nullptr, a2);
  CSF_AT(void*, self, 0) = reinterpret_cast<void*>(CSF_ADDR_VT(0x00842328));
  self->field_0004 = CSF_ADDR_VT(0x008422D8);
  return self;
}

HOOK(0x539C00, CMenuLoadGameInGame_Construct, "auto");
