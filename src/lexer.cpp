#include "lexer.h"
#include <unordered_map>
#include <string_view>
#include <utility>

namespace CCompiler
{
	namespace
	{
		const std::unordered_map<std::string_view, TokenType> keywords = 
		{
			{"auto",     TokenType::KEYWORD_AUTO},
			{"break",    TokenType::KEYWORD_BREAK},
			{"case",     TokenType::KEYWORD_CASE},
			{"char",     TokenType::KEYWORD_CHAR},
			{"const",    TokenType::KEYWORD_CONST},
			{"continue", TokenType::KEYWORD_CONTINUE},
			{"default",  TokenType::KEYWORD_DEFAULT},
			{"do",       TokenType::KEYWORD_DO},
			{"double",   TokenType::KEYWORD_DOUBLE},
			{"else",     TokenType::KEYWORD_ELSE},
			{"enum",     TokenType::KEYWORD_ENUM},
			{"extern",   TokenType::KEYWORD_EXTERN},
			{"float",    TokenType::KEYWORD_FLOAT},
			{"for",      TokenType::KEYWORD_FOR},
			{"goto",     TokenType::KEYWORD_GOTO},
			{"if",       TokenType::KEYWORD_IF},
			{"inline",   TokenType::KEYWORD_INLINE},
			{"int",      TokenType::KEYWORD_INT},
			{"long",     TokenType::KEYWORD_LONG},
			{"register", TokenType::KEYWORD_REGISTER},
			{"restrict", TokenType::KEYWORD_RESTRICT},
			{"return",   TokenType::KEYWORD_RETURN},
			{"short",    TokenType::KEYWORD_SHORT},
			{"signed",   TokenType::KEYWORD_SIGNED},
			{"sizeof",   TokenType::KEYWORD_SIZEOF},
			{"static",   TokenType::KEYWORD_STATIC},
			{"struct",   TokenType::KEYWORD_STRUCT},
			{"switch",   TokenType::KEYWORD_SWITCH},
			{"typedef",  TokenType::KEYWORD_TYPEDEF},
			{"union",    TokenType::KEYWORD_UNION},
			{"unsigned", TokenType::KEYWORD_UNSIGNED},
			{"void",     TokenType::KEYWORD_VOID},
			{"volatile", TokenType::KEYWORD_VOLATILE},
			{"while",    TokenType::KEYWORD_WHILE},
			{"_Alignas", TokenType::KEYWORD_ALIGNAS},
			{"_Alignof", TokenType::KEYWORD_ALIGNOF},
			{"_Atomic",  TokenType::KEYWORD_ATOMIC},
			{"_Bool",    TokenType::KEYWORD_BOOL},
			{"_Complex", TokenType::KEYWORD_COMPLEX},
			{"_Generic", TokenType::KEYWORD_GENERIC},
			{"_Imaginary",TokenType::KEYWORD_IMAGINARY},
			{"_Noreturn", TokenType::KEYWORD_NORETURN},
			{"_Static_assert", TokenType::KEYWORD_STATIC_ASSERT},
			{"_Thread_local", TokenType::KEYWORD_THREAD_LOCAL}
		};
	}

	bool Lexer::isAtEnd() const
	{
		return cursor >= source.length();
	}

	char Lexer::peek() const
	{
		if (isAtEnd()) return '\0';
		return source[cursor];
	}

	char Lexer::peekNext() const
	{
		if (cursor + 1 >= source.length()) return '\0';
		return source[cursor + 1];
	}

	char Lexer::advance()
	{
		if (isAtEnd()) return '\0';
		
		char c = source[cursor];
		cursor++;

		if (c == '\n')
		{
			column = 1;
			line++;
		}
		else 
		{
			column++;
		}

		return c;
	}
	
	void Lexer::skipWhitespace()
	{
		while (true) 
		{
			char c = peek();
			bool whitespace = c == ' ' || c == '\r' || c == '\t' || c == '\n';
			
			if (whitespace)
			{
				advance();
				continue;
			}

			if (c == '/')
			{
				char next_c = peekNext();
				
				if (next_c == '/')
				{
					while (peek() != '\n' && !isAtEnd())
					{
						advance();
					}
					continue;
				}

				if (next_c == '*')
				{
					advance();
					advance(); 

					while (!(peek() == '*' && peekNext() == '/') && !isAtEnd())
					{
						advance();
					}

					if (!isAtEnd())
					{
						advance(); 
						advance();
					}
					continue;
				}
			}
			break;
		}
	}
	
