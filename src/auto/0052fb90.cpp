#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"

// Callee at 0x006683F0: __thiscall with no stack args, returns a sub-object pointer.
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

// Dispatches a command code (a2) for the menu object. Returns a 32-bit value in eax.
// a2 == 1  : invokes virtual slot 19 (+0x4C) on the object located 4 bytes before self.
// a2 == 50 : if the byte at a3+0x38 is nonzero, resolves a sub-object from self and
//            invokes its virtual slot 7 (+0x1C); otherwise returns a3.
// any other: returns a2 - 50.
static int __fastcall MenuMpServerInfo_HandleCommand(void* self, void* /*edx*/, int a2, int a3)
{
    if (a2 == 1) {
        uint8_t* prevObject = reinterpret_cast<uint8_t*>(self) - 4;
        void** vtbl = *reinterpret_cast<void***>(prevObject);
        return reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[19])(prevObject, nullptr);
    }

    int result = a2 - 50;
    if (a2 == 50) {
        result = a3;
        uint8_t* sceneSelect = reinterpret_cast<uint8_t*>(a3);
        if (*(sceneSelect + 0x38) != 0) {
            void* subObject = GetReferencedObject(self, nullptr);
            void** vtbl = *reinterpret_cast<void***>(subObject);
            return reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[7])(subObject, nullptr);
        }
    }
    return result;
}

HOOK(0x0052FB90, MenuMpServerInfo_HandleCommand, "auto");
