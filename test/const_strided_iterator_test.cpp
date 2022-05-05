/**
 * @file const_strided_iterator_test.hpp
 * @author LEE KYOUNGHEON (stripe2933@outlook.com)
 * @brief unit test of const_strided_iterator
 * @version 0.1
 * @date 2022-05-06
 * 
 * @copyright Copyright (c) 2022
 * This code is licensed under MIT license (see LICENSE.txt for details)
 */

#include <const_strided_iterator.hpp>

#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#if __cplusplus > 201703L
TEST(StridedIterator, IsRandomAccessIterator){
    constexpr auto is_random_access_iterator = std::random_access_iterator<const_strided_iterator<int, 1>>;
    EXPECT_TRUE(is_random_access_iterator);
    
    constexpr auto is_random_access_iterator2 = std::random_access_iterator<const_strided_iterator<int, 2>>;
    EXPECT_TRUE(is_random_access_iterator2);
    
    constexpr auto is_random_access_iterator3 = std::random_access_iterator<const_strided_iterator<int, -1>>;
    EXPECT_TRUE(is_random_access_iterator3);
}

TEST(StridedIterator, IsContiguousIterator){
    constexpr auto is_contiguous_iterator = std::contiguous_iterator<const_strided_iterator<int, 1>>;
    EXPECT_TRUE(is_contiguous_iterator);
    
    constexpr auto is_contiguous_iterator2 = std::contiguous_iterator<const_strided_iterator<int, 2>>;
    EXPECT_FALSE(is_contiguous_iterator2);
    
    constexpr auto is_contiguous_iterator3 = std::contiguous_iterator<const_strided_iterator<int, -1>>;
    EXPECT_TRUE(is_contiguous_iterator3);
    
    constexpr auto is_contiguous_iterator4 = std::contiguous_iterator<const_strided_iterator<int, -2>>;
    EXPECT_FALSE(is_contiguous_iterator4);
}
#endif

TEST(const_strided_iterator_with_stride_template_parameter, ForLoopOutputTest){
    const std::vector<int> v { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    // with stride 1
    std::vector<int> v1;
    for (const_strided_iterator<int, 1> it = v.cbegin(); it < v.cend(); ++it){
        v1.push_back(*it);
    }
    EXPECT_THAT(v1, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));

    // with stride 2
    std::vector<int> v2;
    for (const_strided_iterator<int, 2> it = v.cbegin(); it < v.cend(); ++it){
        v2.push_back(*it);
    }
    EXPECT_THAT(v2, ::testing::ElementsAreArray({ 1, 3, 5, 7, 9 }));

    // with stride 2, but starts from v.cbegin() + 1
    std::vector<int> v3;
    for (const_strided_iterator<int, 2> it = v.cbegin() + 1; it < v.cend(); ++it){
        v3.push_back(*it);
    }
    EXPECT_THAT(v3, ::testing::ElementsAreArray({ 2, 4, 6, 8, 10 }));

    return;

    // with stride 4 (10 is not multiple of 4)
    std::vector<int> v4;
    for (const_strided_iterator<int, 4> it = v.cbegin(); it < v.cend(); ++it){
        v4.push_back(*it);
    }
    EXPECT_THAT(v4, ::testing::ElementsAreArray({ 1, 5, 9 }));

    // with stride -1, starts from the rear of v
    std::vector<int> v5;
    for (const_strided_iterator<int, -1> it = v.cend() - 1; it > v.cbegin() - 1; ++it){
        v5.push_back(*it);
    }
    EXPECT_THAT(v5, ::testing::ElementsAreArray({ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }));

    // with stride -3, starts from the v.cend() - 2
    std::vector<int> v6;
    for (const_strided_iterator<int, -2> it = v.cend() - 2; it > v.cbegin() - 1; ++it){
        v6.push_back(*it);
    }
    EXPECT_THAT(v6, ::testing::ElementsAreArray({ 9, 6, 3 }));

    // with stride 0, it must infinite forever
    std::vector<int> v7;
    int i = 0;
    for (const_strided_iterator<int, 0> it = v.cbegin(); it < v.cend() && i < 5; ++it, ++i){
        v7.push_back(*it);
    }
    EXPECT_THAT(v7, ::testing::ElementsAreArray({ 1, 1, 1, 1, 1 }));
}

TEST(const_strided_iterator_without_stride_template_parameter, ForLoopOutputTest){
    std::vector<int> v { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    
    // with stride 1
    std::vector<int> v1;
    for (const_strided_iterator<int> it { v.cbegin(), 1 }; it < v.cend(); ++it){
        v1.push_back(*it);
    }
    EXPECT_THAT(v1, ::testing::ElementsAreArray({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));

    // with stride 2
    std::vector<int> v2;
    for (const_strided_iterator<int> it { v.cbegin(), 2 }; it < v.cend(); ++it){
        v2.push_back(*it);
    }
    EXPECT_THAT(v2, ::testing::ElementsAreArray({ 1, 3, 5, 7, 9 }));

    // with stride 2, but starts from v.cbegin() + 1
    std::vector<int> v3;
    for (const_strided_iterator<int> it { v.cbegin() + 1, 2 }; it < v.cend(); ++it){
        v3.push_back(*it);
    }
    EXPECT_THAT(v3, ::testing::ElementsAreArray({ 2, 4, 6, 8, 10 }));

    return;

    // with stride 4 (10 is not multiple of 4)
    std::vector<int> v4;
    for (const_strided_iterator<int> it { v.cbegin(), 4 }; it < v.cend(); ++it){
        v4.push_back(*it);
    }
    EXPECT_THAT(v4, ::testing::ElementsAreArray({ 1, 5, 9 }));

    // with stride -1, starts from the rear of v
    std::vector<int> v5;
    for (const_strided_iterator<int> it { v.cend() - 1, -1 }; it > v.cbegin() - 1; ++it){
        v5.push_back(*it);
    }
    EXPECT_THAT(v5, ::testing::ElementsAreArray({ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }));

    // with stride -3, starts from the v.cend() - 2
    std::vector<int> v6;
    for (const_strided_iterator<int> it { v.cend() - 2, -2 }; it > v.cbegin() - 1; ++it){
        v6.push_back(*it);
    }
    EXPECT_THAT(v6, ::testing::ElementsAreArray({ 9, 6, 3 }));

    // with stride 0, it must infinite forever
    std::vector<int> v7;
    int i = 0;
    for (const_strided_iterator<int> it { v.cbegin(), 0 }; it < v.cend() && i < 5; ++it, ++i){
        v7.push_back(*it);
    }
    EXPECT_THAT(v7, ::testing::ElementsAreArray({ 1, 1, 1, 1, 1 }));
}