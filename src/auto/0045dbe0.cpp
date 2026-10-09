#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, SceneRequest_Setup, 0x00446100, (void* self, void* edx, int a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(unsigned int*, __fastcall, SceneRequest_Submit, 0x00446F10, (void* self, void* edx, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Maps the mode record (0..3) to the request mode code; anything else maps to 0.
static uint32_t SceneRequestModeCodeFromRecord(uint32_t modeValue)
{
    switch (modeValue)
    {
    case 0:
        return 0;
    case 1:
        return 2;
    case 2:
        return 4;
    case 3:
        return 12;
    default:
        return 0;
    }
}

// Reads the scalar value stored in the record chain a3[index] -> node->field4 -> value at +4.
static int ReadScriptArgValue(uint32_t* argRecords, int index)
{
    uint32_t* node = reinterpret_cast<uint32_t*>(argRecords[index]);
    uint32_t* valueNode = reinterpret_cast<uint32_t*>(node[1]);
    return static_cast<int>(valueNode[1]);
}

// Builds a scene request from the stdcall argument records and submits it to the current scene's subobject at +0xB0.
static unsigned int* __stdcall ScriptCommands_DispatchSceneRequestFromArgs(int /*a1*/, int /*unused*/, uint32_t* a3)
{
    // Mode code from the first argument record.
    uint32_t* modeNode = reinterpret_cast<uint32_t*>(a3[0]);
    uint32_t* modeValueNode = reinterpret_cast<uint32_t*>(modeNode[1]);
    uint32_t modeValue = modeValueNode[1];
    uint32_t modeCode = SceneRequestModeCodeFromRecord(modeValue);

    // Three-dword block from the second argument record.
    uint32_t* vectorNode = reinterpret_cast<uint32_t*>(a3[1]);
    uint32_t* vectorFields = reinterpret_cast<uint32_t*>(vectorNode[1]) + 1;
    uint32_t requestVector[3];
    requestVector[0] = vectorFields[0];
    requestVector[1] = vectorFields[1];
    requestVector[2] = vectorFields[2];

    // Remaining two scalar arguments, evaluated in the original's order (fourth, then third).
    int fourthValue = ReadScriptArgValue(a3, 3);
    int thirdValue = ReadScriptArgValue(a3, 2);

    uint32_t requestBuffer[10];
    SceneRequest_Setup(requestBuffer, nullptr, static_cast<int>(modeCode), requestVector, thirdValue, fourthValue);

    // Submit to the current scene's subobject at +0xB0 (word index 44).
    void* sceneSubobject = CSF_AT(void*, g_pCurrentScene, 0xB0);
    return SceneRequest_Submit(sceneSubobject, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(requestBuffer)));
}

HOOK(0x0045DBE0, ScriptCommands_DispatchSceneRequestFromArgs, "auto");
