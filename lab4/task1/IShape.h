#ifndef OOP_ISHAPE_H
#define OOP_ISHAPE_H

class IShape
{
public:
    virtual ~IShape() = default;

    virtual double GetArea() const = 0;
    virtual double GetPerimeter() const = 0;
    virtual std::string ToString() const = 0;
    virtual uint32_t GetOutlineColor() const = 0;
};

#endif //OOP_ISHAPE_H
