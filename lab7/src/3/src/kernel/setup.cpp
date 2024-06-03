#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"
#include "memory.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;
// 内存管理器
MemoryManager memoryManager;
Semaphore lock;
void second_thread(void *arg){
    //Test case: reallocate
    lock.P();
    int kernelStart = memoryManager.allocatePhysicalPages(AddressPoolType::KERNEL, 1);
    printf("reallocate test pass!\n");
    lock.V();
}
void third_thread(void* arg){
    lock.P();
    int kernelStart = memoryManager.allocatePhysicalPages(AddressPoolType::KERNEL, 1);
    lock.V();
}
void first_thread(void *arg)
{
    // 第1个线程不可以返回
    // stdio.moveCursor(0);
    // for (int i = 0; i < 25 * 80; ++i)
    // {
    //     stdio.print(' ');
    // }
    // stdio.moveCursor(0);
    // Test case: Allocate 10 pages from the kernel pool
    int kernelStart = memoryManager.allocatePhysicalPages(AddressPoolType::KERNEL, 10);
    if (kernelStart == 0){
        printf("Kernel allocation wrong!!!\n");
    }
    else{
        printf("Kernel allocation success!!!\n");
        //printf();
    }

    // Test case: Allocate 20 pages from the user pool
    int userStart = memoryManager.allocatePhysicalPages(AddressPoolType::USER, 20);
    if (userStart == 0){
        printf("User allocation wrong!!!\n");
    }
    else{
        printf("User allocation success!!!\n");
    }

    // Test case: Release the 10 pages from the kernel pool
    memoryManager.releasePhysicalPages(AddressPoolType::KERNEL, kernelStart, 10);
    printf("Kernel pages released.\n" );

    // Test case: Release the 20 pages from the user pool
    memoryManager.releasePhysicalPages(AddressPoolType::USER, userStart, 20);
    printf("User pages released.\n");
    lock.initialize(1);
    programManager.executeThread(second_thread,nullptr,"second_thread",1);
    programManager.executeThread(third_thread,nullptr,"third_thread",1);
    asm_halt();
}

extern "C" void setup_kernel()
{

    // 中断管理器
    interruptManager.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler);

    // 输出管理器
    stdio.initialize();

    // 进程/线程管理器
    programManager.initialize();

    // 内存管理器
    memoryManager.initialize();

    // 创建第一个线程
    int pid = programManager.executeThread(first_thread, nullptr, "first thread", 1);
    if (pid == -1)
    {
        printf("can not execute thread\n");
        asm_halt();
    }

    ListItem *item = programManager.readyPrograms.front();
    PCB *firstThread = ListItem2PCB(item, tagInGeneralList);
    firstThread->status = RUNNING;
    programManager.readyPrograms.pop_front();
    programManager.running = firstThread;
    asm_switch_thread(0, firstThread);

    asm_halt();
}
