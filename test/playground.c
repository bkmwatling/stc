#include <assert.h>
#include <stdlib.h>

#define list_def(T)            \
    struct __list_##T {        \
        T     *__list_data;    \
        size_t len, cap, size; \
    }

#define List(T) struct __list_##T

#define _LIST_TO_VOID_LIST(list) ((List(void) *) &(list)->__list_data)

#define LIST_DEFAULT_CAP 4

#define list_new(T, cap) ((List(T) *) _list_new(sizeof(T), (cap)))
#define list_init(list, cap) \
    (*(List(void) **) &(list) = _list_new(sizeof(*(list)->__list_data), (cap)))
#define list_default(T)         list_new(T, LIST_DEFAULT_CAP)
#define list_default_init(list) list_init(list, LIST_DEFAULT_CAP)

#define list_reserve(list, n) _list_reserve(_LIST_TO_VOID_LIST(list), n)

#define list_at(list, idx) ((list)->__list_data[idx])

list_def(void);
list_def(char);
list_def(int);

List(void) * _list_new(size_t size, size_t cap)
{
    List(void) *list = malloc(sizeof(*list));

    list->__list_data = malloc(size * cap);
    list->len         = 0;
    list->cap         = cap;
    list->size        = size;

    return list;
}

void _list_reserve(List(void) * list, size_t n) { return; }

#define STATIC_ASSERT(cond)   \
    do {                      \
        switch (0) {          \
            case 0:           \
            case cond: break; \
        }                     \
    } while (0);

int main(void)
{
    List(int) *a = list_default(int);
    List(int) * b;

    list_at(a, 1)++;

    STATIC_ASSERT(sizeof(int) == 4);
    list_default_init(b);
    list_reserve(a, 10);

    return EXIT_SUCCESS;
}

// our dictionary
#define WORD_world world,
#define WORD_new   new,
#define WORD_brave brave,
#define WORD_hello hello,
#define WORD_Hello Hello,

// the classics
#define COMMA(...)    ,
#define FIRST(a, ...) a

// apply function f for each argument recursively with tail
#define FOREACHTAIL_1(f, a)                   f(a, )
#define FOREACHTAIL_2(f, a, ...)              f(a, FOREACHTAIL_1(f, __VA_ARGS__))
#define FOREACHTAIL_3(f, a, ...)              f(a, FOREACHTAIL_2(f, __VA_ARGS__))
#define FOREACHTAIL_4(f, a, ...)              f(a, FOREACHTAIL_3(f, __VA_ARGS__))
#define FOREACHTAIL_N(_4, _3, _2, _1, N, ...) FOREACHTAIL_##N
#define FOREACHTAIL(f, ...) \
    FOREACHTAIL_N(__VA_ARGS__, 4, 3, 2, 1)(f, __VA_ARGS__)

// if there are two arguments, expand to true. Otherwise false.
#define IFTWO_N(_0, _1, N, ...) N
#define IFTWO(true, false, ...) IFTWO_N(__VA_ARGS__, true, false)

// If empty, expand to true, otherwise false.
// https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
#define IFEMPTY(true, false, ...) IFTWO(true, false, COMMA __VA_ARGS__())

// Join arguments with `_`.
#define JOIN_U(a, b)      a##_##b
#define JOIN_TWO_IN(a, b) IFEMPTY(FIRST, JOIN_U, b)(a, b)
#define JOIN_TWO(a, b)    JOIN_TWO_IN(a, b)
#define JOIN(...)         FOREACHTAIL(JOIN_TWO, __VA_ARGS__)

// Append WORD_ to each argument and join arguments with spaces.
#define WORD_           /* the last one expands to empty */
#define WORDS_TWO(a, b) WORD_##a b
#define WORDS(...)      FOREACHTAIL(WORDS_TWO, __VA_ARGS__)

#define MAGIC_MACRO(a) JOIN(WORDS(WORDS(WORDS(WORDS(WORDS(a))))))
