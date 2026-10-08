#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x005396D0
// Resolves the child control named "BotonLoad" through the virtual lookup at vtable byte
// offset 0xFC of the object stored in CMenuLoadGame::field_0014, caches the returned
// pointer in CMenuLoadGame::field_0104 and returns it.
static int __fastcall CMenuLoadGame_InitBotonLoad(CMenuLoadGame* self, void* /*edx*/)
{
    typedef int (__fastcall* FindChildByNameFn)(void* self, void* edx, const char* name);

    void* pChildHost = reinterpret_cast<void*>(self->field_0014);
    FindChildByNameFn findChild =
        reinterpret_cast<FindChildByNameFn>(CSF_AT(void**, pChildHost, 0)[63]);
    int result = findChild(pChildHost, nullptr, CSF_STR(0x00841718, "BotonLoad"));
    self->field_0104 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x005396D0, CMenuLoadGame_InitBotonLoad, "auto");
