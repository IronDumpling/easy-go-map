/* 
 * Copyright 2021 University of Toronto
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
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <math.h>
#include <algorithm>
#include <iterator>
#include "m1.h"
#include "MapDBStruct.h"
#include "m3.h"
using namespace std;

/**
 * m1.cpp is the implementation document
 *
 * @param1.Load Helper section
 *         The load helper section helps load sorted data into corresponding structure in MapDBStruct.h file.
 *         It prepares results for function to return and accomplish speed requirements.
 *
 *         @param1.1 Global variable: Declare global variables as data structure for later functions
 *
 *         @param1.2 Load street information
 *                   @param1.2.1 loadStreet
 *                   @param1.2.2 sortStreet
 *
 *         @param1.3 Load segment length and segment travel time information: used to load every segment lengths in the
 *                   vector and load corresponding travel time it may take to walk through this segment
 *                   @param1.3.1 loadDistanceTime: Load segment length and segment travel time information
 *                   @param1.3.2 loadStreetLength: Load the street length information
 *
 *         @param1.4 Load intersection information
 *                   @param1.4.1 loadIntersection
 *
 *         @param1.5 Load spatial information
 *                   @param1.5.1 loadSpatial
 *
 *         @param1.6 Load street name information
 *                   @param1.6.1 modifier
 *                   @param1.6.2 loadNaId
 *
 *         @param1.7 Load feature information
 *                   @param1.7.1 loadFeature
 *
 *         @param1.8 load angle and midpoint for segment
 *                   @param1.8.1 loadAngleAndMidpoint
 *
 * @param2.Load map section
 *         @param2.1 loadMap: calling all helper functions to load data
 *
 *
 * @param3.Close map section : clean used data to prevemnt memory leak
 *         @param3.1 closeMap
 *
 *
 * @param4.Function implementation;
 *         @param4.1 findDistanceBetweenTwoPoints: The function calculates the distance between two LatLon type input,
 *                   it considers conversion from degree to radians, applying in different systems and math calculations.
 *
 *         @param4.2 findStreetSegmentLength: using the loaded data from helper loadDistanceTime, track the related
 *                   segment length and find the right position in the data structure to return segmentLength.
 *
 *         @param4.3 findStreetSegmentTravelTime: Same method as the function "findStreetSegmentLength", track the
 *                   required segment in the helper and return travelTime.
 *
 *         @param4.4 findClosestIntersection: Returns the LatLon of the closest intersection
 *
 *         @param4.5 findStreetSegmentsOfIntersection: Returns all the segments ID that connected to the intersection
 *         @param4.6 findStreetNamesOfIntersection: Returns all the street names of connected segments
 *         @param4.7 findAdjacentIntersections: Return all intersections that are next to this intersection
 *
 *         @param4.8 findIntersectionsOfStreet: Returns all intersections on certain street
 *         @param4.9 findIntersectionsOfTwoStreets: Returns common intersection from two streets
 *
 *         @param4.10 findStreetIdsFromPartialStreetName: Returns all the Street Id with the same prefix
 *
 *         @param4.11 findStreetLength:Using the loaded information from helper "loadStreetLength" and return right
 *         streetLength stored in StreetInfo struct
 *
 *         @param4.12 findStreetBoundingBox: return the smallest rectangle that contains all the intersection points
 *         and curve points, using the "updateHighestLowestLatLon" to help return strcut type and right value
 *                  @param4.12.1 updateHighestLowestLatLon: use if statements to find the max/min value and return them
 *
 *         @param4.13 findClosestPOI: Returns the LatLon of closest POI
 *
 *         @param4.14 findFeatureArea: Returns area only if the feature is a closed polygon
 */

/**
 * 1.1 Global Variable
 */
struct MapDBStruct mapDB;
double avgLat = 0;
double max_y = 0;
double min_y = 0;
double max_x = 0;
double min_x = 0;
float maxSpeedLimit = 0;

/**
 * Function Prototypes
 */
void loadStreet(StreetIdx, IntersectionIdx, StreetSegmentIdx);

void sortStreet();

void loadDistanceTime();

void loadStreetLength();

void loadIntersection();

void loadSpatial();
void loadNaId();
void loadFeature();
void loadAngleAndMidpoint();
void loadMaxSpeedLimit();

/**
 * 1.2.1 Load Street Information
 * Load intersectionId and streetSegmentId for each street
 * Use unordered_set to avoid duplicates
 */
void loadStreet(StreetIdx streetId, IntersectionIdx intersectionId, StreetSegmentIdx streetSegmentId){
    // Load intersectionId corresponding to each street
    mapDB.street[streetId].preLoadIntersections.insert(intersectionId);

    // Load streetSegmentId corresponding to each street
    mapDB.street[streetId].preLoadSegments.insert(streetSegmentId);
}

/**
 * 1.2.2 Sort Street Information
 * Convert unordered_set from 1.2.1 to vector
 * Prepare intersectionId for findIntersectionsOfStreet to return
 * Prepare streetSegmentId for findStreetLength to use
 * Sort intersectionId in ascending order for findIntersectionsOfTwoStreets to use
 */
