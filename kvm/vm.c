#include <err.h>
#include <fcntl.h>
#include <linux/kvm.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define KVM_DEVICE ("/dev/kvm")
#define KVM_VERSION (12)
#define VM_MEMORY (16 * 1024)
#define VM_INITIAL_GUEST_PHYS_ADDR (0x1000)

#define CODE_BIN ("./code.bin")

int setup_kvm() {
  // open kvm
  int kvm = open(KVM_DEVICE, O_RDWR | O_CLOEXEC);
  if (kvm == -1) {
    // TODO: errno handling here
    errx(1, "open %s", KVM_DEVICE);
  }

  // ensure KVM_CAP_USER_MEM
  {
    int ret = ioctl(kvm, KVM_CHECK_EXTENSION, KVM_CAP_USER_MEMORY);
    if (!ret) {
      errx(1, "KVM_CAP_USER_MEM");
    }
  }
  return kvm;
}

struct kvm_run* allocate_kvm_run(int kvm, int vcpu_fd) {
  int mmap_size = ioctl(kvm, KVM_GET_VCPU_MMAP_SIZE, NULL);
  if (mmap_size == -1) {
    err(1, "KVM_GET_VCPU_MMAP_SIZE");
  }
  return mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpu_fd, 0);
}

void* allocate_vm_memory(int vm_fd, size_t size) {
  int page_size = getpagesize();

  if (size == 0 || size % page_size != 0) {
    errx(1, "size must be a multiple of %d, received %lu", page_size, size);
  }

  // note that PROT_EXEC is missing
  void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  struct kvm_userspace_memory_region region = {
      .slot = 0,
      .guest_phys_addr = VM_INITIAL_GUEST_PHYS_ADDR,
      .memory_size = size,
      .userspace_addr = (uint64_t)mem,
  };
  ioctl(vm_fd, KVM_SET_USER_MEMORY_REGION, &region);

  return mem;
}

void load_program_code(void* mem) {
  int code_bin_fd = open(CODE_BIN, O_RDONLY);
  if (code_bin_fd == -1) {
    // TODO: errno handling here
    errx(1, "open %s", CODE_BIN);
  }

  struct stat stat_result;
  fstat(code_bin_fd, &stat_result);

  size_t size = stat_result.st_size;
  void* mapped = mmap(0, size, PROT_READ, MAP_PRIVATE, code_bin_fd, 0);

  // copy the data from the file to the memory
  memcpy(mem, mapped, size);

  munmap(mapped, size);
  close(code_bin_fd);
}

int main() {
  int kvm = setup_kvm();
  int vm_fd = ioctl(kvm, KVM_CREATE_VM, (unsigned long)0);
  int vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, (unsigned long)0);

  struct kvm_run* run = allocate_kvm_run(kvm, vcpu_fd);
  void* vm_mem = allocate_vm_memory(vm_fd, VM_MEMORY);
  load_program_code(vm_mem);

  struct kvm_sregs sregs;
  struct kvm_regs regs;

  // setup sregs
  ioctl(vcpu_fd, KVM_GET_SREGS, &sregs);
  sregs.cs.base = 0;
  sregs.cs.selector = 0;
  ioctl(vcpu_fd, KVM_SET_SREGS, &sregs);

  ioctl(vcpu_fd, KVM_GET_REGS, &regs);
  // setup instruction pointer
  regs.rip = VM_INITIAL_GUEST_PHYS_ADDR;
  // setup stack pointer
  regs.rsp = VM_INITIAL_GUEST_PHYS_ADDR + VM_MEMORY;
  ioctl(vcpu_fd, KVM_SET_REGS, &regs);

  // run
  while (1) {
    ioctl(vcpu_fd, KVM_RUN, NULL);
    // load regs after every step
    ioctl(vcpu_fd, KVM_GET_SREGS, &sregs);
    ioctl(vcpu_fd, KVM_GET_REGS, &regs);
    switch (run->exit_reason) {
      case KVM_EXIT_MMIO:
        printf("KVM_EXIT_MMIO, ");
        printf("rip %p, ", (void*)regs.rip);
        printf("at %p, ", (void*)run->mmio.phys_addr);
        if (run->mmio.is_write) {
          printf("write\n");
        } else {
          printf("read\n");
        }
      case KVM_EXIT_HLT:
        puts("KVM_EXIT_HLT");
        return 0;
        /* Handle exit */
    }
  }
}
