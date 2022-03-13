// column-submit.h

#ifndef COLUMN_SUBMIT_H_
#define COLUMN_SUBMIT_H_

#include <string>
#include <iostream>

/// TODO: Add the definitions of inline global functions below

template <>
inline std::string Column<int>::as_string_at(int index) const {
    return std::to_string((*this)[index]);
}

template <>
inline std::string Column<bool>::as_string_at(int index) const {
    if((*this)[index] == true)
        return "true";
    else return "false";
}

template <>
inline std::string Column<std::string>::as_string_at(int index) const {
    return (*this)[index];
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Column<T>& column) {
    column.operator<<(os);
    return os;
}

/// TODO: Add the missing function definitions of the Column template below

template <typename T>
void Column<T>::reserve_capacity(int new_capacity) {
    //Requested capacity is more than current
    if(new_capacity > capacity_){
        T* temp = new T[new_capacity];
        for(int i=0; i<size_; i++){
            temp[i] = data_[i];
        }
        delete [] data_;
        data_ = temp;
        capacity_ = new_capacity;
    }else{
        return;
    }
}

template <typename T>
void Column<T>::expand_size(int new_size) {
    //Requested size > current size but <= current capacity
    if(new_size > size_ && new_size <= capacity_){
        //Insert default values until meet the requested size
        for(int i=size_; i<new_size; i++){
            data_[i] = default_value_;
        }
        size_ = new_size;
    }else if(new_size > capacity_){
        this->reserve_capacity(new_size);
        for(int i=size_; i<capacity_; i++){
            data_[i] = default_value_;
        }
        size_ = new_size;
    }else{
        return;
    }
}

template <typename T>
void Column<T>::append(const Column<T>& other) {
    if(other.get_size()+size_ <= capacity_){
        for(int i=size_; i<size_+other.get_size(); i++){
            data_[i] = other.at(i-size_);
        }
        size_ += other.get_size();
    }else{
        this->reserve_capacity(other.get_size()+size_);
        for(int i=size_; i<size_+other.get_size(); i++){
            data_[i] = other.at(i-size_);
        }
        size_ += other.get_size();
    }
}

template <typename T>
const Column<T>& Column<T>::operator=(const Column<T>& other) {
    //Memberwise copy of data members
    default_value_ = other.default_value_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    name_ = other.name_;
    //Deep copy of stored data
    T* temp = new T[other.capacity_];
    for(int i=0; i<other.size_; i++){
        temp[i] = other.data_[i];
    }
    //delete original data and copy the new set
    delete [] data_;
    data_ = temp;
    return *this;
}

template <typename T>
Column<T> Column<T>::operator+(const Column<T>& other) const {
    Column<T> result(default_value_, size_, size_);
    for(int i=0; i<size_; i++){
        result.data_[i] = data_[i] + other.data_[i];
    }
    return result;
}

template <typename T>
const Column<T>& Column<T>::operator+=(const Column<T>& other) {
    for(int i=0; i<size_; i++){
        this->data_[i] = this->data_[i] + other.data_[i];
    }
    return *this;
}

template <typename T>
Column<bool> Column<T>::operator==(const Column<T>& other) const {
    Column<bool> result(false, size_, size_);
    for(int i=0; i<size_; i++){
        result.operator[](i) = (data_[i] == other.data_[i]);
    }
    return result;
}

template <typename T>
Column<bool> Column<T>::operator<(const Column<T>& other) const {
    Column<bool> result(false, size_, size_);
    for(int i=0; i<size_; i++){
        result.operator[](i) = (data_[i] < other.data_[i]);
    }
    return result;
}

template <typename T>
Column<bool> Column<T>::operator==(const T& other) const {
    Column<bool> result(false, size_, size_);
    for(int i=0; i<size_; i++){
        result.operator[](i) = (data_[i] == other);
    }
    return result;
}

template <typename T>
Column<bool> Column<T>::operator<(const T& other) const {
    Column<bool> result(false, size_, size_);
    for(int i=0; i<size_; i++){
        result.operator[](i) = (data_[i] < other);
    }
    return result;
}

template <typename T>
Column<bool> Column<T>::operator!() const {
    Column<bool> result(false, size_, size_);
    for(int i=0; i<size_; i++){
        result.operator[](i) = !data_[i];
    }
    return result;
}

template <typename T>
Column<bool> Column<T>::operator&&(const Column<T>& other) const {
    Column<bool> result(false, size_, size_);
    for(int i=0; i<size_; i++){
        result.operator[](i) = data_[i] && other.data_[i];
    }
    return result;
}


template <typename T>
T Column<T>::operator[](int index) const {
    if(index >= 0){
        return data_[index];
    }else{
        return data_[size_+index];
    }
}

template <typename T>
T& Column<T>::operator[](int index) {
    if(index >= 0){
        return data_[index];
    }else{
        return data_[size_+index];
    }
}

template <typename T>
Column<T>::Column(const Column<T>& other, const Column<bool>& selector) : Column(other){
    name_ = "NULL";

    int true_count = 0;
    for(int i=0; i<selector.get_size(); i++){
        if(selector[i] == true){
            true_count++;
        }
    }
    
    T* filter_data = new T[true_count];
    capacity_ = true_count;
    size_ = true_count;
    
    for(int i=0, j=0; i<other.size_; i++){
        if(selector[i] == true){
            filter_data[j] = other[i];
            j++;
        }else{
            continue;
        }
    }

    delete [] data_;
    data_ = filter_data;
    
}

template <typename T>
Column<T> Column<T>::operator[](const Column<bool>& selector) const {
    return Column<T>(*this, selector);
}

#endif  // COLUMN_SUBMIT_H_
