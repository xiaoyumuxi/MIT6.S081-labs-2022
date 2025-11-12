#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char *find_obj, char *path)
{
    char buf[512]; // 用来构建递归的路径
    struct stat info;
    struct dirent de;

    int fd;
    // 打开当前目录文件
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "This is not a dir!");
        return;
    }
    // 从fd中获取stat信息进行存储
    if (fstat(fd, &info) < 0)
    {
        fprintf(2, "cannot find stat!!!");
        close(fd);
        return;
    }
    // 类型不是DIR报错要求重新来
    if (info.type != T_DIR)
    {
        fprintf(2, "This is not a dir!");
        return;
    }

    strcpy(buf, path); // 将当前的路径放进buffer中
    char *p;
    p = buf + strlen(path);
    *p = '/'; // 添加路径分隔符
    p++;
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        // xv中目录de是按照数组形式进行存储的,read系统调用在读取一个打开的目录的时候每次返回的是一个完整的sturct dirent

        if (de.inum == 0)
            continue;
        // 空目录跳过
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        // 当前目录的上一级和上上一级不允许被访问
        // memmove(p, de.name, DIRSIZ); // 字符数组拼接 buf = path + '/' + de.name
        // p[DIRSIZ] = '\0';            // 字符串构造
        strcpy(p, de.name); // 使用strcpy才会自动进行\0的添加
        if (stat(buf, &info) < 0)//从一个路径buf中获取对应的文件信息
        { // 打不开的情况,直接跳过到下一个选项
            fprintf(2, "cannot find stat!!!");
            continue;
        }
        if (strlen(buf) + strlen(de.name) >= sizeof(buf))
        {
            printf("find:the path is too long!!!");
            close(fd);
            return;
        }

        if (info.type == T_FILE && strcmp(de.name, find_obj) == 0)
            printf("%s\n", buf); // 是文件打印目录

        if (info.type == T_DIR)
            find(find_obj, buf); // 是目录开始递归
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage:find <path> <filename>\n");
        exit(1);
    }
    find(argv[2], argv[1]);
    exit(0);
}