#include <bits/stdc++.h>

using namespace std;

// struct to store input data
struct inputData
{
    string inputString;
    vector <string> regexStrings;
};

// function to read input from file
inputData readFile(const string& fileName)
{
    // make a struct to store data
    inputData data;

    // error handling for file
    ifstream inputFile(fileName);
    if (!inputFile.is_open())
    {
        std :: cerr << "Error opening the file." << endl;
        return data;
    }

    // read the first line of file as input string
    if (std :: getline(inputFile, data.inputString))
    {
        // read remaining lines and store in regex container
        string line;

        while (std :: getline(inputFile, line))
            data.regexStrings.push_back(line);
    }

    // close input file
    inputFile.close();

    return data;
}

// testing
int main()
{
    string fileName = "input.txt";
    inputData data = readFile(fileName);

    // display string
    cout << "Input String: " << data.inputString << endl;

    // display regex strings
    cout << "Regex Strings: " << endl;
    for (auto& str : data.regexStrings)
        cout << str << endl;

    return 0;
}