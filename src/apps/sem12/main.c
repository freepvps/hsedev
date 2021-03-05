#define _POSIX_C_SOURCE 200809
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>

enum { MAX_ARGS = 64 };

typedef struct {
    int (*func) (size_t count, char** argv);
    char* name;
} func_t;

#define MODE_CREATE_DEFAULT ((mode_t)(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH))

int require_path_arg(size_t count, char** argv) {
    if (count == 1) {
        printf("Usage: %s path\n", argv[0]);
        errno = EINVAL;
        return -1;
    }
    return 0;
}

int func_echo(size_t count, char** argv) {
    for (size_t i = 1; i < count; i++) {
        if (i + 1 == count) {
            printf("%s\n", argv[i]);
        } else {
            printf("%s ", argv[i]);
        }
    }
}

int func_ls(size_t count, char** argv) {
    char* current_path = ".";
    if (count > 1) {
        current_path = argv[1];
    }
    DIR* dir = opendir(current_path);
    if (dir == NULL) {
        return -1;
    }
    struct dirent* current_value;
    while ((current_value = readdir(dir)) != NULL) {
        printf("%s\n", current_value->d_name);
    }
}

int func_cd(size_t count, char** argv) {
    if (require_path_arg(count, argv) == -1) {
        return -1;
    }
    return chdir(argv[1]);
}

int func_rm(size_t count, char** argv) {
    if (require_path_arg(count, argv) == -1) {
        return -1;
    }
    return remove(argv[1]);
}

int func_rmdir(size_t count, char** argv) {
    if (require_path_arg(count, argv) == -1) {
        return -1;
    }
    return rmdir(argv[1]);
}

int func_mkdir(size_t count, char** argv) {
    if (require_path_arg(count, argv) == -1) {
        return -1;
    }
    return mkdir(argv[1], MODE_CREATE_DEFAULT);
}

int func_cat(size_t count, char** argv) {
    if (require_path_arg(count, argv) == -1) {
        return -1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return -1;
    }
    char buf[4096];
    while (1) {
        ssize_t res = read(fd, buf, sizeof(buf));
        if (res <= 0) {
            break;
        }
        if (write(STDOUT_FILENO, buf, (size_t)res) != res) {
            break;
        }
    }
    close(fd);
    return 0;
}

int func_append(size_t count, char** argv) {
    if (require_path_arg(count, argv) == -1 || count < 3) {
        return -1;
    }
    int fd = open(argv[1], O_APPEND | O_CREAT | O_WRONLY, MODE_CREATE_DEFAULT);
    if (fd == -1) {
        return -1;
    }
    int error = (
        write(fd, argv[2], strlen(argv[2])) == -1
        || write(fd, "\n", 1) == -1
    );
    close(fd);

    return -error;
}

int func_stat(size_t count, char** argv) {
    if (require_path_arg(count, argv) == -1) {
        return -1;
    }
    struct stat stat_buf;
    if (stat(argv[1], &stat_buf) == -1) {
        return -1;
    }

    printf("name: %s\n", argv[1]);
    printf("size: %"PRId64"\n", (int64_t)stat_buf.st_size);
    printf("mode: %"PRIu64"\n", (uint64_t)stat_buf.st_mode);
    return 0;
}

int func_size(size_t count, char** argv) {
    if (require_path_arg(count, argv) == -1) {
        return -1;
    }
    int fd = open(argv[1], 0);
    if (fd == -1) {
        return -1;
    }
    off_t res = lseek(fd, 0, SEEK_END);
    close(fd);
    if (res == -1) {
        return -1;
    }
    printf("name: %s\n", argv[1]);
    printf("size: %"PRId64"\n", (int64_t)res);
    return 0;
}

// stat

func_t func_table[] = {
    {.func = &func_echo, .name = "echo"},
    {.func = &func_ls, .name = "ls"},
    {.func = &func_cd, .name = "cd"},
    {.func = &func_rm, .name = "rm"},
    {.func = &func_rmdir, .name = "rmdir"},
    {.func = &func_mkdir, .name = "mkdir"},
    {.func = &func_cat, .name = "cat"},
    {.func = &func_append, .name = "append"},
    {.func = &func_stat, .name = "stat"},
    {.func = &func_size, .name = "size"},
};

int process_args(size_t count, char** argv) {
    if (!count) {
        return 0;
    }
    size_t funcs_count = sizeof(func_table) / sizeof(*func_table);
    for (size_t i = 0; i < funcs_count; i++) {
        if (strcmp(func_table[i].name, argv[0]) == 0) {
            if (func_table[i].func(count, argv) == -1) {
                perror(argv[0]);
                return -1;
            } else {
                return 0;
            }
        }
    }
    printf("func not found: %s\n", argv[0]);
    return 0;
}

int process_line(char* s) {
    char* buf[MAX_ARGS];
    size_t argc = 0;
    size_t i = 0;
    size_t j = 0;
    for (i = 0; (s[i] && s[i] != '\n') && argc < MAX_ARGS; i++) {
        if (s[i] == ' ') {
            s[i] = '\0';
            if (j < i) {
                buf[argc++] = &s[j];
            }
            j = i + 1;
        }
    }
    s[i] = '\0';
    if (j < i && argc < MAX_ARGS) {
        buf[argc++] = &s[j];
    }
    return process_args(argc, buf);
}

int main() {
    char* s = NULL;
    size_t n = 0;
    while (printf("$~: ") != -1 && getline(&s, &n, stdin) != -1) {
        process_line(s);
    }
}
