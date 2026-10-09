#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// FindRecordIndexById: bound by address per the callee packet.
CSF_FUNC(int, __fastcall, FindRecordIndexById, 0x00485980, (void* self, void* edx, int a2));

// Fallback resolver called when neither the virtual slot nor the record lookup finds an index.
CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveFallbackValue, 0x00483210, (void* self, void* edx, int a2));

// Looks up a per-action value keyed by id. First asks the virtual slot C0 for an index into
// field_0018; if that yields -1, searches by id and reads from field_0078; if that also fails,
// falls back to the secondary resolver.
static int __fastcall CCharActionFsm_LookupActionValue(CCharActionFsm* self, void* /*edx*/, int a2)
{
    int slotIndex = self->vFindProviderIndex(a2);
    if (slotIndex != -1) {
        return *reinterpret_cast<int*>(self->field_0018 + 4 * slotIndex);
    }

    int recordIndex = FindRecordIndexById(self, nullptr, a2);
    if (recordIndex == -1) {
        return CCharActionFsm_ResolveFallbackValue(self, nullptr, a2);
    }
    return *reinterpret_cast<int*>(self->field_0078 + 4 * recordIndex);
}

HOOK(0x004872D0, CCharActionFsm_LookupActionValue, "auto");
