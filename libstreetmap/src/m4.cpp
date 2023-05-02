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
#include "m4.h"
#include "helper.h"
#include <chrono> // Time utilities 
#include<cstdlib>
#include<ctime>

using namespace std;

#define INFINITE 99999999;
#define TIME_LIMIT 50 // m4: 50 second time limit

//extern std::vector<std::vector<travel_info>> timeMatrix;
vector<IntersectionIdx> all_pickdrop_location;
vector<IntersectionIdx> only_pickUp_location;


std::vector<CourierSubPath> travelingCourier(
                            const float turn_penalty,
                            const std::vector<DeliveryInf>& deliveries,
                            const std::vector<IntersectionIdx>& depots){
    
    //Time limit control
    auto startTime = std::chrono::high_resolution_clock::now();
    bool timeOut = false;
    //Could use a while loop but not necessary
    //while(!timeOut){}
    srand(time(0));
    
    
    CourierSubPath OneSubpath;
    vector<CourierSubPath> wholePath;
    vector<CourierSubPath> keptPath;
    
    LatLon start;
    LatLon end;
    double shortest_distance = INFINITE;//initialize it to be very big
    double second_shortest_distance = INFINITE;
    double total_timedistance =  0;
    double shortest_total_timedistance =  INFINITE;
    
    //int depot_id = -1;
    int current_delivery_id = -1;
    int next_delivery_id = -1;
    int second_next_delivery_id = -1;
    
    int next_pick_or_drop = -1;
    int last_pick_or_drop = -1;
    
    //initialize mark for if a position has been visited
    vector<int> travelled_mark;
    const int notVisited=0, picked = 1, dropped = 2;
    travelled_mark.resize(deliveries.size());
    for (int i = 0; i < deliveries.size(); i++){
        travelled_mark[i]=0;
    }
    
    //create all locations
    all_pickdrop_location.resize(2 * deliveries.size() + depots.size());
    for(int i=0; i<deliveries.size(); i++){
        all_pickdrop_location[i] = deliveries[i].pickUp;
        all_pickdrop_location[i+deliveries.size()] = deliveries[i].dropOff;   
    }
    for(int i=0; i<depots.size(); i++){
        all_pickdrop_location[i + 2 * deliveries.size()] = depots[i];
    }
    //pre-load all calculated time
    //version3 - same pick or drop improvement
//    timeMatrix.resize(2 * deliveries.size());
//    bool same_pick = false;
//    bool same_drop = false;
//    for(int i=0; i<deliveries.size(); i++){
//        for(int j=0;j<i;j++){
//            if(deliveries[i].pickUp==deliveries[j].pickUp){
//                timeMatrix[i] = timeMatrix[j];
//                same_pick = true;
//            }
//            if(deliveries[i].pickUp==deliveries[j].dropOff){
//                timeMatrix[i] = timeMatrix[j+deliveries.size()];
//                same_pick = true;
//            }
//            if(deliveries[i].dropOff==deliveries[j].pickUp){
//                timeMatrix[i+deliveries.size()] = timeMatrix[j];
//                same_drop = true;
//            }
//            if(deliveries[i].dropOff==deliveries[j].dropOff){
//                timeMatrix[i+deliveries.size()] = timeMatrix[j+deliveries.size()];
//                same_drop = true;
//            }
//            
//            if(same_pick == false){
//                timeMatrix[i] = multidestDijkstra(turn_penalty, {deliveries[i].pickUp, all_pickdrop_location});
//            }
//            if(same_drop == false){
//                timeMatrix[i+deliveries.size()] = multidestDijkstra(turn_penalty, {deliveries[i].dropOff, all_pickdrop_location});
//            }
//            same_pick = false;
//            same_drop = false;
//        }
//    }
//version2 -- void
//    for(int i=0; i<deliveries.size(); i++){
//        multidestDijkstra(turn_penalty, {deliveries[i].pickUp, all_pickdrop_location});
//    }
//    for(int i=0; i<deliveries.size(); i++){
//        multidestDijkstra(turn_penalty, {deliveries[i].dropOff, all_pickdrop_location});
//    }
    timeMatrix.resize(2 * deliveries.size());
    for(int i=0; i<deliveries.size(); i++){
        timeMatrix[i] = multidestDijkstra(turn_penalty, {deliveries[i].pickUp, all_pickdrop_location});
        timeMatrix[i+deliveries.size()] = multidestDijkstra(turn_penalty, {deliveries[i].dropOff, all_pickdrop_location});
    }
    
    //preload depots to all pickUps time
    only_pickUp_location.resize(deliveries.size());
    for(int i=0; i<deliveries.size(); i++){
        only_pickUp_location[i] = deliveries[i].pickUp;
    }
    
    vector<vector<travel_info>> DepotStartTime;
    DepotStartTime.resize(depots.size());
    for(int i=0; i<depots.size(); i++){
        DepotStartTime[i] = multidestDijkstra(turn_penalty, {depots[i], only_pickUp_location});
    }
    

    //Use a legal(not isolated depot) to start with
    for (int i = 0; i < depots.size(); i++){
        //make sure depot can reach at least one delivery
    
for(int tenpercentIter = 0; tenpercentIter<10; tenpercentIter++){
        //find depot to 1st delivery
        for(int j=0; j < deliveries.size(); j++){
            double record_distance = DepotStartTime[i][j].travel_time;
            if(record_distance < shortest_distance){
                shortest_distance = record_distance;
                next_delivery_id = j;
            }
        }
        if(shortest_distance == INF){
           continue;
        }
        
        OneSubpath.subpath = DepotStartTime[i][next_delivery_id].travel_path;
        OneSubpath.start_intersection = depots[i];
        OneSubpath.end_intersection = deliveries[next_delivery_id].pickUp;
        total_timedistance += DepotStartTime[i][next_delivery_id].travel_time;
        
        current_delivery_id = next_delivery_id;
        travelled_mark[current_delivery_id] = picked;
        last_pick_or_drop = picked;
        shortest_distance = INFINITE;
        
        wholePath.push_back(OneSubpath);
    
        //break;

                
        //Find closest delivery
        bool all_deliveries_done = false;
        while(!all_deliveries_done){

            //connect the current location to the closet legal point
            if(last_pick_or_drop==dropped){
                current_delivery_id = current_delivery_id + deliveries.size();
            }
            
            
            for (int i = 0; i < deliveries.size(); i++){
                if(travelled_mark[i]==notVisited){
                    double record_distance = timeMatrix[current_delivery_id][i].travel_time;
                    /*if (i==0){                   
                        second_shortest_distance = record_distance;
                        second_next_delivery_id = 0;
                        next_pick_or_drop = picked;
                    }
                    next_pick_or_drop = picked;*/
                    if(record_distance < shortest_distance){

                        second_shortest_distance = shortest_distance;
                        second_next_delivery_id = next_delivery_id;

                        shortest_distance = record_distance;
                        next_delivery_id = i;
                        next_pick_or_drop = picked;
                    }
                    /*else if(record_distance < second_shortest_distance){

                        second_shortest_distance = record_distance;
                        second_next_delivery_id = 0;
          
                        next_delivery_id = i;
                        next_pick_or_drop = picked;
                    }*/
                    
                }
                else if(travelled_mark[i]==picked){
                    double record_distance = timeMatrix[current_delivery_id][i+deliveries.size()].travel_time;
                    second_shortest_distance = record_distance;
                    /*if (i==0){
                        
                        second_shortest_distance = record_distance;
                        second_next_delivery_id = 0;
                        next_pick_or_drop = dropped;
                    }*/
                    if(record_distance < shortest_distance){

                        second_shortest_distance = shortest_distance;
                        second_next_delivery_id = next_delivery_id;

                        shortest_distance = record_distance;
                        next_delivery_id = i;
                        next_pick_or_drop = dropped;
                    }
                    /*else if(record_distance < second_shortest_distance){

                        second_shortest_distance = record_distance;
                        second_next_delivery_id = 0;
          
                        next_delivery_id = i;
                        next_pick_or_drop = dropped;
                    }*/
                }
            }
            //Find the next place to go. Now update the information.
            int TenPercentChance = rand()%10;
            if(TenPercentChance == 1 && tenpercentIter != 0){
                
                if(second_shortest_distance != INF){
                    next_delivery_id = second_next_delivery_id;
                }
                if(next_pick_or_drop == picked){

                    if(last_pick_or_drop==picked){
                        OneSubpath.subpath = timeMatrix[current_delivery_id][next_delivery_id].travel_path;
                        OneSubpath.start_intersection = deliveries[current_delivery_id].pickUp;
                    }
                    else if(last_pick_or_drop==dropped){
                        OneSubpath.subpath = timeMatrix[current_delivery_id][next_delivery_id].travel_path;
                        OneSubpath.start_intersection = deliveries[current_delivery_id-deliveries.size()].dropOff;
                    }
                    OneSubpath.end_intersection = deliveries[next_delivery_id].pickUp;
                    total_timedistance += timeMatrix[current_delivery_id][next_delivery_id].travel_time;

                    shortest_distance = INFINITE;
                    second_shortest_distance = INFINITE;
                    current_delivery_id = next_delivery_id;
                    travelled_mark[current_delivery_id] = picked;
                    last_pick_or_drop = picked;
                    wholePath.push_back(OneSubpath);
                    
                    //pick/drop all 
                    for (int i=0; i<deliveries.size(); i++){
                        if(travelled_mark[i] == notVisited){
                            if(deliveries[i].pickUp==deliveries[current_delivery_id].pickUp){
                                travelled_mark[i] = picked;
                            }
                        }
                        if(travelled_mark[i] == picked){
                            if(deliveries[i].dropOff==deliveries[current_delivery_id].pickUp){
                                travelled_mark[i] = dropped;
                            }
                        }
                    }

                }
                else if(next_pick_or_drop == dropped){

                    if(last_pick_or_drop==picked){
                        OneSubpath.subpath = timeMatrix[current_delivery_id][next_delivery_id+deliveries.size()].travel_path;
                        OneSubpath.start_intersection = deliveries[current_delivery_id].pickUp;
                    }
                    else if(last_pick_or_drop==dropped){
                        OneSubpath.subpath = timeMatrix[current_delivery_id][next_delivery_id+deliveries.size()].travel_path;
                        OneSubpath.start_intersection = deliveries[current_delivery_id-deliveries.size()].dropOff;
                    }
                    OneSubpath.end_intersection = deliveries[next_delivery_id].dropOff;
                    total_timedistance += timeMatrix[current_delivery_id][next_delivery_id + deliveries.size()].travel_time;

                    shortest_distance = INFINITE;
                    second_shortest_distance = INFINITE;
                    current_delivery_id = next_delivery_id;
                    travelled_mark[current_delivery_id] = dropped;
                    last_pick_or_drop = dropped;
                    wholePath.push_back(OneSubpath);
                    
                    //pick/drop all
                    for (int i=0; i<deliveries.size(); i++){
                        if(travelled_mark[i] == picked){
                            if(deliveries[i].dropOff==deliveries[current_delivery_id].dropOff){
                                travelled_mark[i] = dropped;
                            }
                        }
                    }

                }
            }
            else{//TenPercentChance != 0, regular shortest path
                if(next_pick_or_drop == picked){

                    if(last_pick_or_drop==picked){
                        OneSubpath.subpath = timeMatrix[current_delivery_id][next_delivery_id].travel_path;
                        OneSubpath.start_intersection = deliveries[current_delivery_id].pickUp;
                    }
                    else if(last_pick_or_drop==dropped){
                        OneSubpath.subpath = timeMatrix[current_delivery_id][next_delivery_id].travel_path;
                        OneSubpath.start_intersection = deliveries[current_delivery_id-deliveries.size()].dropOff;
                    }
                    OneSubpath.end_intersection = deliveries[next_delivery_id].pickUp;
                    total_timedistance += timeMatrix[current_delivery_id][next_delivery_id].travel_time;

                    shortest_distance = INFINITE;
                    second_shortest_distance = INFINITE;
                    current_delivery_id = next_delivery_id;
                    travelled_mark[current_delivery_id] = picked;
                    last_pick_or_drop = picked;
                    wholePath.push_back(OneSubpath);

                    //pick/drop all 
                    for (int i=0; i<deliveries.size(); i++){
                        if(travelled_mark[i] == notVisited){
                            if(deliveries[i].pickUp==deliveries[current_delivery_id].pickUp){
                                travelled_mark[i] = picked;
                            }
                        }
                        if(travelled_mark[i] == picked){
                            if(deliveries[i].dropOff==deliveries[current_delivery_id].pickUp){
                                travelled_mark[i] = dropped;
                            }
                        }
                    }

                }
                else if(next_pick_or_drop == dropped){
                    
                    if(last_pick_or_drop==picked){
                        OneSubpath.subpath = timeMatrix[current_delivery_id][next_delivery_id+deliveries.size()].travel_path;
                        OneSubpath.start_intersection = deliveries[current_delivery_id].pickUp;
                    }
                    else if(last_pick_or_drop==dropped){
                        OneSubpath.subpath = timeMatrix[current_delivery_id][next_delivery_id+deliveries.size()].travel_path;
                        OneSubpath.start_intersection = deliveries[current_delivery_id-deliveries.size()].dropOff;
                    }
                    OneSubpath.end_intersection = deliveries[next_delivery_id].dropOff;
                    total_timedistance += timeMatrix[current_delivery_id][next_delivery_id + deliveries.size()].travel_time;

                    shortest_distance = INFINITE;
                    second_shortest_distance = INFINITE;
                    current_delivery_id = next_delivery_id;
                    travelled_mark[current_delivery_id] = dropped;
                    last_pick_or_drop = dropped;
                    wholePath.push_back(OneSubpath);

                    //pick/drop all 
                    for (int i=0; i<deliveries.size(); i++){
                        if(travelled_mark[i] == picked){
                            if(deliveries[i].dropOff==deliveries[current_delivery_id].dropOff){
                                travelled_mark[i] = dropped;
                            }
                        }
                    }

                }
            }

            
            
            
            //check if all deliveries are completed - if true break the while loop
            int count_dropped = 0;
            for (int i = 0; i < travelled_mark.size(); i++){
                if(travelled_mark[i]==2){
                    count_dropped++;
                }
            }
            if(count_dropped==travelled_mark.size()){
                all_deliveries_done = true;
            }

        }


        //Go to the closest depot and end
        for (int i = 0; i < depots.size(); i++){
            double record_distance = timeMatrix[current_delivery_id][i+ 2 * deliveries.size()].travel_time;
                if(record_distance < shortest_distance){
                    shortest_distance = record_distance;
                    next_delivery_id = i;
                }
        }


        OneSubpath.subpath = timeMatrix[current_delivery_id + deliveries.size()][next_delivery_id + 2 * deliveries.size()].travel_path;
        total_timedistance += timeMatrix[current_delivery_id][next_delivery_id + 2 * deliveries.size()].travel_time;
        OneSubpath.start_intersection = deliveries[current_delivery_id].dropOff;
        OneSubpath.end_intersection = depots[next_delivery_id];
        wholePath.push_back(OneSubpath);


        if(total_timedistance < shortest_total_timedistance){
            //clear keptPath before take new value
            for(int i=0; i<keptPath.size(); i++){
                keptPath[i].subpath.clear();
            }
            keptPath.clear();

            keptPath = wholePath;

            shortest_total_timedistance = total_timedistance;
        }





        //////Global variables reset//////
        //CourierSubPath OneSubpath;
        OneSubpath.subpath.clear();
        //vector<CourierSubPath> wholePath;
        for(int i=0; i<wholePath.size(); i++){
            wholePath[i].subpath.clear();
        }
        wholePath.clear();


        shortest_distance = INFINITE;//initialize it to be very big
        total_timedistance =  0;

        current_delivery_id = -1;
        next_delivery_id = -1;

        next_pick_or_drop = -1;
        last_pick_or_drop = -1;

        ////initialize mark for if a position has been visited
        //vector<int> travelled_mark;
        for (int i = 0; i < deliveries.size(); i++){
            travelled_mark[i]=0;
        }
    
/*TIME LIMIT TEST*/ 
auto currentTime = chrono::high_resolution_clock::now();
auto wallclock = chrono::duration_cast<chrono::duration<double>> (currentTime - startTime);
//Keep optimizing until within 10% of time limit
if (wallclock.count() > 0.9 * TIME_LIMIT) timeOut = true;
//}/*TIME LIMIT TEST*/ 

if(timeOut == true) break;
    
}

if(timeOut == true) break;

    }
    

    return keptPath;
}