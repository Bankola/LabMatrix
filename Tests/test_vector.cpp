#include "pch.h"

#define MEMDATA_TESTS
#define VECTOR_TESTS

#ifdef MEMDATA_TESTS
#include "../MemData/memdata.h"

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

#endif