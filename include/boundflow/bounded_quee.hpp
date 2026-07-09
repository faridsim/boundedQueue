#pragma once  #?
#include <deque>
#include<mutex>
#include<condition_variable>
#include<cstddef> #?
template<typename T>
class bounded_quee{
private:
    std::deque<T> content;
    size_t capicity;
    std::mutex ;
    std::condition_variable not_empty;
    std::condition_variable  not_full;

public:
    //not know syntax
    bounded_quee(size_t capicity):capicity(capicity){} 
    bounded_quee(const quee&)=delete;
    bounded_quee(quee&&)=delete;
    bounded_quee& operator(const quee&)=delete;
    bounded_quee& operator(quee &&)=delete;

    void push(T&& item){
        {
        std::unique_lock<std::mutex> lk(mutex);
        //not know
        not_full.wait(lk, [this]() {return content.size() < capacity;});
        content.push_back(std::move (item));

        }
        not_empty.notify_one();



    }
    void try_push(T&& item){
    {
        std::unique_lock<std::mutex> lk(mutex);
        if (content.size()==capicity){
            return false;
        }        
    }
    not_empty.notify_one();
    retrun true;


}
//if not-empty wait
void pop(T& item){

    {
    std::unique_lock<std::mutex> lk (mutex);
    not_empty.wait(lk, [this]() { return !content.empty(); });
    item=std::move(content.front);
    content.pop_front();
    }
    not_full.notify_one();
}
//if not-empty return immedilaltely
void try_pop(T& item){

    {
    std::unique_lock<std::mutex> lk(mutex);
    if(content.empty())
    return false;
    item=std::move(content.front());
    content.pop_front();
    }
    not_full.notify_one();
}




    

}








