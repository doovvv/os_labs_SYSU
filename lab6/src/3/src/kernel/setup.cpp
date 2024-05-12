#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;

Semaphore semaphore;
Semaphore full;
Semaphore chopstick[5];
//int full;
SpinLock mutex_matcha_cake;
SpinLock mutex_mango_cake;
int cheese_burger;
int matcha_cake;
int mango_cake;
SpinLock mutex;
void a_mother(void *arg)
{
    semaphore.P();
    int delay = 0;

    printf("zc: start to make cheese burger, there are %d cheese burger now\n", cheese_burger);
    // make 10 cheese_burger
    cheese_burger += 10;

    printf("zc: oh, I have to hang clothes out.\n");
    // hanging clothes out
    delay = 0xfffffff;
    while (delay)
        --delay;
    // done

    printf("zc: Oh, Jesus! There are %d cheese burgers\n", cheese_burger);
    semaphore.V();
}

void a_naughty_boy(void *arg)
{
    semaphore.P();
    printf("boy   : Look what I found!\n");
    // eat all cheese_burgers out secretly
    cheese_burger -= 10;
    // run away as fast as possible
    semaphore.V();

}
void waterA(void *arg){
    full.P();
    int delay = 0xfffffff;
    while (delay)
        --delay;
    matcha_cake++;
    printf("waiter-A 6290 put a piece of matcha cake in the plate, rest cake nums:%d\n",matcha_cake);
    int delay_ = 0xfffffff;
    while (delay_)
        --delay_;
}
void waterB(void* arg){
    full.P();
    int delay = 0xfffffff;
    while (delay)
        --delay;
    mango_cake++;
    printf("waiter-B put a piece of mango cake in the plate, rest cake nums:%d\n",mango_cake);
    int delay_ = 0xfffffff;
    while (delay_)
        --delay_;
}
void man(void* arg){
    mutex_matcha_cake.lock();
    if(matcha_cake == 0){
        
        waterA(nullptr);
        int delay = 0xfffffff;
        while (delay)
            --delay;
    }
    /*int delay = 0xfffffff;
    while (delay)
        --delay;*/
    matcha_cake--;
    printf("man get a piece of matcha cake in the plate,rest cake nums:%d\n",matcha_cake);
    full.V();
    mutex_matcha_cake.unlock();
}
void woman(void* arg){
    mutex_mango_cake.lock();
    if(mango_cake == 0){
        waterB(nullptr);
        int delay = 0xfffffff;
        while (delay)
            --delay;
    }
    /*int delay = 0xfffffff;
    while (delay)
        --delay;*/
    mango_cake--;
    printf("woman get a piece of mango cake in the plate, rest cake nums:%d\n",mango_cake);
    full.V();
    mutex_mango_cake.unlock();
}
void philosophers(void* i){
    mutex.lock();
    chopstick[*(int*)i].P();
    int delay = 0xfffffff;
    while (delay)
        --delay;
    chopstick[(*(int*)i + 1) % 5].P();
    mutex.unlock();
    int num = *(int*)i;
    if(num == 0){
        num = 6290;
    }
    printf("Philosophers %d is eating\n",num);
    chopstick[*(int*)i].V();
    chopstick[(*(int*)i + 1) % 5].V();
    printf("Philosophers %d is thinking\n",num);

}
void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);

    //matcha_cake = 2;
    //mango_cake = 1;
    //full.initialize(2);
    //full = 2;
    //mutex_matcha_cake.initialize();
    //mutex_mango_cake.initialize();
    //cheese_burger = 0;
    //semaphore.initialize(1);

    //programManager.executeThread(a_mother, nullptr, "second thread", 1);
    //programManager.executeThread(a_naughty_boy, nullptr, "third thread", 1);
    /*for(int i = 0;i<6;i++){
        programManager.executeThread(man,nullptr,"man",1);
    }
    for(int i = 0;i<4;i++){
        programManager.executeThread(woman,nullptr,"woman",1);
    }*/
    mutex.initialize();
    for(int i = 0;i<5;i++){
        chopstick[i].initialize(1);
    }
    int nums[5] = {0,1,2,3,4};
    for(int i = 0;i<5;i++){
        programManager.executeThread(philosophers,nums+i,"phil",1);
    }

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
