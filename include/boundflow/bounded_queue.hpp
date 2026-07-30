#pragma once 
#include <deque>
#include <mutex>
#include <condition_variable>
#include <cstddef>
#include <utility>
namespace op{
    enum class status{
        SUCCEED,
        closed
    };
}

template<typename T>
class bounded_queue{
private:
    std::deque<T> content;
    std::size_t capacity;
    mutable std::mutex mutex;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    bool closed=false;
    
public:
        
    bounded_queue(std::size_t capacity):capacity(capacity){}    
    bounded_queue(const bounded_queue&) = delete;
    bounded_queue(bounded_queue&&) = delete;

    bounded_queue& operator=(const bounded_queue&) = delete;
    bounded_queue& operator=(bounded_queue&&) = delete;


    bool is_closed() const{
        //WE DONT WANT CLSOE FUCNTON TO CALL WHEN SOEMTHIGN ELE IS CALLING CLSOE 
        std::lock_guard<std::mutex> lk(mutex);
        return closed;
    }


    bool is_empty(){
        std::lock_guard<std::mutex> lk(mutex);
        if(content.size()>0){
            return false;
        }
        else{
            return true;
        }

    }


    void close(){

        {
        
        std::lock_guard<std::mutex> lk(mutex);
        closed=true;
        }
        not_full.notify_all();
        not_empty.notify_all();
    }
    op::status push(T&& item){
        {
        std::unique_lock<std::mutex> lk(mutex);
        
        not_full.wait(lk, [this]() {return closed ||  content.size() < capacity;});
        if (closed ){
            return op::status::closed ;
        }
        content.push_back(std::move (item));

        }
        not_empty.notify_one();
        return op::status::SUCCEED;
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




op::status pop(T& item)
{
    std::unique_lock<std::mutex> lk(mutex);

    not_empty.wait(lk, [this]() {
        return closed || !content.empty();
    });

    
    if (content.empty()) {
        return op::status::closed;
    }



    
    item = std::move(content.front());
    content.pop_front();

    lk.unlock();
    not_full.notify_one();

    return op::status::SUCCEED;
}



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



















