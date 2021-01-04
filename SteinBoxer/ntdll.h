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

    enum KPROCESSOR_MODE
    {
        KernelMode,
        UserMode
    };

    typedef enum _KTHREAD_STATE
    {
        Initialized,
        Ready,
        Running,
        Standby,
        Terminated,
        Waiting,
        Transition,
        DeferredReady,
        GateWaitObsolete,
        WaitingForProcessInSwap,
        MaximumThreadState
    } KTHREAD_STATE, * PKTHREAD_STATE;

    typedef enum _KWAIT_REASON
    {
        Executive,
        FreePage,
        PageIn,
        PoolAllocation,
        DelayExecution,
        Suspended,
        UserRequest,
        WrExecutive,
        WrFreePage,
        WrPageIn,
        WrPoolAllocation,
        WrDelayExecution,
        WrSuspended,
        WrUserRequest,
        WrEventPair,
        WrQueue,
        WrLpcReceive,
        WrLpcReply,
        WrVirtualMemory,
        WrPageOut,
        WrRendezvous,
        WrKeyedEvent,
        WrTerminated,
        WrProcessInSwap,
        WrCpuRateControl,
        WrCalloutStack,
        WrKernel,
        WrResource,
        WrPushLock,
        WrMutex,
        WrQuantumEnd,
        WrDispatchInt,
        WrPreempted,
        WrYieldExecution,
        WrFastMutex,
        WrGuardedMutex,
        WrRundown,
        WrAlertByThreadId,
        WrDeferredPreempt,
        MaximumWaitReason
    } KWAIT_REASON;

    typedef enum _EVENT_TYPE
    {
        NotificationEvent,
        SynchronizationEvent
    } EVENT_TYPE;

    typedef enum _TIMER_TYPE
    {
        NotificationTimer,
        SynchronizationTimer
    } TIMER_TYPE;

    typedef enum _WAIT_TYPE
    {
        WaitAll,
        WaitAny,
        WaitNotification,
        WaitDequeue
    } WAIT_TYPE;

    typedef enum _SECTION_INHERIT
    {
        ViewShare = 1,
        ViewUnmap = 2
    } SECTION_INHERIT;

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

    FORCEINLINE
        VOID
        RtlInitEmptyUnicodeString(
            _Out_ PUNICODE_STRING UnicodeString,
            _In_ PWCHAR Buffer,
            _In_ USHORT BufferSize)
    {
        UnicodeString->Length = 0;
        UnicodeString->MaximumLength = BufferSize;
        UnicodeString->Buffer = Buffer;
    }

    typedef struct _STRING
    {
        USHORT Length;
        USHORT MaximumLength;
        PCHAR Buffer;
    } ANSI_STRING, * PANSI_STRING;

    typedef struct _SYSTEM_SESSION_PROCESS_INFORMATION
    {
        ULONG SessionId;
        ULONG SizeOfBuf;
        PVOID Buffer;
    } SYSTEM_SESSION_PROCESS_INFORMATION, * PSYSTEM_SESSION_PROCESS_INFORMATION;

    typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION
    {
        BOOLEAN KernelDebuggerEnabled;
        BOOLEAN KernelDebuggerNotPresent;
    } SYSTEM_KERNEL_DEBUGGER_INFORMATION, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

    typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX
    {
        BOOLEAN DebuggerAllowed;
        BOOLEAN DebuggerEnabled;
        BOOLEAN DebuggerPresent;
    } SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION_EX;

    typedef struct _LDT_INFORMATION
    {
        ULONG Start;
        ULONG Length;
        LDT_ENTRY LdtEntries[1];
    } PROCESS_LDT_INFORMATION, * PPROCESS_LDT_INFORMATION;

    typedef struct _KERNEL_USER_TIMES
    {
        LARGE_INTEGER CreateTime;
        LARGE_INTEGER ExitTime;
        LARGE_INTEGER KernelTime;
        LARGE_INTEGER UserTime;
    } KERNEL_USER_TIMES, * PKERNEL_USER_TIMES;

    typedef struct _SYSTEM_THREAD_INFORMATION
    {
        LARGE_INTEGER KernelTime;
        LARGE_INTEGER UserTime;
        LARGE_INTEGER CreateTime;
        ULONG WaitTime;
        PVOID StartAddress;
        CLIENT_ID ClientId;
        KPRIORITY Priority;
        LONG BasePriority;
        ULONG ContextSwitches;
        ULONG ThreadState;
        KWAIT_REASON WaitReason;
    } SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

    typedef struct _SYSTEM_PROCESS_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG NumberOfThreads;
        LARGE_INTEGER WorkingSetPrivateSize; // Since Vista
        ULONG HardFaultCount; // Since Windows 7
        ULONG NumberOfThreadsHighWatermark; // Since Windows 7
        ULONGLONG CycleTime; // Since Windows 7
        LARGE_INTEGER CreateTime;
        LARGE_INTEGER UserTime;
        LARGE_INTEGER KernelTime;
        UNICODE_STRING ImageName;
        KPRIORITY BasePriority;
        HANDLE UniqueProcessId;
        HANDLE InheritedFromUniqueProcessId;
        ULONG HandleCount;
        ULONG SessionId;
        ULONG_PTR UniqueProcessKey; // Since Vista (requires SystemExtendedProcessInformation)
        SIZE_T PeakVirtualSize;
        SIZE_T VirtualSize;
        ULONG PageFaultCount;
        SIZE_T PeakWorkingSetSize;
        SIZE_T WorkingSetSize;
        SIZE_T QuotaPeakPagedPoolUsage;
        SIZE_T QuotaPagedPoolUsage;
        SIZE_T QuotaPeakNonPagedPoolUsage;
        SIZE_T QuotaNonPagedPoolUsage;
        SIZE_T PagefileUsage;
        SIZE_T PeakPagefileUsage;
        SIZE_T PrivatePageCount;
        LARGE_INTEGER ReadOperationCount;
        LARGE_INTEGER WriteOperationCount;
        LARGE_INTEGER OtherOperationCount;
        LARGE_INTEGER ReadTransferCount;
        LARGE_INTEGER WriteTransferCount;
        LARGE_INTEGER OtherTransferCount;
        SYSTEM_THREAD_INFORMATION Threads[1];
    } SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

    typedef struct _PROCESS_SESSION_INFORMATION
    {
        ULONG SessionId;
    } PROCESS_SESSION_INFORMATION, * PPROCESS_SESSION_INFORMATION;

    // File attribute values
#define FILE_ATTRIBUTE_READONLY                 0x00000001
#define FILE_ATTRIBUTE_HIDDEN                   0x00000002
#define FILE_ATTRIBUTE_SYSTEM                   0x00000004

#define FILE_ATTRIBUTE_DIRECTORY                0x00000010
#define FILE_ATTRIBUTE_ARCHIVE                  0x00000020
#define FILE_ATTRIBUTE_DEVICE                   0x00000040
#define FILE_ATTRIBUTE_NORMAL                   0x00000080

#define FILE_ATTRIBUTE_TEMPORARY                0x00000100
#define FILE_ATTRIBUTE_SPARSE_FILE              0x00000200
#define FILE_ATTRIBUTE_REPARSE_POINT            0x00000400
#define FILE_ATTRIBUTE_COMPRESSED               0x00000800

#define FILE_ATTRIBUTE_OFFLINE                  0x00001000
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED      0x00002000
#define FILE_ATTRIBUTE_ENCRYPTED                0x00004000

#define FILE_ATTRIBUTE_INTEGRITY_STREAM         0x00008000
#define FILE_ATTRIBUTE_VIRTUAL                  0x00010000
#define FILE_ATTRIBUTE_NO_SCRUB_DATA            0x00020000

#define FILE_ATTRIBUTE_EA                       0x00040000
#define FILE_ATTRIBUTE_PINNED                   0x00080000
#define FILE_ATTRIBUTE_UNPINNED                 0x00100000
#define FILE_ATTRIBUTE_RECALL_ON_OPEN           0x00040000
#define FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS    0x00400000

#if NTDDI_VERSION < NTDDI_WIN8
#define FILE_ATTRIBUTE_VALID_FLAGS              0x00007fb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS          0x000031a7
#elif NTDDI_VERSION < NTDDI_WIN10_RS2
#define FILE_ATTRIBUTE_VALID_FLAGS              0x0002ffb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS          0x000231a7
#else
#define FILE_ATTRIBUTE_VALID_FLAGS              0x005affb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS          0x001a31a7
#endif

// File create disposition values
#define FILE_SUPERSEDE                          0x00000000
#define FILE_OPEN                               0x00000001
#define FILE_CREATE                             0x00000002
#define FILE_OPEN_IF                            0x00000003
#define FILE_OVERWRITE                          0x00000004
#define FILE_OVERWRITE_IF                       0x00000005
#define FILE_MAXIMUM_DISPOSITION                0x00000005

// File create/open option flags
#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008

#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080

#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
#define FILE_OPEN_FOR_RECOVERY                  0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800

#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000

#if NTDDI_VERSION >= NTDDI_WIN7
#define FILE_OPEN_REQUIRING_OPLOCK              0x00010000
#define FILE_DISALLOW_EXCLUSIVE                 0x00020000
#endif
#if NTDDI_VERSION >= NTDDI_WIN8
#define FILE_SESSION_AWARE                      0x00040000
#endif

#define FILE_RESERVE_OPFILTER                   0x00100000
#define FILE_OPEN_REPARSE_POINT                 0x00200000
#define FILE_OPEN_NO_RECALL                     0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x00800000

#define FILE_VALID_OPTION_FLAGS                 0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS            0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS        0x00000032
#define FILE_VALID_SET_FLAGS                    0x00000036

// Named pipe type flags
#define FILE_PIPE_BYTE_STREAM_TYPE              0x00000000
#define FILE_PIPE_MESSAGE_TYPE                  0x00000001
#define FILE_PIPE_ACCEPT_REMOTE_CLIENTS         0x00000000
#define FILE_PIPE_REJECT_REMOTE_CLIENTS         0x00000002
#define FILE_PIPE_TYPE_VALID_MASK               0x00000003

// Named pipe completion mode flags
#define FILE_PIPE_QUEUE_OPERATION               0x00000000
#define FILE_PIPE_COMPLETE_OPERATION            0x00000001

// Named pipe read mode flags
#define FILE_PIPE_BYTE_STREAM_MODE              0x00000000
#define FILE_PIPE_MESSAGE_MODE                  0x00000001

// NamedPipeConfiguration flags
#define FILE_PIPE_INBOUND                       0x00000000
#define FILE_PIPE_OUTBOUND                      0x00000001
#define FILE_PIPE_FULL_DUPLEX                   0x00000002

// NamedPipeState flags
#define FILE_PIPE_DISCONNECTED_STATE            0x00000001
#define FILE_PIPE_LISTENING_STATE               0x00000002
#define FILE_PIPE_CONNECTED_STATE               0x00000003
#define FILE_PIPE_CLOSING_STATE                 0x00000004

// NamedPipeEnd flags
#define FILE_PIPE_CLIENT_END                    0x00000000
#define FILE_PIPE_SERVER_END                    0x00000001

    typedef struct _FILE_BASIC_INFORMATION
    {
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        ULONG FileAttributes;
    } FILE_BASIC_INFORMATION, * PFILE_BASIC_INFORMATION;

    typedef struct _FILE_STANDARD_INFORMATION
    {
        LARGE_INTEGER AllocationSize;
        LARGE_INTEGER EndOfFile;
        ULONG NumberOfLinks;
        BOOLEAN DeletePending;
        BOOLEAN Directory;
    } FILE_STANDARD_INFORMATION, * PFILE_STANDARD_INFORMATION;

    typedef struct _FILE_POSITION_INFORMATION
    {
        LARGE_INTEGER CurrentByteOffset;
    } FILE_POSITION_INFORMATION, * PFILE_POSITION_INFORMATION;

    typedef struct _THREAD_BASIC_INFORMATION
    {
        NTSTATUS ExitStatus;
        PVOID TebBaseAddress;
        CLIENT_ID ClientId;
        ULONG_PTR AffinityMask;
        KPRIORITY Priority;
        LONG BasePriority;
    } THREAD_BASIC_INFORMATION, * PTHREAD_BASIC_INFORMATION;

    typedef struct _MEMORY_REGION_INFORMATION
    {
        PVOID AllocationBase;
        ULONG AllocationProtect;
        union
        {
            ULONG RegionType;
            struct
            {
                ULONG Private : 1;
                ULONG MappedDataFile : 1;
                ULONG MappedImage : 1;
                ULONG MappedPageFile : 1;
                ULONG MappedPhysical : 1;
                ULONG DirectMapped : 1;
                ULONG Reserved : 26;
            } s;
        } u;
        SIZE_T RegionSize;
        SIZE_T CommitSize;
    } MEMORY_REGION_INFORMATION, * PMEMORY_REGION_INFORMATION;

    typedef struct _SECTION_BASIC_INFORMATION
    {
        PVOID BaseAddress;
        ULONG AllocationAttributes;
        LARGE_INTEGER MaximumSize;
    } SECTION_BASIC_INFORMATION, * PSECTION_BASIC_INFORMATION;

    typedef struct _SECTION_IMAGE_INFORMATION
    {
        PVOID TransferAddress; // Entry point
        ULONG ZeroBits;
        SIZE_T MaximumStackSize;
        SIZE_T CommittedStackSize;
        ULONG SubSystemType;
        union
        {
            struct
            {
                USHORT SubSystemMinorVersion;
                USHORT SubSystemMajorVersion;
            } s1;
            ULONG SubSystemVersion;
        } u1;
        union
        {
            struct
            {
                USHORT MajorOperatingSystemVersion;
                USHORT MinorOperatingSystemVersion;
            } s2;
            ULONG OperatingSystemVersion;
        } u2;
        USHORT ImageCharacteristics;
        USHORT DllCharacteristics;
        USHORT Machine;
        BOOLEAN ImageContainsCode;
        union
        {
            UCHAR ImageFlags;
            struct
            {
                UCHAR ComPlusNativeReady : 1;
                UCHAR ComPlusILOnly : 1;
                UCHAR ImageDynamicallyRelocated : 1;
                UCHAR ImageMappedFlat : 1;
                UCHAR BaseBelow4gb : 1;
                UCHAR ComPlusPrefer32bit : 1;
                UCHAR Reserved : 2;
            } s3;
        } u3;
        ULONG LoaderFlags;
        ULONG ImageFileSize;
        ULONG CheckSum;
    } SECTION_IMAGE_INFORMATION, * PSECTION_IMAGE_INFORMATION;

    typedef struct _SECTION_INTERNAL_IMAGE_INFORMATION
    {
        SECTION_IMAGE_INFORMATION SectionInformation;
        union
        {
            ULONG ExtendedFlags;
            struct
            {
                ULONG ImageReturnFlowGuardEnabled : 1;
                ULONG ImageReturnFlowGuardStrict : 1;
                ULONG ImageExportSuppressionEnabled : 1;
                ULONG Reserved : 29;
            } s;
        } u;
    } SECTION_INTERNAL_IMAGE_INFORMATION, * PSECTION_INTERNAL_IMAGE_INFORMATION;

    typedef struct _OBJECT_ATTRIBUTES
    {
        ULONG Length;
        HANDLE RootDirectory;
        PUNICODE_STRING ObjectName;
        ULONG Attributes;
        PVOID SecurityDescriptor;
        PVOID SecurityQualityOfService;
    } OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

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

