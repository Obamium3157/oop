#ifndef OOP_ICANVASDRAWABLE_H
#define OOP_ICANVASDRAWABLE_H

#include "ICanvas.h"

class ICanvasDrawable
{
public:
    virtual ~ICanvasDrawable() = default;
    virtual void Draw(ICanvas& canvas) const = 0;
};

#endif //OOP_ICANVASDRAWABLE_H
