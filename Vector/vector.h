#pragma once
#include <iostream>
#include<cstdlib>
#include "../MemData/memdata.h"

template<typename vector_type>
class Vector {
    MemData<vector_type> _mem;	// хранилище данных + размер  + вместимость
    size_t _front;				// индекс первого элемента
    size_t _back;				// индекс последнего элемента

public:
	//конструкторы
    Vector(size_t size = 0);					// конструктор по размеру + по умолчанию
    Vector(std::initializer_list<vector_type>); // конструктор по списку инициализации
    Vector(vector_type*, size_t);               // конструктор инициализации
    Vector(const Vector<vector_type>&);         // конструктор копирования
    Vector(Vector<vector_type>&&) noexcept;     // конструктор с move-семантикой
	//деструктор
    ~Vector() = default; //напишется компилятором

	//публичные методы проверок
	inline bool is_empty() const noexcept {			// на пустоту
		return _mem.is_empty();
	}
	inline bool is_full() const noexcept {			// на переполнение
		return _mem.is_full();
	}

	//геттеры
	inline size_t get_size() const noexcept {		// размера
		return _mem._size;
	}
	inline size_t get_capacity() const noexcept {	// вместимости
		return _mem._capacity;
	}
	inline vector_type get_front() const {			// первого элемента (возвр. копию)
		if (_mem._size != 0) {
			return (_mem._data)[_front];
		}
		else {
			throw std::logic_error("ERROR: Vector is empty! Can't get front");
		}
	}
	inline vector_type get_back() const {			// последнего элемента (возвр. копию)
		if (_mem._size != 0) {
			return (_mem._data)[_back];
		}
		else {
			throw std::logic_error("ERROR: Vector is empty! Can't get back");
		}
	}
	inline MemData<vector_type> get_mem_copy() const noexcept {            // копии мемдаты (для тестов)
		return _mem;
	}
	inline const MemData<vector_type>& get_mem_original() const noexcept { // оригинала мемдаты (для тестов на move)
		return _mem;
	}

	//сеттеры
	inline vector_type& front_ref() {				// первого элемента (возвр. ссылку)
		if (_mem._size != 0) {
			return (_mem._data)[_front];
		}
		else {
			throw std::logic_error("ERROR: Vector is empty! Can't set front");
		}
	}
	inline vector_type& back_ref() {				// последнего элемента (возвр. ссылку)
		if (_mem._size != 0) {
			return (_mem._data)[_back];
		}
		else {
			throw std::logic_error("ERROR: Vector is empty! Can't set back");
		}
	}

	//публичные методы вставок
    void push_front(vector_type) noexcept;					// 1 элемента в начало
    void push_front_many(vector_type*, size_t) noexcept;	// нескольких в начало
    void push_back(vector_type) noexcept;					// 1 элемента в конец
    void push_back_many(vector_type*, size_t) noexcept;		// нескольких в конец
    void insert(vector_type, size_t);						// 1 элемента по позиции
    void insert_many(vector_type*, size_t, size_t);			// нескольких по позиции

	//удалений
    void pop_front();                               // 1 элемента из начала
    void pop_front_many(size_t);					// нескольких из начала
    void pop_back();                                // 1 элемента из конца
    void pop_back_many(size_t);						// нескольких из конца
    void erase(size_t);                             // 1 элемента по позиции
    void erase_many(size_t, size_t);				// нескольких по позиции

	//перегрузки операторов
    Vector<vector_type>& operator=(const Vector<vector_type>&) noexcept;      // присваивания
    Vector<vector_type>& operator=(Vector<vector_type>&&) noexcept;           // присваивания с move-семантикой
	vector_type operator[](size_t) const noexcept;       // обращения по индексу (не изм., возвр. копию)
    vector_type& operator[](size_t) noexcept;            // обращения по индексу (изм., возвр. ссылку)

	//дружественные функции
	//перегрузки ввода-вывода
	template <typename vector_type>
	friend std::ostream& operator<< (std::ostream&, const Vector<vector_type>&);	// вывода
	template <typename vector_type>
	friend std::istream& operator>> (std::istream&, Vector<vector_type>&);			// ввода
	//сортировки и перемешивания
	template <typename vector_type>
    friend void quick_sort(Vector<vector_type>&);		//сортировки (Хоара)
	template <typename vector_type>
	friend void shuffle(Vector<vector_type>&);	//перемешивания (Фишер-Йетса)

private:
	//служебный метод получения (геттер) физического индекса по относительному
	inline size_t get_mem_index(size_t i) const {
		return ((i + _front) % _mem._capacity);
	}
};