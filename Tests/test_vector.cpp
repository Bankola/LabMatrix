#include "pch.h"
#define MEMDATA_TESTS
#define VECTOR_TESTS
#include <sstream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef MEMDATA_TESTS
#include "../MemData/memdata.h"
#include "../Vector/mathvector.h"
#include "../Vector/matrix.h"

TEST(FunctionsForMemData, can_calculate_capacity) {
    int size1 = 16;
    int size2 = 151;

    EXPECT_EQ(calculate_capacity(size1), MEM_STEP * 2);
    EXPECT_EQ(calculate_capacity(size2), MEM_STEP * 11);
}

TEST(ClassMemData, can_create_with_default_constructor) {
    MemData<double> D1;

    EXPECT_EQ(D1.get_size(), 0);
    EXPECT_EQ(D1.get_capacity(), MEM_STEP);
}

TEST(ClassMemData, can_create_with_constructor_by_size) {
    MemData<double> D1(10);
    MemData<double> D2(1231336);

    EXPECT_EQ(D1.get_size(), 0);
    EXPECT_EQ(D1.get_capacity(), MEM_STEP);
    EXPECT_EQ(D2.get_size(), 0);
    EXPECT_EQ(D2.get_capacity(), (1231336 / MEM_STEP + 1) * MEM_STEP);
}

TEST(ClassMemData, can_create_with_constructor_by_initializer_list) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    MemData<double> D2({});
    double example1[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

    EXPECT_EQ(D1.get_size(), 16);
    EXPECT_EQ(D1.get_capacity(), MEM_STEP * 2);
    EXPECT_EQ(D2.get_size(), 0);
    EXPECT_EQ(D2.get_capacity(), MEM_STEP);
    for (size_t i = 0; i < D1.get_size(); i++) {
        EXPECT_EQ(D1.get_data_const()[i], example1[i]);
    }
}

TEST(ClassMemData, can_create_with_init_constructor) {
    double* list1 = new double[16];
    double* list2 = new double[0];
    MemData<double> D1(list1, 16);
    MemData<double> D2(list2, 0);

    EXPECT_EQ(D1.get_size(), 16);
    EXPECT_EQ(D1.get_capacity(), MEM_STEP * 2);
    EXPECT_EQ(D2.get_size(), 0);
    EXPECT_EQ(D2.get_capacity(), MEM_STEP);
    for (size_t i = 0; i < D1.get_size(); i++) {
        EXPECT_EQ(D1.get_data_const()[i], list1[i]);
    }
    for (size_t i = 0; i < D2.get_size(); i++) {
        EXPECT_EQ(D2.get_data_const()[i], list2[i]);
    }
}

TEST(ClassMemData, can_create_with_copy_constructor) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    MemData<double> D2(D1);
    MemData<double> D3;

    EXPECT_TRUE(D1 == D2);
    EXPECT_FALSE(D1 == D3);
    EXPECT_FALSE(D1.get_data_const() == D2.get_data_const());
}

TEST(ClassMemData, can_create_with_move_constructor) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    MemData<double> D4(D1);
    MemData<double> D2(std::move(D1));
    MemData<double> D3;

    EXPECT_FALSE(D1 == D2);
    EXPECT_TRUE(D1 == D3);
    EXPECT_FALSE(D2 == D3);
    EXPECT_TRUE(D1.get_data_const() == nullptr);
    EXPECT_TRUE(D2 == D4);
}

TEST(ClassMemData, can_is_empty) {
    MemData<double> D1;
    MemData<double> D2(0);
    MemData<double> D3(1);
    MemData<double> D4({ 1,2,3 });
    MemData<double> D5({ 1,2,3,0 });
    double* list1 = new double[3];
    list1[0] = 1;
    list1[1] = 2;
    list1[2] = 3;
    double* list2 = new double[3];
    list2[0] = 4;
    list2[1] = 5;
    list2[2] = 6;
    MemData<double> D6(list1, 3);
    MemData<double> D7(list2, 3);

    EXPECT_TRUE(D1.is_empty());
    EXPECT_TRUE(D2.is_empty());
    EXPECT_TRUE(D3.is_empty());
    EXPECT_FALSE(D4.is_empty());
    EXPECT_FALSE(D5.is_empty());
    EXPECT_FALSE(D6.is_empty());
    EXPECT_FALSE(D7.is_empty());
}

//TEST(ClassMemData, can_is_full) { //убрали, лишний

TEST(ClassMemData, can_set_memory_for_empty) {
    MemData<double> D1;
    D1.set_memory(1000);

    EXPECT_EQ(D1.get_capacity(), (1000 / MEM_STEP + 1) * MEM_STEP);
}

TEST(ClassMemData, can_set_memory_for_not_empty) {
    MemData<double> D1({ 1,2,3 });
    D1.set_memory(1000);

    EXPECT_EQ(D1.get_capacity(), (1000 / MEM_STEP + 1) * MEM_STEP);
}

TEST(ClassMemData, can_reset_memory_for_empty) {
    MemData<double> D1;
    double* old_data = new double[D1.get_capacity()];
    size_t old_size = D1.get_size();
    for (size_t i = 0; i < old_size; i++) {
        old_data[i] = (D1.get_data_const()[i]);
    }
    D1.reset_memory(1000, 0);

    for (size_t i = 0; i < old_size; i++) {
        EXPECT_EQ(D1.get_data_const()[i], old_data[i]);
    }
    EXPECT_EQ(D1.get_capacity(), (1000 / MEM_STEP + 1) * MEM_STEP);
}

TEST(ClassMemData, can_reset_memory_for_not_empty_increase) {
    MemData<double> D1({ 1,2,3,4,5 });
    double* old_data = new double[D1.get_capacity()];
    size_t old_size = D1.get_size();
    for (size_t i = 0; i < old_size; i++) {
        old_data[i] = (D1.get_data_const()[i]);
    }
    D1.reset_memory(1000);

    for (size_t i = 0; i < old_size; i++) {
        EXPECT_EQ(D1.get_data_const()[i], old_data[i]);
    }
    EXPECT_EQ(D1.get_capacity(), (1000 / MEM_STEP + 1) * MEM_STEP);
}

TEST(ClassMemData, can_reset_memory_for_not_empty_decrease) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    int* old_data = new int[D1.get_capacity()];
    size_t old_size = D1.get_size();
    for (size_t i = 0; i < old_size; i++) {
        old_data[i] = (D1.get_data_const()[i]);
    }

    size_t new_size = 13;
    size_t new_cap = calculate_capacity(new_size);

    D1.reset_memory(new_size);

    for (size_t i = 0; i < new_size; i++) {
        EXPECT_EQ(D1.get_data_const()[i], old_data[i]);
    }
    for (size_t i = new_size; i < new_cap; i++) {
        EXPECT_NE(D1.get_data_const()[i], old_data[i]);
    }
    EXPECT_EQ(D1.get_capacity(), new_cap);
}

