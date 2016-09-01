//windows (64-bits esp -> rsp) 
char mem[1<<26];
asm("mov %0,%%esp\n"::"g"(mem + (1<<26)));
//linux
#include <sys/resource.h>
void increase_stack_size() {
    const rlim_t ks = 1 << 26;
    struct rlimit rl;
    if(!getrlimit(RLIMIT_STACK, &rl)){
        if(rl.rlim_cur < ks){
            rl.rlim_cur = ks;
            setrlimit(RLIMIT_STACK, &rl);
        }
    }
}
