#include "lexer.h"

namespace CCompiler
{
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
					while (c != '\n' && !isAtEnd())
					{
						advance();
					}
					continue;
				}

				if (next_c == '*')
				{
					advance();
					advance(); 

					while (!(c == '*' && next_c == '/') && !isAtEnd())
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
		return Token();
	}
	
	Token Lexer::lexNumber()
	{
		return Token();
	}
	
	Token Lexer::lexString()
	{
		return Token();
	}
	
	Token Lexer::createToken(TokenType type, std::optional<std::string> value)
	{
		return Token();
	}
	
	Lexer::Lexer(std::string sourceCode)
	{
	}
	
	Token Lexer::getNextToken()
	{
		return Token();
	}
	
	std::vector<Token> Lexer::tokenize()
	{
		return std::vector<Token>();
	}
}
