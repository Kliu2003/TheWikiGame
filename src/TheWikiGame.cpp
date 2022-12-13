#include "TheWikiGame.h"
#include <queue>
#include <algorithm>

TheWikiGame::TheWikiGame(std::string titleFile, std::string linksFile) {
    //if constructor is not working properly, copy and paste the contents of the two files into new files and use them
    ifstream wordsFile(titleFile);
    std::string word;
    int count = 1;  //arbitrary id assigned to each article
    if (wordsFile.is_open()) {
        while (getline(wordsFile, word)) { //get each title, one title per line
            std::string url = "https://en.wikipedia.org/wiki/"; 
            std::string uword = ""; //convert title to url
            for (char ch : word) { 
                if (ch == ' ') {
                    uword.push_back('_');
                } else { 
                    uword.push_back(ch);
                }
            }
            url = url+uword; 
            linkToId[url] = count; //populate data structures
            idToLink[count] = url;
            count++;     // change the id
        }
    }
    for(int i = 1; i < count; i++){
        idToPageRank[i] = 1/(count-1);
    }
    ifstream wordsFile1(linksFile);
    std::string line;
    bool odd = true; 
    std::string curword = "";
    if (wordsFile1.is_open()) {
        while (getline(wordsFile1, line)) { 
            if(odd) { //the file is formatted with titles on odd lines and links on even lines, use boolean to switch between the two
                if (line != "\n") {
                    curword = line; 
                    std::string url = "https://en.wikipedia.org/wiki/";
                    std::string uword = "";
                    for (char ch : curword) { 
                        if (ch == ' ') {
                            uword.push_back('_');
                        } else { 
                            uword.push_back(ch);
                        }
                    }
                    curword = url+uword; // convert title to url
                    odd = false;                     
                }
            } else { 
                if (line != "\n") {
                    std::vector<int> tempvect; 
                    std::string process = "";
                    for (char c : line) { 
                        if (c == ',') { 
                            if (linkToId.count(process) == 1) { //if the url is a part of the subset, push it back to the vector of links associated with current article
                                tempvect.push_back(linkToId[process]); 
                            }
                            process = ""; 
                        } else {
                            process.push_back(c);  //append characters to form url
                        }
                    }
                    if (linkToId.count(process) == 1) {
                        tempvect.push_back(linkToId[process]);  //push last url back if valid
                    }
                    odd = true; 
                    
                    int curwordint = linkToId[curword]; //get the id associated with current article


                    if (curwordint != 0) { 
                        directedAdjacencyList[curwordint] = tempvect; //if this id is valid, map the article and associated links
                    }                    
                }
            }               
        }
    }

}


std::string TheWikiGame::getLinkByID(int id) {
    if (idToLink.count(id) == 0) {
        return "ID not valid";
    } else { 
        return idToLink[id];
    }
}
int TheWikiGame::getIDByLink(std::string url) {
    if (linkToId.count(url) == 0) {
        return -1;
    } else { 
        return linkToId[url];
    }
}
std::vector<std::string> TheWikiGame::bfs(std::string startLocation, std::string endLocation) {

    //Check if starting and ending locations are valid
    if(linkToId.count(startLocation) == 0 || linkToId.count(endLocation) == 0){
        std::cout<<"Starting or ending location is invalid"<<std::endl;
        return std::vector<string>();
    }

    //Check if starting and ending location are the same
    if(startLocation == endLocation){
        std::cout<<"The shortest path between "<<startLocation<< " and " <<endLocation<<" is 0 steps"<<std::endl;
        return std::vector<string>();
    }
    
    //Set up queue, ID, and vectors to prepare for BFS operation
    std::vector<string> shortestPath;
    std::queue<int>bfsQueue;
    int startId = linkToId[startLocation];
    int endId = linkToId[endLocation];
    
    bfsQueue.push(startId);
    int dist = 0;
    bool foundPath = false;
    bool visited[idToLink.size()];
    int distances[idToLink.size()];
    int prev[idToLink.size()];

    //Set up initial values for vectors
    for(int i = 0; i < idToLink.size(); i++){
        visited[i] = false;
        distances[i] = 100000;
        prev[i] = -1;
    }

    visited[startId] = true;
    distances[startId] = 0;

    //Perform BFS
    while(!foundPath && !bfsQueue.empty()){
        int degreeSize = bfsQueue.size();
        for(int j = 0; j < degreeSize; j++){
            int currentId = bfsQueue.front();
            bfsQueue.pop();
            std::vector<int>linksToOtherPages = directedAdjacencyList[currentId];
            for(int k = 0; k < linksToOtherPages.size(); k++){
                int adjacentId = linksToOtherPages[k];
                if(!visited[adjacentId]){
                    bfsQueue.push(adjacentId); //Push neighboring ID into BFS stack
                    visited[adjacentId] = true; //Mark ID as visited
                    distances[adjacentId] = distances[currentId]+1;
                    prev[adjacentId] = currentId; //The key is  to maintain the path in the BFS operation so we can print it out later
                    if(adjacentId == endId){ //If the ending ID has been found, we end the BFS
                        foundPath = true;
                    }
                }
            }
        }
        dist++;
    }
    if(!foundPath){ //If no paths were found and the queue was emptied, that means we couldn't find a valid path.
        std::cout<<"No paths were found between "<<startLocation<< " and " <<endLocation<<std::endl;
        return std::vector<string>();
    }
    else{ //Otherwise perform backtracking with the prev vector to obtain the path we took
        int backtrack = endId;
        shortestPath.push_back(idToLink[endId]);
        while(prev[backtrack] != -1){
            shortestPath.push_back(idToLink[prev[backtrack]]);
            backtrack = prev[backtrack];
        }
        //Print statements and return shortest path
        std::cout<<"The shortest path between "<<startLocation<< " and " <<endLocation<<" is " << dist<< " steps"<<std::endl;
        std::reverse(shortestPath.begin(), shortestPath.end());
        return shortestPath;
    }
}