#define LDR_GET_DLL_HANDLE_EX_UNCHANGED_REFCOUNT            0x00000001
#define LDR_GET_DLL_HANDLE_EX_PIN                           0x00000002

#define LDR_GET_PROCEDURE_ADDRESS_EX_DONT_RECORD_FORWARDER  0x00000001

#define LDR_LOCK_LOADER_LOCK_FLAG_RAISE_ON_ERRORS           0x00000001
#define LDR_LOCK_LOADER_LOCK_FLAG_TRY_ONLY                  0x00000002

#define LDR_LOCK_LOADER_LOCK_DISPOSITION_INVALID            0
#define LDR_LOCK_LOADER_LOCK_DISPOSITION_LOCK_ACQUIRED      1
#define LDR_LOCK_LOADER_LOCK_DISPOSITION_LOCK_NOT_ACQUIRED  2

#define LDR_UNLOCK_LOADER_LOCK_FLAG_RAISE_ON_ERRORS         0x00000001

    typedef struct _LDR_RESOURCE_INFO
    {
        ULONG_PTR Type;
        ULONG_PTR Name;
        ULONG_PTR Language;
    } LDR_RESOURCE_INFO, * PLDR_RESOURCE_INFO;

    typedef struct _LDR_ENUM_RESOURCE_INFO
    {
        ULONG_PTR Type;
        ULONG_PTR Name;
        ULONG_PTR Language;
        PVOID Data;
        SIZE_T Size;
        ULONG_PTR Reserved;
    } LDR_ENUM_RESOURCE_INFO, * PLDR_ENUM_RESOURCE_INFO;

#define LDR_FIND_RESOURCE_LANGUAGE_CAN_FALLBACK             0x00000000
#define LDR_FIND_RESOURCE_LANGUAGE_EXACT                    0x00000004
#define LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION         0x00000008

    typedef struct _RTL_PROCESS_MODULE_INFORMATION
    {
        HANDLE Section;
        PVOID MappedBase;
        PVOID ImageBase;
        ULONG ImageSize;
        ULONG Flags;
        USHORT LoadOrderIndex;
        USHORT InitOrderIndex;
        USHORT LoadCount;
        USHORT OffsetToFileName;
        UCHAR FullPathName[256];
    } RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

    typedef struct _RTL_PROCESS_MODULES
    {
        ULONG NumberOfModules;
        RTL_PROCESS_MODULE_INFORMATION Modules[1];
    } RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

    typedef struct _RTL_PROCESS_MODULE_INFORMATION_EX
    {
        USHORT NextOffset;
        RTL_PROCESS_MODULE_INFORMATION BaseInfo;
        ULONG ImageChecksum;
        ULONG TimeDateStamp;
        PVOID DefaultBase;
    } RTL_PROCESS_MODULE_INFORMATION_EX, * PRTL_PROCESS_MODULE_INFORMATION_EX;

    typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
    {
        USHORT UniqueProcessId;
        USHORT CreatorBackTraceIndex;
        UCHAR ObjectTypeIndex;
        UCHAR HandleAttributes;
        USHORT HandleValue;
        PVOID Object;
        ULONG GrantedAccess;
    } SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

    typedef struct _SYSTEM_HANDLE_INFORMATION
    {
        ULONG NumberOfHandles;
        SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
    } SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

    typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX
    {
        PVOID Object;
        ULONG_PTR UniqueProcessId;
        ULONG_PTR HandleValue;
        ULONG GrantedAccess;
        USHORT CreatorBackTraceIndex;
        USHORT ObjectTypeIndex;
        ULONG HandleAttributes;
        ULONG Reserved;
    } SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO_EX;

    typedef struct _SYSTEM_HANDLE_INFORMATION_EX
    {
        ULONG_PTR NumberOfHandles;
        ULONG_PTR Reserved;
        SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Handles[1];
    } SYSTEM_HANDLE_INFORMATION_EX, * PSYSTEM_HANDLE_INFORMATION_EX;

    typedef struct _OBJECT_BASIC_INFORMATION
    {
        ULONG Attributes;
        ACCESS_MASK GrantedAccess;
        ULONG HandleCount;
        ULONG PointerCount;
        ULONG PagedPoolCharge;
        ULONG NonPagedPoolCharge;
        ULONG Reserved[3];
        ULONG NameInfoSize;
        ULONG TypeInfoSize;
        ULONG SecurityDescriptorSize;
        LARGE_INTEGER CreationTime;
    } OBJECT_BASIC_INFORMATION, * POBJECT_BASIC_INFORMATION;

    typedef struct _OBJECT_NAME_INFORMATION
    {
        UNICODE_STRING Name;
    } OBJECT_NAME_INFORMATION, * POBJECT_NAME_INFORMATION;

    typedef struct _OBJECT_TYPE_INFORMATION
    {
        UNICODE_STRING TypeName;
        ULONG TotalNumberOfObjects;
        ULONG TotalNumberOfHandles;
        ULONG TotalPagedPoolUsage;
        ULONG TotalNonPagedPoolUsage;
        ULONG TotalNamePoolUsage;
        ULONG TotalHandleTableUsage;
        ULONG HighWaterNumberOfObjects;
        ULONG HighWaterNumberOfHandles;
        ULONG HighWaterPagedPoolUsage;
        ULONG HighWaterNonPagedPoolUsage;
        ULONG HighWaterNamePoolUsage;
        ULONG HighWaterHandleTableUsage;
        ULONG InvalidAttributes;
        GENERIC_MAPPING GenericMapping;
        ULONG ValidAccessMask;
        BOOLEAN SecurityRequired;
        BOOLEAN MaintainHandleCount;
        UCHAR TypeIndex; // Since Windows 8.1
        CHAR ReservedByte;
        ULONG PoolType;
        ULONG DefaultPagedPoolCharge;
        ULONG DefaultNonPagedPoolCharge;
    } OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

    typedef struct _OBJECT_TYPES_INFORMATION
    {
        ULONG NumberOfTypes;
        OBJECT_TYPE_INFORMATION TypeInformation[1];
    } OBJECT_TYPES_INFORMATION, * POBJECT_TYPES_INFORMATION;

    typedef struct _OBJECT_HANDLE_FLAG_INFORMATION
    {
        BOOLEAN Inherit;
        BOOLEAN ProtectFromClose;
    } OBJECT_HANDLE_FLAG_INFORMATION, * POBJECT_HANDLE_FLAG_INFORMATION;

    typedef struct _DBGKM_EXCEPTION
    {
        EXCEPTION_RECORD ExceptionRecord;
        ULONG FirstChance;
    } DBGKM_EXCEPTION, * PDBGKM_EXCEPTION;

    typedef struct _DBGKM_CREATE_THREAD
    {
        ULONG SubSystemKey;
        PVOID StartAddress;
    } DBGKM_CREATE_THREAD, * PDBGKM_CREATE_THREAD;

    typedef struct _DBGKM_CREATE_PROCESS
    {
        ULONG SubSystemKey;
        HANDLE FileHandle;
        PVOID BaseOfImage;
        ULONG DebugInfoFileOffset;
        ULONG DebugInfoSize;
        DBGKM_CREATE_THREAD InitialThread;
    } DBGKM_CREATE_PROCESS, * PDBGKM_CREATE_PROCESS;

    typedef struct _DBGKM_EXIT_THREAD
    {
        NTSTATUS ExitStatus;
    } DBGKM_EXIT_THREAD, * PDBGKM_EXIT_THREAD;

    typedef struct _DBGKM_EXIT_PROCESS
    {
        NTSTATUS ExitStatus;
    } DBGKM_EXIT_PROCESS, * PDBGKM_EXIT_PROCESS;

    typedef struct _DBGKM_LOAD_DLL
    {
        HANDLE FileHandle;
        PVOID BaseOfDll;
        ULONG DebugInfoFileOffset;
        ULONG DebugInfoSize;
        PVOID NamePointer;
    } DBGKM_LOAD_DLL, * PDBGKM_LOAD_DLL;

    typedef struct _DBGKM_UNLOAD_DLL
    {
        PVOID BaseAddress;
    } DBGKM_UNLOAD_DLL, * PDBGKM_UNLOAD_DLL;

    typedef enum _DBG_STATE
    {
        DbgIdle,
        DbgReplyPending,
        DbgCreateThreadStateChange,
        DbgCreateProcessStateChange,
        DbgExitThreadStateChange,
        DbgExitProcessStateChange,
        DbgExceptionStateChange,
        DbgBreakpointStateChange,
        DbgSingleStepStateChange,
        DbgLoadDllStateChange,
        DbgUnloadDllStateChange
    } DBG_STATE, * PDBG_STATE;

    typedef struct _DBGUI_CREATE_THREAD
    {
        HANDLE HandleToThread;
        DBGKM_CREATE_THREAD NewThread;
    } DBGUI_CREATE_THREAD, * PDBGUI_CREATE_THREAD;

    typedef struct _DBGUI_CREATE_PROCESS
    {
        HANDLE HandleToProcess;
        HANDLE HandleToThread;
        DBGKM_CREATE_PROCESS NewProcess;
    } DBGUI_CREATE_PROCESS, * PDBGUI_CREATE_PROCESS;

    typedef struct _DBGUI_WAIT_STATE_CHANGE
    {
        DBG_STATE NewState;
        CLIENT_ID AppClientId;
        union
        {
            DBGKM_EXCEPTION Exception;
            DBGUI_CREATE_THREAD CreateThread;
            DBGUI_CREATE_PROCESS CreateProcessInfo;
            DBGKM_EXIT_THREAD ExitThread;
            DBGKM_EXIT_PROCESS ExitProcess;
            DBGKM_LOAD_DLL LoadDll;
            DBGKM_UNLOAD_DLL UnloadDll;
        } StateInfo;
    } DBGUI_WAIT_STATE_CHANGE, * PDBGUI_WAIT_STATE_CHANGE;

    typedef struct _DBGSS_THREAD_DATA
    {
        struct _DBGSS_THREAD_DATA* Next;
        HANDLE ThreadHandle;
        HANDLE ProcessHandle;
        ULONG ProcessId;
        ULONG ThreadId;
        BOOLEAN HandleMarked;
    } DBGSS_THREAD_DATA, * PDBGSS_THREAD_DATA;

#define DbgSsSetThreadData(d) \
    NtCurrentTeb()->DbgSsReserved[0] = d

#define DbgSsGetThreadData() \
    ((PDBGSS_THREAD_DATA)NtCurrentTeb()->DbgSsReserved[0])

    typedef USHORT RTL_ATOM, * PRTL_ATOM;
    typedef long SECURITY_STATUS;

    typedef struct _RTL_SPLAY_LINKS
    {
        struct _RTL_SPLAY_LINKS* Parent;
        struct _RTL_SPLAY_LINKS* LeftChild;
        struct _RTL_SPLAY_LINKS* RightChild;
    } RTL_SPLAY_LINKS, * PRTL_SPLAY_LINKS;

#define RtlInitializeSplayLinks(Links) \
{ \
    PRTL_SPLAY_LINKS _SplayLinks; \
    _SplayLinks = (PRTL_SPLAY_LINKS)(Links); \
    _SplayLinks->Parent = _SplayLinks; \
    _SplayLinks->LeftChild = NULL; \
    _SplayLinks->RightChild = NULL; \
}

    typedef struct _PREFIX_TABLE_ENTRY
    {
        SHORT NodeTypeCode;
        SHORT NameLength;
        struct _PREFIX_TABLE_ENTRY* NextPrefixTree;
        RTL_SPLAY_LINKS Links;
        PANSI_STRING Prefix;
    } PREFIX_TABLE_ENTRY, * PPREFIX_TABLE_ENTRY;

    typedef struct _PREFIX_TABLE
    {
        SHORT NodeTypeCode;
        SHORT NameLength;
        PPREFIX_TABLE_ENTRY NextPrefixTree;
    } PREFIX_TABLE, * PPREFIX_TABLE;

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

#define DUPLICATE_CLOSE_SOURCE      0x00000001
#define DUPLICATE_SAME_ACCESS       0x00000002
#define DUPLICATE_SAME_ATTRIBUTES   0x00000004

#define RTL_WALK_MAX_STACK_DEPTH    128

    // These cannot be ORed together
