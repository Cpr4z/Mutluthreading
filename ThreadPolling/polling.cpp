#include <iostream>
#include <mutex>
#include <thread>
#include <memory>
#include <queue>
#include <list>
#include <string>
#include <random>
using namespace std;

typedef struct Part
{
  int part_id;
  float volume;
  typedef shared_ptr<Part> part_ptr;
} Part;

static bool done = false;
queue<Part::part_ptr> shared_queue;
mutex lock_queue;
mutex lock_cout;

void locked_output(const string& str)
{
  lock_guard<mutex> raii(lock_cout);
  cout <<  str << endl;
}

void threadAWork(Part::part_ptr& part)
{
  part->volume -= 2;
  this_thread::sleep_for(chrono::milliseconds(500 + rand() & 6000));
  locked_output("threadAWork finished with part" + to_string(part->part_id));
}

void threadBWork(Part::part_ptr& part)
{
  part->volume -= 1;
  this_thread::sleep_for(chrono::milliseconds(500 + rand() & 6000));
  locked_output("threadBWork finished with part" + to_string(part->part_id));
}

void threadA(list <Part::part_ptr>& input)
{
  srand(77777777);
  size_t size = input.size();
  for (size_t i = 0; i < size; ++i)
  {
  threadAWork(*input.begin());
  lock_guard<mutex> raii(lock_queue);
  shared_queue.push(Part::part_ptr(*input.begin()));
  input.remove(*input.begin());
  locked_output("Part was added in queue");
  }
  done = true;
}

void threadB()
{
  srand(10000000);
  while (true)
  {
    Part::part_ptr part_for_work;
    lock_queue.lock();
    if (shared_queue.empty())
    {
      lock_queue.unlock();
      if (done) break;
      locked_output("ThreadB useless check, queue is empty. Going to bed");
      this_thread::sleep_for(chrono::milliseconds(1000));
      continue;
    }
    else
    {
      part_for_work = shared_queue.front();
      shared_queue.pop();
      lock_queue.unlock();
      locked_output("Part was removed from queue");
      }
    threadBWork(part_for_work);
    }
}
int main()
{
  list<Part::part_ptr> spare_parts;
  for (int i = 0; i < 5; i++)
  {
    spare_parts.push_back(Part::part_ptr(new Part{ i + 1, 10.0 }));
  }
  thread ta(threadA, ref(spare_parts));
  thread tb(threadB);

  ta.join();
  tb.join();
}