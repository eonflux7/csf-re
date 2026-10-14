// Asks the global value for its state object (vtable slot +0x70); with a state it tail-jumps to the state's vtable
// slot +0xC with the caller's stack arguments (0x00521F40), without one it returns 0 popping the 8 argument bytes.
#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static hook::Tail __fastcall SceneLoader_UpdateLoadingState(void* /*self*/, const uint32_t* /*args*/) {
    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    void* state = reinterpret_cast<void*>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, globalValue, 0)[0x70 / 4])(globalValue, nullptr));

    if (!state)
        return hook::tail_ret(0, 8);

    return hook::tail_jump(state, (*reinterpret_cast<void***>(state))[0xC / 4]);
}

HOOK_TAIL(0x00521F40, SceneLoader_UpdateLoadingState, "auto");