#define RTL_WALK_KERNEL_MODE_STACK  0x00000000 // Kernel mode callers only
#define RTL_WALK_USER_MODE_STACK    0x00000001
#define RTL_WALK_TRACE_HANDLES      0x00000300

    typedef struct _RTL_PROCESS_VERIFIER_OPTIONS
    {
        ULONG SizeStruct;
        ULONG Option;
        UCHAR OptionData[1];
    } RTL_PROCESS_VERIFIER_OPTIONS, * PRTL_PROCESS_VERIFIER_OPTIONS;

    typedef struct _RTL_DEBUG_INFORMATION
    {
        HANDLE SectionHandleClient;
        PVOID ViewBaseClient;
        PVOID ViewBaseTarget;
        ULONG_PTR ViewBaseDelta;
        HANDLE EventPairClient;
        HANDLE EventPairTarget;
        HANDLE TargetProcessId;
        HANDLE TargetThreadHandle;
        ULONG Flags;
        SIZE_T OffsetFree;
        SIZE_T CommitSize;
        SIZE_T ViewSize;
        union
        {
            PRTL_PROCESS_MODULES Modules;
            PRTL_PROCESS_MODULE_INFORMATION_EX ModulesEx;
        };
        struct _RTL_PROCESS_BACKTRACES* BackTraces;
        struct _RTL_PROCESS_HEAPS* Heaps;
        struct _RTL_PROCESS_LOCKS* Locks;
        PVOID SpecificHeap;
        HANDLE TargetProcessHandle;
        PRTL_PROCESS_VERIFIER_OPTIONS VerifierOptions;
        PVOID ProcessHeap;
        HANDLE CriticalSectionHandle;
        HANDLE CriticalSectionOwnerThread;
        PVOID Reserved[4];
    } RTL_DEBUG_INFORMATION, * PRTL_DEBUG_INFORMATION;

    typedef
        VOID
        (*PPS_APC_ROUTINE)(
            _In_opt_ PVOID ApcArgument1,
            _In_opt_ PVOID ApcArgument2,
            _In_opt_ PVOID ApcArgument3
            );

    typedef struct _RTLP_CURDIR_REF* PRTLP_CURDIR_REF;

    typedef struct _RTL_RELATIVE_NAME_U
    {
        UNICODE_STRING RelativeName;
        HANDLE ContainingDirectory;
        PRTLP_CURDIR_REF CurDirRef;
    } RTL_RELATIVE_NAME_U, * PRTL_RELATIVE_NAME_U;

    typedef enum _RTL_PATH_TYPE
    {
        RtlPathTypeUnknown,
        RtlPathTypeUncAbsolute,
        RtlPathTypeDriveAbsolute,
        RtlPathTypeDriveRelative,
        RtlPathTypeRooted,
        RtlPathTypeRelative,
        RtlPathTypeLocalDevice,
        RtlPathTypeRootLocalDevice,
    } RTL_PATH_TYPE;

#define DOS_MAX_COMPONENT_LENGTH 255
#define DOS_MAX_PATH_LENGTH (DOS_MAX_COMPONENT_LENGTH + 5)

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

#define LDRP_PACKAGED_BINARY            0x00000001
#define LDRP_IMAGE_DLL                  0x00000004
#define LDRP_LOAD_IN_PROGRESS           0x00001000
#define LDRP_ENTRY_PROCESSED            0x00004000
#define LDRP_DONT_CALL_FOR_THREADS      0x00040000
#define LDRP_PROCESS_ATTACH_CALLED      0x00080000
#define LDRP_PROCESS_ATTACH_FAILED      0x00100000
#define LDRP_IMAGE_NOT_AT_BASE          0x00200000 // Vista and below
#define LDRP_COR_IMAGE                  0x00400000
#define LDRP_DONT_RELOCATE              0x00800000
#define LDRP_REDIRECTED                 0x10000000
#define LDRP_COMPAT_DATABASE_PROCESSED  0x80000000

#define LDR_DATA_TABLE_ENTRY_SIZE_WINXP FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, DdagNode)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN7 FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, BaseNameHashValue)
#define LDR_DATA_TABLE_ENTRY_SIZE_WIN8 FIELD_OFFSET(LDR_DATA_TABLE_ENTRY, ImplicitPathOptions)

#define RTL_BALANCED_NODE_RESERVED_PARENT_MASK 3

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

    typedef struct _INITIAL_TEB
    {
        struct
        {
            PVOID OldStackBase;
            PVOID OldStackLimit;
        } OldInitialTeb;
        PVOID StackBase;
        PVOID StackLimit;
        PVOID StackAllocationBase;
    } INITIAL_TEB, * PINITIAL_TEB;

    typedef struct _IO_STATUS_BLOCK
    {
        union
        {
            NTSTATUS Status;
            PVOID Pointer;
        };
        ULONG_PTR Information;
    } IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

    typedef
        VOID
        (NTAPI*
            PIO_APC_ROUTINE)(
                _In_ PVOID ApcContext,
                _In_ PIO_STATUS_BLOCK IoStatusBlock,
                _In_ ULONG Reserved
                );

    typedef struct _FILE_IO_COMPLETION_INFORMATION
    {
        PVOID KeyContext;
        PVOID ApcContext;
        IO_STATUS_BLOCK IoStatusBlock;
    } FILE_IO_COMPLETION_INFORMATION, * PFILE_IO_COMPLETION_INFORMATION;

    typedef struct _FILE_COMPLETION_INFORMATION
    {
        HANDLE Port;
        PVOID Key;
    } FILE_COMPLETION_INFORMATION, * PFILE_COMPLETION_INFORMATION;

#ifdef __cplusplus
    typedef enum _PRIORITY_CLASS : UCHAR
    {
        Undefined,
        Idle,
        Normal,
        High,
        Realtime,
        BelowNormal,
        AboveNormal
    } PRIORITY_CLASS;
#else
    typedef UCHAR PRIORITY_CLASS;
#endif

    typedef struct _PROCESS_PRIORITY_CLASS
    {
        BOOLEAN Foreground;
        PRIORITY_CLASS PriorityClass;
    } PROCESS_PRIORITY_CLASS, * PPROCESS_PRIORITY_CLASS;

    typedef struct _PS_ATTRIBUTE
    {
        ULONG_PTR Attribute;                // PROC_THREAD_ATTRIBUTE_XXX | PROC_THREAD_ATTRIBUTE_XXX modifiers, see ProcThreadAttributeValue macro and Windows Internals 6 (372)
        SIZE_T Size;                        // Size of Value or *ValuePtr
        union
        {
            ULONG_PTR Value;                // Reserve 8 bytes for data (such as a Handle or a data pointer)
            PVOID ValuePtr;                 // data pointer
        };
        PSIZE_T ReturnLength;               // Either 0 or specifies size of data returned to caller via "ValuePtr"
    } PS_ATTRIBUTE, * PPS_ATTRIBUTE;

    typedef struct _PS_ATTRIBUTE_LIST
    {
        SIZE_T TotalLength;                 // sizeof(PS_ATTRIBUTE_LIST)
        PS_ATTRIBUTE Attributes[2];         // Depends on how many attribute entries should be supplied to NtCreateUserProcess
    } PS_ATTRIBUTE_LIST, * PPS_ATTRIBUTE_LIST;

    typedef struct _PS_MEMORY_RESERVE
    {
        PVOID ReserveAddress;
        SIZE_T ReserveSize;
    } PS_MEMORY_RESERVE, * PPS_MEMORY_RESERVE;

#define PS_ATTRIBUTE_NUMBER_MASK    0x0000ffff
#define PS_ATTRIBUTE_THREAD         0x00010000 // Attribute may be used with thread creation
#define PS_ATTRIBUTE_INPUT          0x00020000 // Attribute is input only
#define PS_ATTRIBUTE_ADDITIVE       0x00040000 // Attribute may be "accumulated", e.g. bitmasks, counters, etc.

    typedef enum _PS_ATTRIBUTE_NUM
    {
        PsAttributeParentProcess,                   // in HANDLE
        PsAttributeDebugPort,                       // in HANDLE
        PsAttributeToken,                           // in HANDLE
        PsAttributeClientId,                        // out PCLIENT_ID
        PsAttributeTebAddress,                      // out PTEB
        PsAttributeImageName,                       // in PWSTR
        PsAttributeImageInfo,                       // out PSECTION_IMAGE_INFORMATION
        PsAttributeMemoryReserve,                   // in PPS_MEMORY_RESERVE
        PsAttributePriorityClass,                   // in UCHAR
        PsAttributeErrorMode,                       // in ULONG
        PsAttributeStdHandleInfo,                   // in PPS_STD_HANDLE_INFO
        PsAttributeHandleList,                      // in PHANDLE
        PsAttributeGroupAffinity,                   // in PGROUP_AFFINITY
        PsAttributePreferredNode,                   // in PUSHORT
        PsAttributeIdealProcessor,                  // in PPROCESSOR_NUMBER
        PsAttributeUmsThread,                       // see MSDN UpdateProceThreadAttributeList (CreateProcessW) - in PUMS_CREATE_THREAD_ATTRIBUTES
        PsAttributeMitigationOptions,               // in UCHAR
        PsAttributeProtectionLevel,                 // in ULONG
        PsAttributeSecureProcess,                   // since THRESHOLD (Virtual Secure Mode, Device Guard)
        PsAttributeJobList,
        PsAttributeChildProcessPolicy,              // since THRESHOLD2
        PsAttributeAllApplicationPackagesPolicy,    // since REDSTONE
        PsAttributeWin32kFilter,
        PsAttributeSafeOpenPromptOriginClaim,
        PsAttributeBnoIsolation,
        PsAttributeDesktopAppPolicy,
        PsAttributeMax
    } PS_ATTRIBUTE_NUM;

#define PsAttributeValue(Number, Thread, Input, Additive) \
    (((Number) & PS_ATTRIBUTE_NUMBER_MASK) | \
    ((Thread) ? PS_ATTRIBUTE_THREAD : 0) | \
    ((Input) ? PS_ATTRIBUTE_INPUT : 0) | \
    ((Additive) ? PS_ATTRIBUTE_ADDITIVE : 0))

#define PS_ATTRIBUTE_PARENT_PROCESS \
    PsAttributeValue(PsAttributeParentProcess, FALSE, TRUE, TRUE) // 0x60000
#define PS_ATTRIBUTE_DEBUG_PORT \
    PsAttributeValue(PsAttributeDebugPort, FALSE, TRUE, TRUE) // 0x60001
#define PS_ATTRIBUTE_TOKEN \
    PsAttributeValue(PsAttributeToken, FALSE, TRUE, TRUE) // 0x60002
#define PS_ATTRIBUTE_CLIENT_ID \
    PsAttributeValue(PsAttributeClientId, TRUE, FALSE, FALSE) // 0x10003
#define PS_ATTRIBUTE_TEB_ADDRESS \
    PsAttributeValue(PsAttributeTebAddress, TRUE, FALSE, FALSE) // 0x10004
#define PS_ATTRIBUTE_IMAGE_NAME \
    PsAttributeValue(PsAttributeImageName, FALSE, TRUE, FALSE) // 0x20005
#define PS_ATTRIBUTE_IMAGE_INFO \
    PsAttributeValue(PsAttributeImageInfo, FALSE, FALSE, FALSE) // 0x6
#define PS_ATTRIBUTE_MEMORY_RESERVE \
    PsAttributeValue(PsAttributeMemoryReserve, FALSE, TRUE, FALSE) // 0x20007
#define PS_ATTRIBUTE_PRIORITY_CLASS \
    PsAttributeValue(PsAttributePriorityClass, FALSE, TRUE, FALSE) // 0x20008
#define PS_ATTRIBUTE_ERROR_MODE \
    PsAttributeValue(PsAttributeErrorMode, FALSE, TRUE, FALSE) // 0x20009
#define PS_ATTRIBUTE_STD_HANDLE_INFO \
    PsAttributeValue(PsAttributeStdHandleInfo, FALSE, TRUE, FALSE) // 0x2000A
#define PS_ATTRIBUTE_HANDLE_LIST \
    PsAttributeValue(PsAttributeHandleList, FALSE, TRUE, FALSE) // 0x2000B
#define PS_ATTRIBUTE_GROUP_AFFINITY \
    PsAttributeValue(PsAttributeGroupAffinity, TRUE, TRUE, FALSE) // 0x2000C
#define PS_ATTRIBUTE_PREFERRED_NODE \
    PsAttributeValue(PsAttributePreferredNode, FALSE, TRUE, FALSE) // 0x2000D
#define PS_ATTRIBUTE_IDEAL_PROCESSOR \
    PsAttributeValue(PsAttributeIdealProcessor, TRUE, TRUE, FALSE) // 0x2000E
#define PS_ATTRIBUTE_MITIGATION_OPTIONS \
    PsAttributeValue(PsAttributeMitigationOptions, FALSE, TRUE, TRUE) // 0x60010
#define PS_ATTRIBUTE_PROTECTION_LEVEL \
    PsAttributeValue(PsAttributeProtectionLevel, FALSE, TRUE, FALSE) // 0x20011

    typedef enum _PS_STD_HANDLE_STATE
    {
        PsNeverDuplicate,
        PsRequestDuplicate, // Duplicate standard handles specified by PseudoHandleMask, and only if StdHandleSubsystemType matches the image subsystem
        PsAlwaysDuplicate, // Always duplicate standard handles
        PsMaxStdHandleStates
    } PS_STD_HANDLE_STATE;

#define HANDLE_DETACHED_PROCESS     ((HANDLE)-1)
#define HANDLE_CREATE_NEW_CONSOLE   ((HANDLE)-2)
#define HANDLE_CREATE_NO_WINDOW     ((HANDLE)-3)

