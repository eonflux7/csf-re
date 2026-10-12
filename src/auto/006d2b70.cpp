#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Embedded sub-object at CCamera+0x10: vtable slot 41 (+0xA4) fills a 12-byte
// buffer from the projection type; vtable slot 8 (+0x20) applies it.
typedef void(__fastcall* ProjectionBuildFn)(void* self, void* edx, uint8_t* buffer, int projectionType);
typedef int(__fastcall* ProjectionApplyFn)(void* self, void* edx, uint8_t* buffer);

static int __fastcall CCamera_ApplyProjectionType(CCamera* self, void* /*edx*/, int projectionType)
{
    void* subObject = &self->field_0010;

    alignas(8) uint8_t projectionBuffer[12];

    ProjectionBuildFn buildProjection =
        reinterpret_cast<ProjectionBuildFn>(CSF_AT(void**, subObject, 0)[0xA4 / 4]);
    buildProjection(subObject, nullptr, projectionBuffer, projectionType);

    ProjectionApplyFn applyProjection =
        reinterpret_cast<ProjectionApplyFn>(CSF_AT(void**, subObject, 0)[0x20 / 4]);
    return applyProjection(subObject, nullptr, projectionBuffer);
}

HOOK(0x006D2B70, CCamera_ApplyProjectionType, "auto");
