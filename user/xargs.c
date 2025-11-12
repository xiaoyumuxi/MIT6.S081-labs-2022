#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h" // 添加param.h以获取MAXARG

#define Buffer 512

int main(int argc, char const *argv[])
{
    // 检查参数数量：至少需要xargs和一个命令
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    // 命令名是argv[1]，固定参数从argv[2]开始
    char *cmd = (char *)argv[1];
    int fixed_argc = argc - 2; // 固定参数的数量(xargs+cmd)
    char line[Buffer];
    // 读取标准输入的内容
    while (read(0, line, Buffer) > 0)
    {
        // 重新定义参数数组
        char *all_argv[MAXARG]; // 使用MAXARG（xv6定义的参数上限）

        // 将标准输入得到的东西line放到第一部分，后面拼接的是xargs的参数
        int count = 0;
        all_argv[count++] = cmd; // 新的argv[0]是原来的cmd

        // 添加固定参数（argv[2]到argv[argc-1]）
        for (int i = 0; i < fixed_argc; i++)
        {
            all_argv[count++] = (char *)argv[i + 2];
        }

        // 分割当前行（按空格和换行符分割）
        char *start = line;
        char *end = line;
        while (*start != '\0')
        {
            // 跳过分隔符（空格、换行符）
            while (*start && (*start == ' ' || *start == '\n'))
            {
                start++;
            }

            if (*start == '\0')
            {
                break;
            }

            // 记录当前参数的开始位置
            end = start;

            // 找到下一个分隔符
            while (*start && *start != ' ' && *start != '\n')
            {
                start++;
            }

            // 用\0替换分隔符，标记参数结束
            if (*start != '\0')
            {
                *start = '\0';
                start++;
            }

            // 添加到参数数组
            if (count < MAXARG - 1)
            {
                all_argv[count++] = end;
            }
        }
        all_argv[count] = '\0';

        // 创建子进程执行命令
        int pid = fork();
        if (pid < 0)
        {
            fprintf(2, "fork failed\n");
            exit(1);
        }
        else if (pid == 0)
        {
            // 执行命令
            exec(cmd, all_argv);
            // 如果exec失败，退出
            fprintf(2, "exec failed\n");
            exit(1);
        }
        else
        {
            // 等待子进程完成
            wait(0);
        }
    }
    exit(0);
}