#define PS_STD_INPUT_HANDLE         0x1
#define PS_STD_OUTPUT_HANDLE        0x2
#define PS_STD_ERROR_HANDLE         0x4

    typedef struct _PS_STD_HANDLE_INFO
    {
        union
        {
            ULONG Flags;
            struct
            {
                ULONG StdHandleState : 2; // PS_STD_HANDLE_STATE
                ULONG PseudoHandleMask : 3; // PS_STD_*
            } s;
        };
        ULONG StdHandleSubsystemType;
    } PS_STD_HANDLE_INFO, * PPS_STD_HANDLE_INFO;

    typedef struct _PS_BNO_ISOLATION_PARAMETERS
    {
        UNICODE_STRING IsolationPrefix;
        ULONG HandleCount;
        PVOID* Handles;
        BOOLEAN IsolationEnabled;
    } PS_BNO_ISOLATION_PARAMETERS, * PPS_BNO_ISOLATION_PARAMETERS;

    typedef enum _PS_MITIGATION_OPTION
    {
        PS_MITIGATION_OPTION_NX,
        PS_MITIGATION_OPTION_SEHOP,
        PS_MITIGATION_OPTION_FORCE_RELOCATE_IMAGES,
        PS_MITIGATION_OPTION_HEAP_TERMINATE,
        PS_MITIGATION_OPTION_BOTTOM_UP_ASLR,
        PS_MITIGATION_OPTION_HIGH_ENTROPY_ASLR,
        PS_MITIGATION_OPTION_STRICT_HANDLE_CHECKS,
        PS_MITIGATION_OPTION_WIN32K_SYSTEM_CALL_DISABLE,
        PS_MITIGATION_OPTION_EXTENSION_POINT_DISABLE,
        PS_MITIGATION_OPTION_PROHIBIT_DYNAMIC_CODE,
        PS_MITIGATION_OPTION_CONTROL_FLOW_GUARD,
        PS_MITIGATION_OPTION_BLOCK_NON_MICROSOFT_BINARIES,
        PS_MITIGATION_OPTION_FONT_DISABLE,
        PS_MITIGATION_OPTION_IMAGE_LOAD_NO_REMOTE,
        PS_MITIGATION_OPTION_IMAGE_LOAD_NO_LOW_LABEL,
        PS_MITIGATION_OPTION_IMAGE_LOAD_PREFER_SYSTEM32,
        PS_MITIGATION_OPTION_RETURN_FLOW_GUARD,
        PS_MITIGATION_OPTION_LOADER_INTEGRITY_CONTINUITY,
        PS_MITIGATION_OPTION_STRICT_CONTROL_FLOW_GUARD,
        PS_MITIGATION_OPTION_RESTRICT_SET_THREAD_CONTEXT
    } PS_MITIGATION_OPTION;

    typedef enum _PS_CREATE_STATE
    {
        PsCreateInitialState,
        PsCreateFailOnFileOpen,
        PsCreateFailOnSectionCreate,
        PsCreateFailExeFormat,
        PsCreateFailMachineMismatch,
        PsCreateFailExeName, // Debugger specified
        PsCreateSuccess,
        PsCreateMaximumStates
    } PS_CREATE_STATE;

    typedef struct _PS_CREATE_INFO
    {
        SIZE_T Size;
        PS_CREATE_STATE State;
        union
        {
            // PsCreateInitialState
            struct
            {
                union
                {
                    ULONG InitFlags;
                    struct
                    {
                        UCHAR WriteOutputOnExit : 1;
                        UCHAR DetectManifest : 1;
                        UCHAR IFEOSkipDebugger : 1;
                        UCHAR IFEODoNotPropagateKeyState : 1;
                        UCHAR SpareBits1 : 4;
                        UCHAR SpareBits2 : 8;
                        USHORT ProhibitedImageCharacteristics : 16;
                    } s1;
                } u1;
                ACCESS_MASK AdditionalFileAccess;
            } InitState;

            // PsCreateFailOnSectionCreate
            struct
            {
                HANDLE FileHandle;
            } FailSection;

            // PsCreateFailExeFormat
            struct
            {
                USHORT DllCharacteristics;
            } ExeFormat;

            // PsCreateFailExeName
            struct
            {
                HANDLE IFEOKey;
            } ExeName;

            // PsCreateSuccess
            struct
            {
                union
                {
                    ULONG OutputFlags;
                    struct
                    {
                        UCHAR ProtectedProcess : 1;
                        UCHAR AddressSpaceOverride : 1;
                        UCHAR DevOverrideEnabled : 1; // From Image File Execution Options
                        UCHAR ManifestDetected : 1;
                        UCHAR ProtectedProcessLight : 1;
                        UCHAR SpareBits1 : 3;
                        UCHAR SpareBits2 : 8;
                        USHORT SpareBits3 : 16;
                    } s2;
                } u2;
                HANDLE FileHandle;
                HANDLE SectionHandle;
                ULONGLONG UserProcessParametersNative;
                ULONG UserProcessParametersWow64;
                ULONG CurrentParameterFlags;
                ULONGLONG PebAddressNative;
                ULONG PebAddressWow64;
                ULONGLONG ManifestAddress;
                ULONG ManifestSize;
            } SuccessState;
        };
    } PS_CREATE_INFO, * PPS_CREATE_INFO;

#define PROCESS_CREATE_FLAGS_BREAKAWAY              0x00000001
#define PROCESS_CREATE_FLAGS_NO_DEBUG_INHERIT       0x00000002
#define PROCESS_CREATE_FLAGS_INHERIT_HANDLES        0x00000004
#define PROCESS_CREATE_FLAGS_OVERRIDE_ADDRESS_SPACE 0x00000008
#define PROCESS_CREATE_FLAGS_LARGE_PAGES            0x00000010

    // Only usable with NtCreateUserProcess (Vista+):
#define PROCESS_CREATE_FLAGS_LARGE_PAGE_SYSTEM_DLL  0x00000020
#define PROCESS_CREATE_FLAGS_PROTECTED_PROCESS      0x00000040 // Only allowed if the calling process is itself protected
#define PROCESS_CREATE_FLAGS_CREATE_SESSION         0x00000080
#define PROCESS_CREATE_FLAGS_INHERIT_FROM_PARENT    0x00000100

    typedef enum _MEMORY_RESERVE_TYPE
    {
        MemoryReserveUserApc,
        MemoryReserveIoCompletion,
        MemoryReserveTypeMax
    } MEMORY_RESERVE_TYPE;

    typedef struct _PROCESS_HANDLE_TRACING_ENABLE
    {
        ULONG Flags;
    } PROCESS_HANDLE_TRACING_ENABLE, * PPROCESS_HANDLE_TRACING_ENABLE;