TEST(ClassMemData, can_reset_memory_without_reallocation) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    size_t old_capacity = D1.get_capacity();
    const double* old_data = D1.get_data_const();

    D1.reset_memory(D1.get_size());

    EXPECT_EQ(D1.get_data_const(), old_data);
    EXPECT_EQ(D1.get_capacity(), old_capacity);
}

TEST(ClassMemData, can_reset_memory_with_shift) {
    size_t start_index = 3;
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    double* old_data = new double[D1.get_capacity()];
    size_t old_size = D1.get_size();
    for (size_t i = 0; i < old_size; i++) {
        old_data[i] = (D1.get_data_const()[i]);
    }

    size_t new_size = 14;
    size_t new_cap = calculate_capacity(new_size);

    D1.reset_memory(new_size, start_index);

    for (size_t i = 0; i < new_size; i++) {
        EXPECT_NE(D1.get_data_const()[i], old_data[i]);
    }
    for (size_t i = 0; i < start_index; i++) {
        EXPECT_EQ(D1.get_data_const()[i], old_data[(i + start_index) % old_size]);
    }
    for (size_t i = start_index; i < new_size; i++) {
        EXPECT_EQ(D1.get_data_const()[i - start_index], old_data[i]);
    }
    EXPECT_EQ(D1.get_capacity(), new_cap);
}

TEST(ClassMemData, can_clear_memory_for_empty) {
    MemData<double> D1;
    D1.set_size(3);
    const double* old_data = D1.get_data_const();
    D1.clear_memory();

    EXPECT_EQ(D1.get_size(), 0);
    EXPECT_EQ(D1.get_capacity(), MEM_STEP);
}

TEST(ClassMemData, can_clear_memory_for_not_empty) {
    MemData<double> D1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    const double* old_data = D1.get_data_const();
    D1.clear_memory();

    EXPECT_EQ(D1.get_size(), 0);
    EXPECT_EQ(D1.get_capacity(), MEM_STEP);
}

TEST(ClassMemData, can_set_size) {
    MemData<double> D1;
    double* list1 = new double[3];
    list1[0] = 1;
    list1[1] = 2;
    list1[2] = 3;
    MemData<double> D2(list1, 3);
    MemData<double> D3({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });

    EXPECT_EQ(D1.get_size(), 0);
    EXPECT_EQ(D2.get_size(), 3);

    D2.set_size(1);

    EXPECT_EQ(D2.get_size(), 1);
    EXPECT_EQ(D3.get_size(), 15);
    ASSERT_THROW(D3.set_size(31), std::invalid_argument);
    ASSERT_NO_THROW(D3.set_size(30));
}

TEST(ClassMemData, can_compare) {
    MemData<double> D1(123);
    MemData<double> D2(10);
    double* list1 = new double[3];
    list1[0] = 1;
    list1[1] = 2;
    list1[2] = 3;
    double* list2 = new double[3];
    list2[0] = 1;
    list2[1] = 2;
    list2[2] = 3;
    MemData<double> D4({ 1,2,3 });
    MemData<double> D5({ 1,2,3 });
    MemData<double> D6(list1, 3);
    MemData<double> D7(list2, 3);

    EXPECT_TRUE(D1 == D2);
    EXPECT_TRUE(D5 == D4);
    EXPECT_TRUE(D6 == D4);
    EXPECT_TRUE(D7 == D4);
}

TEST(ClassMemData, can_assigment) {
    MemData<double> D1(123);
    MemData<double> D2(10);
    MemData<double> D3 = D1 = D2;
    double* list1 = new double[3];
    list1[0] = 1;
    list1[1] = 2;
    list1[2] = 3;
    double* list2 = new double[3];
    list2[0] = 4;
    list2[1] = 5;
    list2[2] = 6;
    MemData<double> D4({ 1,2,3 });
    MemData<double> D5({ 101,2,3 });
    MemData<double> D6(list1, 3);
    MemData<double> D7(list2, 3);
    D7 = D6 = D5 = D4;

    EXPECT_TRUE(D1 == D2);
    EXPECT_TRUE(D3 == D2);
    EXPECT_TRUE(D2 == D1);
    EXPECT_TRUE(D5 == D4);
    EXPECT_TRUE(D6 == D4);
    EXPECT_TRUE(D7 == D4);

    EXPECT_TRUE(D1.get_data_const() != D2.get_data_const());
    EXPECT_TRUE(D2.get_data_const() != D3.get_data_const());
    EXPECT_TRUE(D3.get_data_const() != D4.get_data_const());
    EXPECT_TRUE(D5.get_data_const() != D6.get_data_const());
    EXPECT_TRUE(D6.get_data_const() != D7.get_data_const());
}

TEST(ClassMemData, can_move_assigment) {
    MemData<double> D1({ 1,2,3,4,5 });
    MemData<double> D2;
    MemData<double> D3({ 1,2,3,4,5 });
    D2 = std::move(D1);

    EXPECT_TRUE(D1.get_data_const() == nullptr);
    EXPECT_TRUE(D2.get_data_const() != nullptr);
    EXPECT_TRUE(D2 == D3);
}

TEST(FunctionsForMemData, can_quick_sort) {
    MemData<double> D1({ 6,5,4,3,2,1 });
    MemData<double> D2({ 2,2,2,2,2 });
    MemData<double> D3({ 2,3,2,4,100000,1 });

    quick_sort(D1);
    quick_sort(D2);
    quick_sort(D3);

    double example1[6] = { 1,2,3,4,5,6 };
    double example2[5] = { 2,2,2,2,2 };
    double example3[6] = { 1,2,2,3,4,100000 };
    srand(time(NULL));

    size_t random_size = (static_cast<size_t>(rand() % 100)) + 1;
    double* random_array = new double[random_size];

    for (size_t i = 0; i < random_size; i++) {
        double zero_to_one = static_cast<double>(rand()) / RAND_MAX;
        random_array[i] = zero_to_one * 200.0 - 100.0;
    }
    MemData<double> DR(random_array, random_size);
    quick_sort(DR);

    for (size_t i = 0; i < DR.get_size() - 1; i++) {
        EXPECT_TRUE(DR.get_data_const()[i] <= DR.get_data_const()[i + 1]);
    }
    for (size_t i = 0; i < D1.get_size(); i++) {
        EXPECT_EQ(D1.get_data_const()[i], example1[i]);
    }
    for (size_t i = 0; i < D2.get_size(); i++) {
        EXPECT_EQ(D2.get_data_const()[i], example2[i]);
    }
    for (size_t i = 0; i < D3.get_size(); i++) {
        EXPECT_EQ(D3.get_data_const()[i], example3[i]);
    }
}

