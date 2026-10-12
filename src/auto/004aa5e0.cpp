#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, FindActiveEntryIndexByKey, 0x004DC5B0, (void* self, void* , void* keyObject));
CSF_FUNC(char, __fastcall, AiFncStatic_CheckPositionPair, 0x004DC660, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, ResolveEntryStateRecord, 0x004DC6E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static bool __fastcall S_004aa5e0_IsGuardTaskComplete(
    S_004aa5e0* self, void* /*edx*/)
{
    auto* link = reinterpret_cast<std::uint8_t*>(self) + 8;

    int linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    void* firstOwner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    void* secondOwner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

    void* positionObject = CSF_AT(void*, firstOwner, 0x2C);
    auto* keyObject = CSF_AT(std::uint8_t*, secondOwner, 0x2C);

    void* context = reinterpret_cast<void*>(GetValue8B0554());
    auto* manager = CSF_AT(std::uint8_t*, context, 0x1FD8);
    int position = GetObjectPositionVector(positionObject, nullptr);

    if (AiFncStatic_CheckPositionPair(
            manager + 0x54, nullptr,
            reinterpret_cast<int>(keyObject + 0x1C), position))
    {
        return true;
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    void* owner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    keyObject = CSF_AT(std::uint8_t*, owner, 0x2C);

    context = reinterpret_cast<void*>(GetValue8B0554());
    manager = CSF_AT(std::uint8_t*, context, 0x1FD8);
    int entryIndex = FindActiveEntryIndexByKey(
        manager + 0x54, nullptr, keyObject + 0x1C);

    if (entryIndex != -1)
    {
        context = reinterpret_cast<void*>(GetValue8B0554());
        manager = CSF_AT(std::uint8_t*, context, 0x1FD8);
        void* stateRecord = reinterpret_cast<void*>(
            ResolveEntryStateRecord(manager + 0x54, nullptr, entryIndex));

        if (CSF_AT(int, stateRecord, 0) == 2)
            return false;
    }

    return true;
}

HOOK(0x004AA5E0, S_004aa5e0_IsGuardTaskComplete, "auto");
