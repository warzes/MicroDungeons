#pragma once

template <class TYPE>
class Array {
public:
	Array() {
		count = capacity = 0;
		list = NULL;
	}

	Array(const unsigned int iCapasity) {
		count = 0;
		capacity = iCapasity;
		list = (TYPE*)malloc(capacity * sizeof(TYPE));
	}

	~Array() {
		free(list);
	}

	TYPE* getArray() const { return list; }
	TYPE* abandonArray() {
		TYPE* rList = list;
		list = NULL;
		return rList;
	}

	TYPE& operator [] (const unsigned int index) const { return list[index]; }
	unsigned int getCount() const { return count; }

	void setCount(const unsigned int newCount) {
		capacity = count = newCount;
		list = (TYPE*)realloc(list, capacity * sizeof(TYPE));
	}

	unsigned int add(const TYPE object) {
		if (count >= capacity) {
			if (capacity) capacity += capacity; else capacity = 8;
			list = (TYPE*)realloc(list, capacity * sizeof(TYPE));
		}
		list[count] = object;
		return count++;
	}

	void fastRemove(const unsigned int index) {
		if (index < count) {
			count--;
			list[index] = list[count];
		}
	}

	void orderedRemove(const unsigned int index) {
		if (index < count) {
			count--;
			memmove(list + index, list + index + 1, (count - index) * sizeof(TYPE));
		}
	}

	void clear() {
		count = 0;
	}

	void reset() {
		free(list);
		list = NULL;
		count = capacity = 0;
	}

private:
	int partition(int (*compare)(const TYPE& elem0, const TYPE& elem1), int p, int r) {
		TYPE tmp, pivot = list[p];
		int left = p;

		for (int i = p + 1; i <= r; i++) {
			if (compare(list[i], pivot) < 0) {
				left++;
				tmp = list[i];
				list[i] = list[left];
				list[left] = tmp;
			}
		}
		tmp = list[p];
		list[p] = list[left];
		list[left] = tmp;
		return left;
	}

	void quickSort(int (*compare)(const TYPE& elem0, const TYPE& elem1), int p, int r) {
		if (p < r) {
			int q = partition(compare, p, r);
			quickSort(compare, p, q - 1);
			quickSort(compare, q + 1, r);
		}
	}
public:
	void sort(int (*compare)(const TYPE& elem0, const TYPE& elem1)) {
		quickSort(compare, 0, count - 1);
	}
protected:
	unsigned int capacity;
	unsigned int count;
	TYPE* list;
};