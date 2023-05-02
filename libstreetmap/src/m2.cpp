#include <iostream>
#include "m1.h"
#include "m2.h"
#include <math.h>
#include <vector>
#include <unordered_map>
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <map>
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "m3.h"
#include "helper.h"
#include <string.h>


#define PI 3.14159265


//////////////////////////////////////////////////////////////////////////Variables from M1;
extern std::vector<StreetIdx> street_bin;
extern std::vector<double> street_seg_travel_time;

extern std::vector<std::vector<StreetSegmentIdx>> segs_in_street;
extern std::vector<double> street_length;

extern std::unordered_map<OSMID, LatLon> OSM_node_map;

extern std::vector<std::vector<StreetSegmentIdx>> Intersection_StreetSegment_Information;
extern std::vector<std::vector<std::string>> Intersection_StreetName_Information;

extern std::vector<StreetSegmentIdx> get_One_Intersection_StreetSegment_Information(IntersectionIdx intersection_id);
extern std::vector<std::string> get_One_Intersection_StreetName_Information(IntersectionIdx intersection_id);

extern std::vector<std::string> AllStreetNames;
extern std::vector<std::vector<IntersectionIdx>> street_intersections;
extern std::multimap <std::string, StreetIdx> Street_Name;


//////////////////////////////////////////////////////////////////////////Variables from M1;
constexpr ezgl::color PARK_C = ezgl::color(159,214,174,255);
constexpr ezgl::color BEACH_C = ezgl::color(254,239,195,255);
constexpr ezgl::color LAKE_C = ezgl::color(156,192,249,255);
constexpr ezgl::color RIVER_C = ezgl::color(156,192,249,200);
constexpr ezgl::color ISLAND_C = ezgl::color(176,221,189,255);
constexpr ezgl::color BUILDING_C = ezgl::color(188,188,188,255);
constexpr ezgl::color GREENSPACE_C = ezgl::color(176,221,189,200);
constexpr ezgl::color GOLFCOURSE_C = ezgl::color(73,178,101,255);
constexpr ezgl::color GLACIER_C = ezgl::color(159, 244, 243, 255);
constexpr ezgl::color OTHER_ROAD_C = ezgl::color(255,255,255,255);
constexpr ezgl::color OTHER_ROAD_BC = ezgl::color(190,189,188,25);
constexpr ezgl::color MOTORWAY_C = ezgl::color(233,144,160,255);
constexpr ezgl::color MOTORWAY_BC = ezgl::color(227,89,134,255);

constexpr ezgl::color TRUNK_C = ezgl::color(251,192,172,255);
constexpr ezgl::color TRUNK_BC = ezgl::color(227,139,111,255);

constexpr ezgl::color PRIMARY_C = ezgl::color(253,215,161,255);
constexpr ezgl::color PRIMARY_BC = ezgl::color(202,161,71,255);

constexpr ezgl::color SECONDARY_C = ezgl::color(253,226,147,255);
constexpr ezgl::color SECONDARY_BC = ezgl::color(249,191,63,255);
constexpr ezgl::color INTERSECTION_C = ezgl::color(24, 186, 244, 255);
constexpr ezgl::color HIGHLIGHT_C = ezgl::color(253, 107, 50, 255);
constexpr ezgl::color UNKNOWN_C = ezgl::color(0, 0, 0, 255);
constexpr ezgl::color STREAM_C = ezgl::color(25, 160, 190, 255);
constexpr ezgl::color ONEWAY_C = ezgl::color(159, 43, 3, 255);
constexpr ezgl::color SUBWAY_GREEN = ezgl::color(103,255,58,100);
constexpr ezgl::color SUBWAY_YELLOW = ezgl::color(238,236,32, 100);
constexpr ezgl::color SUBWAY_PURPLE = ezgl::color(128, 48, 233, 100);
constexpr ezgl::color BACKGROUND_C = ezgl::color(243,245,246, 255);


/////////////////////////////////////////////////////////////////////////// New Structure for M2
struct intersection_data {
    //LatLon position;
    ezgl::point2d xy_loc;
    bool highlight;
    std:: string name;
};

struct street_segment_data {
    //LatLon position;
    ezgl::point2d xy_loc_from;
    ezgl::point2d xy_loc_to;
    ezgl::point2d middle;
    double length;
    StreetSegmentIdx seg_id;
    std:: vector<ezgl::point2d> curves;
    std::string name;
    double angle = 0;
    bool oneWay;
    bool highlight = false;
};


struct street_data {
    //LatLon position;
    std:: vector<street_segment_data> segments;
    bool highlight;
    bool motorway;
    bool trunk;
    bool primary;
    bool secondary;
    bool other;
    int draw_level = 7;
    std:: string name;
};



struct feature_data{
    
    FeatureType type;
    
    std:: string Name; 
     
    bool highlight;
    
    bool ifclosed;
    
    std:: vector<ezgl::point2d> all_points;
    
    ezgl::color color;
    
    int draw_level = 9;
    
};

struct subway_line{
    std:: string line_name;
    
    std:: string color;
    
    std:: vector<std::string> all_station_name;
    
    std:: vector<ezgl::point2d> all_station_position;
    
    ezgl::color subway_color;
        
};

struct interest_points_data{
    ezgl::point2d xy_loc;
    std::string name;
    std::string type;
    bool highlight;
};



struct zoom_info{
    int level;
    double upper_bound;
    double lower_bound;
};

/////////////////////////////////////M3 struct
//contains the information of one streer segment. 
//The length of a certain street segment, 
//A vector that represnets a ceratin street segment, 
//the time to go though a ceratin  street segment.
struct seg_path_infor{
    std:: string time_string;
    int time;
    int distance;
    std::vector<double> path;
};

          
std::vector<zoom_info> zoom_level = {
    {.level = 9, .upper_bound = DBL_MAX, .lower_bound = 2e+09}, //motorway,trunk
    {.level = 8, .upper_bound = 2e+09, .lower_bound = 7e+07}, //motorway, trunk, primary//secondary,
    {.level = 7, .upper_bound = 7e+07, .lower_bound = 3e+07},
    {.level = 6, .upper_bound = 3e+07, .lower_bound = 3e+06}, //POI, secondary
    {.level = 5, .upper_bound = 3e+06, .lower_bound = 9e+05},
    {.level = 4, .upper_bound = 9e+05, .lower_bound = 4e+05},
    //{.level = 3, .upper_bound = 3e+06, .lower_bound = 0},
    //{.level = 6, .upper_bound = 3e+06 .lower_bound = 0}//intersections,
    {.level = 0, .upper_bound = 4e+05, .lower_bound = 0}
};

//////////////////////////////////////////////////////////////////////////////////Global Variable for M2; 
std::string file_name = "";
std:: vector<intersection_data> intersections;
std:: vector<street_data> streets;
std::vector<const OSMWay*> osm_motorway;
std::vector<const OSMWay*> osm_trunk;
std::vector<const OSMWay*> osm_primary;
std::vector<const OSMWay*> osm_secondary;
std:: vector<street_data> ordered_streets;
//////////////////////////////////////////////////
std::vector<interest_points_data> restaurant;
std::vector<interest_points_data> hospital;
std::vector<interest_points_data> bank;
std::vector<interest_points_data> school;
std::vector<interest_points_data> other_POI;


//////////////////////////////////////////////////////////M3 Global Variable
bool findPathMode = false;
std::vector<IntersectionIdx> findPath_intersections;
std::vector<std::pair<IntersectionIdx, IntersectionIdx>> findPath_intersectionPairs;
std::vector<street_segment_data> ordered_all_segs;
const int turn_penalty = 15;

std::vector<street_segment_data> path_Seg;
std::vector<std::string> direction_information;


/////////////////////////////////////////////////////////////////calculate time test
std::vector<StreetSegmentIdx> street_segs = {110639, 54463, 54466, 6817, 23382, 23381, 23371, 23370, 110650, 22938,
            22617, 110651, 22012, 22011, 110656, 110657, 110654, 110655, 110661, 110662, 110660, 110668, 79753, 79750, 110672,
            110674, 42009, 110682, 110681, 110680, 110689, 110698, 110693, 110694, 110695, 88726, 88725, 88722, 88727, 110704,
            110718, 110724, 216961, 216960, 110723, 110721, 110717, 110719, 110722, 110720, 18071, 110714, 9095, 27287, 151224,
            18070, 110708, 110710, 110706, 110705, 24469};


/////////////////////////////////////////////////////////////////////////////////Haochang's variable
std:: vector<feature_data> all_feature;

std::vector<subway_line> all_subway_line;
std:: vector <interest_points_data> interest_points;


/////////////////////////////////////////////////////////////////////////////////
double max_lat, min_lat, max_lon, min_lon, avg_lat;
double max_x, max_y, min_x, min_y;
int level;

//////////////////////////////////////////////////////////////////////////////////Initialize Functions
void draw_main_canvas(ezgl::renderer *g);
void get_data_and_bound();
void load_street();
void act_on_mouse_click(ezgl::application* app, GdkEventButton* event, double x, double y);
bool my_cmp(const street_data& a, const street_data& b);
void get_zoom_level(double x0, double x1, double y0, double y1);
void draw_text(ezgl::renderer *& g, ezgl::point2d loc, double degree, std::string text, bool oneWay);
///////////////////////////////////////////////////////////////////////////////Haochang's function
feature_data get_one_feature_pos(FeatureIdx featureIdx);
void get_all_feature_pos();
void get_all_subway_line();
void get_all_interest_points();


////////////////////////////////////////clear functions
void all_feature_clear();
void all_subway_line_clear();
void interest_points_clear();
void clear_intersections();
void clear_ordered_street();
void clear_street_and_segment();
void clear_osm_data();
void clear();

//////////////////////////////////////////////////////////////////////////
double x_from_lon (float lon);
double y_from_lat (float lat);
double lon_from_x (float x);
double lat_from_y(float y) ;