TEST(FunctionsForMemData, can_shuffle) {
    MemData<double> D1({ 6,5,4,3,2,1 });
    MemData<double> D2({ 2,3,2,4,100000,1 });
    MemData<double> D3;

    ASSERT_NO_THROW(shuffle(D3));

    double example1[6] = { 6,5,4,3,2,1 };
    double example2[6] = { 2,3,2,4,100000,1 };

    bool shuffled_flag1 = false;
    bool shuffled_flag2 = false;

    for (size_t i = 0; i < 1000; i++) {
        shuffle(D1);
        shuffle(D2);
        for (size_t i = 0; i < D1.get_size(); i++) {
            if (D1.get_data_const()[i] != example1[i]) {
                shuffled_flag1 = true;
                break;
            }
        }
        for (size_t i = 0; i < D2.get_size(); i++) {
            if (D2.get_data_const()[i] != example2[i]) {
                shuffled_flag2 = true;
                break;
            }
        }
        if (shuffled_flag1 && shuffled_flag2) {
            break;
        }
    }
    EXPECT_TRUE(shuffled_flag1 && shuffled_flag2);
}

#endif

#ifdef VECTOR_TESTS
#include "../Vector/vector.h"

TEST(ClassVector, can_create_with_default_constructor) {
    Vector<double> V1;

    EXPECT_EQ(V1.get_size(), 0);
    EXPECT_EQ(V1.get_capacity(), MEM_STEP);
}

TEST(ClassVector, can_create_with_constructor_by_size) {
    Vector<double> V1(10);
    Vector<double> V2(0);
    Vector<double> V3(1000);

    EXPECT_EQ(V1.get_size(), 0);
    EXPECT_EQ(V1.get_capacity(), MEM_STEP);
    EXPECT_EQ(V2.get_size(), 0);
    EXPECT_EQ(V2.get_capacity(), MEM_STEP);
    EXPECT_EQ(V3.get_size(), 0);
    EXPECT_EQ(V3.get_capacity(), (1000 / MEM_STEP + 1) * MEM_STEP);
}

TEST(ClassVector, can_create_with_constructor_by_initializer_list) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    Vector<double> V2({});
    double example1[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

    EXPECT_EQ(V1.get_size(), 16);
    EXPECT_EQ(V1.get_capacity(), MEM_STEP * 2);
    EXPECT_EQ(V2.get_size(), 0);
    EXPECT_EQ(V2.get_capacity(), MEM_STEP);
    for (size_t i = 0; i < V1.get_size(); i++) {
        EXPECT_EQ(V1[i], example1[i]);
    }
}

TEST(ClassVector, can_create_with_init_constructor) {
    double* list1 = new double[16];
    for (int i = 0; i < 16; i++) {
        list1[i] = i;
    }
    Vector<double> V1(list1, 16);

    EXPECT_EQ(V1.get_size(), 16);
    EXPECT_EQ(V1.get_capacity(), MEM_STEP * 2);
    for (size_t i = 0; i < V1.get_size(); i++) {
        EXPECT_EQ(V1[i], list1[i]);
    }
}

TEST(ClassVector, can_create_with_copy_constructor) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    Vector<double> V2(V1);
    double example1[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

    EXPECT_EQ(V1.get_size(), V2.get_size(), 16);
    EXPECT_EQ(V1.get_capacity(), V2.get_capacity(), MEM_STEP * 2);
    for (size_t i = 0; i < V1.get_size(); i++) {
        EXPECT_EQ(V1[i], example1[i]);
    }
    for (size_t i = 0; i < V2.get_size(); i++) {
        EXPECT_EQ(V2[i], example1[i]);
    }
}

TEST(ClassVector, can_create_with_move_constructor) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    const double* old_V1_data_ptr = V1.get_mem_original().get_data_const();
    Vector<double> V2(std::move(V1));

    EXPECT_NE(V1.get_mem_original().get_data_const(), old_V1_data_ptr);
    EXPECT_EQ(V1.get_mem_original().get_data_const(), nullptr);
    EXPECT_EQ(V2.get_mem_original().get_data_const(), old_V1_data_ptr);
}

TEST(ClassVector, can_is_empty) {
    Vector<double> V1;
    Vector<double> V2(0);
    Vector<double> V3(1);
    Vector<double> V4({ 1,2,3 });
    Vector<double> V5({ 1,2,3,0 });
    double* list1 = new double[3];
    list1[0] = 1;
    list1[1] = 2;
    list1[2] = 3;
    double* list2 = new double[3];
    list2[0] = 4;
    list2[1] = 5;
    list2[2] = 6;
    Vector<double> V6(list1, 3);
    Vector<double> V7(list2, 3);

    EXPECT_TRUE(V1.is_empty());
    EXPECT_TRUE(V2.is_empty());
    EXPECT_TRUE(V3.is_empty());
    EXPECT_FALSE(V4.is_empty());
    EXPECT_FALSE(V5.is_empty());
    EXPECT_FALSE(V6.is_empty());
    EXPECT_FALSE(V7.is_empty());
}

//TEST(ClassVector, can_is_full) { //убрали тест как и у MemData<double>

TEST(ClassVector, can_get_front) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    Vector<double> V2({ 1000,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    EXPECT_DOUBLE_EQ(V1.get_front(), 1);
    EXPECT_DOUBLE_EQ(V2.get_front(), 1000);
}

TEST(ClassVector, can_get_back) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    Vector<double> V2({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,19000 });
    EXPECT_DOUBLE_EQ(V1.get_back(), 16);
    EXPECT_DOUBLE_EQ(V2.get_back(), 19000);
}

TEST(ClassVector, can_set_front) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    EXPECT_DOUBLE_EQ(V1.get_front(), 1);
    V1.front_ref() = 11;
    EXPECT_DOUBLE_EQ(V1.get_front(), 11);
}

TEST(ClassVector, can_set_back) {
    Vector<double> V1({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    EXPECT_DOUBLE_EQ(V1.get_back(), 16);
    V1.back_ref() = 13;
    EXPECT_DOUBLE_EQ(V1.get_back(), 13);
}

TEST(ClassVector, throw_when_try_get_front_in_empty_vector) {
    Vector<double> V1;
    ASSERT_THROW(V1.get_front(), std::logic_error);
}

TEST(ClassVector, throw_when_try_get_back_in_empty_vector) {
    Vector<double> V1;
    ASSERT_THROW(V1.get_back(), std::logic_error);
}

TEST(ClassVector, throw_when_try_set_front_in_empty_vector) {
    Vector<double> V1;
    ASSERT_THROW(V1.front_ref() = 1, std::logic_error);
}

TEST(ClassVector, throw_when_try_set_back_in_empty_vector) {
    Vector<double> V1;
    ASSERT_THROW(V1.back_ref() = 1, std::logic_error);
}

TEST(ClassVector, can_output_with_operator_cout) {
    Vector<double> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    std::stringstream out;
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }", out.str());
}

TEST(ClassVector, can_input_with_operator_cin) {
    Vector<double> vec;
    std::stringstream in("9 1 2 3 4 5 6 7 8 9");
    in >> vec;

    EXPECT_EQ(9, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());

    for (size_t i = 0; i < vec.get_size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1);
    }
}

