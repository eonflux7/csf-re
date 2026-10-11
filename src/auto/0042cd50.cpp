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

CSF_FUNC(uint32_t*, __fastcall, sub_4286E0, 0x004286E0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, sub_42CA40, 0x0042CA40, (void* self, void* edx));

static char __fastcall CheatsMenu_ApplyValueAndRefresh(void* self, void* /*edx*/, int a2, int /*a3*/)
{
    // ecx = [this + 0x10] is the object passed as `this` to sub_4286E0; the value 3 and a2 are its stack args.
    void* fieldObject = CSF_AT(void*, self, 0x10);
    sub_4286E0(fieldObject, nullptr, 3, a2);

    // lea ecx, [this - 4]: refresh runs on the enclosing object, four bytes before this.
    return sub_42CA40(reinterpret_cast<C_00830578*>(reinterpret_cast<uint8_t*>(self) - 4), nullptr);
}

HOOK(0x0042CD50, CheatsMenu_ApplyValueAndRefresh, "auto");