// finds the path which uses the most well-known topics (correlation: the pages/links which have more links)
// this does not guarantee shortest path, but avoids obscure topics players may not know about (and therefore will not click on)
std::vector<std::string> TheWikiGame::djikstra(std::string startLocation, std::string endLocation) {
    std::unordered_map<int, std::vector<int>> directedAdjacencyList;
    directedAdjacencyList[0].push_back(2);
    directedAdjacencyList[0].push_back(1);
    directedAdjacencyList[1].push_back(2);
    directedAdjacencyList[1].push_back(3);
    directedAdjacencyList[1].push_back(4);
    directedAdjacencyList[2].push_back(1);
    directedAdjacencyList[3].push_back(1);
    directedAdjacencyList[4].push_back(1);
    std::unordered_map<std::string, int>linkToId;
    linkToId["Andrew Jackson"] = 0;
    linkToId["John Wu"] = 1;
    linkToId["Abraham Lincoln"] = 2;
    linkToId["A"] = 3;
    linkToId["B"] = 4;
    std::unordered_map<int, std::string>idToLink;
    idToLink[0] = "Andrew Jackson";
    idToLink[1] = "John Wu";
    idToLink[2] = "Abraham Lincoln";
    idToLink[3] = "A";
    idToLink[4] = "B";

    int size = int(directedAdjacencyList.size());
    int bestDist[size];
    bool visited[size];
    int weights[size];
    int prev[size];
    for (int i = 0; i < size; i++) {
        bestDist[i] = 999999999;
        visited[i] = false;
        weights[i] = -1 * int(directedAdjacencyList[i].size());
        prev[i] = -1;
    }
    bestDist[linkToId[startLocation]] = 0;

    // for (int i = 0; i < directedAdjacencyList.size(); i++) {
    //     for (int s : directedAdjacencyList[i]) {
    //         std::cout << s << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    std::set<std::pair<int, int>> links;
    links.insert(std::pair<int, int> (0, linkToId[startLocation])); // sets of pairs sort by first element (distance); second element is link id
    std::pair<int, int> curr;
    while (!links.empty()) {
        curr = links.extract(links.begin()).value();
        visited[curr.second] = true;

        // std::cout << "extracted (" << curr.first << ", " << curr.second << ")" << std::endl;
        for (int i : directedAdjacencyList[curr.second]) {
            // std::cout << i << std::endl;
            // std::cout << "second: " << bestDist[i] << " > " << bestDist[curr.second] << " + " << weights[i] << std::endl;
            if ((!visited[i]) && (bestDist[i] > bestDist[curr.second] + weights[i])) {
                bestDist[i] = bestDist[curr.second] + weights[i];
                prev[i] = curr.second;
                // std::cout << "i: " << i << ", curr.second: " << curr.second << std::endl;
                links.insert(std::pair<int, int> (bestDist[i], i));
            }
        }
    }

    std::vector<string> ret;
    int id = linkToId[endLocation];
    while (id != linkToId[startLocation]) {
        ret.insert(ret.begin(), idToLink[id]);
        id = prev[id];
        if (id == -1) {
            std::vector<string> temp = std::vector<string>();
            temp.push_back("no path found.");
            return temp;
        }
    }
    ret.insert(ret.begin(), idToLink[id]);

    std::cout << "output: " << std::endl;
    for (string s : ret) {
        std::cout << s << std::endl;
    }
    return ret;
}

bool TheWikiGame::cmp(pair<int, double>& a, pair<int, double>& b){
    return (a.second > b.second);
}

vector<pair<int, double>> TheWikiGame::pageRank(double dampingFactor, int depth, int topN){
    for(unsigned i = 0; i < depth; i++){
        std::unordered_map<int, double> updatedPageRank = idToPageRank; //Create an new map to hold our updated PageRank scores
        //Set all scores to 0 initially for the updated page rank.
        for(auto& it : updatedPageRank){
            it.second = 0.0;
        }
        for(auto& it : directedAdjacencyList){
            int currentId = it.first; //Find current "page"
            std::vector<int> linksToOtherPages = it.second; //List of adjacent pages within the class
            for(unsigned j = 0; j < linksToOtherPages.size(); j++){
                int adjacentId = linksToOtherPages[j];
                //Perform page rank algorithm based on current scores and adjacent nodes by distributing evenly amongst neighboring pages
                updatedPageRank[adjacentId] += idToPageRank[currentId]/linksToOtherPages.size(); 
            }
        }
        //Update the PageRank with damping factor accounted for
       for (auto& [id, _] : directedAdjacencyList){
            idToPageRank[id] = (1 - dampingFactor)/idToPageRank.size() + dampingFactor*updatedPageRank[id];
       }
    }

    vector<pair<int, double>> tempSort;

    //Processing so we return pages in order of ranking
    for(auto& it : idToPageRank){
        tempSort.push_back(it);
    }
    sort(tempSort.begin(), tempSort.end(), cmp);

    //Prints top N pages
    std::cout<<"The top "<< topN << " pages are:" << std::endl;
    for(int j = 0; j < topN; j++){
        int topId = tempSort[j].first;
        std::string topLink = idToLink[topId];
        std::cout << topLink << " " << idToPageRank[topId] << std::endl;
    }
    //Return the sorted pages based on their PageRank score
    return tempSort;
}

void TheWikiGame::resetPageRank(){
    //Resets PageRank scores
    for(int i = 1; i <= idToPageRank.size(); i++){
        idToPageRank[i] = 1/(idToPageRank.size());
    }
}
