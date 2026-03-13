#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "lexer.h"

int main()
{
	std::string inputFilePath = "tests/sqlite3.c";
	std::string outputFilePath = "tests/lexer_log.txt";

	std::cout << "Loading " << inputFilePath << " into memory...\n";

	// 1. Read the massive file into a string
	std::ifstream inFile(inputFilePath);
	if (!inFile.is_open())
	{
		std::cerr << "Failed to open " << inputFilePath << "! Check your directory.\n";
		return 1;
	}
	std::stringstream buffer;
	buffer << inFile.rdbuf();
	std::string sourceCode = buffer.str();
	inFile.close();

	std::cout << "File loaded. Starting Lexical Analysis...\n";

	// Start a timer for performance benchmarking
	auto start_time = std::chrono::high_resolution_clock::now();

	// 2. Instantiate and run the Lexer
	CCompiler::Lexer lexer(sourceCode);
	std::vector<CCompiler::Token> tokens = lexer.tokenize();

	// Stop the timer
	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = end_time - start_time;

	std::cout << "Tokenized " << tokens.size() << " tokens in " << duration.count() << " ms.\n";
	std::cout << "Writing results to " << outputFilePath << "...\n";

	// 3. Write the output to a log file
	std::ofstream outFile(outputFilePath);
	if (!outFile.is_open())
	{
		std::cerr << "Failed to create log file!\n";
		return 1;
	}

	outFile << std::left
		<< std::setw(15) << "Line:Col"
		<< std::setw(25) << "Token Type (Int)"
		<< "Lexeme\n";
	outFile << std::string(60, '-') << "\n";

	for (const auto& token : tokens)
	{
		std::string position = std::to_string(token.line) + ":" + std::to_string(token.column);
		std::string lexeme = token.value.has_value() ? token.value.value() : "N/A";

		outFile << std::left
			<< std::setw(15) << position
			<< std::setw(25) << static_cast<int>(token.type)
			<< lexeme << "\n";
	}

	outFile.close();
	std::cout << "Done! Open " << outputFilePath << " to view the results.\n";

	return 0;
}