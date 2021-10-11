//
// Created by zhan7417 on 2021-02-03.
//

#ifndef MAPPER_MAPDBSTRUCT_H
#define MAPPER_MAPDBSTRUCT_H
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>
#include <map>
#include "StreetsDatabaseAPI.h"
#include "ezgl/point.hpp"
#include "ezgl/application.hpp"
#include "ezgl/rectangle.hpp"
using namespace std;

/**
 * MapDBStruct is a data structure that contains 4 vectors
 *
 * @param1. Global variables are used to set map boundary and use in many files
 *
 * @param2.Vector-"intersection"-0~(Idx-1)
 *   Each element is a Struct called IntersectionInfo, representing every intersection
 *         @param2.1 "segmentId" returns a vector of SegmentId for function findStreetSegmentsOfIntersection
 *         @param2.2 "streetName" returns a vector of StreetName for function findStreetNamesOfIntersection
 *         @param2.3 "latLonLocation" returns a LatLon type position of every intersection point
 *         @param2.4 "xyLocation" returns a ezgl::point2d type position for every intersection point
 *         @param2.5 "highlight" returns a bool type for highlighting intersection
 *         @param2.6 "intersectionName" returns a string name of every intersection
 *         @param2.7 "timeToHere" is time spent from source intersection to here
 *         @param2.8 "heuristic" is approximate Time spent to destination
 *         @param2.9 "done"= true means this intersection won't be in the wavefront again
 *         @param2.10 "parentStreetID" is Parent Street ID
 *         @param2.11 "parentSegmentID" is parent segment ID
 *         @param2.12 "parentNodeID" is Prent Node ID
 *         @param2.13 "interID" is current intersection ID
 *         @param2.14 "start" is a bollean type to check if it is the start point
 *         @param2.15 "destination" is a bollean to check if it is the end point
 *
 * @param3. Vector-"interPosition"-contains all the positions in LatLon type of all intersections for Function
 *
 * @param4.Vector-"street"-0~(Idx-1)
 *   Each element is a Struct called StreetInfo, representing every street
 *         @param4.1 "intersectionOfStreets" returns a vector of IntersectionId in this Street for function
 *         "findIntersectionOfStreet"
 *         @param4.2 "segmentIdOfStreets" is a vector storing SegmentId in this Street for function
 *                    findStreetBoundingBox(transition)
 *         @param4.3 "streetLength" is a double storing the length of every street for function findStreetLength
 *         @param4.4 unordered_set<IntersectionIdx> preLoadIntersections;
           @param4.5 unordered_set<StreetSegmentIdx> preLoadSegments;
 *
 * @param5.Vector -"SegmentId"
 *   Each element is a struct called segment, represent every segment
 *         @param5.1 "segmentLength" is a double that return the length of every segment
 *                    for function findStreetSegmentLength
 *         @param5.2 "travelTime" is a double that return the amount of time that needed
 *                    for function findStreetSegmentTravelTime
 *         @param5.3 "angle" is a double type degree angle between every segment and map x,y coordinate
 *         @param5.4 "segmentLATLONPosition" is a vector that store the LatLon type of points in the segment
 *         @param5.5 "xyLocation" is a vector that store ezgl::point2d type of points in every segment
 *         @param5.5 "highlight" has a bool type value for find button function
 *
 * @param6.Vector-"pointOfInt"
 *    Each element contains the Information of each Point of Interest
 *         @param6.1 "name" Name of the POI
 *         @param6.2 "position" Position of POI
 *         @param6.3 "latLonPosition" returns the LatLon type position of every POI
 *         @param6.4 "xyPosition" returns the ezgl::point2d type of every POI
 *         @param6.5 "poiRectangle" is used to locate POI position in map display
 *         @param6.6 "isShown" is a bool type that mark if the POI position has shown in the graph
 *         @param6.7 "highlight" is a bool type that helps to highlight POI position for find buttom
 *
 * @param7. MultiMap-"StreetNaId" a MultiMap which contains the StreetName as key and streetId as value for
 *          findStreetIdFromPartialName.
 *
 * @param8. Vector-"Feature"
 *    Each element contain the information needed to compute feature
 *         @param8.1 "name" Name of the feature
 *         @param8.2 "type" type of feature
 *         @param8.3 "latLonPosition" returns a vector that contain LatLon type position of every feature graph
 *         @param8.4 "xyCoordinates" returns vector that contain ezgl::point2d type of every feature graph
 *         @param8.5 "xyLocation" returns vector that contain ezgl::point2d type of feature position
 *         @param8.6 "highlight" is a bool type that helps to highlight POI position for find buttom
 *
 * @param9. Multimap-"featureOrder"
 *     Sorting feature is used to order the way that the team draws map
 *
 * @param10. Vector-"direction"
 *    Each element is a boolean type to check the logo that every element should use
 *          @param10.1 noDirection is to check if the sentence will use the three points logo
 *          @param10.2 time is to check if the sentence will use the clock logo
 *          @param10.3 pathnote is to check if the sentence will use the path logo
 *          @param10.4 yourLoc is to check if we use the direct logo
 *          @param10.5 turnLeft is to check if we use the left logo
 *          @param10.6 turnRight is to check if we use the right logo
 *          @param10.7 go straight is to check if we use go straight logo
 *          @param10.8 end is to check if we use the end flag logo
 */

