#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callee bound by address (bind as given in the packet).
CSF_FUNC(std::uintptr_t, __fastcall, MenuManager_TransitionToGameOrIntro, 0x0050CE60, (float* self, void* , int));

// Returns the C-string stored in a std::string-like member: inline buffer when the
// capacity field is below 16, otherwise the heap pointer kept at the same slot.
static const char* BddLoader_GetLoadedNameCString(uint8_t* info) {
    std::uint32_t capacity = CSF_AT(std::uint32_t, info, 0x94);
    if (capacity < 0x10) {
        return reinterpret_cast<const char*>(info + 0x80);
    }
    return reinterpret_cast<const char*>(CSF_AT(std::uintptr_t, info, 0x80));
}

static int __fastcall BddLoader_ApplyLoadedNameAndTransition(void* self, void* /*edx*/) {
    uint8_t* base = static_cast<uint8_t*>(self);

    // v2 = this->info (+0x14); name = c_str of info's name member.
    uint8_t* info = CSF_AT(uint8_t*, base, 0x14);
    const char* name = BddLoader_GetLoadedNameCString(info);

    // First virtual call on the object at +0x68: slot 1 (+4), argument = name.
    void* resourceObj = CSF_AT(void*, base, 0x68);
    ((void(__fastcall*)(void*, void*, const char*))CSF_AT(void**, resourceObj, 0)[1])(resourceObj, nullptr, name);

    // Second virtual call on the same object (reloaded): slot 3 (+12), argument = this->+0xA0.
    resourceObj = CSF_AT(void*, base, 0x68);
    std::uintptr_t stateValue = CSF_AT(std::uintptr_t, base, 0xA0);
    ((void(__fastcall*)(void*, void*, std::uintptr_t))CSF_AT(void**, resourceObj, 0)[3])(resourceObj, nullptr, stateValue);

    // Menu manager transition: this = this->+0x6C, argument = this->+0xA0 (reloaded).
    float* menuManager = CSF_AT(float*, base, 0x6C);
    std::uintptr_t transitionArg = CSF_AT(std::uintptr_t, base, 0xA0);
    return static_cast<int>(MenuManager_TransitionToGameOrIntro(menuManager, nullptr, static_cast<int>(transitionArg)));
}

HOOK(0x0050EAC0, BddLoader_ApplyLoadedNameAndTransition, "auto");
