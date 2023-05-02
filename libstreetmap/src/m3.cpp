#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include <unordered_map>
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <map>
#include "m3.h"
#include "m1.h"
#include "m2.h"
#include "helper.h"

using namespace std;

extern std::vector <Node> nodes;


// Returns the time required to travel along the path specified, in seconds.
 // The path is given as a vector of street segment ids, and this function can
 // assume the vector either forms a legal path or has size == 0. The travel
 // time is the sum of the length/speed-limit of each street segment, plus the
 // given turn_penalty (in seconds) per turn implied by the path. If there is
 // no turn, then there is no penalty. Note that whenever the street id changes
 // (e.g. going from Bloor Street West to Bloor Street East) we have a turn.
double computePathTravelTime(const double turn_penalty, const std::vector<StreetSegmentIdx>& path){
    double travel_time = 0;
    if (path.size() == 0){
        return travel_time;
    }
    //compute total travel time by adding up travel time for each segment; if street id changes, add a constant penalty time
    for (int i = 0; i < path.size(); i++){              
        travel_time += findStreetSegmentTravelTime(path[i]);
        if ((i < path.size() - 1)){
            if (getStreetSegmentInfo(path[i]).streetID != getStreetSegmentInfo(path[i+1]).streetID){
                travel_time += turn_penalty;   
            }
        }  
    }
    return travel_time;
}

 // Returns a path (route) between the start intersection (1st object in intersect_ids pair)
 // and the destination intersection(2nd object in intersect_ids pair),
 // if one exists. This routine should return the shortest path
 // between the given intersections, where the time penalty to turn right or
 // left is given by turn_penalty (in seconds). If no path exists, this routine
 // returns an empty (size == 0) vector. If more than one path exists, the path
 // with the shortest travel time is returned. The path is returned as a vector
 // of street segment ids; traversing these street segments, in the returned
 // order, would take one from the start to the destination intersection.
std::vector<StreetSegmentIdx> findPathBetweenIntersections(const double turn_penalty, 
                              const std::pair<IntersectionIdx, IntersectionIdx> intersect_ids){
    std::vector<StreetSegmentIdx> path;

    IntersectionIdx from = intersect_ids.first;
    IntersectionIdx to = intersect_ids.second;
    
    Node nodeFrom = nodes[from];
    Node nodeto = nodes[to];
    //return empty path if intersection pair constructed by the same intersection
    if (from == to){
        return path;
    }
    
    //reset the node flag every time call this function
    for (int i = 0; i < nodes.size(); i++){
        nodes[i].best_time = INF;
        nodes[i].reachingEdge = NO_EDGE;
        nodes[i].visit = false;
    }
    
    std::priority_queue <WaveElem, std::vector<WaveElem>, compare> WaveFront;
    //set the from point travel point to 0
    nodes[from].best_time = 0;

    //push the information of first(from) node into wavefront
    for (int segIdx = 0; segIdx < nodeFrom.connecting_edges.size(); segIdx++){     
        IntersectionIdx next_nodeID = findNextNode(from, nodeFrom.connecting_edges[segIdx]);
        double total_time = findStreetSegmentTravelTime(nodeFrom.connecting_edges[segIdx]);
        WaveFront.push(WaveElem(next_nodeID, nodeFrom.connecting_edges[segIdx], total_time));
    }
    nodes[from].visit = true;
    
    //loop is wavefront has some wave elements
    while (!WaveFront.empty()){
        //extract first node with smallest travel time, and remove it from wavefront
        WaveElem front_elem = WaveFront.top();
        WaveFront.pop();
        //skip if node is visted
        if (nodes[front_elem.nodeID].visit){
            continue;
        }
        //mark the node as visited in order to skip at next time reaching it
        nodes[front_elem.nodeID].visit = true;
        //do relation, update the travel time for each reaching node
        if (front_elem.travel_time < nodes[front_elem.nodeID].best_time){     
            nodes[front_elem.nodeID].best_time = front_elem.travel_time;
            nodes[front_elem.nodeID].reachingEdge  = front_elem.edgeID;    
            //return path if wave element reach the destination, trace back all segments and add to path
            if (front_elem.nodeID == to){
                return path = bfsTraceBack(to);
            }
            //if not yet reach the destination, update waveFront by adding new wave elements and update its travel time
            for (StreetSegmentIdx segIdx : nodes[front_elem.nodeID].connecting_edges){
                double total_time = 0;
                IntersectionIdx next_nodeID = findNextNode(front_elem.nodeID, segIdx);
                
                StreetIdx curr_strIdx = getStreetSegmentInfo(front_elem.edgeID).streetID; 
                StreetIdx next_strIdx = getStreetSegmentInfo(segIdx).streetID;
                //update travel time for each wave element
                total_time = total_time + nodes[front_elem.nodeID].best_time + findStreetSegmentTravelTime(segIdx);
                if (curr_strIdx != next_strIdx){
                    total_time +=  turn_penalty;
                }
                //if node not yet visited, adding wave element into the wavefront 
                if (nodes[next_nodeID].visit == false){        
                    WaveElem new_elem = WaveElem(next_nodeID, segIdx, total_time);
                    WaveFront.push(new_elem);
                } 
            }
        }        
    }        
    return path;
}




