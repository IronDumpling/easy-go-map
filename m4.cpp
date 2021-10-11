//
// Created by zhan7417 on 2021-04-09.
//
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

//
// Created by zhan7417 on 2021-04-09.
//
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

/**
 * Include library Files
 */
#include "m1.h"
#include "m3.h"
#include "MapDBStruct.h"
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <random>
#include <iostream>

#define TIME_CONSTRAINT 50

/**
 * m4 is the algorithm cpp file
 *
 * @param1 Global Structs: the section used to stored needed precalculated and new information
 *         @param1.1 nodeType: the numeration type to identify the types of every node
 *         @param1.2 candidatePath: it's struct of each pre-calculated path
 *                   used to store information for every segment path
 *         @param1.3 Compare is the struct to compare which travel time is lesser for two candidate travel time's value
 *         @param1.4 deliveryCondition: it is the struct of each delivery condition
 *                   to check if every points has been visited and if we are on the way to delivery
 *         @param1.5 DeliveryInf: it is the struct to store the pickup and drop off intersection id of the delivery
 *         @param1.6 CourierSubPath: it is the struct to store from and to point for the particular path, and
 *                   how is the path vector return from m3 algorithm
 *
 * @param2 Global Variables: store information in the current file to convenient people use
 *         @param2.1 nextDeliveryId is the intersection point that delivery will go next
 *         @param2.2 depotStart is the vector to store the path from depoit to the first pickup point
 *         @param2.3 pickUpStart is the vector to store the legal path for possible start points
 *         @param2.4 dropOffStart is the vector to store the legal path for possible end points
 *         @param2.5 deliveryConditionVector is the vector to tore the DeliveryInf to check legal path
 *         @param2.6 courier is the vector to store the struct of couriers' paths to store every delivery from and
 *                   to id and the path length
 *
 *
 * @param3 Function Prototypes: prototypes for main functions
 *          @param3.1 travelingCourier
 *          @param3.2candidatePathLoad(const std::vector<DeliveryInf>&, const std::vector<int>&, float);
 *          @param3.3 depotStartPath(int, IntersectionIdx, const std::vector<DeliveryInf>&, float);
 *          @param3.4 pickUpStartPath(int, IntersectionIdx, const std::vector<DeliveryInf> &, float);
 *          @param3.5pickUpStartPathHelper(int, int);
 *          @param3.6 dropOffStartPath(int, IntersectionIdx, const std::vector<DeliveryInf> &, const std::vector<int> &, float);
 *          @param3.7 dropOffStartPathHelper(int, int, bool);
 *          @param3.8 depotMultiPath(vector<MapDBStruct::IntersectionInfo> &, IntersectionIdx, const std::vector<DeliveryInf>&, float);
 *          @param3.9 pickUpMultiPath(vector<MapDBStruct::IntersectionInfo> &, IntersectionIdx, const std::vector<DeliveryInf>&, float);
 *          @param3.10 dropOffMultiPath(vector<MapDBStruct::IntersectionInfo> &, IntersectionIdx, const std::vector<DeliveryInf>&, const std::vector<int>&, float);
 *          @param3.11 initialiseSearch(vector<MapDBStruct::IntersectionInfo>*, IntersectionIdx);
 *          @param3.12 backTracking(vector<MapDBStruct::IntersectionInfo> &, IntersectionIdx, IntersectionIdx);
 *          @param3.13 greedyCourierFinder();
 *
 * @param4 Main Function, has alrady implremented the multithreading
 *         @param4.1 Traveling Courier: store newest information to return
 *         @param4.2 candidatePathLoad: store all vectors of Candidate Path
 *         @param4.3 Multiple Destination Path Finder: to store the possible path between depots points
 *         and first/end interstion id
 *         @param4.4 Depot Start Candidate Queue Load:traverse between depots and all pick up points, update the path's
 *         related from, to node and time. Update the smaller time for the depots
 *         @param4.5 Multiple Destination Path Finder: load possible path for pick up and drop-off points for delivery
 *         @param4.6 Multiple Path Finder for Pick Up start Points:sort multi-path for the pickup points and update
 *         smaller time that paths takes, problem updated
 *         @param4.7 Multiple Destination Path Finder: update paths for drop-off and depots/pick up points
 *         @param4.8 Find Multi Destination Path for Drop Off Start Node: start the path from drop-off node,update the
 *         smaller path and heuristic
 *         @param4.9 initialiseSearch: initialise for Each Multi Destination Search
 *         @param4.10 Back Tracking Path between two points:  avoid miss paths
 *         @param4.11 Pick Up Start Path Helper: find the second smaller time the path take and get information
 *         for pick-up points
 *         @param4.12 Drop Off Start Path Helper: find the second smaller time the path take and get information
 *         for drop-off points
 *         @param4.13 Find Proper Courier: using greedy algorithm to update better legal path
 *         @param4.14 twoOptimization: two opt algorithm implementation
 *         @param4.15 travelTimeCalculator : calculate the travel time for every path
 *         @param4.16 legalityChecker:check legality of the path
 *
 */

/**
 * 1 Global Structs
 */
// Types of each node
enum nodeType {
    unknown,
    pickUp = 1, // Delivery Pick UP point, means +1
    dropOff = -1, // Delivery Drop off point, means -1
    depot = 0 // Depots, means go home
};

// Struct of each pre-calculated path, used to store information for every segment path
struct candidatePath{
    IntersectionIdx fromNode = -1;
    IntersectionIdx toNode = -1;

    nodeType fromNodeType = unknown;
    std::vector<nodeType> toNodeType;

    std::vector<int> toNodeDeliveryId;

    double travelTime = std::numeric_limits<double>::max();
    std::vector<StreetSegmentIdx> path;
};

// Struct of each delivery condition
struct deliveryCondition{
    bool pickUpVisited = false;
    bool dropOffVisited = false;
    bool onTruck = false;
};

// Struct of delivery order
struct DeliveryOrder{
    IntersectionIdx nodeId;
    double travelTime = -1;
    std::vector<int> deliveryId;
    std::vector<nodeType> deliveryType;
};

// Struct of path information for 2 opt
struct TwoOptPath{
    std::vector<StreetSegmentIdx> keyYToKeyXPath;
    double keyYToKeyXTime;
};

// Struct of each delivery
struct DeliveryInf {
    //Specifies a delivery order (input to m3 algorithm).
    //To satisfy the order the item-to-be-delivered must have been picked-up before
    //from the pickUp intersection should occur before visiting the dropOff intersection.
    DeliveryInf(int pick_up, int drop_off)
            : pickUp(pick_up), dropOff(drop_off) {}

    //The intersection id where the item-to-be-delivered should be picked-up.
    IntersectionIdx pickUp;

    //The intersection id where the item-to-be-delivered should be dropped-off.
    IntersectionIdx dropOff;
};

// Struct of return Subpath
struct CourierSubPath {
    // Specifies one subpath of the courier truck route
    // The intersection id where a start depot, pick-up intersection or drop-off intersection is located
    IntersectionIdx start_intersection;

    // The intersection id where this subpath ends. This must be the
    // start_intersection of the next subpath or the intersection of an end depot
    IntersectionIdx end_intersection;

    // Street segment ids of the path between start_intersection and end_intersection
    // They form a connected path (see m3.h)
    std::vector<int> subpath;
};

/**
 * 2 Global Variables
 */
bool noPath = false;
bool first = true;
bool firstNoPath = false;
bool secondNoPath = false;
bool thirdNoPath = false;
bool updated = false; // seems useless for now
bool optimized = false;
bool timeLimit = false;
int test = 0; // delete after debug
int currentIdentityNumber;
int nextIdentityNumber;
double temperature;
double travelTime = 0;
double travelTimeOptimizing = 0;
double travelTimeOptimizingSecond = 0;
std::vector<int> nextDeliveryId;
std::vector<nodeType> nextDeliveryType;
std::vector<std::map<double, candidatePath>> depotStart;
std::vector<std::map<double, candidatePath>> pickUpStart;
std::vector<std::map<double, candidatePath>> dropOffStart;
std::map<IntersectionIdx, std::map<IntersectionIdx, TwoOptPath>> optimizePath;
std::vector<deliveryCondition> deliveryConditionVector;
std::vector<DeliveryOrder> deliveryOrderVector;
std::vector<DeliveryOrder> deliveryOrderVectorOptimizing;
std::vector<DeliveryOrder> deliveryOrderVectorOptimizingSecond;
std::vector<CourierSubPath> courier;
std::vector<CourierSubPath> courierOptimizing;
std::vector<CourierSubPath> courierOptimizingSecond;
std::vector<DeliveryOrder> newDeliveryOrderVector;
std::map<double, int, greater<double>> longPathOrder;
auto firstIterator = longPathOrder.begin();
auto secondIterator = longPathOrder.begin();

