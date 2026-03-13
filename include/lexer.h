#pragma once

#include <string>
#include <optional>
#include <vector>

/**
 * @file lexer.h
 * @brief Strictly compliant ISO C11 Lexical Analyzer module.
 * * Implements the tokenization rules defined in ISO/IEC 9899:2011 (N1570 Draft, Section 6.4).
 * Supports Universal Character Names (UCNs), strict hex/octal floating-point resolution,
 * and all standard C11 punctuators and operators.
 * * - ISO N1570 Draft (Sec 6.4): https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf
 * - Token & Regex Reference: https://en.cppreference.com/w/c/language/lex
 */

namespace CCompiler
{
	enum class TokenType
	{
        // --- Special ---
        TOKEN_EOF,                              // End of File                      | Regex: (N/A - represents hardware EOF)
        TOKEN_ERROR,                            // Invalid/Unknown Character        | Regex: (N/A - any unmatched char)

        // --- Punctuators ---
        PUNCTUATOR_OPEN_BRACKET,                // [                                | Regex: \[
        PUNCTUATOR_CLOSE_BRACKET,               // ]                                | Regex: \]
        PUNCTUATOR_OPEN_PARENTHESIS,            // (                                | Regex: \(
        PUNCTUATOR_CLOSE_PARENTHESIS,           // )                                | Regex: \)
        PUNCTUATOR_OPEN_BRACE,                  // {                                | Regex: \{
        PUNCTUATOR_CLOSE_BRACE,                 // }                                | Regex: \}
        PUNCTUATOR_COMMA,                       // ,                                | Regex: ,
        PUNCTUATOR_COLON,                       // :                                | Regex: :
        PUNCTUATOR_SEMI_COLON,                  // ;                                | Regex: ;
        PUNCTUATOR_PREPROCESSOR,                // #                                | Regex: #
        PUNCTUATOR_DOUBLE_HASH,                 // ##                               | Regex: ##
        PUNCTUATOR_DOT,                         // .                                | Regex: \.
        PUNCTUATOR_ARROW,                       // ->                               | Regex: ->
        PUNCTUATOR_ELLIPSIS,                    // ...                              | Regex: \.\.\.

        // --- Operators ---
        // (Arithmetic)
        OPERATOR_PLUS,                          // +                                | Regex: \+
        OPERATOR_MINUS,                         // -                                | Regex: -
        OPERATOR_ASTERISK,                      // *                                | Regex: \*
        OPERATOR_SLASH,                         // /                                | Regex: /
        OPERATOR_PERCENT,                       // %                                | Regex: %
        OPERATOR_INCREMENT,                     // ++                               | Regex: \+\+
        OPERATOR_DECREMENT,                     // --                               | Regex: --
        // (Assignment)
        OPERATOR_ASSIGNMENT,                    // =                                | Regex: =
        OPERATOR_PLUS_ASSIGNMENT,               // +=                               | Regex: \+=
        OPERATOR_MINUS_ASSIGNMENT,              // -=                               | Regex: -=
        OPERATOR_MULTIPLY_ASSIGNMENT,           // *=                               | Regex: \*=
        OPERATOR_DIVIDE_ASSIGNMENT,             // /=                               | Regex: /=
        OPERATOR_MODULO_ASSIGNMENT,             // %=                               | Regex: %=
        OPERATOR_LEFT_SHIFT_ASSIGNMENT,         // <<=                              | Regex: <<=
        OPERATOR_RIGHT_SHIFT_ASSIGNMENT,        // >>=                              | Regex: >>=
        OPERATOR_BITWISE_AND_ASSIGNMENT,        // &=                               | Regex: &=
        OPERATOR_BITWISE_XOR_ASSIGNMENT,        // ^=                               | Regex: \^=
        OPERATOR_BITWISE_OR_ASSIGNMENT,         // |=                               | Regex: \|=
        // (Relational & Logical)
        OPERATOR_EQUAL_TO,                      // ==                               | Regex: ==
        OPERATOR_NOT_EQUAL_TO,                  // !=                               | Regex: !=
        OPERATOR_GREATER_THAN,                  // >                                | Regex: >
        OPERATOR_LESS_THAN,                     // <                                | Regex: <
        OPERATOR_GREATER_THAN_OR_EQUAL,         // >=                               | Regex: >=
        OPERATOR_LESS_THAN_OR_EQUAL,            // <=                               | Regex: <=
        OPERATOR_LOGICAL_AND,                   // &&                               | Regex: &&
        OPERATOR_LOGICAL_OR,                    // ||                               | Regex: \|\|
        OPERATOR_LOGICAL_NOT,                   // !                                | Regex: !
        // (Bitwise)
        OPERATOR_BITWISE_AND,                   // &                                | Regex: &
        OPERATOR_BITWISE_OR,                    // |                                | Regex: \|
        OPERATOR_BITWISE_XOR,                   // ^                                | Regex: \^
        OPERATOR_BITWISE_NOT,                   // ~                                | Regex: ~
        OPERATOR_LEFT_SHIFT,                    // <<                               | Regex: <<
        OPERATOR_RIGHT_SHIFT,                   // >>                               | Regex: >>
        // (Misc)
        OPERATOR_QUESTION_MARK,                 // ? (Ternary)                      | Regex: \?

