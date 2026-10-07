#include "vector.h"

//конструкторы
template<typename vector_type>
Vector<vector_type>::Vector(size_t size) {
	MemData<vector_type> temp(size);
	_mem = (std::move(temp));
	_front = 0;
	if (size > 0) {
		_back = size - 1;
	}
	else {
		_back = 0;
	}
}
template<typename vector_type>
Vector<vector_type>::Vector(std::initializer_list<vector_type> list) {
	MemData<vector_type> temp(list);
	_mem = (std::move(temp));
	_front = 0;
	if (_mem._size > 0) {
		_back = _mem._size - 1;
	}
	else {
		_back = 0;
	}
}
template<typename vector_type>
Vector<vector_type>::Vector(vector_type* array, size_t size) {
	MemData<vector_type> temp(array, size);
	_mem = (std::move(temp));
	_front = 0;
	if (_mem._size > 0) {
		_back = _mem._size - 1;
	}
	else {
		_back = 0;
	}
}
template<typename vector_type>
Vector<vector_type>::Vector(const Vector<vector_type>& other) {
	_mem = other._mem;
	_front = other._front;
	_back = other._back;
}
template<typename vector_type>
Vector<vector_type>::Vector(Vector<vector_type>&& other) noexcept {
	_mem = std::move(other._mem);
	_front = other._front;
	other._front = 0;
	_back = other._back;
	other._back = 0;
}