void sortStreet(){
    const int STREET_NUMBER = getNumStreets();
    for (StreetIdx streetId = 0; streetId < STREET_NUMBER; ++streetId){
        // Convert unordered_set of intersectionID into vector
        mapDB.street[streetId].intersectionsOfStreet = vector<IntersectionIdx> (
                mapDB.street[streetId].preLoadIntersections.begin(), mapDB.street[streetId].preLoadIntersections.end());

        // Convert unordered_set of streetSegmentID into vector
        mapDB.street[streetId].segmentsOfStreet = vector<StreetSegmentIdx> (
                mapDB.street[streetId].preLoadSegments.begin(), mapDB.street[streetId].preLoadSegments.end());

        // Sort intersectionID vector in ascending order
        std::sort(mapDB.street[streetId].intersectionsOfStreet.begin(),
                  mapDB.street[streetId].intersectionsOfStreet.end());

        // Sort streetSegmentID vector in ascending order
        std::sort(mapDB.street[streetId].segmentsOfStreet.begin(),
                  mapDB.street[streetId].segmentsOfStreet.end());
    }
}


/**
 * 1.3 loadDistance
 * Load Segment length and segment travel time information
 */

void loadDistanceTime() {
    //create a vector to store the LatLon of every curve point
    //we assume the input is segment ID

    //initialize the segmentLength and TravelTime
    const int totalSegment = getNumStreetSegments();
    double segmentTotalLength;
    double segmentTravelTime;

    //find the LatLon of points curve point index0 and curve point currentNumCurvepoints
    LatLon firstCurvePointPosition;
    LatLon lastCurvePointPosition;

    for (int segmentIndex = 0; segmentIndex < totalSegment; ++segmentIndex) {
        StreetSegmentInfo segmentIndexInFor = getStreetSegmentInfo(segmentIndex);
        //bool onewayResult=segmentIndexInFor.oneWay;
        int currentNumCurvePoints = segmentIndexInFor.numCurvePoints;
        //directory here: mapDB.segment[segmentIndex]
        // since pointNum can range between 0 and StreetSegmentInfo.numCurvePoints-1,
        // I think the from and to point is not included

        //get the intersection ID of point from and to for this segment(for function segmentlength)
        IntersectionIdx fromID = segmentIndexInFor.from;
        IntersectionIdx toID = segmentIndexInFor.to;
        //float currentSpeedLimit=segmentIndexInFor.speedLimit;

        // size  = curvePoints + start + end
        mapDB.segment[segmentIndex].xyLocation.resize(currentNumCurvePoints+2);
        mapDB.segment[segmentIndex].segmentLATLONPosition.resize(currentNumCurvePoints+2);

        //LatLon position of from and to points
        LatLon fromPosition = getIntersectionPosition(fromID);
        LatLon toPosition = getIntersectionPosition(toID);

        //speed exceed the limit would be major road
        //float majorLimit=80/3.6;

        //for load data
        if(currentNumCurvePoints==0){
            // Convert to x, y coordinates
            mapDB.segment[segmentIndex].segmentLATLONPosition[0]=fromPosition;
            double xfrom=kEarthRadiusInMeters * kDegreeToRadian * cos(avgLat
                                                                      * kDegreeToRadian) * fromPosition.longitude();
            double yfrom=kEarthRadiusInMeters * kDegreeToRadian *
                         fromPosition.latitude();
            mapDB.segment[segmentIndex].xyLocation[0].x=xfrom;
            mapDB.segment[segmentIndex].xyLocation[0].y=yfrom;
            //from point x success load
            //load the to latlon position at the end
            double xto=kEarthRadiusInMeters * kDegreeToRadian
                       *cos(avgLat* kDegreeToRadian) * toPosition.longitude();
            double yto=kEarthRadiusInMeters * kDegreeToRadian *
                       toPosition.latitude();
            mapDB.segment[segmentIndex].segmentLATLONPosition[1]=toPosition;
            mapDB.segment[segmentIndex].xyLocation[1].x=xto;
            mapDB.segment[segmentIndex].xyLocation[1].y=yto;

        }else{

            //lat avg success
            //sort segments in the streets according to it's from to
            mapDB.segment[segmentIndex].segmentLATLONPosition[0]=fromPosition;
            double xfrom=kEarthRadiusInMeters * kDegreeToRadian
                         *cos(avgLat* kDegreeToRadian) * fromPosition.longitude();
            double yfrom=kEarthRadiusInMeters * kDegreeToRadian *
                         fromPosition.latitude();
            mapDB.segment[segmentIndex].xyLocation[0].x=xfrom;
            mapDB.segment[segmentIndex].xyLocation[0].y=yfrom;
            //load from x success
            //load xy coordinate
            for(int i=0;i<currentNumCurvePoints;i++){
                LatLon newCurvePointPosition = getStreetSegmentCurvePoint(segmentIndex,i);
                mapDB.segment[segmentIndex].segmentLATLONPosition[i+1]=newCurvePointPosition;
                // Convert to x, y coordinates
                double locationX = kEarthRadiusInMeters * kDegreeToRadian * cos(avgLat* kDegreeToRadian)
                        * newCurvePointPosition.longitude();
                double locationY = kEarthRadiusInMeters * kDegreeToRadian * newCurvePointPosition.latitude();
                mapDB.segment[segmentIndex].xyLocation[i+1].x=locationX;
                mapDB.segment[segmentIndex].xyLocation[i+1].y=locationY;
            }

            //load the to latlon position at the end
            double xto=kEarthRadiusInMeters * kDegreeToRadian
                       *cos(avgLat* kDegreeToRadian) * toPosition.longitude();
            double yto=kEarthRadiusInMeters * kDegreeToRadian *
                       toPosition.latitude();
            mapDB.segment[segmentIndex].segmentLATLONPosition[currentNumCurvePoints+1]=toPosition;
            mapDB.segment[segmentIndex].xyLocation[currentNumCurvePoints+1].x=xto;
            mapDB.segment[segmentIndex].xyLocation[currentNumCurvePoints+1].y=yto;
        }
        //for function findStreetSegmentTravelTime,I want to get the speedlimit of every segment
        float currentSegmentSpeedLimit=segmentIndexInFor.speedLimit;

        if(currentNumCurvePoints==0){
            //as we move into this condition, segmentTotoalLength is 0
            segmentTotalLength=0;
            //initialize the travel time
            segmentTravelTime=0;

            segmentTotalLength=findDistanceBetweenTwoPoints(make_pair(fromPosition,toPosition));
            mapDB.segment[segmentIndex].segmentLength=segmentTotalLength;

            //load the travel time into the structure
            segmentTravelTime=segmentTotalLength/currentSegmentSpeedLimit;
            mapDB.segment[segmentIndex].travelTime=segmentTravelTime;
        }else {
            //as we move into this condition, segmentTotoalLength is 0
            segmentTotalLength=0;
            segmentTravelTime=0;

            firstCurvePointPosition = getStreetSegmentCurvePoint(segmentIndex, 0);
            lastCurvePointPosition = getStreetSegmentCurvePoint(segmentIndex, currentNumCurvePoints-1);

            segmentTotalLength += findDistanceBetweenTwoPoints(make_pair(fromPosition,firstCurvePointPosition));
            segmentTotalLength += findDistanceBetweenTwoPoints(make_pair(lastCurvePointPosition,toPosition));

            for (int curvePointIndex = 0; curvePointIndex < currentNumCurvePoints - 1; ++curvePointIndex) {
                //the loop is used to store the current curve point position into the vector
                LatLon currentCurvePointPosition = getStreetSegmentCurvePoint(segmentIndex, curvePointIndex);
                LatLon nextCurvePosition = getStreetSegmentCurvePoint(segmentIndex, (curvePointIndex + 1));
                //load in position here, set will avoid duplicate

                //since the curve point is arranged in order
                // find the segment distance between the curve points(except the from point 0
                // to point 1 and last curve point to to point)
                segmentTotalLength += findDistanceBetweenTwoPoints
                        (make_pair(currentCurvePointPosition,nextCurvePosition));
            }
            // after goes out of the loop, we should get the length between curve points
            //load the travel time into the structure
            segmentTravelTime=segmentTotalLength/currentSegmentSpeedLimit;
            mapDB.segment[segmentIndex].travelTime = segmentTravelTime;
            mapDB.segment[segmentIndex].segmentLength = segmentTotalLength;

        }
    }
}
//need to load segment according to street id


