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

#include <tiny_base/api.h>
#include <tiny_base/log.h>
#include <tiny_base/sharedmemory.h>

#if (SHARED_MEMORY_ANON)

void* SharedMemory::shm_alloc(size_t size)
{
    LOG(Debug) << "alloc shared-memory by mmap(anon)\n";

    m_pSharedData = mmap(NULL, m_nSize,
        PROT_READ | PROT_WRITE,
        MAP_ANON | MAP_SHARED, -1, 0);
    if (m_pSharedData == MAP_FAILED) {
        handle_error("mmap error:");
        return nullptr;
    }
    
    return m_pSharedData;
}

void SharedMemory::shm_free(void* data, size_t size)
{
    LOG(Debug) << "free shared-memory by mmap(anon)\n";

    if (0 != munmap(data, size))
        handle_error("munmap error:");
}

#elif (SHARED_MEMORY_DEVZERO)

void* SharedMemory::shm_alloc(size_t size)
{
    LOG(Debug) << "alloc shared-memory by mmap(dev/zero)\n";

    return nullptr;
}

void SharedMemory::shm_free(void* data, size_t size)
{
    LOG(Debug) << "free shared-memory by mmap(dev/zero)\n";
}

#elif (SHARED_MEMORY_SYSVSHM)

#include <sys/shm.h>

void* SharedMemory::shm_alloc(size_t size)
{
    LOG(Debug) << "alloc shared-memory by shmget()\n";

    int id;
    void* data;

    id = shmget(IPC_PRIVATE, size, (SHM_R | SHM_W | IPC_CREAT));

    if (id == -1) {
        handle_error("shmget error:");
        return nullptr;
    }

    std::cout << "shmget id:" << id << std::endl;

    data = shmat(id, NULL, 0);

    if (data == (void*)-1) {
        handle_error("shmat() failed error:");
    }

    if (shmctl(id, IPC_RMID, NULL) == -1) {
        handle_error("shmctl(IPC_RMID) failed:");
    }

    return data;
}

void SharedMemory::shm_free(void* data, size_t size)
{
    LOG(Debug) << "free shared-memory by shmget()\n";
    if (shmdt(data) == -1) {
        handle_error("shmdt() error:");
    }
}

#endif

SharedMemory::SharedMemory(size_t size)
    : m_nSize(size)
{
    m_pSharedData = shm_alloc(size);
    LOG(Debug) << "class SharedMemory constructor\n";
}

SharedMemory::~SharedMemory()
{
    shm_free(m_pSharedData, m_nSize);
    LOG(Debug) << "class SharedMemory destructor\n";
}