/**
 * 3 Function Prototypes
 */
std::vector<CourierSubPath> travelingCourier(const std::vector<DeliveryInf>&, const std::vector<int>&, float);
void candidatePathLoad(const std::vector<DeliveryInf>&, const std::vector<int>&, float);

//path sorter
void depotStartPath(int, IntersectionIdx, const std::vector<DeliveryInf>&, float);
void pickUpStartPath(int, IntersectionIdx, const std::vector<DeliveryInf> &, float);
void pickUpStartPathHelper(int, DeliveryOrder);
void dropOffStartPath(int, IntersectionIdx, const std::vector<DeliveryInf> &, const std::vector<int> &, float);
void dropOffStartPathHelper(int, bool, DeliveryOrder);
std::map<double, candidatePath> depotMultiPath(std::map<IntersectionIdx, TwoOptPath>*, vector<MapDBStruct::IntersectionInfo> &,
                                               IntersectionIdx,const std::vector<DeliveryInf>&, float);
std::map<double, candidatePath> pickUpMultiPath(std::map<IntersectionIdx, TwoOptPath>*, vector<MapDBStruct::IntersectionInfo> &,
                                                IntersectionIdx, const std::vector<DeliveryInf>&, float);
std::map<double, candidatePath> dropOffMultiPath(std::map<IntersectionIdx, TwoOptPath>*, vector<MapDBStruct::IntersectionInfo> &,
                                                 IntersectionIdx, const std::vector<DeliveryInf>&, const std::vector<int>&, float);

void initialiseSearch(vector<MapDBStruct::IntersectionInfo>*, IntersectionIdx);
vector<StreetSegmentIdx> backTracking(vector<MapDBStruct::IntersectionInfo> &, IntersectionIdx, IntersectionIdx);
void greedyCourierFinder();

void twoOptimizationPreCompute();
void twoOptimization();
bool legalityChecker(const std::vector<DeliveryOrder>&);
double travelTimeCalculator(const std::vector<DeliveryOrder>&);
std::vector<DeliveryOrder> reversePath(std::vector<DeliveryOrder>);
bool optimizePathFinder(const std::vector<DeliveryOrder>&, const std::vector<DeliveryOrder>&, const std::vector<DeliveryOrder>&);
bool updateCourier(const std::vector<DeliveryOrder>&);

// This routine takes in a vector of N deliveries (pickUp, dropOff
// intersection pairs), another vector of M intersections that
// are legal start and end points for the path (depots), and a turn
// penalty in seconds (see m3.h for details on turn penalties).
//
// The first vector 'deliveries' gives the delivery information.  Each delivery
// in this vector has pickUp and dropOff intersection ids.
// A delivery can only be dropped-off after the associated item has been picked-up.
//
// The second vector 'depots' gives the intersection ids of courier company
// depots containing trucks; you start at any one of these depots and end at
// any one of the depots.
//
// This routine returns a vector of CourierSubPath objects that form a delivery route.
// The CourierSubPath is as defined above. The first street segment id in the
// first subpath is connected to a depot intersection, and the last street
// segment id of the last subpath also connects to a depot intersection.
// A package should not be dropped off if you haven't picked it up yet.
//
// The start_intersection of each subpath in the returned vector should be
// at least one of the following (a pick-up and/or drop-off can only happen at
// the start_intersection of a CourierSubPath object):
//      1- A start depot.
//      2- A pick-up location
//      3- A drop-off location.
//
// You can assume that N is always at least one, M is always at least one
// (i.e. both input vectors are non-empty).
//
// It is legal for the same intersection to appear multiple times in the pickUp
// or dropOff list (e.g. you might have two deliveries with a pickUp
// intersection id of #50). The same intersection can also appear as both a
// pickUp location and a dropOff location.
//
// If you have two pickUps to make at an intersection, traversing the
// intersection once is sufficient to pick up both packages. Additionally,
// one traversal of an intersection is sufficient to drop off all the
// (already picked up) packages that need to be dropped off at that intersection.
//
// Depots will never appear as pickUp or dropOff locations for deliveries.
//
// If no valid route to make *all* the deliveries exists, this routine must
// return an empty (size == 0) vector.

/**
 * 4.1 Traveling Courier
 * @param deliveries
 * @param depots
 * @param turnPenalty
 * @return
 */
std::vector<CourierSubPath> travelingCourier(const std::vector<DeliveryInf> &deliveries,
                                             const std::vector<int> &depots, const float turnPenalty) {
    auto timeStart = std::chrono::high_resolution_clock::now();
    timeLimit = false;

    // MS 4.1 pre-calculation
    candidatePathLoad(deliveries, depots, turnPenalty);

    //comment for later use
    /*
    int index = 0;
    cout << "\n";
    for (auto iterator : deliveries) {
        cout << "Delivery ID: " << index << " Pick Up Id: " << iterator.pickUp
             << " Drop Off Id: " << iterator.dropOff << endl;
        ++index;
    }
    for(int i = 0; i < depots.size(); ++i){
        cout << "Depot ID: " << depots[i] << endl;
    }
    */

    // MS 4.1 greedy algorithm
    greedyCourierFinder();

    // MS 4.2 iterative algorithm
    while(!timeLimit){
        if (optimized){
            twoOptimizationPreCompute();
        }
        if (deliveryOrderVector.size() > 2){
            twoOptimization();
        }
        auto timeNow = std::chrono::high_resolution_clock::now();
        auto timeElapsed = std::chrono::duration_cast<chrono::duration<double>> (timeNow - timeStart);

        // Optimize until 5 seconds
        if (timeElapsed.count() > 0.9 * TIME_CONSTRAINT) {
            timeLimit = true;
        }
    }

    // MS 4 return
    return courier;
}

/**
 * 4.2 Load Candidate Sub-path
 * Based on Dijkstra Algorithm
 * input: DeliveryInfo + Depot Idx
 * output: Store all vectors of Candidate Path
 * @param deliveries
 * @param depots
 * @param turnPenalty
 */
void candidatePathLoad(const std::vector<DeliveryInf>& deliveries, const std::vector<IntersectionIdx>& depots,
                       const float turnPenalty) {
    // Clear Vectors
    depotStart.clear();
    pickUpStart.clear();
    dropOffStart.clear();

    // Resize Vectors
    depotStart.resize(depots.size());
    pickUpStart.resize(deliveries.size());
    dropOffStart.resize(deliveries.size());

    // Depots Start, search every depots intersection location and store it
    // find out which prick up location is closer
#pragma omp parallel for
    for (int i = 0; i < depots.size(); ++i) {
        depotStartPath(i, depots[i], deliveries, turnPenalty);
    }

    // Deliveries Start
    // For each member in each Delivery in the vector find their candidate path
#pragma omp parallel for
    for (int i = 0; i < deliveries.size(); ++i) {
        //find path from every pickup location to the corresponding possible delivery path
        pickUpStartPath(i, deliveries[i].pickUp, deliveries, turnPenalty);
        //every drop-off locations to the possible delivery pick-up and drop-off points
        dropOffStartPath(i, deliveries[i].dropOff, deliveries, depots, turnPenalty);
    }
}

/**
 * 4.3 Multiple Destination Path Finder
 * For Depots Points
 * @param sourceNode
 * @param deliveries
 * @param turnPenalty
 */