/**
 * 1.4.1 Load Street Length Information
 */

void loadStreetLength(){
    //given street ID, we should figure out the street segments in the  street firstly
    int totalNumOfStreet=getNumStreets();
    double totalStreetLength;
    for(int streetCurrentIndex = 0; streetCurrentIndex < totalNumOfStreet; streetCurrentIndex++){
        //load the total street segments in the current street, the vector type
        //get the number of total segments in the street
        //clear the total segment length after it done the loop once
        totalStreetLength=0;
        //number of segments in the street
        int totalNumOfSegments=mapDB.street[streetCurrentIndex].segmentsOfStreet.size();
        for(int segmentCurrentIndex=0;segmentCurrentIndex<totalNumOfSegments;segmentCurrentIndex++) {
            //totalStreetLength+=segmentCurrentIndex;(test show can enter the loop)
            //plus every segment in the street
            //get the segment ID here
            totalStreetLength += mapDB.segment[mapDB.street[streetCurrentIndex].segmentsOfStreet[segmentCurrentIndex]].
                    segmentLength;
        }
        //final result should be stored into
        mapDB.street[streetCurrentIndex].streetLength = totalStreetLength;
    }
}


/**
 * 1.5 Load Intersection Information
 * 1. Load intersectionId prepared for findAdjacentIntersection
 * 2. Load SegmentId connected with this intersection for StreetSegmentOfIntersection
 * 3. Load Street Names connected to this intersection for StreetNamesOfIntersection
 * 4. Call loadStreet function to prepare street information
 */
