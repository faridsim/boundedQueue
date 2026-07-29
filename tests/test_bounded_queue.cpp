
#include <gtest/gtest.h>
#include "boundflow/bounded_queue.hpp"
#include<future>
//s0
TEST(BoundedQueueStateTest, NewlyConstructedQueueIsEmpty)
{
    bounded_queue<int> queue(2);
    const bool result=queue.is_empty();
    EXPECT_TRUE(result); 
}





//s1
TEST(BoundedQueueStateTest, NewlyConstructedQueueIsOpen)
{
    bounded_queue<int> queue(2);
    const bool result=queue.is_closed();
    EXPECT_FALSE(result); 
}

//s2
TEST(BoundedQueueStateTest, QueueIsClosedAfterClose)
{
    bounded_queue<int> queue(2);
    queue.close();
    const bool result=queue.is_closed();
    EXPECT_TRUE(result); 
}


//s3 and s7 as interaction test together
//s3-push for open and empty 
//s7-pop for open  and non empty
TEST(BoundedQueueStateTest, PushedItemCanBePopped)
{
    bounded_queue<std::string> queue(6);
    queue.push(std::string ("hello"));
    std::string item;
    queue.pop(item);
    EXPECT_EQ(item,"hello");
}
//s4 
//open-full ...>get blocked >open-empty...>unblocked


TEST(BoundedQueueStateTest,  OpenFullQueueBlocksPushUntilSpaceIsAvailable)
{
    //open and empty
    bounded_queue<std::string> queue(1);
    //open and full?
    queue.push("hello");

    auto futrue=std::async(std::launch::async,[&](){return queue.push("alpha");});
    auto status=futrue.wait_for(std::chrono::milliseconds(50));
    //confriming it is blocked 
    EXPECT_EQ(status,std::future_status::timeout);
    //open and empty
    std::string item;
    queue.pop(item);
    //veirfies that ithas been unblocked
    auto result=futrue.get();
    //open and empty works for pop
    EXPECT_EQ(result,op::status::SUCCEED);
}




//s4
TEST(BoundedQueueStateTest,  OpenFullQueueBlocksPushUntilClosed)
{
    //open and empty
    bounded_queue<std::string> queue(1);
    //open and full
    queue.push("hello");

    auto futrue=std::async(std::launch::async,[&](){return queue.push("alpha");});
    auto status=futrue.wait_for(std::chrono::milliseconds(50));
    //confriming it is blocked 
    EXPECT_EQ(status,std::future_status::timeout);
    queue.close();
    //veirfies that ithas been unblocked
    auto result=futrue.get();
    EXPECT_EQ(result,op::status::closed);
}
//s5
TEST(BoundedQueueStateTest, ClosedEmptyQueueReturnClosedAfterPush)
{
    bounded_queue<std::string> queue(6);
    queue.close();
    op::status result=queue.push(std::string("hello"));
    EXPECT_EQ(result,op::status::closed);
}




//s6
TEST(BoundedQueueStateTest, ClosedNonEmptyQueueReturnClosedAfterPush)
{
    bounded_queue<std::string> queue(6);
    queue.push(std::string("hello"));
    queue.close();
    EXPECT_FALSE(queue.is_empty());
    op::status result =queue.push(std::string("farid"));
    EXPECT_EQ(result,op::status::closed);
}
//s8 open and empty ...>blocks ...>open and non empty>blocks
TEST(BoundedQueueStateTest,OpenEmptyQueueBlocksPopUntillElementIsAvailable)
{
    bounded_queue<std::string> queue(6);
    std::string item;
    //we luanch and q is empty
    auto futrue=std::async(std::launch::async,[&](){return queue.pop(item);});
    auto status=futrue.wait_for(std::chrono::milliseconds(50));
    //this verify the blocking behaviour
    EXPECT_EQ(status,std::future_status::timeout);
    queue.push(std::string("hello"));
    //veirfies that ithas been unblocked
    auto result=futrue.get();
    EXPECT_EQ(result,op::status::SUCCEED);
}




//s8 open and empty ...>blcoks ..>closed and empty ...>unblcoks 
TEST(BoundedQueueStateTest,OpenEmptyQueueBlocksPopUntillClosed)
{
    bounded_queue<std::string> queue(6);
    std::string item;
    //we luanch and q is empty
    auto futrue=std::async(std::launch::async,[&](){return queue.pop(item);});
    auto status=futrue.wait_for(std::chrono::milliseconds(50));
    //this verify the blocking behaviour
    EXPECT_EQ(status,std::future_status::timeout);
    queue.close();
    //veirfies that it has been unblocked
    auto result=futrue.get();
    EXPECT_EQ(result,op::status::closed);
}

//s9
TEST(BoundedQueueStateTest, ClosedNonEmptyQueueReturnsSucceedAfterPop)
{
    bounded_queue<std::string> queue(6);

    //closed  and  empty
    queue.push(std::string ("hello"));
    queue.close();

    //closed and non-empty-statrting state of test
    std::string item;
    //EXPECT_FALSE(queue.is_empty());
    auto result=queue.pop(item);
    EXPECT_EQ(item,"hello");
    EXPECT_EQ(result,op::status::SUCCEED);
}



TEST(BoundedQueueStateTest, ClosedEmptyQueuReturnClosedAfterPop)
{
    bounded_queue<std::string> queue(6);
    queue.close();
    std::string item;
    auto result=queue.pop(item);
    EXPECT_EQ(result,op::status::closed);
}









































//does push  prove non-emptyin interaction test?
//consist of several inner states,that each one has to be proved
//pop works with push as inetraction test in closed and non empty state 
//does not move item after closing 
//why push works after close ?


//question 
//how can quee be empty after push but still pop bring hello














//push does not verify that it is non -empty
//









/*

TEST(BoundedQueueStateTest, ClosedorEmptyQueueReturnClosedAfterPop)
{
    bounded_queue<std::string> queue(6);
    std::string item;
    auto futrue=std::async(std::launch::async,[&](){return queue.pop(item);});
    auto status=futrue.wait_for(std::chrono::milliseconds(50));
    EXPECT_EQ(status,std::future_status::timeout);
    queue.close();

}
*/
































































































