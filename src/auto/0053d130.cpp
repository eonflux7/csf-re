#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// Original helpers, bound by address.
CSF_FUNC(uint8_t*, __cdecl, GetGlobalValue, 0x0075FDF0, ());
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Layout of the 24-byte string temporary built on the stack
// (inline buffer / heap pointer union, then size, then capacity).
struct OkQuestionStringBuffer {
    union {
        char inlineChars[16];
        char* heapChars;
    };
    uint32_t length;
    uint32_t capacity;
};

static uint8_t* __fastcall CMenuSoundOptions_SendOkQuestionGE03(CMenuSoundOptions* self, void* /*edx*/)
{
    // Find the "BotonOkQuestion" child through the vtable of the object stored at +0x1C.
    void* panelObject = reinterpret_cast<void*>(self->field_001c);
    auto findChildByName = reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, panelObject, 0)[63]);
    uint8_t* result = static_cast<uint8_t*>(
        findChildByName(panelObject, nullptr, CSF_STR(0x008413D4, "BotonOkQuestion")));
    self->field_0048 = reinterpret_cast<uint32_t>(result);

    if (result != nullptr) {
        result = GetGlobalValue();
        if (result[204] == 0) {
            // Object returned by the vtable slot at +0x110 on the button (reloaded from +0x48).
            void* buttonObject = reinterpret_cast<void*>(self->field_0048);
            auto getTargetObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, buttonObject, 0)[68]);
            void* targetObject = getTargetObject(buttonObject, nullptr);

            OkQuestionStringBuffer message;
            message.capacity = 7;
            message.length = 0;
            *reinterpret_cast<uint16_t*>(message.inlineChars) = 0;

            uint32_t scratchReturn = 0;
            InitStringFromLiteral(&scratchReturn, CSF_STR(0x008427FC, "GE03"), 1);

            // Use the heap buffer only when the capacity is 8 or more (unsigned compare).
            const char* messageText = (message.capacity >= 8)
                ? message.heapChars
                : message.inlineChars;

            auto sendMessage = reinterpret_cast<uint8_t* (__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, targetObject, 0)[16]);
            result = sendMessage(targetObject, nullptr, messageText);

            if (message.capacity >= 8) {
                result = reinterpret_cast<uint8_t*>(message.heapChars);
                if (result != nullptr) {
                    return reinterpret_cast<uint8_t*>(Mem_FreeDebug(result));
                }
            }
        }
    }
    return result;
}

HOOK(0x0053D130, CMenuSoundOptions_SendOkQuestionGE03, "auto");
