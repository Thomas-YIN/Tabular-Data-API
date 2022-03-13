// table-submit.h

#ifndef TABLE_SUBMIT_H_
#define TABLE_SUBMIT_H_

#include "column.h"

#include <string>
#include <iostream>

/// TODO: Add the missing function definitions below

template <>
inline const Column<int>* Table::get_column_view(std::string name) const {
    return get_column(num_int_columns_, int_columns_, name);
}

template <>
inline const Column<std::string>* Table::get_column_view(std::string name) const {
    return get_column(num_string_columns_, string_columns_, name);
}

template <>
inline const Column<bool>* Table::get_column_view(std::string name) const {
    return get_column(num_bool_columns_, bool_columns_, name);
}

template <>
inline void Table::update_at(std::string name, int index, int value) {
    update_at(index, value, num_int_columns_, int_columns_, name);
}

template <>
inline void Table::update_at(std::string name, int index, std::string value) {
    update_at(index, value, num_string_columns_, string_columns_, name);
}

template <>
inline void Table::update_at(std::string name, int index, bool value) {
    update_at(index, value, num_bool_columns_, bool_columns_, name);
}

template <typename T>
void Table::append_records(const Table& other, int& num_columns, Column<T>* columns) {
    for(int i=0; i<num_columns; i++){
        const Column<T>* target_col = other.get_column_view<T>(columns[i].get_name());
        columns[i].append(*target_col);
    }
}

#endif  // TABLE_SUBMIT_H_