#define PROCESS_HANDLE_TRACING_MAX_SLOTS 0x20000

    typedef struct _PROCESS_HANDLE_TRACING_ENABLE_EX
    {
        ULONG Flags;
        ULONG TotalSlots;
    } PROCESS_HANDLE_TRACING_ENABLE_EX, * PPROCESS_HANDLE_TRACING_ENABLE_EX;

    // Source: http://processhacker.sourceforge.net
    typedef enum _PROCESSINFOCLASS
    {
        ProcessBasicInformation, // q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
        ProcessQuotaLimits, // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
        ProcessIoCounters, // q: IO_COUNTERS
        ProcessVmCounters, // q: VM_COUNTERS, VM_COUNTERS_EX, VM_COUNTERS_EX2
        ProcessTimes, // q: KERNEL_USER_TIMES
        ProcessBasePriority, // s: KPRIORITY
        ProcessRaisePriority, // s: ULONG
        ProcessDebugPort, // q: HANDLE
        ProcessExceptionPort, // s: HANDLE
        ProcessAccessToken, // s: PROCESS_ACCESS_TOKEN
        ProcessLdtInformation, // qs: PROCESS_LDT_INFORMATION // 10
        ProcessLdtSize, // s: PROCESS_LDT_SIZE
        ProcessDefaultHardErrorMode, // qs: ULONG
        ProcessIoPortHandlers, // (kernel-mode only)
        ProcessPooledUsageAndLimits, // q: POOLED_USAGE_AND_LIMITS
        ProcessWorkingSetWatch, // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
        ProcessUserModeIOPL,
        ProcessEnableAlignmentFaultFixup, // s: BOOLEAN
        ProcessPriorityClass, // qs: PROCESS_PRIORITY_CLASS
        ProcessWx86Information,
        ProcessHandleCount, // q: ULONG, PROCESS_HANDLE_INFORMATION // 20
        ProcessAffinityMask, // s: KAFFINITY
        ProcessPriorityBoost, // qs: ULONG
        ProcessDeviceMap, // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
        ProcessSessionInformation, // q: PROCESS_SESSION_INFORMATION
        ProcessForegroundInformation, // s: PROCESS_FOREGROUND_BACKGROUND
        ProcessWow64Information, // q: ULONG_PTR
        ProcessImageFileName, // q: UNICODE_STRING
        ProcessLUIDDeviceMapsEnabled, // q: ULONG
        ProcessBreakOnTermination, // qs: ULONG
        ProcessDebugObjectHandle, // q: HANDLE // 30
        ProcessDebugFlags, // qs: ULONG
        ProcessHandleTracing, // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
        ProcessIoPriority, // qs: IO_PRIORITY_HINT
        ProcessExecuteFlags, // qs: ULONG
        ProcessResourceManagement,
        ProcessCookie, // q: ULONG
        ProcessImageInformation, // q: SECTION_IMAGE_INFORMATION
        ProcessCycleTime, // q: PROCESS_CYCLE_TIME_INFORMATION // since VISTA
        ProcessPagePriority, // q: ULONG
        ProcessInstrumentationCallback, // 40
        ProcessThreadStackAllocation, // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
        ProcessWorkingSetWatchEx, // q: PROCESS_WS_WATCH_INFORMATION_EX[]
        ProcessImageFileNameWin32, // q: UNICODE_STRING
        ProcessImageFileMapping, // q: HANDLE (input)
        ProcessAffinityUpdateMode, // qs: PROCESS_AFFINITY_UPDATE_MODE
        ProcessMemoryAllocationMode, // qs: PROCESS_MEMORY_ALLOCATION_MODE
        ProcessGroupInformation, // q: USHORT[]
        ProcessTokenVirtualizationEnabled, // s: ULONG
        ProcessConsoleHostProcess, // q: ULONG_PTR
        ProcessWindowInformation, // q: PROCESS_WINDOW_INFORMATION // 50
        ProcessHandleInformation, // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
        ProcessMitigationPolicy, // s: PROCESS_MITIGATION_POLICY_INFORMATION
        ProcessDynamicFunctionTableInformation,
        ProcessHandleCheckingMode,
        ProcessKeepAliveCount, // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
        ProcessRevokeFileHandles, // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
        ProcessWorkingSetControl, // s: PROCESS_WORKING_SET_CONTROL
        ProcessHandleTable, // since WINBLUE
        ProcessCheckStackExtentsMode,
        ProcessCommandLineInformation, // q: UNICODE_STRING // 60
        ProcessProtectionInformation, // q: PS_PROTECTION
        ProcessMemoryExhaustion, // PROCESS_MEMORY_EXHAUSTION_INFO // since THRESHOLD
        ProcessFaultInformation, // PROCESS_FAULT_INFORMATION
        ProcessTelemetryIdInformation, // PROCESS_TELEMETRY_ID_INFORMATION
        ProcessCommitReleaseInformation, // PROCESS_COMMIT_RELEASE_INFORMATION
        ProcessDefaultCpuSetsInformation,
        ProcessAllowedCpuSetsInformation,
        ProcessSubsystemProcess,
        ProcessJobMemoryInformation, // PROCESS_JOB_MEMORY_INFO
        ProcessInPrivate, // since THRESHOLD2 // 70
        ProcessRaiseUMExceptionOnInvalidHandleClose,
        ProcessIumChallengeResponse,
        ProcessChildProcessInformation, // PROCESS_CHILD_PROCESS_INFORMATION
        ProcessHighGraphicsPriorityInformation,
        ProcessSubsystemInformation, // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
        ProcessEnergyValues, // PROCESS_ENERGY_VALUES, PROCESS_EXTENDED_ENERGY_VALUES
        ProcessActivityThrottleState, // PROCESS_ACTIVITY_THROTTLE_STATE
        ProcessActivityThrottlePolicy, // PROCESS_ACTIVITY_THROTTLE_POLICY
        ProcessWin32kSyscallFilterInformation,
        ProcessDisableSystemAllowedCpuSets,
        ProcessWakeInformation, // PROCESS_WAKE_INFORMATION
        ProcessEnergyTrackingState, // PROCESS_ENERGY_TRACKING_STATE
        MaxProcessInfoClass
    } PROCESSINFOCLASS;

    // Source: http://processhacker.sourceforge.net
    typedef enum _SYSTEM_INFORMATION_CLASS
    {
        SystemBasicInformation, // q: SYSTEM_BASIC_INFORMATION
        SystemProcessorInformation, // q: SYSTEM_PROCESSOR_INFORMATION
        SystemPerformanceInformation, // q: SYSTEM_PERFORMANCE_INFORMATION
        SystemTimeOfDayInformation, // q: SYSTEM_TIMEOFDAY_INFORMATION
        SystemPathInformation, // not implemented
        SystemProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
        SystemCallCountInformation, // q: SYSTEM_CALL_COUNT_INFORMATION
        SystemDeviceInformation, // q: SYSTEM_DEVICE_INFORMATION
        SystemProcessorPerformanceInformation, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
        SystemFlagsInformation, // q: SYSTEM_FLAGS_INFORMATION
        SystemCallTimeInformation, // not implemented // SYSTEM_CALL_TIME_INFORMATION // 10
        SystemModuleInformation, // q: RTL_PROCESS_MODULES
        SystemLocksInformation, // q: RTL_PROCESS_LOCKS
        SystemStackTraceInformation, // q: RTL_PROCESS_BACKTRACES
        SystemPagedPoolInformation, // not implemented
        SystemNonPagedPoolInformation, // not implemented
        SystemHandleInformation, // q: SYSTEM_HANDLE_INFORMATION
        SystemObjectInformation, // q: SYSTEM_OBJECTTYPE_INFORMATION mixed with SYSTEM_OBJECT_INFORMATION
        SystemPageFileInformation, // q: SYSTEM_PAGEFILE_INFORMATION
        SystemVdmInstemulInformation, // q
        SystemVdmBopInformation, // not implemented // 20
        SystemFileCacheInformation, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemCache)
        SystemPoolTagInformation, // q: SYSTEM_POOLTAG_INFORMATION
        SystemInterruptInformation, // q: SYSTEM_INTERRUPT_INFORMATION
        SystemDpcBehaviorInformation, // q: SYSTEM_DPC_BEHAVIOR_INFORMATION; s: SYSTEM_DPC_BEHAVIOR_INFORMATION (requires SeLoadDriverPrivilege)
        SystemFullMemoryInformation, // not implemented
        SystemLoadGdiDriverInformation, // s (kernel-mode only)
        SystemUnloadGdiDriverInformation, // s (kernel-mode only)
        SystemTimeAdjustmentInformation, // q: SYSTEM_QUERY_TIME_ADJUST_INFORMATION; s: SYSTEM_SET_TIME_ADJUST_INFORMATION (requires SeSystemtimePrivilege)
        SystemSummaryMemoryInformation, // not implemented
        SystemMirrorMemoryInformation, // s (requires license value "Kernel-MemoryMirroringSupported") (requires SeShutdownPrivilege) // 30
        SystemPerformanceTraceInformation, // q; s: (type depends on EVENT_TRACE_INFORMATION_CLASS)
        SystemObsolete0, // not implemented
        SystemExceptionInformation, // q: SYSTEM_EXCEPTION_INFORMATION
        SystemCrashDumpStateInformation, // s (requires SeDebugPrivilege)
        SystemKernelDebuggerInformation, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION
        SystemContextSwitchInformation, // q: SYSTEM_CONTEXT_SWITCH_INFORMATION
        SystemRegistryQuotaInformation, // q: SYSTEM_REGISTRY_QUOTA_INFORMATION; s (requires SeIncreaseQuotaPrivilege)
        SystemExtendServiceTableInformation, // s (requires SeLoadDriverPrivilege) // loads win32k only
        SystemPrioritySeperation, // s (requires SeTcbPrivilege)
        SystemVerifierAddDriverInformation, // s (requires SeDebugPrivilege) // 40
        SystemVerifierRemoveDriverInformation, // s (requires SeDebugPrivilege)
        SystemProcessorIdleInformation, // q: SYSTEM_PROCESSOR_IDLE_INFORMATION
        SystemLegacyDriverInformation, // q: SYSTEM_LEGACY_DRIVER_INFORMATION
        SystemCurrentTimeZoneInformation, // q
        SystemLookasideInformation, // q: SYSTEM_LOOKASIDE_INFORMATION
        SystemTimeSlipNotification, // s (requires SeSystemtimePrivilege)
        SystemSessionCreate, // not implemented
        SystemSessionDetach, // not implemented
        SystemSessionInformation, // not implemented
        SystemRangeStartInformation, // q: SYSTEM_RANGE_START_INFORMATION // 50
        SystemVerifierInformation, // q: SYSTEM_VERIFIER_INFORMATION; s (requires SeDebugPrivilege)
        SystemVerifierThunkExtend, // s (kernel-mode only)
        SystemSessionProcessInformation, // q: SYSTEM_SESSION_PROCESS_INFORMATION
        SystemLoadGdiDriverInSystemSpace, // s (kernel-mode only) (same as SystemLoadGdiDriverInformation)
        SystemNumaProcessorMap, // q
        SystemPrefetcherInformation, // q: PREFETCHER_INFORMATION; s: PREFETCHER_INFORMATION // PfSnQueryPrefetcherInformation
        SystemExtendedProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
        SystemRecommendedSharedDataAlignment, // q
        SystemComPlusPackage, // q; s
        SystemNumaAvailableMemory, // 60
        SystemProcessorPowerInformation, // q: SYSTEM_PROCESSOR_POWER_INFORMATION
        SystemEmulationBasicInformation, // q
        SystemEmulationProcessorInformation,
        SystemExtendedHandleInformation, // q: SYSTEM_HANDLE_INFORMATION_EX
        SystemLostDelayedWriteInformation, // q: ULONG
        SystemBigPoolInformation, // q: SYSTEM_BIGPOOL_INFORMATION
        SystemSessionPoolTagInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION
        SystemSessionMappedViewInformation, // q: SYSTEM_SESSION_MAPPED_VIEW_INFORMATION
        SystemHotpatchInformation, // q; s
        SystemObjectSecurityMode, // q // 70
        SystemWatchdogTimerHandler, // s (kernel-mode only)
        SystemWatchdogTimerInformation, // q (kernel-mode only); s (kernel-mode only)
        SystemLogicalProcessorInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION
        SystemWow64SharedInformationObsolete, // not implemented
        SystemRegisterFirmwareTableInformationHandler, // s (kernel-mode only)
        SystemFirmwareTableInformation, // SYSTEM_FIRMWARE_TABLE_INFORMATION
        SystemModuleInformationEx, // q: RTL_PROCESS_MODULE_INFORMATION_EX
        SystemVerifierTriageInformation, // not implemented
        SystemSuperfetchInformation, // q; s: SUPERFETCH_INFORMATION // PfQuerySuperfetchInformation
        SystemMemoryListInformation, // q: SYSTEM_MEMORY_LIST_INFORMATION; s: SYSTEM_MEMORY_LIST_COMMAND (requires SeProfileSingleProcessPrivilege) // 80
        SystemFileCacheInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (same as SystemFileCacheInformation)
        SystemThreadPriorityClientIdInformation, // s: SYSTEM_THREAD_CID_PRIORITY_INFORMATION (requires SeIncreaseBasePriorityPrivilege)
        SystemProcessorIdleCycleTimeInformation, // q: SYSTEM_PROCESSOR_IDLE_CYCLE_TIME_INFORMATION[]
        SystemVerifierCancellationInformation, // not implemented // name:wow64:whNT32QuerySystemVerifierCancellationInformation
        SystemProcessorPowerInformationEx, // not implemented
        SystemRefTraceInformation, // q; s: SYSTEM_REF_TRACE_INFORMATION // ObQueryRefTraceInformation
        SystemSpecialPoolInformation, // q; s (requires SeDebugPrivilege) // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
        SystemProcessIdInformation, // q: SYSTEM_PROCESS_ID_INFORMATION
        SystemErrorPortInformation, // s (requires SeTcbPrivilege)
        SystemBootEnvironmentInformation, // q: SYSTEM_BOOT_ENVIRONMENT_INFORMATION // 90
        SystemHypervisorInformation, // q; s (kernel-mode only)
        SystemVerifierInformationEx, // q; s: SYSTEM_VERIFIER_INFORMATION_EX
        SystemTimeZoneInformation, // s (requires SeTimeZonePrivilege)
        SystemImageFileExecutionOptionsInformation, // s: SYSTEM_IMAGE_FILE_EXECUTION_OPTIONS_INFORMATION (requires SeTcbPrivilege)
        SystemCoverageInformation, // q; s // name:wow64:whNT32QuerySystemCoverageInformation; ExpCovQueryInformation
        SystemPrefetchPatchInformation, // not implemented
        SystemVerifierFaultsInformation, // s (requires SeDebugPrivilege)
        SystemSystemPartitionInformation, // q: SYSTEM_SYSTEM_PARTITION_INFORMATION
        SystemSystemDiskInformation, // q: SYSTEM_SYSTEM_DISK_INFORMATION
        SystemProcessorPerformanceDistribution, // q: SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION // 100
        SystemNumaProximityNodeInformation, // q
        SystemDynamicTimeZoneInformation, // q; s (requires SeTimeZonePrivilege)
        SystemCodeIntegrityInformation, // q: SYSTEM_CODEINTEGRITY_INFORMATION // SeCodeIntegrityQueryInformation
        SystemProcessorMicrocodeUpdateInformation, // s
        SystemProcessorBrandString, // q // HaliQuerySystemInformation -> HalpGetProcessorBrandString, info class 23
        SystemVirtualAddressInformation, // q: SYSTEM_VA_LIST_INFORMATION[]; s: SYSTEM_VA_LIST_INFORMATION[] (requires SeIncreaseQuotaPrivilege) // MmQuerySystemVaInformation
        SystemLogicalProcessorAndGroupInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX // since WIN7 // KeQueryLogicalProcessorRelationship
        SystemProcessorCycleTimeInformation, // q: SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION[]
        SystemStoreInformation, // q; s // SmQueryStoreInformation
        SystemRegistryAppendString, // s: SYSTEM_REGISTRY_APPEND_STRING_PARAMETERS // 110
        SystemAitSamplingValue, // s: ULONG (requires SeProfileSingleProcessPrivilege)
        SystemVhdBootInformation, // q: SYSTEM_VHD_BOOT_INFORMATION
        SystemCpuQuotaInformation, // q; s // PsQueryCpuQuotaInformation
        SystemNativeBasicInformation, // not implemented
        SystemSpare1, // not implemented
        SystemLowPriorityIoInformation, // q: SYSTEM_LOW_PRIORITY_IO_INFORMATION
        SystemTpmBootEntropyInformation, // q: TPM_BOOT_ENTROPY_NT_RESULT // ExQueryTpmBootEntropyInformation
        SystemVerifierCountersInformation, // q: SYSTEM_VERIFIER_COUNTERS_INFORMATION
        SystemPagedPoolInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypePagedPool)
        SystemSystemPtesInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemPtes) // 120
        SystemNodeDistanceInformation, // q
        SystemAcpiAuditInformation, // q: SYSTEM_ACPI_AUDIT_INFORMATION // HaliQuerySystemInformation -> HalpAuditQueryResults, info class 26
        SystemBasicPerformanceInformation, // q: SYSTEM_BASIC_PERFORMANCE_INFORMATION // name:wow64:whNtQuerySystemInformation_SystemBasicPerformanceInformation
        SystemQueryPerformanceCounterInformation, // q: SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION // since WIN7 SP1
        SystemSessionBigPoolInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION // since WIN8
        SystemBootGraphicsInformation, // q; s: SYSTEM_BOOT_GRAPHICS_INFORMATION (kernel-mode only)
        SystemScrubPhysicalMemoryInformation, // q; s: MEMORY_SCRUB_INFORMATION
        SystemBadPageInformation,
        SystemProcessorProfileControlArea, // q; s: SYSTEM_PROCESSOR_PROFILE_CONTROL_AREA
        SystemCombinePhysicalMemoryInformation, // s: MEMORY_COMBINE_INFORMATION, MEMORY_COMBINE_INFORMATION_EX, MEMORY_COMBINE_INFORMATION_EX2 // 130
        SystemEntropyInterruptTimingCallback,
        SystemConsoleInformation, // q: SYSTEM_CONSOLE_INFORMATION
        SystemPlatformBinaryInformation, // q: SYSTEM_PLATFORM_BINARY_INFORMATION
        SystemThrottleNotificationInformation,
        SystemHypervisorProcessorCountInformation, // q: SYSTEM_HYPERVISOR_PROCESSOR_COUNT_INFORMATION
        SystemDeviceDataInformation, // q: SYSTEM_DEVICE_DATA_INFORMATION
        SystemDeviceDataEnumerationInformation,
        SystemMemoryTopologyInformation, // q: SYSTEM_MEMORY_TOPOLOGY_INFORMATION
        SystemMemoryChannelInformation, // q: SYSTEM_MEMORY_CHANNEL_INFORMATION
        SystemBootLogoInformation, // q: SYSTEM_BOOT_LOGO_INFORMATION // 140
        SystemProcessorPerformanceInformationEx, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION_EX // since WINBLUE
        SystemSpare0,
        SystemSecureBootPolicyInformation, // q: SYSTEM_SECUREBOOT_POLICY_INFORMATION
        SystemPageFileInformationEx, // q: SYSTEM_PAGEFILE_INFORMATION_EX
        SystemSecureBootInformation, // q: SYSTEM_SECUREBOOT_INFORMATION
        SystemEntropyInterruptTimingRawInformation,
        SystemPortableWorkspaceEfiLauncherInformation, // q: SYSTEM_PORTABLE_WORKSPACE_EFI_LAUNCHER_INFORMATION
        SystemFullProcessInformation, // q: SYSTEM_PROCESS_INFORMATION with SYSTEM_PROCESS_INFORMATION_EXTENSION (requires admin)
        SystemKernelDebuggerInformationEx, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX
        SystemBootMetadataInformation, // 150
        SystemSoftRebootInformation,
        SystemElamCertificateInformation, // s: SYSTEM_ELAM_CERTIFICATE_INFORMATION
        SystemOfflineDumpConfigInformation,
        SystemProcessorFeaturesInformation, // q: SYSTEM_PROCESSOR_FEATURES_INFORMATION
        SystemRegistryReconciliationInformation,
        SystemEdidInformation,
        SystemManufacturingInformation, // q: SYSTEM_MANUFACTURING_INFORMATION // since THRESHOLD
        SystemEnergyEstimationConfigInformation, // q: SYSTEM_ENERGY_ESTIMATION_CONFIG_INFORMATION
        SystemHypervisorDetailInformation, // q: SYSTEM_HYPERVISOR_DETAIL_INFORMATION
        SystemProcessorCycleStatsInformation, // q: SYSTEM_PROCESSOR_CYCLE_STATS_INFORMATION // 160
        SystemVmGenerationCountInformation,
        SystemTrustedPlatformModuleInformation, // q: SYSTEM_TPM_INFORMATION
        SystemKernelDebuggerFlags,
        SystemCodeIntegrityPolicyInformation, // q: SYSTEM_CODEINTEGRITYPOLICY_INFORMATION
        SystemIsolatedUserModeInformation, // q: SYSTEM_ISOLATED_USER_MODE_INFORMATION
        SystemHardwareSecurityTestInterfaceResultsInformation,
        SystemSingleModuleInformation, // q: SYSTEM_SINGLE_MODULE_INFORMATION
        SystemAllowedCpuSetsInformation,
        SystemDmaProtectionInformation, // q: SYSTEM_DMA_PROTECTION_INFORMATION
        SystemInterruptCpuSetsInformation, // q: SYSTEM_INTERRUPT_CPU_SET_INFORMATION // 170
        SystemSecureBootPolicyFullInformation, // q: SYSTEM_SECUREBOOT_POLICY_FULL_INFORMATION
        SystemCodeIntegrityPolicyFullInformation,
        SystemAffinitizedInterruptProcessorInformation,
        SystemRootSiloInformation, // q: SYSTEM_ROOT_SILO_INFORMATION
        SystemCpuSetInformation, // q: SYSTEM_CPU_SET_INFORMATION // since THRESHOLD2
        SystemCpuSetTagInformation, // q: SYSTEM_CPU_SET_TAG_INFORMATION
        SystemWin32WerStartCallout,
        SystemSecureKernelProfileInformation, // q: SYSTEM_SECURE_KERNEL_HYPERGUARD_PROFILE_INFORMATION
        SystemCodeIntegrityPlatformManifestInformation, // q: SYSTEM_SECUREBOOT_PLATFORM_MANIFEST_INFORMATION // since REDSTONE
        SystemInterruptSteeringInformation, // 180
        SystemSupportedProcessorArchitectures,
        SystemMemoryUsageInformation, // q: SYSTEM_MEMORY_USAGE_INFORMATION
        SystemCodeIntegrityCertificateInformation, // q: SYSTEM_CODEINTEGRITY_CERTIFICATE_INFORMATION
        SystemPhysicalMemoryInformation, // q: SYSTEM_PHYSICAL_MEMORY_INFORMATION // since REDSTONE2
        SystemControlFlowTransition,
        SystemKernelDebuggingAllowed,
        SystemActivityModerationExeState, // SYSTEM_ACTIVITY_MODERATION_EXE_STATE
        SystemActivityModerationUserSettings, // SYSTEM_ACTIVITY_MODERATION_USER_SETTINGS
        SystemCodeIntegrityPoliciesFullInformation,
        SystemCodeIntegrityUnlockInformation, // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION // 190
        SystemIntegrityQuotaInformation,
        SystemFlushInformation, // q: SYSTEM_FLUSH_INFORMATION
        MaxSystemInfoClass
    } SYSTEM_INFORMATION_CLASS;

    typedef enum _OBJECT_INFORMATION_CLASS
    {
        ObjectBasicInformation, // OBJECT_BASIC_INFORMATION
        ObjectNameInformation, // OBJECT_NAME_INFORMATION
        ObjectTypeInformation, // OBJECT_TYPE_INFORMATION
        ObjectTypesInformation, // OBJECT_TYPES_INFORMATION
        ObjectHandleFlagInformation, // OBJECT_HANDLE_FLAG_INFORMATION
        ObjectSessionInformation,
        ObjectSessionObjectInformation,
        MaxObjectInfoClass
    } OBJECT_INFORMATION_CLASS;

    //Source: http://processhacker.sourceforge.net
    typedef enum _THREADINFOCLASS
    {
        ThreadBasicInformation, // q: THREAD_BASIC_INFORMATION
        ThreadTimes, // q: KERNEL_USER_TIMES
        ThreadPriority, // s: KPRIORITY
        ThreadBasePriority, // s: LONG
        ThreadAffinityMask, // s: KAFFINITY
        ThreadImpersonationToken, // s: HANDLE
        ThreadDescriptorTableEntry, // q: DESCRIPTOR_TABLE_ENTRY (or WOW64_DESCRIPTOR_TABLE_ENTRY)
        ThreadEnableAlignmentFaultFixup, // s: BOOLEAN
        ThreadEventPair,
        ThreadQuerySetWin32StartAddress, // q: PVOID
        ThreadZeroTlsCell, // 10
        ThreadPerformanceCount, // q: LARGE_INTEGER
        ThreadAmILastThread, // q: ULONG
        ThreadIdealProcessor, // s: ULONG
        ThreadPriorityBoost, // qs: ULONG
        ThreadSetTlsArrayAddress,
        ThreadIsIoPending, // q: ULONG
        ThreadHideFromDebugger, // s: void
        ThreadBreakOnTermination, // qs: ULONG
        ThreadSwitchLegacyState,
        ThreadIsTerminated, // q: ULONG // 20
        ThreadLastSystemCall, // q: THREAD_LAST_SYSCALL_INFORMATION
        ThreadIoPriority, // qs: IO_PRIORITY_HINT
        ThreadCycleTime, // q: THREAD_CYCLE_TIME_INFORMATION
        ThreadPagePriority, // q: ULONG
        ThreadActualBasePriority,
        ThreadTebInformation, // q: THREAD_TEB_INFORMATION (requires THREAD_GET_CONTEXT + THREAD_SET_CONTEXT)
        ThreadCSwitchMon,
        ThreadCSwitchPmu,
        ThreadWow64Context, // q: WOW64_CONTEXT
        ThreadGroupInformation, // q: GROUP_AFFINITY // 30
        ThreadUmsInformation, // q: THREAD_UMS_INFORMATION
        ThreadCounterProfiling,
        ThreadIdealProcessorEx, // q: PROCESSOR_NUMBER
        ThreadCpuAccountingInformation, // since WIN8
        ThreadSuspendCount, // since WINBLUE
        ThreadHeterogeneousCpuPolicy, // q: KHETERO_CPU_POLICY // since THRESHOLD
        ThreadContainerId, // q: GUID
        ThreadNameInformation, // qs: THREAD_NAME_INFORMATION
        ThreadSelectedCpuSets,
        ThreadSystemThreadInformation, // q: SYSTEM_THREAD_INFORMATION // 40
        ThreadActualGroupAffinity, // since THRESHOLD2
        ThreadDynamicCodePolicyInfo,
        ThreadExplicitCaseSensitivity,
        ThreadWorkOnBehalfTicket,
        ThreadSubsystemInformation, // q: SUBSYSTEM_INFORMATION_TYPE // since REDSTONE2
        ThreadDbgkWerReportActive,
        ThreadAttachContainer,
        MaxThreadInfoClass
    } THREADINFOCLASS;

    typedef enum _FSINFOCLASS
    {
        FileFsVolumeInformation = 1,    // FILE_FS_VOLUME_INFORMATION
        FileFsLabelInformation = 2,    // FILE_FS_LABEL_INFORMATION
        FileFsSizeInformation = 3,    // FILE_FS_SIZE_INFORMATION
        FileFsDeviceInformation = 4,    // FILE_FS_DEVICE_INFORMATION
        FileFsAttributeInformation = 5,    // FILE_FS_ATTRIBUTE_INFORMATION
        FileFsControlInformation = 6,    // FILE_FS_CONTROL_INFORMATION
        FileFsFullSizeInformation = 7,    // FILE_FS_FULL_SIZE_INFORMATION
        FileFsObjectIdInformation = 8,    // FILE_FS_OBJECTID_INFORMATION
        FileFsDriverPathInformation = 9,    // FILE_FS_DRIVER_PATH_INFORMATION
        FileFsVolumeFlagsInformation = 10,   // FILE_FS_VOLUME_FLAGS_INFORMATION
        FileFsSectorSizeInformation = 11,   // FILE_FS_SECTOR_SIZE_INFORMATION // since WIN8
        FileFsDataCopyInformation = 12,   // FILE_FS_DATA_COPY_INFORMATION
        FileFsMetadataSizeInformation = 13,   // FILE_FS_METADATA_SIZE_INFORMATION // since THRESHOLD
        FileFsMaximumInformation
    } FS_INFORMATION_CLASS, * PFS_INFORMATION_CLASS;

    typedef enum _MEMORY_INFORMATION_CLASS
    {
        MemoryBasicInformation, // MEMORY_BASIC_INFORMATION
        MemoryWorkingSetInformation, // MEMORY_WORKING_SET_INFORMATION
        MemoryMappedFilenameInformation, // UNICODE_STRING
        MemoryRegionInformation, // MEMORY_REGION_INFORMATION
        MemoryWorkingSetExInformation, // MEMORY_WORKING_SET_EX_INFORMATION
        MemorySharedCommitInformation, // MEMORY_SHARED_COMMIT_INFORMATION
        MemoryImageInformation, // MEMORY_IMAGE_INFORMATION
        MemoryRegionInformationEx,
        MemoryPrivilegedBasicInformation
    } MEMORY_INFORMATION_CLASS;

    typedef enum _SECTION_INFORMATION_CLASS
    {
        SectionBasicInformation,
        SectionImageInformation,
        SectionRelocationInformation, // name:wow64:whNtQuerySection_SectionRelocationInformation
        SectionOriginalBaseInformation, // PVOID BaseAddress
        SectionInternalImageInformation, // SECTION_INTERNAL_IMAGE_INFORMATION // since REDSTONE2
        MaxSectionInfoClass
    } SECTION_INFORMATION_CLASS;

    // Boot condition flags (NtInitializeRegistry)
