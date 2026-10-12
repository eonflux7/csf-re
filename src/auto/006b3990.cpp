#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_InitializeHitInfo, 0x0040ED90, (uint8_t* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static bool __fastcall C_00860288_QueryHitInfo(
    C_00860288* self, void* /*edx*/, int query,
    uint32_t* position, uint32_t* normal, uint32_t* childId,
    char alternateMode)
{
    *childId = static_cast<uint32_t>(-1);
    if (!self->field_000c)
        return false;

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* controller =
        reinterpret_cast<void*>(SceneLoader_GetSceneController(global, nullptr));

    void* queryService =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x38 / 4]))(controller, nullptr);

    void* hit =
        (reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t, int, int)>(
            CSF_AT(void**, queryService, 0)[0x40 / 4]))(
                queryService, nullptr, self->field_000c, query,
                alternateMode ? 0x100 : 3);

    if (!hit)
        return false;

    alignas(8) uint8_t hitInfo[0x68];
    CharActionFsm_InitializeHitInfo(hitInfo, nullptr);
    (reinterpret_cast<void (__fastcall*)(void*, void*, uint8_t*)>(
        CSF_AT(void**, hit, 0)[0xC / 4]))(hit, nullptr, hitInfo);

    uint32_t positionX = CSF_AT(uint32_t, hitInfo, 0x04);
    uint32_t positionY = CSF_AT(uint32_t, hitInfo, 0x08);
    uint32_t hitObject = CSF_AT(uint32_t, hitInfo, 0x24);

    position[0] = positionX;
    position[1] = positionY;
    position[2] = CSF_AT(uint32_t, hitInfo, 0x0C);

    uint32_t normalX = CSF_AT(uint32_t, hitInfo, 0x10);
    uint32_t normalY = CSF_AT(uint32_t, hitInfo, 0x14);
    normal[0] = normalX;
    normal[1] = normalY;
    normal[2] = CSF_AT(uint32_t, hitInfo, 0x18);

    if (!hitObject)
        return true;

    uint32_t childCount = self->field_001c;
    if (!childCount)
        return true;

    void** children = reinterpret_cast<void**>(self->field_0018);
    for (uint32_t index = 0; index < childCount; ++index)
    {
        if (CSF_AT(uint32_t, children[index], 0x0C) != hitObject)
            continue;

        void* child = reinterpret_cast<void**>(self->field_0018)[index];
        *childId =
            (reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x10 / 4]))(child, nullptr);
        return true;
    }

    return true;
}

HOOK(0x006B3990, C_00860288_QueryHitInfo, "auto");
