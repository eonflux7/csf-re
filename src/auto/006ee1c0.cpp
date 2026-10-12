#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int a1, int a2));
static CSF_FUNC(void, __fastcall, ProcessSectorObject, 0x006EDE50, (void* self, void* edx, int a2, int a3));

static unsigned int __fastcall C_00862028_ProcessSectorObjects(
    C_00862028* self, void* /*edx*/, int sector, int query)
{
    if (!sector)
        return 0;

    SceneLoader_CollectSectorObjects(
        sector, reinterpret_cast<int>(&self->field_0044));

    unsigned int count = self->field_004c;
    for (unsigned int index = 0; index < count; ++index)
    {
        const int object = reinterpret_cast<const int*>(self->field_0044)[index];
        ProcessSectorObject(self, nullptr, object, query);
        count = self->field_004c;
    }
    return count;
}

HOOK(0x006EE1C0, C_00862028_ProcessSectorObjects, "auto");
