#ifndef OOP_ISOLIDSHAPE_H
#define OOP_ISOLIDSHAPE_H

#include <cstdint>

#include "IShape.h"

class ISolidShape : public IShape
{
public:
    virtual uint32_t GetFillColor() const = 0;
};

#endif //OOP_ISOLIDSHAPE_H