void loadIntersection(){
    //Initialise IntersectionIdx
    const int INTERSECTION_NUMBER = getNumIntersections();

    //Traverse each intersection to find street segments that came from this intersection
    for(IntersectionIdx intersectionId = 0; intersectionId < INTERSECTION_NUMBER; ++intersectionId){

        // Load intersectionID into the struct
        mapDB.intersection[intersectionId].interID = intersectionId;

        // Load for Closest Intersection
        LatLon location = getIntersectionPosition(intersectionId);
        mapDB.intersection[intersectionId].latLonLocation = location;

        // Convert latitude and longitude into x, y coordinates
        double locationX = location.longitude() * kEarthRadiusInMeters * kDegreeToRadian * cos(avgLat * kDegreeToRadian);
        double locationY = kEarthRadiusInMeters * kDegreeToRadian * location.latitude();

        // Store x, y coordinates into data structure xyLocation
        mapDB.intersection[intersectionId].xyLocation.x = locationX;
        mapDB.intersection[intersectionId].xyLocation.y = locationY;

        // Store intersection name
        mapDB.intersection[intersectionId].intersectionName = getIntersectionName(intersectionId);

        // As we enter the loop as we are trying to build in information for the intersection struct
        int streetSegmentNumber = getNumIntersectionStreetSegment(intersectionId);

        //Traverse each StreetSegment connected with this intersection
        for(int segmentNumber = 0; segmentNumber < streetSegmentNumber; ++segmentNumber) {

            //Find streetSegmentId from its StreetSegmentInfo Struct for 1.2
            StreetSegmentIdx streetSegmentId = getIntersectionStreetSegment(intersectionId, segmentNumber);

            //struct 1.2 Load segmentIdOfIntersection into intersection data structure
            mapDB.intersection[intersectionId].segmentId.emplace_back(streetSegmentId);

            // Find its streetId from its StreetSegmentInfo Struct for 1.3
            // Find street name of this street segment
            StreetIdx streetId = (getStreetSegmentInfo(streetSegmentId)).streetID;
            string streetNames = getStreetName(streetId);

            //struct 1.3 Load streetNames into a temporary unordered_set
            mapDB.intersection[intersectionId].streetName.emplace_back(streetNames);

            //struct 2.1 Load intersectionId into StreetInfo data structure
            //struct 2.3 Load segmentIdOfStreet into StreetInfo data Structure
            loadStreet(streetId, intersectionId, streetSegmentId);
        }
    }
}

/**
 * 1.5 Load Spatial Information
 */
void loadSpatial(){
    // Load for Closest POI
    const int POI_NUMBER = getNumPointsOfInterest();

    //traverse POI struct
    for(POIIdx POIId = 0; POIId < POI_NUMBER; ++POIId){
        //1.5.1 load name
        mapDB.pointOfInt[POIId].name = getPOIName(POIId);

        //1.5.2 load type
        mapDB.pointOfInt[POIId].type = getPOIType(POIId);

        //1.5.3 latLon Position
        mapDB.pointOfInt[POIId].latLonPosition = getPOIPosition(POIId);

        //1.5.4 xy Position
        mapDB.pointOfInt[POIId].xyPosition.x = (getPOIPosition(POIId).longitude()) * kEarthRadiusInMeters *
                                               kDegreeToRadian * cos(avgLat * kDegreeToRadian);
        mapDB.pointOfInt[POIId].xyPosition.y = kEarthRadiusInMeters * kDegreeToRadian * (getPOIPosition(POIId).latitude());

        //1.5.5 POI rectangle
        ezgl::point2d bottom_left(mapDB.pointOfInt[POIId].xyPosition.x-2000,
                                  mapDB.pointOfInt[POIId].xyPosition.y-2000);
        ezgl::point2d top_right(mapDB.pointOfInt[POIId].xyPosition.x+2000,
                                mapDB.pointOfInt[POIId].xyPosition.y+2000);

        ezgl::rectangle poi(bottom_left, top_right);

        mapDB.pointOfInt[POIId].poiRectangle = poi;
    }
}

/**
 * 1.6.1 modifier helper
 */
