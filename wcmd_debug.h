/*
 * Minimal Wine debug compatibility helpers for standalone builds.
 *
 * Prefer the real wine/debug.h when available; otherwise provide no-op
 * tracing macros so the sources still compile with plain mingw.
 */
#pragma once

#if defined(__has_include)
#  if __has_include(<wine/debug.h>)
#    define WCMD_HAVE_WINE_DEBUG 1
#  endif
#elif defined(__WINE__) || defined(WINE)
#  define WCMD_HAVE_WINE_DEBUG 1
#endif

#ifdef WCMD_HAVE_WINE_DEBUG
#include <wine/debug.h>
#else
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

#define WINE_DEFAULT_DEBUG_CHANNEL(x)
#define WINE_DECLARE_DEBUG_CHANNEL(x)

#ifndef TRACE
#define TRACE(...) do {} while (0)
#endif
#ifndef WINE_TRACE
#define WINE_TRACE(...) do {} while (0)
#endif
#ifndef WINE_WARN
#define WINE_WARN(...) fprintf(stderr, __VA_ARGS__)
#endif
#ifndef WINE_FIXME
#define WINE_FIXME(...) fprintf(stderr, __VA_ARGS__)
#endif
#ifndef WINE_ERR
#define WINE_ERR(...) fprintf(stderr, __VA_ARGS__)
#endif
#ifndef ERR
#define ERR(...) WINE_ERR(__VA_ARGS__)
#endif
#ifndef WARN
#define WARN(...) WINE_WARN(__VA_ARGS__)
#endif
#ifndef FIXME
#define FIXME(...) WINE_FIXME(__VA_ARGS__)
#endif

static inline const char *wine_dbgstr_a(const char *str)
{
    return str ? str : "<null>";
}

static inline const char *wine_dbgstr_w(const WCHAR *str)
{
    static char buffer[256];
    int len;

    if (!str) return "<null>";
    len = wcstombs(buffer, str, sizeof(buffer) - 1);
    if (len < 0) return "<bad-wstr>";
    buffer[len] = '\0';
    return buffer;
}

static inline const char *wine_dbg_sprintf(const char *fmt, ...)
{
    static char buffer[256];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    return buffer;
}

#ifndef debugstr_w
#define debugstr_w(str) wine_dbgstr_w(str)
#endif
#endif /* WCMD_HAVE_WINE_DEBUG */
