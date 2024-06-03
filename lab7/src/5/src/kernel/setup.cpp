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
void LRU(int page_num,int now_time,int len,int& miss_num,bool flag[],int memory[],int& memory_full,int page_time[]);
void FIFO(int page_num,bool flag[],int memory[],int& memory_full,int len,int& index);
void first_thread(void *arg)
{
    // 第1个线程不可以返回
    // stdio.moveCursor(0);
    // for (int i = 0; i < 25 * 80; ++i)
    // {
    //     stdio.print(' ');
    // }
    // stdio.moveCursor(0);

    //Assignment3
    /*char *p1 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 100);
    char *p2 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 10);
    char *p3 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 100);

    printf("%x %x %x\n", p1, p2, p3);
    if(p2 - p1 == PAGE_SIZE*100 && p3 - p2 == PAGE_SIZE*10){
        printf("test1 pass!\n");
    }
    memoryManager.releasePages(AddressPoolType::KERNEL, (int)p2, 10);
    p2 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 100);

    printf("%x\n", p2);
    if(p2 - p3 == 100*PAGE_SIZE){
        printf("test2 pass!\n");
    }
    p2 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 10);
    
    printf("%x\n", p2);
    if(p2 - p1 == 100*PAGE_SIZE){
        printf("test3 pass!\n");
    }*/

    //Assignment4 LRU
    int memory[3];
    int memory_full = 0; //how many pages in memory
    int now_time = 0;
    int miss_num = 0;
    int index = 0;
    int visit[100] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
    int page_time[3];
    bool flag[10];
    for(int i = 0;i<10;i++){
        flag[i] = false;
    }
    //FIFO
    for(int i = 0;i<20;i++){
        FIFO(visit[i],flag,memory,memory_full,3,index);
    }
    //LRU
    /*for(int i = 0;i<20;i++){
        LRU(visit[i],now_time,3,miss_num,flag,memory,memory_full,page_time);
        now_time++;
    }*/
    asm_halt();
}
void FIFO(int page_num,bool flag[],int memory[],int& memory_full,int len,int& index){
    if(flag[page_num] == false){
        if(memory_full == len){
            flag[memory[index]] = false;
            flag[page_num] = true;
            memory[index] = page_num;
        }
        else{
            memory[index] = page_num;
            flag[page_num] = true;
            memory_full++;
        }
        index++;
        index = index % len;
        for(int i = 0;i<len;i++){
            if(i<memory_full){
                printf("%d ",memory[i]);
            }
            else{
                printf("  ");
            }
        }
        printf("\n");
    }
    else{
        printf("\n");
    }
}
void LRU(int page_num,int now_time,int len,int& miss_num,bool flag[],int memory[],int& memory_full,int page_time[]){
    if(flag[page_num] == false){
        miss_num++;
        if(memory_full == len){
            int min_time = 0x3f3f3f,index = 0;
            for(int i = 0;i<len;i++){
                if(page_time[i] < min_time){
                    min_time = page_time[i];
                    index = i;
                }
            }
            flag[page_num] = true;
            flag[memory[index]] = false;
            page_time[index] = now_time;
            memory[index] = page_num;
        }
        else{
            memory[memory_full] = page_num;
            page_time[memory_full] = now_time;
            memory_full++;
            flag[page_num] = true;
        }
        for(int i = 0;i<len;i++){
            if(i<memory_full){
                printf("%d ",memory[i]);
            }
            else{
                printf("  ");
            }
        }
        printf("\n");
    }
    else{
        for(int i = 0;i<len;i++){
            if(memory[i] == page_num){
                page_time[i] = now_time;
            }
        }
        printf("\n");
    }
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
    memoryManager.openPageMechanism();
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
