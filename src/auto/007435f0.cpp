#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, NetSession_IsDuplicate, 0x0064B5C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_74FB40, 0x0074FB40, (void* self, void* edx, char a2, char a3, char a4, char a5, char a6, char a7, char a8, char a9));
CSF_FUNC(int, __fastcall, C_0083e248_ApplyPlayerEvent, 0x004E16C0, (C_0083e248* self, void* edx, int value1, int value2, int value3, int value4, int playerId, int value6, int value7, int value8));

// Dispatches a net entry event: duplicates are forwarded to sub_74FB40, otherwise
// the event goes to the player-event sink stored at +0xD4, if one is set.
static char __fastcall ForwardEntryEventWhenDuplicate(C_00858180* self, void* /*edx*/, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);

static char __fastcall C_00858180_DispatchEntryEvent(C_00858180* self, void* /*edx*/, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
    char result = NetSession_IsDuplicate(self, nullptr);
    if (result)
    {
        return ForwardEntryEventWhenDuplicate(self, nullptr, a2, a3, a4, a5, a6, a7, a8, a9);
    }

    C_0083e248* playerEventSink = CSF_AT(C_0083e248*, self, 0xD4);
    if (playerEventSink)
    {
        return static_cast<char>(C_0083e248_ApplyPlayerEvent(playerEventSink, nullptr, a2, a3, a4, a5, a6, a7, a8, a9));
    }
    return result;
}

static char __fastcall ForwardEntryEventWhenDuplicate(C_00858180* self, void* /*edx*/, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
    return sub_74FB40(self, nullptr,
                      static_cast<char>(a2), static_cast<char>(a3), static_cast<char>(a4), static_cast<char>(a5),
                      static_cast<char>(a6), static_cast<char>(a7), static_cast<char>(a8), static_cast<char>(a9));
}

HOOK(0x007435F0, C_00858180_DispatchEntryEvent, "auto");
