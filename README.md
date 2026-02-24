# MIT 6.S081 xv6 Labs Implementation

![Status](https://img.shields.io/badge/status-in%20progress-yellow)
![Language](https://img.shields.io/badge/language-C-blue)
![Platform](https://img.shields.io/badge/platform-RISC--V-orange)

This repository contains my solutions for the [MIT 6.S081: Operating System Engineering](https://pdos.csail.mit.edu/6.828/2020/schedule.html) labs. The code is based on **xv6-riscv**.

本仓库包含了我对 MIT 6.S081 操作系统工程课程 Labs 的实现代码。基于 xv6-riscv 架构。

## 🚀 Progress Overview (进度概览)

Current Progress: **Lab 0 - Lab 4 Completed**

| Lab | Topic | Description & Key Features | Status |
| :--- | :--- | :--- | :---: |
| **Lab 0** | **Unix Utilities** | Implemented user-space utilities (`sleep`, `pingpong`, `primes`, `find`, `xargs`) using system calls. <br> 熟悉系统调用接口、管道(pipes)和文件描述符。 | ✅ |
| **Lab 1** | **System Calls** | Added `trace` (syscall tracing) and `sysinfo` (system statistics) system calls. <br> 了解内核态与用户态边界、参数传递。 | ✅ |
| **Lab 2** | **Page Tables** | Implemented custom page table mappings. Speed up syscalls and print page table structure. <br> 深入理解三级页表、虚拟内存映射机制。 | ✅ |
| **Lab 3** | **Traps** | Handled assembly traps, backtrace, and periodic alerts (`sigalarm`). <br> 掌握 RISC-V 汇编、栈帧结构(Stack Frame)及中断处理流程。 | ✅ |
| **Lab 4** | **Lazy Allocation** / **COW** | Implemented Lazy Page Allocation / Copy-On-Write Fork. <br> 实现了内存懒加载 / 写时复制机制，优化内存使用。 | ✅ |

## 🛠 Environment Setup (环境配置)

To run this project, you need a Linux environment (Ubuntu 20.04+ recommended) with the RISC-V toolchain and QEMU installed.</br>
You will need a RISC-V "newlib" tool chain from https://github.com/riscv/riscv-gnu-toolchain, and qemu compiled for riscv64-softmmu.</br>
Once they are installed, and in your shell search path, you can run "make qemu".

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
```
```bash
# MacOS
brew tap riscv-software-src/riscv
brew install riscv-tools
PATH=$PATH:/opt/homebrew/Celler/riscv-gnu-toolchain/main/bin
brew install qemu
```
此处的PATH可能需要进行修改，我的实验在WSL和MacOS上都是进行了的

### 原来仓库的代码中存在一些问题
由于makefile中配置的设置是将所有的警告都视为保存，因此如果不修改这个直接clone官方仓库可能就跑不了了</br>
```bash
git clone git://g.csail.mit.edu/xv6-labs-2022
cd xv6-labs-2022
git status
make qemu
```
经过我的测试2022版本需要修改user/usertest:rwsbrk()函数的内容：</br>
1. void rwsbrk(char *arg) 添加新的参数
2. **后面第一行写(void)arg;这样就可以忽略警告了**
