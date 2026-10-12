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

// Default handler the original forwards to in every path (0x49DF50, __thiscall, ret 4).
CSF_FUNC(int, __fastcall, C_00834198_DefaultHandler, 0x0049DF50, (void* self, void* edx, int a2));

// Method of C_00834198 (__thiscall, ret 4).
// If the low byte of a2 is set, or the helper object stored at +0x18 is missing,
// or the helper's slot 2 (vtable +8) returns a nonzero low byte, delegate to the default handler.
// Otherwise return the helper's result.
static int __fastcall C_00834198_ResolveWithHelper(C_00834198* self, void* /*edx*/, int a2)
{
    if (static_cast<uint8_t>(a2) != 0)
        return C_00834198_DefaultHandler(self, nullptr, a2);

    void* helper = CSF_AT(void*, self, 0x18);
    if (helper == nullptr)
        return C_00834198_DefaultHandler(self, nullptr, a2);

    using HelperCheckFn = int(__fastcall*)(void* self, void* edx);
    HelperCheckFn checkFn = reinterpret_cast<HelperCheckFn>(CSF_AT(void**, helper, 0)[2]);
    int helperResult = checkFn(helper, nullptr);

    if (static_cast<uint8_t>(helperResult) != 0)
        return C_00834198_DefaultHandler(self, nullptr, a2);

    return helperResult;
}

HOOK(0x0049E190, C_00834198_ResolveWithHelper, "auto");
