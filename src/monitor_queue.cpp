#include "monitor_queue.hpp"
MonitorQueue::MonitorQueue(size_t initial_size, size_t wait_time)
//initalizer list
:queue(new int[initial_size]),size(initial_size),front(0),rear(0),items_stored(0),max_wait_time(wait_time),log_file(nullptr)
{}

void MonitorQueue::resize(size_t new_size){
    //assumed the mutex is already locked, like this whole thing is critical
    int* old_queue = queue;
    size_t old_front = front;
    size_t old_rear = rear;
    queue = new int[new_size];
    front = 0; rear = 0;
    queue[0] = old_queue[old_front];
    while(old_front != old_rear){
        old_front = (old_front+1)%size;
        rear = (rear+1)%new_size;
        queue[rear]=old_queue[old_front];
    }
    delete[] old_queue;
    size = new_size;
}

bool MonitorQueue::set_log_file(std::ofstream& new_log_file){
    log_file = &new_log_file;
    return log_file->is_open();
}

void MonitorQueue::push(int value){
    mutex.lock();

    if(items_stored==size){
        resize(size*2);
        if(log_file->is_open()) *log_file<<"Monitor: Queue size doubled.\n";
    }
    rear = (rear+1)%size;
    queue[rear]=value;
    items_stored++;

    if(log_file->is_open()) *log_file<<"Monitor: ["<<value<<"] added into queue\n";

    log_file->flush();
    consumer_go.notify_one();
    mutex.unlock();
}

std::optional<int> MonitorQueue::pop(){
    std::optional<int> return_value;
    std::chrono::seconds timeout(max_wait_time);
    std::unique_lock lock(mutex);
    if(items_stored==0){
        if(!consumer_go.wait_for(lock,timeout, [this](){return items_stored>0;})){
            if(log_file->is_open()) *log_file<<"Monitor: Pop timed out\n";
            return std::nullopt;
        }
    }

    return_value = queue[front];
    front = (front+1)%size;
    items_stored--; 

    if(log_file->is_open()) *log_file << "Monitor: [" << *return_value << "] extracted from queue.\n";

    if(items_stored*4<size){
        resize(size/2);
        if(log_file->is_open()) *log_file<<"Monitor: Queue size halved.\n";
    }

    log_file->flush();
    mutex.unlock();
    return return_value;
}
