/* 
 * Copyright 2022 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <iostream>
#include "m1.h"
#include <math.h>
#include <vector>
#include <unordered_map>
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <map>
#include "helper.h"

std::vector<StreetIdx> street_bin;
std::vector<double> street_seg_travel_time;

std::vector<std::vector<StreetSegmentIdx>> segs_in_street;
std::vector<double> street_length;

std::unordered_map<OSMID, LatLon> OSM_node_map;

std::vector<std::vector<StreetSegmentIdx>> Intersection_StreetSegment_Information;
std::vector<std::vector<std::string>> Intersection_StreetName_Information;

std::vector<StreetSegmentIdx> get_One_Intersection_StreetSegment_Information(IntersectionIdx intersection_id);
std::vector<std::string> get_One_Intersection_StreetName_Information(IntersectionIdx intersection_id);

std::vector<std::string> AllStreetNames;
std::vector<std::vector<IntersectionIdx>> street_intersections;
std::multimap <std::string, StreetIdx> Street_Name;

void create_street_bin();
void find_segment_travel_time();

void clear_street_bin();
void clear_segment_travel_time();

void create_segs_in_street();
void clear_segs_in_street();

void find_street_length();
void clear_street_length();

void OSM_node_unordered_map();
void clear_OSM_node_map();

void get_All_Intersection_StreetSegment_Information();
void get_All_Intersection_StreetName_Information();

void get_AllStreetNames();
//void clear_AllStreetNames();///////////////////////////////////////////

void create_street_intersections();
void clear_street_intersections();

void trim(std:: string &s);


void load_street_names();
void clear_street_names();




/////////////////////////////////////////////m3
/*#define inf 99999999
#define NO_EDGE -1  // Illegal edge ID à 

void load_all_nodes();

struct WaveElem{ 
    IntersectionIdx nodeID;
    StreetSegmentIdx edgeID;  // ID of edge used to reach nodeID
    double travel_time;
    WaveElem (int n, int e, double time) {nodeID = n; edgeID = e; travel_time = time;}
};


struct Node{
// Maybe store outgoing edges, ...
// ID of the edge used to reach this node
    IntersectionIdx nodeID = 0;
    StreetSegmentIdx reachingEdge = NO_EDGE; 
    std::vector<StreetSegmentIdx> connecting_edges;
    double best_time;
    bool visit = false;
};

std::vector <Node> nodes;


void load_all_nodes(){
    nodes.resize(getNumIntersections());
    for (int inter_idx = 0; inter_idx <getNumIntersections(); inter_idx++){
        Node n;
        
        for ( auto i : findStreetSegmentsOfIntersection(inter_idx)){

            if (getStreetSegmentInfo(i).oneWay){
                if (getStreetSegmentInfo(i).to == inter_idx)
                    continue;
            }
            n.connecting_edges.push_back(i);
        }
        nodes[inter_idx] = n;
    }

}*/


    
// loadMap will be called with the name of the file that stores the "layer-2"
// map data accessed through StreetsDatabaseAPI: the street and intersection 
// data that is higher-level than the raw OSM data). 
// This file name will always end in ".streets.bin" and you 
// can call loadStreetsDatabaseBIN with this filename to initialize the
// layer 2 (StreetsDatabase) API.
// If you need data from the lower level, layer 1, API that provides raw OSM
// data (nodes, ways, etc.) you will also need to initialize the layer 1 
// OSMDatabaseAPI by calling loadOSMDatabaseBIN. That function needs the 
// name of the ".osm.bin" file that matches your map -- just change 
// ".streets" to ".osm" in the map_streets_database_filename to get the proper
// name.
bool loadMap(std::string map_streets_database_filename) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully

    std::cout << "loadMap: " << map_streets_database_filename << std::endl;

    //
    // Load your map related data structures here.
    //
    load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);

    //get index of target filename
    int street_name_index = map_streets_database_filename.find(".streets.bin");
    int osm_name_index = map_streets_database_filename.find(".osm.bin");
    
    bool load_street_successful = false;
    bool load_osm_successful = false;
    
    std::string filename = map_streets_database_filename;
    
    //justify is it a street file or osm file, and load them seperately
    if(street_name_index!=std::string::npos && osm_name_index == std::string::npos){
        filename.replace(street_name_index,filename.length(),".osm.bin");
        load_osm_successful = loadOSMDatabaseBIN(filename);
        load_street_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    }
    else if (street_name_index == std::string::npos && osm_name_index != std::string::npos){
        filename.replace(osm_name_index,filename.length(),".streets.bin");
        load_osm_successful = loadOSMDatabaseBIN(filename);
        load_street_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    }
    
    //only load_successful if both database loaded successfully
    if(load_street_successful && load_osm_successful)
        load_successful = true;
               
    
    if (load_successful != true){
        return load_successful;
    }
    
    create_street_bin();
    find_segment_travel_time();
    create_segs_in_street();
    find_street_length();
    OSM_node_unordered_map();
    create_street_intersections();
    get_AllStreetNames();
    
    get_All_Intersection_StreetSegment_Information();
    get_All_Intersection_StreetName_Information();
   
    //partial names
    load_street_names();
    
    load_all_nodes();
 
    load_successful = true; //Make sure this is updated to reflect whether
                            //loading the map succeeded or failed
