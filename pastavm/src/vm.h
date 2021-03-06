#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

/* using:
 * type:
 *    FILE
 */
#include <stdio.h>
#include "positions.h"
#include "heap.h"

#define CSTPOOL_INDEX_TABLE_LEN(cst_pool) (*((int *)cst_pool))
#define CSTPOOL_INDEX_TABLE(cst_pool) (cst_pool + 1)

#define DEFAULT_HEAP_CAPACITY ((unsigned long)1 << 30) /* 1 GB */ 
#define MAX_HEAP_CAPACITY     ((unsigned long)1 << 33) /* 16 GB */
#define MIN_HEAP_CAPACITY     ((unsigned long)1 << 19) /* 512 MB */

#define DEFAULT_STACK_CAPACITY ((unsigned long)1 << 22) /* 4 MB */
#define MIN_STACK_CAPACITY      ((unsigned long)1 << 19) /* 512 KB */
#define MAX_STACK_CAPACITY      ((unsigned long)1 << 26) /* 64 MB */

#define __CFNAME_HEAP_CAPACITY  "HeapCapacity"
#define __CFNAME_STACK_CAPACITY "StackCapacity"
#define __CFNAME_CONF_FILE_NAME "vm.conf"

typedef long long int item_t;

/* ITEMS IN THE CONSTANT POOL WOULD BE HEAP ITEMS */

struct conf {
    unsigned long int stack_capacity, heap_capacity;
};

struct stack {
    size_t capacity;
    item_t *stack;
};

/* These registers store offsets instead of real address */
struct registers {
    item_t *sp, *bp, *hr; 
    char *pc; 
};

struct inslist {
    unsigned long int length;
    char *inslist;
};

struct constant_pool {
    unsigned long int count;
    unsigned long int *offsets;
};

struct vm {
    void *rawcode;
    struct stack stack;
    struct registers registers;
    struct constant_pool constant_pool;
    Heap heap;
    struct inslist instructions;
};

struct vm vm;

int load(const char *src);

int execute();

void halt();

int set_stack_capacity(size_t capacity);

int set_heap_capacity(size_t capacity);

/* PRIVATE FUNCTIONS, DO NOT CALL IT FOR ORDINARY USE */
int  __load_config(struct conf *config);
int  __get_config(const char *config_json, struct conf *config);
int  __create_default_config(struct conf *config);
char *__read_file_content(FILE *fp);
void __init_heap(size_t size);
int  __init_stack(size_t size);
int  __load_bytecode_file(const char *src);
void __free_stack();
void __free_heap();

#endif
