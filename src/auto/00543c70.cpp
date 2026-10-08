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

// 0x00543C70
// Resolves the child control named "BotonExit" through the virtual lookup at vtable byte
// offset 0xFC of the object stored in CMenuMain::field_0018, then caches the returned
// pointer in CMenuMain::field_0014 and returns it.
static int __fastcall CMenuMainPC_InitBotonExit(CMenuMainPC* self, void* /*edx*/)
{
    typedef int (__fastcall* FindChildByNameFn)(void* self, void* edx, const char* name);

    void* pChildHost = reinterpret_cast<void*>(self->field_0018);
    FindChildByNameFn findChild =
        reinterpret_cast<FindChildByNameFn>(CSF_AT(void**, pChildHost, 0)[63]);
    int result = findChild(pChildHost, nullptr, CSF_STR(0x00841E0C, "BotonExit"));
    self->field_0014 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x00543C70, CMenuMainPC_InitBotonExit, "auto");