TEST(ClassVector, can_push_front) {
    Vector<double> vec({ 44, 5, 7, 8 });
    std::stringstream out;
    for (size_t i = 0; i < 4; i++) {
        vec.push_front(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8 }", out.str());
    EXPECT_EQ(8, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_push_front_many) {
    Vector<double> vec({ 44, 5, 7, 8 });
    std::stringstream out;
    double list[6] = { 0,1,2,3,4,5 };
    vec.push_front_many(list, 4);
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8 }", out.str());
    EXPECT_EQ(8, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_push_front_in_empty_vector) {
    Vector<double> vec;
    std::stringstream out;
    EXPECT_EQ(0, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    for (size_t i = 0; i < 4; i++) {
        vec.push_front(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_push_front_many_in_empty_vector) {
    Vector<double> vec;
    std::stringstream out;
    double list[6] = { 0,1,2,3,4,5 };
    vec.push_front_many(list, 4);
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_push_front_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
    std::stringstream out;
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    for (size_t i = 0; i < 3; i++) {
        vec.push_front(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }", out.str());
    EXPECT_EQ(17, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
}

TEST(ClassVector, can_push_front_many_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
    std::stringstream out;
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    double list[6] = { 1,2,3,4,5,6 };
    vec.push_front_many(list, 3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }", out.str());
    EXPECT_EQ(17, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
}

TEST(ClassVector, can_push_back) {
    Vector<double> vec({ 44, 5, 7, 8 });
    std::stringstream out;
    for (size_t i = 0; i < 4; i++) {
        vec.push_back(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 44, 5, 7, 8, 3, 2, 1, 0 }", out.str());
    EXPECT_EQ(8, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_push_back_many) {
    Vector<double> vec({ 44, 5, 7, 8 });
    std::stringstream out;
    double list[6] = { 0,1,2,3,4,5 };
    vec.push_back_many(list, 4);
    out << vec;
    EXPECT_EQ("{ 44, 5, 7, 8, 0, 1, 2, 3 }", out.str());
    EXPECT_EQ(8, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_push_back_in_empty_vector) {
    Vector<double> vec;
    std::stringstream out;
    for (size_t i = 0; i < 4; i++) {
        vec.push_back(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 3, 2, 1, 0 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_push_back_many_in_empty_vector) {
    Vector<double> vec;
    std::stringstream out;
    double list[6] = { 0,1,2,3,4,5 };
    vec.push_back_many(list, 4);
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_push_back_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
    std::stringstream out;
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    for (size_t i = 0; i < 3; i++) {
        vec.push_back(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 3, 2, 1 }", out.str());
    EXPECT_EQ(17, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
}

TEST(ClassVector, can_push_back_many_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
    std::stringstream out;
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    double list[3] = { 3,2,1 };
    vec.push_back_many(list, 3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 3, 2, 1 }", out.str());
    EXPECT_EQ(17, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
}

TEST(ClassVector, can_insert) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.insert(99, 2);
    out << vec;
    EXPECT_EQ("{ 1, 2, 99, 3, 4, 5 }", out.str());
    EXPECT_EQ(6, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_insert_many) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    double list[3] = { 99, 100, 101 };
    vec.insert_many(list, 3, 2);
    out << vec;
    EXPECT_EQ("{ 1, 2, 99, 100, 101, 3, 4, 5 }", out.str());
    EXPECT_EQ(8, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_insert_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
    std::stringstream out;
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    vec.insert(99, 7);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 99, 8, 9, 10, 11, 12, 13, 14 }", out.str());
    EXPECT_EQ(15, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
}

TEST(ClassVector, can_insert_many_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14 });
    std::stringstream out;
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    double list[3] = { 99, 100, 101 };
    vec.insert_many(list, 3, 7);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 99, 100, 101, 8, 9, 10, 11, 12, 13, 14 }", out.str());
    EXPECT_EQ(17, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
}

TEST(ClassVector, can_insert_to_front) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.insert(99, 0);
    out << vec;
    EXPECT_EQ("{ 99, 1, 2, 3, 4, 5 }", out.str());
    EXPECT_EQ(6, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_insert_many_to_front) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    double list[3] = { 99, 100, 101 };
    vec.insert_many(list, 3, 0);
    out << vec;
    EXPECT_EQ("{ 99, 100, 101, 1, 2, 3, 4, 5 }", out.str());
    EXPECT_EQ(8, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, throw_when_try_insert_with_wrong_position) {
    Vector<double> vec({ 1,2,3,4,5 });
    EXPECT_THROW(vec.insert(99, 10), std::out_of_range);
    EXPECT_THROW(vec.insert_many(nullptr, 3, 10), std::out_of_range);
}

TEST(ClassVector, can_pop_front) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.pop_front();
    out << vec;
    EXPECT_EQ("{ 2, 3, 4, 5 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_pop_front_many) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.pop_front_many(3);
    out << vec;
    EXPECT_EQ("{ 4, 5 }", out.str());
    EXPECT_EQ(2, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_pop_front_with_reallocation) {
    Vector<double> vec({ 99,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });
    std::stringstream out;
    EXPECT_EQ(15, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
    vec.pop_front();
    out << vec;
    EXPECT_EQ("{ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }", out.str());
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_pop_front_many_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });
    std::stringstream out;
    EXPECT_EQ(15, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
    vec.pop_front_many(3);
    out << vec;
    EXPECT_EQ("{ 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }", out.str());
    EXPECT_EQ(12, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, throw_when_try_pop_front_from_empty_vector) {
    Vector<double> vec;
    EXPECT_THROW(vec.pop_front(), std::logic_error);
}

TEST(ClassVector, throw_when_try_pop_front_many_from_empty_vector) {
    Vector<double> vec;
    EXPECT_THROW(vec.pop_front_many(3), std::logic_error);
}

TEST(ClassVector, can_pop_back) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.pop_back();
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_pop_back_many) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.pop_back_many(3);
    out << vec;
    EXPECT_EQ("{ 1, 2 }", out.str());
    EXPECT_EQ(2, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_pop_back_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });
    std::stringstream out;
    EXPECT_EQ(15, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
    vec.pop_back();
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }", out.str());
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_pop_back_many_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    std::stringstream out;
    EXPECT_EQ(16, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
    vec.pop_back_many(5);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }", out.str());
    EXPECT_EQ(11, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, throw_when_try_pop_back_from_empty_vector) {
    Vector<double> vec;
    EXPECT_THROW(vec.pop_back(), std::logic_error);
}

TEST(ClassVector, throw_when_try_pop_back_many_from_empty_vector) {
    Vector<double> vec;
    EXPECT_THROW(vec.pop_back_many(3), std::logic_error);
}

TEST(ClassVector, can_correctly_recalc_back_in_area_of_zero) {
    Vector<double> vec;

    for (size_t i = 0; i < 14; i++) {
        vec.push_back(i + 1);
    }

    vec.pop_front();
    vec.push_back(15);

    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    EXPECT_DOUBLE_EQ(15.0, vec.get_back());

    for (size_t i = 0; i < vec.get_size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 2);
    }

    vec.pop_back();

    EXPECT_EQ(13, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    EXPECT_DOUBLE_EQ(14.0, vec.get_back());

    for (size_t i = 0; i < vec.get_size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 2);
    }
}

TEST(ClassVector, can_correctly_recalc_front_in_area_of_zero) {
    Vector<double> vec;

    for (size_t i = 0; i < 14; i++) {
        vec.push_back(i + 1);
    }

    vec.pop_back();
    vec.push_front(0);

    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    EXPECT_DOUBLE_EQ(0.0, vec.get_front());

    for (size_t i = 0; i < vec.get_size() - 1; i++) {
        EXPECT_DOUBLE_EQ(vec[i + 1], i + 1);
    }

    vec.pop_front();

    EXPECT_EQ(13, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
    EXPECT_DOUBLE_EQ(1.0, vec.get_front());

    for (size_t i = 0; i < vec.get_size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1);
    }
}

TEST(ClassVector, can_erase) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.erase(2);
    out << vec;
    EXPECT_EQ("{ 1, 2, 4, 5 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_erase_many) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8 });
    std::stringstream out;
    vec.erase_many(2, 3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 6, 7, 8 }", out.str());
    EXPECT_EQ(5, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_erase_front) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.erase(0);
    out << vec;
    EXPECT_EQ("{ 2, 3, 4, 5 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_erase_back) {
    Vector<double> vec({ 1,2,3,4,5 });
    std::stringstream out;
    vec.erase(4);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4 }", out.str());
    EXPECT_EQ(4, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_erase_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });
    std::stringstream out;
    EXPECT_EQ(15, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
    vec.erase(5);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15 }", out.str());
    EXPECT_EQ(14, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, can_erase_many_with_reallocation) {
    Vector<double> vec({ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 });
    std::stringstream out;
    EXPECT_EQ(16, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());
    vec.erase_many(5, 3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 16 }", out.str());
    EXPECT_EQ(13, vec.get_size());
    EXPECT_EQ(15, vec.get_capacity());
}

TEST(ClassVector, throw_when_try_erase_from_empty_vector) {
    Vector<double> vec;
    EXPECT_THROW(vec.erase(0), std::logic_error);
}

TEST(ClassVector, throw_when_try_erase_many_from_empty_vector) {
    Vector<double> vec;
    EXPECT_THROW(vec.erase_many(0, 3), std::logic_error);
}

TEST(ClassVector, throw_when_try_erase_with_wrong_position) {
    Vector<double> vec({ 1,2,3,4,5 });
    EXPECT_THROW(vec.erase(10), std::out_of_range);
}

TEST(ClassVector, throw_when_try_erase_many_with_wrong_count) {
    Vector<double> vec({ 1,2,3,4,5 });
    EXPECT_THROW(vec.erase_many(10, 2), std::logic_error);
    EXPECT_THROW(vec.erase_many(2, 10), std::logic_error);
}

TEST(ClassVector, combination_push_pop_insert_erase) {
    Vector<double> vec({ 3, 44, 5, 7, 8 });

    std::stringstream out;
    out << vec;
    EXPECT_EQ("{ 3, 44, 5, 7, 8 }", out.str());
    out.str("");

    vec.pop_front();
    out << vec;
    EXPECT_EQ("{ 44, 5, 7, 8 }", out.str());
    out.str("");

    for (size_t i = 0; i < 4; i++) {
        vec.push_front(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8 }", out.str());
    out.str("");

    vec.pop_back();
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7 }", out.str());
    out.str("");

    for (size_t i = 0; i < 4; i++) {
        vec.push_back(8 + i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.erase(0);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 44, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.erase(3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.insert(6, 4);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 6, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    for (size_t i = 0; i < 5; i++) {
        vec.push_back(12 + i);
    }
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }", out.str());
    out.str("");

    vec.insert(4, 3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }", out.str());
    out.str("");

    EXPECT_EQ(16, vec.get_size());
    EXPECT_EQ(30, vec.get_capacity());

    for (size_t i = 0; i < vec.get_size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1);
    }
}

TEST(ClassVector, can_assigment) {
    Vector<double> vec_1{ 1,2,3,4 };
    Vector<double> vec_2;

    vec_2 = vec_1;

    EXPECT_EQ(4, vec_1.get_size());
    EXPECT_EQ(15, vec_1.get_capacity());
    EXPECT_EQ(4, vec_2.get_size());
    EXPECT_EQ(15, vec_2.get_capacity());

    for (size_t i = 0; i < vec_2.get_size(); i++) {
        EXPECT_DOUBLE_EQ(vec_1[i], vec_2[i]);
        EXPECT_DOUBLE_EQ(vec_2[i], i + 1);
    }

    vec_1.pop_back();
    EXPECT_EQ(3, vec_1.get_size());
    EXPECT_EQ(4, vec_2.get_size());
}

TEST(ClassVector, can_move_assigment) {
    Vector<double> vec_1;
    Vector<double> vec_2;

    for (size_t i = 0; i < 4; i++) {
        vec_1.push_back(5 + i);
    }

    for (size_t i = 0; i < 4; i++) {
        vec_1.push_front(4 - i);
    }

    vec_2 = std::move(vec_1);

    EXPECT_EQ(0, vec_1.get_size());
    EXPECT_EQ(0, vec_1.get_capacity());

    EXPECT_EQ(8, vec_2.get_size());
    EXPECT_EQ(15, vec_2.get_capacity());

    for (size_t i = 0; i < vec_2.get_size(); i++) {
        EXPECT_DOUBLE_EQ(vec_2[i], i + 1);
    }
}

//shrink_to_fit
TEST(MemDataShrink, ReducesCapacityToExactSize) {
    MemData<double> md{ 1.0, 2.0, 3.0 };
    ASSERT_EQ(md.get_size(), 3u);
    ASSERT_EQ(md.get_capacity(), 15u);         
    md.shrink_to_fit(3, 0);
    EXPECT_EQ(md.get_capacity(), 3u);           
    EXPECT_EQ(md.get_size(), 3u);
    EXPECT_DOUBLE_EQ(md.get_data_const()[0], 1.0);
    EXPECT_DOUBLE_EQ(md.get_data_const()[1], 2.0);
    EXPECT_DOUBLE_EQ(md.get_data_const()[2], 3.0);
}

TEST(MemDataShrink, NoOpWhenSizeEqualsCapacity) {
    MemData<double> md(5);
    size_t cap = md.get_capacity();
    md.shrink_to_fit(cap, 0);
    EXPECT_EQ(md.get_capacity(), cap);
}

TEST(MemDataShrink, NoOpWhenSizeGreaterThanCapacity) {
    MemData<double> md(5);
    size_t cap = md.get_capacity();
    md.shrink_to_fit(cap + 100, 0);
    EXPECT_EQ(md.get_capacity(), cap);
}

TEST(MemDataShrink, ToZero) {
    MemData<double> md{ 1.0, 2.0 };
    md.shrink_to_fit(0, 0);
    EXPECT_EQ(md.get_size(), 0u);
    EXPECT_EQ(md.get_capacity(), 0u);
}

TEST(VectorShrink, ReducesCapacityToSize) {
    Vector<double> v;
    for (int i = 0; i < 3; i++) v.push_back(i + 1.0);
    ASSERT_GT(v.get_capacity(), v.get_size());
    v.shrink_to_fit();
    EXPECT_EQ(v.get_capacity(), v.get_size());
    EXPECT_EQ(v.get_size(), 3u);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(VectorShrink, OnEmptyVector) {
    Vector<double> v;
    v.shrink_to_fit();
    EXPECT_EQ(v.get_capacity(), 0u);
    EXPECT_EQ(v.get_size(), 0u);
}

TEST(VectorShrink, NoOpWhenSizeEqualsCapacityAfterShrink) {
    Vector<double> v;
    for (int i = 0; i < 15; i++) v.push_back(i + 1.0);
    v.shrink_to_fit();                       
    size_t cap = v.get_capacity();
    ASSERT_EQ(cap, v.get_size());           
    v.shrink_to_fit();                        
    EXPECT_EQ(v.get_capacity(), cap);
    EXPECT_EQ(v.get_size(), 15u);
}

TEST(VectorShrink, WithRingOffsetPreservesOrder) {
    Vector<double> v;
    for (int i = 0; i < 10; i++) v.push_back(i + 1.0);
    for (int i = 0; i < 5; i++) v.pop_front();
    ASSERT_GT(v.get_capacity(), v.get_size());
    v.shrink_to_fit();
    EXPECT_EQ(v.get_capacity(), v.get_size());
    EXPECT_EQ(v.get_size(), 5u);
    EXPECT_DOUBLE_EQ(v[0], 6.0);
    EXPECT_DOUBLE_EQ(v[1], 7.0);
    EXPECT_DOUBLE_EQ(v[2], 8.0);
    EXPECT_DOUBLE_EQ(v[3], 9.0);
    EXPECT_DOUBLE_EQ(v[4], 10.0);
}

TEST(VectorShrink, AllowsPushingAfterShrink) {
    Vector<double> v;
    v.push_back(1.0);
    v.push_back(2.0);
    v.push_front(0.0);
    v.shrink_to_fit();
    v.push_back(3.0);
    EXPECT_EQ(v.get_size(), 4u);
    EXPECT_DOUBLE_EQ(v[3], 3.0);
    v.push_front(-1.0);
    EXPECT_EQ(v.get_size(), 5u);
    EXPECT_DOUBLE_EQ(v[0], -1.0);
}

//TMathVector

using Vec = TMathVector<double>;
using Veci = TMathVector<int>;

TEST(TMathVector, DefaultConstructorIsEmpty) {
    Vec v;
    EXPECT_EQ(v.size(), 0u);
    EXPECT_TRUE(v.is_zero());
}

TEST(TMathVector, SizeConstructorIsEmptyWithCapacity) {
    Vec v(3);
    EXPECT_EQ(v.size(), 0u);
    EXPECT_GE(v.get_capacity(), 3u);
}

TEST(TMathVector, FillConstructor) {
    Vec v(3, 2.5);
    Vec vv;
    vv.push_back(2.5);
    vv.push_back(2.5);
    vv.push_back(2.5);
    for (size_t i = 0; i < 3; i++) EXPECT_DOUBLE_EQ(vv[i], 2.5);
}

TEST(TMathVector, InitListConstructor) {
    Vec v{ 1.0, 2.0, 3.0 };
    EXPECT_EQ(v.size(), 3u);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(TMathVector, CopyConstructorIsDeep) {
    Vec a{ 1.0, 2.0, 3.0 };
    Vec b(a);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_DOUBLE_EQ(b[1], 2.0);
    b[0] = 99.0;
    EXPECT_DOUBLE_EQ(a[0], 1.0);   
}

TEST(TMathVector, MoveConstructor) {
    Vec a{ 1.0, 2.0, 3.0 };
    Vec b(std::move(a));
    EXPECT_EQ(b.size(), 3u);
    EXPECT_DOUBLE_EQ(b[1], 2.0);
}

TEST(TMathVector, CopyAssignmentIsDeep) {
    Vec a{ 1.0, 2.0 };
    Vec b;
    b = a;
    b[0] = 99.0;
    EXPECT_DOUBLE_EQ(a[0], 1.0);
    EXPECT_DOUBLE_EQ(b[0], 99.0);
}

TEST(TMathVector, BracketReturnsRef) {
    Vec v{ 1.0, 2.0, 3.0 };
    v[0] = 42.0;
    v[0] += 1.0;
    EXPECT_DOUBLE_EQ(v[0], 43.0);
}

TEST(TMathVector, SizeMatchesGetSize) {
    Vec v{ 1.0, 2.0 };
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v.get_size(), 2u);
}

TEST(TMathVector, InheritedPushBackWorks) {
    Vec v;
    v.push_back(1.0);
    v.push_back(2.0);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
}

TEST(TMathVector, InheritedPushFrontWorks) {
    Vec v;
    v.push_front(2.0);
    v.push_front(1.0);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
}

TEST(TMathVector, InheritedPopFrontWorks) {
    Vec v{ 1.0, 2.0, 3.0 };
    v.pop_front();
    EXPECT_EQ(v.size(), 2u);
    EXPECT_DOUBLE_EQ(v[0], 2.0);
}

TEST(TMathVector, InheritedPopBackWorks) {
    Vec v{ 1.0, 2.0, 3.0 };
    v.pop_back();
    EXPECT_EQ(v.size(), 2u);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
}

TEST(TMathVector, InheritedIsEmpty) {
    Vec v;
    EXPECT_TRUE(v.is_empty());
    v.push_back(1.0);
    EXPECT_FALSE(v.is_empty());
}

TEST(TMathVector, CompoundAdd) {
    Vec a{ 1.0, 2.0, 3.0 };
    a += Vec{ 10.0, 20.0, 30.0 };
    EXPECT_DOUBLE_EQ(a[0], 11.0);
    EXPECT_DOUBLE_EQ(a[1], 22.0);
    EXPECT_DOUBLE_EQ(a[2], 33.0);
}

TEST(TMathVector, CompoundSub) {
    Vec a{ 10.0, 20.0, 30.0 };
    a -= Vec{ 1.0, 2.0, 3.0 };
    EXPECT_DOUBLE_EQ(a[0], 9.0);
    EXPECT_DOUBLE_EQ(a[2], 27.0);
}

TEST(TMathVector, CompoundMul) {
    Vec a{ 1.0, 2.0, 3.0 };
    a *= 2.0;
    EXPECT_DOUBLE_EQ(a[0], 2.0);
    EXPECT_DOUBLE_EQ(a[2], 6.0);
}

TEST(TMathVector, CompoundDiv) {
    Vec a{ 2.0, 4.0, 6.0 };
    a /= 2.0;
    EXPECT_DOUBLE_EQ(a[0], 1.0);
    EXPECT_DOUBLE_EQ(a[2], 3.0);
}

TEST(TMathVector, CompoundDivByZeroThrows) {
    Vec a{ 1.0, 2.0 };
    EXPECT_THROW(a /= 0.0, std::domain_error);
}

TEST(TMathVector, Add) {
    Vec a{ 1.0, 2.0 }, b{ 3.0, 4.0 };
    Vec c = a + b;
    EXPECT_DOUBLE_EQ(c[0], 4.0);
    EXPECT_DOUBLE_EQ(c[1], 6.0);
}

TEST(TMathVector, Sub) {
    Vec a{ 3.0, 4.0 }, b{ 1.0, 2.0 };
    Vec c = a - b;
    EXPECT_DOUBLE_EQ(c[0], 2.0);
    EXPECT_DOUBLE_EQ(c[1], 2.0);
}

TEST(TMathVector, UnaryMinus) {
    Vec a{ 1.0, -2.0, 3.0 };
    Vec b = -a;
    EXPECT_DOUBLE_EQ(b[0], -1.0);
    EXPECT_DOUBLE_EQ(b[1], 2.0);
    EXPECT_DOUBLE_EQ(b[2], -3.0);
}

TEST(TMathVector, ScalarMulRight) {
    Vec a{ 1.0, 2.0 };
    Vec b = a * 3.0;
    EXPECT_DOUBLE_EQ(b[0], 3.0);
    EXPECT_DOUBLE_EQ(b[1], 6.0);
}

TEST(TMathVector, ScalarMulLeft) {
    Vec a{ 1.0, 2.0 };
    Vec b = 3.0 * a;
    EXPECT_DOUBLE_EQ(b[0], 3.0);
    EXPECT_DOUBLE_EQ(b[1], 6.0);
}

TEST(TMathVector, ScalarDiv) {
    Vec a{ 2.0, 6.0 };
    Vec b = a / 2.0;
    EXPECT_DOUBLE_EQ(b[0], 1.0);
    EXPECT_DOUBLE_EQ(b[1], 3.0);
}

TEST(TMathVector, ScalarDivByZeroThrows) {
    Vec a{ 1.0, 2.0 };
    EXPECT_THROW(a / 0.0, std::domain_error);
}

TEST(TMathVector, AddSizeMismatchThrows) {
    Vec a{ 1.0, 2.0 }, b{ 1.0, 2.0, 3.0 };
    EXPECT_THROW(a + b, std::invalid_argument);
}

TEST(TMathVector, SubSizeMismatchThrows) {
    Vec a{ 1.0, 2.0 }, b{ 1.0, 2.0, 3.0 };
    EXPECT_THROW(a - b, std::invalid_argument);
}

TEST(TMathVector, CompoundAddSizeMismatchThrows) {
    Vec a{ 1.0, 2.0 }, b{ 1.0, 2.0, 3.0 };
    EXPECT_THROW(a += b, std::invalid_argument);
}

TEST(TMathVector, CompoundSubSizeMismatchThrows) {
    Vec a{ 1.0, 2.0 }, b{ 1.0, 2.0, 3.0 };
    EXPECT_THROW(a -= b, std::invalid_argument);
}

TEST(TMathVector, Dot) {
    Vec a{ 1.0, 2.0, 3.0 }, b{ 4.0, 5.0, 6.0 };
    EXPECT_DOUBLE_EQ(a.dot(b), 32.0);
}

TEST(TMathVector, DotSizeMismatchThrows) {
    Vec a{ 1.0, 2.0 }, b{ 1.0, 2.0, 3.0 };
    EXPECT_THROW(a.dot(b), std::invalid_argument);
}

TEST(TMathVector, DotWithSelfEqualsNormSquared) {
    Vec a{ 3.0, 4.0 };
    EXPECT_DOUBLE_EQ(a.dot(a), a.norm_squared());
}

TEST(TMathVector, NormSquared) {
    Vec a{ 3.0, 4.0 };
    EXPECT_DOUBLE_EQ(a.norm_squared(), 25.0);
}

TEST(TMathVector, Norm) {
    Vec a{ 3.0, 4.0 };
    EXPECT_DOUBLE_EQ(a.norm(), 5.0);
}

TEST(TMathVector, NormOf3D) {
    Vec a{ 1.0, 2.0, 2.0 };
    EXPECT_DOUBLE_EQ(a.norm(), 3.0);
}

TEST(TMathVector, NormOfZeroIsZero) {
    Vec a(3);
    EXPECT_DOUBLE_EQ(a.norm(), 0.0);
}

TEST(TMathVector, NormalizedSimple) {
    Vec a{ 3.0, 0.0, 0.0 };
    auto n = a.normalized();
    EXPECT_DOUBLE_EQ(n[0], 1.0);
    EXPECT_DOUBLE_EQ(n[1], 0.0);
    EXPECT_DOUBLE_EQ(n[2], 0.0);
}

TEST(TMathVector, NormalizedHasUnitNorm) {
    Vec a{ 1.0, 2.0, 3.0 };
    auto n = a.normalized();
    EXPECT_NEAR(n.norm(), 1.0, 1e-12);
}

TEST(TMathVector, NormalizedPreservesDirection) {
    Vec a{ 3.0, 4.0 };
    auto n = a.normalized();
    EXPECT_NEAR(a.dot(n), a.norm(), 1e-12);
}

TEST(TMathVector, NormalizeZeroThrows) {
    Vec a{ 0.0, 0.0, 0.0 };
    EXPECT_THROW(a.normalized(), std::domain_error);
}

TEST(TMathVector, AngleOrthogonal) {
    Vec a{ 1.0, 0.0, 0.0 }, b{ 0.0, 1.0, 0.0 };
    EXPECT_NEAR(a.angle(b), M_PI / 2.0, 1e-12);
}

TEST(TMathVector, AngleParallel) {
    Vec a{ 1.0, 0.0, 0.0 }, b{ 5.0, 0.0, 0.0 };
    EXPECT_NEAR(a.angle(b), 0.0, 1e-12);
}

TEST(TMathVector, AngleOpposite) {
    Vec a{ 1.0, 0.0, 0.0 }, b{ -1.0, 0.0, 0.0 };
    EXPECT_NEAR(a.angle(b), M_PI, 1e-12);
}

TEST(TMathVector, AngleIsSymmetric) {
    Vec a{ 1.0, 2.0, 3.0 }, b{ 4.0, 5.0, 6.0 };
    EXPECT_NEAR(a.angle(b), b.angle(a), 1e-12);
}

TEST(TMathVector, AngleWithZeroThrows) {
    Vec a{ 1.0, 0.0, 0.0 }, b{ 0.0, 0.0, 0.0 };
    EXPECT_THROW(a.angle(b), std::domain_error);
}

TEST(TMathVector, CrossIJisK) {
    Vec i{ 1.0, 0.0, 0.0 }, j{ 0.0, 1.0, 0.0 };
    Vec k = i.cross(j);
    EXPECT_DOUBLE_EQ(k[0], 0.0);
    EXPECT_DOUBLE_EQ(k[1], 0.0);
    EXPECT_DOUBLE_EQ(k[2], 1.0);
}

TEST(TMathVector, CrossJIisMinusK) {
    Vec i{ 1.0, 0.0, 0.0 }, j{ 0.0, 1.0, 0.0 };
    Vec k = j.cross(i);
    EXPECT_DOUBLE_EQ(k[2], -1.0);
}

TEST(TMathVector, CrossAntiCommutative) {
    Vec a{ 1.0, 2.0, 3.0 }, b{ 4.0, 5.0, 6.0 };
    Vec ab = a.cross(b);
    Vec ba = b.cross(a);
    EXPECT_DOUBLE_EQ(ab[0], -ba[0]);
    EXPECT_DOUBLE_EQ(ab[1], -ba[1]);
    EXPECT_DOUBLE_EQ(ab[2], -ba[2]);
}

TEST(TMathVector, CrossOrthogonalToBoth) {
    Vec a{ 1.0, 2.0, 3.0 }, b{ 4.0, 5.0, 6.0 };
    Vec c = a.cross(b);
    EXPECT_NEAR(a.dot(c), 0.0, 1e-12);
    EXPECT_NEAR(b.dot(c), 0.0, 1e-12);
}

TEST(TMathVector, CrossOfParallelIsZero) {
    Vec a{ 1.0, 2.0, 3.0 }, b{ 2.0, 4.0, 6.0 };   // b = 2*a
    Vec c = a.cross(b);
    EXPECT_TRUE(c.is_zero());
}

TEST(TMathVector, CrossWrongSizeThrows) {
    Vec a{ 1.0, 2.0 }, b{ 3.0, 4.0 };
    EXPECT_THROW(a.cross(b), std::invalid_argument);
}

TEST(TMathVector, CrossNon3DThrows) {
    Vec a{ 1.0, 2.0, 3.0, 4.0 }, b{ 5.0, 6.0, 7.0, 8.0 };
    EXPECT_THROW(a.cross(b), std::invalid_argument);
}

TEST(TMathVector, IsZeroTrue) {
    Vec v(3);
    EXPECT_TRUE(v.is_zero());
}

TEST(TMathVector, IsZeroFalse) {
    Vec v{ 0.0, 1.0, 0.0 };
    EXPECT_FALSE(v.is_zero());
}

TEST(TMathVector, IsZeroEmptyIsTrue) {
    Vec v;
    EXPECT_TRUE(v.is_zero());
}

TEST(TMathVector, BasisE1) {
    Vec e1 = Vec::basis(3, 0);
    EXPECT_EQ(e1.size(), 3u);
    EXPECT_DOUBLE_EQ(e1[0], 1.0);
    EXPECT_DOUBLE_EQ(e1[1], 0.0);
    EXPECT_DOUBLE_EQ(e1[2], 0.0);
}

TEST(TMathVector, BasisE2) {
    Vec e2 = Vec::basis(3, 1);
    EXPECT_DOUBLE_EQ(e2[0], 0.0);
    EXPECT_DOUBLE_EQ(e2[1], 1.0);
    EXPECT_DOUBLE_EQ(e2[2], 0.0);
}

TEST(TMathVector, BasisE3) {
    Vec e3 = Vec::basis(3, 2);
    EXPECT_DOUBLE_EQ(e3[0], 0.0);
    EXPECT_DOUBLE_EQ(e3[1], 0.0);
    EXPECT_DOUBLE_EQ(e3[2], 1.0);
}

TEST(TMathVector, BasisIsUnit) {
    Vec e = Vec::basis(5, 3);
    EXPECT_DOUBLE_EQ(e.norm(), 1.0);
}

TEST(TMathVector, BasisOutOfRangeThrows) {
    EXPECT_THROW(Vec::basis(3, 5), std::out_of_range);
}

TEST(TMathVector, OutputFormat) {
    Vec v{ 1.0, 2.0, 3.0 };
    std::ostringstream oss;
    oss << v;
    EXPECT_EQ(oss.str(), "(1, 2, 3)");
}

TEST(TMathVector, OutputEmpty) {
    Vec v;
    std::ostringstream oss;
    oss << v;
    EXPECT_EQ(oss.str(), "()");
}

TEST(TMathVector, OutputSingle) {
    Vec v{ 5.0 };
    std::ostringstream oss;
    oss << v;
    EXPECT_EQ(oss.str(), "(5)");
}

TEST(TMathVector, IntAdd) {
    Veci a{ 1, 2, 3 }, b{ 4, 5, 6 };
    Veci c = a + b;
    EXPECT_EQ(c[0], 5);
    EXPECT_EQ(c[1], 7);
    EXPECT_EQ(c[2], 9);
}

TEST(TMathVector, IntDot) {
    Veci a{ 1, 2, 3 }, b{ 4, 5, 6 };
    EXPECT_EQ(a.dot(b), 32);
}

TEST(TMathVector, IntScalarMul) {
    Veci a{ 1, 2, 3 };
    Veci b = a * 2;
    EXPECT_EQ(b[0], 2);
    EXPECT_EQ(b[2], 6);
}

TEST(TMathVector, IntBasis) {
    Veci e = Veci::basis(3, 2);
    EXPECT_EQ(e[0], 0);
    EXPECT_EQ(e[1], 0);
    EXPECT_EQ(e[2], 1);
}

TEST(TMathVector, IntCross) {
    Veci i{ 1, 0, 0 }, j{ 0, 1, 0 };
    Veci k = i.cross(j);
    EXPECT_EQ(k[0], 0);
    EXPECT_EQ(k[1], 0);
    EXPECT_EQ(k[2], 1);
}
#endif