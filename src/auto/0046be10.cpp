#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MissionScript_LookupSymbolId, 0x00769760, (void* self, void* edx, char* String1));

// Looks up a name in the primary context (this->field_0044->field_0028). If that is not found (-1),
// looks it up again in the secondary context (this->field_0048) and returns that id tagged with 0x800000.
static unsigned int __fastcall MissionScript_LookupSymbolOrFlag(void* self, void* /*edx*/, char* name)
{
    void* outerObject = CSF_AT(void*, self, 0x44);
    void* primaryContext = reinterpret_cast<void*>(CSF_AT(uint32_t, outerObject, 0x28));

    int result = MissionScript_LookupSymbolId(primaryContext, nullptr, name);
    if (result == -1)
    {
        void* secondaryContext = CSF_AT(void*, self, 0x48);
        int flaggedId = MissionScript_LookupSymbolId(secondaryContext, nullptr, name);
        return static_cast<unsigned int>(flaggedId) | 0x800000u;
    }
    return static_cast<unsigned int>(result);
}

HOOK(0x0046BE10, MissionScript_LookupSymbolOrFlag, "auto");