void depotStartPath(int i, IntersectionIdx sourceNodeID, const std::vector<DeliveryInf>& deliveries,
                    const float turnPenalty){
    // Part 1. Initialise
    vector<MapDBStruct::IntersectionInfo> virtualGlobal;
    initialiseSearch(&virtualGlobal, sourceNodeID);

    std::map<IntersectionIdx, TwoOptPath> optPath;

    // Part 2. Load Candidate Path Queue in Local
    std::map<double, candidatePath> SourcePath = depotMultiPath(&optPath, virtualGlobal, sourceNodeID, deliveries, turnPenalty);

    // Part 3. Return Candidate Path Queue to Global
    depotStart[i] = SourcePath;
    optimizePath.emplace(std::make_pair(sourceNodeID, optPath));
}

/**
 * 4.4 Depot Start Candidate Queue Load
 * @param sourceNodeID
 * @param deliveries
 * @param turnPenalty
 */
std::map<double, candidatePath> depotMultiPath(std::map<IntersectionIdx, TwoOptPath>* optPath,
                                               vector<MapDBStruct::IntersectionInfo> &virtualGlobal, IntersectionIdx sourceNodeID,
                                               const std::vector<DeliveryInf> &deliveries, const float turnPenalty) {
    // Part 1. Initialise Priority Queue
    // 1.1 Initialise Candidate Path Map
    std::map<double, candidatePath> sourcePath;

    // 1.2 Initialise Wavefront Queue
    std::priority_queue<MapDBStruct::IntersectionInfo, std::vector<MapDBStruct::IntersectionInfo>,
            MapDBStruct::Compare> waveFront;
    // Push intersection struct of Source Node Into Wavefront
    MapDBStruct::IntersectionInfo sourceNode = virtualGlobal[sourceNodeID];
    //push source node info to the wavefront
    waveFront.push(sourceNode);

    // 1.3 Current Path Num
    // if current Path Num == deliveries.size() -> End While Loop
    int count = 0;
    int totalPath = deliveries.size();

    // cout << "Source Node ID: " << sourceNode.interID << endl;

    // Part 2. Traverse through all Delivery Pick Up Nodes
    while(!waveFront.empty()){
        // 2.1 Extract the minimum unvisited node out of wavefront
        MapDBStruct::IntersectionInfo currentNode = waveFront.top();
        waveFront.pop();

        // Store Current Wavefront intersection id to access global information
        IntersectionIdx currentNodeID = currentNode.interID;

        // CurrentNode is Done (Prevent From been a child again)
        if (virtualGlobal[currentNodeID].done) continue;
        else virtualGlobal[currentNodeID].done = true;

        // 2.2 Destination Checking
        int index = 0;
        candidatePath newPath;
        bool isKeyPoint = false;
        for(auto iterator : deliveries){
            if(iterator.pickUp == currentNodeID){
                // Load
                newPath.toNodeType.emplace_back(pickUp);
                newPath.toNodeDeliveryId.emplace_back(index);
                // Count ++
                ++count;
                isKeyPoint = true;
            }
            ++index;
        }

        // Push
        if(isKeyPoint){
            newPath.fromNode = sourceNodeID;
            newPath.toNode = currentNodeID;
            newPath.fromNodeType = depot;
            newPath.travelTime = currentNode.heuristic;
            newPath.path = backTracking(virtualGlobal, sourceNodeID, currentNodeID);
            sourcePath.emplace(std::make_pair(currentNode.heuristic, newPath));

            TwoOptPath newOptPath;
            newOptPath.keyYToKeyXPath = newPath.path;
            newOptPath.keyYToKeyXTime = newPath.travelTime;
            optPath->emplace(std::make_pair(currentNodeID, newOptPath));
        }

        // End Case Checking
        if(count >= totalPath) break;

        // 2.3 Scan
        // Update all connected & not visited & not oneway node's time & parent
        const double &parentStreetID = virtualGlobal[currentNodeID].parentStreetID;

        for(int i = 0; i < mapDB.intersection[currentNodeID].segmentId.size(); ++i){
            // ConnectNode: The Node we are going to explore, from global
            IntersectionIdx connectNodeID;

            // 2.3.1 OneWay Test
            // Current Segment Information
            StreetSegmentIdx currentSegmentID = mapDB.intersection[currentNodeID].segmentId[i];
            StreetSegmentInfo currentSegmentInfo = getStreetSegmentInfo(currentSegmentID);

            const IntersectionIdx fromNode = currentSegmentInfo.from;
            const IntersectionIdx toNode = currentSegmentInfo.to;

            // If current is fromNode -> pass
            // If current is toNode and segment isn't oneWay -> pass
            if(currentNodeID == fromNode){
                connectNodeID = toNode;
            }else if(!currentSegmentInfo.oneWay){
                connectNodeID = fromNode;
            }else {
                continue;
            }

            // 2.3.2 Done Test
            if(virtualGlobal[connectNodeID].done)
                continue;

            // 2.3.3 Turn Penalty Test
            // Travel time initialisation
            const double & currentSegmentTravelTime = mapDB.segment[currentSegmentID].travelTime;
            double tempHeuristic = 0;

            if(currentNodeID != sourceNodeID && currentSegmentInfo.streetID != parentStreetID){
                tempHeuristic = currentNode.heuristic + currentSegmentTravelTime + turnPenalty;
            }else{
                tempHeuristic = currentNode.heuristic + currentSegmentTravelTime;
            }

            // 2.3.4 Add
            // Initialise a new element for Local Wavefront
            MapDBStruct::IntersectionInfo connectNode;

            connectNode.interID = connectNodeID;
            connectNode.heuristic = tempHeuristic;
            connectNode.parentSegmentID = currentSegmentID;
            connectNode.parentNodeID = currentNodeID;
            connectNode.parentStreetID = currentSegmentInfo.streetID;

            // Update message to the global if Time is Smaller
            if(connectNode.heuristic < virtualGlobal[connectNodeID].heuristic) {
                virtualGlobal[connectNodeID].heuristic = tempHeuristic;
                virtualGlobal[connectNodeID].parentSegmentID = currentSegmentID;
                virtualGlobal[connectNodeID].parentNodeID = currentNodeID;
                virtualGlobal[connectNodeID].parentStreetID = currentSegmentInfo.streetID;
            }

            // Push New Local Element into WaveFront
            waveFront.push(connectNode);
        }
    }

    // Part 3.
    return sourcePath;
}

/**
 * 4.5 Multiple Destination Path Finder
 * For Pick Up Points
 * @param sourceNodeID
 * @param deliveries
 * @param turnPenalty
 */
void pickUpStartPath(int i, IntersectionIdx sourceNodeID, const std::vector<DeliveryInf>& deliveries,
                     const float turnPenalty) {
    // Part 1. Initialise
    vector<MapDBStruct::IntersectionInfo> virtualGlobal;
    initialiseSearch(&virtualGlobal, sourceNodeID);
    std::map<IntersectionIdx, TwoOptPath> optPath;

    // Part 2. Load Candidate Path Queue in Local
    std::map<double, candidatePath> SourcePath = pickUpMultiPath(&optPath, virtualGlobal, sourceNodeID, deliveries, turnPenalty);

    // Part 3. Return Candidate Path Queue to Global
    pickUpStart[i] = SourcePath;
    optimizePath.emplace(std::make_pair(sourceNodeID, optPath));
}

/**
 * 4.6 Multiple Path Finder for Pick Up start Points
 * @param sourceNodeID
 * @param deliveries
 * @param turnPenalty
 */
