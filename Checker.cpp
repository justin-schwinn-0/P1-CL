#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
void readConfig(std::string configFile)
{
    std::ifstream file(configFile) ;
    Utils::log( "reading file: " , configFile );

    std::string wholeFile;
    std::string line;

    while(std::getline(file,line))
    {
        wholeFile += line + "\n";
    }
    auto lines = Utils::split(wholeFile,"\n");

    // sanitize lines to remove comments
    
    for(auto it = lines.rbegin(); it != lines.rend(); it++)
    {
        // erase the while line if it starts with #
        std::string& str = *it;
        if(str.at(0) == '#')
        {
            lines.erase((it+1).base());
        }
        else
        {
           size_t pos = 0;
           // if we find a #, erase it and everything after
           if((pos = str.find("#")) != std::string::npos)
           {
               str.erase(str.begin()+pos,str.end());
           }
        }
    }
    // use lines to create N nodes, and return 1 for this process
    auto firstLine = Utils::split(lines[0]," ");

    int numNodes;
    if(firstLine.size() != 6)
    {
        // if length of the first line isn't 1,
        // then something when wrong in the sanitization 
        // or the config is invalid
        Utils::log( "First valid line of Config is wrong length!" );
    }
    else
    {
        numNodes = Utils::strToInt(firstLine[0]);
    }

    std::vector<int> uids;
    for(int i = 1; i < numNodes+1;i++)
    {
        auto splitNode = Utils::split(lines[i]," ");

        if(splitNode.size() == 3)
        {
            uids.push_back(Utils::strToInt(splitNode[0])); 
        }
        else
        {
            Utils::log( "node Line " , i , " is invalid" );
        }
    }

    std::vector<std::vector<VectorClock>> clocks(numNodes);

    for(int uid : uids)
    {
        std::string fn = configFile+"-"+std::to_string(uid)+".out";
        Utils::log("reading",fn);

        std::ifstream outputfile(fn);

        int snapNum = 0;
        while(std::getline(outputfile,line))
        {
            clocks[uid].push_back(line); 
            Utils::log(clock)
        }

        outputfile.close();
    }


    for(auto& str : fileContents)
    {
        std::cout << str<< std::endl;
    }
}

int main(int argc,char** argv)
{
    if(argc == 2)
    {
        readConfig(argv[1]);
    }
}