extern string modifier(string & input){
    //erase all the spaces
    input.erase(remove(input.begin(), input.end(), ' '), input.end());

    //transform all letters into lower cases
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

/**
 * 1.6.2 loadNaId (Name and street ID)
 * load multiMap<string, streetIdx>>
 * For findStreetIdxOfPartialName
 */

void loadNaId(){
    //initialise
    const int STREET_NUMBER = getNumStreets();

    for(int streetId = 0; streetId < STREET_NUMBER; ++streetId){
        //get and modify the street name
        string temp = getStreetName(streetId);
        string streetName = modifier(temp);

        //load multiMap
        mapDB.streetNaId.emplace(make_pair(streetName,streetId));
    }
}

/**
 * 1.7 loadFeature
 * load vector<FeatureInfo>
 * For findFeatureArea
 */

void loadFeature(){
    const int FEATURE_NUMBER = getNumFeatures();

    for (FeatureIdx featureId = 0; featureId < FEATURE_NUMBER; ++featureId){
        // Get the number of points for each feature
        double latTot = 0;
        int pointNum = getNumFeaturePoints(featureId);
        mapDB.feature[featureId].latLonLocation.resize(pointNum);
        mapDB.feature[featureId].xyCoordinates.resize(pointNum);
        mapDB.feature[featureId].xyLocation.resize(pointNum);

        if ((getFeatureName(featureId) == "<noname>") || (getFeatureName(featureId) == "<unspecified>")){
            mapDB.feature[featureId].name = "0";
        } else {
            mapDB.feature[featureId].name = getFeatureName(featureId);
        }
        mapDB.feature[featureId].type = getFeatureType(featureId);

        // Calculate average latitude in radian
        for (int num = 0; num < pointNum; ++num) {
            latTot += getFeaturePoint(featureId, num).latitude();
        }
        double latAvg = kDegreeToRadian * latTot / pointNum;

        for (int num = 0; num < pointNum; ++num) {
            LatLon location = getFeaturePoint(featureId, num);

            // Store latitude and longitude into data structure latLonLocation
            mapDB.feature[featureId].latLonLocation[num] = location;

            // Convert to x, y coordinates
            double coordinateX = kEarthRadiusInMeters * kDegreeToRadian * cos(latAvg) * location.longitude();
            double coordinateY = kEarthRadiusInMeters * kDegreeToRadian * location.latitude();

            // Store x, y coordinates into data structure xyLocation
            mapDB.feature[featureId].xyCoordinates[num].x = coordinateX;
            mapDB.feature[featureId].xyCoordinates[num].y = coordinateY;

            double locationX = kEarthRadiusInMeters * kDegreeToRadian * cos(avgLat * kDegreeToRadian) * location.longitude();
            double locationY = kEarthRadiusInMeters * kDegreeToRadian * location.latitude();

            // Store x, y position into data structure xyLocation
            mapDB.feature[featureId].xyLocation[num].x = locationX;
            mapDB.feature[featureId].xyLocation[num].y = locationY;
        }
    }

    // Prepare feature order
    for (FeatureIdx featureId = 0; featureId < FEATURE_NUMBER; ++featureId){
        mapDB.featureOrder.emplace(make_pair(findFeatureArea(featureId), featureId));
    }
}

/** 1.8 loadAngleAndMidpoint
 *  to store needed information for draw street name
 *  keep angle and midpoint of every curvepoint
 */
void loadAngleAndMidpoint() {
    //load every segment angle
    for (StreetSegmentIdx segmentIdx = 0; segmentIdx < mapDB.segment.size(); segmentIdx++) {

        //mapDB.segment[segmentIdx].xyLocation[]
        StreetSegmentInfo currentStruct =
                getStreetSegmentInfo(segmentIdx);
        int numOfCurvePoints = currentStruct.numCurvePoints;

        double fromY = mapDB.segment[segmentIdx].xyLocation[0].y;
        double fromX = mapDB.segment[segmentIdx].xyLocation[0].x;
        double toY = mapDB.segment[segmentIdx].xyLocation[mapDB.segment[segmentIdx].xyLocation.size() - 1].y;
        double toX = mapDB.segment[segmentIdx].xyLocation[mapDB.segment[segmentIdx].xyLocation.size() - 1].x;

        if (numOfCurvePoints == 0) {
            double xAvg = (fromX + toX) / 2;
            double yAvg = (fromY + toY) / 2;

            mapDB.segment[segmentIdx].lastMidposition.x = xAvg;
            mapDB.segment[segmentIdx].lastMidposition.y = yAvg;
        } else {
            int middlePointPosition = (numOfCurvePoints + 2) / 2;

            mapDB.segment[segmentIdx].lastMidposition.x = mapDB.segment[segmentIdx].xyLocation[middlePointPosition].x;
            mapDB.segment[segmentIdx].lastMidposition.y = mapDB.segment[segmentIdx].xyLocation[middlePointPosition].y;
        }

        double angle = atan2((toY - fromY), (toX - fromX)) / kDegreeToRadian;
        if (angle < 0) {
            angle += 360;
        }
        mapDB.segment[segmentIdx].angle = angle;
    }
}


/**
 * 2.15 load path and minimum time from algorithm
 * @param intersect_id_start
 * @param intersect_id_destination
 * @param turn_penalty
 */
void loadMaxSpeedLimit(){
    float maxspeedLimit = 0;
    for(int segmentID = 0; segmentID < getNumStreetSegments(); ++segmentID){
            float speedLimit = getStreetSegmentInfo(segmentID).speedLimit;
            maxspeedLimit = std::max(maxspeedLimit, speedLimit);
    }
    maxSpeedLimit = maxspeedLimit;
}


/**
 * 2.1 loadMap
 * will be called with the name of the file that stores the "layer-2"
 * map data accessed through StreetsDatabaseAPI: the street and intersection
 * data that is higher-level than the raw OSM data).
 * This file name will always end in ".streets.bin" and you
 * can call loadStreetsDatabaseBIN with this filename to initialize the
 * layer 2 (StreetsDatabase) API.
 * If you need data from the lower level, layer 1, API that provides raw OSM
 * data (nodes, ways, etc.) you will also need to initialize the layer 1
 * OSMDatabaseAPI by calling loadOSMDatabaseBIN. That function needs the
 * name of the ".osm.bin" file that matches your map -- just change
 * ".streets" to ".osm" in the map_streets_database_filename to get the proper
 * name.
 */
bool loadMap(std::string map_streets_database_filename) {
    bool load_successful = false; // Indicates whether the map has loaded
                                  // successfully

    std::cout << "loadMap: " << map_streets_database_filename << std::endl;

    load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    if(!load_successful) {
        return false;
    }

    // Load your map related data structures here.
    // Constant variables
    const int INTERSECTION_NUMBER = getNumIntersections();
    const int SEGMENT_NUMBER = getNumStreetSegments();
    const int STREET_NUMBER = getNumStreets();
    const int POI_NUMBER = getNumPointsOfInterest();
    const int FEATURE_NUMBER = getNumFeatures();

    // Resize each container in data structure
    mapDB.intersection.resize(INTERSECTION_NUMBER);
    mapDB.segment.resize(SEGMENT_NUMBER);
    mapDB.street.resize(STREET_NUMBER);
    mapDB.pointOfInt.resize(POI_NUMBER);
    mapDB.feature.resize(FEATURE_NUMBER);

    //calculate average latitude
    double max_lat = getIntersectionPosition(0).latitude();
    double min_lat = max_lat;
    double max_lon = getIntersectionPosition(0).longitude();
    double min_lon = max_lon;

    for (int id = 0; id < getNumIntersections(); ++id) {
        max_lat = std::max(max_lat, getIntersectionPosition(id).latitude());
        min_lat = std::min(min_lat, getIntersectionPosition(id).latitude());
        max_lon = std::max(max_lon, getIntersectionPosition(id).longitude());
        min_lon = std::min(min_lon, getIntersectionPosition(id).longitude());
    }

    //calculate average latitude
    avgLat = (min_lat + max_lat) / 2.0;

    // Intersection load helper function
    // Call loadStreet inside
    loadIntersection();

    // Street sort helper function
    sortStreet();

    // Distance and time load helper function
    loadDistanceTime();

    // Street length load helper function
    loadStreetLength();

    // Spatial load helper function
    loadSpatial();

    // Find StreetId for Partial name
    loadNaId();

    loadFeature();

    loadAngleAndMidpoint();

    loadMaxSpeedLimit();

    mapDB.result.clear();
    // cout << "Result cleared\n";
    mapDB.direction.clear();
    // cout << "Direction cleared\n";

    //For Loop 1: Each Segment Curve Point
    max_y = mapDB.intersection[0].xyLocation.y;
    min_y = max_y;
    max_x = mapDB.intersection[0].xyLocation.x;
    min_x = max_x;

    for (int id = 0; id < getNumStreetSegments(); ++id) {
        for (int num = 0; num < mapDB.segment[id].xyLocation.size(); ++num) {
            max_y = std::max(max_y, mapDB.segment[id].xyLocation[num].y);
            min_y = std::min(min_y, mapDB.segment[id].xyLocation[num].y);
            max_x = std::max(max_x, mapDB.segment[id].xyLocation[num].x);
            min_x = std::min(min_x, mapDB.segment[id].xyLocation[num].x);
        }
    }

    //For Loop 2: Each Feature Point
    for(int id = 0; id < getNumFeatures(); ++id) {
        for(int num = 0; num < mapDB.feature[id].xyLocation.size(); ++num) {
            max_y = std::max(max_y, mapDB.feature[id].xyLocation[num].y);
            min_y = std::min(min_y, mapDB.feature[id].xyLocation[num].y);
            max_x = std::max(max_x, mapDB.feature[id].xyLocation[num].x);
            min_x = std::min(min_x, mapDB.feature[id].xyLocation[num].x);
        }
    }

    return true;
}

/**
 * 3.1 closeMap
 */
void closeMap() {
    // Clean-up your map related data structures here
    closeStreetDatabase();
    mapDB.intersection.clear();
    mapDB.street.clear();
    mapDB.segment.clear();
    mapDB.pointOfInt.clear();
    mapDB.streetNaId.clear();
    mapDB.feature.clear();
    mapDB.featureOrder.clear();
    max_y = 0;
    min_y = 0;
    max_x = 0;
    min_x = 0;
    avgLat = 0;
}

/**
 * 4.1 findDistanceBetweenTwoPoints
 * Returns the distance between two (lattitude, longitude) coordinates in meters
 * Speed Requirement --> moderate
 */
double findDistanceBetweenTwoPoints(std::pair<LatLon, LatLon> points){
    const double x_diff = (points.second.longitude()-points.first.longitude())*
                    cos(
                    ((points.first.latitude()+points.second.latitude())/2)*kDegreeToRadian
                    )*kDegreeToRadian;
    const double y_diff = (points.second.latitude() - points.first.latitude())*kDegreeToRadian;
    return kEarthRadiusInMeters*sqrt(y_diff*y_diff+x_diff*x_diff);
}

/**
 * 4.2 findStreetSegmentLength
 * Returns the length of the given street segment in meters
 * Speed Requirement --> moderate
 */
double findStreetSegmentLength(StreetSegmentIdx street_segment_id){
   return mapDB.segment[street_segment_id].segmentLength;
}

/**
 * 4.3 findStreetSegmentTravelTime
 * Returns the travel time to drive from one end of a street segment in
 * to the other, in seconds, when driving at the speed limit
 * Note: (time = distance/speed_limit)
 * Speed Requirement --> high
 */
double findStreetSegmentTravelTime(StreetSegmentIdx street_segment_id){
    return mapDB.segment[street_segment_id].travelTime;
}

/**
 * 4.4 findClosestIntersection
 * Returns the nearest intersection to the given position
 * Speed Requirement --> none
 */
IntersectionIdx findClosestIntersection (LatLon my_position){
    const int INTERSECTION_NUMBER = getNumIntersections();
    map<double, int, less<double>> distance;
    for(int intersectionId = 0; intersectionId < INTERSECTION_NUMBER; ++intersectionId) {
        double temp = findDistanceBetweenTwoPoints(std::make_pair(my_position, mapDB.intersection[intersectionId].latLonLocation));
        distance.insert(std::make_pair(temp,intersectionId));
    }
    auto output = distance.begin();
    return output->second;
}

/**
 * 4.5 findStreetSegmentsOfIntersection
 * Returns the street segments that connect to the given intersection
 * Speed Requirement --> high
 */
std::vector<StreetSegmentIdx> findStreetSegmentsOfIntersection(IntersectionIdx intersection_id){
    return mapDB.intersection[intersection_id].segmentId;
}

/**
 * 4.6 findStreetNames of Intersection
 * @param intersection_id
 * @return vector<string>
 */
std::vector<std::string> findStreetNamesOfIntersection(IntersectionIdx intersection_id){
    return mapDB.intersection[intersection_id].streetName;
}

/**
 * 4.7 findAdjacentIntersections
 * Returns all intersections reachable by traveling down one street segment
 * from the given intersection (hint: you can't travel the wrong way on a 1-way street)
 * the returned vector should NOT contain duplicate intersections
 * Speed Requirement --> high
 */
std::vector<IntersectionIdx> findAdjacentIntersections(IntersectionIdx intersection_id) {
    vector<IntersectionIdx> intersectionTo;
    for (int index = 0; index < getNumIntersectionStreetSegment(intersection_id); ++index) {
        IntersectionIdx fromPoint = getStreetSegmentInfo((mapDB.intersection[intersection_id]).segmentId[index]).from;
        IntersectionIdx toPoint = getStreetSegmentInfo((mapDB.intersection[intersection_id]).segmentId[index]).to;

        if (intersection_id == fromPoint) {
            auto iter1 = std::find(intersectionTo.begin(), intersectionTo.end(), toPoint);
            if (iter1 == intersectionTo.end())
                intersectionTo.push_back(toPoint);
        } else if (!getStreetSegmentInfo((mapDB.intersection[intersection_id]).segmentId[index]).oneWay) {
            auto iter2 = std::find(intersectionTo.begin(), intersectionTo.end(), fromPoint);
            if (iter2 == intersectionTo.end())
                intersectionTo.push_back(fromPoint);
        }
    }
    return intersectionTo;
}

/**
 * 4.8 findIntersectionsOfStreet
 * Returns all intersections along the a given street
 * Speed Requirement --> high
 */
std::vector<IntersectionIdx> findIntersectionsOfStreet(StreetIdx street_id){
    return mapDB.street[street_id].intersectionsOfStreet;
}

/**
 * 4.9 findIntersectionsOfTwoStreets
 * Return all intersection ids at which the two given streets intersect
 * This function will typically return one intersection id for streets
 * that intersect and a length 0 vector for streets that do not. For unusual
 * curved streets it is possible to have more than one intersection at which
 * two streets cross
 * Speed Requirement --> high
 */
std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(std::pair<StreetIdx, StreetIdx> street_ids){
    vector<IntersectionIdx> commonIntersections;

    // Find common intersections from intersections data structure of two streets
    std::set_intersection(mapDB.street[street_ids.first].intersectionsOfStreet.begin(),
                          mapDB.street[street_ids.first].intersectionsOfStreet.end(),
                          mapDB.street[street_ids.second].intersectionsOfStreet.begin(),
                          mapDB.street[street_ids.second].intersectionsOfStreet.end(),
                          std::back_inserter(commonIntersections));
    return commonIntersections;
}

/**
 * 4.10 findStreetIdsFromPartialStreetName
 * Returns all street ids corresponding to street names that start with the
 * given prefix
 * The function should be case-insensitive to the street prefix.
 * The function should ignore spaces.
 * For example, both "bloor " and "BloOrst" are prefixes to
 * "Bloor Street East".
 * If no street names match the given prefix, this routine returns an empty
 * (length 0) vector.
 * You can choose what to return if the street prefix passed in is an empty
 * (length 0) string, but your program must not crash if street_prefix is a
 * length 0 string.
 * Speed Requirement --> high
 */
std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix){
    vector<StreetIdx> output(0);

    //modify the string
    string preFix = modifier(street_prefix);

    //if enter an empty string
    if(preFix.size() == 0) {
        return output;
    }else{

        //find upper bound prefix
        string prefixPlus = preFix;
        prefixPlus[preFix.size()-1]++;

        //find lower bound and upper bound of the prefix
        auto low = mapDB.streetNaId.lower_bound(preFix);
        auto up = mapDB.streetNaId.lower_bound(prefixPlus);

        //find the specific bound of the element
        for(auto iter = low; iter != up; ++iter){
            output.emplace_back(iter->second);
        }
        return output;
    }
}

