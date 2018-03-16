#pragma once

#include <vector>
#include <string>
#include <fstream>

#define LINE '\n'

class ExclWriter {
public:
    /*
        This row major write to file
    */
    template<typename t>
    static void writeToFile(std::string path, int headers, const std::string *header,
        std::vector<std::vector<t>> values, char sep = ',')
    {
        std::ofstream str(path);

        for (int i = 0; i < headers; i++) {
            str << header[i];
            if (i < headers - 1)
                str << sep;
        }
        
        str << LINE;

        for (int i = 0; i < values.size(); i++) {
            for (int j = 0; j < values[i].size(); j++) {
                str << values[i][j];
                if (j < values[i].size() - 1)
                    str << sep;
            }
            if (i < values.size() - 1)
                str << LINE;
        }

        str.close();
    }
};