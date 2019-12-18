#pragma once
// Do not add or modify anything in this file.

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "Exceptions.h"

template <typename T>
class Comparator {
public:
	virtual int compare(const T& item1, const T& item2) const = 0;
	virtual ~Comparator() = default;
};

#endif //! COMPARATOR_H