std::map<double, candidatePath> pickUpMultiPath(std::map<IntersectionIdx, TwoOptPath>* optPath,
                                                vector<MapDBStruct::IntersectionInfo> &virtualGlobal, IntersectionIdx sourceNodeID,
                                                const std::vector<DeliveryInf> &deliveries, const float turnPenalty) {
    // Part 1. Initialise Priority Queue
    // 1.1 Initialise Candidate Path Queue
    std::map<double, candidatePath> sourcePath;

    // 1.2 Initialise Wavefront Queue
    std::priority_queue<MapDBStruct::IntersectionInfo, std::vector<MapDBStruct::IntersectionInfo>,
            MapDBStruct::Compare> waveFront;
    // Push Source Node Into Wavefront
    MapDBStruct::IntersectionInfo sourceNode = virtualGlobal[sourceNodeID];
    waveFront.push(sourceNode);

    // 1.3 Current Path Num
    // if current Path Num == deliveries.size() -> End While Loop
    int count = 0;
    int totalPath = deliveries.size()*2 - 1;

    // Part 2. Traverse through all Delivery Pick Up Nodes
    while(!waveFront.empty()){
        // 2.1 Extract the minimum unvisited node out of wavefront
        MapDBStruct::IntersectionInfo currentNode = waveFront.top();
        waveFront.pop();

        // Store Current Wavefront Node's ID to access global information
        IntersectionIdx currentNodeID = currentNode.interID;

        // CurrentNode is Done (Prevent From been a child again)
        if (virtualGlobal[currentNodeID].done) continue;
        else virtualGlobal[currentNodeID].done = true;

        // 2.2 Destination Checking
        int index = 0;
        candidatePath newPath;
        bool isKeyPoint = false;
        // Pick Up Points
        for(auto iterator : deliveries){
            if(currentNodeID != sourceNodeID && iterator.pickUp == currentNodeID){
                // Load
                newPath.toNodeType.emplace_back(pickUp);
                newPath.toNodeDeliveryId.emplace_back(index);
                // Count ++
                ++count;
                isKeyPoint = true;
            }
            ++index;
        }

        // Drop Off points
        index = 0;
        for(auto iterator : deliveries){
            if(iterator.dropOff == currentNodeID){
                // Load
                newPath.toNodeType.emplace_back(dropOff);
                newPath.toNodeDeliveryId.emplace_back(index);
                // Count ++
                ++count;
                isKeyPoint = true;
            }
            ++index;
        }

        // Push
        if(isKeyPoint){
            newPath.fromNode = sourceNodeID;
            newPath.toNode = currentNodeID;
            newPath.fromNodeType = pickUp;
            newPath.travelTime = currentNode.heuristic;
            newPath.path = backTracking(virtualGlobal, sourceNodeID, currentNodeID);
            sourcePath.emplace(std::make_pair(currentNode.heuristic, newPath));

            TwoOptPath newOptPath;
            newOptPath.keyYToKeyXPath = newPath.path;
            newOptPath.keyYToKeyXTime = newPath.travelTime;
            optPath->emplace(std::make_pair(currentNodeID, newOptPath));
        }

        // End Case Checking
        if(count >= totalPath) break;

        // 2.3 Scan
        // Update all connected & not visited & not oneway node's time & parent
        const double &parentStreetID = virtualGlobal[currentNodeID].parentStreetID;

        for(int i = 0; i < mapDB.intersection[currentNodeID].segmentId.size(); ++i){
            // ConnectNode: The Node we are going to explore, from global
            IntersectionIdx connectNodeID;

            // 2.3.1 OneWay Test
            // Current Segment Information
            StreetSegmentIdx currentSegmentID = mapDB.intersection[currentNodeID].segmentId[i];
            StreetSegmentInfo currentSegmentInfo = getStreetSegmentInfo(currentSegmentID);

            const IntersectionIdx fromNode = currentSegmentInfo.from;
            const IntersectionIdx toNode = currentSegmentInfo.to;

            // If current is fromNode -> pass
            // If current is toNode and segment isn't oneWay -> pass
            if(currentNodeID == fromNode){
                connectNodeID = toNode;
            }else if(!currentSegmentInfo.oneWay){
                connectNodeID = fromNode;
            }else {
                continue;
            }

            // 2.3.2 Done Test
            if(mapDB.intersection[connectNodeID].done)
                continue;

            // 2.3.3 Turn Penalty Test
            // Travel time initialisation
            const double & currentSegmentTravelTime = mapDB.segment[currentSegmentID].travelTime;
            double tempHeuristic = 0;

            if(currentNodeID != sourceNodeID && currentSegmentInfo.streetID != parentStreetID){
                tempHeuristic = currentNode.heuristic + currentSegmentTravelTime + turnPenalty;
            }else{
                tempHeuristic = currentNode.heuristic + currentSegmentTravelTime;
            }

            // 2.3.4 Add
            // Initialise a new element for Local Wavefront
            MapDBStruct::IntersectionInfo connectNode;

            connectNode.interID = connectNodeID;
            connectNode.heuristic = tempHeuristic;
            connectNode.parentSegmentID = currentSegmentID;
            connectNode.parentNodeID = currentNodeID;
            connectNode.parentStreetID = currentSegmentInfo.streetID;

            // Update message to the global if Time is Smaller
            if(connectNode.heuristic < virtualGlobal[connectNodeID].heuristic) {
                virtualGlobal[connectNodeID].heuristic = tempHeuristic;
                virtualGlobal[connectNodeID].parentSegmentID = currentSegmentID;
                virtualGlobal[connectNodeID].parentNodeID = currentNodeID;
                virtualGlobal[connectNodeID].parentStreetID = currentSegmentInfo.streetID;
            }

            // Push New Local Element into WaveFront
            waveFront.push(connectNode);
        }
    }

    // Part 3. Return
    return sourcePath;
}

/**
 * 4.7 Multiple Destination Path Finder
 * For Drop Off Points
 * @param sourceNode
 * @param deliveries
 * @param depots
 * @param turnPenalty
 */
void dropOffStartPath(int i, IntersectionIdx sourceNodeID, const std::vector<DeliveryInf>& deliveries,
                      const std::vector<int>& depots, const float turnPenalty){
    // Part 1. Initialise
    vector<MapDBStruct::IntersectionInfo> virtualGlobal;
    initialiseSearch(&virtualGlobal, sourceNodeID);
    std::map<IntersectionIdx, TwoOptPath> optPath;

    // Part 2. Load Candidate Path Queue in Local
    std::map<double, candidatePath> SourcePath = dropOffMultiPath(&optPath, virtualGlobal, sourceNodeID, deliveries, depots, turnPenalty);

    // Part 3. Return Candidate Path Queue to Global
    dropOffStart[i] = SourcePath;
    optimizePath.emplace(std::make_pair(sourceNodeID, optPath));
}

/**
 * 4.8 Find Multi Destination Path for Drop Off Start Node
 * @param sourceNodeID
 * @param deliveries
 * @param depots
 * @param turnPenalty
 */
