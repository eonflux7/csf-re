#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, RwBBoxAddPoint, 0x0063C9B0, (float* a1, float* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_008605d8_FinalizeEnvironment(
    C_008605d8* self, void* /*edx*/)
{
    int result = static_cast<int32_t>(self->field_0184);
    if (!result)
        return result;

    result = static_cast<int32_t>(
        self->field_0188 - static_cast<uint32_t>(result)) / 24;
    if (!result)
        return result;

    result = static_cast<int32_t>(self->field_0004);
    if (!result)
        return result;

    void* node = CSF_AT(void*, reinterpret_cast<void*>(result), 0x1C);
    void* pendingNodes[64];
    pendingNodes[0] = nullptr;
    int depth = 0;
    uint32_t boundsOffset = 0;

    do
    {
        if (CSF_AT(int32_t, node, 0) >= 0)
        {
            void* rightChild = CSF_AT(void*, node, 0x0C);
            node = CSF_AT(void*, node, 0x08);
            pendingNodes[++depth] = rightChild;
        }
        else
        {
            void* source = reinterpret_cast<void*>(
                self->field_0184 + boundsOffset);

            CSF_AT(uint32_t, node, 0x48) = CSF_AT(uint32_t, source, 0x00);
            CSF_AT(uint32_t, node, 0x4C) = CSF_AT(uint32_t, source, 0x04);
            CSF_AT(uint32_t, node, 0x50) = CSF_AT(uint32_t, source, 0x08);
            CSF_AT(uint32_t, node, 0x54) = CSF_AT(uint32_t, source, 0x0C);
            CSF_AT(uint32_t, node, 0x58) = CSF_AT(uint32_t, source, 0x10);
            CSF_AT(uint32_t, node, 0x5C) = CSF_AT(uint32_t, source, 0x14);
            boundsOffset += 24;

            auto* nodeBytes = static_cast<uint8_t*>(node);
            auto* bounds = reinterpret_cast<float*>(nodeBytes + 0x48);
            RwBBoxAddPoint(bounds, reinterpret_cast<float*>(nodeBytes + 0x60));
            RwBBoxAddPoint(bounds, reinterpret_cast<float*>(nodeBytes + 0x6C));

            node = pendingNodes[depth];
            --depth;
        }
    } while (depth >= 0);

    if (self->field_0184)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0184));

    self->field_0184 = 0;
    self->field_0188 = 0;
    self->field_018c = 0;
    return 0;
}

HOOK(0x006BDB50, C_008605d8_FinalizeEnvironment, "auto");