ezgl::point2d get_midpoint(ezgl::point2d from, ezgl::point2d to);
double get_angle(ezgl::point2d from, ezgl::point2d to);
double get_length(ezgl::point2d from, ezgl::point2d to);
////////////////////////////////////////////////////////////////////////////
bool pinkify = false;
void displayMoreMessage(GtkWidget* /*widget*/, ezgl::application* application);
void load_beijing(GtkWidget* /*widget*/, ezgl::application* application);
void test_button(GtkWidget *widget, ezgl::application *application);
void load_toronto(GtkWidget* /*widget*/, ezgl::application* application);
void clear(GtkWidget *widget, ezgl::application *application);
void find(GtkWidget *widget, ezgl::application *application);
POIIdx findClosestPOI_oneInput(LatLon my_position);

void initial_setup(ezgl::application* application, bool /*new_window*/);
void loadAll();

//////////////////////////////////////////////////////////////////////////////////////M3 Function Declaration
void highlight_path(const std:: vector<StreetSegmentIdx>& path);

void get_path(const std:: vector<StreetSegmentIdx>& path);//std::vector<street_segment_data> path_Seg;
void get_direction_information(std::vector<street_segment_data> path_Seg_A);//std::vector<std::string> direction_information;

//double computePathTravelTime(const double turn_penalty, const std::vector<StreetSegmentIdx>& path);
int crossProduct(std:: vector<double> vect_A, std::vector<double> vect_B);
std::vector<double> getVector(double x_start, double y_start, double x_finish, double y_finish);
seg_path_infor getinfor_SegIdx(street_segment_data data);
void reset();



GtkListStore* ListFromPrimarySearch;

double x_from_lon (float lon) {
  return (lon * kDegreeToRadian * kEarthRadiusInMeters * std::cos (avg_lat * kDegreeToRadian));
}

double y_from_lat (float lat) {
  return (lat * kDegreeToRadian * kEarthRadiusInMeters);
}

double lon_from_x (float x) {
  return (x / (kDegreeToRadian * kEarthRadiusInMeters * std::cos (avg_lat * kDegreeToRadian)));
}

double lat_from_y(float y) {
  return (y / ( kDegreeToRadian * kEarthRadiusInMeters));
}

bool my_cmp(const street_data& a, const street_data& b){
    // smallest comes first
    return a.draw_level < b.draw_level;
}

////////////////////////////////////////////////////////////////////////////////// Full part of Functions

void get_data_and_bound(){    
    max_lat = getIntersectionPosition(0).latitude();
    min_lat = max_lat;
    max_lon = getIntersectionPosition(0).longitude();
    min_lon = max_lon;
   
    intersections.resize(getNumIntersections());
    
    for(int id=0; id<getNumIntersections(); ++id){
        LatLon inter_pos = getIntersectionPosition(id);
  
        max_lat = std::max(max_lat, inter_pos.latitude());
        min_lat = std::min(min_lat, inter_pos.latitude());
        max_lon = std::max(max_lon, inter_pos.longitude());
        min_lon = std::min(min_lon, inter_pos.longitude());
    } 
    avg_lat = (max_lat + min_lat)/2;
    
    for (int id =0; id < getNumIntersections(); ++id){
        LatLon inter_pos = getIntersectionPosition(id);
        intersections[id].xy_loc.x = x_from_lon(inter_pos.longitude());
        intersections[id].xy_loc.y = y_from_lat(inter_pos.latitude());
        intersections[id].name = getIntersectionName(id);
        intersections[id].highlight = false;
    }
}