std::map<double, candidatePath> dropOffMultiPath(std::map<IntersectionIdx, TwoOptPath>* optPath,
                                                 vector<MapDBStruct::IntersectionInfo> &virtualGlobal, IntersectionIdx sourceNodeID, const std::vector<DeliveryInf>& deliveries,
                                                 const std::vector<int>& depots, const float turnPenalty){
    // Part 1. Initialise Priority Queue
    // 1.1 Initialise Candidate Path Queue
    std::map<double, candidatePath> sourcePath;

    // 1.2 Initialise Wavefront Queue
    std::priority_queue<MapDBStruct::IntersectionInfo, std::vector<MapDBStruct::IntersectionInfo>,
            MapDBStruct::Compare> waveFront;
    // Push Source Node Into Wavefront
    MapDBStruct::IntersectionInfo sourceNode = virtualGlobal[sourceNodeID];
    waveFront.push(sourceNode);

    // 1.3 Current Path Num
    // if current Path Num == deliveries.size() -> End While Loop
    int count = 0;
    int totalPath = deliveries.size()*2-1 + depots.size();

    // Part 2. Traverse through all Delivery Pick Up Nodes
    while(!waveFront.empty()){
        // 2.1 Extract the minimum unvisited node out of wavefront
        MapDBStruct::IntersectionInfo currentNode = waveFront.top();
        waveFront.pop();

        // Store Current Wavefront Node's ID to access global information
        IntersectionIdx currentNodeID = currentNode.interID;

        // CurrentNode is Done (Prevent From been a child again)
        if (virtualGlobal[currentNodeID].done) continue;
        else virtualGlobal[currentNodeID].done = true;

        // 2.2 Destination Checking
        int index = 0;
        candidatePath newPath;
        bool isKeyPoint = false;
        for(auto iterator : deliveries){
            if(iterator.pickUp == currentNodeID){
                // Load
                newPath.toNodeType.emplace_back(pickUp);
                newPath.toNodeDeliveryId.emplace_back(index);
                // Count ++
                ++count;
                isKeyPoint = true;
            }
            ++index;
        }

        // Drop Off
        index = 0;
        for(auto iterator : deliveries){
            if(currentNodeID != sourceNodeID && iterator.dropOff == currentNodeID){
                // Load
                newPath.toNodeType.emplace_back(dropOff);
                newPath.toNodeDeliveryId.emplace_back(index);
                // Count ++
                ++count;
                isKeyPoint = true;
            }
            ++index;
        }

        // Push if its Delivery Point
        if(isKeyPoint){
            newPath.fromNode = sourceNodeID;
            newPath.toNode = currentNodeID;
            newPath.fromNodeType = dropOff;
            newPath.travelTime = currentNode.heuristic;
            newPath.path = backTracking(virtualGlobal, sourceNodeID, currentNodeID);
            sourcePath.emplace(std::make_pair(currentNode.heuristic, newPath));

            TwoOptPath newOptPath;
            newOptPath.keyYToKeyXPath = newPath.path;
            newOptPath.keyYToKeyXTime = newPath.travelTime;
            optPath->emplace(std::make_pair(currentNodeID, newOptPath));
        }

        // Depot Destination Checking
        if(!isKeyPoint){
            for(auto iterator : depots){
                if(iterator == currentNodeID){
                    // Create
                    candidatePath NewPath;
                    // Load
                    NewPath.fromNode = sourceNodeID;
                    NewPath.toNode = currentNodeID;
                    NewPath.fromNodeType = dropOff;
                    NewPath.toNodeType.emplace_back(depot);
                    NewPath.toNodeDeliveryId.emplace_back(-1);
                    NewPath.travelTime = currentNode.heuristic;
                    NewPath.path = backTracking(virtualGlobal, sourceNodeID, currentNodeID);
                    // Push
                    sourcePath.emplace(std::make_pair(currentNode.heuristic, NewPath));
                    // Push to 2-opt
                    TwoOptPath newOptPath;
                    newOptPath.keyYToKeyXPath = NewPath.path;
                    newOptPath.keyYToKeyXTime = NewPath.travelTime;
                    optPath->emplace(std::make_pair(currentNodeID, newOptPath));
                    // Count ++
                    ++count;
                    // Break Loop Once Found
                    break;
                }
            }
        }

        // End Case Checking
        if(count >= totalPath) break;

        // 2.3 Scan
        // Update all connected & not visited & not oneway node's time & parent
        const double &parentStreetID = virtualGlobal[currentNodeID].parentStreetID;

        for(int i = 0; i < mapDB.intersection[currentNodeID].segmentId.size(); ++i){
            // ConnectNode: The Node we are going to explore, from global
            IntersectionIdx connectNodeID;

            // 2.3.1 OneWay Test
            // Current Segment Information
            StreetSegmentIdx currentSegmentID = mapDB.intersection[currentNodeID].segmentId[i];
            StreetSegmentInfo currentSegmentInfo = getStreetSegmentInfo(currentSegmentID);

            const IntersectionIdx fromNode = currentSegmentInfo.from;
            const IntersectionIdx toNode = currentSegmentInfo.to;

            // If current is fromNode -> pass
            // If current is toNode and segment isn't oneWay -> pass
            if(currentNodeID == fromNode){
                connectNodeID = toNode;
            }else if(!currentSegmentInfo.oneWay){
                connectNodeID = fromNode;
            }else {
                continue;
            }

            // 2.3.2 Done Test
            if(virtualGlobal[connectNodeID].done)
                continue;

            // 2.3.3 Turn Penalty Test
            // Travel time initialisation
            const double & currentSegmentTravelTime = mapDB.segment[currentSegmentID].travelTime;
            double tempHeuristic = 0;

            if(currentNodeID != sourceNodeID && currentSegmentInfo.streetID != parentStreetID){
                tempHeuristic = currentNode.heuristic + currentSegmentTravelTime + turnPenalty;
            }else{
                tempHeuristic = currentNode.heuristic + currentSegmentTravelTime;
            }

            // 2.3.4 Add
            // Initialise a new element for Local Wavefront
            MapDBStruct::IntersectionInfo connectNode;

            connectNode.interID = connectNodeID;
            connectNode.heuristic = tempHeuristic;
            connectNode.parentSegmentID = currentSegmentID;
            connectNode.parentNodeID = currentNodeID;
            connectNode.parentStreetID = currentSegmentInfo.streetID;

            // Update message to the global if Time is Smaller
            if(connectNode.heuristic < virtualGlobal[connectNodeID].heuristic) {
                virtualGlobal[connectNodeID].heuristic = tempHeuristic;
                virtualGlobal[connectNodeID].parentSegmentID = currentSegmentID;
                virtualGlobal[connectNodeID].parentNodeID = currentNodeID;
                virtualGlobal[connectNodeID].parentStreetID = currentSegmentInfo.streetID;
            }

            // Push New Local Element into WaveFront
            waveFront.push(connectNode);
        }
    }

    // Part 3.
    return sourcePath;
}

/**
 * 4.9 Initialise for Each Multi Destination Search
 * @param sourceNode
 */
void initialiseSearch(vector<MapDBStruct::IntersectionInfo>* virtualGlobal, IntersectionIdx sourceNodeID){
    virtualGlobal->resize((getNumIntersections()));
    for(int intersectionID = 0; intersectionID < getNumIntersections(); ++intersectionID){
        // Part 1.1 Nodes' time are the largest
        (virtualGlobal->at(intersectionID)).heuristic = std::numeric_limits<double>::max();

        // Part 1.2 All the parent ID are out of bound
        (virtualGlobal->at(intersectionID)).parentSegmentID = -1;
        (virtualGlobal->at(intersectionID)).parentNodeID = -1;
        (virtualGlobal->at(intersectionID)).parentStreetID = -1;

        // Part 1.3 All the nodes are not done
        (virtualGlobal->at(intersectionID)).done = false;
        (virtualGlobal->at(intersectionID)).interID = intersectionID;
    }

    // Part 1.4 Start Node's Time is 0
    (virtualGlobal->at(sourceNodeID)).heuristic = 0;
}

/**
 * 4.10 Back Tracking Path between two points
 * @param startID
 * @param destiID
 * @return
 */
vector<StreetSegmentIdx> backTracking(vector<MapDBStruct::IntersectionInfo> &virtualGlobal,
                                      IntersectionIdx startID, IntersectionIdx destiID){
    // Initialise path vector
    std::vector<StreetSegmentIdx> path;

    // General Case: Fetch the Path
    MapDBStruct::IntersectionInfo currentNode = virtualGlobal[destiID];

    while(currentNode.interID != startID){
        // insert current segmentID into path
        path.insert(path.begin(), currentNode.parentSegmentID);

        // Go to parent Node
        currentNode = virtualGlobal[currentNode.parentNodeID];
    }

    return path;
}

/**
 * 4.11 Pick Up Start Path Helper
 * @param deliveryId
 * @param identity
 */
void pickUpStartPathHelper(int deliveryId, DeliveryOrder next){
    // Variables needed for CourierSubPath
    IntersectionIdx minFrom = -1;
    IntersectionIdx minTo = -1;
    vector<StreetSegmentIdx> minPath;
    double minTime;
    bool pathIsFound = false;

    if (pickUpStart[deliveryId].empty()){
        noPath = true;
    } else {
        for (auto it = pickUpStart[deliveryId].begin(); it != pickUpStart[deliveryId].end(); ++it){
            nextDeliveryType = (it->second).toNodeType;
            nextDeliveryId = (it->second).toNodeDeliveryId;
            nextIdentityNumber = nextDeliveryId.size();

            for (int i = 0; i < nextIdentityNumber; ++i){
                // May optimize here (choose one with more possibility)
                if ( ((nextDeliveryType[i] == pickUp) && (!deliveryConditionVector[nextDeliveryId[i]].pickUpVisited))
                     || ((nextDeliveryType[i] == dropOff) && (deliveryConditionVector[nextDeliveryId[i]].onTruck)) ){
                    minFrom = (it->second).fromNode;
                    minTo = (it->second).toNode;
                    minPath = (it->second).path;
                    minTime = (it->second).travelTime;
                    // Break once shortest path is found
                    pathIsFound = true;
                    break;
                }
            }
            if(pathIsFound) break;
        }

        if (!pathIsFound) {
            noPath = true;
        } else {
            CourierSubPath pickUpNext;
            pickUpNext.start_intersection = minFrom;
            pickUpNext.end_intersection = minTo;
            pickUpNext.subpath = minPath;
            courier.emplace_back(pickUpNext);

            // Load delivery order vector
            next.nodeId = minFrom;
            next.travelTime = minTime;
            deliveryOrderVector.emplace_back(next);
            travelTime += minTime;
        }
    }
}

