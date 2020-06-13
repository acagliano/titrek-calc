#ifndef TITREK_UTIL_H
#define TITREK_UTIL_H

// https://gist.github.com/aprell/3722962
#define VA_NARGS_IMPL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(_, ## __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#endif //TITREK_UTIL_H