void load_street(){
    //load all street segments to all streets
    streets.resize(getNumStreets());

    for (int id =0; id < getNumStreetSegments(); ++id){
        StreetSegmentInfo seg_info = getStreetSegmentInfo(id);
        int street_id = seg_info.streetID;
        
        LatLon intersection_1_pos = getIntersectionPosition(seg_info.from);
        LatLon intersection_2_pos = getIntersectionPosition(seg_info.to);
        
        street_segment_data seg;
        seg.xy_loc_from = {x_from_lon(intersection_1_pos.longitude()),y_from_lat(intersection_1_pos.latitude())};
        seg.xy_loc_to = {x_from_lon(intersection_2_pos.longitude()),y_from_lat(intersection_2_pos.latitude())};
        
       
        seg.name = getStreetName(street_id);
        seg.seg_id = id;
        seg.oneWay = seg_info.oneWay;
        if (seg_info.numCurvePoints > 0){
            for (int num_curves = 0; num_curves < seg_info.numCurvePoints; ++num_curves){
                LatLon curve_pos = getStreetSegmentCurvePoint(num_curves, seg.seg_id);
                ezgl::point2d curve_loc = {x_from_lon(curve_pos.longitude()), y_from_lat(curve_pos.latitude())};
                seg.curves.push_back(curve_loc);
            }
        }
        if (seg_info.numCurvePoints == 0){
            seg.length = std::sqrt(std::pow((seg.xy_loc_to.y - seg.xy_loc_from.y),2) + std::pow((seg.xy_loc_to.x - seg.xy_loc_from.x),2));
            seg.middle = {(seg.xy_loc_from.x + seg.xy_loc_to.x)/2, (seg.xy_loc_from.y + seg.xy_loc_to.y)/2};
            if (seg.xy_loc_to.x != seg.xy_loc_from.x){
                seg.angle = std::atan((seg.xy_loc_to.y - seg.xy_loc_from.y) / (seg.xy_loc_to.x - seg.xy_loc_from.x)) * 180 / PI;
            }
            else if(seg.xy_loc_to.x == seg.xy_loc_from.x){
                if (seg.xy_loc_to.y > seg.xy_loc_from.y){
                    seg.angle = 90;
                } 
                else{
                    seg.angle = -90;
                }        
            }    
        }
        
        ordered_all_segs.push_back(seg);
        streets[street_id].segments.push_back(seg);
        streets[street_id].name = getStreetName(street_id);
        streets[street_id].highlight = false;
        streets[street_id].motorway = false;
        streets[street_id].trunk = false;
        streets[street_id].primary = false;
        streets[street_id].secondary = false;
        streets[street_id].other = true;
    }
    
    //load all highways to different vectors categorized by types
    for (int i = 0; i < getNumberOfWays(); ++i){
        const OSMWay *way = getWayByIndex(i);
        
        if (isClosedWay(way) == false){
            for (int j = 0; j < getTagCount(way); ++j){
                std::pair<std::string, std::string> tagPair = getTagPair(way, j);
                /////
                if (tagPair.first == "highway"){
                    if (tagPair.second == "motorway"){
                        osm_motorway.push_back(way);
                    }
                    else if (tagPair.second == "trunk"){
                        osm_trunk.push_back(way);
                    }
                    else if (tagPair.second == "primary"){
                        osm_primary.push_back(way);
                    }
                    else if (tagPair.second == "secondary"){
                        osm_secondary.push_back(way);
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < osm_motorway.size(); ++i){
        for (int j = 0; j <getTagCount(osm_motorway[i]); ++j){
            std::pair<std::string, std::string> tagPair = getTagPair(osm_motorway[i], j);
            
            for (int k = 0; k < getNumStreets(); ++k){
                if (tagPair.first == "name"){
                    if (streets[k].name == tagPair.second){
                        streets[k].motorway = true;
                        streets[k].other = false;
                        streets[k].draw_level = 9;
                    }
                }
            }  
        }
    }
    for (int i = 0; i < osm_trunk.size(); ++i){
        for (int j = 0; j <getTagCount(osm_trunk[i]); ++j){
            std::pair<std::string, std::string> tagPair = getTagPair(osm_trunk[i], j);
            
            for (int k = 0; k < getNumStreets(); ++k){
                if (tagPair.first == "name"){
                    if (streets[k].name == tagPair.second){
                        streets[k].trunk = true;
                        streets[k].other = false;
                        streets[k].draw_level = 9;
                    }
                }
            }  
        }
    }
    for (int i = 0; i < osm_primary.size(); ++i){
        for (int j = 0; j <getTagCount(osm_primary[i]); ++j){
            std::pair<std::string, std::string> tagPair = getTagPair(osm_primary[i], j);
            
            for (int k = 0; k < getNumStreets(); ++k){
                if (tagPair.first == "name"){
                    if (streets[k].name == tagPair.second){
                        streets[k].primary = true;
                        streets[k].other = false;
                        streets[k].draw_level = 8;
                    }
                }
            }  
        }
    }
    for (int i = 0; i < osm_secondary.size(); ++i){
        for (int j = 0; j <getTagCount(osm_secondary[i]); ++j){
            std::pair<std::string, std::string> tagPair = getTagPair(osm_secondary[i], j);
            
            for (int k = 0; k < getNumStreets(); ++k){
                if (tagPair.first == "name"){
                    if (streets[k].name == tagPair.second){
                        streets[k].secondary = true;
                        streets[k].other = false;
                        streets[k].draw_level = 8;
                    }
                }
            }  
        }
    }
    ordered_streets.assign(streets.begin(),streets.end());
    std::sort(ordered_streets.begin(), ordered_streets.end(), my_cmp);
}
feature_data get_one_feature_pos(FeatureIdx featureIdx){
    
    feature_data Feature_area;
    Feature_area.type = getFeatureType(featureIdx);
    
    if(Feature_area.type == PARK){
        Feature_area.color = PARK_C;
    }
    else if(Feature_area.type == BEACH){
        Feature_area.color = BEACH_C;
    }
    else if(Feature_area.type == LAKE){
        Feature_area.color = LAKE_C;
    }
    else if(Feature_area.type == RIVER){
        Feature_area.color = RIVER_C;
        Feature_area.draw_level = 8;   
    }
    else if(Feature_area.type == ISLAND){
        Feature_area.color = ISLAND_C;
    }
    else if(Feature_area.type == BUILDING){
        Feature_area.color = BUILDING_C;
        Feature_area.draw_level = 6;
    }
    else if(Feature_area.type == GREENSPACE){
        Feature_area.color = GREENSPACE_C;
    }
    else if(Feature_area.type == GOLFCOURSE){
        Feature_area.color = GOLFCOURSE_C;
    }
    else if(Feature_area.type == STREAM){
        Feature_area.color = STREAM_C;
        Feature_area.draw_level = 8; 
    }
    else if(Feature_area.type == GLACIER){
        Feature_area.color = GLACIER_C;
    }
    else{
        Feature_area.color = UNKNOWN_C;
    }
    
    Feature_area.Name = getFeatureName(featureIdx);
    
    LatLon start_points, finish_points;
    LatLon cur_pos;
    
 
    ezgl::point2d cur_xy_loc;
    
    int numOfPoints = getNumFeaturePoints(featureIdx);
    //// need to check one point
    Feature_area.all_points.resize(numOfPoints);

    
    for(int i = 0; i< numOfPoints; i++){
        cur_pos = getFeaturePoint(i, featureIdx);
        cur_xy_loc.x = x_from_lon(cur_pos.longitude());
        cur_xy_loc.y = y_from_lat(cur_pos.latitude());
        Feature_area.all_points[i] = cur_xy_loc;
    }
    
    start_points = getFeaturePoint(0, featureIdx);
    finish_points = getFeaturePoint((numOfPoints - 1), featureIdx);
    
    if(start_points == finish_points){
        Feature_area.ifclosed = true;
    }
    else{
        Feature_area.ifclosed = false;
    }
    
    return Feature_area;
}


void get_all_feature_pos(){
    int totalFeature = getNumFeatures();
    
    all_feature.resize(totalFeature);
    
    for(int i =0; i < totalFeature; i++){
        all_feature[i] = get_one_feature_pos(i);
    }
    
}

void get_all_interest_points(){
    LatLon current_pos;
    interest_points.resize(getNumPointsOfInterest());
    for( int id = 0; id < getNumPointsOfInterest(); ++ id){
        current_pos = getPOIPosition(id);
        
        interest_points[id].xy_loc.x = x_from_lon(current_pos.longitude());
        interest_points[id].xy_loc.y = y_from_lat(current_pos.latitude());
        interest_points[id].name = getPOIName(id);
        interest_points[id].type = getPOIType(id);
        
    
    }

}

void get_all_subway_line(){
    std::vector <const OSMRelation *> osm_subway_lines;
    subway_line certain_Line;

    
    LatLon cur_pos;
    ezgl::point2d cur_xy_loc;

    for(unsigned i = 0 ; i< getNumberOfRelations(); i++){
        const OSMRelation *currRel = getRelationByIndex(i);
        for(unsigned j =0; j < getTagCount(currRel); j++){
            std::pair<std::string, std::string> tagPair = getTagPair(currRel, j);
            
            if(tagPair.first == "route" && tagPair.second == "subway"){
                osm_subway_lines.push_back(currRel);
                break;
            }
        }
    }
    
    
    all_subway_line.resize(osm_subway_lines.size());
    
    for (unsigned i = 0; i < osm_subway_lines.size(); i++) {


        for (unsigned j = 0; j < getTagCount(osm_subway_lines[i]); j++) {
            std::pair <std::string, std::string> tagPair = getTagPair(osm_subway_lines[i], j);

            if (tagPair.first == "colour") {
                certain_Line.color = tagPair.second; 
            } 
            else if (tagPair.first == "name") {
                 certain_Line.line_name = tagPair.second;
            }
                
            if(certain_Line.color == "green"){
                certain_Line.subway_color = SUBWAY_GREEN;
            }
            else if(certain_Line.color == "yellow"){
                certain_Line.subway_color = SUBWAY_YELLOW;
            }
            else if(certain_Line.color == "purple"){
                certain_Line.subway_color = SUBWAY_PURPLE;
            }
            else{
                certain_Line.subway_color = ezgl::color(0, 0, 0, 255);
            }
            
            
            
        }
        std::vector <TypedOSMID> route_members = getRelationMembers(osm_subway_lines[i]);

        for(unsigned j =0; j < route_members.size(); j++){
            
            if(route_members[j].type() == TypedOSMID::Node){
                const OSMNode *currNode = nullptr;
            
                for(unsigned k = 0; k < getNumberOfNodes(); k++){
                    currNode = getNodeByIndex(k);
                    if(currNode ->id() == route_members[j]){
                        break;
                    }
                }
                
                cur_pos = getNodeCoords(currNode);
                cur_xy_loc.x = x_from_lon(cur_pos.longitude());
                cur_xy_loc.y = y_from_lat(cur_pos.latitude()); 
                
                certain_Line.all_station_position.push_back(cur_xy_loc);
                

                for(unsigned k = 0; k < getTagCount(currNode); k++){
                    std:: pair<std::string,std::string> tagPair = getTagPair(currNode, k);
                    if(tagPair.first == "name"){
                       // std::cout << "\t" << tagPair.second << std::endl;
                        certain_Line.all_station_name.push_back(tagPair.second);
                        break;
                        
                    }
                    
                }
            }
            
        }
        
        all_subway_line[i] = certain_Line;
        certain_Line.all_station_name.clear();
        certain_Line.all_station_position.clear();
        route_members.clear();
        
    }
     osm_subway_lines.clear();
}

void get_zoom_level(double x0, double x1, double y0, double y1){
    double cur_area = abs(x0 - x1) *abs(y0 - y1);
    for (int i = 0; i < zoom_level.size(); ++i){
        if (cur_area <= zoom_level[i].upper_bound && cur_area > zoom_level[i].lower_bound){
            level = zoom_level[i].level;
        }
    }
}


void highlight_path(const std:: vector<StreetSegmentIdx>& path){
    int numOfStreet_Seg;
    bool ifFind;

    std::vector <StreetIdx> street_Used;
    for(int i = 0; i < path.size(); i++){
        street_Used.push_back(getStreetSegmentInfo(path[i]).streetID);
    }


    for(int i; i < path.size(); i++){
        ifFind = false;
        for(int j = 0; j < street_Used.size(); j++){
            for(int k = 0; j<streets[j].segments.size(); k++){
                if(streets[j].segments[k].seg_id == path[i]){
                    streets[j].segments[k].highlight == true;
                    ifFind == true;
                    break;
                }
            }
            if(ifFind){
                break;
            }
        }
    }
}


//Passing in a vector path contains the streetSegment Id that needs to go though.
//Go though the path vector and find all struct segments that has the same id in the path vector,
//and push these segments into a global vector called path_Seg
void get_path(const std:: vector<StreetSegmentIdx>& path){
    int numOfStreet_Seg;
    bool ifFind;
    for(int i; i < path.size(); i++){
        ifFind = false;
        for(int j = 0; j < streets.size(); j++){
            for(int k = 0; k<streets[j].segments.size(); k++){
                if(streets[j].segments[k].seg_id == path[i]){
                    path_Seg.push_back(streets[j].segments[k]);
                    streets[j].segments[k].highlight == true;
                    ifFind == true;
                    break;
                }
            }
            if(ifFind){
                break;
            }
        }
    }
}


std::vector<double> getVector(double x_start, double y_start, double x_finish, double y_finish){
    std::vector<double> street_Vector;
    street_Vector.push_back(x_finish - x_start);
    street_Vector.push_back(y_finish - y_start);
    return street_Vector;   
}

//Passing in two vectors and cross product them. 
//Getting the Z value of the cross product to determin if user need to turn left or right.
int crossProduct(std:: vector<double> vect_A, std::vector<double> vect_B){ 
    double check;
    check = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];

    return check;
}

 seg_path_infor getinfor_SegIdx(street_segment_data data){
    double x_from, y_from, x_to, y_to;
    seg_path_infor current_infor;

    x_from = data.xy_loc_from.x;
    y_from = data.xy_loc_from.y;
    x_to = data.xy_loc_to.x;
    y_to = data.xy_loc_to.y;

    current_infor.time = round(findStreetSegmentTravelTime(data.seg_id));
    int rounded_to_tens = round(findStreetSegmentLength(data.seg_id) / 10) * 10;
    current_infor.distance = rounded_to_tens;

    current_infor.time_string = std::to_string(current_infor.time);
    current_infor.path = getVector(x_from, y_from, x_to, y_to);

    return current_infor;
}


void get_direction_information(std::vector<street_segment_data> path_Seg_A){
    double x_dis;
    double y_dis;
    double check;
    
    int time = 0;
    int distance = 0;
    
    std:: string time_string;
    std:: string distance_string;
    
    
    std:: string A;    //used to store direction

    
    seg_path_infor current_infor;
    seg_path_infor next_infor;

    
    
    if(path_Seg_A.size() == 1){            // special case for only 1 street Segments.
        x_dis = path_Seg_A[0].xy_loc_to.x - path_Seg_A[0].xy_loc_from.x;      
        y_dis = path_Seg_A[0].xy_loc_to.y - path_Seg_A[0].xy_loc_from.y;     
        
        current_infor = getinfor_SegIdx(path_Seg_A[0]); 
     
            if(((x_dis > 0) && (y_dis > 0) && (abs(x_dis)/abs(y_dis) < tan(15/180))) ||  ((x_dis < 0) && (y_dis > 0) && (abs(x_dis)/abs(y_dis) < tan(15/180))) ){               
                time = current_infor.time;
                distance = current_infor.distance;
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);     
                A = "Going to North at " + path_Seg_A[0].name + "it will takes " + time_string+ " seconds to reach destination and the total distance is " + distance_string + "meters";
                direction_information.push_back(A);
                
            }
            else if( ((x_dis > 0) && (y_dis > 0) && (abs(y_dis)/abs(x_dis) < tan(15/180))) ||  ((x_dis > 0) && (y_dis < 0) && (abs(y_dis)/abs(x_dis) < tan(15/180))) ){
                time = current_infor.time;
                distance = current_infor.distance;
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);
                A = "Going to East at " + path_Seg_A[0].name + "it will takes " + time_string + " seconds to reach destination and the total distance is " + distance_string + "meters";
                direction_information.push_back(A);
                 
            }
            else if( ((x_dis > 0) && (y_dis < 0) && (abs(x_dis)/abs(y_dis) < tan(15/180))) ||  ((x_dis < 0) && (y_dis < 0) && (abs(x_dis)/abs(y_dis) < tan(15/180))) ){
                time = current_infor.time;
                distance = current_infor.distance;
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);
                A = "Going to South at " + path_Seg_A[0].name+ "it will takes " + time_string + " seconds to reach destination and the total distance is " + distance_string +"meters";
                direction_information.push_back(A);
                 
            }
            else if( ((x_dis < 0) && (y_dis < 0) && (abs(y_dis)/abs(x_dis) < tan(15/180))) ||  ((x_dis < 0) && (y_dis > 0) && (abs(y_dis)/abs(x_dis) < tan(15/180))) ){
                time = current_infor.time;
                distance = current_infor.distance;
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);
                A = "Going to West at " + path_Seg_A[0].name +  "it will takes " + time_string + " seconds to reach destination and the total distance is " + distance_string +"meters";
                direction_information.push_back(A);
                
            }
            else if((x_dis > 0) && (y_dis > 0)){
                time = current_infor.time;
                distance = current_infor.distance;
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);
                A = "Going to North-East at " + path_Seg_A[0].name +  "it will takes " + time_string + " seconds to reach destination and the total distance is " + distance_string +"meters";
                direction_information.push_back(A);
                
            }
            else if((x_dis > 0) && (y_dis < 0)){
                time = current_infor.time;
                distance = current_infor.distance;
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);
                A = "Going to South-East at " + path_Seg_A[0].name +  "it will takes " + time_string + " seconds to reach destination and the total distance is " + distance_string +"meters";
                direction_information.push_back(A);
                

            }
            else if((x_dis < 0) && (y_dis < 0)){
                time = current_infor.time;
                distance = current_infor.distance;
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);
                A = "Going to South-West at " + path_Seg_A[0].name +  "it will takes " + time_string + " seconds to reach destination and the total distance is " + distance_string +"meters";
                direction_information.push_back(A);
                
            }
            else if((x_dis < 0) && (y_dis > 0)){
                time = current_infor.time;
                distance = current_infor.distance;
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);
                A = "Going to North-West at " + path_Seg_A[0].name +  "it will takes " + time_string + " seconds to reach destination and the total distance is " + distance_string +"meters";
                direction_information.push_back(A);
            }
    }
    
    
    
    for(int i = 0; i < path_Seg_A.size()- 1; i++){   // go though all the street segments
       
        x_dis = path_Seg_A[0].xy_loc_to.x - path_Seg_A[0].xy_loc_from.x;      
        y_dis = path_Seg_A[0].xy_loc_to.y - path_Seg_A[0].xy_loc_from.y;     // get X distance and Y distance of the first street segments.
        
        current_infor = getinfor_SegIdx(path_Seg_A[i]); 
     
        if(i == 0){
            if(((x_dis > 0) && (y_dis > 0) && (abs(x_dis)/abs(y_dis) < tan(15/180))) ||  ((x_dis < 0) && (y_dis > 0) && (abs(x_dis)/abs(y_dis) < tan(15/180))) ){
                A = "Going to North at " + path_Seg_A[0].name; ///////if the x_dis > 0, y> dis 0, the angle of 
                time = current_infor.time;
                distance = current_infor.distance;
                direction_information.push_back(A);
                
            }
            else if( ((x_dis > 0) && (y_dis > 0) && (abs(y_dis)/abs(x_dis) < tan(15/180))) ||  ((x_dis > 0) && (y_dis < 0) && (abs(y_dis)/abs(x_dis) < tan(15/180))) ){
                A = "Going to East at " + path_Seg_A[0].name;
                time = current_infor.time;
                distance = current_infor.distance;
                direction_information.push_back(A);
                 
            }
            else if( ((x_dis > 0) && (y_dis < 0) && (abs(x_dis)/abs(y_dis) < tan(15/180))) ||  ((x_dis < 0) && (y_dis < 0) && (abs(x_dis)/abs(y_dis) < tan(15/180))) ){
                A = "Going to South at " + path_Seg_A[0].name;
                time = current_infor.time;
                distance = current_infor.distance;
                direction_information.push_back(A);
                 
            }
            else if( ((x_dis < 0) && (y_dis < 0) && (abs(y_dis)/abs(x_dis) < tan(15/180))) ||  ((x_dis < 0) && (y_dis > 0) && (abs(y_dis)/abs(x_dis) < tan(15/180))) ){
                A = "Going to West at " + path_Seg_A[0].name;
                time = current_infor.time;
                distance = current_infor.distance;
                direction_information.push_back(A);
                
            }
            else if((x_dis > 0) && (y_dis > 0)){
                A = "Going to North-East at " + path_Seg_A[0].name;
                time = current_infor.time;
                distance = current_infor.distance;
                direction_information.push_back(A);
                
            }
            else if((x_dis > 0) && (y_dis < 0)){
                A = "Going to South-East at " + path_Seg_A[0].name;
                time = current_infor.time;
                distance = current_infor.distance;
                direction_information.push_back(A);
                

            }
            else if((x_dis < 0) && (y_dis < 0)){
                A = "Going to South-West at " + path_Seg_A[0].name;
                time = current_infor.time;
                distance = current_infor.distance;
                direction_information.push_back(A);
                
            }
            else if((x_dis < 0) && (y_dis > 0)){
                A = "Going to North-West at " + path_Seg_A[0].name;
                time = current_infor.time;
                distance = current_infor.distance;
                direction_information.push_back(A);
            }
        }
        
        
        if((getStreetSegmentInfo(path_Seg_A[i].seg_id).streetID == getStreetSegmentInfo(path_Seg_A[i+1].seg_id).streetID) && i != 0){
                
                time = time+ current_infor.time;
                distance = distance+ current_infor.distance;
            
                
        }
        
        
        if(getStreetSegmentInfo(path_Seg_A[i].seg_id).streetID != getStreetSegmentInfo(path_Seg_A[i+1].seg_id).streetID){
                next_infor = getinfor_SegIdx(path_Seg_A[i+1]);
                check = crossProduct( current_infor.path, next_infor.path);
                time_string = std:: to_string(time);
                distance_string = std:: to_string(distance);
                
                
                if(i != 0){
                    time = time+ current_infor.time;
                    distance = distance+ current_infor.distance;
                }
                
                
                A = "It takes " + time_string + " seconds. " +" Total Distance is " + distance_string + " meters.";
                direction_information.push_back(A);
                
                if(check < 0){
                    A = "Then turn Right to " + path_Seg_A[i+1].name + " It takes 15s.";
                    time = 0;
                    distance = 0;
                    direction_information.push_back(A);
                    A = "Keep going on " + path_Seg_A[i+1].name;
                    direction_information.push_back(A);
                    
                }
                else if (check > 0){
                    A = "Then turn Left to " + path_Seg_A[i+1].name + " It takes 15s.";
                    time = 0;
                    distance = 0;
                    direction_information.push_back(A);
                    A = "Keep going on " + path_Seg_A[i+1].name;
                    direction_information.push_back(A);
                     
                }
                else{
                    A = "Keep going on the same Direction, but the street is changed.";
                    time = 0;
                    distance = 0;
                    direction_information.push_back(A);
                }

        }
        
        if(i == path_Seg_A.size()- 2){
            next_infor = getinfor_SegIdx(path_Seg_A[i+1]);
            time = time + next_infor.time;
            distance = distance + next_infor.distance;
            
            time_string = std:: to_string(time);
            distance_string = std:: to_string(distance);
            
            A = "It takes " + time_string + " seconds. " +" Total Distance is " + distance_string + " meters. Then will reach destination";
            direction_information.push_back(A);
            
            
        }
    }
}
void reset(){
    path_Seg.clear();
    direction_information.clear();
}