/**
 * 4.12 Drop Off Start Path Helper
 * @param deliveryId
 * @param done
 */
void dropOffStartPathHelper(int deliveryId, bool done, DeliveryOrder next){
    // Variables needed for CourierSubPath
    IntersectionIdx minFrom = -1;
    IntersectionIdx minTo = -1;
    vector<StreetSegmentIdx> minPath;
    double minTime;
    bool pathIsFound = false;

    if (dropOffStart[deliveryId].empty()){
        noPath = true;
    } else {
        for (auto it = dropOffStart[deliveryId].begin(); it != dropOffStart[deliveryId].end(); ++it){
            nextDeliveryType = (it->second).toNodeType;
            nextDeliveryId = (it->second).toNodeDeliveryId;
            nextIdentityNumber = nextDeliveryId.size();

            for (int i = 0; i < nextIdentityNumber; ++i){
                if (((nextDeliveryType[i] == pickUp) && (!deliveryConditionVector[nextDeliveryId[i]].pickUpVisited))
                    || ((nextDeliveryType[i] == dropOff) && (deliveryConditionVector[nextDeliveryId[i]].onTruck))
                    || ((nextDeliveryType[i] == depot) && done)){
                    minFrom = (it->second).fromNode;
                    minTo = (it->second).toNode;
                    minPath = (it->second).path;
                    minTime = (it->second).travelTime;
                    // Break once shortest path is found
                    pathIsFound = true;
                    break;
                }
            }
            if(pathIsFound) break;
        }

        if (!pathIsFound) {
            noPath = true;
        } else {
            CourierSubPath dropOffNext;
            dropOffNext.start_intersection = minFrom;
            dropOffNext.end_intersection = minTo;
            dropOffNext.subpath = minPath;
            courier.emplace_back(dropOffNext);

            // Load delivery order vector
            next.nodeId = minFrom;
            if (!done){
                next.travelTime = minTime;
            }

            deliveryOrderVector.emplace_back(next);
            travelTime += minTime;
            if (done){
                first = true;
                optimized = true;
            }
        }
    }
}

/**
 * 4.13 Find Proper Courier
 * Based on Greedy Algorithm
 * input: std::vector<candidatePath>& depotStart, std::vector<candidatePath>& deliveryStart, std::vector<bool>& onTruck
 * output: Store
 * @return empty vector if courier is not found
 */
void greedyCourierFinder(){
    // need to return intersection A, intersection B, and paths between them
    // Variables needed for CourierSubPath
    double minTime = -1.0;
    IntersectionIdx minFrom = -1;
    IntersectionIdx minTo = -1;
    vector<StreetSegmentIdx> minPath;
    bool deliveryDone = false;

    // Initialization
    noPath = false;
    int deliveryNumber = pickUpStart.size();
    courier.clear();
    nextDeliveryType.clear();
    nextDeliveryId.clear();
    deliveryConditionVector.clear();
    deliveryConditionVector.resize(deliveryNumber);
    for (int i = 0; i < deliveryNumber; ++i){
        deliveryConditionVector[i].pickUpVisited = false;
        deliveryConditionVector[i].dropOffVisited = false;
        deliveryConditionVector[i].onTruck = false;
    }

    // Set an initial travel time
    int depotNumber = depotStart.size();
    for (int i = 0; i < depotNumber; ++i){
        if (depotStart[i].begin() != depotStart[i].end()){
            minTime = depotStart[i].begin()->second.travelTime;
            break;
        }
    }

    if (minTime == -1.0){
        noPath = true;
    }

    if (!noPath){
        // Find the shortest path from any depot to first pickup
        for (int i = 0; i < depotNumber; ++i){
            if (!depotStart[i].empty()){
                if (((depotStart[i].begin())->second).travelTime <= minTime){
                    // Find shorter path, and update needed information for CourierSubPath
                    minTime = ((depotStart[i].begin())->second).travelTime;
                    minFrom = ((depotStart[i].begin())->second).fromNode;
                    minTo = ((depotStart[i].begin())->second).toNode;
                    minPath = ((depotStart[i].begin())->second).path;

                    nextDeliveryId = ((depotStart[i].begin())->second).toNodeDeliveryId;
                    nextDeliveryType = ((depotStart[i].begin())->second).toNodeType;
                }
            }
        }
        CourierSubPath depotFirst;
        depotFirst.start_intersection = minFrom;
        depotFirst.end_intersection = minTo;
        depotFirst.subpath = minPath;
        courier.emplace_back(depotFirst);
        travelTime += minTime;

        while (!deliveryDone){
            DeliveryOrder nextNode;
            currentIdentityNumber = nextDeliveryId.size();
            for (int index = 0; index < currentIdentityNumber; ++index){
                if (nextDeliveryType[index] == pickUp){
                    // Load delivery order vector
                    nextNode.deliveryType.emplace_back(pickUp);
                    nextNode.deliveryId.emplace_back(nextDeliveryId[index]);

                    // Update pick up and visited conditions
                    if (!deliveryConditionVector[nextDeliveryId[index]].pickUpVisited){
                        deliveryConditionVector[nextDeliveryId[index]].onTruck = true;
                    }
                    deliveryConditionVector[nextDeliveryId[index]].pickUpVisited = true;

                    // Call path finder until last identity is updated
                    if (index == currentIdentityNumber - 1){
                        pickUpStartPathHelper(nextDeliveryId[index], nextNode);
                    }
                    if (noPath){
                        goto afterWhile;
                    }
                } else {
                    // Load delivery order vector
                    nextNode.deliveryType.emplace_back(dropOff);
                    nextNode.deliveryId.emplace_back(nextDeliveryId[index]);

                    // Update drop off and visited conditions
                    deliveryConditionVector[nextDeliveryId[index]].dropOffVisited = true;
                    deliveryConditionVector[nextDeliveryId[index]].onTruck = false;

                    // End condition: all pick up and drop off are visited
                    // May use count to speed up
                    deliveryDone = true;
                    for (int i = 0; i < deliveryNumber; ++i) {
                        if ((!deliveryConditionVector[i].pickUpVisited) || (!deliveryConditionVector[i].dropOffVisited)
                            || (deliveryConditionVector[i].onTruck)){
                            deliveryDone = false;
                            break;
                        }
                    }

                    // Call path finder until last identity is updated
                    if (index == currentIdentityNumber - 1){
                        dropOffStartPathHelper(nextDeliveryId[index], deliveryDone, nextNode);
                    }
                    if (noPath){
                        goto afterWhile;
                    }
                }
            }
        }
    }

    afterWhile:
    if (noPath) {
        courier.clear();
    }
}

/**
 *  4.14 Two Optimization Main Function
 *  two opt algorithm implementation
 */
