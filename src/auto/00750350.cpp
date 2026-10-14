#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int index));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** output, uint8_t* key));
CSF_FUNC(int, __fastcall, NetPlayerDdl_ApplyProperty, 0x0066CD80, (void* self, void* edx, int object, int field, int id, int update, void* context));
// 0064AF90 is just ret 8: preserve the preceding call's entire eax.
CSF_FUNC(void, __fastcall, FinalizeSpawnUpdate, 0x0064AF90, (void* self, void* edx, void* context, int update));

static int __fastcall NetPostmanDdl_HandleMessage(char* self, void*, void* context, int update)
{
    // [ebp-8] is an output, not initialized by the original.
    void* firstProperty;
    uint8_t firstKey[4];
    firstKey[0] = 1;
    auto* object = reinterpret_cast<char*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, &firstProperty, firstKey);
    const auto savedContext = context;
    const int savedUpdate = update;
    NetPlayerDdl_ApplyProperty(CSF_AT(void*, firstProperty, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x44), 1, savedUpdate, savedContext);

    // Reuse the high byte of the first stack argument and the second argument's output slot.
    reinterpret_cast<uint8_t*>(&context)[3] = 2;
    object = reinterpret_cast<char*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, reinterpret_cast<void**>(&update),
        reinterpret_cast<uint8_t*>(&context) + 3);
    NetPlayerDdl_ApplyProperty(CSF_AT(void*, update, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x50), 2, savedUpdate, savedContext);

    // Reuse the high byte of the first stack argument and the second argument's output slot.
    reinterpret_cast<uint8_t*>(&context)[3] = 3;
    object = reinterpret_cast<char*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, reinterpret_cast<void**>(&update),
        reinterpret_cast<uint8_t*>(&context) + 3);
    NetPlayerDdl_ApplyProperty(CSF_AT(void*, update, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x5C), 3, savedUpdate, savedContext);

    // Reuse the high byte of the first stack argument and the second argument's output slot.
    reinterpret_cast<uint8_t*>(&context)[3] = 4;
    object = reinterpret_cast<char*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, reinterpret_cast<void**>(&update),
        reinterpret_cast<uint8_t*>(&context) + 3);
    NetPlayerDdl_ApplyProperty(CSF_AT(void*, update, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x6C), 4, savedUpdate, savedContext);

    // Reuse the high byte of the first stack argument and the second argument's output slot.
    reinterpret_cast<uint8_t*>(&context)[3] = 5;
    object = reinterpret_cast<char*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, reinterpret_cast<void**>(&update),
        reinterpret_cast<uint8_t*>(&context) + 3);
    NetPlayerDdl_ApplyProperty(CSF_AT(void*, update, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x78), 5, savedUpdate, savedContext);

    // Reuse the high byte of the first stack argument and the second argument's output slot.
    reinterpret_cast<uint8_t*>(&context)[3] = 6;
    object = reinterpret_cast<char*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, reinterpret_cast<void**>(&update),
        reinterpret_cast<uint8_t*>(&context) + 3);
    NetPlayerDdl_ApplyProperty(CSF_AT(void*, update, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0x88), 6, savedUpdate, savedContext);

    // Reuse the high byte of the first stack argument and the second argument's output slot.
    reinterpret_cast<uint8_t*>(&context)[3] = 7;
    object = reinterpret_cast<char*>(LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    NetCogibleDdl_FindProperty(object + 0xC, nullptr, reinterpret_cast<void**>(&update),
        reinterpret_cast<uint8_t*>(&context) + 3);
    const int result = NetPlayerDdl_ApplyProperty(CSF_AT(void*, update, 0x10), nullptr,
        reinterpret_cast<int>(self), reinterpret_cast<int>(self + 0xA0), 7, savedUpdate, savedContext);

    FinalizeSpawnUpdate(self, nullptr, savedContext, savedUpdate);
    return result;
}

HOOK(0x00750350, NetPostmanDdl_HandleMessage, "auto");
