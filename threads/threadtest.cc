// threadtest.cc
//	Simple test case for the threads assignment.
//
//	Create several threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield,
//	to illustrate the inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.
//
// Parts from Copyright (c) 2007-2009 Universidad de Las Palmas de Gran Canaria
//

#include "copyright.h"
#include "system.h"

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 10 times, yielding the CPU to another ready thread
//	each iteration.
//
//	"name" points to a string with a thread name, just for
//      debugging purposes.
//----------------------------------------------------------------------




//----------------------------------------------------------------------

#include "producer.h"
#include "consumer.h"
#include "sharedBuffer.h"

//----------------------------------------------------------------------


void
SimpleThread(void* name)
{
    // Reinterpret arg "name" as a string
    char* threadName = (char*)name;

    // If the lines dealing with interrupts are commented,
    // the code will behave incorrectly, because
    // printf execution may cause race conditions.
    for (int num = 0; num < 10; num++) {
        //IntStatus oldLevel = interrupt->SetLevel(IntOff);
	printf("*** thread %s looped %d times\n", threadName, num);
	//interrupt->SetLevel(oldLevel);
        //currentThread->Yield();
    }
    //IntStatus oldLevel = interrupt->SetLevel(IntOff);
    printf(">>> Thread %s has finished\n", threadName);
    //interrupt->SetLevel(oldLevel);
}


void
startProd(void* arg)
{
    Producer* p = (Producer*) arg;
    p->startProducing();
}


void
startCons(void* arg)
{
    Consumer* p = (Consumer*) arg;
    p->startConsuming();
}


















//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between several threads, by launching
//	ten threads which call SimpleThread, and finally calling
//	SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering SimpleTest");

    Lock* tableLock = new Lock("Table lock");

    Lock* productionLock = new Lock("Production lock");
    Lock* consumptionLock = new Lock("Consumption lock");

//    Condition* productionCondition = new Condition("Production Condition", productionLock);
//    Condition* consumptionCondition = new Condition("Consumption Condition", consumptionLock);

    Condition* productionCondition = new Condition("Production Condition", tableLock);
    Condition* consumptionCondition = new Condition("Consumption Condition", tableLock);

    SharedBuffer* foodTable = new SharedBuffer(5);

    Producer* prod1 = new Producer("Producer 1", tableLock, productionCondition, consumptionCondition, foodTable);
    Thread* producerThread1 = new Thread("Producer Thread 1");
    producerThread1->Fork(startProd, (void*)prod1);

    Producer* prod2 = new Producer("Producer 2", tableLock, productionCondition, consumptionCondition, foodTable);
    Thread* producerThread2 = new Thread("Producer Thread 2");
    producerThread2->Fork(startProd, (void*)prod2);

    Consumer* cons = new Consumer("Consumer", tableLock, productionCondition, consumptionCondition, foodTable);
    Thread* consumerThread = new Thread("Consumer Thread");
    consumerThread->Fork(startCons, (void*)cons);


//    Thread* producerThread = new Thread("Producer Thread");
//    Thread* consumerThread = new Thread("Consumer Thread");
//
//    producerThread->Fork(prod->startProducing, (void*)"hudai");
//    consumerThread->Fork(cons->startConsuming, (void*)"hudai");


//    for ( int k=1; k<=10; k++) {
//      char* threadname = new char[100];
//      sprintf(threadname, "Hilo %d", k);
//      Thread* newThread = new Thread (threadname);
//      newThread->Fork (SimpleThread, (void*)threadname);
//    }
//
//   SimpleThread( (void*)"Hilo 0");
}

