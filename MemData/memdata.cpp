#include "memdata.h"

//функции вне класса
int calculate_capacity(size_t size) {
	size_t new_capacity = size / MEM_STEP;
	return (new_capacity + 1) * MEM_STEP;
}

//конструкторы
template <typename vector_type>
MemData<vector_type>::MemData(size_t size) {								//по размеру + по умолчанию
	_data = nullptr;
	set_memory(size);
	_size = 0;
}
template <typename vector_type>
MemData<vector_type>::MemData(std::initializer_list<vector_type> list) {	//по списку инициализации
	_data = nullptr;
	set_memory(list.size());
	for (size_t i = 0; i < _size; i++) {
		_data[i] = *(list.begin() + i);
	}
}
template <typename vector_type>
MemData<vector_type>::MemData(const vector_type* array, size_t size) {		//инициализации
	_size = size;
	_data = nullptr;
	set_memory(_size);
	for (size_t i = 0; i < _size; i++) {
		_data[i] = array[i];
	}
}
template <typename vector_type>
MemData<vector_type>::MemData(const MemData& other) {						//копирования
	_size = other._size;
	_data = nullptr;
	set_memory(_size);
	for (size_t i = 0; i < _size; i++) {
		_data[i] = other._data[i];
	}
}
template <typename vector_type>
MemData<vector_type>::MemData(MemData&& other) noexcept {					//с move-семантикой
	_size = other._size;
	_capacity = other._capacity;
	_data = other._data;
	other._size = 0;
	other._capacity = 0;
	other._data = nullptr;
}

//деструктор
template <typename vector_type>
MemData<vector_type>::~MemData() {
	if (_data) {
		delete[] _data;
	}
}

//сеттеры памяти
template <typename vector_type>
void MemData<vector_type>::set_memory(size_t size) noexcept {				//установка без сохр
	_size = size;
	_capacity = calculate_capacity(size);
	if (_data) {
		delete[] _data;
	}
	_data = new vector_type[_capacity];
}
template <typename vector_type>
void MemData<vector_type>::reset_memory(size_t size, size_t start_index) {	//перевыделение с сохранением данных
	if (start_index >= size) {
		throw std::invalid_argument("ERROR: Can't reset memory! start_index >= size");
	}
	size_t new_capacity = calculate_capacity(size);
	size_t old_capacity = _capacity;
	size_t copy_size = _size;
	_size = size;
	if (old_capacity == new_capacity) {
		if (start_index == 0) {
			return;
		}
		vector_type* temp = new vector_type[start_index];
		for (size_t i = 0; i < start_index; i++) {
			temp[i] = _data[i];
		}
		for (size_t i = 0; i < copy_size; i++) {
			if (i + start_index < old_capacity) {
				_data[i] = _data[(i + start_index)];
			}
			else {
				_data[i] = temp[(i + start_index) % old_capacity];
			}
		}
		return;
	}
	_capacity = new_capacity;
	vector_type * temp = new vector_type[_capacity];
	for (size_t i = 0; i < copy_size; i++) {
		if (i >= _size) {
			break;
		}
		temp[i] = _data[(i + start_index) % old_capacity];
	}
	delete[]_data;
	_data = temp;
}
template <typename vector_type>
void MemData<vector_type>::clear_memory() noexcept {						//очистка памяти
	set_memory(0);
}

//операторы
template <typename vector_type>
MemData<vector_type>& MemData<vector_type>::operator=(const MemData& other) noexcept {	//присваивания
	if (this != &other) {
		_size = other._size;
		_capacity = other._capacity;
		if (_data) {
			delete[] _data;
		}
		_data = new vector_type[_capacity];
		for (size_t i = 0; i < _size; i++) {
			_data[i] = other._data[i];
		}
	}
	return *this;
}
template <typename vector_type>
MemData<vector_type>& MemData<vector_type>::operator=(MemData&& other) noexcept {		//присваивания с move-семантикой
	if (this != &other) {
		_size = other._size;
		_capacity = other._capacity;
		delete[] _data;
		_data = other._data;
	}
	other._size = 0;
	other._capacity = 0;
	other._data = nullptr;
	return *this;
}
template <typename vector_type>
bool MemData<vector_type>::operator==(const MemData& other) const noexcept {			//сравнения
	if (_size != other._size) {
		return false;
	}
	else if (_data == other._data) {
		return true;
	}
	else if (_size == 0) {
		return true;
	}
	else {
		for (size_t i = 0; i < _size; i++) {
			if (_data[i] != other._data[i]) {
				return false;
			}
		}
		return true;
	}
}

//дружественные функции
template <typename vector_type>
void quick_sort(MemData<vector_type>& md) {				//метод сортировки Хоара
	if (md._size <= 1) {
		return;
	}
	quick_sort_recursive(md._data, 0, md._size - 1);
}
template <typename vector_type>
void shuffle(MemData<vector_type>& md) {				//перемешивание Фишер-Йетса
	if (md._size <= 1) {
		return;
	}
	srand(time(NULL));
	for (size_t i = md._size - 1; i > 0; i--) {
		size_t j = rand() % (i + 1);
		std::swap(md._data[i], md._data[j]);
	}
}

//функции вне класса
template <typename vector_type>
int partition(vector_type* data, int left, int right) { //разбиение по Хоару
	vector_type pivot = data[(left + right) / 2]; //опорный эл-т
	int i = left;
	int j = right;
	while (i <= j) {
		while (data[i] < pivot) i++;
		while (data[j] > pivot) j--;
		if (i <= j) {
			std::swap(data[i], data[j]);
			i++;
			j--;
		}
	}
	return i;
}
template <typename vector_type>
void quick_sort_recursive(vector_type* data, int left, int right) {
	if (left < right) {
		int pivot_index = partition(data, left, right);
		quick_sort_recursive(data, left, pivot_index - 1);
		quick_sort_recursive(data, pivot_index, right);
	}
}

//инстанцирование шаблона (генерация объектного файла под определенный тд):
template class MemData<double>;
template void quick_sort(MemData<double>&);
template void shuffle(MemData<double>&);