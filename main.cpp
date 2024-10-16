#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <functional>
#include <climits>

#include "Node.h"
#include "Connection.h"
#include "Utils.h"
#include "Snapshotter.h"

NodeInfo readConfig(std::string configFile, int popId = -1)
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
    int minPerActive;
    int maxPerActive;
    int minSendDelay;
    int snapshotDelay;
    int maxNumber;
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
        minPerActive = Utils::strToInt(firstLine[1]);
        maxPerActive = Utils::strToInt(firstLine[2]);
        minSendDelay = Utils::strToInt(firstLine[3]);
        snapshotDelay = Utils::strToInt(firstLine[4]);
        maxNumber = Utils::strToInt(firstLine[5]);
    }


    std::vector<Node> nodes;
    for(int i = 1; i < numNodes+1;i++)
    {
        auto splitNode = Utils::split(lines[i]," ");

        if(splitNode.size() == 3)
        {
            std::istringstream uidSS(splitNode[0]); 
            std::istringstream portSS(splitNode[2]); 

            int uid;
            uint32_t port;

            uidSS >> uid;
            portSS >> port;

            Node n(uid,{uid,splitNode[1],port});
            nodes.push_back(n);
            //Utils::log( "adding node " , uid );

        }
        else
        {
            Utils::log( "node Line " , i , " is invalid" );
        }
        
    }


    for(int i = numNodes+1; i < lines.size(); i++)
    {
        std::string conLine = lines[i];
        auto connections = Utils::split(conLine," ");

        int nearUid = i - numNodes - 1;
        for(auto connectionNodeId : connections)
        {
            int uid = Utils::strToInt(connectionNodeId);

            for(int j = 0; j < nodes.size();j++)
            {
                if(nodes[j].getUid() == uid)
                {
                    nodes[nearUid].addConnection(nodes[j].getOwnConnection());
                }
            }
        }
    }

/*    for(auto& n : nodes)
    {
        n.addConnection(
            {n.getUid(),
             n.getOwnConnection().getHostname(),
             n.getOwnConnection().getPort(),
             INT_MAX});
    }*/

    /*for(auto& n : nodes)
    {
        n.print();
    }*/
    if(popId != -1)
    {
        for(auto n : nodes)
        {
            if(n.getUid() == popId)
            {
                return {minPerActive,
                        maxPerActive,
                        minSendDelay,
                        snapshotDelay,
                        maxNumber,
                        n};
            }
        }   

    }

    Utils::log("returning node 0");
    return {minPerActive,
            maxPerActive,
            minSendDelay,
            snapshotDelay,
                maxNumber,
                nodes[0]};
}

void runAlg(NodeInfo& ni)
{
    ni.n.print();
    ni.n.openSocket();

    Snapshotter s(ni);

    ni.n.setHandler(std::bind(&Snapshotter::handleMsg,&s,std::placeholders::_1));

    ni.n.connectAll();

    ni.n.acceptNeighbors();

    if(ni.n.getUid() == 0)
    {
        s.init();
    }

    std::thread releaseThrd(&Node::releaseMessagesThread, &ni.n,250);
    releaseThrd.detach();

    ni.n.listenToNeighbors();
}

int main(int argc,char** argv)
{
    if(argc == 3)
    {
        int uid = std::stoi(argv[2]);

        auto nodeData = readConfig(argv[1],uid);
        nodeData.configName = argv[1];

        runAlg(nodeData);
    }
    else
    {
        readConfig("asyncConfig.txt");
    }
    return 0;
}