#define REG_INIT_BOOT_SM                0x0000
#define REG_INIT_BOOT_SETUP             0x0001
#define REG_INIT_BOOT_ACCEPTED_BASE     0x0002
#define REG_INIT_BOOT_ACCEPTED_MAX      REG_INIT_BOOT_ACCEPTED_BASE + 999

#define REG_MAX_KEY_VALUE_NAME_LENGTH   32767
#define REG_MAX_KEY_NAME_LENGTH         512

    typedef enum _KEY_INFORMATION_CLASS
    {
        KeyBasicInformation, // KEY_BASIC_INFORMATION
        KeyNodeInformation, // KEY_NODE_INFORMATION
        KeyFullInformation, // KEY_FULL_INFORMATION
        KeyNameInformation, // KEY_NAME_INFORMATION
        KeyCachedInformation, // KEY_CACHED_INFORMATION
        KeyFlagsInformation, // KEY_FLAGS_INFORMATION
        KeyVirtualizationInformation, // KEY_VIRTUALIZATION_INFORMATION
        KeyHandleTagsInformation, // KEY_HANDLE_TAGS_INFORMATION
        KeyTrustInformation, // KEY_TRUST_INFORMATION
        KeyLayerInformation, // KEY_LAYER_INFORMATION
        MaxKeyInfoClass
    } KEY_INFORMATION_CLASS;

    typedef struct _KEY_BASIC_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
        ULONG TitleIndex;
        ULONG NameLength;
        WCHAR Name[1];
    } KEY_BASIC_INFORMATION, * PKEY_BASIC_INFORMATION;

    typedef struct _KEY_NODE_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
        ULONG TitleIndex;
        ULONG ClassOffset;
        ULONG ClassLength;
        ULONG NameLength;
        WCHAR Name[1];
        // ...
        // WCHAR Class[1];
    } KEY_NODE_INFORMATION, * PKEY_NODE_INFORMATION;

    typedef struct _KEY_FULL_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
        ULONG TitleIndex;
        ULONG ClassOffset;
        ULONG ClassLength;
        ULONG SubKeys;
        ULONG MaxNameLen;
        ULONG MaxClassLen;
        ULONG Values;
        ULONG MaxValueNameLen;
        ULONG MaxValueDataLen;
        WCHAR Class[1];
    } KEY_FULL_INFORMATION, * PKEY_FULL_INFORMATION;

    typedef struct _KEY_NAME_INFORMATION
    {
        ULONG NameLength;
        WCHAR Name[1];
    } KEY_NAME_INFORMATION, * PKEY_NAME_INFORMATION;

    typedef struct _KEY_CACHED_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
        ULONG TitleIndex;
        ULONG SubKeys;
        ULONG MaxNameLen;
        ULONG Values;
        ULONG MaxValueNameLen;
        ULONG MaxValueDataLen;
        ULONG NameLength;
        WCHAR Name[1];
    } KEY_CACHED_INFORMATION, * PKEY_CACHED_INFORMATION;

    typedef struct _KEY_FLAGS_INFORMATION
    {
        ULONG UserFlags;
    } KEY_FLAGS_INFORMATION, * PKEY_FLAGS_INFORMATION;

    typedef struct _KEY_VIRTUALIZATION_INFORMATION
    {
        ULONG VirtualizationCandidate : 1; // Tells whether the key is part of the virtualization namespace scope (only HKLM\Software for now).
        ULONG VirtualizationEnabled : 1; // Tells whether virtualization is enabled on this key. Can be 1 only if above flag is 1.
        ULONG VirtualTarget : 1; // Tells if the key is a virtual key. Can be 1 only if above 2 are 0. Valid only on the virtual store key handles.
        ULONG VirtualStore : 1; // Tells if the key is a part of the virtual store path. Valid only on the virtual store key handles.
        ULONG VirtualSource : 1; // Tells if the key has ever been virtualized, can be 1 only if VirtualizationCandidate is 1.
        ULONG Reserved : 27;
    } KEY_VIRTUALIZATION_INFORMATION, * PKEY_VIRTUALIZATION_INFORMATION;

    // private
    typedef struct _KEY_TRUST_INFORMATION
    {
        ULONG TrustedKey : 1;
        ULONG Reserved : 31;
    } KEY_TRUST_INFORMATION, * PKEY_TRUST_INFORMATION;

    // private
    typedef struct _KEY_LAYER_INFORMATION
    {
        ULONG IsTombstone;
        ULONG IsSupersedeLocal;
        ULONG IsSupersedeTree;
        ULONG ClassIsInherited;
        ULONG Reserved;
    } KEY_LAYER_INFORMATION, * PKEY_LAYER_INFORMATION;

    typedef enum _KEY_SET_INFORMATION_CLASS
    {
        KeyWriteTimeInformation, // KEY_WRITE_TIME_INFORMATION
        KeyWow64FlagsInformation, // KEY_WOW64_FLAGS_INFORMATION
        KeyControlFlagsInformation, // KEY_CONTROL_FLAGS_INFORMATION
        KeySetVirtualizationInformation, // KEY_SET_VIRTUALIZATION_INFORMATION
        KeySetDebugInformation,
        KeySetHandleTagsInformation, // KEY_HANDLE_TAGS_INFORMATION
        MaxKeySetInfoClass
    } KEY_SET_INFORMATION_CLASS;

    typedef struct _KEY_WRITE_TIME_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
    } KEY_WRITE_TIME_INFORMATION, * PKEY_WRITE_TIME_INFORMATION;

    typedef struct _KEY_WOW64_FLAGS_INFORMATION
    {
        ULONG UserFlags;
    } KEY_WOW64_FLAGS_INFORMATION, * PKEY_WOW64_FLAGS_INFORMATION;

    typedef struct _KEY_HANDLE_TAGS_INFORMATION
    {
        ULONG HandleTags;
    } KEY_HANDLE_TAGS_INFORMATION, * PKEY_HANDLE_TAGS_INFORMATION;

    typedef struct _KEY_CONTROL_FLAGS_INFORMATION
    {
        ULONG ControlFlags;
    } KEY_CONTROL_FLAGS_INFORMATION, * PKEY_CONTROL_FLAGS_INFORMATION;

    typedef struct _KEY_SET_VIRTUALIZATION_INFORMATION
    {
        ULONG VirtualTarget : 1;
        ULONG VirtualStore : 1;
        ULONG VirtualSource : 1; // true if key has been virtualized at least once
        ULONG Reserved : 29;
    } KEY_SET_VIRTUALIZATION_INFORMATION, * PKEY_SET_VIRTUALIZATION_INFORMATION;

    typedef enum _KEY_VALUE_INFORMATION_CLASS
    {
        KeyValueBasicInformation, // KEY_VALUE_BASIC_INFORMATION
        KeyValueFullInformation, // KEY_VALUE_FULL_INFORMATION
        KeyValuePartialInformation, // KEY_VALUE_PARTIAL_INFORMATION
        KeyValueFullInformationAlign64,
        KeyValuePartialInformationAlign64, // KEY_VALUE_PARTIAL_INFORMATION_ALIGN64
        KeyValueLayerInformation, // KEY_VALUE_LAYER_INFORMATION
        MaxKeyValueInfoClass
    } KEY_VALUE_INFORMATION_CLASS;

    typedef struct _KEY_VALUE_BASIC_INFORMATION
    {
        ULONG TitleIndex;
        ULONG Type;
        ULONG NameLength;
        WCHAR Name[1];
    } KEY_VALUE_BASIC_INFORMATION, * PKEY_VALUE_BASIC_INFORMATION;

    typedef struct _KEY_VALUE_FULL_INFORMATION
    {
        ULONG TitleIndex;
        ULONG Type;
        ULONG DataOffset;
        ULONG DataLength;
        ULONG NameLength;
        WCHAR Name[1];
        // ...
        // UCHAR Data[1];
    } KEY_VALUE_FULL_INFORMATION, * PKEY_VALUE_FULL_INFORMATION;

    typedef struct _KEY_VALUE_PARTIAL_INFORMATION
    {
        ULONG TitleIndex;
        ULONG Type;
        ULONG DataLength;
        UCHAR Data[1];
    } KEY_VALUE_PARTIAL_INFORMATION, * PKEY_VALUE_PARTIAL_INFORMATION;

    typedef struct _KEY_VALUE_PARTIAL_INFORMATION_ALIGN64
    {
        ULONG Type;
        ULONG DataLength;
        UCHAR Data[1];
    } KEY_VALUE_PARTIAL_INFORMATION_ALIGN64, * PKEY_VALUE_PARTIAL_INFORMATION_ALIGN64;

    // private
    typedef struct _KEY_VALUE_LAYER_INFORMATION
    {
        ULONG IsTombstone;
        ULONG Reserved;
    } KEY_VALUE_LAYER_INFORMATION, * PKEY_VALUE_LAYER_INFORMATION;

    typedef struct _KEY_VALUE_ENTRY
    {
        PUNICODE_STRING ValueName;
        ULONG DataLength;
        ULONG DataOffset;
        ULONG Type;
    } KEY_VALUE_ENTRY, * PKEY_VALUE_ENTRY;

    typedef enum _REG_ACTION
    {
        KeyAdded,
        KeyRemoved,
        KeyModified
    } REG_ACTION;

    typedef struct _REG_NOTIFY_INFORMATION
    {
        ULONG NextEntryOffset;
        REG_ACTION Action;
        ULONG KeyLength;
        WCHAR Key[1];
    } REG_NOTIFY_INFORMATION, * PREG_NOTIFY_INFORMATION;

    typedef struct _KEY_PID_ARRAY
    {
        HANDLE PID;
        UNICODE_STRING KeyName;
    } KEY_PID_ARRAY, * PKEY_PID_ARRAY;

    typedef struct _KEY_OPEN_SUBKEYS_INFORMATION
    {
        ULONG Count;
        KEY_PID_ARRAY KeyArray[1];
    } KEY_OPEN_SUBKEYS_INFORMATION, * PKEY_OPEN_SUBKEYS_INFORMATION;

    typedef enum _SYSDBG_COMMAND
    {
        SysDbgQueryModuleInformation,
        SysDbgQueryTraceInformation,
        SysDbgSetTracepoint,
        SysDbgSetSpecialCall,
        SysDbgClearSpecialCalls,
        SysDbgQuerySpecialCalls,
        SysDbgBreakPoint,
        SysDbgQueryVersion,
        SysDbgReadVirtual,
        SysDbgWriteVirtual,
        SysDbgReadPhysical,
        SysDbgWritePhysical,
        SysDbgReadControlSpace,
        SysDbgWriteControlSpace,
        SysDbgReadIoSpace,
        SysDbgWriteIoSpace,
        SysDbgReadMsr,
        SysDbgWriteMsr,
        SysDbgReadBusData,
        SysDbgWriteBusData,
        SysDbgCheckLowMemory,
        SysDbgEnableKernelDebugger,
        SysDbgDisableKernelDebugger,
        SysDbgGetAutoKdEnable,
        SysDbgSetAutoKdEnable,
        SysDbgGetPrintBufferSize,
        SysDbgSetPrintBufferSize,
        SysDbgGetKdUmExceptionEnable,
        SysDbgSetKdUmExceptionEnable,
        SysDbgGetTriageDump,
        SysDbgGetKdBlockEnable,
        SysDbgSetKdBlockEnable,
        SysDbgRegisterForUmBreakInfo,
        SysDbgGetUmBreakPid,
        SysDbgClearUmBreakPid,
        SysDbgGetUmAttachPid,
        SysDbgClearUmAttachPid,
        SysDbgGetLiveKernelDump
    } SYSDBG_COMMAND, * PSYSDBG_COMMAND;

    typedef enum _DEBUGOBJECTINFOCLASS
    {
        DebugObjectFlags = 1,
        MaxDebugObjectInfoClass
    } DEBUGOBJECTINFOCLASS, * PDEBUGOBJECTINFOCLASS;

    //Source: http://processhacker.sourceforge.net
    typedef enum _FILE_INFORMATION_CLASS
    {
        FileDirectoryInformation = 1, // FILE_DIRECTORY_INFORMATION
        FileFullDirectoryInformation, // FILE_FULL_DIR_INFORMATION
        FileBothDirectoryInformation, // FILE_BOTH_DIR_INFORMATION
        FileBasicInformation, // FILE_BASIC_INFORMATION
        FileStandardInformation, // FILE_STANDARD_INFORMATION
        FileInternalInformation, // FILE_INTERNAL_INFORMATION
        FileEaInformation, // FILE_EA_INFORMATION
        FileAccessInformation, // FILE_ACCESS_INFORMATION
        FileNameInformation, // FILE_NAME_INFORMATION
        FileRenameInformation, // FILE_RENAME_INFORMATION // 10
        FileLinkInformation, // FILE_LINK_INFORMATION
        FileNamesInformation, // FILE_NAMES_INFORMATION
        FileDispositionInformation, // FILE_DISPOSITION_INFORMATION
        FilePositionInformation, // FILE_POSITION_INFORMATION
        FileFullEaInformation, // FILE_FULL_EA_INFORMATION
        FileModeInformation, // FILE_MODE_INFORMATION
        FileAlignmentInformation, // FILE_ALIGNMENT_INFORMATION
        FileAllInformation, // FILE_ALL_INFORMATION
        FileAllocationInformation, // FILE_ALLOCATION_INFORMATION
        FileEndOfFileInformation, // FILE_END_OF_FILE_INFORMATION // 20
        FileAlternateNameInformation, // FILE_NAME_INFORMATION
        FileStreamInformation, // FILE_STREAM_INFORMATION
        FilePipeInformation, // FILE_PIPE_INFORMATION
        FilePipeLocalInformation, // FILE_PIPE_LOCAL_INFORMATION
        FilePipeRemoteInformation, // FILE_PIPE_REMOTE_INFORMATION
        FileMailslotQueryInformation, // FILE_MAILSLOT_QUERY_INFORMATION
        FileMailslotSetInformation, // FILE_MAILSLOT_SET_INFORMATION
        FileCompressionInformation, // FILE_COMPRESSION_INFORMATION
        FileObjectIdInformation, // FILE_OBJECTID_INFORMATION
        FileCompletionInformation, // FILE_COMPLETION_INFORMATION // 30
        FileMoveClusterInformation, // FILE_MOVE_CLUSTER_INFORMATION
        FileQuotaInformation, // FILE_QUOTA_INFORMATION
        FileReparsePointInformation, // FILE_REPARSE_POINT_INFORMATION
        FileNetworkOpenInformation, // FILE_NETWORK_OPEN_INFORMATION
        FileAttributeTagInformation, // FILE_ATTRIBUTE_TAG_INFORMATION
        FileTrackingInformation, // FILE_TRACKING_INFORMATION
        FileIdBothDirectoryInformation, // FILE_ID_BOTH_DIR_INFORMATION
        FileIdFullDirectoryInformation, // FILE_ID_FULL_DIR_INFORMATION
        FileValidDataLengthInformation, // FILE_VALID_DATA_LENGTH_INFORMATION
        FileShortNameInformation, // FILE_NAME_INFORMATION // 40
        FileIoCompletionNotificationInformation, // FILE_IO_COMPLETION_NOTIFICATION_INFORMATION // since VISTA
        FileIoStatusBlockRangeInformation, // FILE_IOSTATUSBLOCK_RANGE_INFORMATION
        FileIoPriorityHintInformation, // FILE_IO_PRIORITY_HINT_INFORMATION
        FileSfioReserveInformation, // FILE_SFIO_RESERVE_INFORMATION
        FileSfioVolumeInformation, // FILE_SFIO_VOLUME_INFORMATION
        FileHardLinkInformation, // FILE_LINKS_INFORMATION
        FileProcessIdsUsingFileInformation, // FILE_PROCESS_IDS_USING_FILE_INFORMATION
        FileNormalizedNameInformation, // FILE_NAME_INFORMATION
        FileNetworkPhysicalNameInformation, // FILE_NETWORK_PHYSICAL_NAME_INFORMATION
        FileIdGlobalTxDirectoryInformation, // FILE_ID_GLOBAL_TX_DIR_INFORMATION // since WIN7 // 50
        FileIsRemoteDeviceInformation, // FILE_IS_REMOTE_DEVICE_INFORMATION
        FileUnusedInformation,
        FileNumaNodeInformation, // FILE_NUMA_NODE_INFORMATION
        FileStandardLinkInformation, // FILE_STANDARD_LINK_INFORMATION
        FileRemoteProtocolInformation, // FILE_REMOTE_PROTOCOL_INFORMATION
        FileRenameInformationBypassAccessCheck, // (kernel-mode only); FILE_RENAME_INFORMATION // since WIN8
        FileLinkInformationBypassAccessCheck, // (kernel-mode only); FILE_LINK_INFORMATION
        FileVolumeNameInformation, // FILE_VOLUME_NAME_INFORMATION
        FileIdInformation, // FILE_ID_INFORMATION
        FileIdExtdDirectoryInformation, // FILE_ID_EXTD_DIR_INFORMATION
        FileReplaceCompletionInformation, // FILE_COMPLETION_INFORMATION // since WINBLUE
        FileHardLinkFullIdInformation, // FILE_LINK_ENTRY_FULL_ID_INFORMATION
        FileIdExtdBothDirectoryInformation, // FILE_ID_EXTD_BOTH_DIR_INFORMATION // since THRESHOLD
        FileDispositionInformationEx, // FILE_DISPOSITION_INFO_EX // since REDSTONE
        FileRenameInformationEx,
        FileRenameInformationExBypassAccessCheck,
        FileDesiredStorageClassInformation, // FILE_DESIRED_STORAGE_CLASS_INFORMATION // since REDSTONE2
        FileStatInformation, // FILE_STAT_INFORMATION
        FileMaximumInformation
    } FILE_INFORMATION_CLASS, * PFILE_INFORMATION_CLASS;

    typedef struct _SYSTEM_BASIC_INFORMATION
    {
        ULONG Reserved;
        ULONG TimerResolution;
        ULONG PageSize;
        ULONG NumberOfPhysicalPages;
        ULONG LowestPhysicalPageNumber;
        ULONG HighestPhysicalPageNumber;
        ULONG AllocationGranularity;
        ULONG_PTR MinimumUserModeAddress;
        ULONG_PTR MaximumUserModeAddress;
        ULONG_PTR ActiveProcessorsAffinityMask;
        CCHAR NumberOfProcessors;
    } SYSTEM_BASIC_INFORMATION, * PSYSTEM_BASIC_INFORMATION;

    typedef struct _FILE_PIPE_PEEK_BUFFER
    {
        ULONG NamedPipeState;
        ULONG ReadDataAvailable;
        ULONG NumberOfMessages;
        ULONG MessageLength;
        CHAR Data[1];
    } FILE_PIPE_PEEK_BUFFER, * PFILE_PIPE_PEEK_BUFFER;

    typedef struct _NAMED_PIPE_CREATE_PARAMETERS
    {
        ULONG NamedPipeType;
        ULONG ReadMode;
        ULONG CompletionMode;
        ULONG MaximumInstances;
        ULONG InboundQuota;
        ULONG OutboundQuota;
        LARGE_INTEGER DefaultTimeout;
        BOOLEAN TimeoutSpecified;
    } NAMED_PIPE_CREATE_PARAMETERS, * PNAMED_PIPE_CREATE_PARAMETERS;

    typedef struct _FILE_NETWORK_OPEN_INFORMATION
    {
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        LARGE_INTEGER AllocationSize;
        LARGE_INTEGER EndOfFile;
        ULONG FileAttributes;
    } FILE_NETWORK_OPEN_INFORMATION, * PFILE_NETWORK_OPEN_INFORMATION;

    typedef struct _SYSTEM_TIMEOFDAY_INFORMATION
    {
        LARGE_INTEGER BootTime;
        LARGE_INTEGER CurrentTime;
        LARGE_INTEGER TimeZoneBias;
        ULONG TimeZoneId;
        ULONG Reserved;
        ULONGLONG BootTimeBias;
        ULONGLONG SleepTimeBias;
    } SYSTEM_TIMEOFDAY_INFORMATION, * PSYSTEM_TIMEOFDAY_INFORMATION;

    typedef struct _SYSTEM_CONSOLE_INFORMATION
    {
        ULONG DriverLoaded : 1;
        ULONG Spare : 31;
    } SYSTEM_CONSOLE_INFORMATION, * PSYSTEM_CONSOLE_INFORMATION;

    typedef struct _KSYSTEM_TIME
    {
        ULONG LowPart;
        LONG High1Time;
        LONG High2Time;
    } KSYSTEM_TIME, * PKSYSTEM_TIME;

    typedef struct _PROCESS_ACCESS_TOKEN
    {
        HANDLE Token; // Needs TOKEN_ASSIGN_PRIMARY access
        HANDLE Thread; // Handle to initial/only thread; needs THREAD_QUERY_INFORMATION access
    } PROCESS_ACCESS_TOKEN, * PPROCESS_ACCESS_TOKEN;

