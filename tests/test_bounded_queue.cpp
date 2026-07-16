
#include <gtest/gtest.h>
#include "boundflow/bounded_queue.hpp"
TEST(BoundedQueueStateTest, NewlyConstructedQueueIsOpen)
{
    bounded_queue<int> queue(2);
    const bool result=queue.is_closed();
    EXPECT_FALSE(result); 
}

TEST(BoundedQueueStateTest, QueueIsClosedAfterClose)
{
    bounded_queue<int> queue(2);
    queue.close();
    const bool result=queue.is_closed();
    EXPECT_TRUE(result); 

}

TEST(BoundedQueueStateTest, QueueReturnSuccessAfterPush)
{
    bounded_queue<std::string> queue(6);
    queue.push(std::string ("hello"));
    std::string item;
    queue.pop(item);
    EXPECT_EQ(item,"hello");
}