	Token Lexer::lexIdentifierOrKeyword()
	{
		std::string lexeme = "";

		// Regex: ([a-zA-Z_]|\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})([a-zA-Z0-9_]|\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})*
		while (true)
		{
			if (std::isalpha(peek()) || std::isdigit(peek()) || peek() == '_')
			{
				lexeme += advance();
			}
			else if (peek() == '\\')
			{
				if (peekNext() == 'u' || peekNext() == 'U')
				{
					handleEscapeSequence(lexeme);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		// Keyword
		auto it = keywords.find(lexeme);
		if (it != keywords.end())
		{
			return createToken(it->second);
		}

		// Identifier
		return createToken(TokenType::IDENTIFIER, std::string(lexeme));
	}
	
	Token Lexer::lexNumber()
	{
		std::string lexeme = "";
		bool is_float = false;
		bool is_hex = false;
		
		// Hex		| Regex: 0[xX][0-9a-fA-F]+
		if (peek() == '0' && (peekNext() == 'x' || peekNext() == 'X'))
		{
			is_hex = true;
			lexeme += advance();										// 0
			lexeme += advance();										// x or X
			
			while (std::isxdigit(peek()))
			{
				lexeme += advance();
			}
		}

		// Octal	| Regex: 0[0-7]*
		else if (peek() == '0')
		{
			lexeme += advance();										// 0

			while (peek() >= '0' && peek() <= '7')
			{
				lexeme += advance();
			}
		}

		// Decimal	| Regex: [1-9][0-9]*
		else
		{
			while (std::isdigit(peek()))
			{
				lexeme += advance();
			}
		}

		// Float	| Regex: \.[0-9]*
		if (peek() == '.')
		{
			is_float = true;
			lexeme += advance();										// .
			
			while (is_hex ? std::isxdigit(peek()) : std::isdigit(peek()))
			{
				lexeme += advance();
			}
		}

		// Exponent	| Regex: [eEpP][+-]?[0-9]+
		if (peek() == 'e' || peek() == 'E' || peek() == 'p' || peek() == 'P')
		{
			is_float = true;
			lexeme += advance();										// e or E
			
			if (peek() == '+' || peek() == '-')
			{
				lexeme += advance();									// + or -
			}

			while (std::isdigit(peek()))
			{
				lexeme += advance();
			}
		}

		// Suffix	| Regex: [uUlLfF]+
		while (true)
		{
			char c = peek();
			
			if (c == 'u' || c == 'U' || c == 'l' || c == 'L' || c == 'f' || c == 'F')
			{
				lexeme += advance();
			}

			else
			{
				break;
			}
		}

		// Final state
		if (is_float)
		{
			return createToken(TokenType::CONSTANT_FLOAT, lexeme);
		}
		else
		{
			return createToken(TokenType::CONSTANT_INTEGER, lexeme);
		}
	}
	
	Token Lexer::lexString()
	{
		advance();
		std::string lexeme = "";

		while (peek() != '"' && !isAtEnd())
		{
			if (peek() == '\\')
			{
				handleEscapeSequence(lexeme);
			}
			// Normal characters
			else
			{
				lexeme += advance();
			}
		}

		// Error handler
		if (isAtEnd())
		{
			return createToken(TokenType::TOKEN_ERROR, "Unterminated string literal");
		}

		advance();														// "

		return createToken(TokenType::STRING_LITERAL, lexeme);			// Final state
	}

	Token Lexer::lexCharacter()
	{
		advance();
		std::string lexeme = "";

		while (peek() != '\'' && !isAtEnd())
		{
			if (peek() == '\\')
			{
				handleEscapeSequence(lexeme);
			}
			else
			{
				lexeme += advance();
			}
		}

		if (isAtEnd()) return createToken(TokenType::TOKEN_ERROR, "Unterminated character constant");

		advance();														// '\''
		return createToken(TokenType::CONSTANT_CHARACTER, lexeme);
	}
	
	Token Lexer::createToken(TokenType type, std::optional<std::string> value)
	{
		return Token{ type, value, start_line, start_column };
	}
	
	Lexer::Lexer(std::string sourceCode)
		: 
		source{ std::move(sourceCode) }, cursor{ 0 }, line{ 1 }, column{ 1 },
		start_cursor{ 0 }, start_line{ 1 }, start_column{ 1 }
	{
	}
	
	Token Lexer::getNextToken()
	{
		// Skip whitespace
		skipWhitespace();
		
		start_line = line;
		start_column = column;
		start_cursor = cursor;

		// Cursor at EOF
		if (isAtEnd())
		{
			return createToken(TokenType::TOKEN_EOF);
		}

		// Identifier or Keyword
		if (std::isalpha(peek()) || peek() == '_' ||
			(peek() == '\\' && (peekNext() == 'u' || peekNext() == 'U')))
		{
			return lexIdentifierOrKeyword();
		}

		// Integer or Float or Hex or Octal
		if (std::isdigit(peek()))
		{
			return lexNumber();
		}

		// Float
		if (peek() == '.' && std::isdigit(peekNext()))
		{
			return lexNumber();
		}

		// String Literal
		if (peek() == '"')
		{
			return lexString();
		}

		// Character Constant
		if (peek() == '\'')
		{
			return lexCharacter();
		}

		// Operators & Punctuators
		return lexOperatorOrPunctuator();
	}

	Token Lexer::lexOperatorOrPunctuator()
	{
		char c = advance();

		switch (c)
		{
		// --- Single-Character Punctuators ---
		case '[': return createToken(TokenType::PUNCTUATOR_OPEN_BRACKET);
		case ']': return createToken(TokenType::PUNCTUATOR_CLOSE_BRACKET);
		case '(': return createToken(TokenType::PUNCTUATOR_OPEN_PARENTHESIS);
		case ')': return createToken(TokenType::PUNCTUATOR_CLOSE_PARENTHESIS);
		case '{': return createToken(TokenType::PUNCTUATOR_OPEN_BRACE);
		case '}': return createToken(TokenType::PUNCTUATOR_CLOSE_BRACE);
		case ',': return createToken(TokenType::PUNCTUATOR_COMMA);
		case ':': return createToken(TokenType::PUNCTUATOR_COLON);
		case ';': return createToken(TokenType::PUNCTUATOR_SEMI_COLON);
		case '?': return createToken(TokenType::OPERATOR_QUESTION_MARK);
		case '~': return createToken(TokenType::OPERATOR_BITWISE_NOT);

		// --- Multi-Character Operators & Punctuators ---
		case '#':
			return createToken(match('#') ? TokenType::PUNCTUATOR_DOUBLE_HASH
				: TokenType::PUNCTUATOR_PREPROCESSOR);

		case '+':
			if (match('=')) return createToken(TokenType::OPERATOR_PLUS_ASSIGNMENT);
			if (match('+')) return createToken(TokenType::OPERATOR_INCREMENT);
			return createToken(TokenType::OPERATOR_PLUS);

		case '-':
			if (match('=')) return createToken(TokenType::OPERATOR_MINUS_ASSIGNMENT);
			if (match('-')) return createToken(TokenType::OPERATOR_DECREMENT);
			if (match('>')) return createToken(TokenType::PUNCTUATOR_ARROW);
			return createToken(TokenType::OPERATOR_MINUS);

		case '*':
			return createToken(match('=') ? TokenType::OPERATOR_MULTIPLY_ASSIGNMENT
				: TokenType::OPERATOR_ASTERISK);

		case '/':
			return createToken(match('=') ? TokenType::OPERATOR_DIVIDE_ASSIGNMENT
				: TokenType::OPERATOR_SLASH);

		case '%':
			return createToken(match('=') ? TokenType::OPERATOR_MODULO_ASSIGNMENT
				: TokenType::OPERATOR_PERCENT);

		case '=':
			return createToken(match('=') ? TokenType::OPERATOR_EQUAL_TO
				: TokenType::OPERATOR_ASSIGNMENT);

		case '!':
			return createToken(match('=') ? TokenType::OPERATOR_NOT_EQUAL_TO
				: TokenType::OPERATOR_LOGICAL_NOT);

		case '&':
			if (match('=')) return createToken(TokenType::OPERATOR_BITWISE_AND_ASSIGNMENT);
			if (match('&')) return createToken(TokenType::OPERATOR_LOGICAL_AND);
			return createToken(TokenType::OPERATOR_BITWISE_AND);

		case '|':
			if (match('=')) return createToken(TokenType::OPERATOR_BITWISE_OR_ASSIGNMENT);
			if (match('|')) return createToken(TokenType::OPERATOR_LOGICAL_OR);
			return createToken(TokenType::OPERATOR_BITWISE_OR);

		case '^':
			return createToken(match('=') ? TokenType::OPERATOR_BITWISE_XOR_ASSIGNMENT
				: TokenType::OPERATOR_BITWISE_XOR);

		case '<':
			if (match('=')) return createToken(TokenType::OPERATOR_LESS_THAN_OR_EQUAL);
			if (match('<'))
			{
				return createToken(match('=') ? TokenType::OPERATOR_LEFT_SHIFT_ASSIGNMENT
					: TokenType::OPERATOR_LEFT_SHIFT);
			}
			return createToken(TokenType::OPERATOR_LESS_THAN);

		case '>':
			if (match('=')) return createToken(TokenType::OPERATOR_GREATER_THAN_OR_EQUAL);
			if (match('>'))
			{
				return createToken(match('=') ? TokenType::OPERATOR_RIGHT_SHIFT_ASSIGNMENT
					: TokenType::OPERATOR_RIGHT_SHIFT);
			}
			return createToken(TokenType::OPERATOR_GREATER_THAN);

		case '.':
			if (match('.'))
			{
				if (match('.')) return createToken(TokenType::PUNCTUATOR_ELLIPSIS);
				return createToken(TokenType::TOKEN_ERROR, "..");
			}
			return createToken(TokenType::PUNCTUATOR_DOT);

		// --- Fallthrough Error Handling ---
		default:
			return createToken(TokenType::TOKEN_ERROR, std::string(1, c));
		}
	}
	
	std::vector<Token> Lexer::tokenize()
	{
		std::vector<Token> tokens;

		while (true)
		{
			Token token = getNextToken();
			tokens.push_back(token);

			if (token.type == TokenType::TOKEN_EOF)
			{
				break;
			}
		}

		return tokens;
	}

	bool Lexer::match(char expected)
	{
		if (isAtEnd() || peek() != expected) return false;
		advance();
		return true;
	}

	void Lexer::handleEscapeSequence(std::string& lexeme)
	{
		advance();														// '\'
		char escape = advance();

		switch (escape)
		{
		// --- Simple Escapes ---
		case 'n':  lexeme += '\n'; break;
		case 't':  lexeme += '\t'; break;
		case 'r':  lexeme += '\r'; break;
		case '\\': lexeme += '\\'; break;
		case '"':  lexeme += '"';  break;
		case '\'': lexeme += '\''; break;
		case '?':  lexeme += '\?'; break;								// Question mark
		case 'a':  lexeme += '\a'; break;								// Alert / Bell
		case 'b':  lexeme += '\b'; break;								// Backspace
		case 'f':  lexeme += '\f'; break;								// Form feed
		case 'v':  lexeme += '\v'; break;								// Vertical tab

		// --- Hexadecimal Escapes (\x1A) ---
		case 'x':
		{
			int hexValue = 0;
			// Hexadecimal escapes consume all valid subsequent hex digits
			while (std::isxdigit(peek()))
			{
				char h = advance();
				int digit = std::isdigit(h) ? (h - '0') : (std::tolower(h) - 'a' + 10);
				hexValue = (hexValue * 16) + digit;
			}
			lexeme += static_cast<char>(hexValue);
			break;
		}

		// --- Universal Character Names (\u03A9 or \U000003A9) ---
		case 'u':
		case 'U':
		{
			int numDigits = (escape == 'u') ? 4 : 8;
			uint32_t codePoint = 0;
			bool valid = true;

			for (int i = 0; i < numDigits; ++i)
			{
				if (!std::isxdigit(peek()))
				{
					valid = false;
					break; // Malformed UCN
				}
				char h = advance();
				uint32_t digit = std::isdigit(h) ? (h - '0') : (std::tolower(h) - 'a' + 10);
				codePoint = (codePoint << 4) | digit;
			}

			if (!valid || codePoint > 0x10FFFF)
			{
				// Fallback for invalid/malformed Unicode
				lexeme += '\\';
				lexeme += escape;
				break;
			}

			// Convert valid code point to UTF-8 bytes
			if (codePoint <= 0x7F)
			{
				lexeme += static_cast<char>(codePoint);
			}
			else if (codePoint <= 0x7FF)
			{
				lexeme += static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F));
				lexeme += static_cast<char>(0x80 | (codePoint & 0x3F));
			}
			else if (codePoint <= 0xFFFF)
			{
				lexeme += static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F));
				lexeme += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
				lexeme += static_cast<char>(0x80 | (codePoint & 0x3F));
			}
			else // 0x10000 to 0x10FFFF
			{
				lexeme += static_cast<char>(0xF0 | ((codePoint >> 18) & 0x07));
				lexeme += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
				lexeme += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
				lexeme += static_cast<char>(0x80 | (codePoint & 0x3F));
			}
			break;
		}

		// --- Octal Escapes (\0, \123) ---
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		{
			int octalValue = escape - '0';
			int count = 1;

			// Octal escapes have a strict maximum of 3 digits
			while (count < 3 && peek() >= '0' && peek() <= '7')
			{
				octalValue = (octalValue * 8) + (advance() - '0');
				count++;
			}
			lexeme += static_cast<char>(octalValue);
			break;
		}

		// --- Invalid Escapes ---
		default:
			// Fallback for invalid escape sequences: emit the literal character
			lexeme += escape;
			break;
		}
	}
}
