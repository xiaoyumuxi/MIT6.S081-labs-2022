#include "kernel/types.h"
#include "user/user.h"

void prime_filter(int left0,int depth)//fd表示的是左边来的管道而且只是用读端因此就是left[0]表示为left0
{
    if(depth == 35)return;
    int p;
    read(left0, &p, sizeof(int));

    printf("%d\n", p);//打印素数

    int right[2];
    pipe(right);//创建指向右边的管道

    int pid = fork(); // 开始子线程

    if (pid == 0)
    {
        // 子线程中开始往后面读数据

        int x = 0;
        while (read(left0, &x, sizeof(int)) > 0)
        {
            if (x % p != 0)
            {
                // 不被整除那就直接放行
                write(right[1], &x, sizeof(int));
            }
        }
        close(right[1]);
        exit(0);
    }
    close(left0);//关闭左边的管道
    prime_filter(right[0],depth+1);//主线程开始递归调用
}

int main(int argc, char const *argv[])
{
    int left[2];
    pipe(left);

    for(int i =2;i<=35;i++){
        write(left[1],&i,sizeof(int));
    }
    close(left[1]);

    prime_filter(left[0],0);//从0开始进行计数表示深度

    exit(0);
}