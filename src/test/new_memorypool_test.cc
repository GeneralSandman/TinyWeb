/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <tiny_base/memorypool.h>
#include <tiny_http/http_chain.h>
#include <tiny_http/http_buffer.h>

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct block_t
{
    void * data;
    unsigned long len;
    block_t * next;
}block_t;

class NewMemoryPool
{
    private:
        size_t allocate;
        block_t * blocks;

    public:
        NewMemoryPool()
        {
            allocate = 0;
            blocks = nullptr;
            std::cout << "NewMemoryPool construct" << std::endl;
        }

        http_chain_t* getNewChain(MemoryPool *pool, size_t num)
        {
            http_chain_t *new_chain = nullptr;
            http_chain_t *tmp = nullptr;

            while (num--)
            {
                new_chain = (http_chain_t *) pool->allocate(sizeof(http_chain_t));
                new_chain->buffer = (http_buffer_t *) pool->allocate(sizeof(http_buffer_t));
                
                new_chain->buffer->begin = nullptr;
                new_chain->buffer->end = nullptr;
                new_chain->next = tmp;
                tmp = new_chain;
            }

            return new_chain;
        }

        inline void * malloc_my(MemoryPool * pool, size_t size)
        {
            void * res = nullptr;
            res = malloc(size);

            block_t * new_block = (block_t *)pool->allocate(sizeof(block_t));
            new_block->data = res;
            new_block->len = size;
            new_block->next = blocks;
            blocks = new_block;

            return res;
        }

        void mallocSpace(http_chain_t *chain, MemoryPool * pool, size_t size)
        {
            void * new_mem = nullptr;
            while (nullptr != chain)
            {
                new_mem = malloc_my(pool, size);
                chain->buffer->begin = (unsigned char *) new_mem;
                chain->buffer->end = (unsigned char *) new_mem + size;
                chain = chain->next;
            }
        }

        ~NewMemoryPool()
        {
            size_t num = 0;
            size_t size = 0;
            void * tmp = nullptr;
            while (nullptr != blocks)
            {
                num++;
                size += blocks->len;

                tmp = (void *)blocks->data;
                blocks = blocks->next;
                free(tmp);
            }
            std::cout << "block num(" << num << "),all-size(" << size << ")" << std::endl;
        }

};

size_t countChain(http_chain_t * chain)
{
    size_t num = 0;
    while (nullptr != chain)
    {
        num++;
        chain = chain->next;
    }
    return num;
}

int main()
{
    {
        std::cout << "http_buffer_t size(" << sizeof(http_buffer_t) << ")\n";
        std::cout << "http_chain_t size(" << sizeof(http_chain_t) << ")\n";
        std::cout << "block_t size(" << sizeof(block_t) << ")\n";
    }
    {
        MemoryPool basicpool;
        NewMemoryPool pool;
        for (int i=0; i<1; i++)
        {
            http_chain_t * chain1 = nullptr;

            chain1 = pool.getNewChain(&basicpool, 8);

            pool.mallocSpace(chain1, &basicpool, 128);

            std::cout << "chain size:" << countChain(chain1) << std::endl;
        }
    }
    return 0;
}
