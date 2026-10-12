#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

static CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));

static hook::Tail __fastcall C_00860288_RefreshAndForwardTail(void* self, const uint32_t* args)
{
    (void)args;
    C_00860288* obj = reinterpret_cast<C_00860288*>(self);

    // Optional child at +0x28: notify it via vtable slot 0x88.
    uint32_t childAt28 = obj->field_0028;
    if (childAt28 != 0) {
        void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childAt28));
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            (*reinterpret_cast<void***>(child))[0x88 / 4])(child, nullptr);
    }

    // Object at +0x68: query its entry table through vtable slot 0x1C with key 5.
    uint32_t ownerAt68 = obj->field_0068;
    void* owner68 = reinterpret_cast<void*>(static_cast<uintptr_t>(ownerAt68));
    int tableResult = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        (*reinterpret_cast<void***>(owner68))[0x1C / 4])(owner68, nullptr, 5);
    C_0085e7b8* entryTable = reinterpret_cast<C_0085e7b8*>(static_cast<uintptr_t>(tableResult));

    int entryPayloadAddr = C_0085e7b8_FindEntryPayloadByKey(entryTable, nullptr, 5);
    if (entryPayloadAddr != 0) {
        uint32_t* entryPayloadByKey = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entryPayloadAddr));
        void* payloadObj = reinterpret_cast<void*>(static_cast<uintptr_t>(*entryPayloadByKey));
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            (*reinterpret_cast<void***>(payloadObj))[4 / 4])(payloadObj, nullptr, 1.0f);
    }

    // Tail target at +0x6C: if its slot 0x28 test returns true, jump to its slot 0x24.
    uint32_t tailCandidate = obj->field_006c;
    if (tailCandidate != 0) {
        void* candidate = reinterpret_cast<void*>(static_cast<uintptr_t>(tailCandidate));
        int canForward = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            (*reinterpret_cast<void***>(candidate))[0x28 / 4])(candidate, nullptr);
        if (static_cast<uint8_t>(canForward) != 0) {
            void* tailObj = reinterpret_cast<void*>(static_cast<uintptr_t>(obj->field_006c));
            return hook::tail_jump(tailObj, (*reinterpret_cast<void***>(tailObj))[0x24 / 4]);
        }
        return hook::tail_ret(canForward);
    }

    return hook::tail_ret(0);
}

HOOK_TAIL(0x006B2930, C_00860288_RefreshAndForwardTail, "gfx_object");
