#include <Library/EntryLib.hpp>

EFI_STATUS
EFIAPI
GetCommandLine(
    _Out_ uint16_t*& CommandLineStr,
    _Out_ uint64_t& CommandLineSize
)
{
    EFI_LOADED_IMAGE_PROTOCOL* LoadedImage = nullptr;
    EFI_STATUS Status = gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, reinterpret_cast<void**>(&LoadedImage));

    if (EFI_ERROR(Status))
    {
        CommandLineStr = nullptr;
        CommandLineSize = 0;
        return Status;
    }

    CommandLineStr = reinterpret_cast<uint16_t*>(LoadedImage->LoadOptions);
    CommandLineSize = LoadedImage->LoadOptionsSize;

    return Status;
}

EFI_STATUS
EFIAPI
GetArgs(
    _In_ uint16_t* CommandLineStr,
    _In_ uint64_t CommandLineSize,
    _Out_ char**& ArgV,
    _Out_ int& ArgC
)
{
    if (!CommandLineStr || !CommandLineSize)
    {
        ArgV = nullptr;
        return EFI_INVALID_PARAMETER;
    }

    //
    // Count the arguments and save their addresses (64 args supported).
    //
    uint16_t* Temp = CommandLineStr;
    uint16_t* ArgsW[64];

    while (*Temp != L'\0')
    {
        while (*Temp == L' ')
        {
            Temp++;
        }

        if (*Temp == L'\0')
        {
            break;
        }

        ArgsW[ArgC] = Temp;
        ArgC++;

        while (*Temp != L' ' && *Temp != L'\0')
        {
            Temp++;
        }

        if (*Temp == L' ')
        {
            *Temp = L'\0';
            Temp++;
        }
    }

    if (!ArgC)
    {
        ArgV = nullptr;
        return EFI_NOT_FOUND;
    }

    //
    // Create the argument array.
    //
    ArgV = reinterpret_cast<char**>(AllocatePool(ArgC * sizeof(char*)));

    if (!ArgV)
    {
        return EFI_OUT_OF_RESOURCES;
    }

    for (uint64_t i = 0; i < ArgC; i++)
    {
        uint64_t ArgLength = 0;

        while (ArgsW[i][ArgLength] != L'\0')
        {
            ArgLength++;
        }

        ArgV[i] = reinterpret_cast<char*>(AllocatePool((ArgLength + 1) * sizeof(char)));

        if (!ArgV[i])
        {
            for (uint64_t j = 0; j < i; j++)
            {
                FreePool(ArgV[j]);
            }

            FreePool(ArgV);
            ArgV = nullptr;

            return EFI_OUT_OF_RESOURCES;
        }
        
        for (uint64_t j = 0; j < ArgLength; j++)
        {
            ArgV[i][j] = static_cast<char>(ArgsW[i][j]);
        }

        ArgV[i][ArgLength] = '\0';
    }

    return EFI_SUCCESS;
}

void
EFIAPI
CleanupArgs(
    _In_ int ArgC,
    _In_ char** ArgV
)
{
    if (ArgV)
    {
        for (int i = 0; i < ArgC; i++)
        {
            if (ArgV[i])
            {
                FreePool(ArgV[i]);
            }
        }

        FreePool(ArgV);
    }
}

extern "C"
EFI_STATUS
EFIAPI
UefiMain(
    _In_ [[maybe_unused]] EFI_HANDLE ImageHandle,
    _In_ [[maybe_unused]] EFI_SYSTEM_TABLE* SystemTable
)
{
    uint16_t* CommandLineStr = nullptr;
    uint64_t CommandLineSize = 0;
    EFI_STATUS Status = GetCommandLine(CommandLineStr, CommandLineSize);

    if (EFI_ERROR(Status))
    {
        return EFI_SUCCESS;
    }
    
    char** ArgV = nullptr;
    int ArgC = 0;

    Status = GetArgs(CommandLineStr, CommandLineSize, ArgV, ArgC);

    if (EFI_ERROR(Status))
    {
        return Status;
    }

    Main(ArgC, ArgV);

    CleanupArgs(ArgC, ArgV);

    return EFI_SUCCESS;
}