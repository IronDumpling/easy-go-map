//
// Created by ruyun on 2021-03-26.
//


#include "m1.h"
#include "m3.h"
#include <queue>
#include <vector>
#include <algorithm>
#include "MapDBStruct.h"

/**
 * m3 is the algorithm cpp
 *
 * Functions:
 * @param computePathTravelTime: based on computed optimal path and turn penalty in every direction change,
 *        get the ouput time
 *
 * @param findPathBetweenIntersections: based on translated input intersection id, we get the desired vector that
 * contains output segment ids
 *
 */

// Returns the time required to travel along the path specified, in seconds.
// The path is given as a vector of street segment ids, and this function can
// assume the vector either forms a legal path or has size == 0.The travel
// time is the sum of the length/speed-limit of each street segment, plus the
// given turn_penalty (in seconds) per turn implied by the path.If there is
// no turn, then there is no penalty. Note that whenever the street id changes
// (e.g. going from Bloor Street West to Bloor Street East) we have a turn.
double computePathTravelTime(const std::vector <StreetSegmentIdx> &path, const double turn_penalty) {
    // initialisation
    double travelTime = 0;
    int size = path.size();
    StreetIdx currentStreet = getStreetSegmentInfo(path[0]).streetID;
    StreetIdx previousStreet = currentStreet;

    for(int segment = 0; segment < size; ++segment){
        // Segment Travel Time
        int segmentID = path[segment];
        double segmentTime = mapDB.segment[segmentID].travelTime;

        travelTime += segmentTime;

        // Turn Penalty
        StreetSegmentInfo segmentINFO = getStreetSegmentInfo(segmentID);
        previousStreet = currentStreet;
        currentStreet = segmentINFO.streetID;

        if(currentStreet != previousStreet){
            travelTime += turn_penalty;
        }
    }
    
    return travelTime;
}

// Returns a path (route) between the start intersection and the destination
// intersection, if one exists. This routine should return the shortest path
// between the given intersections, where the time penalty to turn right or
// left is given by turn_penalty (in seconds).If no path exists, this routine
// returns an empty (size == 0) vector. If more than one path exists, the path
// with the shortest travel time is returned. The path is returned as a vector
// of street segment ids; traversing these street segments, in the returned
// order, would take one from the start to the destination intersection.
std::vector<StreetSegmentIdx> findPathBetweenIntersections(const IntersectionIdx startID, const IntersectionIdx destiID,
                                                           const double turnPenalty){
    // Check if the path is found
    bool pathFound = false;

    // Part 1. Initialise heuristic and parentID
    for(int intersectionID = 0; intersectionID < getNumIntersections(); ++intersectionID){
        // Part 1.1 Nodes' time are the largest
        mapDB.intersection[intersectionID].heuristic = std::numeric_limits<double>::max();
        mapDB.intersection[intersectionID].timeToHere = std::numeric_limits<double>::max();

        // Part 1.2 All the parent ID are out of bound
        mapDB.intersection[intersectionID].parentSegmentID = -1;
        mapDB.intersection[intersectionID].parentNodeID = -1;
        mapDB.intersection[intersectionID].parentStreetID = -1;

        // Part 1.3 All the nodes are not done
        mapDB.intersection[intersectionID].done = false;
    }

    // Part 1.4 Start Node's Time is 0
    mapDB.intersection[startID].timeToHere = 0;

    // Part 1.5 Initialize priority_queue
    std::priority_queue<MapDBStruct::IntersectionInfo, std::vector<MapDBStruct::IntersectionInfo>, MapDBStruct::Compare> waveFront;

    // Part 1.6 Initialise start intersection and destination intersection
    MapDBStruct::IntersectionInfo startNode = mapDB.intersection[startID];
    waveFront.push(startNode);

    // Part 2. Dijkstra's While loop
    while(!waveFront.empty()){
        // Part 2.1 Scan all unvisited node, Extract the minimum node currentNode out of wavefront
        MapDBStruct::IntersectionInfo currentNode = waveFront.top();
        waveFront.pop();

        // Store Current Wavefront Node's ID to access global information
        IntersectionIdx currentNodeID = currentNode.interID;

        // CurrentNode is Done (Prevent From been a child again)
        if (mapDB.intersection[currentNodeID].done)
            continue;
        else
            mapDB.intersection[currentNodeID].done = true;

        // End Case: Check if this node is the destination
        if(currentNodeID == destiID) {
            // Path has been found
            pathFound = true;
            // Break Loop
            break;
        }

        // Step 2. Scan
        // Update all connected & not visited & not oneway node's time & parent
        const double &parentStreetID = mapDB.intersection[currentNodeID].parentStreetID;

        for(int i = 0; i < mapDB.intersection[currentNodeID].segmentId.size(); ++i){
            // ConnectNode: The Node we are going to explore, from global
            IntersectionIdx connectNodeID;

            // 2.1 OneWay Test
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

            // 2.2 Done Test
            if(mapDB.intersection[connectNodeID].done)
                continue;

            // 2.3 Turn Penalty Test
            // Travel time initialisation
            const double &currentSegmentTravelTime = mapDB.segment[currentSegmentID].travelTime;
            double tempTimeToHere = 0;

            if(currentNodeID != startID && currentSegmentInfo.streetID != parentStreetID){
                tempTimeToHere = currentNode.timeToHere + currentSegmentTravelTime + turnPenalty;
            }else{
                tempTimeToHere = currentNode.timeToHere + currentSegmentTravelTime;
            }

           // 2.4 Heuristic Calculation
           double heuristic = findDistanceBetweenTwoPoints(std::make_pair(getIntersectionPosition(connectNodeID),
                                                                          getIntersectionPosition(destiID))) / maxSpeedLimit;
            double tempHeuristic = heuristic + tempTimeToHere;

            // Step 3. Add
            // Initialise a new element for Wavefront
            MapDBStruct::IntersectionInfo connectNode;
            connectNode.interID = connectNodeID;
            connectNode.heuristic = tempHeuristic;
            connectNode.timeToHere = tempTimeToHere;
            connectNode.parentSegmentID = currentSegmentID;
            connectNode.parentNodeID = currentNodeID;
            connectNode.parentStreetID = currentSegmentInfo.streetID;

            // Update message to the global if Time is Smaller
            if(connectNode.heuristic < mapDB.intersection[connectNodeID].heuristic) {
                mapDB.intersection[connectNodeID].heuristic = tempHeuristic;
                mapDB.intersection[connectNodeID].timeToHere = tempTimeToHere;
                mapDB.intersection[connectNodeID].parentSegmentID = currentSegmentID;
                mapDB.intersection[connectNodeID].parentNodeID = currentNodeID;
                mapDB.intersection[connectNodeID].parentStreetID = currentSegmentInfo.streetID;
            }
            // Push New Local Element into Queue
            waveFront.push(connectNode);
        }
    }

    // Initialise path vector
    std::vector<StreetSegmentIdx> path;

    // Corner case: Path not found
    if(!pathFound)
        return path;

    // General Case: Fetch the Path
    MapDBStruct::IntersectionInfo currentNode = mapDB.intersection[destiID];

    while(currentNode.interID != startID){
        // insert current segmentID into path
        path.insert(path.begin(), currentNode.parentSegmentID);

        // Go to parent Node
        currentNode = mapDB.intersection[currentNode.parentNodeID];
    }

    return path;
}



