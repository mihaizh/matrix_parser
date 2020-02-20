#include "matrix_parser.h"
#include <iostream>

int main(int argc, char* argv[])
{
    const char* texts[] = {
            "[[1, 2, 3.14], [4.2, 5, 6.9], [7, 8, 9]]",
            "[[1 2 3.14], [4.2 5 6.9], [7 8 9]]",
            "[3.14, 4.2, 6.9]"
    };

    MatrixParser p;
    for (const char* text : texts)
    {
        if (p.parse(text))
        {
            for (size_t i = 0; i < p.getMatrix().size(); ++i)
            {
                for (size_t j = 0; j < p.getMatrix()[i].size(); ++j)
                {
                    std::cout << p.getMatrix()[i][j] << ' ';
                }
                std::cout << std::endl;
            }
        }
        else
        {
            std::cerr << "Input parsing failed." << std::endl;
            std::cerr << "Error code #" << p.getError() << ": " << p.getErrorMessage() << std::endl;
        }
    }

    return 0;
}