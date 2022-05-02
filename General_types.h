#ifndef __GENERAL_TYPE__
#define __GENERAL_TYPE__

#include <string>
#include <ostream>
#include <functional>

typedef unsigned int uint_t;
enum Side { INVALID, VOID, WALL, DOOR };

static const int CELL_SX = 6;
static const int CELL_SY = 4;

Side strSide(char s);

struct Position {
    Position(uint_t i, uint_t j) :x(i), y(j) {};
    uint_t x;
    uint_t y;
    Position& operator+(const Position& p) { x += p.x; y += p.y; return *this; }
    bool operator==(const Position& p) { return x == p.x && y == p.y; }
    bool operator!=(const Position& p) { return x != p.x || y != p.y; }
};

template<typename T>
class Bidim
{
public:

private:
    uint_t _nx;
    uint_t _ny;
    T* _data = nullptr;
    typedef void rw_apply_fn_t(uint_t, uint_t, T&); // R/W function to apply to any element
    typedef void ro_apply_fn_t(uint_t, uint_t, const T&); // RO function to apply to any element
    typedef void rw_apply_no_pos_fn_t(T&); // R/W function to apply to any element
    typedef void ro_apply_no_pos_fn_t(const T&); // RO function to apply to any element
public:
    Bidim(uint_t nx = 0, uint_t ny = 0);
    virtual ~Bidim();
    void redim(uint_t nx = 0, uint_t ny = 0);
    uint_t ij_n(uint_t i, uint_t j) const { return j * _nx + i; }
    Position n_ij(uint_t n) const { return  Position(n % _nx, uint_t(n / _nx)); }
    const T& operator()(uint_t i, uint_t j) const { return _data[ij_n(i, j)]; }
    T& operator()(uint_t i, uint_t j) { return _data[ij_n(i, j)]; }
    const T& operator[](uint_t n) const { return _data[n]; }
    T& operator[](uint_t n) { return _data[n]; }
    uint_t nx() const { return _nx; }
    uint_t ny() const { return _ny; }
    uint_t n() const { return _nx * _ny; }
    void apply_rw(const std::function<rw_apply_fn_t>& F) { for (uint_t n = 0; n < _nx * _ny; ++n) { Position P = n_ij(n); F(P.x, P.y, _data[n]); } }
    void apply_ro(const std::function<ro_apply_fn_t>& F) { for (uint_t n = 0; n < _nx * _ny; ++n) { Position P = n_ij(n); F(P.x, P.y, _data[n]); } }
    void apply_rw_nopos(const std::function<rw_apply_no_pos_fn_t>& F) { for (uint_t n = 0; n < _nx * _ny; ++n) { F(_data[n]); } }
    void apply_ro_nopos(const std::function<ro_apply_no_pos_fn_t>& F) { for (uint_t n = 0; n < _nx * _ny; ++n) { F(_data[n]); } }
    void fill(const T& v) { apply_rw_nopos([&v](T& x)->void {x = v; }); }
};

//typedef Bidim<char> strBuffer;

class strBuffer : public Bidim<char>
{
public:
    strBuffer(uint_t nx = 0, uint_t ny = 0) : Bidim<char>(nx, ny) {};
    void hline(uint_t i, uint_t j, uint_t w, char c = '-');
    void vline(uint_t i, uint_t j, uint_t w, char c = '|');
    void rect(uint_t i, uint_t j, uint_t w, uint_t h, char c = '#');
    void txt(uint_t i, uint_t j, const std::string& s);
    void txt(uint_t i, uint_t j, int x);
    void txt(uint_t i, uint_t j, float x);
};


std::ostream& operator<<(std::ostream& o, const strBuffer& buffer);

template <typename T>
Bidim<T>::Bidim(uint_t nx, uint_t ny) :_nx(nx), _ny(ny)
{
    if (nx > 0 && ny > 0) {
        _data = new T[nx * ny];
    }
    else {
        _data = nullptr;
    }
}

template <typename T>
void Bidim<T>::redim(uint_t nx , uint_t ny )
{
    if (_data != nullptr) {
        delete[] _data;
    }
    if (nx > 0 && ny > 0) {
        if (nx > 0 && ny > 0) {
            _data = new T[nx * ny];
        }
        else {
            _data = nullptr;
        }
    }
    _nx = nx;
    _ny = ny;
}

template <typename T>
Bidim<T>::~Bidim()
{
    if (_data != nullptr) {
        delete[] _data;
    }
}

#endif // __GENERAL_TYPE__
