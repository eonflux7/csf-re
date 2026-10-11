#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TCharTask_UpdateSubObject, 0x0048C680, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));

static int __fastcall TCharTask_UpdateWithSubObject(TCharTask* self, void* /*edx*/, int argumentBits)
{
    float argument;
    std::memcpy(&argument, &argumentBits, sizeof(argument));

    TCharTask_UpdateSubObject(
        reinterpret_cast<std::uint8_t*>(self) + 0x2B8, nullptr, argument);
    return TCharTask_BaseUpdate(self, nullptr, argumentBits);
}

HOOK(0x0047B1B0, TCharTask_UpdateWithSubObject, "auto");
