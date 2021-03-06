#pragma once

#define __STRINGIFY_HELPER(x) #x
#define __STRINGIFY(x) __STRINGIFY_HELPER(x)

#ifdef DEBUG
[[noreturn]] void __assertion_failed(const char* msg, const char* file, unsigned line, const char* func);
#    define VERIFY(expr) (static_cast<bool>(expr) ? void(0) : __assertion_failed(#    expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#    define VERIFY_NOT_REACHED() VERIFY(false)
#else
#    define VERIFY(expr)
#    define VERIFY_NOT_REACHED() CRASH()
#endif
#define CRASH()              \
    do {                     \
        asm volatile("ud2"); \
    } while (0)

#define VERIFY_INTERRUPTS_DISABLED() VERIFY(!(cpu_flags() & 0x200))
#define VERIFY_INTERRUPTS_ENABLED() VERIFY(cpu_flags() & 0x200)
#define TODO VERIFY_NOT_REACHED
