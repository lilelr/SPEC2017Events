#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <sys/stat.h>
#include <set>

using boost::lexical_cast;
using boost::algorithm::is_any_of;
using boost::token_compress_off;


using namespace std;

// /Users/yuxiao/Lab/projects/SPEC-CPU2017/资料
int main() {

    FILE *new_events = fopen("/Users/yuxiao/Lab/projects/SPEC-CPU2017/资料/newhaswell-e.csv", "r");
    FILE *new_events_code = fopen("/Users/yuxiao/Lab/projects/SPEC-CPU2017/资料/newhaswell-events.csv", "w");
    FILE *set_events = fopen("/Users/yuxiao/Lab/projects/SPEC-CPU2017/资料/sethaswell-e.csv", "w");
    int lines = 0;
    char line[400];
    vector<string> line_cols;
    set<string> code_set;
    vector<string> events_v;
    while (!feof(new_events)) {
        if (fscanf(new_events, "%[^\n]%*[\n]", &line[0]) > 0) {
            boost::split(line_cols, line, is_any_of(","), token_compress_off);
            if (code_set.find(line_cols[0]) == code_set.end()) {
                code_set.insert(line_cols[0]);


//                string temp;
//                for(auto iter=line_cols.begin()+1;iter!=line_cols.end()-1;iter++){
//                    temp +=*iter+",";
//                }
//                temp+=line_cols[line_cols.size()-1];
//                cout<<line_cols[0]<<"  "<<temp<<endl;
                fprintf(new_events_code, "%s,\n", line_cols[0].c_str());
                fprintf(set_events, "%s\n", line);
                events_v.push_back(line_cols[0]);
                lines++;
            }
        }
    }
    fclose(new_events);
    fclose(new_events_code);
    fclose(set_events);
    cout << "events count" << lines << endl;
    int output_event_c = 0;
    for (int i = 1; (i+3) <= lines; i = i + 4) {
        output_event_c++;
        string file_name = "/Users/yuxiao/Lab/projects/SPEC-CPU2017/资料/perf-events/archi_";
        file_name += lexical_cast<string>(output_event_c);
        file_name += ".txt";
        cout<<file_name<<endl;
        FILE *temp_file_event_code = fopen(file_name.c_str(), "w");
        string content = "cycles,instructions,";
        content+=  events_v[i-1]+",";
        content+=events_v[i]+",";
        content+=events_v[i+1]+",";
        content+=events_v[i+2];
        cout<<content<<endl;
        fprintf(temp_file_event_code, "%s\n",content.c_str());
        fclose(temp_file_event_code);
    }


    string file_name = "/Users/yuxiao/Lab/projects/SPEC-CPU2017/资料/perf-events/archi_59.txt";
    FILE *temp_file_event_code = fopen(file_name.c_str(), "w");
    string content = "cycles,instructions,";
    content+=  events_v[events_v.size()-2]+",";
    content+=events_v[events_v.size()-1];

    fprintf(temp_file_event_code, "%s\n",content.c_str());
    fclose(temp_file_event_code);

    cout << "Hello, World!" << endl;
    return 0;
}