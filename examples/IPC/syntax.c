// @ #include <sys/ipc.h>
    key_t key = ftok(const char*, int);

// @ #include <sys/shm.h>
    int shmget(key_t, size_t, int);
    void* shmat (int, const void*, int); // cast to char*

    int shmctl(shmget(...), IPC_RMID, 0); // deallocate Shared_Memory 

// @ #include <sys/msg.h>
    struct msgBuff { long, char[] } msg;

    int msgget(key_t, 0666 | IPC_CREAT); // create Message Queue
    int msgget(key_t, 0666); // locate Message Queue

    int msgsnd (msgget(...), &msg, sizeof(msg)-sizeof(long), 0); // writing to Message Queue
    int msgrcv(msget(...), &msg, sizeof(msg)-sizeof(long), msg.long, 0) // read from Message Queue

    int msgctl(msget(...), IPC_RMID, NULL); // deallocate Message Queue

// @ #include <unistd.h>
    int close(fd[0 || 1]);
    ssize_t read(fd[0], char[SIZE], SIZE); 
    write(fd[1], char __name[], strlen(__name)); 

// @ String Length
    size_t strlen(const char*);

// @ String Copy
    strncpy(char[] to, char* from, strlen(from)+1);

// @ String Initialize
    char str[SIZE];
    memset(str, int, BUFFER)

// @ Write String to Shared_Memory
    const char* word;
    snprintf(const char*, strlen(word)+1, "%s", word);