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
#include <algorithm>
#include <numeric>
#include <array>
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
    for (const_strided_iterator<int, -3> it = v.cend() - 2; it > v.cbegin() - 1; ++it){
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

TEST(const_strided_iterator_with_stride_template_parameter, STLCompatibilityTest){
    std::vector<int> v { 1, -2, 3, -4, 5, -6, 7, -8, 9, -10 };

    // Accumulation

    int sum = std::accumulate<const_strided_iterator<int, 1>, int>(v.cbegin(), v.cend(), 0);
    EXPECT_EQ(sum, -5);

    int odd_sum = std::accumulate<const_strided_iterator<int, 2>, int>(v.cbegin(), v.cend(), 0);
    EXPECT_EQ(odd_sum, 25);

    /*
        BE AWARE!

        The last-first must be the multiple of Stride, because STL iterates the iterator with != (not eq) condition, i.e.
        for (auto it = first; it != last; ++it){ // <- compare with != operator
            // use *it
        }
    */
    int even_sum = std::accumulate<const_strided_iterator<int, 2>, int>(v.cbegin() + 1, v.cend() + 1, 0);
    EXPECT_EQ(even_sum, -30);

    // Matrix Multiplication

    std::array<double, 2*3> matrix2x3 { 
        2, 1, 4, 
        0, 1, 1,
    };
    std::array<double, 3*4> matrix3x4 { 
        6, 3, -1, 0, 
        1, 1, 0, 4, 
        -2, 5, 0, 2,
    };
    std::array<double, 2*4> multiple;
    for (std::size_t i = 0; i < multiple.size(); ++i){
        std::size_t row = i / 4, column = i % 4;
        multiple[i] = std::inner_product<const_strided_iterator<double, 1>, const_strided_iterator<double, 4>>(
            matrix2x3.cbegin() + 3 * row, matrix2x3.cbegin() + 3 * (row + 1),
            matrix3x4.cbegin() + column, 
            0
        );
    }
    EXPECT_THAT(multiple, ::testing::ElementsAreArray({ 
        5, 27, -2, 12, 
        -1, 6, 0, 6,
    }));
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
    for (const_strided_iterator<int> it { v.cend() - 2, -3 }; it > v.cbegin() - 1; ++it){
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

TEST(const_strided_iterator_without_stride_template_parameter, STLCompatibilityTest){
    std::vector<int> v { 1, -2, 3, -4, 5, -6, 7, -8, 9, -10 };

    // Accumulation

    int sum = std::accumulate(const_strided_iterator<int>{ v.cbegin(), 1 }, const_strided_iterator<int>{ v.cend(), 1 }, 0);
    EXPECT_EQ(sum, -5);

    int odd_sum = std::accumulate(const_strided_iterator<int>{ v.cbegin(), 2 }, const_strided_iterator<int>{ v.cend(), 2 }, 0);
    EXPECT_EQ(odd_sum, 25);

    /*
        BE AWARE!

        The last-first must be the multiple of Stride, because STL iterates the iterator with != (not eq) condition, i.e.
        for (auto it = first; it != last; ++it){ // <- compare with != operator
            // use *it
        }
    */
    int even_sum = std::accumulate(const_strided_iterator<int>{ v.cbegin() + 1, 2 }, const_strided_iterator<int>{ v.cend() + 1, 2 }, 0);
    EXPECT_EQ(even_sum, -30);

    // Matrix Multiplication
    
    std::array<double, 2*3> matrix2x3 { 
        2, 1, 4, 
        0, 1, 1,
    };
    std::array<double, 3*4> matrix3x4 { 
        6, 3, -1, 0, 
        1, 1, 0, 4, 
        -2, 5, 0, 2,
    };
    std::array<double, 2*4> multiple;
    for (std::size_t i = 0; i < multiple.size(); ++i){
        std::size_t row = i / 4, column = i % 4;
        multiple[i] = std::inner_product(
            matrix2x3.cbegin() + 3 * row, matrix2x3.cbegin() + 3 * (row + 1),
            const_strided_iterator<double>{ matrix3x4.cbegin() + column, 4 }, 
            0
        );
    }
    EXPECT_THAT(multiple, ::testing::ElementsAreArray({ 
        5, 27, -2, 12, 
        -1, 6, 0, 6,
    }));
}