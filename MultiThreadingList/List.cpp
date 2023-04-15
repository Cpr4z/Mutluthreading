#include "List.h";
#include <thread>
#include <future>


FineGrainedQueue::FineGrainedQueue()
{
    srand(1252131);
    head = NULL;
    queue_mutex = new std::mutex;
}

void FineGrainedQueue::pushback(int val)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 50));
    std::lock_guard<std::mutex> m(*queue_mutex);
    if (head == NULL)
    {
        head = new Node;
        head->next = NULL;
        head->value = val;
        head->node_mutex = new std::mutex;
        return;
    }
    else
    {
        Node* loopPtr = head;
        while (loopPtr->next != NULL)
        {
            loopPtr = loopPtr->next;
        }
        Node* newNode = new Node;
        newNode->next = NULL;
        newNode->value = val;
        newNode->node_mutex = new std::mutex;
        loopPtr->next = newNode;
        return;
    }
}

void FineGrainedQueue::print()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 50));
    std::lock_guard<std::mutex> m(*queue_mutex);
    Node* loopPtr = head;
    while (loopPtr != NULL)
    {
        locked_output(std::to_string(loopPtr->value));
        locked_output(" ");
        loopPtr = loopPtr->next;
    }
    locked_output("\n");
}

void FineGrainedQueue::insertIntoMiddle(int val, int pos)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 50));

   
    int counter = 0;    

    Node* prev;

    queue_mutex->lock();        

    Node* loopPtr = head;

    loopPtr->node_mutex->lock();        
                                       
    queue_mutex->unlock();


    while ((loopPtr->next != NULL) && (counter != pos))
    {
        ++counter;
        prev = loopPtr;                 
        loopPtr = loopPtr->next;
        loopPtr->node_mutex->lock();    
        prev->node_mutex->unlock();     
    }


    Node* ToInsert = new Node;
    ToInsert->value = val;
    ToInsert->node_mutex = new std::mutex;
    ToInsert->node_mutex->lock();       
                                                                        
    ToInsert->next = loopPtr->next;     
    loopPtr->next = ToInsert;           

    loopPtr->node_mutex->unlock();        
    ToInsert->node_mutex->unlock();     
}