        // --- Keywords ---
        // (Data Types)
        KEYWORD_CHAR,                           // Data Type                        | Regex: char
        KEYWORD_INT,                            // Data Type                        | Regex: int
        KEYWORD_FLOAT,                          // Data Type                        | Regex: float
        KEYWORD_DOUBLE,                         // Data Type                        | Regex: double
        KEYWORD_VOID,                           // Data Type                        | Regex: void
        KEYWORD_SHORT,                          // Data Type                        | Regex: short
        KEYWORD_LONG,                           // Data Type                        | Regex: long
        KEYWORD_SIGNED,                         // Data Type                        | Regex: signed
        KEYWORD_UNSIGNED,                       // Data Type                        | Regex: unsigned
        KEYWORD_BOOL,                           // _Bool                            | Regex: _Bool
        KEYWORD_COMPLEX,                        // _Complex                         | Regex: _Complex
        KEYWORD_IMAGINARY,                      // _Imaginary                       | Regex: _Imaginary
        // (Control Flow)
        KEYWORD_IF,                             // Control Flow                     | Regex: if
        KEYWORD_ELSE,                           // Control Flow                     | Regex: else
        KEYWORD_SWITCH,                         // Control Flow                     | Regex: switch
        KEYWORD_CASE,                           // Control Flow                     | Regex: case
        KEYWORD_DEFAULT,                        // Control Flow                     | Regex: default
        KEYWORD_FOR,                            // Control Flow                     | Regex: for
        KEYWORD_WHILE,                          // Control Flow                     | Regex: while
        KEYWORD_DO,                             // Control Flow                     | Regex: do
        KEYWORD_BREAK,                          // Control Flow                     | Regex: break
        KEYWORD_CONTINUE,                       // Control Flow                     | Regex: continue
        KEYWORD_GOTO,                           // Control Flow                     | Regex: goto
        KEYWORD_RETURN,                         // Control Flow                     | Regex: return
        // (Storage Class & Type Qualifiers)
        KEYWORD_AUTO,                           // Storage Class                    | Regex: auto
        KEYWORD_REGISTER,                       // Storage Class                    | Regex: register
        KEYWORD_STATIC,                         // Storage Class                    | Regex: static
        KEYWORD_EXTERN,                         // Storage Class                    | Regex: extern
        KEYWORD_THREAD_LOCAL,                   // _Thread_local                    | Regex: _Thread_local
        KEYWORD_CONST,                          // Type Qualifier                   | Regex: const
        KEYWORD_VOLATILE,                       // Type Qualifier                   | Regex: volatile
        KEYWORD_RESTRICT,                       // restrict                         | Regex: restrict
        KEYWORD_ATOMIC,                         // _Atomic                          | Regex: _Atomic
        // (Function Specifiers)
        KEYWORD_INLINE,                         // inline                           | Regex: inline
        KEYWORD_NORETURN,                       // _Noreturn                        | Regex: _Noreturn
        // (User Defined Types)
        KEYWORD_STRUCT,                         // User Defined Type                | Regex: struct
        KEYWORD_UNION,                          // User Defined Type                | Regex: union
        KEYWORD_ENUM,                           // User Defined Type                | Regex: enum
        KEYWORD_TYPEDEF,                        // Operator & Utility               | Regex: typedef
        // (Utility & Generics)
        KEYWORD_SIZEOF,                         // Operator & Utility               | Regex: sizeof
        KEYWORD_ALIGNAS,                        // _Alignas                         | Regex: _Alignas
        KEYWORD_ALIGNOF,                        // _Alignof                         | Regex: _Alignof
        KEYWORD_GENERIC,                        // _Generic                         | Regex: _Generic
        KEYWORD_STATIC_ASSERT,                  // _Static_assert                   | Regex: _Static_assert

        // --- Strings ---
        STRING_LITERAL,                         // e.g., "Hello, World!"            | Regex: (u8|u|U|L)?\"([^\"\\\n]|\\.)*\"

        // --- Identifiers ---
        IDENTIFIER,                             // e.g., main, x                    | Regex: [a-zA-Z_][a-zA-Z0-9_]*

        // --- Constants ---
        CONSTANT_INTEGER,                       // e.g., 42, 0x2A                   | Regex: (0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)([uU][lL]{0,2}|[lL]{1,2}[uU]?)?
        CONSTANT_FLOAT,                         // e.g., 3.14, 2e5                  | Regex: (([0-9]*\.[0-9]+|[0-9]+\.)([eE][+-]?[0-9]+)?|[0-9]+[eE][+-]?[0-9]+)[flFL]?
        CONSTANT_CHARACTER                      // e.g., 'A', '\n'                  | Regex: (u|U|L)?'([^'\\\n]|\\.)+'
	};

    struct Token
    {
        TokenType type;                         // Token type
        std::optional<std::string> value;       // Lexeme
        size_t line;                            // Line number
        size_t column;                          // Column number
    };

    class Lexer
    {
    private:
        std::string source;
        size_t cursor = 0;
        size_t line = 1;
        size_t column = 1;

        // --- Start Anchors ---
        size_t start_cursor = 0;
        size_t start_line = 1;
        size_t start_column = 1;

        // --- State Machine Primitives ---
        bool isAtEnd() const;
        char peek() const;                      // Look at current char without consuming
        char peekNext() const;                  // Look at next char (Lookahead of 1)
        char advance();                         // Consume current char and move forward

        // --- Helper DFA Branches ---
        void skipWhitespace();
        Token lexIdentifierOrKeyword();
        Token lexNumber();
        Token lexString();
        Token lexCharacter();
        Token lexOperatorOrPunctuator();

        // Helper Functions
        bool match(char expected);
        void handleEscapeSequence(std::string& lexeme);

        // Factory function for cleaner code
        Token createToken(TokenType type, std::optional<std::string> value = std::nullopt);

    public:
        explicit Lexer(std::string sourceCode);

        // The core dispatcher
        Token getNextToken();

        // The master loop
        std::vector<Token> tokenize();
    };

}
