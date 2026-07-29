
#include <gtest/gtest.h>
#include "boundflow/bounded_queue.hpp"
#include<future>

TEST(BoundedQueueStateTest, MultipleOpenEmptyQueuesReturnClosed)
{   //open and empty
    bounded_queue<std::string> queue(6);
    queue.close();
    auto futrue_1=std::async(std::launch::async,[&](){return queue.push("alpha");});
    auto futrue_2=std::async(std::launch::async,





//s9