/**
 * 4.11 findStreetLength
 * Returns the length of a given street in meters
 * Speed Requirement --> high
 */
double findStreetLength(StreetIdx street_id){
    return mapDB.street[street_id].streetLength;
}

/**
 * 4.12.1 updateHighestLowestLatLon function
 * Assign the value of highest/lowest latitude and longtitude into the function
 * initialize a value of a latitude and longtitude for each variable in the function
 * as it enters the function, this helper function will help us update the corresponding value
 * until we goes out the function
 * */
static void updateHighestLowestLatLon(
        const LatLon &point,
        double &highestLat,
        double &highestLong,
        double &lowestLat,
        double &lowestLong) {
    if (point.latitude() > highestLat) {
        highestLat = point.latitude();
    }
    if (point.longitude() > highestLong) {
        highestLong = point.longitude();
    }
    if (point.latitude() < lowestLat) {
        lowestLat = point.latitude();
    }
    if (point.longitude() < lowestLong) {
        lowestLong = point.longitude();
    }
}

/**
 * 4.12 findStreetBoundingBox
 * Return the smallest rectangle that contains all the intersections and
 * curve points of the given street (i.e. the min,max lattitude
 * and longitude bounds that can just contain all points of the street).
 * peed Requirement --> none
 */
LatLonBounds findStreetBoundingBox(StreetIdx street_id) {
    //the result of all intersections in the street
    const auto &segmentsOfStreet = mapDB.street[street_id].segmentsOfStreet;

    double highestLat = std::numeric_limits<double>::lowest();
    double highestLong = std::numeric_limits<double>::lowest();
    double lowestLat = std::numeric_limits<double>::max();
    double lowestLong = std::numeric_limits<double>::max();

    for (int segmentsOfStreetIdx = 0; segmentsOfStreetIdx < segmentsOfStreet.size(); segmentsOfStreetIdx++) {
        const StreetSegmentInfo segInfo = getStreetSegmentInfo(segmentsOfStreet[segmentsOfStreetIdx]);

        updateHighestLowestLatLon(getIntersectionPosition(segInfo.from), highestLat,
                                  highestLong, lowestLat,lowestLong);
        updateHighestLowestLatLon(getIntersectionPosition(segInfo.to), highestLat,
                                  highestLong, lowestLat, lowestLong);

        //find the most point in intersections
        const int &numCurvePoints = segInfo.numCurvePoints;
        for (int curvePointIdx = 0; curvePointIdx < numCurvePoints; curvePointIdx++) {
            LatLon curvePoint = getStreetSegmentCurvePoint(segmentsOfStreet[segmentsOfStreetIdx],curvePointIdx);
            updateHighestLowestLatLon(curvePoint, highestLat, highestLong, lowestLat, lowestLong);
        }
    }
    //as we enter here, we should get all needed points
    return {
            LatLon(lowestLat, lowestLong),
            LatLon(highestLat, highestLong)
    };

}

