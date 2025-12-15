#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/semaphore.h>
#include <linux/delay.h>

#define N 5 // Number of philosophers

// Global semaphores to represent chopsticks
static struct semaphore chopsticks[N];
static int initialized = 0;

/* * Helper to initialize semaphores on the first call.
 * In a real scenario, this might be done during kernel boot, 
 * but for a project system call, we can check a flag.
 */
void init_forks(void) {
    int i;
    if (!initialized) {
        for (i = 0; i < N; i++) {
            sema_init(&chopsticks[i], 1); // Init mutex to 1 (unlocked)
        }
        initialized = 1;
        printk(KERN_INFO "Dining Philosophers: Forks Initialized.\n");
    }
}

/*
 * System Call: sys_dining_philosophers
 * Arguments:
 * id: Philosopher ID (0-4)
 * action: 0 = EAT (Pick up forks), 1 = THINK (Put down forks)
 */
SYSCALL_DEFINE2(dining_philosophers, int, id, int, action)
{
    int left = id;
    int right = (id + 1) % N;

    init_forks();

    if (id < 0 || id >= N) return -EINVAL;

    if (action == 0) { // WANT TO EAT
        printk(KERN_INFO "Philosopher %d is hungry.\n", id);

        // Pick up left fork
        down(&chopsticks[left]);
        printk(KERN_INFO "Philosopher %d picked up left fork (%d).\n", id, left);

        // Small delay to simulate race conditions or realistic timing
        msleep(100);

        // Pick up right fork
        down(&chopsticks[right]);
        printk(KERN_INFO "Philosopher %d picked up right fork (%d).\n", id, right);

        printk(KERN_INFO "Philosopher %d is EATING.\n", id);
        return 0;

    } else if (action == 1) { // FINISHED EATING
        
        // Put down left fork
        up(&chopsticks[left]);
        
        // Put down right fork
        up(&chopsticks[right]);
        
        printk(KERN_INFO "Philosopher %d put down forks and is THINKING.\n", id);
        return 0;
    }

    return -EINVAL;
}
