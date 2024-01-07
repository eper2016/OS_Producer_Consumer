# OS_Producer_Consumer
A program that takes in three numbers: sleep time for the main, the number of producers, and the number of consumers. 
As the main thread sleeps for a certain amount, both producer and consumer work synchronously, and as soon as the main thread wakes up, the main program exits and kills all threads. 
Both producer and consumer threads produce a random number to sleep. 
The producer thread will insert an item, while the consumer thread will remove the item the producer inserted. 
