#ifndef OOP_CCOMPOUND_H
#define OOP_CCOMPOUND_H

#include <memory>
#include <vector>

#include "CBody.h"

class CCompound : public CBody
{
public:
    bool AddChildBody(const std::shared_ptr<CBody>& child);
    double GetVolume() const override;
    double GetTotalMass() const;
    double GetDensity() const override;
    bool ContainsBody(const CBody* body) const override;
    std::string ToString() const override;

private:
    std::string ToStringWithIndent(int indentLevel) const;

    std::vector<std::shared_ptr<CBody>> m_children;
};

#endif //OOP_CCOMPOUND_H
