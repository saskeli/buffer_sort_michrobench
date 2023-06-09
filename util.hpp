#pragma once

#include <cstdint>
#include <iostream>

class be {
    static const constexpr uint32_t IDX_BITS = 24;
    static const constexpr uint32_t MASK = (uint32_t(1) << IDX_BITS) - 1;
    uint32_t v;

    be(uint32_t i) : v(i) {}

    public:
    be(uint32_t i, bool val) : v(i | (val << IDX_BITS)) {}
    be() : v(0) {}

    bool operator<(be& rhs) const {
        return this->idx() < rhs.idx();
    }

    bool operator<=(const be& rhs) {
        return this->idx() <= rhs.idx();
    }

    bool operator>(const be& rhs) const {
        return this->idx() > rhs.idx();
    }

    bool operator>=(const be& rhs) const {
        return this->idx() >= rhs.idx();
    }

    uint32_t idx() const {
        return v & MASK;
    }

    operator uint32_t() {
        return idx();
    }

    bool val() const {
        return v >> IDX_BITS;
    }

    be& operator++() {
        ++v;
        return *this;
    }

    be operator+(uint16_t i) {
        return {v + i};
    }

    be operator+(bool i) {
        return {v + uint32_t(i)};
    }

    be& operator+=(uint16_t rhs) {
        v += rhs;
        return *this;
    }
};

std::ostream &operator<<(std::ostream& os, const be& b) {
    os << "(" << b.idx() << ", " << b.val() << ")";
    return os;
}

std::istream &operator>>(std::istream& is, be& b) {
    uint32_t i;
    bool v;
    is >> i >> v;
    b = {i, v};
    return is;
}


typedef be B_type;