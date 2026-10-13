#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));

// Refreshes the child at +0x28, looks up entry 5 in the table at +0x68, sets that
// payload's float to 1.0, then forwards the tail call to +0x6C when its test succeeds.
static hook::Tail __fastcall C_00860288_RefreshAndForwardTail(void* selfRaw, const uint32_t* /*args*/)
{
    C_00860288* self = static_cast<C_00860288*>(selfRaw);

    // Notify the child at +0x28 (vtable slot 0x88), if present.
    if (self->field_0028 != 0) {
        void* childAt28 = reinterpret_cast<void*>(self->field_0028);
        ((void(__fastcall*)(void*, void*))(CSF_AT(void**, childAt28, 0)[0x88 / 4]))(childAt28, nullptr);
    }

    // Query the entry table at +0x68 (slot 0x1C, no-argument getter).
    // The key 5 is passed to FindEntryPayloadByKey.
    void* tableAt68 = reinterpret_cast<void*>(self->field_0068);
    C_0085e7b8* entryTable =
        ((C_0085e7b8*(__fastcall*)(void*, void*))(CSF_AT(void**, tableAt68, 0)[0x1C / 4]))(tableAt68, nullptr);
    int entryPayloadAddr = C_0085e7b8_FindEntryPayloadByKey(entryTable, nullptr, 5);

    // Set the found payload's float to 1.0 via its vtable slot 4.
    if (entryPayloadAddr != 0) {
        void* entryPayload = reinterpret_cast<void*>(static_cast<uintptr_t>(entryPayloadAddr));
        void* payloadObj = *reinterpret_cast<void**>(entryPayload);
        ((void(__fastcall*)(void*, void*, float))(CSF_AT(void**, payloadObj, 0)[0x04 / 4]))(payloadObj, nullptr, 1.0f);
    }

    // Test the object at +0x6C via slot 0x28.
    if (self->field_006c == 0) {
        return hook::tail_ret(0);
    }
    void* flagObj = reinterpret_cast<void*>(self->field_006c);
    int flagResult = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, flagObj, 0)[0x28 / 4]))(flagObj, nullptr);
    if ((flagResult & 0xFF) == 0) {
        return hook::tail_ret(flagResult);
    }

    // Tail-forward to the object at +0x6C's slot 0x24 (original re-reads +0x6C here).
    void* tailObj = reinterpret_cast<void*>(self->field_006c);
    return hook::tail_jump(tailObj, (*reinterpret_cast<void***>(tailObj))[0x24 / 4]);
}

HOOK_TAIL(0x006B2930, C_00860288_RefreshAndForwardTail, "gfx_object");
