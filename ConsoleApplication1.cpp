#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <stdexcept>

std::string GetCurrentDate()
{
    std::time_t now = std::time(nullptr);
    char buffer[9];
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::strftime(buffer, sizeof(buffer), "%Y%m%d", &localTime);
    return std::string(buffer);
}

std::string GetPreviousDate()
{
    std::time_t now = std::time(nullptr);
    std::time_t previousDay = now - (24 * 60 * 60); // Subtract 24 hours in seconds
    char buffer[9];
    std::tm localTime;
    localtime_s(&localTime, &previousDay);
    std::strftime(buffer, sizeof(buffer), "%Y%m%d", &localTime);
    return std::string(buffer);
}

std::string ModifyFileContent(const std::string& content)
{
    std::stringstream modifiedContent;
    std::stringstream reader(content);
    std::string line;
    while (std::getline(reader, line))
    {
        if (line.find("START_DATE=") == 0)
        {
            modifiedContent << "START_DATE=" << GetPreviousDate() << std::endl;
        }
        else if (line.find("END_DATE=") == 0)
        {
            modifiedContent << "END_DATE=" << GetCurrentDate() << std::endl;
        }
        else
        {
            modifiedContent << line << std::endl;
        }
    }
    return modifiedContent.str();
}

int main()
{
    std::string filePath = "D:\\Jobs\\JOB0661-Move_Data_Ship2OtherSite\\SFIS.ini";

    try
    {
        std::ifstream inputFile(filePath);
        if (!inputFile)
        {
            std::cerr << "Error opening input file." << std::endl;
            return 1;
        }

        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        std::string fileContent = buffer.str();

        std::string modifiedContent = ModifyFileContent(fileContent);

        inputFile.close();

        std::ofstream outputFile(filePath);
        if (!outputFile)
        {
            std::cerr << "Error opening output file." << std::endl;
            return 1;
        }
        outputFile << modifiedContent;
        outputFile.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
