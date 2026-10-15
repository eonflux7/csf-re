#include <cstdint>
#include "globals.h"
#include "hook.h"
template<class R, class... A> static __forceinline R call(void* object, uint32_t slot, A... args) {
    return reinterpret_cast<R(__fastcall*)(void*, void*, A...)>(CSF_AT(void**, object, 0)[slot / 4])(object, nullptr, args...);
}
static uint8_t __cdecl MissionScript_ProcessDirectValue(void* value, void* state, uint16_t* end) {
    // The push edi at 0x691419 saves a register, not a virtual-call argument.
    alignas(4) uint8_t record[16];
    record[1] = value == nullptr;
    if (!value) {
        if (CSF_AT(uint8_t, state, 0) ||
            call<uint8_t>(CSF_AT(void*, state, 4), 0x88, record, state)) {
            CSF_AT(uint8_t, state, 0) = 0;
            return 0;
        }
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }
    int kind = call<int>(value, 0x10);
    int index = call<int>(value, 0x14);
    *reinterpret_cast<int*>(record + 4) = index;
    *reinterpret_cast<int*>(record + 8) = kind;
    *reinterpret_cast<uint16_t*>(record + 12) = *end - index;
    *reinterpret_cast<uint16_t*>(record + 14) = reinterpret_cast<uint32_t>(value) - index;
    record[0] = call<uint8_t>(CSF_AT(void*, state, 0xC), 4, index) ||
        kind == 1 || (kind == 0 && !CSF_AT(uint8_t, state, 0x15));
    bool active = !CSF_AT(uint8_t, state, 0) &&
        !call<uint8_t>(CSF_AT(void*, state, 4), 0x88, record, state);
    CSF_AT(uint8_t, state, 0) = active;
    call<void>(CSF_AT(void*, state, 0xC), 0xC, *reinterpret_cast<int*>(record + 4));
    if (record[0]) return record[0];
    call<void>(CSF_AT(void*, state, 0xC), 8, *reinterpret_cast<int*>(record + 4));
    call<void>(value, 0xC, state);
    return call<uint8_t>(value, 8, state);
}
HOOK(0x006913D0, MissionScript_ProcessDirectValue, "auto");
