#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static unsigned int __fastcall AiDistracted_ResetAndDispatchCoverAttack(
    void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x6C0) = -1;
    CSF_AT(int, self, 0x6C4) = -1;

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    auto* dispatcher = CSF_AT(uint8_t*, manager, 0x1FD8);

    int linkedValue = GetLinkedObjectValue_4DB5C0(
        static_cast<uint8_t*>(self) + 8, nullptr);
    void* linkedObject = reinterpret_cast<void*>(
        linkedValue ? linkedValue - 4 : 0);
    auto* source = CSF_AT(uint8_t*, linkedObject, 0x2C) + 0x1C;

    alignas(8) uint8_t argumentBytes[0x1C];
    CSF_AT(uint32_t, argumentBytes, 0x00) = 0;
    CSF_AT(void**, argumentBytes, 0x04) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, argumentBytes, 0x08) = CSF_AT(uint32_t, source, 4);
    CSF_AT(uint32_t, argumentBytes, 0x0C) = CSF_AT(uint32_t, source, 8);
    CSF_AT(float, argumentBytes, 0x10) = g_ZeroVectorX;
    CSF_AT(float, argumentBytes, 0x14) = g_ZeroVectorY;
    CSF_AT(float, argumentBytes, 0x18) = g_ZeroVectorZ;

    return DispatchWithArgBlock(
        dispatcher + 0x44, nullptr,
        reinterpret_cast<uint32_t*>(argumentBytes));
}

HOOK(0x004B9BA0, AiDistracted_ResetAndDispatchCoverAttack, "auto");