/**
 * 4.13 findClosestPOI
 * Returns the nearest point of interest of the given name to the given position
 * Speed Requirement --> none
 */
POIIdx findClosestPOI(LatLon my_position, std::string POIname){
    map<double, int, less<double>> distance;

    const int POI_NUMBER = getNumPointsOfInterest();

    for(int POIId = 0; POIId < POI_NUMBER; ++POIId){
        if(POIname.compare(mapDB.pointOfInt[POIId].name) == 0) {
            double temp = findDistanceBetweenTwoPoints(make_pair(my_position,mapDB.pointOfInt[POIId].latLonPosition));
            distance.insert(make_pair(temp, POIId));
        }
    }

    auto output = distance.begin();
    return output->second;
}

/**
 * 4.14 findFeatureArea
 * returns the area of the given closed feature in square meters
 * Assume a non self-intersecting polygon (i.e. no holes)
 * Return 0 if this feature is not a closed polygon.
 * Speed Requirement --> moderate
 */
double findFeatureArea(FeatureIdx feature_id){
    double area = 0;
    int pointNum = mapDB.feature[feature_id].latLonLocation.size();

    // Check whether it is a polygon or polyline
    LatLon start = mapDB.feature[feature_id].latLonLocation[0];
    LatLon end = mapDB.feature[feature_id].latLonLocation[pointNum - 1];

    // Calculate area only if it is a closed polygon, otherwise return 0
    if(!(start == end)){
        return 0;
    } else {

        // Calculate polygon area using formula: sum of (Y(i+1) - Yi) * (X(i+1) + Xi) / 2
        for (int num = 1; num < pointNum; ++num) {

            // Convert to x, y coordinates for last point and current point
            double lastX = mapDB.feature[feature_id].xyCoordinates[num - 1].x;
            double lastY = mapDB.feature[feature_id].xyCoordinates[num - 1].y;
            double currentX = mapDB.feature[feature_id].xyCoordinates[num].x;
            double currentY = mapDB.feature[feature_id].xyCoordinates[num].y;
            area += (currentY - lastY) * (currentX + lastX) / 2;
        }
    }

    // If area is negative, return positive value
    if (area < 0){
        return -area;
    } else {
        return area;
    }
}