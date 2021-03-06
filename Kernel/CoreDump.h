#pragma once

#include <AK/LexicalPath.h>
#include <AK/NonnullRefPtr.h>
#include <AK/OwnPtr.h>
#include <Kernel/Forward.h>
#include <LibELF/exec_elf.h>

namespace Kernel {

class Process;

class CoreDump {
public:
    static OwnPtr<CoreDump> create(NonnullRefPtr<Process>, const String& output_path);

    ~CoreDump() = default;
    [[nodiscard]] KResult write();

private:
    CoreDump(NonnullRefPtr<Process>, NonnullRefPtr<FileDescription>&&);
    static RefPtr<FileDescription> create_target_file(const Process&, const String& output_path);

    [[nodiscard]] KResult write_elf_header();
    [[nodiscard]] KResult write_program_headers(size_t notes_size);
    [[nodiscard]] KResult write_regions();
    [[nodiscard]] KResult write_notes_segment(ByteBuffer&);

    ByteBuffer create_notes_segment_data() const;
    ByteBuffer create_notes_process_data() const;
    ByteBuffer create_notes_threads_data() const;
    ByteBuffer create_notes_regions_data() const;
    ByteBuffer create_notes_metadata_data() const;

    NonnullRefPtr<Process> m_process;
    NonnullRefPtr<FileDescription> m_fd;
    const size_t m_num_program_headers;
};

}
