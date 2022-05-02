#include "General_types.h"

using namespace std;

Side strSide(char s)
{
    if (s == 'V') {
        return Side::VOID;
    }
    else if (s == 'D') {
        return Side::DOOR;
    }
    else if (s == 'W') {
        return Side::WALL;
    }
    else {
        return Side::INVALID;
    }
}


std::ostream& operator<<(std::ostream& o, const strBuffer& buffer)
{
    for (uint_t j = 0; j < buffer.ny(); ++j) {
        for (uint_t i = 0; i < buffer.nx(); ++i) {
            o << buffer(i, j);
        }
        o << std::endl;
    }
    return o;
}


void strBuffer::hline(uint_t i, uint_t j, uint_t w, char c)
{
    for (uint_t x = i; x < i + w; ++x) (*this)(x, j) = c;
}
void strBuffer::vline(uint_t i, uint_t j, uint_t w, char c)
{
    for (uint_t x = j; x < j + w; ++x) (*this)(i, x) = c;
}
void strBuffer::rect(uint_t i, uint_t j, uint_t w, uint_t h, char c)
{
    for (uint_t x = j; x < j + h; ++x) hline(i, x, w, c);
}

void strBuffer::txt(uint_t i, uint_t j, const string& s)
{
    for (size_t n = 0; n < s.length(); ++n) {
        (*this)(i + n, j) = s[n];
    }
}
void strBuffer::txt(uint_t i, uint_t j, int x)
{
    txt(i, j, to_string(x));
}
void strBuffer::txt(uint_t i, uint_t j, float x)
{
    txt(i, j, to_string(x));
}
