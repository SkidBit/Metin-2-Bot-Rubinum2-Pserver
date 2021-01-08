#ifndef _NTDLL_H
#define _NTDLL_H

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32_NO_STATUS
#define WIN32_NO_STATUS
#endif
#include <Windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>
#include <intrin.h>

#define NT_SUCCESS(Status)              ((NTSTATUS)(Status) >= 0)
#define NT_ERROR(Status)                ((((ULONG)(Status)) >> 30) == 3)

#define FASTCALL __fastcall

#ifndef _Reserved_
#define _Reserved_
#endif

#define ALIGN_DOWN(length, type) \
    ((ULONG)(length) & ~(sizeof(type) - 1))

#define ALIGN_UP(length, type) \
    (ALIGN_DOWN(((ULONG)(length) + sizeof(type) - 1), type))

#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))

    typedef LONG NTSTATUS, * PNTSTATUS;
    typedef LONG KPRIORITY, * PKPRIORITY;
    typedef ULONG LOGICAL, * PLOGICAL;

    typedef struct _CLIENT_ID
    {
        HANDLE UniqueProcess;
        HANDLE UniqueThread;
    } CLIENT_ID, * PCLIENT_ID;

#define HARDERROR_OVERRIDE_ERRORMODE        0x10000000

    typedef enum _HARDERROR_RESPONSE_OPTION
    {
        OptionAbortRetryIgnore,
        OptionOk,
        OptionOkCancel,
        OptionRetryCancel,
        OptionYesNo,
        OptionYesNoCancel,
        OptionShutdownSystem,
        OptionOkNoWait,
        OptionCancelTryContinue
    } HARDERROR_RESPONSE_OPTION, * PHARDERROR_RESPONSE_OPTION;

    typedef enum _HARDERROR_RESPONSE
    {
        ResponseReturnToCaller,
        ResponseNotHandled,
        ResponseAbort,
        ResponseCancel,
        ResponseIgnore,
        ResponseNo,
        ResponseOk,
        ResponseRetry,
        ResponseYes,
        ResponseTryAgain,
        ResponseContinue
    } HARDERROR_RESPONSE, * PHARDERROR_RESPONSE;

    typedef struct _UNICODE_STRING
    {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR Buffer;
    } UNICODE_STRING, * PUNICODE_STRING;
    typedef const UNICODE_STRING* PCUNICODE_STRING;

