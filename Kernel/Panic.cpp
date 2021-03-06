// includes
#include <AK/Format.h>
#include <Kernel/Arch/x86/CPU.h>
#include <Kernel/KSyms.h>
#include <Kernel/Panic.h>

namespace Kernel {

void __panic(const char* file, unsigned int line, const char* function)
{
    dmesgln("at {}:{} in {}", file, line, function);
    dump_backtrace();
    Processor::halt();
}

}
