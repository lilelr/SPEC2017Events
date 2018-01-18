//
// Created by YuXiao on 18/1/18.
//

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <sys/stat.h>
#include <set>
#include <unordered_map>

using boost::lexical_cast;
using boost::algorithm::is_any_of;
using boost::token_compress_off;
using namespace boost;


using namespace std;

int main() {

    string output_name = "it-65.csv";
    string output_path = "/Users/yuxiao/Lab/projects/SPEC-CPU2017/508-name-d/namd_r_base.myt-result/";
    string input_path = "/Users/yuxiao/Lab/projects/SPEC-CPU2017/508-name-d/namd_r_base.myt-result/it-65/";
    int lines = 0;
    char line[400];
    vector<string> line_cols;
    unordered_map<string, string> code_name_map;

    FILE *set_events = fopen("/Users/yuxiao/Lab/projects/SPEC-CPU2017/资料/sethaswell-e.csv", "r");
    string first_line = "index";
    while (!feof(set_events)) {
        if (fscanf(set_events, "%[^\n]%*[\n]", &line[0]) > 0) {
            boost::split(line_cols, line, is_any_of(","), token_compress_off);
            string temp(line_cols[1]);
            temp = to_lower_copy(temp);
//            cout<<line_cols[0]<<"  "<<temp<<endl;
            code_name_map.insert({line_cols[0], temp});
            first_line += ("," + temp);
        }
    }
    first_line += (",IPC");
    fclose(set_events);

    string input_file_name = "namd_r_base.myt_it-65_";
    string output_file_path = output_path + output_name;
    cout << output_file_path << endl;
    FILE *output_table = fopen(output_file_path.c_str(), "w");
    fprintf(output_table, "%s\n", first_line.c_str());
    int total = 1;
    for (int i = 1; i <= 59; i++) {
        string input_file_path = input_path + input_file_name + lexical_cast<string>(i) + ".csv";
        cout << input_file_path << endl;
        FILE *input_file = fopen(input_file_path.c_str(), "r");

        // pass the first two rows
        fscanf(input_file, "%[^\n]%*[\n]", &line[0]);

        while (!feof(input_file)) {

            vector<string> temp_line(236, "0");
            temp_line[0] = lexical_cast<string>(total);
            total++;
            double instructions;
            double cycles;
            double ipc;
            if (fscanf(input_file, "%[^\n]%*[\n]", &line[0]) > 0) {
                boost::split(line_cols, line, is_any_of(","), token_compress_off);
                // cycles == <not counted>
                if (line_cols[1] == "<not counted>") {
                    break;
                }
                cycles = lexical_cast<double>(line_cols[1]);

            }

            if (fscanf(input_file, "%[^\n]%*[\n]", &line[0]) > 0) {
                boost::split(line_cols, line, is_any_of(","), token_compress_off);
                instructions = lexical_cast<double>(line_cols[1]);
            }
            ipc = instructions / cycles;
            temp_line[235] = lexical_cast<string>(ipc);

            string event_1, event_2, event_3, event_4;
            int start_index = 4 * i - 3;
            if (fscanf(input_file, "%[^\n]%*[\n]", &line[0]) > 0) {
                boost::split(line_cols, line, is_any_of(","), token_compress_off);
                event_1 = line_cols[1];
                temp_line[start_index] = event_1;
            }

            if (fscanf(input_file, "%[^\n]%*[\n]", &line[0]) > 0) {
                boost::split(line_cols, line, is_any_of(","), token_compress_off);
                event_2 = line_cols[1];
                temp_line[start_index + 1] = event_2;

            }

            if (i == 59) {
                string final_line = join(temp_line, ",");
                fprintf(output_table, "%s\n", final_line.c_str());
                continue;
            }
            if (fscanf(input_file, "%[^\n]%*[\n]", &line[0]) > 0) {
                boost::split(line_cols, line, is_any_of(","), token_compress_off);
                event_3 = line_cols[1];
                temp_line[start_index + 2] = event_3;

            }
            if (fscanf(input_file, "%[^\n]%*[\n]", &line[0]) > 0) {
                boost::split(line_cols, line, is_any_of(","), token_compress_off);
                event_4 = line_cols[1];
                temp_line[start_index + 3] = event_4;
            }
            string final_line = join(temp_line, ",");
            fprintf(output_table, "%s\n", final_line.c_str());

        }

        fclose(input_file);
    }


    fclose(output_table);

}