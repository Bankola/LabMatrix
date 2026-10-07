#pragma once
#include <initializer_list>
#include <iostream>
#include <cstdlib>      // дл€ рандома
#include <ctime>
#include <string>       // дл€ std::getline
#include <sstream>      // дл€ std::istringstream

#define MEM_STEP 15     //шаг: сколько выдел€етс€ €чеек пам€ти минимум при добавлении эл-тов

template <typename vector_type>
class Vector;

template <typename vector_type>
class MemData {
    vector_type* _data;        // хранилище данных
    size_t _size;              // размер заполненной части хранилища
    size_t _capacity;          // вместимость хранилища

public: 
    //конструкторы
    MemData(size_t size = 0);                       // по размеру + по умолчанию
    MemData(std::initializer_list<vector_type>);    // по списку инициализации
    MemData(const vector_type*, size_t);            // инициализации
    MemData(const MemData&);                        // копировани€
    MemData(MemData&&) noexcept;                    // с move-семантикой
    //деструктор
    ~MemData();

    //публичные методы проверок
    inline bool is_empty() const noexcept {         // на пустоту
        return (_size == 0);
    }

    //геттеры
    inline size_t get_size() const noexcept {                           // размера
        return _size;
    }
    inline size_t get_capacity() const noexcept {                       // вместимости
        return _capacity;
    }
    inline const vector_type* const get_data_const() const noexcept {   // хранилища (возвр указ-ль по которому Ќ≈Ћ№«я мен€ть)
        return _data;
    }
    inline vector_type* const get_data_changeable() noexcept {          // хранилища (возвр указ-ль по которому ћќ∆Ќќ мен€ть)
        return _data;
    }

    //сеттеры пам€ти и размера заполненной части
    void set_memory(size_t) noexcept;                           // установка пам€ти без сохранени€ данных
    void reset_memory(size_t size, size_t start_index = 0);     // перевыделение пам€ти с сохранением данных (”Ѕ–јЌ NOEXCEPT)
    void clear_memory() noexcept;                               // очистка пам€ти
    inline void set_size(size_t size) {                         //установка размера заполненной части
        if (size > _capacity) {
            throw std::invalid_argument("ERROR: Size is bigger than capacity!");
        }
        else {
            _size = size;
        }
    }

    //операторы
    MemData& operator=(const MemData&) noexcept;         // присваивани€
    MemData& operator=(MemData&&) noexcept;              // присваивани€ с move-семантикой
    bool operator==(const MemData&) const noexcept;      // сравнени€

    //друзь€шки:
    //-функции
    template <typename vector_type> //<----иначе не компилируетс€
    friend void quick_sort(MemData<vector_type>& md);            // сортировки
    template <typename vector_type>
    friend void shuffle(MemData<vector_type>&);                  // перемешивани€

    //-классы
    friend class Vector<vector_type>;

private:
    //служебные методы
    inline bool is_full() const noexcept {      // проверка на переполнение
        return (_size >= _capacity);
    }
};

//функции вне класса
int calculate_capacity(size_t);     //какую вместимость выставить при заданном кол-ве эл-тов
template <typename vector_type>
void quick_sort_recursive(vector_type* data, int left, int right); 
template <typename vector_type>
int partition(vector_type* data, int left, int right);