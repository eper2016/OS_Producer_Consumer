# OS_Producer_Consumer
A program that takes in three numbers: sleep time for the main, the number of producer, and the number of consumer. 
As the main thread sleeps for a certain amount, both producer and consumer works sychronously, and as soon as the main thread wakes up, the main program exits and kill all threads. 
Both producer and consumer threads produce a random number to sleep. 
Producer thread will insert an item, while the consumer thread will remove the item that the producer inserted. 
