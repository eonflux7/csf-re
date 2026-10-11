#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, sub_761090, 0x00761090, (int a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Forwards a 16-byte (type, value) message record to the global state's
// message sink. Message 7 is used for the "sub 16 == 0" case, message 6 for "== 2".
static int __fastcall C_0083e5e8_HandleSceneMenuEvent(C_0083e5e8* self, void* /*edx*/, int a2)
{
    uint8_t* argBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    int result = static_cast<int>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (static_cast<uint32_t>(result) != self->field_000c)
        return result;

    result = sub_761090(CSF_AT(int, argBase, 0x0C));
    if (static_cast<uint8_t>(result) == 0)
        return result;

    result = GetGlobalState_50D190();
    uint8_t* globalState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
    int sink = CSF_AT(int, globalState, 0x74);
    if (sink == 0)
        return result;

    float amount = static_cast<float>(CSF_AT(int, argBase, 0x10));
    int kind = CSF_AT(int, argBase, 0x0C) - 16;

    uint32_t msg[4] = {0, 0, 0, 0};
    std::memcpy(&msg[3], &amount, sizeof(float));

    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(sink));
    using MessageFn = int(__fastcall*)(void*, void*, int, uint32_t*);

    if (kind == 0)
        return reinterpret_cast<MessageFn>(CSF_AT(void**, obj, 0)[1])(obj, nullptr, 7, msg);

    result = kind - 2;
    if (result == 0)
        return reinterpret_cast<MessageFn>(CSF_AT(void**, obj, 0)[1])(obj, nullptr, 6, msg);

    return result;
}

HOOK(0x004E1AC0, C_0083e5e8_HandleSceneMenuEvent, "auto");
