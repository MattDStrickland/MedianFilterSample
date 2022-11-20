//----------------------------------------------------------------------------------------------------
// Main.cpp
// Reads a text file containing floating point numbers and outputs the median of the last 10 numbers.
//----------------------------------------------------------------------------------------------------

#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

/// @brief Reads through an input stream, reading in floats, and ignoring non-numbers, then working out the median,
///        after each line is read in.
/// @param input_file The file stream of in open text file.
void process_file(std::ifstream& input_file)
{
    // A vector to keep track of all the numbers read in.
    std::vector<float> current_numbers;
    
    // Loop through the file, grabbing each line.
    std::string line;
    while (std::getline(input_file, line))
    {
        // Convert to istringstream to read into a float easily.
        std::istringstream line_stream{line};
        auto number = 0.0f;
        if (line_stream >> number)
        {
            // We have successfully got a float so push it into our vector.
            current_numbers.push_back(number);

            auto start = current_numbers.begin();
            auto calc_size = static_cast<int>(current_numbers.size());

            // Check if we have more than 10 values in our vector in which case we just want to operate on the last 10.
            if (calc_size > 10)
            {
                start = current_numbers.end() - 10;
                calc_size = 10;
            }
            // Work out where median index we be.
            auto median_index = calc_size / 2;

            // Copy the last up to 10 values from all the numbers we have read in.
            std::vector<float> sorted_numbers;
            std::copy(start, current_numbers.end(), std::back_inserter(sorted_numbers));

            // Do a sort up to the point where the median will be, as that is all we need.
            std::nth_element(sorted_numbers.begin(), sorted_numbers.begin() + median_index, sorted_numbers.end());

            // If the number of values we have in our sorted vector is even then we average the 2 middle values.
            auto median = 0.0f;
            if (calc_size % 2 == 0)
            {
                median = (sorted_numbers[median_index - 1] + sorted_numbers[median_index]) / 2.0f;
            }
            else
            {
                median = sorted_numbers[median_index];
            }

            // If the number is within +/- 5 of the median then print it out.
            if (number <= median + 5.0f && number >= median - 5.0f)
            {
                std::cout << number << std::endl;
            }
        }
    }
}


int main(int argc, char* argv[])
{
    // Check if we have a filepath argument
    if (argc > 1)
    {
        // Get the path of the file we want to load.
        std::filesystem::path file_path(argv[1]);

        try
        {
            // Check whether the path exists.
            if (std::filesystem::exists(file_path))
            {
                // Open file and process the file.
                std::ifstream input_file{ file_path };
                if (input_file.is_open())
                {
                    process_file(input_file);
                }
                else
                {
                    std::cout << "Unable to open file!" << std::endl;
                }
            }
            else
            {
                std::cout << "File does not exist!" << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Exception returned: " << e.what() << std::endl;
        }
    }
    else
    {
        std::cout << "You need to enter a filename to read in." << std::endl;
    }

    return 0;
}