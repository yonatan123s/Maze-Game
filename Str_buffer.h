#ifndef STRBUFFER_H
#define STRBUFFER_H

#include "General_types.h"



class StrBuffer : public Bidim<char>
{
public:
    StrBuffer(uint_t nx, uint_t ny);
    virtual ~StrBuffer();

protected:

private:
};

#endif // STRBUFFER_H