void draw_main_canvas(ezgl::renderer *g){
    if (file_name == "/cad2/ece297s/public/maps/beijing_china.streets.bin"){
        g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal, ezgl::font_weight::normal);
    }
    
    
    //draw streets    
    ezgl::rectangle cur_screen = g->get_visible_world();
    double area = abs((cur_screen.m_first.x - cur_screen.m_second.x)*(cur_screen.m_first.y-cur_screen.m_second.y));
    double visible_size = std::max(10.0,20-1.6*1e-2*std::sqrt(area));
    std::cout<<"first x: "<<cur_screen.m_first.x<<"  first y: "<<cur_screen.m_first.y<<"  second x: "<<cur_screen.m_second.x<<"  second y: "<<cur_screen.m_second.y<<std::endl;
    std::cout<<"min x: "<<x_from_lon(min_lon)<<"  min y: "<<y_from_lat(min_lat)<<"  max x: "<<x_from_lon(max_lon)<<"  max y: "<<y_from_lat(max_lat)<<std::endl<<"area: "<<area<<std::endl;
    
    get_zoom_level(cur_screen.m_first.x, cur_screen.m_second.x, cur_screen.m_first.y, cur_screen.m_second.y);
    std::cout<<"level: "<<level<<std::endl;
    
    ezgl::rectangle initial_world({min_x,min_y},{max_x,max_y});
    
    g->set_color(BACKGROUND_C);
    g->fill_rectangle(initial_world);
    
    for(int i = 0; i < getNumFeatures(); ++i){
        if (all_feature[i].draw_level >= level){
            
            if(all_feature[i].ifclosed == true && all_feature[i].all_points.size() > 1){
                g->set_color(all_feature[i].color);
                 g->fill_poly(all_feature[i].all_points);
            }
            else{
               int numOfPoints = all_feature[i].all_points.size();
                for(int j = 0; j < (numOfPoints-1); j++){
                    g->set_color(all_feature[i].color);
                    g->draw_line(all_feature[i].all_points[j], all_feature[i].all_points[j+1]);
                }
            }
        }
    }  
    
    
    
    
    
    
    for (size_t i =0; i < ordered_streets.size(); ++i) {
        ezgl::color colour, bound_colour;
        double width;
        if (ordered_streets[i].motorway == true){
            width = 5 + 9 - level;
            colour = MOTORWAY_C;
            bound_colour = MOTORWAY_BC;
        }
        else if (ordered_streets[i].trunk == true){
            width = 3 + 9 - level;
            colour = TRUNK_C;
            bound_colour = TRUNK_BC;
        }
        else if (ordered_streets[i].primary == true){
            width = 2 + 9 - level;
            colour = PRIMARY_C;
            bound_colour = PRIMARY_BC;
        }
        else if (ordered_streets[i].secondary == true){
            width = 2 + 9 - level;
            colour = SECONDARY_C;
            bound_colour = SECONDARY_BC;
        }
        else{
            width = 1 + 9 - level;
            colour = OTHER_ROAD_C;
            bound_colour = OTHER_ROAD_BC;
        }
        //g->set_line_cap(ezgl::line_cap::round); //need to be justify
        if (ordered_streets[i].draw_level >= level){
            for (size_t j =0; j < ordered_streets[i].segments.size(); ++j){
                g->set_color(colour);
                g->set_line_width(width);
                //StreetSegmentInfo seg_info = getStreetSegmentInfo(streets[i].segments[j].seg_id);
                StreetSegmentIdx seg_id = ordered_streets[i].segments[j].seg_id;
                StreetSegmentInfo seg_info = getStreetSegmentInfo(seg_id);

                ezgl::point2d from = ordered_streets[i].segments[j].xy_loc_from;
                ezgl::point2d to = ordered_streets[i].segments[j].xy_loc_to;

                if (seg_info.numCurvePoints == 0){   
                    if (ordered_streets[i].draw_level -2 >= level){
                        g->set_color(bound_colour);
                        g->set_line_width(width*1.2);
                        g->draw_line(from, to);
                    }
                    g->set_color(colour);
                    g->set_line_width(width);
                    g->draw_line(from, to);

                    //std::cout<<from.x<<std::endl<<from.y<<std::endl<<to.x<<std::endl<<to.y<<std::endl;
                    //std::cout<<streets[i].name<<std::endl;
                }
                else{
                    for (int k =0; k < seg_info.numCurvePoints; ++k){
                        if (k == 0){
                           
                            if (ordered_streets[i].draw_level -2 >= level){
                                //g->set_line_cap(ezgl::line_cap::round);
                                g->set_color(bound_colour);
                                g->set_line_width(width*1.2);
                                g->draw_line(from, ordered_streets[i].segments[j].curves[k]);
                            }
                            g->set_color(colour);
                            g->set_line_width(width);
                            g->draw_line(from, ordered_streets[i].segments[j].curves[k]);
                        }
                        if (k == seg_info.numCurvePoints-1){
                           
                            if (ordered_streets[i].draw_level -2 >= level){
                                //g->set_line_cap(ezgl::line_cap::round);
                                g->set_color(bound_colour);
                                g->set_line_width(width*1.2);
                                g->draw_line(ordered_streets[i].segments[j].curves[k], to);
                            }
                            g->set_color(colour);
                            g->set_line_width(width);
                            g->draw_line(ordered_streets[i].segments[j].curves[k], to);
                        }
                        else{                       
                            if (ordered_streets[i].draw_level -2 >= level){
                                //g->set_line_cap(ezgl::line_cap::round);
                                g->set_color(bound_colour);
                                g->set_line_width(width*1.2);
                                g->draw_line(ordered_streets[i].segments[j].curves[k], ordered_streets[i].segments[j].curves[k+1]);
                            }
                            g->set_color(colour);
                            g->set_line_width(width);
                            g->draw_line(ordered_streets[i].segments[j].curves[k], ordered_streets[i].segments[j].curves[k+1]); 
                        }
                    }  
                } 
            }
        }
    }
    
    //draw subway station and lines
    if (level <= 9){
        g->set_line_width(3);
        std::vector<std::string>names;

        for(int i = 0; i< all_subway_line.size(); i++){
            int numOfPoints = all_subway_line[i].all_station_position.size();
            int numOfNames = all_subway_line[i].all_station_name.size();
            g->set_color(all_subway_line[i].subway_color);
            for(int j =0 ; j < (numOfPoints - 1) ; j++){
                //fill station
                g->fill_rectangle(all_subway_line[i].all_station_position[j], 10, 10);
                //draw station route
                g->draw_line(all_subway_line[i].all_station_position[j],all_subway_line[i].all_station_position[j+1]);
                //std::cout<< all_subway_line[i].all_station_name.size()<< numOfPoints << std::endl;
            }
            for(int j =0; j<numOfNames; j++){
                if (level <= 6){
                    
                    bool printed = false;
                    for (int k = 0; k < names.size(); ++k){
                        //std::cout<<"size: "<<names.size()<<"   to be displayed: "<<all_subway_line[i].all_station_name[j]<<"   names stored: "<<names[k]<<std::endl;

                        if (all_subway_line[i].all_station_name[j] == names[k]){
                            printed = true;
                        }       
                    }
                    //std::cout<<"name: "<<all_subway_line[i].all_station_name[j]<<"id: "<<all_subway_line[i].all_station_position[j].x<<" "<<all_subway_line[i].all_station_position[j].y<<" "<<std::endl;
                    if (printed == false){
                        g->set_color(ezgl::BLACK);
                        g->set_font_size(visible_size);
                        g->draw_text(all_subway_line[i].all_station_position[j],all_subway_line[i].all_station_name[j]);
                        names.push_back(all_subway_line[i].all_station_name[j]);
                    }
      
                    
                    
                }
            }
        }
    }
    
    
