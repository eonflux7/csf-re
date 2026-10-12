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

CSF_FUNC(int, __fastcall, SceneObjList_FindIndex, 0x00458930, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneObjList_AppendRecordAt, 0x00458FE0, (void* self, void* edx, uint32_t* a2, int a3));

// Returns true when the scene object was found in the list and its record was appended.
static bool __fastcall HudStatusIcons_TryRegisterSceneObject(S_00458930* self, void* /*edx*/, int a2)
{
    (void)CSF_VTBL(0x00458930);

    uint32_t listBegin = self->field_002c;
    if (!listBegin)
        return false;

    // Signed count of 20-byte slots between the list begin and end.
    int slotCount = (int)(self->field_0030 - listBegin) / 20;
    if (!slotCount)
        return false;

    int index = SceneObjList_FindIndex(self, nullptr, a2);
    if (index == -1)
        return false;

    // Each record is 40 bytes, starting at field_000c.
    int entry = (int)(self->field_000c + 40u * static_cast<uint32_t>(index));

    SceneObjList_AppendRecordAt(reinterpret_cast<uint8_t*>(self) + 8, nullptr,
                                reinterpret_cast<uint32_t*>(&a2), entry);
    return true;
}

HOOK(0x004590D0, HudStatusIcons_TryRegisterSceneObject, "auto");
