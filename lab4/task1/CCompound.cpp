#include "CCompound.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

namespace
{
    std::string IndentAllButFirstLine(const std::string& text, const std::string& prefix)
    {
        std::ostringstream result;
        std::istringstream in(text);
        std::string line;
        bool isFirst = true;

        while (std::getline(in, line))
        {
            if (!isFirst)
            {
                result << "\n" << prefix;
            }
            result << line;
            isFirst = false;
        }

        return result.str();
    }

    std::string FormatChild(const size_t index, const std::string& childString)
    {
        std::ostringstream label;
        label << "  [" << index << "] ";
        const std::string subsequentIndent(label.str().size(), ' ');
        return label.str() + IndentAllButFirstLine(childString, subsequentIndent);
    }
}

bool CCompound::AddChildBody(const std::shared_ptr<CBody>& child)
{
    if (child.get() == this)
    {
        return false;
    }
    if (child->ContainsBody(this))
    {
        return false;
    }

    m_children.push_back(child);
    return true;
}

double CCompound::GetVolume() const
{
    return std::ranges::fold_left(m_children, 0.0, [](double sum, const auto& child)
    {
        return sum + child->GetVolume();
    });
}

double CCompound::GetTotalMass() const
{
    return std::ranges::fold_left(m_children, 0.0, [](double sum, const auto& child)
    {
        return sum + child->GetMass();
    });
}

double CCompound::GetDensity() const
{
    const double totalVolume = GetVolume();
    if (totalVolume == 0)
    {
        return 0.0;
    }

    const double totalMass = GetTotalMass();

    return totalMass / totalVolume;
}

bool CCompound::ContainsBody(const CBody* body) const
{
    return std::ranges::any_of(m_children, [body](const auto& child)
    {
        return child.get() == body || child->ContainsBody(body);
    });
}

std::string CCompound::ToString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Compound:\n";
    oss << "  Volume: " << GetVolume() << " m3\n";
    oss << "  Average density: " << GetDensity() << " kg/m3\n";
    oss << "  Mass: " << GetMass() << " kg\n";
    oss << "  Children (" << m_children.size() << "):";

    for (size_t i = 0; i < m_children.size(); ++i)
    {
        oss << "\n" << FormatChild(i + 1, m_children[i]->ToString());
    }

    return oss.str();
}