void twoOptimization() {
    // Initialization
    bool legal = false;
    bool cannotOptimize = false;
    firstNoPath = false;
    secondNoPath = false;
    thirdNoPath = false;

    // Traverse current delivery order to find the longest path
    secondIterator++;
    if (secondIterator == longPathOrder.end()){
        firstIterator++;
        if (firstIterator == --longPathOrder.end()){
            firstIterator = longPathOrder.begin();
        }
        secondIterator = firstIterator;
        secondIterator++;
    }

    if (!cannotOptimize){

        // Randomly choose two connections to break
        // Temporary No Use
        /*int breakSize = deliveryOrderVectorOptimizing.size() - 1;         // delivery.size() is at least 3
        int firstBreak = rand() % breakSize;                    // 0 to delivery.size() - 2
        int secondBreak = rand() % breakSize;
        if (firstBreak == secondBreak) {
            bool equal = true;
            while (equal) {
                secondBreak = rand() % breakSize;
                if (firstBreak != secondBreak) {
                    equal = false;
                }
            }
        }*/

        int firstBreak = firstIterator->second;
        int secondBreak = secondIterator->second;

        if (firstBreak > secondBreak){
            int temp = firstBreak;
            firstBreak = secondBreak;
            secondBreak = temp;
        }
        test = deliveryOrderVectorOptimizing.size();
        //cout << "size of the delivery order is " << deliveryOrderVectorOptimizing.size() << endl;
        //if (test == 59) cout << "first break is " << firstBreak << endl;
        //if (test == 59) cout << "second break is " << secondBreak << endl;

        // 3 sub paths
        std::vector<DeliveryOrder> firstSubPath;
        for (int i = 0; i < firstBreak + 1; ++i){
            firstSubPath.push_back(deliveryOrderVectorOptimizing[i]);
        }
        std::vector<DeliveryOrder> secondSubPath;
        for (int i = firstBreak + 1; i < secondBreak + 1; ++i){
            secondSubPath.push_back(deliveryOrderVectorOptimizing[i]);
        }
        std::vector<DeliveryOrder> thirdSubPath;
        for (int i = secondBreak + 1; i < deliveryOrderVectorOptimizing.size(); ++i){
            thirdSubPath.push_back(deliveryOrderVectorOptimizing[i]);
        }

        // Generate 3 reversed paths
        std::vector<DeliveryOrder> firstReversePath(firstSubPath.rbegin(), firstSubPath.rend());
        std::vector<DeliveryOrder> secondReversePath(secondSubPath.rbegin(), secondSubPath.rend());
        std::vector<DeliveryOrder> thirdReversePath(thirdSubPath.rbegin(), thirdSubPath.rend());

        // 48 combinations
// #pragma omp parallel for
        for (int i = 0; i < 8; i++){
            if (!i){
                legal = optimizePathFinder(firstSubPath, secondSubPath, thirdSubPath);
            } else if (i == 1){
                legal = optimizePathFinder(firstReversePath, secondSubPath, thirdSubPath);
            } else if (i == 2){
                legal = optimizePathFinder(firstSubPath, secondReversePath, thirdSubPath);
            } else if (i == 3){
                legal = optimizePathFinder(firstSubPath, secondSubPath, thirdReversePath);
            } else if (i == 4){
                legal = optimizePathFinder(firstReversePath, secondReversePath, thirdSubPath);
            } else if (i == 5){
                legal = optimizePathFinder(firstReversePath, secondSubPath, thirdReversePath);
            } else if (i == 6){
                legal = optimizePathFinder(firstSubPath, secondReversePath, thirdReversePath);
            } else if (i == 7){
                legal = optimizePathFinder(firstReversePath, secondReversePath, thirdReversePath);
            }
            if (legal) break; // Multi Threading For Loop can't use Break
        }
    }
}

/**
 * Two Optimization Pre Compute
 */
void twoOptimizationPreCompute(){
    if (first){
        travelTimeOptimizing = travelTime;
        deliveryOrderVectorOptimizing = deliveryOrderVector;
        courierOptimizing = courier;
        temperature = 1250;
        first = false;
    }
    longPathOrder.clear();
    for (int index = 0; index < deliveryOrderVectorOptimizing.size() - 1; ++index){
        longPathOrder.emplace(std::make_pair(deliveryOrderVectorOptimizing[index].travelTime, index));
    }
    firstIterator = longPathOrder.begin();
    secondIterator = longPathOrder.begin();
    secondIterator++;
    optimized = false;
}

/**
 *
 * @param firstPath
 * @param secondPath
 * @param thirdPath
 * @return
 */
bool optimizePathFinder(const std::vector<DeliveryOrder>& firstPath, const std::vector<DeliveryOrder>& secondPath,
                        const std::vector<DeliveryOrder>& thirdPath){
    bool legal = false;

// #pragma omp parallel for
    for (int i = 0; i < 6; ++i){

        // Part 1. Path Recombination
        newDeliveryOrderVector.clear();
        if (!i){
            // Order: 1, 2, 3
            for (int j = 0; j < firstPath.size(); ++j){
                newDeliveryOrderVector.push_back(firstPath[j]);
            }
            for (int j = 0; j < secondPath.size(); ++j){
                newDeliveryOrderVector.push_back(secondPath[j]);
            }
            for (int j = 0; j < thirdPath.size(); ++j){
                newDeliveryOrderVector.push_back(thirdPath[j]);
            }
        } else if (i == 1){
            // Order: 1, 3, 2
            for (int j = 0; j < firstPath.size(); ++j){
                newDeliveryOrderVector.push_back(firstPath[j]);
            }
            for (int j = 0; j < thirdPath.size(); ++j){
                newDeliveryOrderVector.push_back(thirdPath[j]);
            }
            for (int j = 0; j < secondPath.size(); ++j){
                newDeliveryOrderVector.push_back(secondPath[j]);
            }
        } else if (i == 2){
            // Order: 2, 1, 3
            for (int j = 0; j < secondPath.size(); ++j){
                newDeliveryOrderVector.push_back(secondPath[j]);
            }
            for (int j = 0; j < firstPath.size(); ++j){
                newDeliveryOrderVector.push_back(firstPath[j]);
            }
            for (int j = 0; j < thirdPath.size(); ++j){
                newDeliveryOrderVector.push_back(thirdPath[j]);
            }
        } else if (i == 3){
            // Order: 2, 3, 1
            for (int j = 0; j < secondPath.size(); ++j){
                newDeliveryOrderVector.push_back(secondPath[j]);
            }
            for (int j = 0; j < thirdPath.size(); ++j){
                newDeliveryOrderVector.push_back(thirdPath[j]);
            }
            for (int j = 0; j < firstPath.size(); ++j){
                newDeliveryOrderVector.push_back(firstPath[j]);
            }
        } else if (i == 4){
            // Order: 3, 1, 2
            for (int j = 0; j < thirdPath.size(); ++j){
                newDeliveryOrderVector.push_back(thirdPath[j]);
            }
            for (int j = 0; j < firstPath.size(); ++j){
                newDeliveryOrderVector.push_back(firstPath[j]);
            }
            for (int j = 0; j < secondPath.size(); ++j){
                newDeliveryOrderVector.push_back(secondPath[j]);
            }
        } else if (i == 5){
            // Order: 3, 2, 1
            for (int j = 0; j < thirdPath.size(); ++j){
                newDeliveryOrderVector.push_back(thirdPath[j]);
            }
            for (int j = 0; j < secondPath.size(); ++j){
                newDeliveryOrderVector.push_back(secondPath[j]);
            }
            for (int j = 0; j < firstPath.size(); ++j){
                newDeliveryOrderVector.push_back(firstPath[j]);
            }
        }

        // Part 2. Legality Checker
        legal = true;
        int deliveryNumber = pickUpStart.size();
        for (int m = 0; m < deliveryNumber; ++m){
            deliveryConditionVector[m].pickUpVisited = false;
            deliveryConditionVector[m].dropOffVisited = false;
            deliveryConditionVector[m].onTruck = false;
        }

        for (int index = 0; index < newDeliveryOrderVector[0].deliveryType.size(); ++index) {
            if (newDeliveryOrderVector[0].deliveryType[index] == pickUp){
                deliveryConditionVector[newDeliveryOrderVector[0].deliveryId[index]].pickUpVisited = true;
                deliveryConditionVector[newDeliveryOrderVector[0].deliveryId[index]].onTruck = true;
            } else {
                legal = false;
                break;
            }
        }

        if (legal){
            for (int m = 1; m < newDeliveryOrderVector.size(); ++m){
                for (int index = 0; index < newDeliveryOrderVector[m].deliveryType.size(); ++index) {
                    if (newDeliveryOrderVector[m].deliveryType[index] == pickUp){
                        if (!deliveryConditionVector[newDeliveryOrderVector[m].deliveryId[index]].pickUpVisited){
                            deliveryConditionVector[newDeliveryOrderVector[m].deliveryId[index]].onTruck = true;
                        }
                        deliveryConditionVector[newDeliveryOrderVector[m].deliveryId[index]].pickUpVisited = true;
                    } else {
                        deliveryConditionVector[newDeliveryOrderVector[m].deliveryId[index]].dropOffVisited = true;
                        deliveryConditionVector[newDeliveryOrderVector[m].deliveryId[index]].onTruck = false;
                    }
                }
            }

            for (int m = 0; m < deliveryNumber; ++m){
                if ((!deliveryConditionVector[m].pickUpVisited) || (!deliveryConditionVector[m].dropOffVisited)
                    || (deliveryConditionVector[m].onTruck)){
                    legal = false;
                    break;
                }
            }
        }

        // Part 3. Simulated Annealing
        if (legal){
            //cout << "ready to enter update courier" << endl;
            bool update = updateCourier(newDeliveryOrderVector);

            if (update){
                if (travelTimeOptimizingSecond < travelTime){
                    travelTimeOptimizing = travelTimeOptimizingSecond;
                    deliveryOrderVectorOptimizing = newDeliveryOrderVector;
                    courierOptimizing = courierOptimizingSecond;

                    travelTime = travelTimeOptimizingSecond;
                    deliveryOrderVector = newDeliveryOrderVector;
                    courier = courierOptimizingSecond;
                    // temperature = 0.95 * temperature;
                    optimized = true;
                    break;
                } else if (travelTimeOptimizingSecond > travelTimeOptimizing){
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> number(0.0, 1.0);
                    double timeDifference = travelTimeOptimizingSecond - travelTime;

                    if (number(gen) < exp(- timeDifference / temperature)) {
                        travelTimeOptimizing = travelTimeOptimizingSecond;
                        deliveryOrderVectorOptimizing = newDeliveryOrderVector;
                        courierOptimizing = courierOptimizingSecond;
                        cout << "travel time is " << travelTimeOptimizingSecond << endl;
                        // temperature = 0.95 * temperature;
                        optimized = true;
                        break;
                    }
                }
            } else {
                legal = false;
            }
        }
        temperature = 0.95 * temperature;
    }
    return legal;
}