/**
 * Global Variable
 */
extern struct MapDBStruct mapDB;
extern double avgLat;
extern ezgl::application *appPtr;
extern double latavg;
extern double max_y;
extern double min_y;
extern double max_x;
extern double min_x;
extern string modifier(string &input);
extern std::vector<StreetSegmentIdx> pathVec;
extern double travelTimeS;
extern float maxSpeedLimit;

struct MapDBStruct {

    /**
     * Data Structure for Intersections
     */
    struct IntersectionInfo {
        vector<StreetSegmentIdx> segmentId; // M1: Connected segments' ID
        vector<string> streetName;          // M1: Connected Streets' ID
        LatLon latLonLocation;              // M2: Location of this intersection in LatLon
        ezgl::point2d xyLocation;           // M2: Location of this intersection in XY
        bool highlight = false;             // M2: Check whether this intersection is highlighted
        string intersectionName;            // M2: Name of this intersection

        double timeToHere;                  // M3: Time spent from source intersection to here
        double heuristic;                   // M3: Approximate Time spent to destination
        bool done;                          // M3: done = true means this intersection won't be in the wavefront again
        StreetIdx parentStreetID;           // M3: Parent Street ID
        StreetSegmentIdx parentSegmentID;   // M3: parent segment ID
        IntersectionIdx parentNodeID;       // M3: Prent Node ID
        IntersectionIdx interID;            // M3: current intersection ID

        bool start = false;                 // M3: If this is Start Point
        bool destination = false;           // M3: If this is Destination
    };

    //intersection vector
    vector<IntersectionInfo> intersection;

    /**
     * Struct for comparison
     */
    struct Compare{
        bool operator() (IntersectionInfo & a, IntersectionInfo & b){
            return a.heuristic >  b.heuristic;
        }
    };

    /**
     * Data Structure For Streets
     */
    struct StreetInfo {
        vector <IntersectionIdx> intersectionsOfStreet;
        unordered_set<IntersectionIdx> preLoadIntersections;
        double streetLength;
        unordered_set<StreetSegmentIdx> preLoadSegments;
        vector <StreetSegmentIdx> segmentsOfStreet;
        bool firstWrite = false;
    };

    vector<StreetInfo> street;

    /**
     * Data Structure For Street Segments
     */
    struct SegmentInfo{
        double segmentLength;
        double travelTime;
        double angle;
        vector<LatLon> segmentLATLONPosition;
        vector<ezgl::point2d> xyLocation;
        ezgl::point2d lastMidposition;
        bool highlight = false;
    };

    vector<SegmentInfo> segment;

    /**
     * Data Structure For Points of Interest+
     */
    struct POIInfo{
        string name;
        string type;
        LatLon latLonPosition;
        ezgl::point2d xyPosition;
        ezgl::rectangle poiRectangle;
        bool isShown = false;
        bool highlight = false;
    };

    vector<POIInfo> pointOfInt;

    /**
     * FindStreetIdsFromPartialStreetName
     * Street Name and Id
     */
    multimap<string, StreetIdx> streetNaId;

    /**
     * Data Structure For Features
     */
    struct FeatureInfo{
        string name;
        FeatureType type;
        vector <LatLon> latLonLocation;
        vector <ezgl::point2d> xyCoordinates;
        vector <ezgl::point2d> xyLocation;
        bool highlight = false;
    };

    vector <FeatureInfo> feature;

    multimap<double, FeatureIdx> featureOrder;

  /**
   * Path Output
   */
    unordered_map<StreetIdx, double> sortStreetLen;
    struct dir {
        bool noDirection = false;
        bool time = false;
        bool pathNote = false;
        bool yourLoc = false;
        bool turnLeft = false;
        bool turnRight = false;
        bool goStraight = false;
        bool end = false;
    };
    vector<dir> direction;
    vector<string> result;
};

#endif //MAPPER_MAPDBSTRUCT_H
