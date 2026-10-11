#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound by address: text input subobject initializer (__thiscall, one stack arg).
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

// Returns the result of initializing the embedded text-input subobject at +0x28
// with the given name, or with the default "COMM3DServer" when the name is null or empty.
static void* __fastcall NetSession_InitWithNameOrDefault(void* self, void* /*edx*/, uint8_t* name)
{
    void* textInputSubobject = reinterpret_cast<uint8_t*>(self) + 0x28;

    if (name && *name)
        return TextInputSubobject_Init(textInputSubobject, nullptr, name);

    return TextInputSubobject_Init(
        textInputSubobject,
        nullptr,
        reinterpret_cast<uint8_t*>(const_cast<char*>(CSF_STR(0x867614, "COMM3DServer"))));
}

HOOK(0x0073C350, NetSession_InitWithNameOrDefault, "auto");
