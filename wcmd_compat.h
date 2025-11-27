/*
 * Standalone build compatibility helpers for APIs missing outside Wine.
 */
#pragma once

#include <windows.h>
#include <winternl.h>

#ifndef VerifyConsoleIoHandle
static inline BOOL VerifyConsoleIoHandle(HANDLE h)
{
    return h && h != INVALID_HANDLE_VALUE;
}
#endif

#ifndef RtlGetVersion
static inline LONG wcmd_RtlGetVersion(RTL_OSVERSIONINFOW *info)
{
    info->dwOSVersionInfoSize = sizeof(*info);
    return GetVersionExW((LPOSVERSIONINFOW)info) ? 0 : GetLastError();
}
#define RtlGetVersion(info) wcmd_RtlGetVersion((RTL_OSVERSIONINFOW*)(info))
#endif
