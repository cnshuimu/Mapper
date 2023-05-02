/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   helper.h
 * Author: wangc471
 *
 * Created on March 31, 2022, 1:39 AM
 */

#ifndef HELPER_H
#define HELPER_H
#endif /* HELPER_H */

#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include <unordered_map>
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <unordered_set>


#define INF 99999999
#define NO_EDGE -1  // Illegal edge ID à 

////declare all structs
//wave element struct, used to locate corresponding node
struct WaveElem { 
    IntersectionIdx nodeID;
    StreetSegmentIdx edgeID;  // ID of edge used to reach nodeID
    double travel_time;
    WaveElem (int n, int e, double time) {nodeID = n; edgeID = e; travel_time = time;}
};

//node struct, store detailed information
struct Node {
    IntersectionIdx nodeID = 0;
    StreetSegmentIdx reachingEdge = NO_EDGE; 
    std::vector<StreetSegmentIdx> connecting_edges;
    double best_time = INF;
    bool visit = false;
};

//compare struct, used in priority queue to sort in the order of smallest travel time at top 
struct compare{
    bool operator()(const WaveElem& lhs, const WaveElem& rhs){
        return lhs.travel_time > rhs.travel_time;
    }
};

/////////////////////////////////////////////////////////////////////////////////updates for m4
struct travel_info{
    double travel_time;
    std::vector<StreetSegmentIdx> travel_path;
};
/////////////////////////////////////////////////////////////////////////////////up to above


////declare all global vectors
extern std::vector<Node> nodes;
extern std::vector<std::vector<travel_info>> timeMatrix;
extern std::vector<std::vector<travel_info>> timeMatrix_depots;

////declare all functions
void load_all_nodes();
void clear_all_node();
IntersectionIdx findNextNode(IntersectionIdx currNode, StreetSegmentIdx edge);
std::vector<StreetSegmentIdx> bfsTraceBack (int destID);


std::vector<travel_info> multidestDijkstra(const double turn_penalty, 
                              const std::pair<IntersectionIdx, std::vector<IntersectionIdx>> intersect_ids);

