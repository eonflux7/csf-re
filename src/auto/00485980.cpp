#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Scans the pointer array [field_0078, field_007c) of a CCharActionFsm and returns the index of the
// first element whose GetCachedObjectProvider value equals objectId, or -1 when none matches.
static int __fastcall CCharActionFsm_FindRecordIndexById(CCharActionFsm* self, void* /*edx*/, int objectId)
{
    for (uint32_t i = 0; ; ++i)
    {
        // The original reloads the array start on every iteration.
        uint32_t arrayBegin = self->field_0078;
        if (arrayBegin == 0)
            break;

        int byteSpan = static_cast<int>(self->field_007c - arrayBegin);
        uint32_t elementCount = static_cast<uint32_t>(byteSpan >> 2);
        if (i >= elementCount)
            break;

        C_0085e7b8* element = reinterpret_cast<C_0085e7b8**>(arrayBegin)[i];
        void* provider = GetCachedObjectProvider(element, nullptr);
        if (reinterpret_cast<uintptr_t>(provider) == static_cast<uintptr_t>(objectId))
            return static_cast<int>(i);
    }
    return -1;
}

HOOK(0x00485980, CCharActionFsm_FindRecordIndexById, "auto");
