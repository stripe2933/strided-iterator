/**
 * @file strided_iterator_test.hpp
 * @author LEE KYOUNGHEON (stripe2933@outlook.com)
 * @brief unit test of strided_iterator
 * @version 0.1
 * @date 2022-05-06
 * 
 * @copyright Copyright (c) 2022
 * This code is licensed under MIT license (see LICENSE.txt for details)
 */

#include <strided_iterator.hpp>

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#if __cplusplus > 201703L
TEST(StridedIterator, IsRandomAccessIterator){
    constexpr auto is_random_access_iterator = std::random_access_iterator<strided_iterator<int, 1>>;
    EXPECT_TRUE(is_random_access_iterator);
    
    constexpr auto is_random_access_iterator2 = std::random_access_iterator<strided_iterator<int, 2>>;
    EXPECT_TRUE(is_random_access_iterator2);
    
    constexpr auto is_random_access_iterator3 = std::random_access_iterator<strided_iterator<int, -1>>;
    EXPECT_TRUE(is_random_access_iterator3);
}

TEST(StridedIterator, IsContiguousIterator){
    constexpr auto is_contiguous_iterator = std::contiguous_iterator<strided_iterator<int, 1>>;
    EXPECT_TRUE(is_contiguous_iterator);
    
    constexpr auto is_contiguous_iterator2 = std::contiguous_iterator<strided_iterator<int, 2>>;
    EXPECT_FALSE(is_contiguous_iterator2);
    
    constexpr auto is_contiguous_iterator3 = std::contiguous_iterator<strided_iterator<int, -1>>;
    EXPECT_TRUE(is_contiguous_iterator3);
    
    constexpr auto is_contiguous_iterator4 = std::contiguous_iterator<strided_iterator<int, -2>>;
    EXPECT_FALSE(is_contiguous_iterator4);
}
#endif

TEST(strided_iterator_with_stride_template_parameter, ForLoopOutputTest){
    std::vector<int> v { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    // with stride 1
    std::vector<int> v1;
    for (strided_iterator<int, 1> it = v.begin(); it < v.end(); ++it){
        v1.push_back(*it);
    }
    EXPECT_THAT(v1, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));

    // with stride 2
    std::vector<int> v2;
    for (strided_iterator<int, 2> it = v.begin(); it < v.end(); ++it){
        v2.push_back(*it);
    }
    EXPECT_THAT(v2, ::testing::ElementsAreArray({ 1, 3, 5, 7, 9 }));

    // with stride 2, but starts from v.begin() + 1
    std::vector<int> v3;
    for (strided_iterator<int, 2> it = v.begin() + 1; it < v.end(); ++it){
        v3.push_back(*it);
    }
    EXPECT_THAT(v3, ::testing::ElementsAreArray({ 2, 4, 6, 8, 10 }));

    return;

    // with stride 4 (10 is not multiple of 4)
    std::vector<int> v4;
    for (strided_iterator<int, 4> it = v.begin(); it < v.end(); ++it){
        v4.push_back(*it);
    }
    EXPECT_THAT(v4, ::testing::ElementsAreArray({ 1, 5, 9 }));

    // with stride -1, starts from the rear of v
    std::vector<int> v5;
    for (strided_iterator<int, -1> it = v.end() - 1; it > v.begin() - 1; ++it){
        v5.push_back(*it);
    }
    EXPECT_THAT(v5, ::testing::ElementsAreArray({ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }));

    // with stride -3, starts from the v.end() - 2
    std::vector<int> v6;
    for (strided_iterator<int, -2> it = v.end() - 2; it > v.begin() - 1; ++it){
        v6.push_back(*it);
    }
    EXPECT_THAT(v6, ::testing::ElementsAreArray({ 9, 6, 3 }));

    // with stride 0, it must infinite forever
    std::vector<int> v7;
    int i = 0;
    for (strided_iterator<int, 0> it = v.begin(); it < v.end() && i < 5; ++it, ++i){
        v7.push_back(*it);
    }
    EXPECT_THAT(v7, ::testing::ElementsAreArray({ 1, 1, 1, 1, 1 }));
}

