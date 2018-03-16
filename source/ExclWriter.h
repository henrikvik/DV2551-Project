#pragma once

#include <vector>
#include <string>
#include <fstream>

class ExclWriter {
public:
    /*
        This row major write to file
    */
    template<typename t>
    static void writeToFile(std::string path, std::vector<std::vector<t>> values, char sep = ',')
    {
        std::ofstream str(path);

        for (int i = 0; i < values.size(); i++) {
            for (int j = 0; j < values[i].size(); j++) {
                str << values[i][j];
                if (j < values[i].size() - 1)
                    str << sep;
            }
            if (i < values.size() - 1)
                str << "\n";
        }

        str.close();
    }
};