#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6D69E0, 0x006D69E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, sub_81CA70, 0x0081CA70, (int a2));

// Packet-listed vtable reference (S_006d69e0's vtable). The original does not store it here,
// so this binding only keeps the address referenced; it has no runtime effect.
[[maybe_unused]] static void* const kS006d69e0Vtable = CSF_VTBL(0x006D69E0);

// Resolves the object referenced by `a2` against this S_006d69e0 instance. If the first lookup
// fails (returns 0), falls back to the global handler with the referenced object again.
static int __fastcall S_006d69e0_TryResolveReferencedObject(S_006d69e0* self, void* /*edx*/, C_0085ec18* a2)
{
    void* referencedObject = GetReferencedObject(a2, nullptr);
    int result = sub_6D69E0(self, nullptr, reinterpret_cast<int>(referencedObject));
    if (result != 0)
    {
        return result;
    }

    void* fallbackObject = GetReferencedObject(a2, nullptr);
    return sub_81CA70(reinterpret_cast<int>(fallbackObject));
}

HOOK(0x006D6E20, S_006d69e0_TryResolveReferencedObject, "auto");