std::cout << "success" << std::endl;
    return load_successful;
}

void closeMap() {
    //Clean-up your map related data structures here
    
    //clean own helper functions
    clear_street_bin();
    clear_segment_travel_time();
    clear_segs_in_street();
    clear_street_length();
    clear_OSM_node_map();
    clear_street_intersections();
    
    //clear_AllStreetNames();/////////////////////////
    //partial name
    clear_street_names();
    
    //clear nodes for path-finding 
    clear_all_node();

    //close database
    closeStreetDatabase();
    closeOSMDatabase();
}

void create_street_bin(){
    int street_num = getNumStreets();
    for (int i = 0; i < street_num; i++){
        street_bin.push_back(i);
    }
}
void clear_street_bin(){
    street_bin.clear();
}


void find_segment_travel_time(){
    int seg_num = getNumStreetSegments();
    
    for (int i = 0; i < seg_num; i++){
        double length = findStreetSegmentLength(i);
        StreetSegmentInfo seg_info = getStreetSegmentInfo(i);
        double time = length / seg_info.speedLimit;
        street_seg_travel_time.push_back(time);
    }
}
void clear_segment_travel_time(){
    street_seg_travel_time.clear();
}


void create_segs_in_street(){
    int street_num = getNumStreets();
    int seg_num = getNumStreetSegments();
    //create outer vectors contained streets
    for (int j = 0; j < street_num; j++){
       std::vector<StreetSegmentIdx> streets;
       segs_in_street.push_back(streets);
    }
    //create inner vectors, store segments to corresponding street vectors
    for (int i = 0; i < seg_num; i++){
        StreetSegmentInfo seg_info = getStreetSegmentInfo(i);
        int street_belongings = seg_info.streetID;
        segs_in_street[street_belongings].push_back(i); 
    }
}

void clear_segs_in_street(){
    for (int i = segs_in_street.size(); i > 0; i-- ){
        segs_in_street[i-1].clear();
    }
    segs_in_street.clear();
}

void find_street_length(){    
    //create vector contains length for each street
    for (int i = 0; i < segs_in_street.size(); i++){
        double length = 0;
        for (int j = 0; j < segs_in_street[i].size(); j++){
            length += findStreetSegmentLength(segs_in_street[i][j]);
        }
        street_length.push_back(length);
    }
}

void clear_street_length(){
    street_length.clear();
}

void OSM_node_unordered_map(){
    for (int i = 0; i < getNumberOfNodes(); i++){
        const OSMNode* o_node = getNodeByIndex(i);
        OSMID o_node_id = o_node->id();
        LatLon o_node_pos = getNodeCoords(o_node);
        //insert element to map with given node_id and its position
        OSM_node_map.emplace(o_node_id, o_node_pos);
    }
}

void clear_OSM_node_map(){
    OSM_node_map.clear();
}


void create_street_intersections(){
    for (int j = 0; j < getNumStreets(); j++){
        std::vector<IntersectionIdx> intersections_in_street;
        street_intersections.push_back(intersections_in_street);
    }
    
    for (int i = 0; i < getNumStreetSegments(); i++){
        StreetSegmentInfo seg_info = getStreetSegmentInfo(i);
        int street_belongings = seg_info.streetID;
        street_intersections[street_belongings].push_back(seg_info.from); 
        street_intersections[street_belongings].push_back(seg_info.to); 
    }
}

 
void clear_street_intersections(){
    for (int i = street_intersections.size(); i > 0; i-- ){
        street_intersections[i-1].clear();
    }
    street_intersections.clear();
}

