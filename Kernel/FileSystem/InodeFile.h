#pragma once

// includes
#include <Kernel/FileSystem/File.h>

namespace Kernel {

class Inode;

class InodeFile final : public File {
public:
    static NonnullRefPtr<InodeFile> create(NonnullRefPtr<Inode>&& inode)
    {
        return adopt_ref(*new InodeFile(move(inode)));
    }

    virtual ~InodeFile() override;

    const Inode& inode() const { return *m_inode; }
    Inode& inode() { return *m_inode; }

    virtual bool can_read(const FileDescription&, size_t) const override { return true; }
    virtual bool can_write(const FileDescription&, size_t) const override { return true; }

    virtual KResultOr<size_t> read(FileDescription&, u64, UserOrKernelBuffer&, size_t) override;
    virtual KResultOr<size_t> write(FileDescription&, u64, const UserOrKernelBuffer&, size_t) override;
    virtual int ioctl(FileDescription&, unsigned request, FlatPtr arg) override;
    virtual KResultOr<Region*> mmap(Process&, FileDescription&, const Range&, u64 offset, int prot, bool shared) override;

    virtual String absolute_path(const FileDescription&) const override;

    virtual KResult truncate(u64) override;
    virtual KResult chown(FileDescription&, uid_t, gid_t) override;
    virtual KResult chmod(FileDescription&, mode_t) override;

    virtual const char* class_name() const override { return "InodeFile"; }

    virtual bool is_seekable() const override { return true; }
    virtual bool is_inode() const override { return true; }

private:
    explicit InodeFile(NonnullRefPtr<Inode>&&);
    NonnullRefPtr<Inode> m_inode;
};

}
