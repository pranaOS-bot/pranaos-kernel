#pragma once

// includes
#include <AK/Types.h>
#include <Kernel/KBuffer.h>
#include <Kernel/Lock.h>
#include <Kernel/Thread.h>
#include <Kernel/UserOrKernelBuffer.h>

namespace Kernel {

class DoubleBuffer {
public:
    explicit DoubleBuffer(size_t capacity = 65536);

    [[nodiscard]] ssize_t write(const UserOrKernelBuffer&, size_t);
    [[nodiscard]] ssize_t write(const u8* data, size_t size)
    {
        return write(UserOrKernelBuffer::for_kernel_buffer(const_cast<u8*>(data)), size);
    }
    [[nodiscard]] ssize_t read(UserOrKernelBuffer&, size_t);
    [[nodiscard]] ssize_t read(u8* data, size_t size)
    {
        auto buffer = UserOrKernelBuffer::for_kernel_buffer(data);
        return read(buffer, size);
    }

    bool is_empty() const { return m_empty; }

    size_t space_for_writing() const { return m_space_for_writing; }

    void set_unblock_callback(Function<void()> callback)
    {
        VERIFY(!m_unblock_callback);
        m_unblock_callback = move(callback);
    }

private:
    void flip();
    void compute_lockfree_metadata();

    struct InnerBuffer {
        u8* data { nullptr };
        size_t size;
    };

    InnerBuffer* m_write_buffer { nullptr };
    InnerBuffer* m_read_buffer { nullptr };
    InnerBuffer m_buffer1;
    InnerBuffer m_buffer2;

    KBuffer m_storage;
    Function<void()> m_unblock_callback;
    size_t m_capacity { 0 };
    size_t m_read_buffer_index { 0 };
    size_t m_space_for_writing { 0 };
    bool m_empty { true };
    mutable Lock m_lock { "DoubleBuffer" };
};

}