#ifdef __cplusplus
    typedef enum _PS_PROTECTED_TYPE : UCHAR
    {
        PsProtectedTypeNone,
        PsProtectedTypeProtectedLight,
        PsProtectedTypeProtected,
        PsProtectedTypeMax
    } PS_PROTECTED_TYPE;
#else
    typedef UCHAR PS_PROTECTED_TYPE;
#endif

#ifdef __cplusplus
    typedef enum _PS_PROTECTED_SIGNER : UCHAR
    {
        PsProtectedSignerNone,
        PsProtectedSignerAuthenticode,
        PsProtectedSignerCodeGen,
        PsProtectedSignerAntimalware,
        PsProtectedSignerLsa,
        PsProtectedSignerWindows,
        PsProtectedSignerWinTcb,
        PsProtectedSignerWinSystem,
        PsProtectedSignerApp,
        PsProtectedSignerMax
    } PS_PROTECTED_SIGNER;
#else
    typedef UCHAR PS_PROTECTED_SIGNER;
#endif

    typedef struct _PS_PROTECTION
    {
        union
        {
            struct
            {
                PS_PROTECTED_TYPE Type : 3;
                BOOLEAN Audit : 1;
                PS_PROTECTED_SIGNER Signer : 4;
            } s;
            UCHAR Level;
        };
    } PS_PROTECTION, * PPS_PROTECTION;

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

    typedef struct _RTL_USER_PROCESS_INFORMATION
    {
        ULONG Length;
        HANDLE Process;
        HANDLE Thread;
        CLIENT_ID ClientId;
        SECTION_IMAGE_INFORMATION ImageInformation;
    } RTL_USER_PROCESS_INFORMATION, * PRTL_USER_PROCESS_INFORMATION;

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

