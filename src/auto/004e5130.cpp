#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_4E2BF0, 0x004E2BF0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, sub_4E1640, 0x004E1640, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, sub_4E50D0, 0x004E50D0, (void* self, void* edx, int a2, int a3));

// Allocates a game object sized for the requested kind and hands it to the matching constructor-like
// initializer. Returns the initializer's result, or nullptr for an unknown kind or failed allocation.
static uint32_t* CreateObjectByKind(int kind, int a2, int a3) {
    switch (kind) {
    case 4: {
        void* obj = Mem_Alloc(0x24);
        if (obj)
            return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(sub_4E2BF0(obj, nullptr, a2, a3)));
        break;
    }
    case 5: {
        void* obj = Mem_Alloc(0x54);
        if (obj)
            return sub_4E1640(obj, nullptr, a2, a3);
        break;
    }
    case 6: {
        void* obj = Mem_Alloc(0x48);
        if (obj)
            return sub_4E50D0(obj, nullptr, a2, a3);
        break;
    }
    default:
        return nullptr;
    }
    return nullptr;
}

HOOK(0x4E5130, CreateObjectByKind, "auto");
