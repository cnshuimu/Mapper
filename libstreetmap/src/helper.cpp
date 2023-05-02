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
#define inf 99999999
#define NO_EDGE -1  // Illegal edge ID à 

std::vector<Node> nodes;
std::vector<std::vector<travel_info>> timeMatrix;
std::vector<std::vector<travel_info>> timeMatrix_depots;


//load all nodes information and store in nodes vector
void load_all_nodes(){
    nodes.resize(getNumIntersections());
    for (int inter_idx = 0; inter_idx <getNumIntersections(); inter_idx++){
        Node n;
        n.nodeID = inter_idx;
        //load all possible reaching edges for each node, only added edges if it can be traveled from a node
        for ( auto i : findStreetSegmentsOfIntersection(inter_idx)){
            if (getStreetSegmentInfo(i).oneWay){
                if (getStreetSegmentInfo(i).to == inter_idx)
                    continue;
            }
            n.connecting_edges.push_back(i);
        }
        nodes[inter_idx] = n;
    }

}

void clear_all_node(){
    for (int i = 0; i < nodes.size(); i++){
        nodes[i].connecting_edges.clear();
    }
    nodes.clear();
}

//finding the next node by passing the current node index and the reaching edge index
IntersectionIdx findNextNode(IntersectionIdx currNode, StreetSegmentIdx edge){
    if (getStreetSegmentInfo(edge).from == currNode){
        return getStreetSegmentInfo(edge).to;
    }
    else{
        return getStreetSegmentInfo(edge).from;
    }
    
}

//trace back all segments went through and add into the path
std::vector<StreetSegmentIdx> bfsTraceBack (IntersectionIdx destID) {
    std::vector<StreetSegmentIdx> path;
    int currNodeID = destID;
    //find the edges that is reaching from and add to path, use it to get previous node information.
    //continued looping until it reaches the from node
    StreetSegmentIdx prevEdge = nodes[currNodeID].reachingEdge;
    if (prevEdge == NO_EDGE){
        return path;
    }
    while (prevEdge != NO_EDGE) {
        path.push_back (prevEdge);
        
        if (currNodeID == getStreetSegmentInfo(prevEdge).from){
            currNodeID = getStreetSegmentInfo(prevEdge).to;
        }
        else{
            currNodeID = getStreetSegmentInfo(prevEdge).from;
        }       
        prevEdge = nodes[currNodeID].reachingEdge;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

/////////////////////////////////////////////////////////////////////////////////updates for m4

//vector<vector<travel_info>> travel_matrix;

//1: start  2, 4 ,5: end -> <12, 14, 15>travel_info (travel_time(respect to 1), travel_path(respect to 1));
vector<travel_info> multidestDijkstra(const double turn_penalty, 
                              const std::pair<IntersectionIdx, vector<IntersectionIdx>> intersect_ids){
    vector<travel_info> one_to_other;
    one_to_other.resize(intersect_ids.second.size());
    std::vector<StreetSegmentIdx> path;

    IntersectionIdx from = intersect_ids.first;
    vector<IntersectionIdx> to = intersect_ids.second;
    
    //bool all_visited = false;
    
    Node nodeFrom = nodes[from];
    //Node nodeto = nodes[to];
 
    
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
        int num_of_visited = 0;
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
            /*for (int i = 0; i < intersect_ids.second.size(); i++){
                if (nodes[intersect_ids.second[i]].best_time != INF){
                    num_of_visited += 1;
                }    
            }
            if (num_of_visited == intersect_ids.second.size()){
                break;
            } */
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
    for (int i = 0; i < intersect_ids.second.size(); i++){
        path = bfsTraceBack(to[i]);
        one_to_other[i].travel_path = path;
        /*if (path.size() < 1 && to[i] != from){
            one_to_other[i].travel_time = INF;
        }
        else{
            one_to_other[i].travel_time = nodes[to[i]].best_time;
        }      */
        one_to_other[i].travel_time = nodes[to[i]].best_time;
    }
    
    
    
    return one_to_other;
}


/////////////////////////////////////////////////////////////////////////////////up to above

