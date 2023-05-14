#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using std::cerr;
using std::cin;
using std::copy;
using std::cout;
using std::endl;
using std::sort;
using std::string;
using std::to_string;
using std::vector;

class Console
{
public:
    enum Color
    {
        RESET = 0,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        CYAN = 36
    };

    static string color_code(Console::Color color)
    {
        return "\033[" + to_string(color) + "m";
    }

    static void move_up(unsigned lines)
    {
        cout << "\033[0G\033[" << lines << "A";
    }

    static void erase_line()
    {
        cout << "\033[2K";
    }
};

class SearchData
{
private:
    void display_array(vector<Console::Color> colors)
    {
        if (colors.size() < this->array.size())
        {
            for (vector<int>::size_type i = 0; i < this->array.size() - colors.size(); i++)
            {
                colors.push_back(Console::Color::RESET);
            }
        }

        for (vector<int>::size_type i = 0; i < this->array.size(); i++)
        {
            cout << Console::color_code(colors[i]) << this->array[i] << Console::color_code(Console::Color::RESET) << " ";
        }

        cout << endl;
    }

public:
    int search_value;
    vector<int> array;
    vector<int>::size_type result_index;
    vector<vector<int>::size_type> steps;

    SearchData()
        : result_index((size_t)(-1)), steps({})
    {
        cout << "Do you want to generate randomized array? [Y/n]: " << Console::color_code(Console::Color::YELLOW);

        char decision = getchar();
        cout << Console::color_code(Console::Color::RESET);

        cout << endl
             << "Specify the size of the array: " << Console::color_code(Console::Color::YELLOW);

        vector<int>::size_type array_size;
        cin >> array_size;
        cout << Console::color_code(Console::Color::RESET) << "Array: ";

        this->array.reserve(array_size);
        int value;

        if (decision == 'n' || decision == 'N')
        {
            for (vector<int>::size_type i = 0; i < array_size; i++)
            {
                cin >> value;
                this->array.push_back(value);
            }
        }
        else
        {
            srand(time(0));

            for (vector<int>::size_type i = 0; i < array_size; i++)
            {
                value = rand() % 200 - 100;
                this->array.push_back(value);

                cout << value << " ";
            }

            cout << endl;
        }

        sort(this->array.begin(), this->array.end());
        cout << endl
             << Console::color_code(Console::Color::RED) << "Binary search algorithm can only search in a sorted array, so the array will be sorted." << endl
             << Console::color_code(Console::Color::RESET) << "Sorted array:";
        for (vector<int>::size_type i = 0; i < array_size; i++)
        {

            cout << this->array[i] << " ";
        }
        cout << endl;

        cout << endl
             << "Specify the value you want to search: " << Console::color_code(Console::Color::YELLOW);
        cin >> this->search_value;
        cout << Console::color_code(Console::Color::RESET) << endl;
    }

    void clear()
    {
        this->result_index = (size_t)(-1);
        this->steps.clear();
    }

    void render_search()
    {
        vector<Console::Color> colors(this->array.size());

        cout << "Searching for: " << Console::color_code(Console::Color::YELLOW) << this->search_value << Console::color_code(Console::Color::RESET) << endl
             << "Steps done: " << Console::color_code(Console::Color::YELLOW) << 0 << Console::color_code(Console::Color::RESET) << endl;

        this->display_array(colors);

        for (vector<int>::size_type i = 0; i < this->steps.size(); i++)
        {
            Console::move_up(2);

            cout << "Steps done: " << Console::color_code(Console::Color::YELLOW) << (i + 1) << Console::color_code(Console::Color::RESET) << endl;

            colors[this->steps[i]] = Console::Color::BLUE;
            Console::erase_line();
            display_array(colors);
            colors[this->steps[i]] = Console::Color::CYAN;

#ifdef _WIN32
            Sleep(1000);
#else
            sleep(1)
#endif
        }

        Console::move_up(1);

        if (this->result_index != (size_t)(-1))
        {
            colors[this->result_index] = Console::Color::GREEN;
            display_array(colors);

            cout << "Value found at the position " << Console::color_code(Console::Color::YELLOW) << this->result_index << Console::color_code(Console::Color::RESET) << endl;
        }
        else
        {
            display_array(colors);

            cout << Console::color_code(Console::Color::RED) << "Value was not found" << Console::color_code(Console::Color::RESET) << endl;
        }
    }
};

class SearchAlgorithms
{
public:
    static void binary_search(SearchData &data)
    {

        vector<int>::size_type left = 0;
        vector<int>::size_type right = data.array.size();

        while (left <= right)
        {
            int position = left + (right - left) / 2;

            data.steps.push_back(position);

            if (data.array[position] == data.search_value)
            {
                data.result_index = position;
                break;
            }

            if (data.array[position] < data.search_value)
            {
                left = position + 1;
            }
            else
            {
                right = position - 1;
            }
        }
    }

    static void linear_search(SearchData &data)
    {
        for (vector<int>::size_type i = 0; i < data.array.size(); i++)
        {
            data.steps.push_back(i);

            if (data.array[i] == data.search_value)
            {
                data.result_index = i;
                return;
            }
        }
    }
};

int main(void)
{
    SearchData search_data;

    std::cout << Console::color_code(Console::Color::BLUE) << "Binary search" << Console::color_code(Console::Color::RESET) << endl;
    SearchAlgorithms::binary_search(search_data);
    search_data.render_search();

    std::cout << endl;
    search_data.clear();

    std::cout << Console::color_code(Console::Color::BLUE) << "Linear search" << Console::color_code(Console::Color::RESET) << endl;
    SearchAlgorithms::linear_search(search_data);
    search_data.render_search();

    return EXIT_SUCCESS;
}