TEST(strided_iterator_with_stride_template_parameter, ForLoopInputTest){
    std::vector<int> v(10);

    // with stride 2
    for (strided_iterator<int, 2> it = v.begin(); it < v.end(); ++it){
        *it = 1;
    }
    EXPECT_THAT(v, ::testing::ElementsAreArray({ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }));

    std::fill(v.begin(), v.end(), 0);

    // with stride 3, starts from v.begin() + 1, differing fill value
    int i = 1;
    for (strided_iterator<int, 3> it = v.begin() + 1; it < v.end(); ++it, ++i){
        *it = i;
    }
    EXPECT_THAT(v, ::testing::ElementsAreArray({ 0, 1, 0, 0, 2, 0, 0, 3, 0, 0 }));
}

TEST(strided_iterator_without_stride_template_parameter, ForLoopOutputTest){
    std::vector<int> v { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    // with stride 1
    std::vector<int> v1;
    for (strided_iterator<int> it { v.begin(), 1 }; it < v.end(); ++it){
        v1.push_back(*it);
    }
    EXPECT_THAT(v1, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));

    // with stride 2
    std::vector<int> v2;
    for (strided_iterator<int> it { v.begin(), 2 }; it < v.end(); ++it){
        v2.push_back(*it);
    }
    EXPECT_THAT(v2, ::testing::ElementsAreArray({ 1, 3, 5, 7, 9 }));

    // with stride 2, but starts from v.begin() + 1
    std::vector<int> v3;
    for (strided_iterator<int> it { v.begin() + 1, 2 }; it < v.end(); ++it){
        v3.push_back(*it);
    }
    EXPECT_THAT(v3, ::testing::ElementsAreArray({ 2, 4, 6, 8, 10 }));

    return;

    // with stride 4 (10 is not multiple of 4)
    std::vector<int> v4;
    for (strided_iterator<int> it { v.begin(), 4 }; it < v.end(); ++it){
        v4.push_back(*it);
    }
    EXPECT_THAT(v4, ::testing::ElementsAreArray({ 1, 5, 9 }));

    // with stride -1, starts from the rear of v
    std::vector<int> v5;
    for (strided_iterator<int> it { v.end() - 1, -1 }; it > v.begin() - 1; ++it){
        v5.push_back(*it);
    }
    EXPECT_THAT(v5, ::testing::ElementsAreArray({ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }));

    // with stride -3, starts from the v.end() - 2
    std::vector<int> v6;
    for (strided_iterator<int> it { v.end() - 2, -2 }; it > v.begin() - 1; ++it){
        v6.push_back(*it);
    }
    EXPECT_THAT(v6, ::testing::ElementsAreArray({ 9, 6, 3 }));

    // with stride 0, it must infinite forever
    std::vector<int> v7;
    int i = 0;
    for (strided_iterator<int> it { v.begin(), 0 }; it < v.end() && i < 5; ++it, ++i){
        v7.push_back(*it);
    }
    EXPECT_THAT(v7, ::testing::ElementsAreArray({ 1, 1, 1, 1, 1 }));
}

TEST(strided_iterator_without_stride_template_parameter, ForLoopInputTest){
    std::vector<int> v(10);

    // with stride 2
    for (strided_iterator<int> it { v.begin(), 2 }; it < v.end(); ++it){
        *it = 1;
    }
    EXPECT_THAT(v, ::testing::ElementsAreArray({ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 }));

    std::fill(v.begin(), v.end(), 0);

    // with stride 3, starts from v.begin() + 1, differing fill value
    int i = 1;
    for (strided_iterator<int> it { v.begin() + 1, 3 }; it < v.end(); ++it, ++i){
        *it = i;
    }
    EXPECT_THAT(v, ::testing::ElementsAreArray({ 0, 1, 0, 0, 2, 0, 0, 3, 0, 0 }));
}