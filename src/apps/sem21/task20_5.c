#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <sys/errno.h>
#include <sys/timerfd.h>
#include <sys/wait.h>

typedef struct {
    int efd;
    int is_finished;
    int return_code;

    const char *input;
    char **poutput;
    char **perror;
} run_ctx_t;

typedef struct run_handler run_handler_t;
struct run_handler {
    run_ctx_t* ctx;
    int fd;
    void (*callback)(run_handler_t* state);
};

void run_close_fd(run_ctx_t* ctx, int fd) {
    epoll_ctl(ctx->efd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
}

void run_input_writer_handler(run_handler_t* state) {
    size_t str_len = strlen(state->ctx->input);
    if (str_len == 0) {
        run_close_fd(state->ctx, state->fd);
        return;
    }
    ssize_t res = write(state->fd, state->ctx->input, str_len);
    if (res == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            return;
        } else {
            run_close_fd(state->ctx, state->fd);
        }
    } else {
        state->ctx->input += res;
    }
}

void run_timeout_handler(run_handler_t* state) {
    uint64_t x = 0;
    ssize_t s = read(state->fd, (void*)&x, sizeof(x));
    if (s == sizeof(x) && x) {
        state->ctx->return_code = -2;
        state->ctx->is_finished = 1;
    }
}

int run_epoll_add(run_handler_t* handler, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = handler;
    return epoll_ctl(handler->ctx->efd, EPOLL_CTL_ADD, handler->fd, &ev);
}

int run_unblock(int fd) {
    int flags;
    return (
        (flags = fcntl(fd, F_GETFL)) != -1
        && fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1
    ) ? 0 : -1;
}

int run(const char *cmd, const char *input, char **poutput, char **perror, int timeout /*ms*/) {
    int efd = -1;
    int timeout_fd = -1;
    int in_pipe[2] = {-1, -1};

    // TODO: check overflow if timeout >= 1000 (ms)
    struct itimerspec s = {{0, timeout * 1000000}, {0, timeout * 1000000}};
    int ok = (
        (efd = epoll_create1(0)) != -1
        && (timeout_fd = timerfd_create(CLOCK_REALTIME, O_NONBLOCK)) != -1
        && timerfd_settime(timeout_fd, 0, &s, NULL) != -1
        && pipe2(&in_pipe, 0) != -1
        && run_unblock(in_pipe[1]) != -1
    );

    run_ctx_t ctx = {
        .efd = efd,
        .is_finished = 0,
        .return_code = 0,
        .input = input,
        .poutput = poutput,
        .perror = perror,
    };
    run_handler_t input_handler = {
        .ctx = &ctx,
        .fd = in_pipe[1],
        .callback = &run_input_writer_handler,
    };
    run_handler_t timeout_handler = {
        .ctx = &ctx,
        .fd = timeout_fd,
        .callback = &run_timeout_handler,
    };

    ok = ok && (
        run_epoll_add(&input_handler, EPOLLOUT) != -1
        && run_epoll_add(&timeout_handler, EPOLLIN) != -1
    );

    if (!ok) {
        int err_code = errno;
        if (efd != -1) {
            close(efd);
        }
        if (in_pipe[0] != -1) {
            close(in_pipe[0]);
        }
        if (in_pipe[1] != -1) {
            close(in_pipe[1]);
        }
        if (timeout_fd != -1) {
            close(timeout_fd);
        }
        errno = err_code;
        return -1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        close(efd);
        close(in_pipe[1]);
        close(timeout_fd);
        dup2(in_pipe[0], STDIN_FILENO);
        if (STDIN_FILENO != in_pipe[0]) {
            close(in_pipe[0]);
        }
        execlp(cmd, cmd, NULL);
        exit(1);
    } else {
        while (!ctx.is_finished) {
            struct epoll_event ev;
            int epoll_status = epoll_wait(ctx.efd, &ev, 1, 1);
            if (epoll_status != 1) {
                continue;
            }
            run_handler_t* handler = (run_handler_t*)ev.data.ptr;
            handler->callback(handler);

            int child_status;
            if (waitpid(pid, &child_status, WNOHANG) == pid) {
                if (WIFEXITED(child_status)) {
                    ctx.return_code = WEXITSTATUS(child_status);
                } else {
                    ctx.return_code = -3;
                }
                ctx.is_finished = 1;
            }
        }
        kill(pid, SIGKILL);
        // TODO: close file descriptors
        return ctx.return_code;
    }
}

int main() {

}
