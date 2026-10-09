#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, C_00833ee8_FindCandidateEntity, 0x00496E00, (void* self, void* edx, unsigned int* arg0));
CSF_FUNC(int, __fastcall, C_00833908_QueryPartInfo, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

// Looks for a candidate target; if it is acceptable to our filter object (+0x4C),
// arms the pending action (type 7) with the target's id pair, the part index and the "G006" id.
// Returns whatever the last step left: the already pending action type, null when no candidate,
// the filter's result when rejected, or the parsed description id on success.
static uint32_t __fastcall C_00833ee8_TrySetPendingActionG006(C_00833ee8* self, void* /*edx*/)
{
    uint32_t pendingType = self->field_0298;
    if (pendingType != 0)
        return pendingType;

    unsigned int partIndex = 0;
    C_00833908* target = static_cast<C_00833908*>(C_00833ee8_FindCandidateEntity(self, nullptr, &partIndex));
    if (!target)
        return 0;

    if (target->vGetField016c() != 0)
    {
        uint32_t partInfo[3] = { 0, 0, 0 };
        C_00833908_QueryPartInfo(target, nullptr, static_cast<int>(partIndex), partInfo);

        void* model = reinterpret_cast<void*>(target->vGetField016c());
        int partId = ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, model, 0)[0x98 / 4])(model, nullptr, partInfo[1]);

        model = reinterpret_cast<void*>(target->vGetField016c());
        void* part = ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, model, 0)[0x8C / 4])(model, nullptr, partId);

        void* partOwner = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, part, 0)[0x18 / 4])(part, nullptr);

        void** filterVtbl = CSF_AT(void**, reinterpret_cast<void*>(self->field_004c), 0);

        int resolved = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, partOwner, 0)[0x10 / 4])(partOwner, nullptr, target);

        uint32_t filterResult = ((uint32_t(__fastcall*)(void*, void*, int))filterVtbl[0x40 / 4])(
            reinterpret_cast<void*>(self->field_004c), nullptr, resolved);
        if (static_cast<uint8_t>(filterResult) == 0)
            return filterResult;
    }

    self->field_0298 = 7;
    self->field_02a0 = target->field_0020;
    self->field_02a4 = target->field_0024;
    self->field_02a8 = partIndex;
    uint32_t descriptionId = static_cast<uint32_t>(MenuExtras_ParseDescriptionId(CSF_STR(0x833E24, "G006")));
    self->field_02ac = descriptionId;
    return descriptionId;
}

HOOK(0x004973E0, C_00833ee8_TrySetPendingActionG006, "auto");
