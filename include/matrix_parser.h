#ifndef MATRIX_PARSER_H
#define MATRIX_PARSER_H

#include <vector>

enum MatrixParserError
{
    MPARSER_OK,
    MPARSER_UNEXPECTED_CHAR,
    MPARSER_TOO_MANY_DIMS,
    MPARSER_TOO_MANY_CLOSED_PARAN,
    MPARSER_UNBALANCED_PARANTHESES,
    MPARSER_UNMATCHED_COLUMNS,
    MPARSER_UNEXPECTED_COMMA
}; // MatrixParserError

class MatrixParser
{
public:
    using matrix_type = std::vector<std::vector<float>>;

    MatrixParser() = default;
    MatrixParser(const MatrixParser&) = delete;
    MatrixParser(MatrixParser&&) = delete;
    MatrixParser& operator=(const MatrixParser&) = delete;
    MatrixParser& operator=(MatrixParser&&) = delete;
    ~MatrixParser() = default;

    bool parse(const char* text);
    const matrix_type& getMatrix();

    MatrixParserError getError();
    const char* getErrorMessage();

private:
    void consume();
    char current();

    bool eof();

    void increase_depth();
    void decrease_depth();

    void parse_number();
    void comma();

    void add_row();

    int m_rows = 0;
    int m_col = 0;
    int m_depth = 0;

    int m_last_cols = -1;

    const char* m_current_ptr = nullptr;

    MatrixParserError m_error = MPARSER_OK;
    matrix_type m_matrix;
}; // MatrixParser

#endif // MATRIX_PARSER_H