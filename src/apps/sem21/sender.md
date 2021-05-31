1 process:
  queue1 -> server1 (fd)
  queue2 -> server2 (fd)
  eventfd (fd)

2 process:
  input -> queue1
        -> queue2


queue1: 10 bytes
queue2: 0 bytes

2 process put 12 bytes in queue2

queue1: 10 bytes  <--- blocked
queue2: 12 bytes
