#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, ForwardToDelegate, 0x0048E710, (void* self, void* edx, CPlayerCharacter* character));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CPlayerCharacter_EvaluatePrimaryTarget(
    CPlayerCharacter* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0298);
    if (result != 0)
        return result;

    result = static_cast<int>(self->field_0248);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x240));
    if (result == 0)
        return result;

    auto* target = reinterpret_cast<C_00833908*>(result);
    if (target->field_0050 != 26)
        return result;

    result = ForwardToDelegate(target, nullptr, self);
    if (result == 0)
        return result;

    self->field_0298 = 10;
    self->field_02a0 = CSF_AT(uint32_t, self, 0x244);
    self->field_02a4 = self->field_0248;

    void* targetProperties = reinterpret_cast<void*>(target->field_0034);
    uint32_t flags = CSF_AT(uint32_t, targetProperties, 0x44);
    result = static_cast<int>(flags);
    if ((flags & 0x40) != 0 || (flags & 0x80) != 0)
        return result;

    if ((flags & 0xC2) != 0)
    {
        result = MenuExtras_ParseDescriptionId(CSF_STR(0x00833DB0, "G092"));
    }
    else
    {
        alignas(8) uint8_t boundsBytes[0x44];
        auto* boundsProvider =
            reinterpret_cast<C_00833908*>(target->vGetField016c());
        int bounds = boundsProvider->vSlot2C(reinterpret_cast<int>(boundsBytes));
        float doubledHeight =
            CSF_AT(float, reinterpret_cast<void*>(bounds), 0x10) +
            CSF_AT(float, reinterpret_cast<void*>(bounds), 0x10);

        int settings = GetGlobalValue_8BC1E0();
        int position = GetObjectPositionVector(target, nullptr);
        float targetHeight =
            doubledHeight *
            CSF_AT(float, reinterpret_cast<void*>(settings), 0x2280) *
            g_ProgressScale +
            CSF_AT(float, reinterpret_cast<void*>(position), 0x4);

        if (targetHeight > self->field_0260)
        {
            self->field_0298 = 11;
            result = MenuExtras_ParseDescriptionId(CSF_STR(0x00833DC8, "G088"));
        }
        else if ((CSF_AT(uint32_t, targetProperties, 0x44) & 4) != 0)
        {
            result = MenuExtras_ParseDescriptionId(CSF_STR(0x00833DC0, "G152"));
        }
        else
        {
            result = MenuExtras_ParseDescriptionId(CSF_STR(0x00833DB8, "G009"));
        }
    }

    self->field_02ac = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x004940A0, CPlayerCharacter_EvaluatePrimaryTarget, "auto");
