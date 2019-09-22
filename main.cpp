#include <iostream>
#include <vector>
#include <cmath>

#include "R3DNoise/R3DNoise.h"

int main()
{
	int32_t m_chunk_line_elements = 16;
	int32_t m_chunk_index_X = 1;
	int32_t m_chunk_index_Y = 1;

	std::vector<float> noise;
	float y_var;
	float x_var;

	float first_pass;
	float second_pass;
	float third_pass;
	float fourth_pass;


	std::cout << "##### Computing RESULT #######" << std::endl;
	for (int32_t y = 0; y < m_chunk_line_elements; y++)
	{
		for (int32_t x = 0; x < m_chunk_line_elements; x++)
		{
			first_pass = R3DNoise::Noise2D(x * 0.001f, y * 0.001f, 5.0f);
			second_pass = R3DNoise::Noise2D(x * 0.01f, y * 0.01f);
			third_pass = R3DNoise::Noise2D(x * 0.004f, y * 0.004f);

			std::cout << first_pass << "-" << second_pass << "-" << third_pass << "-" << std::endl;

			first_pass *= 4.0f;
			second_pass *= 8.0f;
			third_pass *= 16.0f;

			noise.push_back(std::floor(first_pass + second_pass + third_pass));
		}
	}


	std::cout << std::endl << "##### FINAL RESULT #######" << std::endl;
	for (size_t i = 0; i < noise.size(); i++)
	{
		std::cout << noise[i] << std::endl;
	}

	return 0;
}