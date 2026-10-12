#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(std::uint16_t, g_SessionRequestTemplateSuffix, 0x008680E4);
CSF_GLOBAL(std::uint8_t, g_SessionRequestTemplateTerminator, 0x008680E6);

CSF_GLOBAL(int, g_SessionRequest, 0x008BC164);
CSF_GLOBAL(std::uint8_t, g_SessionRequestNetwork, 0x008BC165);
CSF_GLOBAL(std::uint8_t, g_SessionRequestAccess, 0x008BC166);
CSF_GLOBAL(std::uint8_t, g_SessionRequestMode, 0x008BC167);
CSF_GLOBAL(std::uint16_t, g_SessionRequestSuffix, 0x008BC168);
CSF_GLOBAL(std::uint8_t, g_SessionRequestOption, 0x008BC169);
CSF_GLOBAL(std::uint8_t, g_SessionRequestTerminator, 0x008BC16A);

static int* __cdecl NetDdlObjectManager_CreateSessionRequest(
    char network, int access, int mode, int variant, char option)
{
    const int templatePrefix =
        CSF_AT(int, CSF_STR(0x008680E0, "^ABCDE"), 0);
    const std::uint16_t templateSuffix = g_SessionRequestTemplateSuffix;
    const std::uint8_t templateTerminator = g_SessionRequestTemplateTerminator;

    g_SessionRequest = templatePrefix;
    g_SessionRequestNetwork = network == 0 ? 87 : 76;
    g_SessionRequestSuffix = templateSuffix;
    g_SessionRequestTerminator = templateTerminator;

    switch (access)
    {
    case 0:
        g_SessionRequestAccess = 46;
        break;
    case 1:
        g_SessionRequestAccess = 80;
        break;
    case 2:
        g_SessionRequestAccess = 65;
        break;
    default:
        break;
    }

    switch (mode)
    {
    case 0:
        g_SessionRequestMode = 68;
        variant = 0;
        break;
    case 1:
        g_SessionRequestMode = 84;
        break;
    case 3:
        g_SessionRequestMode = 83;
        break;
    case 5:
        g_SessionRequestMode = 46;
        break;
    default:
        break;
    }

    switch (variant)
    {
    case 0:
        CSF_AT(std::uint8_t, &g_SessionRequestSuffix, 0) = 46;
        break;
    case 1:
        CSF_AT(std::uint8_t, &g_SessionRequestSuffix, 0) = 70;
        break;
    case 2:
        CSF_AT(std::uint8_t, &g_SessionRequestSuffix, 0) = 88;
        break;
    default:
        break;
    }

    g_SessionRequestOption = option == 0 ? 80 : 46;
    return &g_SessionRequest;
}

HOOK(0x00749820, NetDdlObjectManager_CreateSessionRequest, "auto");
