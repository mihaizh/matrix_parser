#include "matrix_parser.h"
#include <cctype> // isdigit
#include <cstdlib> // strtof

#if defined(ENABLE_LOGGING) || defined(ENABLE_DEBUGGING)
#include <iostream>
#endif // defined(ENABLE_LOGGING) || defined(ENABLE_DEBUGGING)

static const char* MatrixParserErrorMessages[] = {
        "No error.",
        "Unexpected character occurred.",
        "Too many dimensions. Maximum allowed is 2.",
        "Too many closed parentheses.",
        "Unbalanced parentheses.",
        "Number of columns between rows are different.",
        "Unexpected comma."
};

MatrixParserError MatrixParser::getError()
{
    return m_error;
}

const char* MatrixParser::getErrorMessage()
{
    return MatrixParserErrorMessages[m_error];
}

const MatrixParser::matrix_type& MatrixParser::getMatrix()
{
    return m_matrix;
}

void MatrixParser::consume()
{
    ++m_current_ptr;
}

char MatrixParser::current()
{
    return *m_current_ptr;
}

bool MatrixParser::eof()
{
    return current() == 0;
}

void MatrixParser::add_row()
{
    m_matrix.resize(m_matrix.size() + 1);
    if (m_last_cols > 0)
    {
        m_matrix[m_rows].reserve(m_last_cols);
    }
}

void MatrixParser::increase_depth()
{
    if (m_depth == 0)
    {
        add_row();
    }

    ++m_depth;
}

void MatrixParser::decrease_depth()
{
    --m_depth;
    if (m_depth == 1)
    {
        if ((m_last_cols != -1) && (m_last_cols != m_col))
        {
            m_error = MPARSER_UNMATCHED_COLUMNS;
#ifdef ENABLE_LOGGING
            std::cerr << getErrorMessage() << std::endl;
            std::cerr << "At row " << m_rows << " was " << m_last_cols << ", but next row was " << m_col << std::endl;
#endif // ENABLE_LOGGING
        }

        m_last_cols = m_col;
    }
}

void MatrixParser::parse_number()
{
    const char* start_ptr = m_current_ptr;
    while(isdigit(current()))
    {
        consume();
    }

    if (current() == '.') // floating point number
    {
        consume();
        while (isdigit(current()))
        {
            consume();
        }
    }

    const float value = std::strtof(start_ptr, nullptr);
    m_matrix[m_rows].resize(m_col + 1);
    m_matrix[m_rows][m_col] = value;
    ++m_col;
#ifdef ENABLE_DEBUGGING
    std::cout << "(" << m_rows << ", " << m_col << "): " << value << std::endl;
#endif // ENABLE_DEBUGGING
}

void MatrixParser::comma()
{
    if (m_depth == 0)
    {
        m_error = MPARSER_UNEXPECTED_COMMA;
#ifdef ENABLE_LOGGING
        std::cerr << getErrorMessage() << std::endl;
#endif // ENABLE_LOGGING
    }

    if (m_depth == 1)
    {
        ++m_rows;
        m_col = 0;

        add_row();
    }
}

bool MatrixParser::parse(const char* text)
{
    m_rows = 0;
    m_col = 0;
    m_depth = 0;

    m_last_cols = -1;

    m_current_ptr = text;
    m_error = MPARSER_OK;

    m_matrix.clear();

    while (!eof() && (m_error == MPARSER_OK))
    {
        if (isdigit(current()))
        {
#ifdef ENABLE_DEBUGGING
            std::cout << "Parsing number" << std::endl;
#endif // ENABLE_DEBUGGING
            parse_number();
        }

        switch (current())
        {
            case '\n':
            case '\r':
            case '\t':
            case ' ':
#ifdef ENABLE_DEBUGGING
                std::cout << "Ignoring character '" << current() << "'" << std::endl;
#endif // ENABLE_DEBUGGING
                break;
            case '[':
#ifdef ENABLE_DEBUGGING
                std::cout << "Increasing depth" << std::endl;
#endif // ENABLE_DEBUGGING
                increase_depth();
                break;
            case ']':
#ifdef ENABLE_DEBUGGING
                std::cout << "Decreasing depth" << std::endl;
#endif // ENABLE_DEBUGGING
                decrease_depth();
                break;
            case ',':
#ifdef ENABLE_DEBUGGING
                std::cout << "Parsing comma" << std::endl;
#endif // ENABLE_DEBUGGING
                comma();
                break;
            default:
                m_error = MPARSER_UNEXPECTED_CHAR;
#ifdef ENABLE_LOGGING
                std::cerr << getErrorMessage() << " '" << current() << '\'' << std::endl;
#endif // ENABLE_LOGGING
                break;
        }

        if (m_depth < 0)
        {
            m_error = MPARSER_TOO_MANY_CLOSED_PARAN;
#ifdef ENABLE_LOGGING
            std::cerr << getErrorMessage() << std::endl;
#endif // ENABLE_LOGGING
        }

        if (m_depth > 2)
        {
            m_error = MPARSER_TOO_MANY_DIMS;
#ifdef ENABLE_LOGGING
            std::cerr << getErrorMessage() << std::endl;
#endif // ENABLE_LOGGING
        }

        consume();
    }

    if ((m_error == MPARSER_OK) && (m_depth != 0))
    {
        m_error = MPARSER_UNBALANCED_PARANTHESES;
#ifdef ENABLE_LOGGING
        std::cerr << getErrorMessage() << std::endl;
#endif // ENABLE_LOGGING
    }

    return m_error == MPARSER_OK;
}
