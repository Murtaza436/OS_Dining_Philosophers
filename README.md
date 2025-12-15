# Dining Philosophers: Linux System Call Implementation

**Course:** CS311 Operating Systems  
**Kernel Version:** Linux 6.8.1  
**Project Type:** Custom System Call & Concurrency Control

## 📖 Project Overview
This project solves the classic **Dining Philosophers** synchronization problem by implementing a **Custom System Call** in the Linux Kernel.

Unlike standard solutions that use user-space threads and mutexes, this project manages resources (forks) directly inside **Kernel Space (Ring 0)** using **Kernel Semaphores**. The user-space program simply makes a request to the OS, demonstrating how Operating Systems handle atomic resource allocation and process scheduling.

## 🚀 Key Features
* **Custom System Call:** Implemented `sys_dining_philosophers` (Syscall #548).
* **Kernel Synchronization:** Uses `struct semaphore` and `down()`/`up()` for atomic locking.
* **Deadlock Prevention:** Implements an asymmetry solution in user-space (Odd/Even logic) to prevent circular wait conditions.
* **Process Management:** Demonstrates the use of `TASK_UNINTERRUPTIBLE` sleep states when resources are unavailable.

## 📂 File Structure
* `dining.c` - The kernel-level source code containing the system call logic.
* `test_dining.c` - The user-space C program that simulates the philosophers and calls the kernel.
