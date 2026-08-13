/* LFQTests.cpp */

#include "RingBuffer.hpp"
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <cstddef>
#include <thread>

static constexpr int DEFAULT_BUFFER_CAPACITY = 1024;

TEST(RingBufferTest, PushTest) {
    RingBuffer<int, DEFAULT_BUFFER_CAPACITY> rb;
    for(int i{0}; i < DEFAULT_BUFFER_CAPACITY; ++i) {
        EXPECT_TRUE(rb.Push(i));
    }

    // should be full now
    EXPECT_FALSE(rb.Push(67));
}

TEST(RingBufferTest, PopTest1) {
    RingBuffer<int, DEFAULT_BUFFER_CAPACITY> rb;

    // should be full now
    for(int i{0}; i < DEFAULT_BUFFER_CAPACITY; ++i) {
        EXPECT_TRUE(rb.Push(i));
    }

    for(int i{0}; i < DEFAULT_BUFFER_CAPACITY; ++i) {
        int val;
        EXPECT_TRUE(rb.Pop(val)); // val is reassigned to i in pop
        EXPECT_EQ(val, i);
    }
}

TEST(RingBufferTest, PopTest2) {
    RingBuffer<int, DEFAULT_BUFFER_CAPACITY> rb;

    // should be full now
    for(int i{0}; i < DEFAULT_BUFFER_CAPACITY; ++i) {
        EXPECT_TRUE(rb.Push(i));
    }

    std::vector<int> randomsChosen;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, DEFAULT_BUFFER_CAPACITY - 1);
    bool randFound = false;

    for(int i{0}; i < DEFAULT_BUFFER_CAPACITY; ++i) {
        int randNum = distrib(gen);
        for(std::size_t j{0}; j < randomsChosen.size(); ++j) {
            if(randomsChosen.at(j) == randNum) {
                randFound = true;
                break;
            }
        }

        if(!randFound) {
            EXPECT_TRUE(rb.Pop(randNum));
            randomsChosen.emplace_back(randNum);
        }
        randFound = false;
    }
}

TEST(RingBufferTest, MultithreadingTest) {
    RingBuffer<int, DEFAULT_BUFFER_CAPACITY> rb;

    std::thread producer([](RingBuffer<int, DEFAULT_BUFFER_CAPACITY>& rb){
        int i = 0, max = 1'000'000;
        while(i < max) {
            if(rb.Push(i)) {
                ++i;
            }
            // spin and retry at the same i
        }
    }, std::ref(rb));
    
    std::vector<int> received; 
    std::thread consumer([](RingBuffer<int, DEFAULT_BUFFER_CAPACITY>& rb, std::vector<int>& received){
        int count = 0, max = 1'000'000;
        while(count < max) {
            int val;
            if(rb.Pop(val)) {
                received.emplace_back(val);
                ++count;
            }
            // spin and retry at the same i
        }
    }, std::ref(rb), std::ref(received));

    producer.join();
    consumer.join();

    ASSERT_EQ(received.size(), 1'000'000);
    for(int i{0}; i < 1'000'000; ++i) {
        EXPECT_EQ(received.at(i), i);
    }
}