///////////////////////////highlight streetsegs
    if(findPathMode){
        if(findPath_intersections.size()<2){
            std::cout << "Too few intersections selected." << std::endl;
            //application->update_message("Too few intersections selected.");
        }
        else{// if (findPath_intersections.size()>=2)
            for(int i=0; i<findPath_intersections.size()-1; i++){
                std::pair<IntersectionIdx, IntersectionIdx> two_intersections (findPath_intersections[i], findPath_intersections[i+1]);
                std::vector<StreetSegmentIdx> segs_to_be_highlighted = findPathBetweenIntersections(turn_penalty, two_intersections);
            
//void get_path(const std:: vector<StreetSegmentIdx>& path);//std::vector<street_segment_data> path_Seg;
//void get_direction_information(std::vector<street_segment_data> path_Seg_A);//std::vector<std::string> direction_information;
                path_Seg.clear();
                direction_information.clear();
                get_path(segs_to_be_highlighted);//return path_Seg--Global Variable
                get_direction_information(path_Seg);//return direction_information
                
                for(int j=0; j<segs_to_be_highlighted.size(); j++){
                

                    
                    street_segment_data seg = ordered_all_segs[segs_to_be_highlighted[j]];
std::cout <<  seg.seg_id << std::endl;
                    ezgl::color colour, bound_colour;
                    double width = 3 + 9 - level;
                    colour = ezgl::color(255,0,0,255); //rgb value-red
                //g->set_line_cap(ezgl::line_cap::round); //need to be justify
                
                    g->set_color(colour); //set color
                    g->set_line_width(width); //set width
                    ezgl::point2d from = seg.xy_loc_from; //w¹
                    ezgl::point2d to = seg.xy_loc_to; //È¹
                    StreetSegmentInfo seg_info = getStreetSegmentInfo(seg.seg_id);

                    if (seg_info.numCurvePoints == 0){  //¡	curve pointsÎw¹;0È¹
                        g->draw_line(from, to);
                    }
                    else{
                       for (int k =0; k < seg_info.numCurvePoints; ++k){
                          if (k == 0){ //curvepoint, from
                              g->draw_line(from, seg.curves[k]);
                          }
                            // curve
                            if (k == seg_info.numCurvePoints-1){                 
                               g->draw_line(seg.curves[k], to);
                           }
                            //curve 
                           else{                       
                               g->draw_line(seg.curves[k], seg.curves[k+1]); 
                            }
                
                        }
                    }
                
                }
            }
            
            
            //findPath_intersectionPairs.push_back(two_intersections);
            //highlight_path(segs_to_be_highlighted);
        }
        
        //std::vector<StreetSegmentIdx> findPathBetweenIntersections(turn_penalty, std::pair<IntersectionIdx, IntersectionIdx> intersect_ids);
        //highlight_path(const std:: vector<StreetSegmentIdx>& path);
    }
    
    
     
     
    //draw intersections & POIs
    if (level <= 5){
        //draw POIs
        int numOfPoints = getNumPointsOfInterest();
        ezgl::surface *surface_normal = ezgl::renderer::load_png("libstreetmap/resources/small_image.png");
        ezgl::surface *surface_highlight = ezgl::renderer::load_png("libstreetmap/resources/image_highlight.png");

        for (size_t i = 0; i < numOfPoints; i++) {
            if(interest_points[i].highlight==false){
                g->draw_surface(surface_normal, interest_points[i].xy_loc);
            }
            else{
                g->draw_surface(surface_highlight, interest_points[i].xy_loc);
            }
            
        }
        //draw intersections
        for (size_t i =0; i < intersections.size(); ++i) {
            ezgl::point2d inter_loc = intersections[i].xy_loc;
            if(intersections[i].highlight == true ){
                g->set_color(HIGHLIGHT_C);
            }
            else{
                g->set_color(INTERSECTION_C);
            }
            g->fill_arc(inter_loc, 5, 0, 360);
        }
    }
    
    for (size_t i =0; i < ordered_streets.size(); ++i) {
        for (size_t j =0; j < ordered_streets[i].segments.size(); ++j){
            if (level <= 9 ){
                std::string text;
                text = ordered_streets[i].segments[j].name;
                g->set_font_size(visible_size);
                if (text != "<unknown>"){
                    
               
                    if (ordered_streets[i].segments[j].oneWay == true){
                        text += " >>>";

                    }
                    if (ordered_streets[i].segments[j].curves.size() == 0){
                        g->set_color(ezgl::BLACK);
                        g->set_text_rotation(ordered_streets[i].segments[j].angle);
                        g->draw_text(ordered_streets[i].segments[j].middle, text, ordered_streets[i].segments[j].length, 100);
                        //g->set_text_rotation(0);
                    }

                    else if (ordered_streets[i].segments[j].curves.size() > 0){
                        double length, angle;
                        ezgl::point2d mid;
                        for (size_t k = 0; k < ordered_streets[i].segments[j].curves.size(); ++k){                   
                            if (k == 0){  
                                length = get_length(ordered_streets[i].segments[j].xy_loc_from, ordered_streets[i].segments[j].curves[0]);
                                angle = get_angle(ordered_streets[i].segments[j].xy_loc_from, ordered_streets[i].segments[j].curves[0]);                    
                                mid = get_midpoint(ordered_streets[i].segments[j].xy_loc_from, ordered_streets[i].segments[j].curves[0]);

                                g->set_text_rotation(angle);
                                g->draw_text(mid, text, length, 100);
                            }
                            if (k == ordered_streets[i].segments[j].curves.size()-1){
                                length = get_length(ordered_streets[i].segments[j].curves[k], ordered_streets[i].segments[j].xy_loc_to);
                                angle = get_angle(ordered_streets[i].segments[j].curves[k], ordered_streets[i].segments[j].xy_loc_to);
                                mid = get_midpoint(ordered_streets[i].segments[j].curves[k], ordered_streets[i].segments[j].xy_loc_to);

                                g->set_text_rotation(angle);
                                g->draw_text(mid, text, length, 100); 
                            }
                            else{
                                length = get_length(ordered_streets[i].segments[j].curves[k], ordered_streets[i].segments[j].curves[k+1]);
                                angle = get_angle(ordered_streets[i].segments[j].curves[k], ordered_streets[i].segments[j].curves[k+1]);
                                mid = get_midpoint(ordered_streets[i].segments[j].curves[k], ordered_streets[i].segments[j].curves[k+1]);

                                g->set_text_rotation(angle);
                                g->draw_text(mid, text, length, 100);         
                            }

                        }    
                    }
                }
                //std::cout<<"length: "<<ordered_streets[i].segments[j].length<<std::endl;
                //std::cout<<"text "<<text<<std::endl;
                //std::cout<<"degree: "<< c<<std::endl;
                //std::cout<<"x: "<<ordered_streets[i].segments[j].middle.x<<" y: "<<ordered_streets[i].segments[j].middle.y<<std::endl;
            }
            
        }
    }
    
    

}






