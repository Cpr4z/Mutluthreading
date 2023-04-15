#include <mutex>
#include <string>
#include <iostream>

std::mutex m_cout;

void locked_output(const std::string& msg)
{
    m_cout.lock();
    std::cout << msg;
    m_cout.unlock();
}

struct Node
{
    int value;
    Node* next;
    std::mutex* node_mutex;
};


class FineGrainedQueue
{
public:
    FineGrainedQueue();
    void pushback(int val);                         
    void print();                                   
    void insertIntoMiddle(int val, int pos);        
private:
    Node* head;
    std::mutex* queue_mutex;
};