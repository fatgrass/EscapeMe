#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>
#include "kvm_handler.h"
#include "vm.h"
#include "utils/gmalloc.h"

int kvm_handle_io(struct vm *vm, struct vcpu *vcpu){
	/*
	int vcpufd = vcpu->fd;
	struct kvm_run *run = vcpu->run;
	 */
	return 0;
}

int kvm_handle_hypercall(struct vm *vm, struct vcpu *vcpu){
	struct kvm_regs regs;
	int vcpufd = vcpu->fd;
	unsigned long ret = -1;

	if(ioctl(vcpufd, KVM_GET_REGS, &regs)){
		perror("ioctl KVM_GET_REGS");
		return -1;
	}
	unsigned nr = regs.rax;
	unsigned long arg[] = {regs.rbx, regs.rcx, regs.rdx, regs.rsi};

	switch(nr){
		case 0x10:		// read(0, buf, size)
			if(check_addr(vm, arg[0]))
				ret = read(STDIN_FILENO, guest2phys(vm, arg[0]), arg[1]);
			break;
		case 0x11:		// write(1, buf, size)
			if(check_addr(vm, arg[0]))
				ret = write(STDOUT_FILENO, guest2phys(vm, arg[0]), arg[1]);
			break;
		case 0x20:
			ret = get_gmem_remain();
			break;
		case 0x21:		// gmalloc(addr, size)
			ret = gmalloc(arg[0], arg[1]);
			break;
		case 0x22:		// gfree(addr);
			ret = gfree(arg[0]);
			break;
	}

	regs.rax  = ret;
	regs.rip += 3;

	if(ioctl(vcpufd, KVM_SET_REGS, &regs) < 0){
		perror("ioctl KVM_SET_REGS");
		return -1;
	}

	return 0;
}