std::vector<StreetSegmentIdx> get_One_Intersection_StreetSegment_Information(IntersectionIdx intersection_id){
    
    std::vector<StreetSegmentIdx> find_Intersection;
    
    int numOfIntersection = getNumIntersectionStreetSegment(intersection_id);
    for(int i=0;i<numOfIntersection;i++){
        StreetSegmentIdx id = getIntersectionStreetSegment(i, intersection_id);
        find_Intersection.push_back(id);
    }
    return find_Intersection;
}
void get_All_Intersection_StreetSegment_Information(){
    int totalIntersection = getNumIntersections();
    
    for(int i=0; i< totalIntersection;i++){
        //Intersection_StreetSegment_Information.resize(totalIntersection);//Caused error
        Intersection_StreetSegment_Information.push_back(get_One_Intersection_StreetSegment_Information(i));
    }
}


std::vector<std::string> get_One_Intersection_StreetName_Information(IntersectionIdx intersection_id){
    
    std::vector<std::string> find_Intersection;
    
    int numOfIntersection = getNumIntersectionStreetSegment(intersection_id);
    for(int i=0;i<numOfIntersection;i++){
        StreetSegmentIdx id = getIntersectionStreetSegment(i, intersection_id);
        int streetID = getStreetSegmentInfo(id).streetID;
        std::string streetName = getStreetName(streetID);
        find_Intersection.push_back(streetName);
    }
    return find_Intersection;
}
void get_All_Intersection_StreetName_Information(){
    int totalIntersection = getNumIntersections();
    
    for(int i=0; i< totalIntersection;i++){
        //Intersection_StreetSegment_Information.resize(totalIntersection);//Caused error
        Intersection_StreetName_Information.push_back(get_One_Intersection_StreetName_Information(i));
    }
}

void load_street_names(){
    
    int numStreet = getNumStreets();
    for(int i=0; i<numStreet; i++){
        std::string single_name = getStreetName(i);
        trim(single_name);
        std::transform(single_name.begin(), single_name.end(), single_name.begin(),:: tolower);
        Street_Name.insert(make_pair(single_name,i));
    }
}


void clear_street_names(){
    
    Street_Name.clear();
    
}



void trim(std:: string &s){
    int index =0;
    if (!s.empty()){
        while ((index = s.find(' ',index))!= std::string::npos){
            s.erase(index,1); 
        }
    }
}

void get_AllStreetNames(){
    
    int numStreet = getNumStreets();
    for(int i=0; i<numStreet; i++){
        std::string single_name = getStreetName(i);
        trim(single_name);
        //single_name.erase(std::remove(single_name.begin(), single_name.end(), ' '), single_name.end());
        std::transform(single_name.begin(), single_name.end(), single_name.begin(),:: tolower);
        
        AllStreetNames.push_back(single_name);
    }
}

/*
void clear_AllStreetNames(){
    AllStreetNames.clear();
}
*/

// Returns the distance between two (lattitude,longitude) coordinates in meters
// Speed Requirement --> moderate
double findDistanceBetweenTwoPoints(std::pair<LatLon, LatLon> points){
    double x1, y1, x2, y2, distance, lat_avg;
    
    lat_avg = (points.first.latitude()* kDegreeToRadian + points.second.latitude()* kDegreeToRadian)/2; 
    
    //calculate x,y coordinate respectively
    x1 = kEarthRadiusInMeters * points.first.longitude()* kDegreeToRadian * cos(lat_avg);
    y1 = kEarthRadiusInMeters * points.first.latitude()* kDegreeToRadian;
    x2 = kEarthRadiusInMeters * points.second.longitude()* kDegreeToRadian * cos(lat_avg);
    y2 = kEarthRadiusInMeters * points.second.latitude()* kDegreeToRadian;
    
    //compute distance
    distance = sqrt(pow((y2 - y1),2) + pow((x2 - x1),2));
    
    return distance;
    
}

// Returns the length of the given street segment in meters
// Speed Requirement --> moderate
double findStreetSegmentLength(StreetSegmentIdx street_segment_id){
    std::vector<LatLon> street_seg_points_pos;

    StreetSegmentInfo seg_info = getStreetSegmentInfo(street_segment_id);
    
    IntersectionIdx intersection_1 = seg_info.from;
    IntersectionIdx intersection_2 = seg_info.to;
    int num_curve_points = seg_info.numCurvePoints;
    
    //add start intersection , all curve points and end intersection in order to vector
    street_seg_points_pos.push_back(getIntersectionPosition(intersection_1));
    if (num_curve_points != 0){
        for (int i=0; i < num_curve_points; i++){
            street_seg_points_pos.push_back(getStreetSegmentCurvePoint(i, street_segment_id));
        }
    }
    street_seg_points_pos.push_back(getIntersectionPosition(intersection_2));
        
    double length = 0;
    
    //make adjacent LatLon in pair, calculate each segment and add to the length
    for (int j = 1; j < street_seg_points_pos.size(); j++){
        std::pair<LatLon, LatLon> points (street_seg_points_pos[j-1], street_seg_points_pos[j]);
        length += findDistanceBetweenTwoPoints(points);
    }
    
    return length;
}

