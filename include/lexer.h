#pragma once

#include <string>
#include <optional>
#include <vector>

namespace CCompiler
{
	enum class TokenType
	{
        // --- Special ---
        TOKEN_EOF,                              // End of File
        TOKEN_ERROR,                            // Invalid/Unknown Character

        // --- Punctuators ---
        PUNCTUATOR_OPEN_BRACKET,                // [
        PUNCTUATOR_CLOSE_BRACKET,               // ]
        PUNCTUATOR_OPEN_PARENTHESIS,            // (
        PUNCTUATOR_CLOSE_PARENTHESIS,           // )
        PUNCTUATOR_OPEN_BRACE,                  // {
        PUNCTUATOR_CLOSE_BRACE,                 // }
        PUNCTUATOR_COMMA,                       // ,
        PUNCTUATOR_COLON,                       // :
        PUNCTUATOR_SEMI_COLON,                  // ;
        PUNCTUATOR_PREPROCESSOR,                // #
        PUNCTUATOR_DOUBLE_HASH,                 // ##
        PUNCTUATOR_DOT,                         // .
        PUNCTUATOR_ARROW,                       // ->
        PUNCTUATOR_ELLIPSIS,                    // ...

        // --- Operators ---
        // (Arithmetic)
        OPERATOR_PLUS,                          // +
        OPERATOR_MINUS,                         // -
        OPERATOR_ASTERISK,                      // *
        OPERATOR_SLASH,                         // /
        OPERATOR_PERCENT,                       // %
        OPERATOR_INCREMENT,                     // ++
        OPERATOR_DECREMENT,                     // --
        // (Assignment)
        OPERATOR_ASSIGNMENT,                    // =
        OPERATOR_PLUS_ASSIGNMENT,               // +=
        OPERATOR_MINUS_ASSIGNMENT,              // -=
        OPERATOR_MULTIPLY_ASSIGNMENT,           // *=
        OPERATOR_DIVIDE_ASSIGNMENT,             // /=
        OPERATOR_MODULO_ASSIGNMENT,             // %=
        OPERATOR_LEFT_SHIFT_ASSIGNMENT,         // <<=
        OPERATOR_RIGHT_SHIFT_ASSIGNMENT,        // >>=
        OPERATOR_BITWISE_AND_ASSIGNMENT,        // &=
        OPERATOR_BITWISE_XOR_ASSIGNMENT,        // ^=
        OPERATOR_BITWISE_OR_ASSIGNMENT,         // |=
        // (Relational & Logical)
        OPERATOR_EQUAL_TO,                      // ==
        OPERATOR_NOT_EQUAL_TO,                  // !=
        OPERATOR_GREATER_THAN,                  // >
        OPERATOR_LESS_THAN,                     // <
        OPERATOR_GREATER_THAN_OR_EQUAL,         // >=
        OPERATOR_LESS_THAN_OR_EQUAL,            // <=
        OPERATOR_LOGICAL_AND,                   // &&
        OPERATOR_LOGICAL_OR,                    // ||
        OPERATOR_LOGICAL_NOT,                   // !
        // (Bitwise)
        OPERATOR_BITWISE_AND,                   // &
        OPERATOR_BITWISE_OR,                    // |
        OPERATOR_BITWISE_XOR,                   // ^
        OPERATOR_BITWISE_NOT,                   // ~
        OPERATOR_LEFT_SHIFT,                    // <<
        OPERATOR_RIGHT_SHIFT,                   // >>
        // (Misc)
        OPERATOR_QUESTION_MARK,                 // ? (Ternary)

        // --- Keywords ---
        // (Data Types)
        KEYWORD_CHAR,                           // Data Type
        KEYWORD_INT,                            // Data Type
        KEYWORD_FLOAT,                          // Data Type
        KEYWORD_DOUBLE,                         // Data Type
        KEYWORD_VOID,                           // Data Type
        KEYWORD_SHORT,                          // Data Type
        KEYWORD_LONG,                           // Data Type
        KEYWORD_SIGNED,                         // Data Type
        KEYWORD_UNSIGNED,                       // Data Type
        // (Control Flow)
        KEYWORD_IF,                             // Control Flow
        KEYWORD_ELSE,                           // Control Flow
        KEYWORD_SWITCH,                         // Control Flow
        KEYWORD_CASE,                           // Control Flow
        KEYWORD_DEFAULT,                        // Control Flow
        KEYWORD_FOR,                            // Control Flow
        KEYWORD_WHILE,                          // Control Flow
        KEYWORD_DO,                             // Control Flow
        KEYWORD_BREAK,                          // Control Flow
        KEYWORD_CONTINUE,                       // Control Flow
        KEYWORD_GOTO,                           // Control Flow
        KEYWORD_RETURN,                         // Control Flow
        // (Storage Class & Type Qualifiers)
        KEYWORD_AUTO,                           // Storage Class
        KEYWORD_REGISTER,                       // Storage Class
        KEYWORD_STATIC,                         // Storage Class
        KEYWORD_EXTERN,                         // Storage Class
        KEYWORD_CONST,                          // Type Qualifier
        KEYWORD_VOLATILE,                       // Type Qualifier
        // (User Defined Types)
        KEYWORD_STRUCT,                         // User Defined Type
        KEYWORD_UNION,                          // User Defined Type
        KEYWORD_ENUM,                           // User Defined Type
        KEYWORD_TYPEDEF,                        // Operator & Utility
        // (Utility)
        KEYWORD_SIZEOF,                         // Operator & Utility

        // --- Strings ---
        STRING_LITERAL,                         // e.g., "Hello, World!"

        // --- Identifiers ---
        IDENTIFIER,                             // e.g., main, x, my_variable

        // --- Constants ---
        CONSTANT_INTEGER,                       // e.g., 42, 0x2A, 052
        CONSTANT_FLOAT,                         // e.g., 3.14, 2.0e5
        CONSTANT_CHARACTER                      // e.g., 'A', '\n'

	};

    struct Token
    {
        TokenType type;
        std::optional<std::string> value;
    };
}