#define DECLARE_UNICODE_STRING_SIZE(_var, _size) \
WCHAR _var ## _buffer[_size]; \
__pragma(warning(push)) \
__pragma(warning(disable:4221)) __pragma(warning(disable:4204)) \
UNICODE_STRING _var = { 0, (_size) * sizeof(WCHAR) , _var ## _buffer } \
__pragma(warning(pop))

#define DECLARE_STATIC_UNICODE_STRING_SIZE(_var, _size) \
WCHAR _var ## _buffer[_size]; \
__pragma(warning(push)) \
__pragma(warning(disable:4221)) __pragma(warning(disable:4204)) \
static UNICODE_STRING _var = { 0, (_size) * sizeof(WCHAR) , _var ## _buffer } \
__pragma(warning(pop))

#if defined(__clang__)
#define RTL_CONSTANT_STRING(s) \
__pragma(clang diagnostic push) \
__pragma(clang diagnostic ignored "-Wwritable-strings") \
{ sizeof(s) - sizeof((s)[0]), sizeof(s), s } \
__pragma(clang diagnostic pop)
#else
#define RTL_CONSTANT_STRING(s) { sizeof(s) - sizeof((s)[0]), sizeof(s), (PWSTR)s }
#define RTL_CONSTANT_ANSI_STRING(s) { sizeof(s) - sizeof((s)[0]), sizeof(s), (PSTR)s }
#endif

    // https://stackoverflow.com/questions/36961152/detect-windows-kit-8-0-and-windows-kit-8-1-sdks
#if defined(WINAPI_PARTITION_APP)
#if defined(WINAPI_FAMILY_SYSTEM)
#define USING_WINDOWS_10_SDK
#elif (WINAPI_PARTITION_APP == 0x00000002) || (WINAPI_PARTITION_APP == 1)
#define USING_WINDOWS_8_x_SDK
#endif
#endif

// This struct was included in winnt.h starting in the Windows 8.0 SDK
#if !(defined(USING_WINDOWS_8_x_SDK) || defined(USING_WINDOWS_10_SDK))
    typedef struct _EXCEPTION_REGISTRATION_RECORD
    {
        _EXCEPTION_REGISTRATION_RECORD* Next;
        _EXCEPTION_DISPOSITION Handler;
    } EXCEPTION_REGISTRATION_RECORD, * PEXCEPTION_REGISTRATION_RECORD;
#endif

    typedef struct _RTL_BITMAP
    {
        ULONG SizeOfBitMap;
        PULONG Buffer;
    } RTL_BITMAP, * PRTL_BITMAP;

    typedef struct _RTL_BITMAP_RUN
    {
        ULONG StartingIndex;
        ULONG NumberOfBits;
    } RTL_BITMAP_RUN, * PRTL_BITMAP_RUN;

    typedef enum
    {
        RtlBsdItemVersionNumber = 0x00,
        RtlBsdItemProductType,
        RtlBsdItemAabEnabled,
        RtlBsdItemAabTimeout,
        RtlBsdItemBootGood,
        RtlBsdItemBootShutdown,
        RtlBsdItemMax
    } RTL_BSD_ITEM_TYPE, * PRTL_BSD_ITEM_TYPE;

    typedef struct _CURDIR
    {
        UNICODE_STRING DosPath;
        HANDLE Handle;
    } CURDIR, * PCURDIR;

#define RTL_USER_PROC_CURDIR_CLOSE 0x00000002
#define RTL_USER_PROC_CURDIR_INHERIT 0x00000003

    typedef struct _RTL_DRIVE_LETTER_CURDIR
    {
        USHORT Flags;
        USHORT Length;
        ULONG TimeStamp;
        UNICODE_STRING DosPath;
    } RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;

#define RTL_MAX_DRIVE_LETTERS 32
#define RTL_DRIVE_LETTER_VALID (USHORT)0x0001

    typedef struct _LDR_SERVICE_TAG_RECORD
    {
        struct _LDR_SERVICE_TAG_RECORD* Next;
        ULONG ServiceTag;
    } LDR_SERVICE_TAG_RECORD, * PLDR_SERVICE_TAG_RECORD;

    typedef struct _LDRP_CSLIST
    {
        PSINGLE_LIST_ENTRY Tail;
    } LDRP_CSLIST, * PLDRP_CSLIST;

    typedef enum _LDR_DDAG_STATE
    {
        LdrModulesMerged = -5,
        LdrModulesInitError = -4,
        LdrModulesSnapError = -3,
        LdrModulesUnloaded = -2,
        LdrModulesUnloading = -1,
        LdrModulesPlaceHolder = 0,
        LdrModulesMapping = 1,
        LdrModulesMapped = 2,
        LdrModulesWaitingForDependencies = 3,
        LdrModulesSnapping = 4,
        LdrModulesSnapped = 5,
        LdrModulesCondensed = 6,
        LdrModulesReadyToInit = 7,
        LdrModulesInitializing = 8,
        LdrModulesReadyToRun = 9
    } LDR_DDAG_STATE;

    typedef struct _LDR_DDAG_NODE
    {
        LIST_ENTRY Modules;
        PLDR_SERVICE_TAG_RECORD ServiceTagList;
        ULONG LoadCount;
        ULONG LoadWhileUnloadingCount;
        ULONG LowestLink;
        union
        {
            LDRP_CSLIST Dependencies;
            SINGLE_LIST_ENTRY RemovalLink;
        };
        LDRP_CSLIST IncomingDependencies;
        LDR_DDAG_STATE State;
        SINGLE_LIST_ENTRY CondenseLink;
        ULONG PreorderNumber;
    } LDR_DDAG_NODE, * PLDR_DDAG_NODE;

    typedef struct _LDR_DEPENDENCY_RECORD
    {
        SINGLE_LIST_ENTRY DependencyLink;
        PLDR_DDAG_NODE DependencyNode;
        SINGLE_LIST_ENTRY IncomingDependencyLink;
        PLDR_DDAG_NODE IncomingDependencyNode;
    } LDR_DEPENDENCY_RECORD, * PLDR_DEPENDENCY_RECORD;

    typedef enum _LDR_DLL_LOAD_REASON
    {
        LoadReasonStaticDependency,
        LoadReasonStaticForwarderDependency,
        LoadReasonDynamicForwarderDependency,
        LoadReasonDelayloadDependency,
        LoadReasonDynamicLoad,
        LoadReasonAsImageLoad,
        LoadReasonAsDataLoad,
        LoadReasonUnknown = -1
    } LDR_DLL_LOAD_REASON, * PLDR_DLL_LOAD_REASON;

    typedef struct _RTL_BALANCED_NODE
    {
        union
        {
            struct _RTL_BALANCED_NODE* Children[2];
            struct
            {
                struct _RTL_BALANCED_NODE* Left;
                struct _RTL_BALANCED_NODE* Right;
            } s;
        };
        union
        {
            UCHAR Red : 1;
            UCHAR Balance : 2;
            ULONG_PTR ParentValue;
        } u;
    } RTL_BALANCED_NODE, * PRTL_BALANCED_NODE;

    typedef struct _LDR_DATA_TABLE_ENTRY
    {
        LIST_ENTRY InLoadOrderLinks;
        LIST_ENTRY InMemoryOrderLinks;
        union
        {
            LIST_ENTRY InInitializationOrderLinks;
            LIST_ENTRY InProgressLinks;
        };
        PVOID DllBase;
        PVOID EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        union
        {
            UCHAR FlagGroup[4];
            ULONG Flags;
            struct
            {
                ULONG PackagedBinary : 1;
                ULONG MarkedForRemoval : 1;
                ULONG ImageDll : 1;
                ULONG LoadNotificationsSent : 1;
                ULONG TelemetryEntryProcessed : 1;
                ULONG ProcessStaticImport : 1;
                ULONG InLegacyLists : 1;
                ULONG InIndexes : 1;
                ULONG ShimDll : 1;
                ULONG InExceptionTable : 1;
                ULONG ReservedFlags1 : 2;
                ULONG LoadInProgress : 1;
                ULONG LoadConfigProcessed : 1;
                ULONG EntryProcessed : 1;
                ULONG ProtectDelayLoad : 1;
                ULONG ReservedFlags3 : 2;
                ULONG DontCallForThreads : 1;
                ULONG ProcessAttachCalled : 1;
                ULONG ProcessAttachFailed : 1;
                ULONG CorDeferredValidate : 1;
                ULONG CorImage : 1;
                ULONG DontRelocate : 1;
                ULONG CorILOnly : 1;
                ULONG ReservedFlags5 : 3;
                ULONG Redirected : 1;
                ULONG ReservedFlags6 : 2;
                ULONG CompatDatabaseProcessed : 1;
            } s;
        } u;
        USHORT ObsoleteLoadCount;
        USHORT TlsIndex;
        LIST_ENTRY HashLinks;
        ULONG TimeDateStamp;
        struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
        PVOID Lock;
        PLDR_DDAG_NODE DdagNode;
        LIST_ENTRY NodeModuleLink;
        struct _LDRP_LOAD_CONTEXT* LoadContext;
        PVOID ParentDllBase;
        PVOID SwitchBackContext;
        RTL_BALANCED_NODE BaseAddressIndexNode;
        RTL_BALANCED_NODE MappingInfoIndexNode;
        ULONG_PTR OriginalBase;
        LARGE_INTEGER LoadTime;
        ULONG BaseNameHashValue;
        LDR_DLL_LOAD_REASON LoadReason;
        ULONG ImplicitPathOptions;
        ULONG ReferenceCount;
        ULONG DependentLoadFlags;
        UCHAR SigningLevel; // Since Windows 10 RS2
    } LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;


#define RTL_CREATE_ENVIRONMENT_TRANSLATE            0x1 // Translate from multi-byte to Unicode
#define RTL_CREATE_ENVIRONMENT_TRANSLATE_FROM_OEM   0x2 // Translate from OEM to Unicode (Translate flag must also be set)
#define RTL_CREATE_ENVIRONMENT_EMPTY                0x4 // Create empty environment block

    typedef struct _RTL_BUFFER
    {
        PUCHAR Buffer;
        PUCHAR StaticBuffer;
        SIZE_T Size;
        SIZE_T StaticSize;
        SIZE_T ReservedForAllocatedSize; // for future doubling
        PVOID ReservedForIMalloc; // for future pluggable growth
    } RTL_BUFFER, * PRTL_BUFFER;

    typedef struct _RTL_UNICODE_STRING_BUFFER
    {
        UNICODE_STRING String;
        RTL_BUFFER ByteBuffer;
        UCHAR MinimumStaticBufferForTerminalNul[sizeof(WCHAR)];
    } RTL_UNICODE_STRING_BUFFER, * PRTL_UNICODE_STRING_BUFFER;

    typedef struct _RTL_USER_PROCESS_PARAMETERS
    {
        ULONG MaximumLength;
        ULONG Length;

        ULONG Flags;
        ULONG DebugFlags;

        HANDLE ConsoleHandle;
        ULONG ConsoleFlags;
        HANDLE StandardInput;
        HANDLE StandardOutput;
        HANDLE StandardError;

        CURDIR CurrentDirectory;
        UNICODE_STRING DllPath;
        UNICODE_STRING ImagePathName;
        UNICODE_STRING CommandLine;
        PWCHAR Environment;

        ULONG StartingX;
        ULONG StartingY;
        ULONG CountX;
        ULONG CountY;
        ULONG CountCharsX;
        ULONG CountCharsY;
        ULONG FillAttribute;

        ULONG WindowFlags;
        ULONG ShowWindowFlags;
        UNICODE_STRING WindowTitle;
        UNICODE_STRING DesktopInfo;
        UNICODE_STRING ShellInfo;
        UNICODE_STRING RuntimeData;
        RTL_DRIVE_LETTER_CURDIR CurrentDirectories[RTL_MAX_DRIVE_LETTERS];

        ULONG_PTR EnvironmentSize;
        ULONG_PTR EnvironmentVersion;
        PVOID PackageDependencyData;
        ULONG ProcessGroupId;
        ULONG LoaderThreads;
    } RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

#define RTL_USER_PROCESS_PARAMETERS_NORMALIZED              0x01
#define RTL_USER_PROCESS_PARAMETERS_PROFILE_USER            0x02
#define RTL_USER_PROCESS_PARAMETERS_PROFILE_KERNEL          0x04
#define RTL_USER_PROCESS_PARAMETERS_PROFILE_SERVER          0x08
#define RTL_USER_PROCESS_PARAMETERS_RESERVE_1MB             0x20
#define RTL_USER_PROCESS_PARAMETERS_RESERVE_16MB            0x40
#define RTL_USER_PROCESS_PARAMETERS_CASE_SENSITIVE          0x80
#define RTL_USER_PROCESS_PARAMETERS_DISABLE_HEAP_DECOMMIT   0x100
#define RTL_USER_PROCESS_PARAMETERS_DLL_REDIRECTION_LOCAL   0x1000
#define RTL_USER_PROCESS_PARAMETERS_APP_MANIFEST_PRESENT    0x2000
#define RTL_USER_PROCESS_PARAMETERS_IMAGE_KEY_MISSING       0x4000
#define RTL_USER_PROCESS_PARAMETERS_NX_OPTIN                0x20000

    // Handle tag bits for PEB stdio file handles
#define PEB_STDIO_HANDLE_NATIVE     0
#define PEB_STDIO_HANDLE_SUBSYS     1
#define PEB_STDIO_HANDLE_PM         2
#define PEB_STDIO_HANDLE_RESERVED   3

#define GDI_HANDLE_BUFFER_SIZE32    34
#define GDI_HANDLE_BUFFER_SIZE64    60

#ifndef _WIN64
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE32
#else
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE64
#endif

    typedef ULONG GDI_HANDLE_BUFFER32[GDI_HANDLE_BUFFER_SIZE32];
    typedef ULONG GDI_HANDLE_BUFFER64[GDI_HANDLE_BUFFER_SIZE64];
    typedef ULONG GDI_HANDLE_BUFFER[GDI_HANDLE_BUFFER_SIZE];

#define FLS_MAXIMUM_AVAILABLE 128
#define TLS_MINIMUM_AVAILABLE 64
#define TLS_EXPANSION_SLOTS 1024

    typedef struct _PEB_LDR_DATA
    {
        ULONG Length;
        BOOLEAN Initialized;
        HANDLE SsHandle;
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
        PVOID EntryInProgress;
        BOOLEAN ShutdownInProgress;
        HANDLE ShutdownThreadId;
    } PEB_LDR_DATA, * PPEB_LDR_DATA;

    typedef struct _ACTIVATION_CONTEXT_STACK
    {
        struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME* ActiveFrame;
        LIST_ENTRY FrameListCache;
        ULONG Flags;
        ULONG NextCookieSequenceNumber;
        ULONG StackId;
    } ACTIVATION_CONTEXT_STACK, * PACTIVATION_CONTEXT_STACK;

    typedef struct _PEB
    {
        BOOLEAN InheritedAddressSpace;
        BOOLEAN ReadImageFileExecOptions;
        BOOLEAN BeingDebugged;
        union
        {
            BOOLEAN BitField;
            struct
            {
                BOOLEAN ImageUsesLargePages : 1;
                BOOLEAN IsProtectedProcess : 1;
                BOOLEAN IsImageDynamicallyRelocated : 1;
                BOOLEAN SkipPatchingUser32Forwarders : 1;
                BOOLEAN IsPackagedProcess : 1;
                BOOLEAN IsAppContainer : 1;
                BOOLEAN IsProtectedProcessLight : 1;
                BOOLEAN IsLongPathAwareProcess : 1;
            } s1;
        } u1;

        HANDLE Mutant;

        PVOID ImageBaseAddress;
        PPEB_LDR_DATA Ldr;
        PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
        PVOID SubSystemData;
        PVOID ProcessHeap;
        PRTL_CRITICAL_SECTION FastPebLock;
        PVOID AtlThunkSListPtr;
        PVOID IFEOKey;
        union
        {
            ULONG CrossProcessFlags;
            struct
            {
                ULONG ProcessInJob : 1;
                ULONG ProcessInitializing : 1;
                ULONG ProcessUsingVEH : 1;
                ULONG ProcessUsingVCH : 1;
                ULONG ProcessUsingFTH : 1;
                ULONG ProcessPreviouslyThrottled : 1;
                ULONG ProcessCurrentlyThrottled : 1;
                ULONG ReservedBits0 : 25;
            } s2;
        } u2;
        union
        {
            PVOID KernelCallbackTable;
            PVOID UserSharedInfoPtr;
        } u3;
        ULONG SystemReserved[1];
        ULONG AtlThunkSListPtr32;
        PVOID ApiSetMap;
        ULONG TlsExpansionCounter;
        PVOID TlsBitmap;
        ULONG TlsBitmapBits[2];

        PVOID ReadOnlySharedMemoryBase;
        PVOID SharedData; // HotpatchInformation
        PVOID* ReadOnlyStaticServerData;

        PVOID AnsiCodePageData; // PCPTABLEINFO
        PVOID OemCodePageData; // PCPTABLEINFO
        PVOID UnicodeCaseTableData; // PNLSTABLEINFO

        ULONG NumberOfProcessors;
        ULONG NtGlobalFlag;

        LARGE_INTEGER CriticalSectionTimeout;
        SIZE_T HeapSegmentReserve;
        SIZE_T HeapSegmentCommit;
        SIZE_T HeapDeCommitTotalFreeThreshold;
        SIZE_T HeapDeCommitFreeBlockThreshold;

        ULONG NumberOfHeaps;
        ULONG MaximumNumberOfHeaps;
        PVOID* ProcessHeaps; // PHEAP

        PVOID GdiSharedHandleTable;
        PVOID ProcessStarterHelper;
        ULONG GdiDCAttributeList;

        PRTL_CRITICAL_SECTION LoaderLock;

        ULONG OSMajorVersion;
        ULONG OSMinorVersion;
        USHORT OSBuildNumber;
        USHORT OSCSDVersion;
        ULONG OSPlatformId;
        ULONG ImageSubsystem;
        ULONG ImageSubsystemMajorVersion;
        ULONG ImageSubsystemMinorVersion;
        ULONG_PTR ActiveProcessAffinityMask;
        GDI_HANDLE_BUFFER GdiHandleBuffer;
        PVOID PostProcessInitRoutine;

        PVOID TlsExpansionBitmap;
        ULONG TlsExpansionBitmapBits[32];

        ULONG SessionId;

        ULARGE_INTEGER AppCompatFlags;
        ULARGE_INTEGER AppCompatFlagsUser;
        PVOID pShimData;
        PVOID AppCompatInfo; // APPCOMPAT_EXE_DATA

        UNICODE_STRING CSDVersion;

        PVOID ActivationContextData; // ACTIVATION_CONTEXT_DATA
        PVOID ProcessAssemblyStorageMap; // ASSEMBLY_STORAGE_MAP
        PVOID SystemDefaultActivationContextData; // ACTIVATION_CONTEXT_DATA
        PVOID SystemAssemblyStorageMap; // ASSEMBLY_STORAGE_MAP

        SIZE_T MinimumStackCommit;

        PVOID* FlsCallback;
        LIST_ENTRY FlsListHead;
        PVOID FlsBitmap;
        ULONG FlsBitmapBits[FLS_MAXIMUM_AVAILABLE / (sizeof(ULONG) * 8)];
        ULONG FlsHighIndex;

        PVOID WerRegistrationData;
        PVOID WerShipAssertPtr;
        PVOID pUnused; // pContextData
        PVOID pImageHeaderHash;
        union
        {
            ULONG TracingFlags;
            struct
            {
                ULONG HeapTracingEnabled : 1;
                ULONG CritSecTracingEnabled : 1;
                ULONG LibLoaderTracingEnabled : 1;
                ULONG SpareTracingBits : 29;
            } s3;
        } u4;
        ULONGLONG CsrServerReadOnlySharedMemoryBase;
        PVOID TppWorkerpListLock;
        LIST_ENTRY TppWorkerpList;
        PVOID WaitOnAddressHashTable[128];
        PVOID TelemetryCoverageHeader; // REDSTONE3
        ULONG CloudFileFlags;
    } PEB, * PPEB;

    typedef struct _PROCESS_BASIC_INFORMATION
    {
        NTSTATUS ExitStatus;
        PPEB PebBaseAddress;
        ULONG_PTR AffinityMask;
        KPRIORITY BasePriority;
        HANDLE UniqueProcessId;
        HANDLE InheritedFromUniqueProcessId;
    } PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;

    typedef struct _PROCESS_EXTENDED_BASIC_INFORMATION
    {
        SIZE_T Size; // Set to sizeof structure on input
        PROCESS_BASIC_INFORMATION BasicInfo;
        union
        {
            ULONG Flags;
            struct
            {
                ULONG IsProtectedProcess : 1;
                ULONG IsWow64Process : 1;
                ULONG IsProcessDeleting : 1;
                ULONG IsCrossSessionCreate : 1;
                ULONG IsFrozen : 1;
                ULONG IsBackground : 1;
                ULONG IsStronglyNamed : 1;
                ULONG IsSecureProcess : 1;
                ULONG IsSubsystemProcess : 1;
                ULONG SpareBits : 23;
            } s;
        } u;
    } PROCESS_EXTENDED_BASIC_INFORMATION, * PPROCESS_EXTENDED_BASIC_INFORMATION;
#endif
}