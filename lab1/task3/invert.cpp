#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <array>

constexpr double EPSILON = 1e-12;

using Matrix3x3 = std::array<std::array<double, 3>, 3>;

std::string Trim(const std::string& text)
{
    size_t start = 0;
    while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start])))
    {
        ++start;
    }

    if (start == text.size())
    {
        return "";
    }

    size_t end = text.size() - 1;
    while (end > start && std::isspace(static_cast<unsigned char>(text[end])))
    {
        --end;
    }

    return text.substr(start, end - start + 1);
}

bool SplitByTab(const std::string& line, std::vector<std::string>& tokens)
{
    tokens.clear();
    std::string current;
    for (const char ch : line)
    {
        if (ch == '\t')
        {
            tokens.push_back(current);
            current.clear();
        }
        else
        {
            current.push_back(ch);
        }
    }

    tokens.push_back(current);
    return true;
}

bool ReadMatrixFromStream(std::istream& input, Matrix3x3& matrix, std::string& errorMessage)
{
    for (size_t row = 0; row < 3; ++row)
    {
        std::string line;

        if (!std::getline(input, line))
        {
            errorMessage = "Invalid matrix format";
            return false;
        }

        std::vector<std::string> tokens;
        SplitByTab(line, tokens);

        if (tokens.size() != 3)
        {
            errorMessage = "Invalid matrix format";
            return false;
        }

        for (size_t col = 0; col < 3; ++col)
        {
            std::string tokenTrimmed = Trim(tokens[col]);
            if (tokenTrimmed.empty())
            {
                errorMessage = "Invalid matrix format";
                return false;
            }

            try
            {
                size_t idx = 0;
                double value = std::stod(tokenTrimmed, &idx);
                if (idx != tokenTrimmed.size())
                {
                    errorMessage = "Invalid matrix";
                    return false;
                }

                matrix.at(row).at(col) = value;
            }
            catch (std::exception&)
            {
                errorMessage = "Invalid matrix";
                return false;
            }
        }
    }

    return true;
}

double ComputeDeterminant(const Matrix3x3& m)
{
    return
        m.at(0).at(0) * (m.at(1).at(1) * m.at(2).at(2) - m.at(1).at(2) * m.at(2).at(1)) -
        m.at(0).at(1) * (m.at(1).at(0) * m.at(2).at(2) - m.at(1).at(2) * m.at(2).at(0)) +
        m.at(0).at(2) * (m.at(1).at(0) * m.at(2).at(1) - m.at(1).at(1) * m.at(2).at(0));
}

double MinorDeterminant(const Matrix3x3& m, const int row, const int col)
{
    std::array<double, 4> vals{};
    int idx = 0;
    for (int i = 0; i < 3; ++i)
    {
        if (i == row)
        {
            continue;
        }

        for (int j = 0; j < 3; ++j)
        {
            if (j == col)
            {
                continue;
            }

            vals.at(idx++) = m.at(i).at(j);
        }
    }

    return vals.at(0) * vals.at(3) - vals.at(1) * vals.at(2);
}

bool InvertMatrix(const Matrix3x3& input, Matrix3x3& output)
{
    const double det = ComputeDeterminant(input);
    if (std::fabs(det) < EPSILON)
    {
        return false;
    }

    Matrix3x3 cofactor;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            const double minor = MinorDeterminant(input, i, j);
            const double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            cofactor.at(i).at(j) = sign * minor;
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            output.at(i).at(j) = cofactor.at(j).at(i) / det;
        }
    }

    return true;
}

void PrintMatrix(const Matrix3x3& m)
{
    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(3);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cout << m.at(i).at(j);
            if (j != 2)
            {
                std::cout << '\t';
            }
        }

        if (i != 2)
        {
            std::cout << '\n';
        }
    }
}

void PrintUsage()
{
    std::cout << "Usage:\n";
    std::cout << "  ./invert\n";
    std::cout << "  ./invert <file>\n";
    std::cout << "  ./invert -h\n";
}

int main(const int argc, char *argv[])
{
    if (argc == 2)
    {
        std::string arg = argv[1];
        if (arg == "-h")
        {
            PrintUsage();
            return 0;
        }

        std::ifstream infile(arg);
        if (!infile.is_open())
        {
            std::cout << "Invalid matrix format";
            return 0;
        }

        Matrix3x3 matrix;
        std::string errorMessage;
        bool ok = ReadMatrixFromStream(infile, matrix, errorMessage);
        infile.close();

        if (!ok)
        {
            std::cout << errorMessage;
            return 0;
        }

        Matrix3x3 inv;
        if (!InvertMatrix(matrix, inv))
        {
            std::cout << "Non-invertible";
            return 0;
        }

        PrintMatrix(inv);
        return 0;
    }

    if (argc == 1)
    {
        Matrix3x3 matrix;
        std::string errorMessage;
        if (bool ok = ReadMatrixFromStream(std::cin, matrix, errorMessage); !ok)
        {
            std::cout << errorMessage;
            return 0;
        }

        Matrix3x3 inv;
        if (!InvertMatrix(matrix, inv))
        {
            std::cout << "Non-invertible";
            return 0;
        }

        PrintMatrix(inv);
        return 0;
    }

    PrintUsage();
    return 0;
}
