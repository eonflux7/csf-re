#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_00863cd4_Initialize, 0x007114E0, (C_00863cd4* self, void*));
CSF_FUNC(bool, __fastcall, SoundBuffer_Create, 0x00710FF0, (uint8_t* self, void*, int* format, uint8_t* context));

static uint32_t* __fastcall C_00863ba8_CreateSoundEffect(C_00863ba8* self, void* /*edx*/, int a2)
{
    uint32_t* effect = nullptr;

    int rawAllocation = CoreMemory_AllocUntagged(12);
    if (rawAllocation != 0)
    {
        C_00863cd4* effectObject = reinterpret_cast<C_00863cd4*>(static_cast<uintptr_t>(rawAllocation));
        effect = C_00863cd4_Initialize(effectObject, nullptr);
    }
    else
    {
        effect = nullptr;
    }

    bool bufferCreated = SoundBuffer_Create(
        reinterpret_cast<uint8_t*>(effect),
        nullptr,
        reinterpret_cast<int*>(static_cast<uintptr_t>(a2)),
        reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004)));

    if (bufferCreated || effect == nullptr)
        return effect;

    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, effect, 0)[0])(effect, nullptr, 1);
    return nullptr;
}

HOOK(0x0070EE20, C_00863ba8_CreateSoundEffect, "auto");
