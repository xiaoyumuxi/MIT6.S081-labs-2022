#include "kernel/types.h"
//这里的顺序有需求的!!!type.h中讲unsigned int重定义为uin在后面的user.h中有应用
#include "user/user.h"


int main(int argc, char const *argv[])
{
    //创建一个父亲->孩子的pipe1
    int pipe1[2];
    //创建一个孩子->父亲的pipe2
    int pipe2[2];

    if(pipe(pipe1)<0){
        fprintf(2,"There is something wrong with pipe1 (parent to child)");
        exit(1);
    }
    if(pipe(pipe2)<0){
        fprintf(2,"There is something wrong with pipe2 (child to parent)");
        exit(1);
    }

    int pid= fork();//创建子进程，为pid
    if(pid == 0){
        //子进程
        close(pipe1[1]);//parent->child只需要读取不需要写入
        close(pipe2[0]);//child->parent只需要写入不需要读取

        char x;
        if(read(pipe1[0],&x,1)!=1){
            exit(1);
        };
        fprintf(1,"%d:received ping\n",getpid());

        if(write(pipe2[1],&x,1)!=1){
            exit(1);
        }
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    }else{
        //父进程
        close(pipe1[0]);//parent->child只需要发数据不需要读取
        close(pipe2[1]);//child->parent只需要读取不需要写入

        char x = 'A';
        if(write(pipe1[1],&x,1)!=1){
            exit(1);
        }
        char recv;
        if (read(pipe2[0],&recv,1)!=1){
            exit(1);
        }
        fprintf(1,"%d:received pong\n",getpid());
        close(pipe1[1]);
        close(pipe2[0]);
        wait(0);//等待pid=0的子进程结束
        exit(0);
    }
}