// Returns the travel time to drive from one end of a street segment 
// to the other, in seconds, when driving at the speed limit
// Note: (time = distance/speed_limit)
// Speed Requirement --> high 
double findStreetSegmentTravelTime(StreetSegmentIdx street_segment_id){
    //return corresponding travel time
    return street_seg_travel_time[street_segment_id];
}

// Returns true if the two intersections are directly connected, meaning you can legally 
// drive from the first intersection to the second using only one streetSegment.
// Speed Requirement --> moderate 
bool intersectionsAreDirectlyConnected(std::pair<IntersectionIdx, IntersectionIdx> intersection_ids){
    int num_of_inter_1 =  getNumIntersectionStreetSegment(intersection_ids.first);

    //check if the road is one way or not
        for(int i=0;i<num_of_inter_1;i++){
            StreetSegmentIdx id = getIntersectionStreetSegment(i, intersection_ids.first);
            StreetSegmentInfo seg_info = getStreetSegmentInfo(id);
            IntersectionIdx intersection_1 = seg_info.from;
            IntersectionIdx intersection_2 = seg_info.to;
            //check if road is one way or not. 
            if(intersection_2==intersection_ids.second || intersection_1 ==intersection_ids.second){
                return true;
            }
            else if (intersection_1 == intersection_ids.second){
                if(seg_info.oneWay == false){
                    return true;
                }
                else{
                    return false;
                }
            }        
                
        }
     return false;
}

// Returns the geographically nearest intersection (i.e. as the crow flies) to the given position
// Speed Requirement --> none
IntersectionIdx findClosestIntersection(LatLon my_position){   
    double distance_min = 1000000000000000;
    IntersectionIdx closest_id = 0;
    for(int i = 0 ; i < getNumIntersections(); i++){
        
        LatLon Pos_of_Inter = getIntersectionPosition(i);
        std::pair<LatLon, LatLon> two_points (my_position, Pos_of_Inter);
        double distance = findDistanceBetweenTwoPoints(two_points);
        
        if(distance_min - distance > 0){
            distance_min = distance;
            closest_id = i;
        }
        else{
        
        }

    }
    return closest_id;
}

// Returns the street segments that connect to the given intersection 
// Speed Requirement --> high
std::vector<StreetSegmentIdx> findStreetSegmentsOfIntersection(IntersectionIdx intersection_id){//functionality good
    return Intersection_StreetSegment_Information[intersection_id];
}

// Returns the street names at the given intersection (includes duplicate 
// street names in the returned vector)
// Speed Requirement --> high 
std::vector<std::string> findStreetNamesOfIntersection(IntersectionIdx intersection_id){
    return Intersection_StreetName_Information[intersection_id];
}

// Returns all intersections along the a given street.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high
std::vector<IntersectionIdx> findIntersectionsOfStreet(StreetIdx street_id){
    
    std::sort(street_intersections[street_id].begin(), street_intersections[street_id].end()); 
    auto last = std::unique(street_intersections[street_id].begin(), street_intersections[street_id].end());
    street_intersections[street_id].erase(last, street_intersections[street_id].end());
    return street_intersections[street_id];
} 

// Return all intersection ids at which the two given streets intersect
// This function will typically return one intersection id for streets
// that intersect and a length 0 vector for streets that do not. For unusual 
// curved streets it is possible to have more than one intersection at which 
// two streets cross.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high
std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(std::pair<StreetIdx, StreetIdx> street_ids){
    std::vector<IntersectionIdx> intersection_street_1 = findIntersectionsOfStreet(street_ids.first);
    std::vector<IntersectionIdx> intersection_street_2 = findIntersectionsOfStreet(street_ids.second);
    
    std::vector<IntersectionIdx> common_intersections;
    
    for (int i = 0; i < intersection_street_1.size(); i++){
        for (int j = 0; j < intersection_street_2.size(); j++){
            if (intersection_street_1[i] == intersection_street_2[j]){
                common_intersections.push_back(intersection_street_1[i]);
            }
        }
    }
    return common_intersections;
}

// Returns all street ids corresponding to street names that start with the 
// given prefix 
// The function should be case-insensitive to the street prefix. 
// The function should ignore spaces.
//  For example, both "bloor " and "BloOrst" are prefixes to 
// "Bloor Street East".
// If no street names match the given prefix, this routine returns an empty 
// (length 0) vector.
// You can choose what to return if the street prefix passed in is an empty 
// (length 0) string, but your program must not crash if street_prefix is a 
// length 0 string.
// Speed Requirement --> high 