ezgl::point2d get_midpoint(ezgl::point2d from, ezgl::point2d to){
    ezgl::point2d mid = {(from.x + to.x)/2, (from.y + to.y)/2};
    return mid;
}
double get_angle(ezgl::point2d from, ezgl::point2d to){
    double angle = std::atan((from.y - to.y) / (from.x - to.x)) * 180 / PI;
    if (from.y == to.y){
        angle = 0;
    }
    if(from.x == to.x){
        if (from.y > to.y){
            angle = 90;
        } 
        else{
            angle = -90;
        }        
    }
    return angle;
}
double get_length(ezgl::point2d from, ezgl::point2d to){
    double length = std::sqrt(std::pow((from.x - to.x)/2,2)+ std::pow((from.y - to.y)/2,2));
    return length;
}





void clear_intersections(){
    intersections.clear();
}

void all_feature_clear(){
    for(int i = 0 ; i < all_feature.size(); i++){
        all_feature[1].all_points.clear();
    }
    all_feature.clear();
}

void all_subway_line_clear(){
    for(int i = 0; i< all_subway_line.size(); i++){
        all_subway_line[i].all_station_name.clear();
        all_subway_line[i].all_station_position.clear();
    }
    all_subway_line.clear();
}

void interest_points_clear(){
    interest_points.clear();
}

void clear_street_and_segment(){
    for (int i = 0; i < streets.size(); ++i){
        for (int j = 0; j < streets[i].segments.size(); ++j){
            streets[i].segments[j].curves.clear();
        }
        streets[i].segments.clear();
    }
    streets.clear();
}

void clear_ordered_street(){
    for (int i = 0; i < ordered_streets.size(); ++i){
        for (int j = 0; j < ordered_streets[i].segments.size(); ++j){
            ordered_streets[i].segments[j].curves.clear();
        }
        ordered_streets[i].segments.clear();
    }
    ordered_streets.clear();
}

void clear_osm_data(){
    osm_motorway.clear();
    osm_trunk.clear();
    osm_primary.clear();
    osm_secondary.clear();   
}

void clear_ordered_all_segs(){
    ordered_all_segs.clear();
}

//std::vector<IntersectionIdx> findPath_intersections;
//std::vector<std::pair<IntersectionIdx, IntersectionIdx>> findPath_intersectionPairs;
//std::vector<street_segment_data> ordered_all_segs;
void clear_findPath_interections(){
    findPath_intersections.clear();
}
void clear_findPath_intersectionPairs(){
    //for(int i=0; i<findPath_intersectionPairs.size(); i++){
        //findPath_intersectionPairs[i].clear();
    //}
    findPath_intersectionPairs.clear();
}


void clear(){
    all_feature_clear();
    all_subway_line.clear();
    interest_points_clear();
    clear_ordered_street();
    clear_street_and_segment();
    clear_osm_data();
    clear_intersections();
    clear_ordered_all_segs();
    clear_all_node();
    reset();
    clear_findPath_interections();
    clear_findPath_intersectionPairs();



}




void drawMap () {
    ////////////////////////////////////////////////////////////////////////////call functions -- load all information
    get_data_and_bound();
    load_street();
    get_all_feature_pos();
    get_all_subway_line();
    get_all_interest_points();
    
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    ezgl::application application(settings);
    
    max_x = x_from_lon(max_lon);
    max_y = y_from_lat(max_lat);
    min_x = x_from_lon(min_lon);
    min_y = y_from_lat(min_lat);
    
    
    
    ezgl::rectangle initial_world({min_x,min_y},{max_x,max_y});

    
    application.add_canvas("MainCanvas", draw_main_canvas,initial_world);
   
    
    application.run(initial_setup, act_on_mouse_click, nullptr, nullptr);
    
    clear();
}





void act_on_mouse_click(ezgl::application* app, GdkEventButton* event, double x, double y){
    std::cout << "Mouse clicked at (" << x << "," << y << ")\n";
    LatLon pos = LatLon(lat_from_y(y), lon_from_x(x));
    
    int intersectionID = findClosestIntersection(pos);
    std::cout << "Closest Intersection: " << intersections[intersectionID].name << "\n";
    int POIID = findClosestPOI_oneInput(pos);
    std::cout << "Closest POI: " << interest_points[POIID].name << "\n";
    
    std::pair<LatLon, LatLon> position_of_mouse_and_intersection (pos, getIntersectionPosition(intersectionID));
    int distance_intersection = findDistanceBetweenTwoPoints(position_of_mouse_and_intersection);
    std::pair<LatLon, LatLon> position_of_mouse_and_POI (pos, getPOIPosition(POIID));
    int distance_POI = findDistanceBetweenTwoPoints(position_of_mouse_and_POI);
    
    if(distance_intersection <= 5){
        intersections[intersectionID].highlight = !intersections[intersectionID].highlight;
  
        std::stringstream ss;
        ss << "Intersection Selected: " << intersections[intersectionID].name;
        app->update_message(ss.str());
        
        if(findPathMode){
            intersections[intersectionID].highlight = true;
            findPath_intersections.push_back(intersectionID);

        }
        
    }
    if(distance_POI <= 5){
        interest_points[POIID].highlight = !interest_points[POIID].highlight;
     
        std::stringstream ss;
        ss << "Point of Interest -- " <<  interest_points[POIID].type << ": " << interest_points[POIID].name;
        app->update_message(ss.str());
    }

    app->refresh_drawing();
}

POIIdx findClosestPOI_oneInput(LatLon my_position){
    double  distance_Min = 10000000000;
    double cur_Distance; 
    POIIdx  bestPos = 0;
    
    int totalNum_POI = getNumPointsOfInterest();
    for(int i = 0; i < totalNum_POI; i++){
            LatLon position = getPOIPosition(i);
            std::pair<LatLon, LatLon> points (my_position, position);
            cur_Distance = findDistanceBetweenTwoPoints(points);
            if(distance_Min - cur_Distance > 0){
                distance_Min = cur_Distance;
                bestPos = i;
            }
    } 
    return bestPos;
}

void dropdownMenu1(GtkWidget *widget, ezgl::application *application);
void dropdownMenu2(GtkWidget *widget, ezgl::application *application);
void load_newyork(GtkWidget* /*widget*/, ezgl::application* application);
void load_tokyo(GtkWidget* /*widget*/, ezgl::application* application);
void help(GtkWidget* /*widget*/, ezgl::application* application);

void findPath(GtkWidget* /*widget*/, ezgl::application* application);