/**
 *
 * @param delivery
 * @return
 */
bool updateCourier(const std::vector<DeliveryOrder>& delivery){
    // Convert delivery order vector to CourierSubPath
    bool update = true;

    // Find closest depot through first pick up
    IntersectionIdx firstPickUp = delivery[0].nodeId;
    IntersectionIdx depotId = -1;
    bool firstTime = true;
    double depotTime = -1.0;
    std::vector<StreetSegmentIdx> depotPath;

    for (int i = 0; i < depotStart.size(); ++i){
        for (auto it = depotStart[i].begin(); it != depotStart[i].end(); ++it){
            if ((it->second).toNode == firstPickUp){
                if (firstTime){
                    depotId = (it->second).fromNode;
                    depotTime = (it->second).travelTime;
                    depotPath = (it->second).path;
                    firstTime = false;
                } else {
                    if ((it->second).travelTime <= depotTime){
                        depotId = (it->second).fromNode;
                        depotTime = (it->second).travelTime;
                        depotPath = (it->second).path;
                    }
                }
            }
        }
    }

    if (firstTime){
        update = false;
        //cout << "current first pick up id is " << firstPickUp << endl;
        //cout << "cannot find depot to first pick up" << endl;
    } else {
        courierOptimizingSecond.clear();
        courierOptimizingSecond.resize(1);
        CourierSubPath depotNext;
        depotNext.start_intersection = depotId;
        depotNext.end_intersection = firstPickUp;
        depotNext.subpath = depotPath;
        courierOptimizingSecond[0] = depotNext;
        travelTimeOptimizingSecond = depotTime;

        // Find path between two nodes
        for (int i = 0; i < delivery.size() - 1; ++i){
            CourierSubPath next;
            next.start_intersection = delivery[i].nodeId;
            next.end_intersection = delivery[i + 1].nodeId;
            if (optimizePath[next.start_intersection][next.end_intersection].keyYToKeyXPath.empty()){
                update = false;
                break;
            } else {
                next.subpath = optimizePath[next.start_intersection][next.end_intersection].keyYToKeyXPath;
                courierOptimizingSecond.emplace_back(next);
                travelTimeOptimizingSecond += optimizePath[next.start_intersection][next.end_intersection].keyYToKeyXTime;
                newDeliveryOrderVector[i].travelTime = optimizePath[next.start_intersection][next.end_intersection].keyYToKeyXTime;
            }
        }

        if (update){
            // Find closest depot through last drop off
            int deliveryId = delivery[delivery.size() - 1].deliveryId[0];
            for (auto it = dropOffStart[deliveryId].begin(); it != dropOffStart[deliveryId].end(); ++it){
                nodeType nextType = (it->second).toNodeType[0];
                if (nextType == depot) {
                    CourierSubPath next;
                    next.start_intersection = (it->second).fromNode;
                    next.end_intersection = (it->second).toNode;
                    next.subpath = (it->second).path;
                    courierOptimizingSecond.emplace_back(next);
                    travelTimeOptimizingSecond += (it->second).travelTime;
                    newDeliveryOrderVector[delivery.size() - 1].travelTime = optimizePath[next.start_intersection][next.end_intersection].keyYToKeyXTime;
                    break;
                }
            }
        }
    }

    return update;
}

/**
 *
 * @param path
 * @return
 */
std::vector<DeliveryOrder> reversePath(std::vector<DeliveryOrder> path){
    std::vector<DeliveryOrder> reversedPath(path.rbegin(), path.rend());
    if (reversedPath.size() > 1){
        for (int i = 0; i < reversedPath.size() - 1; ++i){
            int from = reversedPath[i].nodeId;
            int to = path[i + 1].nodeId;
            if (reversedPath[i].travelTime == -1.0){
                noPath = true;
                break;
            } else {
                reversedPath[i].travelTime = optimizePath[from][to].keyYToKeyXTime;
            }
        }
    }
    return reversedPath;
}

/**
 * 4.15 travelTimeCalculator
 * calculate the travel time for every path
 * TEMPORARILY NO USE
 * @param delivery
 * @return
 */
double travelTimeCalculator(const std::vector<DeliveryOrder> &delivery) {
    double time = 0;
    int pathSize = delivery.size() - 1;
    for (int i = 0; i < pathSize; ++i) {
        time += delivery[i].travelTime;
    }
    return time;
}

/**
 * 4.16 legalityChecker
 * check legality of the path
 * @param delivery
 * @return
 */
bool legalityChecker(const std::vector<DeliveryOrder> &delivery) {
    // Initialization
    bool legal = false;
    int deliveryNumber = pickUpStart.size();
    deliveryConditionVector.clear();
    deliveryConditionVector.resize(deliveryNumber);
    for (int i = 0; i < deliveryNumber; ++i){
        deliveryConditionVector[i].pickUpVisited = false;
        deliveryConditionVector[i].dropOffVisited = false;
        deliveryConditionVector[i].onTruck = false;
    }

    for (int index = 0; index < delivery[0].deliveryType.size(); ++index) {
        if (delivery[0].deliveryType[index] == pickUp){
            deliveryConditionVector[delivery[0].deliveryId[index]].pickUpVisited = true;
            deliveryConditionVector[delivery[0].deliveryId[index]].onTruck = true;
        } else {
            legal = false;
            break;
        }
    }

    if (legal){
        for (int i = 1; i < delivery.size(); ++i){
            for (int index = 0; index < delivery[i].deliveryType.size(); ++index) {
                if (delivery[i].deliveryType[index] == pickUp){
                    if (!deliveryConditionVector[delivery[i].deliveryId[index]].pickUpVisited){
                        deliveryConditionVector[delivery[i].deliveryId[index]].onTruck = true;
                    }
                    deliveryConditionVector[delivery[i].deliveryId[index]].pickUpVisited = true;
                } else {
                    deliveryConditionVector[delivery[i].deliveryId[index]].dropOffVisited = true;
                    deliveryConditionVector[delivery[i].deliveryId[index]].onTruck = false;
                }
            }
        }
    }

    // Check whether all deliveries have made
    /*if (legal){
        for (int i = 0; i < deliveryNumber; ++i){
            if ((!deliveryConditionVector[i].pickUpVisited) || (!deliveryConditionVector[i].dropOffVisited)
                || (deliveryConditionVector[i].onTruck)){
                legal = false;
                break;
            }
        }
    }*/

    return legal;
}

