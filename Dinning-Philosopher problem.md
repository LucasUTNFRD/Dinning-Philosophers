---
date: 2024-05-21
tags: 
hubs:
  - "[[]]"
  - "[[Concurrency]]"
urls:
---
# Dinning-Philosopher problem

## Problem
philosopher either:
- Thinks->When a philosopher thinks, he does not interact with his colleagues
-  Eats-> When a philosopher gets hungry he tries to pick up the two forks that are closest to him (left & right).A philosopher may pick up only one fork a time. Also one cannot pick up a fork that is already in the hand of a neighbor. Then when a hungry philosopher has both his forks at the same time, he eats without releasing his forks. When he has finishes eating, he puts down both of his forks and starts thinking again.

## Relation with OS 
This is problem represents resource allocation in OS, philosophers represent processes and forks represents resources that has to be shared between processes in synchronized manner.
## naive approach using semaphores
represent each fork with semaphore

a philosopher tries to grab a fork by executing P() operation on that semaphore.

he releases his fork() by executing V() operation on the appropriate semaphores.

```
sem_t forks[5]
//structure of philosopher_i
do{
wait(fork[i]);
wait(fork[i+1]%5);

//eat
signal(fork[i]);
signal(fork[i+1]%5);
//think
}while(true)
```

we encounter a problem a deadlock. If each philosopher happens to grab the fork on their left before any philosopher can grab the fork on their right,each will be stuck holding one fork and waiting for another

## Solutions
1. Allow at most four philosophers to be sitting simultaneously at the table
2. Allow a philosopher to pick up his fork only if both forks are available(to do this he must pick them up in a critical section)