void initial_setup(ezgl::application* application, bool /*new_window*/){
    //application->create_button("Useless Change street colour", 6, displayMoreMessage);
    application->create_button("Load Beijing", 8, load_beijing);
    application->create_button("Load Toronto", 9, load_toronto);
    //application->create_button("Load New York", 13, load_newyork);
    //application->create_button("Load Tokyo", 14, load_tokyo);
    
    application->create_button("Show Directions", 12, test_button);
    application->create_button("Path Finding Mode", 13, findPath);
    application->create_button("Clear", 11, clear);
    application->create_button("Find", 10, find);
    application->create_button("Help", 14, help);
    
    GObject *myWidget1 = application ->get_object("PrimarySearchBox");
    g_signal_connect(myWidget1, "changed", G_CALLBACK(dropdownMenu1), application);
    GObject *myWidget2 = application ->get_object("SecondarySearchBox");
    g_signal_connect(myWidget2, "changed", G_CALLBACK(dropdownMenu2), application);
    
    
    
    
}
void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data);
void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data)
{
    // For demonstration purposes, this will show the enum name and int value of the button that was pressed
    std::cout << "response is ";
    switch(response_id) {
        case GTK_RESPONSE_ACCEPT:
            std::cout << "GTK_RESPONSE_ACCEPT ";
            break;
        case GTK_RESPONSE_DELETE_EVENT:
            std::cout << "GTK_RESPONSE_DELETE_EVENT (i.e. 'X' button) ";
            break;
        case GTK_RESPONSE_REJECT:
            std::cout << "GTK_RESPONSE_REJECT ";
            break;
        default:
            std::cout << "UNKNOWN ";
            break;
    }
    std::cout << "(" << response_id << ")\n";
    // This will cause the dialog to be destroyed and close.
    // without this line the dialog remains open unless the
    // response_id is GTK_RESPONSE_DELETE_EVENT which
    // automatically closes the dialog without the following line.
    gtk_widget_destroy(GTK_WIDGET (dialog));
}
void test_button(GtkWidget *widget, ezgl::application *application){
    // Get the GtkEntry object
    //GtkEntry* text_entry = (GtkEntry *) application->get_object("PrimarySearchBox");
    // Get the text written in the widget
    //const char* text = gtk_entry_get_text(text_entry);
    // Update the status bar message
    
    application->update_message("Showing directions.");
    // Redraw the graphics
    application->refresh_drawing();
    
    
    GObject *window; // the parent window over which to add the dialog
    GtkWidget *content_area; // the content area of the dialog
    GtkWidget *label; // the label we will create to display a message in the content area
    GtkWidget *dialog;
        
    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Create the dialog window.
    // Modal windows prevent interaction with other windows in the same application
    dialog = gtk_dialog_new_with_buttons(
    "Directions",
    (GtkWindow*) window,
    GTK_DIALOG_MODAL,
    ("OK"),
    GTK_RESPONSE_ACCEPT,
    ("CANCEL"),
    GTK_RESPONSE_REJECT,
    NULL    
    );
    
    // Create a label and attach it to the content area of the dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    std::string nothing = "";
    std::string direction_instructions = "No path was chosen.";
    //direction_instructions = nothing + "Simple Dialog With a Label." + "\n" + "Press OK or CANCEL too close.";
    
    for(int i=0; i<direction_information.size(); i++){
        if(i==0){
            direction_instructions = nothing;
        }
        direction_instructions = direction_instructions + direction_information[i] + "\n";
    }

            
            
    //convert string to char
    gchar* char_array = new char[direction_instructions.length()+1];
    strncpy(char_array, direction_instructions.c_str(), direction_instructions.length()+1);
    
    label = gtk_label_new(char_array);
    gtk_container_add(GTK_CONTAINER(content_area), label);

    // The main purpose of this is to show dialog's child widget, label
    gtk_widget_show_all(dialog);
    
    // Connecting the "response" signal from the user to the associated callback function
    g_signal_connect(
        GTK_DIALOG(dialog),
        "response",
        G_CALLBACK(on_dialog_response),
        NULL
    );

}

void help(GtkWidget *widget, ezgl::application *application){
    // Get the GtkEntry object
    GtkEntry* text_entry = (GtkEntry *) application->get_object("PrimarySearchBox");
    // Get the text written in the widget
    const char* text = gtk_entry_get_text(text_entry);
    // Update the status bar message
    //application->update_message(text);
    // Redraw the graphics
    application->refresh_drawing();
    
    
    GObject *window; // the parent window over which to add the dialog
    GtkWidget *content_area; // the content area of the dialog
    GtkWidget *label; // the label we will create to display a message in the content area
    GtkWidget *dialog;
        
    // get a pointer to the main application window
    window = application->get_object(application->get_main_window_id().c_str());
    
    // Create the dialog window.
    // Modal windows prevent interaction with other windows in the same application
    dialog = gtk_dialog_new_with_buttons(
    "Help Instructions",
    (GtkWindow*) window,
    GTK_DIALOG_MODAL,
    ("OK"),
    GTK_RESPONSE_ACCEPT,
    ("CANCEL"),
    GTK_RESPONSE_REJECT,
    NULL    
    );
    
    // Create a label and attach it to the content area of the dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    std::string nothing = "";
    std::string direction_instructions;
    direction_instructions = nothing + "- Click on intersections/stars to see more information." 
                            +"\n"+ "- Enter two street names and press Find to see the intersections."
                            +"\n"+ "- Enter a city name in the first search bar and press Find to switch maps."
                            +"\n"+ "- Enter a type of interest and press Find to highlight all related points of interest."
                            +"\n"+ "- Turn on Path Finding Mode and click on intersections to find the closet path."
                            +"\n"+ "- Click on Show Directions to see detailed instruction on how to follow the closest path."
            ;
    
    //convert string to char
    gchar* char_array = new char[direction_instructions.length()+1];
    strncpy(char_array, direction_instructions.c_str(), direction_instructions.length()+1);
    
    label = gtk_label_new(char_array);
    gtk_container_add(GTK_CONTAINER(content_area), label);

    // The main purpose of this is to show dialogs child widget, label
    gtk_widget_show_all(dialog);
    
    // Connecting the "response" signal from the user to the associated callback function
    g_signal_connect(
        GTK_DIALOG(dialog),
        "response",
        G_CALLBACK(on_dialog_response),
        NULL
    );

}

void displayMoreMessage(GtkWidget* /*widget*/, ezgl::application* application){
    pinkify = !pinkify;
    application->refresh_drawing();
}

void loadAll(){
    get_data_and_bound();
    load_street();
    get_all_feature_pos();
    get_all_subway_line();
    get_all_interest_points();
}
void load_beijing(GtkWidget* /*widget*/, ezgl::application* application){
    std::cout << "load beijing clicked" << std::endl;
    application->update_message("load beijing clicked");

    closeMap();
    
std::cout << "before clear" << std::endl;

    //Clear variables from M2
    clear();

std::cout << "after clear" << std::endl;

    file_name = "/cad2/ece297s/public/maps/beijing_china.streets.bin";
    loadMap(file_name);
    
    loadAll();
    
    ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)},{x_from_lon(max_lon),y_from_lat(max_lat)});
    application->change_canvas_world_coordinates("MainCanvas", initial_world);

    application->refresh_drawing();
}
void load_toronto(GtkWidget* /*widget*/, ezgl::application* application){
    std::cout << "load toronto clicked" << std::endl;
    application->update_message("load toronto clicked");

    closeMap();

    //Clear variables from M2
    clear();
    

    loadMap("/cad2/ece297s/public/maps/toronto_canada.streets.bin");

    loadAll();

    ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)},{x_from_lon(max_lon),y_from_lat(max_lat)});
    application->change_canvas_world_coordinates("MainCanvas", initial_world);

    application->refresh_drawing();
}
void load_newyork(GtkWidget* /*widget*/, ezgl::application* application){
    std::cout << "load New York clicked" << std::endl;
    application->update_message("load New York clicked");

    closeMap();

    //Clear variables from M2
    clear();
    

    loadMap("/cad2/ece297s/public/maps/new-york_usa.streets.bin");

    loadAll();

    ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)},{x_from_lon(max_lon),y_from_lat(max_lat)});
    application->change_canvas_world_coordinates("MainCanvas", initial_world);

    application->refresh_drawing();
}
void load_tokyo(GtkWidget* /*widget*/, ezgl::application* application){
    std::cout << "load New York clicked" << std::endl;
    application->update_message("load Tokyo clicked");

    closeMap();

    //Clear variables from M2
    clear();
    

    loadMap("/cad2/ece297s/public/maps/tokyo_japan.streets.bin");

    loadAll();

    ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)},{x_from_lon(max_lon),y_from_lat(max_lat)});
    application->change_canvas_world_coordinates("MainCanvas", initial_world);

    application->refresh_drawing();
}

void clear(GtkWidget* /*widget*/, ezgl::application* application){
    pinkify = false;
    
    findPath_intersections.clear();
    for(int i=0; i<intersections.size(); ++i){
        intersections[i].highlight = false;
    }
    for(int i=0; i<getNumPointsOfInterest(); ++i){
        interest_points[i].highlight = false;
    }
    application->refresh_drawing();
}

GtkEntry* getText;
GtkListStore* streetList;


void dropdownMenu1(GtkWidget* /*widget*/, ezgl::application *application){
    
    auto dropdownMenu = static_cast<ezgl::application*> (application);
    std::string input=gtk_entry_get_text((GtkEntry*)dropdownMenu->get_object("PrimarySearchBox"));
    gtk_list_store_clear((GtkListStore*) dropdownMenu->get_object("ListFromPrimarySearch"));
    
    
    std::vector<StreetIdx> all_streetID = findStreetIdsFromPartialStreetName(input);
    
    
    streetList = (GtkListStore*) dropdownMenu->get_object("ListFromPrimarySearch");
    GtkTreeIter Iterator;
    for(int i=0;i<all_streetID.size();i++){
        
        //convert string to char
        std::string string = getStreetName(all_streetID[i]);
        gchar* char_array = new char[string.length()+1];
        strncpy(char_array, string.c_str(), string.length()+1);
        
        gtk_list_store_append(streetList, &Iterator);
        gtk_list_store_set(streetList, &Iterator, 0, char_array, -1);
        delete [] char_array;

    }
    
}

void dropdownMenu2(GtkWidget* /*widget*/, ezgl::application *application){
    
    auto dropdownMenu = static_cast<ezgl::application*> (application);
    std::string input=gtk_entry_get_text((GtkEntry*)dropdownMenu->get_object("SecondarySearchBox"));
    gtk_list_store_clear((GtkListStore*) dropdownMenu->get_object("ListFromSecondarySearch"));
    
    
    std::vector<StreetIdx> all_streetID = findStreetIdsFromPartialStreetName(input);
    
    
    streetList = (GtkListStore*) dropdownMenu->get_object("ListFromSecondarySearch");
    GtkTreeIter Iterator;
    for(int i=0;i<all_streetID.size();i++){
        
        //convert string to char
        std::string string = getStreetName(all_streetID[i]);
        gchar* char_array = new char[string.length()+1];
        strncpy(char_array, string.c_str(), string.length()+1);
        
        gtk_list_store_append(streetList, &Iterator);
        gtk_list_store_set(streetList, &Iterator, 0, char_array, -1);
        delete [] char_array;
    }
    
}


void findPath(GtkWidget* /*widget*/, ezgl::application* application){
    findPathMode = !findPathMode;
    
    if(findPathMode){
        findPath_intersections.clear();
        std::cout << "Path Find Mode: On" << std::endl;
        application->update_message("Path Find Mode: On");
    }
    else{
        findPath_intersections.clear();
        std::cout << "Path Find Mode: Off" << std::endl;
        application->update_message("Path Find Mode: Off");
    }
    
}