std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix){
    
    std::vector<StreetIdx> names_id;

    if(street_prefix.size() == 0){
        return names_id;
    }
    
    std::string transfered_name = street_prefix;
    trim(transfered_name);
    std::transform(transfered_name.begin(), transfered_name.end(), transfered_name.begin(),:: tolower);
    
    std::string upper_name=transfered_name;
    upper_name.erase(upper_name.length()-1);
    upper_name.push_back(transfered_name[transfered_name.length()-1]+1);
    
    auto upper_bound = Street_Name.lower_bound(upper_name);
    
    for(auto lower_bound = Street_Name.lower_bound(transfered_name);lower_bound != upper_bound; lower_bound++){
        names_id.push_back(lower_bound -> second);
    
    }
    

      return names_id;
}
    
    


/*std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix){
    
    std::vector<StreetIdx> names_id;
    //return names_id;
    if(street_prefix.size() == 0){
        return names_id;
    }

    std::string transfered_name = street_prefix;
    trim(transfered_name);
    //transfered_name.erase(std::remove(transfered_name.begin(), transfered_name.end(), ' '), transfered_name.end());
    std::transform(transfered_name.begin(), transfered_name.end(), transfered_name.begin(),:: tolower);
    for(int i = 0; i < getNumStreets();i ++){
        std::string first_n_letter = AllStreetNames[i].substr(0, transfered_name.length());
        if (first_n_letter == transfered_name){
            names_id.push_back(i);
        }
    }
    return names_id;
}*/

// Returns the length of a given street in meters
// Speed Requirement --> high 
double findStreetLength(StreetIdx street_id){
    return street_length[street_id];
}

// Returns the nearest point of interest of the given type (e.g. "restaurant") 
// to the given position
// Speed Requirement --> none 
POIIdx findClosestPOI(LatLon my_position, std::string POItype){
    double  distance_Min = 10000000000;
    double cur_Distance; 
    POIIdx  bestPos = 0;
    
    int totalNum_POI = getNumPointsOfInterest();
    for(int i = 0; i < totalNum_POI; i++){
        if(POItype == getPOIType(i)){
            LatLon position = getPOIPosition(i);
            std::pair<LatLon, LatLon> points (my_position, position);
            cur_Distance = findDistanceBetweenTwoPoints(points);
            if(distance_Min - cur_Distance > 0){
                distance_Min = cur_Distance;
                bestPos = i;
            }
            else{
            }
        }
    } 
    return bestPos;
}

// Returns the area of the given closed feature in square meters
// Assume a non self-intersecting polygon (i.e. no holes)
// Return 0 if this feature is not a closed polygon.
// Speed Requirement --> moderate
double findFeatureArea(FeatureIdx feature_id){
    int num_feature_points = getNumFeaturePoints(feature_id);
    //return if no feature points
    if (num_feature_points == 0){
        return 0;
    }
    LatLon start_point = getFeaturePoint(0, feature_id);
    LatLon end_point = getFeaturePoint(num_feature_points - 1, feature_id);
    
    //return if not a closed polygon
    if (!(start_point == end_point)){
        return 0;
    }
    
    double lat_avg = 0;
    
    //get average value of latitude
    for (int i = 0; i < num_feature_points; i++){
        LatLon point_i = getFeaturePoint(i, feature_id);
        lat_avg += point_i.latitude() * kDegreeToRadian;
    }
    lat_avg = lat_avg / num_feature_points;
    
    double area = 0;
    
    for (int j = 0; j < num_feature_points - 1; j++){
        LatLon point_1 = getFeaturePoint(j, feature_id);
        LatLon point_2 = getFeaturePoint(j + 1, feature_id);
        
        //get x,y for current point and next point
        double x1 = kEarthRadiusInMeters * point_1.longitude()* kDegreeToRadian * cos(lat_avg);
        double y1 = kEarthRadiusInMeters * point_1.latitude()* kDegreeToRadian;
        double x2 = kEarthRadiusInMeters * point_2.longitude()* kDegreeToRadian * cos(lat_avg);
        double y2 = kEarthRadiusInMeters * point_2.latitude()* kDegreeToRadian;
        
        //Shoelace formula
        area += (x1 * y2 - x2 * y1);
    }
    //maintain positive area value
    return fabs(area / 2);
}

// Return the LatLon of an OSM node; will only be called with OSM nodes (not ways or relations)
// Speed requirement --> high
LatLon findLatLonOfOSMNode (OSMID OSMid){
    return OSM_node_map.find(OSMid)->second;
}