#define GDI_BATCH_BUFFER_SIZE 310

    typedef struct _GDI_TEB_BATCH
    {
        ULONG Offset;
        ULONG_PTR HDC;
        ULONG Buffer[GDI_BATCH_BUFFER_SIZE];
    } GDI_TEB_BATCH, * PGDI_TEB_BATCH;

    typedef struct _TEB_ACTIVE_FRAME_CONTEXT
    {
        ULONG Flags;
        PSTR FrameName;
    } TEB_ACTIVE_FRAME_CONTEXT, * PTEB_ACTIVE_FRAME_CONTEXT;

    typedef struct _TEB_ACTIVE_FRAME
    {
        ULONG Flags;
        struct _TEB_ACTIVE_FRAME* Previous;
        PTEB_ACTIVE_FRAME_CONTEXT Context;
    } TEB_ACTIVE_FRAME, * PTEB_ACTIVE_FRAME;

    typedef struct _TEB
    {
        NT_TIB NtTib;

        PVOID EnvironmentPointer;
        CLIENT_ID ClientId;
        PVOID ActiveRpcHandle;
        PVOID ThreadLocalStoragePointer;
        PPEB ProcessEnvironmentBlock;

        ULONG LastErrorValue;
        ULONG CountOfOwnedCriticalSections;
        PVOID CsrClientThread;
        PVOID Win32ThreadInfo;
        ULONG User32Reserved[26];
        ULONG UserReserved[5];
        PVOID WOW32Reserved;
        LCID CurrentLocale;
        ULONG FpSoftwareStatusRegister;
        PVOID ReservedForDebuggerInstrumentation[16];
#ifdef _WIN64
        PVOID SystemReserved1[30];
#else
        PVOID SystemReserved1[26];
#endif
        CHAR PlaceholderCompatibilityMode;
        CHAR PlaceholderReserved[11];
        ULONG ProxiedProcessId;
        ACTIVATION_CONTEXT_STACK ActivationStack;

        UCHAR WorkingOnBehalfTicket[8];
        NTSTATUS ExceptionCode;

        PACTIVATION_CONTEXT_STACK ActivationContextStackPointer;
        ULONG_PTR InstrumentationCallbackSp;
        ULONG_PTR InstrumentationCallbackPreviousPc;
        ULONG_PTR InstrumentationCallbackPreviousSp;
#ifdef _WIN64
        ULONG TxFsContext;
#endif
        BOOLEAN InstrumentationCallbackDisabled;
#ifndef _WIN64
        UCHAR SpareBytes[23];
        ULONG TxFsContext;
#endif
        GDI_TEB_BATCH GdiTebBatch;
        CLIENT_ID RealClientId;
        HANDLE GdiCachedProcessHandle;
        ULONG GdiClientPID;
        ULONG GdiClientTID;
        PVOID GdiThreadLocalInfo;
        ULONG_PTR Win32ClientInfo[62];
        PVOID glDispatchTable[233];
        ULONG_PTR glReserved1[29];
        PVOID glReserved2;
        PVOID glSectionInfo;
        PVOID glSection;
        PVOID glTable;
        PVOID glCurrentRC;
        PVOID glContext;

        NTSTATUS LastStatusValue;
        UNICODE_STRING StaticUnicodeString;
        WCHAR StaticUnicodeBuffer[261];

        PVOID DeallocationStack;
        PVOID TlsSlots[64];
        LIST_ENTRY TlsLinks;

        PVOID Vdm;
        PVOID ReservedForNtRpc;
        PVOID DbgSsReserved[2];

        ULONG HardErrorMode;
#ifdef _WIN64
        PVOID Instrumentation[11];
#else
        PVOID Instrumentation[9];
#endif
        GUID ActivityId;

        PVOID SubProcessTag;
        PVOID PerflibData;
        PVOID EtwTraceData;
        PVOID WinSockData;
        ULONG GdiBatchCount;

        union
        {
            PROCESSOR_NUMBER CurrentIdealProcessor;
            ULONG IdealProcessorValue;
            struct
            {
                UCHAR ReservedPad0;
                UCHAR ReservedPad1;
                UCHAR ReservedPad2;
                UCHAR IdealProcessor;
            } s1;
        } u1;

        ULONG GuaranteedStackBytes;
        PVOID ReservedForPerf;
        PVOID ReservedForOle;
        ULONG WaitingOnLoaderLock;
        PVOID SavedPriorityState;
        ULONG_PTR ReservedForCodeCoverage;
        PVOID ThreadPoolData;
        PVOID* TlsExpansionSlots;
#ifdef _WIN64
        PVOID DeallocationBStore;
        PVOID BStoreLimit;
#endif
        ULONG MuiGeneration;
        ULONG IsImpersonating;
        PVOID NlsCache;
        PVOID pShimData;
        USHORT HeapVirtualAffinity;
        USHORT LowFragHeapDataSlot;
        HANDLE CurrentTransactionHandle;
        PTEB_ACTIVE_FRAME ActiveFrame;
        PVOID FlsData;

        PVOID PreferredLanguages;
        PVOID UserPrefLanguages;
        PVOID MergedPrefLanguages;
        ULONG MuiImpersonation;

        union
        {
            USHORT CrossTebFlags;
            USHORT SpareCrossTebBits : 16;
        } u2;
        union
        {
            USHORT SameTebFlags;
            struct
            {
                USHORT SafeThunkCall : 1;
                USHORT InDebugPrint : 1;
                USHORT HasFiberData : 1;
                USHORT SkipThreadAttach : 1;
                USHORT WerInShipAssertCode : 1;
                USHORT RanProcessInit : 1;
                USHORT ClonedThread : 1;
                USHORT SuppressDebugMsg : 1;
                USHORT DisableUserStackWalk : 1;
                USHORT RtlExceptionAttached : 1;
                USHORT InitialThread : 1;
                USHORT SessionAware : 1;
                USHORT LoadOwner : 1;
                USHORT LoaderWorker : 1;
                USHORT SkipLoaderInit : 1;
                USHORT SpareSameTebBits : 1;
            } s2;
        } u3;

        PVOID TxnScopeEnterCallback;
        PVOID TxnScopeExitCallback;
        PVOID TxnScopeContext;
        ULONG LockCount;
        LONG WowTebOffset;
        PVOID ResourceRetValue;
        PVOID ReservedForWdf;
        ULONGLONG ReservedForCrt;
        GUID EffectiveContainerId;
    } TEB, * PTEB;

    typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE
    {
        StandardDesign,
        NEC98x86,
        EndAlternatives
    } ALTERNATIVE_ARCHITECTURE_TYPE;

#define PROCESSOR_FEATURE_MAX       64

#define MAX_WOW64_SHARED_ENTRIES    16

#define NX_SUPPORT_POLICY_ALWAYSOFF 0
#define NX_SUPPORT_POLICY_ALWAYSON  1
#define NX_SUPPORT_POLICY_OPTIN     2
#define NX_SUPPORT_POLICY_OPTOUT    3

#pragma pack(push, 4)
    typedef struct _KUSER_SHARED_DATA
    {
        ULONG TickCountLowDeprecated;
        ULONG TickCountMultiplier;

        volatile KSYSTEM_TIME InterruptTime;
        volatile KSYSTEM_TIME SystemTime;
        volatile KSYSTEM_TIME TimeZoneBias;

        USHORT ImageNumberLow;
        USHORT ImageNumberHigh;

        WCHAR NtSystemRoot[260];

        ULONG MaxStackTraceDepth;

        ULONG CryptoExponent;

        ULONG TimeZoneId;
        ULONG LargePageMinimum;
        ULONG AitSamplingValue;
        ULONG AppCompatFlag;
        ULONGLONG RNGSeedVersion;
        ULONG GlobalValidationRunlevel;
        LONG TimeZoneBiasStamp;

        ULONG NtBuildNumber;
        ULONG NtProductType;
        BOOLEAN ProductTypeIsValid;
        UCHAR Reserved0[1];
        USHORT NativeProcessorArchitecture;

        ULONG NtMajorVersion;
        ULONG NtMinorVersion;

        BOOLEAN ProcessorFeatures[PROCESSOR_FEATURE_MAX];

        ULONG Reserved1;
        ULONG Reserved3;

        volatile ULONG TimeSlip;

        ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;
        ULONG BootId;

        LARGE_INTEGER SystemExpirationDate;

        ULONG SuiteMask;

        BOOLEAN KdDebuggerEnabled;
        union
        {
            UCHAR MitigationPolicies;
            struct
            {
                UCHAR NXSupportPolicy : 2;
                UCHAR SEHValidationPolicy : 2;
                UCHAR CurDirDevicesSkippedForDlls : 2;
                UCHAR Reserved : 2;
            } s1;
        } u1;
        UCHAR Reserved6[2];

        volatile ULONG ActiveConsoleId;

        volatile ULONG DismountCount;

        ULONG ComPlusPackage;

        ULONG LastSystemRITEventTickCount;

        ULONG NumberOfPhysicalPages;

        BOOLEAN SafeBootMode;
        UCHAR VirtualizationFlags;
        UCHAR Reserved12[2];

        union
        {
            ULONG SharedDataFlags;
            struct
            {
                ULONG DbgErrorPortPresent : 1;
                ULONG DbgElevationEnabled : 1;
                ULONG DbgVirtEnabled : 1;
                ULONG DbgInstallerDetectEnabled : 1;
                ULONG DbgLkgEnabled : 1;
                ULONG DbgDynProcessorEnabled : 1;
                ULONG DbgConsoleBrokerEnabled : 1;
                ULONG DbgSecureBootEnabled : 1;
                ULONG DbgMultiSessionSku : 1;
                ULONG DbgMultiUsersInSessionSku : 1;
                ULONG SpareBits : 22;
            } s2;
        } u2;
        ULONG DataFlagsPad[1];

        ULONGLONG TestRetInstruction;
        LONGLONG QpcFrequency;
        ULONG SystemCall;
        ULONG SystemCallPad0;
        ULONGLONG SystemCallPad[2];

        union
        {
            volatile KSYSTEM_TIME TickCount;
            volatile ULONG64 TickCountQuad;
            ULONG ReservedTickCountOverlay[3];
        };
        ULONG TickCountPad[1];

        ULONG Cookie;
        ULONG CookiePad[1];

        LONGLONG ConsoleSessionForegroundProcessId;
        ULONGLONG TimeUpdateLock;
        ULONGLONG BaselineSystemTimeQpc;
        ULONGLONG BaselineInterruptTimeQpc;
        ULONGLONG QpcSystemTimeIncrement;
        ULONGLONG QpcInterruptTimeIncrement;
        UCHAR QpcSystemTimeIncrementShift;
        UCHAR QpcInterruptTimeIncrementShift;

        USHORT UnparkedProcessorCount;
        ULONG EnclaveFeatureMask[4];
        ULONG Reserved8;
        USHORT UserModeGlobalLogger[16];
        ULONG ImageFileExecutionOptions;

        ULONG LangGenerationCount;
        ULONGLONG Reserved4;
        volatile ULONG64 InterruptTimeBias;
        volatile ULONG64 QpcBias;

        ULONG ActiveProcessorCount;
        volatile UCHAR ActiveGroupCount;
        UCHAR Reserved9;
        union
        {
            USHORT QpcData;
            struct
            {
                UCHAR QpcBypassEnabled : 1;
                UCHAR QpcShift : 1;
            } s3;
        } u3;

        LARGE_INTEGER TimeZoneBiasEffectiveStart;
        LARGE_INTEGER TimeZoneBiasEffectiveEnd;
        XSTATE_CONFIGURATION XState;
    } KUSER_SHARED_DATA, * PKUSER_SHARED_DATA;
#pragma pack(pop)

#ifdef __cplusplus
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, TickCountMultiplier) == 0x4, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, InterruptTime) == 0x8, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, SystemTime) == 0x14, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, TimeZoneBias) == 0x20, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, ImageNumberLow) == 0x2c, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, ImageNumberHigh) == 0x2e, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, NtSystemRoot) == 0x30, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, MaxStackTraceDepth) == 0x238, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, CryptoExponent) == 0x23c, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, TimeZoneId) == 0x240, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, LargePageMinimum) == 0x244, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, NtProductType) == 0x264, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, ProductTypeIsValid) == 0x268, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, NtMajorVersion) == 0x26c, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, NtMinorVersion) == 0x270, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, ProcessorFeatures) == 0x274, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, Reserved1) == 0x2b4, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, Reserved3) == 0x2b8, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, TimeSlip) == 0x2bc, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, AlternativeArchitecture) == 0x2c0, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, SystemExpirationDate) == 0x2c8, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, SuiteMask) == 0x2d0, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, KdDebuggerEnabled) == 0x2d4, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, ActiveConsoleId) == 0x2d8, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, DismountCount) == 0x2dc, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, ComPlusPackage) == 0x2e0, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, LastSystemRITEventTickCount) == 0x2e4, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, NumberOfPhysicalPages) == 0x2e8, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, SafeBootMode) == 0x2ec, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, TestRetInstruction) == 0x2f8, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, SystemCallPad) == 0x310, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, TickCount) == 0x320, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, TickCountQuad) == 0x320, "Offset check");
    static_assert(FIELD_OFFSET(KUSER_SHARED_DATA, XState) == 0x3d8, "Offset check");
#endif

#if !defined(_KERNEL_MODE) && !defined(KERNELMODE)
#define USER_SHARED_DATA            0x7FFE0000
#define SharedUserData              ((KUSER_SHARED_DATA * const)USER_SHARED_DATA)
#else
#if defined(_M_IX86)
#define KI_USER_SHARED_DATA     0xFFDF0000
#elif defined (_M_AMD64)
#define KI_USER_SHARED_DATA     0xFFFFF78000000000Ui64
#elif defined (_M_ARM)
#define KI_USER_SHARED_DATA     0xFFFF9000
#elif defined(M_ARM64)
#define KI_USER_SHARED_DATA     0xFFFFF78000000000Ui64
#endif
#define SharedUserData              ((KUSER_SHARED_DATA * const)KI_USER_SHARED_DATA)
#endif

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