//публичные методы
//вставок
template<typename vector_type>
void Vector<vector_type>::push_front(vector_type element) noexcept {
	_mem._size++;
	if (_mem._size > 1) {
		_front = (_front + _mem._capacity - 1) % _mem._capacity;
	}
	_mem._data[_front] = element;
	if (is_full()) {
		_mem.reset_memory(_mem._size, _front);
		_front = 0;
		_back = _mem._size - 1;
	}
}
template<typename vector_type>
void Vector<vector_type>::push_front_many(vector_type* elements, size_t size) noexcept {
	for (int i = size - 1; i >= 0; i--) { //идем с конца тк кладем в начало
		push_front(elements[i]);
	}
}
template<typename vector_type>
void Vector<vector_type>::push_back(vector_type element) noexcept {
	_mem._size++;
	if (_mem._size > 1) {
		_back = (_back + 1) % _mem._capacity;
	}
	_mem._data[_back] = element;
	if (is_full()) {
		_mem.reset_memory(_mem._size, _front);
		_front = 0;
		_back = _mem._size - 1;
	}
}
template<typename vector_type>
void Vector<vector_type>::push_back_many(vector_type* elements, size_t size) noexcept {
	for (int i = 0; i < size; i++) { //идем с начала тк кладем в конец
		push_back(elements[i]);
	}
}
template<typename vector_type>
void Vector<vector_type>::insert(vector_type element, size_t i) {
	if (i > _mem._size) {
		throw std::out_of_range("ERROR: Insert index out of range!");
	}
	else {
		if (i == 0) {
			push_front(element);
		}
		else if (i == _mem._size) {
			push_back(element);
		}
		else {
			_mem._size++;
			_back = (_back + 1) % _mem._capacity;
			for (int j = _mem._size - 1; j > i; j--) {
				(*this)[j] = (*this)[j - 1];
			}
			(*this)[i] = element;
			if (is_full()) {
				_mem.reset_memory(_mem._size, _front);
				_front = 0;
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename vector_type>
void Vector<vector_type>::insert_many(vector_type* elements, size_t size, size_t i) {
	if (i > _mem._size) {
		throw std::out_of_range("ERROR: Insert (many) index out of range!");
	}
	else {
		for (int j = 0; j < size; j++) {
			insert(elements[j], i + j);
		}
	}
}

//удалений
template<typename vector_type>
void Vector<vector_type>::pop_front() {
	if (_mem._size == 0) {
		throw std::logic_error("ERROR: Empty vector! Can't pop front");
	}
	else {
		_mem._size--;
		if (_mem._size != 0) {
			_front = (_front + 1) % _mem._capacity;
		}
		else {
			_front = 0;
			_back = 0;
		}
		if (_mem._capacity - _mem._size > MEM_STEP) {
			_mem.reset_memory(_mem._size, _front);
			_front = 0;
			if (_mem._size != 0) {
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename vector_type>
void Vector<vector_type>::pop_front_many(size_t count) {
	if (_mem._size < count) {
		throw std::logic_error("ERROR: Popping (front) too many elements!");
	}
	else {
		_mem._size-=count;
		if (_mem._size != 0) {
			_front = (_front + count) % _mem._capacity;
		}
		else {
			_front = 0;
			_back = 0;
		}
		if (_mem._capacity - _mem._size > MEM_STEP) {
			_mem.reset_memory(_mem._size, _front);
			_front = 0;
			if (_mem._size != 0) {
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename vector_type>
void Vector<vector_type>::pop_back() {
	if (_mem._size == 0) {
		throw std::logic_error("ERROR: Empty vector! Can't pop back");
	}
	else {
		_mem._size--;
		if (_mem._size != 0) {
			_back = (_back + _mem._capacity - 1) % _mem._capacity;
		}
		else {
			_front = 0;
			_back = 0;
		}
		if (_mem._capacity - _mem._size > MEM_STEP) {
			_mem.reset_memory(_mem._size, _front);
			_front = 0;
			if (_mem._size != 0) {
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename vector_type>
void Vector<vector_type>::pop_back_many(size_t count) {
	if (_mem._size < count) {
		throw std::logic_error("ERROR: Popping (back) too many elements!");
	}
	else {
		_mem._size-=count;
		if (_mem._size != 0) {
			_back = (_back + _mem._capacity - count) % _mem._capacity;
		}
		else {
			_front = 0;
			_back = 0;
		}
		if (_mem._capacity - _mem._size > MEM_STEP) {
			_mem.reset_memory(_mem._size, _front);
			_front = 0;
			if (_mem._size != 0) {
				_back = _mem._size - 1;
			}
		}
	}
}
template<typename vector_type>
void Vector<vector_type>::erase(size_t i) {
	if (i >= _mem._size) {
		throw std::out_of_range("ERROR: Erase index out of range!");
	}
	else {
		if (i == 0) {
			pop_front();
		}
		else if (i == _mem._size - 1) {
			pop_back();
		}
		else {
			_back = (_back - 1 + _mem._capacity) % _mem._capacity;
			for (int j = i; j < _mem._size - 1; j++) {
				(*this)[j] = (*this)[j + 1];
			}
			_mem._size--;
			if (_mem._size == 0) {
				_front = 0;
				_back = 0;
			}
			if (_mem._capacity - _mem._size > MEM_STEP) {
				_mem.reset_memory(_mem._size, _front);
				_front = 0;
				_back = _mem._size == 0 ? 0 : _mem._size - 1;
			}
		}
	}
}
template<typename vector_type>
void Vector<vector_type>::erase_many(size_t i, size_t count) {
	if (i + count > _mem._size) {
		throw std::logic_error("ERROR: Erasing too many elements!");
	}
	for (int j = 0; j < count; j++) {
		erase(i);
	}
}

template<typename vector_type>
void Vector<vector_type>::shrink_to_fit() {
	if (_mem._size == _mem._capacity) return;
	_mem.shrink_to_fit(_mem._size, _front);
	_front = 0;
	_back = (_mem._size == 0) ? 0 : _mem._size - 1;
}

//перегрузки операторов
template<typename vector_type>
Vector<vector_type>& Vector<vector_type>::operator=(const Vector<vector_type>& other) noexcept {
	if (this != &other) {
		_mem = other._mem;
		_front = other._front;
		_back = other._back;
	}
	return (*this);
}
template<typename vector_type>
Vector<vector_type>& Vector<vector_type>::operator=(Vector<vector_type>&& other) noexcept {
	if (this != &other) {
		_mem = std::move(other._mem);
		_front = other._front;
		other._front = 0;
		_back = other._back;
		other._back = 0;
	}
	return (*this);
}
template<typename vector_type>
vector_type Vector<vector_type>::operator[](size_t i) const noexcept {
	return _mem._data[(*this).get_mem_index(i)];
}
template<typename vector_type>
vector_type& Vector<vector_type>::operator[](size_t i) noexcept {
		return _mem._data[(*this).get_mem_index(i)];
}

//дружественные функции
//сортировки и перемешивания
template<typename vector_type>
void quick_sort(Vector<vector_type>& vector) {
	quick_sort(vector._mem);
}
template<typename vector_type>
void shuffle(Vector<vector_type>& vector) {
	shuffle(vector._mem);
}
//перегрузки ввода-вывода
template <typename vector_type>
std::ostream& operator<< (std::ostream& out, const Vector<vector_type>& vector) {	// вывода
	out << "{ ";
	size_t size = vector.get_size();
	if (size != 0) {
		out << vector._mem.get_data_const()[vector._front];
	}
	for (size_t i = 1; i < size; i++) {
		out << ", " << vector._mem.get_data_const()[vector.get_mem_index(i)];
	}
	out << " }";
	return out;
};
template <typename vector_type>
std::istream& operator>> (std::istream& in, Vector<vector_type>& vector) {			// ввода
	Vector<vector_type> temp;
	vector_type element;
	in >> element;
	while (in >> element) {
		temp.push_back(element);
	}
	vector = std::move(temp);
	return in;
};

//инстанцирование шаблона (генерация объектного файла под определенный тд):
template class Vector<double>;
template std::ostream& operator<< (std::ostream& out, const Vector<double>& v1);
template std::istream& operator>> (std::istream& in, Vector<double>& v1);
template void shuffle(Vector<double>&);
template void quick_sort(Vector<double>&);