void find(GtkWidget *widget, ezgl::application *application){
     std::cout << "Finding..." << std::endl;
     application->update_message("Finding...");
     
    // Get the GtkEntry object
    GtkEntry* text_entry1 = (GtkEntry *) application->get_object("PrimarySearchBox");
    // Get the text written in the widget
    std::string text1 = gtk_entry_get_text(text_entry1);
    
    GtkEntry* text_entry2 = (GtkEntry *) application->get_object("SecondarySearchBox");
    // Get the text written in the widget
    std::string text2 = gtk_entry_get_text(text_entry2);

    
    if(text1=="restaurant" || text1=="school" || text1=="cafe" || text1=="dentist" || text1=="fast_food" || text1=="doctors" || 
       text1=="clinic"     || text1=="pharmacy"|| text1=="bar" || text1=="car_wash" || text1=="music_school" || text1=="bank" || 
       text1=="college" || text1=="kindergarten"){
        
        
        /////////////lightup
        int numOfPoints = getNumPointsOfInterest();
        for (size_t i = 0; i < numOfPoints; i++) {
            if(interest_points[i].type==text1){
                interest_points[i].highlight=true;
            }       
        }
    }
    else if(text1=="beijing"){
        std::cout << "loading beijing" << std::endl;
        application->update_message("loading beijing");

        closeMap();
    
        //Clear variables from M2
        clear();
    
        file_name = "/cad2/ece297s/public/maps/beijing_china.streets.bin";
        loadMap(file_name);
    
        loadAll();
    
        ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)},{x_from_lon(max_lon),y_from_lat(max_lat)});
        application->change_canvas_world_coordinates("MainCanvas", initial_world);

        application->refresh_drawing();
    }
    else if(text1=="tokyo"){
        std::cout << "loading tokyo" << std::endl;
        application->update_message("loading tokyo");

        closeMap();
    
        //Clear variables from M2
        clear();
    
        file_name = "/cad2/ece297s/public/maps/tokyo_japan.streets.bin";
        loadMap(file_name);
    
        loadAll();
    
         ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)},{x_from_lon(max_lon),y_from_lat(max_lat)});
        application->change_canvas_world_coordinates("MainCanvas", initial_world);

        application->refresh_drawing();
    }
    else if(text1=="new york"){
        std::cout << "load New York clicked" << std::endl;
        application->update_message("load New York clicked");

        closeMap();

        //Clear variables from M2
        clear();
    

        loadMap("/cad2/ece297s/public/maps/new-york_usa.streets.bin");

        loadAll();

        ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)},{x_from_lon(max_lon),y_from_lat(max_lat)});
        application->change_canvas_world_coordinates("MainCanvas", initial_world);

        application->refresh_drawing();
    }
    else if(text1=="toronto"){
         std::cout << "load toronto clicked" << std::endl;
         application->update_message("load toronto clicked");

        closeMap();

        //Clear variables from M2
        clear();
    

        loadMap("/cad2/ece297s/public/maps/toronto_canada.streets.bin");

        loadAll();

        ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)},{x_from_lon(max_lon),y_from_lat(max_lat)});
        application->change_canvas_world_coordinates("MainCanvas", initial_world);

        application->refresh_drawing();
    }

////Find intersections////
    std::vector<StreetIdx> foundStreets_fromEntry1;
    std::vector<StreetIdx> foundStreets_fromEntry2;
    foundStreets_fromEntry1 = findStreetIdsFromPartialStreetName(text1);//(text1);//("college street");
    foundStreets_fromEntry2 = findStreetIdsFromPartialStreetName(text2);//(text2);//("bay");

    for(int StreetNum=0; StreetNum<foundStreets_fromEntry1.size(); StreetNum++)
    {
        //Find current streetname
        std::string currentStreetName = getStreetName(foundStreets_fromEntry1[StreetNum]);
    }

    std::string outputMessage = "";
    if(findStreetIdsFromPartialStreetName(text1).size()==0){
        outputMessage = "Invalid Input: cannot find street from entry1";
        application->update_message(outputMessage);
    }
    else if(findStreetIdsFromPartialStreetName(text2).size()==0 ){
        outputMessage = "Invalid Input: cannot find street from entry2";
        application->update_message(outputMessage);
    }
    else if(findStreetIdsFromPartialStreetName(text1).size()>1 && findStreetIdsFromPartialStreetName(text2).size()>1){
        outputMessage = "More than 1 street are found from both entry_1 and entry_2.";
        application->update_message(outputMessage);
        
        int intersectionExistance_count=0;
        for(int i=0; i<findStreetIdsFromPartialStreetName(text1).size(); i++){
            for(int j=0; i<findStreetIdsFromPartialStreetName(text2).size(); i++){
                std::pair<StreetIdx, StreetIdx> streetPair(findStreetIdsFromPartialStreetName(text1)[i], findStreetIdsFromPartialStreetName(text2)[j]);
                
                std::vector<IntersectionIdx> commonIntersections;
                commonIntersections = findIntersectionsOfTwoStreets(streetPair);
                
                if(commonIntersections.size()!=0){
                    if(findPathMode==true && commonIntersections.size()>1){
                        outputMessage = "More than 1 intersections are found. Click on one of them to choose.";
                        application->update_message(outputMessage);
                    }
                    
                    
                    intersectionExistance_count++;
                    
                    for(int i=0; i<commonIntersections.size(); ++i){
                       intersections[commonIntersections[i]].highlight = true;
std::cout << intersections[commonIntersections[i]].name << std::endl;            
                    }
                }
                
            }  
        }
        if(intersectionExistance_count==0){
            std::cout << "No intersections were found" << std::endl;   
            outputMessage = "No intersections were found. Streets have no intersections.";
            application->update_message(outputMessage);
        }
        
    }
    else if(findStreetIdsFromPartialStreetName(text1).size()>1){
        outputMessage = "More than 1 street are found from entry_1.";
        application->update_message(outputMessage);
        
        int intersectionExistance_count=0;
        for(int i=0; i<findStreetIdsFromPartialStreetName(text1).size(); i++){
            for(int j=0; i<findStreetIdsFromPartialStreetName(text2).size(); i++){
                std::pair<StreetIdx, StreetIdx> streetPair(findStreetIdsFromPartialStreetName(text1)[i], findStreetIdsFromPartialStreetName(text2)[j]);
                
                std::vector<IntersectionIdx> commonIntersections;
                commonIntersections = findIntersectionsOfTwoStreets(streetPair);
                
                if(commonIntersections.size()!=0){
                    
                    if(findPathMode==true && commonIntersections.size()>1){
                        outputMessage = "More than 1 intersections are found. Click on one of them to choose.";
                        application->update_message(outputMessage);
                    }
                    
                    intersectionExistance_count++;
                    
                    for(int i=0; i<commonIntersections.size(); ++i){
                       intersections[commonIntersections[i]].highlight = true;
std::cout << intersections[commonIntersections[i]].name << std::endl;   
                    }
                }
                
            }  
        }
        if(intersectionExistance_count==0){
            std::cout << "No intersections were found" << std::endl;   
            outputMessage = "No intersections were found. Streets have no intersections.";
            application->update_message(outputMessage);
        }
        
    }
    else if(findStreetIdsFromPartialStreetName(text2).size()>1){
        
        outputMessage = "More than 1 street are found from entry_2.";
        application->update_message(outputMessage);
        
        int intersectionExistance_count=0;
        for(int i=0; i<findStreetIdsFromPartialStreetName(text1).size(); i++){
            for(int j=0; i<findStreetIdsFromPartialStreetName(text2).size(); i++){
                std::pair<StreetIdx, StreetIdx> streetPair(findStreetIdsFromPartialStreetName(text1)[i], findStreetIdsFromPartialStreetName(text2)[j]);
                
                std::vector<IntersectionIdx> commonIntersections;
                commonIntersections = findIntersectionsOfTwoStreets(streetPair);
                
                if(commonIntersections.size()!=0){
                    
                    if(findPathMode==true && commonIntersections.size()>1){
                        outputMessage = "More than 1 intersections are found. Click on one of them to choose.";
                        application->update_message(outputMessage);
                    }
                    
                    intersectionExistance_count++;
                    
                    for(int i=0; i<commonIntersections.size(); ++i){
                       intersections[commonIntersections[i]].highlight = true;
std::cout << intersections[commonIntersections[i]].name << std::endl;   
                    }
                }
                
            }  
        }
        if(intersectionExistance_count==0){
            std::cout << "No intersections were found" << std::endl;   
            outputMessage = "No intersections were found. Streets have no intersections.";
            application->update_message(outputMessage);
        }

    }
    else{//find unique two streets from 2 entries

        std::cout << "Finding intersections..." << std::endl;   
        outputMessage = "Finding intersections...";
        application->update_message(outputMessage);

        
        std::pair<StreetIdx, StreetIdx> streetPair(findStreetIdsFromPartialStreetName(text1)[0], findStreetIdsFromPartialStreetName(text2)[0]);
    
        //Declaration    std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(streetPair);
        std::vector<IntersectionIdx> commonIntersections;
        commonIntersections = findIntersectionsOfTwoStreets(streetPair);
        
                    if(findPathMode==true && commonIntersections.size()>1){
                        outputMessage = "More than 1 intersections are found. Click on one of them to choose.";
                        application->update_message(outputMessage);
                    }
                    else if(findPathMode==true && commonIntersections.size()==1){
                        findPath_intersections.push_back(commonIntersections[0]);
                    }
        
        if(commonIntersections.size()==0){
            std::cout << "No intersections were found" << std::endl;   
            outputMessage = "No intersections were found. Two streets have no intersections.";
            application->update_message(outputMessage);
        }
        else{
            for(int i=0; i<commonIntersections.size(); ++i){
                intersections[commonIntersections[i]].highlight = true;
std::cout << intersections[commonIntersections[i]].name << std::endl;   
            }
        }
        
       
    }
   




    
    
    // Redraw the graphics
    application->refresh_drawing();
}

