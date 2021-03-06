// includes
#include <Kernel/Process.h>
#include <Kernel/Tasks/FinalizerTask.h>

namespace Kernel {

void FinalizerTask::spawn()
{
    RefPtr<Thread> finalizer_thread;
    Process::create_kernel_process(
        finalizer_thread, "FinalizerTask", [](void*) {
            Thread::current()->set_priority(THREAD_PRIORITY_LOW);
            for (;;) {
                g_finalizer_wait_queue->wait_forever("FinalizerTask");

                if (g_finalizer_has_work.exchange(false, AK::MemoryOrder::memory_order_acq_rel) == true)
                    Thread::finalize_dying_threads();
            }
        },
        nullptr);
    g_finalizer = finalizer_thread;
}

}