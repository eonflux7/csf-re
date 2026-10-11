#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_445A10, 0x00445A10, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, sub_445EB0, 0x00445EB0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, sub_446C80, 0x00446C80, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, AudioSoundManager_GetSoundDescriptor, 0x0075A480, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static uint32_t* __fastcall S_00445eb0_SubmitSceneRequest(
    S_00445eb0* self, void* /*edx*/, int requestAddress)
{
    auto* request = reinterpret_cast<uint8_t*>(requestAddress);
    float duration = CSF_AT(float, request, 0x24);

    if (duration == g_fZero && CSF_AT(int, request, 0x04) != -1)
    {
        auto* context = reinterpret_cast<void*>(GetValue8B0554());
        void* soundManager = CSF_AT(void*, context, 0x1FD0);
        int soundIndex =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, soundManager, 0)[0x0C / 4])(
                    soundManager, nullptr, CSF_AT(int, request, 0x04));

        if (soundIndex != -1)
        {
            auto* descriptor = reinterpret_cast<void*>(
                AudioSoundManager_GetSoundDescriptor(soundManager, nullptr, soundIndex));
            duration = CSF_AT(float, descriptor, 0x1C);
        }
    }

    // The packet labels this direct-call address as a vtable reference too.
    (void)CSF_VTBL(0x00445EB0);
    int index = sub_445EB0(
        self, nullptr, reinterpret_cast<int>(request + 0x08),
        CSF_AT(int, request, 0x00));

    if (index != -1)
    {
        uint32_t byteOffset = static_cast<uint32_t>(index) * 40;
        auto* entry = reinterpret_cast<uint8_t*>(self->field_001c) + byteOffset;

        CSF_AT(uint32_t, entry, 0x00) = CSF_AT(uint32_t, request, 0x00);
        CSF_AT(uint32_t, entry, 0x04) = CSF_AT(uint32_t, request, 0x04);
        CSF_AT(uint32_t, entry, 0x0C) = CSF_AT(uint32_t, request, 0x0C);
        CSF_AT(uint32_t, entry, 0x10) = CSF_AT(uint32_t, request, 0x10);
        CSF_AT(uint32_t, entry, 0x14) = CSF_AT(uint32_t, request, 0x14);
        CSF_AT(uint32_t, entry, 0x18) = CSF_AT(uint32_t, request, 0x18);
        CSF_AT(uint32_t, entry, 0x1C) = CSF_AT(uint32_t, request, 0x1C);
        CSF_AT(uint32_t, entry, 0x20) = CSF_AT(uint32_t, request, 0x20);
        CSF_AT(uint32_t, entry, 0x24) = CSF_AT(uint32_t, request, 0x24);

        CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_001c) + byteOffset,
               0x24) = duration;
        return reinterpret_cast<uint32_t*>(self);
    }

    uint32_t count = self->field_0024;
    uint32_t freeIndex = 0;
    uint8_t* entry = nullptr;
    bool foundFreeEntry = false;

    if (count != 0)
    {
        entry = reinterpret_cast<uint8_t*>(self->field_001c);
        while (freeIndex < count)
        {
            if (CSF_AT(int, entry, 0x00) == -1)
            {
                foundFreeEntry = true;
                break;
            }
            ++freeIndex;
            entry += 40;
        }
    }

    if (!foundFreeEntry || freeIndex == UINT32_MAX)
    {
        sub_446C80(
            reinterpret_cast<uint8_t*>(self) + 0x1C, nullptr,
            reinterpret_cast<uint32_t*>(request), 0);

        uint32_t updatedCount = self->field_0024;
        auto* entries = reinterpret_cast<uint8_t*>(self->field_001c);
        CSF_AT(float, entries + (40 * updatedCount - 4), 0x00) = duration;
        return reinterpret_cast<uint32_t*>(updatedCount);
    }

    uint32_t byteOffset = 40 * freeIndex;
    sub_445A10(entry, nullptr, reinterpret_cast<uint32_t*>(request));
    auto* entries = reinterpret_cast<uint8_t*>(self->field_001c);
    CSF_AT(float, entries + byteOffset, 0x24) = duration;
    return reinterpret_cast<uint32_t*>(entries);
}

HOOK(0x00446F10, S_00445eb0_SubmitSceneRequest, "auto");
