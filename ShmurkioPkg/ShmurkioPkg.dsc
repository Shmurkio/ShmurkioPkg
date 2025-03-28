[Defines]
    PLATFORM_NAME                   = ShmurkioPkg
    PLATFORM_GUID                   = 7CE89584-E916-4ECB-92BE-F0DD24F391B5
    PLATFORM_VERSION                = 1.0
    DSC_SPECIFICATION               = 0x00010005
    OUTPUT_DIRECTORY                = Build/ShmurkioPkg
    SUPPORTED_ARCHITECTURES         = X64
    BUILD_TARGETS                   = DEBUG|RELEASE
    SKUID_IDENTIFIER                = DEFAULT

[LibraryClasses]
    SerialLib|ShmurkioPkg/Library/SerialLib/SerialLib.inf
    EntryLib|ShmurkioPkg/Library/EntryLib/EntryLib.inf