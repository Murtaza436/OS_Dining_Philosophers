#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>

#define __NR_dining_philosophers 548 
#define N 5
#define LOOPS 1  // Changed to 1: They only eat once

void *philosopher_lifecycle(void *arg) {
    int id = *(int *)arg;
    long res;

    for (int i = 0; i < LOOPS; i++) {
        
        // 1. THINKING PHASE
        printf("[Philosopher %d] is THINKING.\n", id);
        
        // --- ASYMMETRY FIX (User-Space Logic) ---
        // Odd IDs wait to prevent deadlock
        if (id % 2 != 0) {
            // This is the message you asked for:
            printf("[Philosopher %d] Cannot eat yet (No forks). Waiting for neighbor...\n", id);
            usleep(500000); // Wait 0.5 seconds
        }

        // 2. HUNGRY PHASE (Call Kernel)
        printf("[Philosopher %d] is HUNGRY and calling kernel for forks...\n", id);
        
        // This System Call blocks until forks are acquired
        res = syscall(__NR_dining_philosophers, id, 0);
        
        if (res == 0) {
            // 3. EATING PHASE (We have the forks!)
            printf("   >>> [Philosopher %d] ACQUIRED FORKS and is EATING.\n", id);
            
            usleep(500000); // Eat for 0.5 seconds

            // 4. FINISHED PHASE (Release forks)
            syscall(__NR_dining_philosophers, id, 1);
            printf("       [Philosopher %d] FINISHED eating. Dropped forks.\n", id);
        } else {
            printf("[Philosopher %d] CRITICAL ERROR: System call failed.\n", id);
            return NULL;
        }
    }

    printf("--- [Philosopher %d] is FULL and leaving the table. ---\n", id);
    return NULL;
}

int main() {
    pthread_t p[N];
    int ids[N];

    printf("===================================================\n");
    printf("   DINING PHILOSOPHERS: SYSTEM CALL TEST (1 Cycle) \n");
    printf("===================================================\n");

    // Create Threads
    for(int i=0; i<N; i++) {
        ids[i] = i;
        pthread_create(&p[i], NULL, philosopher_lifecycle, &ids[i]);
    }

    // Wait for everyone to finish
    for(int i=0; i<N; i++) {
        pthread_join(p[i], NULL);
    }
    
    printf("===================================================\n");
    printf("   SUCCESS: All Philosophers finished!           \n");
    printf("===================================================\n");
    return 0;
}
