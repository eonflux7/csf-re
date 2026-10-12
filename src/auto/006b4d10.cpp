#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, GfxObject_UpdateTransform, 0x006B2D60, (void* self, void* edx, float a2));
CSF_FUNC(uint32_t*, __fastcall, GfxObject_UpdateAtomics, 0x006B4720, (void* self, void* edx, float arg0));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(void, __fastcall, GfxObject_UpdateAttachment, 0x006E50C0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, GfxObject_UpdateChild, 0x006E5720, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, C_00861e98_UpdateAnimation, 0x006EB410, (C_00861e98* self, void* , float deltaTime, uint32_t* rootMotionOutput, int intervalBits));

static char __fastcall C_00860288_UpdateClump(
    C_00860288* self, void* /*edx*/, float deltaTime)
{
    const uint32_t flags = self->field_0064;
    if ((flags & 0x100) == 0)
        return static_cast<char>(~static_cast<uint8_t>(flags >> 8));

    auto* animation = reinterpret_cast<C_00861e98*>(self->field_0028);
    if (animation)
    {
        C_00861e98_UpdateAnimation(
            animation, nullptr, deltaTime, &self->field_0030,
            static_cast<int>(self->field_0078));
    }

    void* controller = reinterpret_cast<void*>(self->field_002c);
    if (controller)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x28 / 4])(
                controller, nullptr);
    }

    if ((self->field_0064 & 0x10000) != 0)
        GfxObject_UpdateTransform(self, nullptr, deltaTime);

    void* resource = reinterpret_cast<void*>(self->field_0068);
    auto* entries = reinterpret_cast<C_0085e7b8* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, resource, 0)[0x1C / 4])(resource, nullptr);

    auto* payload = reinterpret_cast<uint32_t*>(
        C_0085e7b8_FindEntryPayloadByKey(entries, nullptr, 5));
    if (payload)
    {
        void* attachment = reinterpret_cast<void*>(*payload);
        if (attachment)
            GfxObject_UpdateAttachment(attachment, nullptr, deltaTime);
    }

    GfxObject_UpdateAtomics(self, nullptr, deltaTime);

    for (unsigned int index = 0; index < self->field_001c; ++index)
    {
        auto* children = reinterpret_cast<uint32_t*>(self->field_0018);
        GfxObject_UpdateChild(
            reinterpret_cast<void*>(children[index]), nullptr, deltaTime);
    }

    const uint32_t updatedFlags = self->field_0064 | 0x20000;
    self->field_0064 = updatedFlags;
    return static_cast<char>(updatedFlags);
}

HOOK(0x006B4D10, C_00860288_UpdateClump, "auto");
