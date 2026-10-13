#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_Destroy, 0x00719B10, (CGuiControl* self, void*));

static int __fastcall CGuiTacticalMap_Destroy(CGuiTacticalMap* self, void* /*edx*/)
{
    const uint32_t spriteCount = self->field_0168;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00830A48);

    for (uint32_t index = 0; index < spriteCount; ++index)
    {
        const uint32_t sprite = reinterpret_cast<uint32_t*>(self->m_sprites)[index];
        if (sprite)
        {
            ReleaseObjectReference(reinterpret_cast<uint8_t*>(sprite) + 4, nullptr);
            reinterpret_cast<uint32_t*>(self->m_sprites)[index] = 0;
        }
    }

    const uint32_t medicSprite = self->m_medicSprite;
    if (medicSprite)
    {
        ReleaseObjectReference(reinterpret_cast<uint8_t*>(medicSprite) + 4, nullptr);
        self->m_medicSprite = 0;
    }

    const uint32_t auxiliaryObject = self->field_0158;
    if (auxiliaryObject)
    {
        ReleaseObjectReference(reinterpret_cast<uint8_t*>(auxiliaryObject) + 4, nullptr);
        self->field_0158 = 0;
    }

    void* mapTexture = reinterpret_cast<void*>(self->m_mapTexture);
    if (mapTexture)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, mapTexture, 0)[0])(mapTexture, nullptr, 1);
        self->m_mapTexture = 0;
    }

    void* sprites = reinterpret_cast<void*>(self->m_sprites);
    if (sprites)
    {
        Mem_Free(sprites);
        self->m_sprites = 0;
    }

    return CGuiControl_Destroy(self, nullptr);
}

HOOK(0x00436F40, CGuiTacticalMap_Destroy, "auto");
