#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Assigns the string stored at (object->field_0004 + 4) into the sub-object at +0x1A9C
// of the global Value8B0554 instance, taking the whole source (position 0, count -1).
static int __stdcall AssignObjectFieldStringToValueSlot1A9C(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    // a3 points to a pointer to an object; read the 32-bit field at +4 of that object.
    std::uint8_t* sourceOwner = *reinterpret_cast<std::uint8_t**>(static_cast<std::uintptr_t>(a3));
    std::uint32_t fieldValue = *reinterpret_cast<std::uint32_t*>(sourceOwner + 4);

    // The original reads the field before calling GetValue8B0554; keep that order.
    std::uint8_t* valueBase = reinterpret_cast<std::uint8_t*>(GetValue8B0554());

    const void* sourceString = reinterpret_cast<const void*>(static_cast<std::uintptr_t>(fieldValue + 4));
    std::uint8_t* targetSubObject = valueBase + 0x1A9C;

    void* result = AssignStringSubstring(targetSubObject, nullptr, sourceString, 0, 0xFFFFFFFFu);
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x00465030, AssignObjectFieldStringToValueSlot1A9C, "auto");
