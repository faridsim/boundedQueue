#pragma once 
#include <deque>
#include <mutex>
#include <condition_variable>
#include <cstddef>
#include <utility>
template<typename T>
class bounded_queue{
private:
    std::deque<T> content;
    std::size_t capacity;
    std::mutex mutex;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    bool closed=false;
    bool succeed=true;
    bool failed=false;
    

public:
        //not know syntax
    bounded_queue(std::size_t capacity):capacity(capacity){}    
    bounded_queue(const bounded_queue&) = delete;
    bounded_queue(bounded_queue&&) = delete;

    bounded_queue& operator=(const bounded_queue&) = delete;
    bounded_queue& operator=(bounded_queue&&) = delete;


    bool is_closed() const{
        return closed;
    }
    void close(){
        closed=true;
    }




    void push(T&& item){
        {
        std::unique_lock<std::mutex> lk(mutex);
        //not know
        not_full.wait(lk, [this]() {return content.size() < capacity;});
        content.push_back(std::move (item));

        }
        not_empty.notify_one();
        
    }
    

    
    bool try_push(T&& item){
    {
        std::unique_lock<std::mutex> lk(mutex);
        if (content.size()==capacity){
            return false;
        }    
        content.push_back(std::move(item));    
    }
    not_empty.notify_one();
    return true;
}



void pop(T& item){

    {
    std::unique_lock<std::mutex> lk (mutex);
    not_empty.wait(lk, [this]() { return !content.empty(); });
    item=std::move(content.front());
    content.pop_front();
    }
    not_full.notify_one();
}



//if not-empty return immedilaltely
bool try_pop(T& item){

    {
    std::unique_lock<std::mutex> lk(mutex);
    if(content.empty())
    return false;
    item=std::move(content.front());
    content.pop_front();
    }
    not_full.notify_one();
    return true;
}


};









