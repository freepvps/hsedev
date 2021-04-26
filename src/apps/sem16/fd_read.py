import os
import sys
import typing


T = typing.TypeVar('T')


def magic(x: typing.List[T]) -> typing.Dict[int, T]:
    return {
        i: v
        for i, v in enumerate(x)
    }


def main():
    # arr = ['s', 'y', 'x']
    # # arr = [1, 2, 3]
    # d = magic(arr)
    # d[0].capitalize()

    argv = sys.argv
    fd: int = int(argv[1])
    res = os.read(fd, 10).decode()
    print('input: ' + res)


if __name__ == '__main__':
    main()

