#include "m1.h"
#include "m2.h"
#include "m3.h"
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "ezgl/camera.hpp"
#include "ezgl/color.hpp"
#include "MapDBStruct.h"
#include <stdio.h>
#include "bits/stdc++.h"
/**
 * m2.cpp is the drawing map cpp file
 *
 * @param1 Global Variables and Function Prototypes
 *         @param1.1 Global Variables
 *         @param1.2 Function Prototypes
 *
 * @param2 Helper Functions for Displaying on maps
 *         @param2.1 Coordination projection helpers for coordinating map display
 *                    @param2.1.1 latFromY is the callback function for transmitting y coordinate into LatLon position
 *                    @param2.1.2 latFromX is the callback function for transmitting X coordinate into LatLon position
 *
 *         @param2.2 High way width helper: The helper is used to set highway width in different scale
 *
 *         @param2.3 Major width helper: The helper is used to set major width in different scale
 *
 *         @param2.4 Minor width helper: The helper is used to set minor width in different scale
 *
 *         @param2.5 drawStreetLine: The helper is used to draw highway lines based on segment ID
 *
 *         @param2.6 drawStreetName: The helper is used to draw street name in different scale
 *
 *         @param2.7 Draw Street Helper: The helper is used to draw major and minor road in different scale
 *
 *         @param2.8 Draw street highlight helper: The helper is used with find button to highlight
 *                   corresponding street
 *
 *         @param2.9 Draw Intersection Helper: The helper is used to draw intersections
 *
 *         @param2.10 Draw Feature Helper:
 *
 *         @param2.11 Draw feature highlight helper
 *
 *         @param2.12 Draw feature name helper
 *
 *         @param2.13 POI type selector and drawer
 *
 *         @param2.14 Draw POI helper
 *
 *         @param2.15 draw legend
 *
 *         @param2.16 set world to Zoom Fit Level
 *
 *         @param2.17 draw  box
 *
 *         @param2.18 draw input text
 *
 *         @param2.19 draw output way direction in the map
 *
 *         @param2.20 load path from algorithm
 *
 *         @param2.21 basic Information Helper
 *
 *         @param2.22 path Turning Helper
 *
 *         @param2.23 Store Output information for multi segment Street
 *
 *         @param2.24 store output
 *
 *         @param2.25 draw output display
 *
 *         @param2.26 drawOutput function
 *
 *         @param2.27 setScreenForPath function
 *
 * @param3 Utility Functions: activate buttons
 *         @param3.1 Draw_main_canvas
 *
 *         @param3.2 Act on Mouse click
 *
 *         @param3.3 Act on Mouse CLick
 *
 *         @param3.4 Act On Mouse Move
 *
 *         @param3.5 Act on Key Press
 *
 *         @param3.6 Act on Find Button
 *
 *         @param3.7 press zoom in
 *
 *         @param3.8 Act on Change Map
 *
 *         @param3.9 act on Type List
 *
 *         @param3.10 scroll mounse helper
 *
 *         @param3.11 Act on Key Press
 *
 *         @param3.12 act On Key
 *
 *         @param3.13 act On Navigation
 *
 *         @param3.14 act On CitySeeker
 *
 *         @param3.15 act On Help
 *
 *         @param3.16 Other Callback Setup
 *
 * @param4 drawMap Functions: run the map with helpers and buttons
 *
 */


/**
 * 1.1 Global variables
 */
ezgl::application *appPtr = nullptr;

// Signals for city seeker mode
bool isFind = false;
bool isType = false;

// Strings for navigation mode
string firstInput = "Enter a start position";
string secondInput = "Enter a destination";
double intersection_id_start = 0;
double intersection_id_destination = 0;
// Signals for navigation mode
bool isNavigation = false;
bool drawFirstInput = false;
bool drawSecondInput = false;
bool readyForOutput = false;
bool firstDrew = false;
// Signals for mouse click navigation mode
bool selectStartIntersection = false;
bool selectDestination = false;
bool startFound = false;
bool destinationFound = false;
bool isUp = false;
bool isDown = false;

// Variables for output display
std::vector<StreetSegmentIdx> path;
int currentFirstLine = 0;
double travelTimeInSeconds = 0;


/**
 * 1.2 Function prototypes
 */
float latFromY(float y);
float lonFromX(float x);
void highwayWidth(ezgl::renderer *g, float scale);
void majorWidth(ezgl::renderer *g, float scale);
void minorWidth(ezgl::renderer *g, float scale);
void wayWidth(ezgl::renderer *g, float scale);
string removeLastChar(string s);
void drawStreetLine(float currentSpeed, int segmentID, int NumOfCurvePoints, ezgl::renderer* g, float currentScale);
void drawStreetName(ezgl::renderer *g, ezgl::camera &c, int segmentId);
void drawStreetHelper(ezgl::renderer *g, ezgl::camera &c);
void drawStreetHighlightHelper(ezgl::renderer *g, ezgl::camera &c);
void drawIntersectionHelper(ezgl::renderer *g, ezgl::camera &c, ezgl::surface *Icon,
                            ezgl::surface *flag, ezgl::surface *locate);
void drawFeatureHelper(ezgl::renderer *g, ezgl::camera &c);
void drawFeatureHighlightHelper(ezgl::renderer *g, ezgl::camera &c, ezgl::surface *highlight);
void stringSpliter(ezgl::renderer* g, int currentScale, int i, ezgl::point2d textPosition, bool isPOI);
void drawFeatureNameHelper(ezgl::renderer *g, ezgl::camera &c);
void typeSelector(ezgl::renderer *g, int i, float x, float y, float width, float height, ezgl::surface *icon,
                  ezgl::surface *parking, ezgl::surface *restaurant, ezgl::surface *school, ezgl::surface *theatre,
                  ezgl::surface *bar, ezgl::surface *bus, ezgl::surface *hospital, ezgl::surface *bank,
                  ezgl::surface *gas, ezgl::surface *police, ezgl::surface *cinema);
void drawPOIHelper(ezgl::renderer *g, ezgl::camera &c, ezgl::surface *icon,
                   ezgl::surface *parking, ezgl::surface *restaurant, ezgl::surface *school, ezgl::surface *theatre,
                   ezgl::surface *bar, ezgl::surface *bus, ezgl::surface *hospital, ezgl::surface *bank,
                   ezgl::surface *gas, ezgl::surface *police, ezgl::surface *highlight, ezgl::surface *cinema);
void drawLegend(ezgl::renderer *g);
void setScreen(ezgl::application *application, bool isStreet, bool isPOI, bool isFeature, bool isIntersection);
void worldToFit(ezgl::application *app);
void drawBox(ezgl::renderer *g, ezgl::surface *detective, ezgl::surface *close,
             ezgl::surface *transfer, ezgl::surface *direct);
void drawInput(ezgl::renderer *g);
void loadPath(const IntersectionIdx intersect_id_start, const IntersectionIdx intersect_id_destination,
              const double turn_penalty);
void drawOutputHelper(ezgl::renderer *g, std::vector<StreetSegmentIdx> path, ezgl::camera &c);
void basicInformationHelper(double travelTimeInSeconds);
void pathTurningHelper(double crossProduct, string nextStreetName);
void storeForBet(std::vector <StreetSegmentIdx> input, double travelTimeInSeconds);
void storeOutput(std::vector <StreetSegmentIdx> path, double travelTimeInSeconds);
void noValidPath(ezgl::renderer *g, ezgl::surface *alram);
void drawOutput(ezgl::renderer *g, bool up, bool down, ezgl::surface *car, ezgl::surface *clock,
                ezgl::surface *flag, ezgl::surface *loc, ezgl::surface *more, ezgl::surface *left,
                ezgl::surface *right, ezgl::surface *straight);
void setScreenForPath(ezgl::application *application);
void press_zoom_in(GtkWidget *widget, ezgl::application *application, ezgl::renderer *g);
void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data, ezgl::application *application);
void actOnMouseClick(ezgl::application *app, GdkEventButton *event, double x, double y);
void actOnScrollMouse(GtkWidget *, GdkEvent *event, gpointer data);
void actOnFind(GtkWidget *, ezgl::application *application);
gboolean actOnChangeMap(GtkComboBox *combo_box, ezgl::application *application);
void actOnTypeList(GtkComboBox *combo_box, ezgl::application *application);
void actOnKey(ezgl::application *app, GdkEventKey *, char *key_name);
void actOnNavigation(GtkWidget *widget, ezgl::application *application);
void actOnCitySeeker(GtkWidget *widget, ezgl::application *application);
void actOnHelp(GtkWidget *, ezgl::application *app);
void initialSetUp(ezgl::application *app, bool);

/**
 * 2.1 Coordination projection helpers for coordinating map display
 */
float latFromY(float y) {
    return y / (kDegreeToRadian * kEarthRadiusInMeters);
}

float lonFromX(float x) {
    return x / (kDegreeToRadian * kEarthRadiusInMeters * std::cos(avgLat * kDegreeToRadian));
}

/**
 * 2.2 High way width helper
 * @param inputs
 * @param used g
 * @param used current screen scale
 */
void highwayWidth(ezgl::renderer *g, float scale) {
    if (scale > 300) {
        g->set_line_width(2 * log10(scale));
    }else if(scale > 20){
        g->set_line_width(3 * log10(scale));
    }else if(scale >5){
        g->set_line_width(5 * log10(scale));
    }else if (scale > 1) {
        g->set_line_width(5 * scale);
    }else if (scale < 1) {
        g->set_line_width(50 / scale);
    }
}

/**
 * 2.3 Major width helper
 * @param inputs
 * @param used g
 * @param used current screen scale
 */
void majorWidth(ezgl::renderer *g, float scale) {
    if (scale < 45 && scale > 29) {
        g->set_line_width(scale / 6);
    } else if (scale < 29 && scale > 25) {
        g->set_line_width(scale / 6);
    } else if (scale > 17) {
        g->set_line_width(scale / 5.5);
    } else if (scale > 12){
        g->set_line_width(scale / 3);
    } else if (scale > 11) {
        g->set_line_width(scale / 2.5);
    } else if (scale < 11 && scale > 7){
        g->set_line_width(scale / 1.5);
    } else if (scale < 7 && scale > 1.5) {
        g->set_line_width(scale * exp(-scale / 2) * 25);
    } else if (scale < 1.5) {
        g->set_line_width(20 / scale);
    }
}

/**
 * 2.4 Minor width helper
 * @param inputs
 * @param g
 * @param scale
 */
void minorWidth(ezgl::renderer *g, float scale) {
    if(scale < 15 && scale > 12){
        g->set_line_width(3 * log10(scale));
    }else if (scale > 7.5) {
        g->set_line_width(4 * log10(scale));
    }else if (scale > 1) {
        g->set_line_width(scale * exp(-scale / 2) * 11);
    } else if (scale < 1) {
        g->set_line_width(10 / scale);
    }
}

/**
 * 2.5 drawStreetLine
 * @param inputs
 * @param currentSpeed
 * @param segmentID
 * @param NumOfCurvePoints
 * @param g
 * @param currentScale
 */
void drawStreetLine(float currentSpeed, int segmentID, int NumOfCurvePoints, ezgl::renderer* g, float currentScale) {

    float highwaySpeedLimit = 90 / 3.6;
    float majorLimit = 50 / 3.6;
    float minorLimit = 30 / 3.6;

    // draw line between from position and first curve point
    ezgl::point2d from = mapDB.segment[segmentID].xyLocation[0];
    ezgl::point2d to = mapDB.segment[segmentID].xyLocation[NumOfCurvePoints + 1];

    if (NumOfCurvePoints == 0) {
        if (currentSpeed > majorLimit && currentSpeed < highwaySpeedLimit && currentScale < 45) {
            g->set_color(ezgl::MAJORROAD);
            majorWidth(g, currentScale);
            g->draw_line(from, to);
        }

        if (currentSpeed < majorLimit && currentSpeed > minorLimit && currentScale < 15) {
            g->set_color(ezgl::MINORROAD);
            //set width
            minorWidth(g, currentScale);
            g->draw_line(from, to);
        }
    } else {
        // draw from first curvePoint
        ezgl::point2d firstCurvePoint = mapDB.segment[segmentID].xyLocation[1];

        if (currentSpeed < majorLimit && currentSpeed > minorLimit && currentScale < 15) {
            g->set_color(ezgl::MINORROAD);
            //set width
            minorWidth(g, currentScale);
            g->draw_line(from, firstCurvePoint);
        }

        if (currentSpeed > majorLimit && currentSpeed < highwaySpeedLimit && currentScale < 45) {
            g->set_color(ezgl::MAJORROAD);
            //set width
            majorWidth(g, currentScale);
            g->draw_line(from, firstCurvePoint);
        }

        // traverse from the second to the last
        for (int curvePointIndex = 0; curvePointIndex < NumOfCurvePoints; curvePointIndex++) {
            ezgl::point2d start = mapDB.segment[segmentID].xyLocation[curvePointIndex + 1];
            ezgl::point2d end = mapDB.segment[segmentID].xyLocation[curvePointIndex + 2];

            if (currentSpeed < majorLimit && currentSpeed > minorLimit && currentScale < 15) {
                g->set_color(ezgl::MINORROAD);
                //set width
                minorWidth(g, currentScale);
                g->draw_line(start, end);
                g->set_line_cap(ezgl::line_cap(1));
            }
            if (currentSpeed > majorLimit && currentSpeed < highwaySpeedLimit && currentScale < 45) {
                g->set_color(ezgl::MAJORROAD);
                majorWidth(g, currentScale);
                g->draw_line(start, end);
                g->set_line_cap(ezgl::line_cap(1));
            }
        }

        // draw line between the last curvepoint and to position
        ezgl::point2d lastCurvePointPosition = mapDB.segment[segmentID].xyLocation[NumOfCurvePoints];
        if (currentSpeed < majorLimit && currentSpeed > minorLimit && currentScale < 15) {
            g->set_color(ezgl::MINORROAD);
            //set width
            minorWidth(g, currentScale);
            g->draw_line(lastCurvePointPosition, to);
            g->set_line_cap(ezgl::line_cap(1));
        }

        if (currentSpeed > majorLimit && currentSpeed < highwaySpeedLimit && currentScale < 45) {
            g->set_color(ezgl::MAJORROAD);
            //set width
            majorWidth(g, currentScale);
            g->draw_line(lastCurvePointPosition, to);
            g->set_line_cap(ezgl::line_cap(1));
        }
    }
}

/**
 *2.6 Draw street name
 * @param g
 * @param c
 * @param SegmentId
 */

void drawStreetName(ezgl::renderer *g, ezgl::camera &c, int segmentId) {
    // 1. Initialise Needed variables
    ezgl::point2d currentScale = c.get_world_scale_factor();

    // 2. access to the street name of current segment
    StreetSegmentInfo segmentInfo = getStreetSegmentInfo(segmentId);
    StreetIdx streetId = segmentInfo.streetID;
    std::string currentStreetName = getStreetName(streetId);

    // 3. access to the angle of current segment
    double angle = mapDB.segment[segmentId].angle;

    // 4. Set Font format and color
    if (currentScale.y > 60 && currentScale.y < 1000) { ;
    } else if (currentScale.y < 60 && currentScale.y > 40) {
        g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                       ezgl::font_weight::bold, 9);
    } else if (currentScale.y < 40 && currentScale.y > 30) {
        g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                       ezgl::font_weight::bold, 7);
    } else if (currentScale.y < 30 && currentScale.y > 15) {
        g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                       ezgl::font_weight::bold, 8);
    } else if (currentScale.y < 15 && currentScale.y > 7.3) {
        g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                       ezgl::font_weight::bold, 7.5);
    } else if (currentScale.y < 7.3 && currentScale.y > 3){
        g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                       ezgl::font_weight::bold, 10);
    } else {
        g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                       ezgl::font_weight::bold, 10);
    }
    g->set_color(ezgl::MAJORFONT);

    // 5 Draw Text
    if (currentStreetName != "<unknown>") {
        if (angle > 90 && angle < 270) {
            angle -= 180;
            currentStreetName = std::string(segmentInfo.oneWay ? "←" : "→").append(currentStreetName).append("←");
        } else {
            currentStreetName = std::string(segmentInfo.oneWay ? "→" : "←").append(currentStreetName).append("→");
        }
        g->set_text_rotation(angle);
        g->draw_text(mapDB.segment[segmentId].lastMidposition, currentStreetName);
    }
}

/**
 * 2.7 Draw street helper
 * @param g
 * @param currentCanvas
 */
void drawStreetHelper(ezgl::renderer *g, ezgl::camera &c) {
    ezgl::point2d currentScale = c.get_world_scale_factor();
    ezgl::rectangle currentScreen = g->get_visible_world();
    float highwaySpeedLimit = 90 / 3.6;
    float majorLimit = 50 / 3.6;
    float maxSpeed;

    // 1. Draw Highway
    for (StreetSegmentIdx segmentID = 0; segmentID < mapDB.segment.size(); segmentID++) {
        // check out of screen
        if (!currentScreen.contains(mapDB.segment[segmentID].lastMidposition)) {
            continue;
        }

        // initialise need value
        StreetSegmentInfo segmentInfo = getStreetSegmentInfo(segmentID);
        float currentSpeed = segmentInfo.speedLimit;
        int NumOfCurvePoints = segmentInfo.numCurvePoints;

        // draw line between from position and first curve point
        ezgl::point2d from = mapDB.segment[segmentID].xyLocation[0];
        ezgl::point2d to = mapDB.segment[segmentID].xyLocation[NumOfCurvePoints + 1];

        // classify by curvePoint number
        if (NumOfCurvePoints == 0) {
            if (currentSpeed > highwaySpeedLimit) {
                // set highway width
                highwayWidth(g, currentScale.x);
                g->set_color(ezgl::HIGHWAY);
                g->draw_line(from, to);
            }
        } else {
            // NUM OF CURVE POINTS>0
            if (currentSpeed > highwaySpeedLimit) {
                highwayWidth(g, currentScale.x);
                g->set_color(ezgl::HIGHWAY);

                // draw from first curvePoint
                ezgl::point2d firstCurvePoint = mapDB.segment[segmentID].xyLocation[1];
                g->draw_line(from, firstCurvePoint);

                // traverse from the second to the last
                for (int curvePointIndex = 0; curvePointIndex < NumOfCurvePoints; curvePointIndex++) {
                    g->set_color(ezgl::HIGHWAY);
                    ezgl::point2d start = mapDB.segment[segmentID].xyLocation[curvePointIndex + 1];
                    ezgl::point2d end = mapDB.segment[segmentID].xyLocation[curvePointIndex + 2];
                    g->draw_line(start, end);
                    g->set_line_cap(ezgl::line_cap(1));
                }

                // draw line between the last curvepoint and to position
                ezgl::point2d lastCurvePointPosition = mapDB.segment[segmentID].xyLocation[NumOfCurvePoints];
                g->set_color(ezgl::HIGHWAY);
                g->draw_line(lastCurvePointPosition, to);

            }
        }
    }

    // 2. Draw all streets
    if (currentScale.x < 45) {
        //find max speed limit
        for (int currentStreetIndex = 0; currentStreetIndex < getNumStreets(); currentStreetIndex++) {
            //the first loop is to find the maximum speed limit in the street
            //loop through all segments to find the maximum speedLimit
            maxSpeed = 0;
            for (int i = 0; i < mapDB.street[currentStreetIndex].segmentsOfStreet.size(); i++) {
                //for the segment in the street
                StreetSegmentIdx currentSegnmentIndex = mapDB.street[currentStreetIndex].segmentsOfStreet[i];
                StreetSegmentInfo segmentIntheStreet = getStreetSegmentInfo(currentSegnmentIndex);

                //find max speed
                float speedIntheStreet = segmentIntheStreet.speedLimit;
                maxSpeed = std::max(maxSpeed, speedIntheStreet);
            }

            //after we find the maximum speed in the street
            //draw all segments in the street
            for (int i = 0; i < mapDB.street[currentStreetIndex].segmentsOfStreet.size(); i++) {
                StreetSegmentIdx currentSegmentIndex = mapDB.street[currentStreetIndex].segmentsOfStreet[i];
                StreetSegmentInfo segmentInfo = getStreetSegmentInfo(currentSegmentIndex);
                int NumOfCurvePoints = segmentInfo.numCurvePoints;

                //3.1 Check whether the segment is within the range
                if (!currentScreen.contains(mapDB.segment[currentSegmentIndex].lastMidposition)) {
                    continue;
                }

                //draw each segment
                drawStreetLine(maxSpeed, currentSegmentIndex, NumOfCurvePoints, g, currentScale.x);
            }
        }
    }

    // 3. Draw Highway Name
    for (StreetSegmentIdx segmentID = 0; segmentID < mapDB.segment.size(); segmentID++) {
        // check out of screen
        if (!currentScreen.contains(mapDB.segment[segmentID].lastMidposition)) {
            continue;
        }

        // initialise needed value
        StreetSegmentInfo segmentInfo = getStreetSegmentInfo(segmentID);
        float currentSpeed = segmentInfo.speedLimit;
        int NumOfCurvePoints = segmentInfo.numCurvePoints;
        int segmentLength = mapDB.segment[segmentID].segmentLength;

        //Get Street ID
        StreetIdx streetId = segmentInfo.streetID;

        //1. level 1: Draw Highway
        if(currentSpeed > highwaySpeedLimit && segmentLength > currentScale.x / 2
           && currentScale.x > 1 && NumOfCurvePoints == 0) {
            int i = 0;
            for (i = 0; i < mapDB.street[streetId].segmentsOfStreet.size(); i++) {
                if (mapDB.street[streetId].segmentsOfStreet[i] == segmentID)
                    break;
            }

            /*if ((i + 1) % 20 == 0 && segmentID % 10 == 0)
                drawStreetName(g, c, segmentID);*/
        }

        //2. level 2: Draw Major Street
        if(currentSpeed < highwaySpeedLimit && currentSpeed >= majorLimit &&
           currentScale.x < 17 && currentScale.x > 1 && NumOfCurvePoints == 0) {
            int i = 0;
            for (i = 0; i < mapDB.street[streetId].segmentsOfStreet.size(); i++) {
                if (mapDB.street[streetId].segmentsOfStreet[i] == segmentID)
                    break;
            }
            /*if ((i + 1) % 3 == 0 && segmentID % 9 == 0)
                drawStreetName(g, c, segmentID);*/
        }

        // 2. level 3: Draw Minor Street
        if(currentSpeed < majorLimit && currentScale.x < 4 && currentScale.x > 1) {
            int i = 0;
            for (i = 0; i < mapDB.street[streetId].segmentsOfStreet.size(); i++) {
                if (mapDB.street[streetId].segmentsOfStreet[i] == segmentID)
                    break;
            }
            /*if ((i + 1) % 3 == 0 && segmentID % 5 == 0)
                drawStreetName(g, c, segmentID);*/
        }

        // 3. level 4: Draw Every Street
        if(currentScale.x <= 1) {
            int i = 0;
            for (i = 0; i < mapDB.street[streetId].segmentsOfStreet.size(); i++) {
                if (mapDB.street[streetId].segmentsOfStreet[i] == segmentID)
                    break;
            }
            /*if ((i + 1) % 5 == 0)
                drawStreetName(g, c, segmentID);*/


        }
    }
}

/**
 * 2.8 Draw street highlight helper
 * @param g
 * @param c
 */
void drawStreetHighlightHelper(ezgl::renderer *g, ezgl::camera &c) {
    ezgl::point2d currentScale = c.get_world_scale_factor();
    for (StreetSegmentIdx segmentID = 0; segmentID < mapDB.segment.size(); segmentID++){
        if (mapDB.segment[segmentID].highlight){
            int NumOfCurvePoints = mapDB.segment[segmentID].xyLocation.size();
            for (int curvePointIndex = 0; curvePointIndex < NumOfCurvePoints - 1; curvePointIndex++) {
                ezgl::point2d start = mapDB.segment[segmentID].xyLocation[curvePointIndex];
                ezgl::point2d end = mapDB.segment[segmentID].xyLocation[curvePointIndex + 1];
                g->set_color(ezgl::HIGHTLIGHT);
                majorWidth(g, currentScale.x);
                g->draw_line(start, end);
                g->set_line_cap(ezgl::line_cap(1));
            }
        }
    }
}

/**
 * 2.9 Draw Intersection Helper
 * @param g
 * @param currentCanvas
 */
void drawIntersectionHelper(ezgl::renderer *g, ezgl::camera &c, ezgl::surface *Icon,
                            ezgl::surface *flag, ezgl::surface *locate){
    ezgl::point2d currentScale = c.get_world_scale_factor();
    g->set_coordinate_system(ezgl::WORLD);

    for (size_t i = 0; i < mapDB.intersection.size(); ++i) {
        if (mapDB.intersection[i].start){
            float x = mapDB.intersection[i].xyLocation.x;
            float y = mapDB.intersection[i].xyLocation.y;

            float width = 25 * currentScale.x;
            float height = 50 * currentScale.y;

            //set color of intersections
            g->set_color(ezgl::HIGHTLIGHT);

            //set shape of the intersections
            g->draw_surface(locate, {x - width * 2 / 3, y + height / 2});
        } else if (mapDB.intersection[i].destination){
            float x = mapDB.intersection[i].xyLocation.x;
            float y = mapDB.intersection[i].xyLocation.y;

            float width = 25 * currentScale.x;
            float height = 50 * currentScale.y;

            //set color of intersections
            g->set_color(ezgl::HIGHTLIGHT);

            //set shape of the intersections
            g->draw_surface(flag, {x - width / 3, y + height / 2});
        } else {
            if (mapDB.intersection[i].highlight) {
                float x = mapDB.intersection[i].xyLocation.x;
                float y = mapDB.intersection[i].xyLocation.y;

                float width = 25 * currentScale.x;
                float height = 50 * currentScale.y;

                //set color of intersections
                g->set_color(ezgl::HIGHTLIGHT);

                //set shape of the intersections
                g->draw_surface(Icon, {x - width, y + height});
            }
        }
    }
}

/**
 * 2.9 Draw feature helper
 * @param g
 * @param currentCanvas
 */
void drawFeatureHelper(ezgl::renderer *g, ezgl::camera &c) {
    for (auto iter = mapDB.featureOrder.rbegin(); iter != mapDB.featureOrder.rend(); ++iter) {
        int i = iter->second;
        ezgl::point2d currentScale = c.get_world_scale_factor();
        int pointNum = mapDB.feature[i].latLonLocation.size();

        // Check whether it is a polygon or polyline
        LatLon start = mapDB.feature[i].latLonLocation[0];
        LatLon end = mapDB.feature[i].latLonLocation[pointNum - 1];

        if (currentScale.x > 60) {
            // The outermost level not displaying beach, building, stream, and unknown type of features
            // Use the same colour to display park, greenspace, and golfcourse
            if (!(start == end)) {
                if ((mapDB.feature[i].type == PARK) || (mapDB.feature[i].type == GREENSPACE)
                    || (mapDB.feature[i].type == GOLFCOURSE)) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::LIGHTGREEN);
                        g->draw_line(last, current);
                    }
                } else if (mapDB.feature[i].type == ISLAND) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::ISLAND);
                        g->draw_line(last, current);
                    }
                } else if ((mapDB.feature[i].type == LAKE) || (mapDB.feature[i].type == RIVER)) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::WATER);
                        //set line width
                        g->draw_line(last, current);
                    }
                } else {}
            } else {
                if (pointNum != 1) {
                    if ((mapDB.feature[i].type == PARK) || (mapDB.feature[i].type == GREENSPACE)
                        || (mapDB.feature[i].type == GOLFCOURSE)) {
                        if (findFeatureArea(i) > 7500 * currentScale.x) {
                            g->set_color(ezgl::LIGHTGREEN);
                            g->fill_poly(mapDB.feature[i].xyLocation);
                        }
                    } else if (mapDB.feature[i].type == ISLAND) {
                        g->set_color(ezgl::ISLAND);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if ((mapDB.feature[i].type == LAKE) || (mapDB.feature[i].type == RIVER)) {
                        g->set_color(ezgl::WATER);
                        //set line width
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else {}
                }
            }
        } else if (currentScale.x <= 60 && currentScale.x > 40) {
            // The middle level not displaying beach and unknown type
            // Use different colour to classify park, greenspace, and golfcourse
            if (!(start == end)) {
                if (mapDB.feature[i].type == PARK) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::LIGHTGREEN);
                        g->draw_line(last, current);
                    }
                } else if ((mapDB.feature[i].type == GREENSPACE) || (mapDB.feature[i].type == GOLFCOURSE)) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::DARKGREEN);
                        g->draw_line(last, current);
                    }
                } else if (mapDB.feature[i].type == BUILDING) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::BUILDING);
                        g->draw_line(last, current);
                    }
                } else if ((mapDB.feature[i].type == LAKE) || (mapDB.feature[i].type == RIVER)
                           || (mapDB.feature[i].type == STREAM)) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::WATER);
                        g->draw_line(last, current);
                    }
                } else if (mapDB.feature[i].type == ISLAND) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::ISLAND);
                        g->draw_line(last, current);
                    }
                } else {}
            } else {
                if (pointNum != 1) {
                    if (mapDB.feature[i].type == PARK) {
                        g->set_color(ezgl::LIGHTGREEN);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if ((mapDB.feature[i].type == GREENSPACE) || (mapDB.feature[i].type == GOLFCOURSE)) {
                        g->set_color(ezgl::DARKGREEN);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if (mapDB.feature[i].type == BUILDING) {
                        g->set_color(ezgl::BUILDING);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if ((mapDB.feature[i].type == LAKE) || (mapDB.feature[i].type == RIVER)
                               || (mapDB.feature[i].type == STREAM)) {
                        g->set_color(ezgl::WATER);
                        //set line width
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if (mapDB.feature[i].type == ISLAND) {
                        g->set_color(ezgl::ISLAND);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else {}
                }
            }
        } else {
            // The innermost scale displaying every feature types
            if (!(start == end)) {
                if (mapDB.feature[i].type == PARK) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::LIGHTGREEN);
                        g->draw_line(last, current);
                    }
                } else if ((mapDB.feature[i].type == GREENSPACE) || (mapDB.feature[i].type == GOLFCOURSE)) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::DARKGREEN);
                        g->draw_line(last, current);
                    }
                } else if (mapDB.feature[i].type == BUILDING) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::BUILDING);
                        g->draw_line(last, current);
                    }
                } else if ((mapDB.feature[i].type == LAKE) || (mapDB.feature[i].type == RIVER)
                           || (mapDB.feature[i].type == STREAM)) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::WATER);
                        g->draw_line(last, current);
                    }
                } else if (mapDB.feature[i].type == ISLAND) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::ISLAND);
                        g->draw_line(last, current);
                    }
                } else if (mapDB.feature[i].type == BEACH) {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::BEACH);
                        g->draw_line(last, current);
                    }
                } else {
                    for (int num = 1; num < pointNum; ++num) {
                        ezgl::point2d last = mapDB.feature[i].xyLocation[num - 1];
                        ezgl::point2d current = mapDB.feature[i].xyLocation[num];
                        g->set_color(ezgl::UNKNOWN);
                        g->draw_line(last, current);
                    }
                }
            } else {
                if (pointNum != 1) {
                    if (mapDB.feature[i].type == PARK) {
                        g->set_color(ezgl::LIGHTGREEN);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if ((mapDB.feature[i].type == GREENSPACE) || (mapDB.feature[i].type == GOLFCOURSE)) {
                        g->set_color(ezgl::DARKGREEN);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if (mapDB.feature[i].type == BUILDING) {
                        g->set_color(ezgl::BUILDING);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if ((mapDB.feature[i].type == LAKE) || (mapDB.feature[i].type == RIVER)
                               || (mapDB.feature[i].type == STREAM)) {
                        g->set_color(ezgl::WATER);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if (mapDB.feature[i].type == ISLAND) {
                        g->set_color(ezgl::ISLAND);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else if (mapDB.feature[i].type == BEACH) {
                        g->set_color(ezgl::BEACH);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    } else {
                        g->set_color(ezgl::UNKNOWN);
                        g->fill_poly(mapDB.feature[i].xyLocation);
                    }
                }
            }
        }
    }
}

/**
 * 2.11 Draw feature highlight helper
 * @param g
 * @param c
 * @param highlight
 */
void drawFeatureHighlightHelper(ezgl::renderer *g, ezgl::camera &c, ezgl::surface *highlight) {
    for (auto iter = mapDB.featureOrder.rbegin(); iter != mapDB.featureOrder.rend(); ++iter) {
        int i = iter->second;
        ezgl::point2d currentScale = c.get_world_scale_factor();
        int pointNum = mapDB.feature[i].latLonLocation.size();

        // Check whether it is a polygon or polyline
        LatLon start = mapDB.feature[i].latLonLocation[0];
        LatLon end = mapDB.feature[i].latLonLocation[pointNum - 1];

        if (mapDB.feature[i].highlight) {
            if (!(start == end)) {
                double xMax = mapDB.feature[i].xyLocation[0].x;
                double xMin = mapDB.feature[i].xyLocation[0].x;
                double yMax = mapDB.feature[i].xyLocation[0].y;
                double yMin = mapDB.feature[i].xyLocation[0].y;
                for (int num = 0; num < pointNum; ++num) {
                    if (mapDB.feature[i].xyLocation[num].x > xMax) {
                        xMax = mapDB.feature[i].xyLocation[num].x;
                    }
                    if (mapDB.feature[i].xyLocation[num].x < xMin) {
                        xMin = mapDB.feature[i].xyLocation[num].x;
                    }
                    if (mapDB.feature[i].xyLocation[num].y > yMax) {
                        yMax = mapDB.feature[i].xyLocation[num].y;
                    }
                    if (mapDB.feature[i].xyLocation[num].y < yMin) {
                        yMin = mapDB.feature[i].xyLocation[num].y;
                    }
                }
                ezgl::point2d iconPosition;
                iconPosition.x = (xMax + xMin) / 2;
                iconPosition.y = (yMax + yMin) / 2;
                g->draw_surface(highlight,
                                {iconPosition.x - 25 * currentScale.x, iconPosition.y + 50 * currentScale.x});
            } else {
                double xMax = mapDB.feature[i].xyLocation[0].x;
                double xMin = mapDB.feature[i].xyLocation[0].x;
                double yMax = mapDB.feature[i].xyLocation[0].y;
                double yMin = mapDB.feature[i].xyLocation[0].y;
                for (int num = 0; num < pointNum; ++num) {
                    if (mapDB.feature[i].xyLocation[num].x > xMax) {
                        xMax = mapDB.feature[i].xyLocation[num].x;
                    }
                    if (mapDB.feature[i].xyLocation[num].x < xMin) {
                        xMin = mapDB.feature[i].xyLocation[num].x;
                    }
                    if (mapDB.feature[i].xyLocation[num].y > yMax) {
                        yMax = mapDB.feature[i].xyLocation[num].y;
                    }
                    if (mapDB.feature[i].xyLocation[num].y < yMin) {
                        yMin = mapDB.feature[i].xyLocation[num].y;
                    }
                }
                ezgl::point2d iconPosition;
                iconPosition.x = (xMax + xMin) / 2;
                iconPosition.y = (yMax + yMin) / 2;
                g->draw_surface(highlight,
                                {iconPosition.x - 25 * currentScale.x, iconPosition.y + 50 * currentScale.x});
            }
        }
    }
}

/**
 * Split the string into multiple strings
 * @param g
 * @param currentScale
 * @param i
 * @param textPosition
 */
void stringSpliter(ezgl::renderer* g, int currentScale, int i, ezgl::point2d textPosition, bool isPOI){
    if(isPOI) {
        istringstream isName(mapDB.pointOfInt[i].name);
        vector<string> names{istream_iterator<string>{isName},
                             istream_iterator<string>{}};
        for (int j = 0; j < names.size(); ++j){
            g->set_text_rotation(0);
            g->draw_text({textPosition.x, textPosition.y - j * 14 * currentScale} , names[j]);
        }
    }else {
        istringstream isName(mapDB.feature[i].name);
        vector<string> names{istream_iterator<string>{isName},
                             istream_iterator<string>{}};

        if (names.size() > 4) {
            for (int j = 0; j < names.size(); ++j){
                // If it is an even number
                if (names.size() % 2 == 0){
                    if (j <= names.size() / 2) {
                        g->set_text_rotation(0);
                        g->draw_text({textPosition.x,
                                      textPosition.y + names.size() * 6.5 * currentScale
                                      - j * 13 * currentScale} , names[j]);
                    } else {
                        g->set_text_rotation(0);
                        g->draw_text({textPosition.x,
                                      textPosition.y - (j - names.size() / 2) * 13 * currentScale}, names[j]);
                    }
                } else {
                    if (j <= (names.size() - 1) / 2) {
                        g->set_text_rotation(0);
                        g->draw_text({textPosition.x,
                                      textPosition.y + (names.size() - 1) * 6.5 * currentScale
                                      - j * 13 * currentScale} , names[j]);
                    } else {
                        g->set_text_rotation(0);
                        g->draw_text({textPosition.x,
                                      textPosition.y - (j - (names.size() - 1) / 2) * 13 * currentScale},
                                     names[j]);
                    }
                }
            }
        } else {
            g->set_text_rotation(0);
            g->draw_text(textPosition, mapDB.feature[i].name);
        }
    }
}

/**
 * 2.12 Draw feature name helper
 * @param g
 * @param c
 */
void drawFeatureNameHelper(ezgl::renderer *g, ezgl::camera &c) {
    // Display names of major features
    for (size_t i = 0; i < mapDB.feature.size(); ++i) {
        ezgl::point2d currentScale = c.get_world_scale_factor();
        int pointNum = mapDB.feature[i].latLonLocation.size();

        // Only display names for closed polygons
        if ((currentScale.x < 30) && (findFeatureArea(i) > 17500 * currentScale.x) && (mapDB.feature[i].name != "0")) {

            // Find position for text
            double xMax = mapDB.feature[i].xyLocation[0].x;
            double xMin = mapDB.feature[i].xyLocation[0].x;
            double yMax = mapDB.feature[i].xyLocation[0].y;
            double yMin = mapDB.feature[i].xyLocation[0].y;
            for (int num = 0; num < pointNum; ++num) {
                if (mapDB.feature[i].xyLocation[num].x > xMax) {
                    xMax = mapDB.feature[i].xyLocation[num].x;
                }
                if (mapDB.feature[i].xyLocation[num].x < xMin) {
                    xMin = mapDB.feature[i].xyLocation[num].x;
                }
                if (mapDB.feature[i].xyLocation[num].y > yMax) {
                    yMax = mapDB.feature[i].xyLocation[num].y;
                }
                if (mapDB.feature[i].xyLocation[num].y < yMin) {
                    yMin = mapDB.feature[i].xyLocation[num].y;
                }
            }
            ezgl::point2d textPosition;
            textPosition.x = (xMax + xMin) / 2;
            textPosition.y = (yMax + yMin) / 2;

            // Draw text
            g->set_color(ezgl::MINORFONT);
            if (currentScale.x >= 25) {
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, currentScale.x / 3.5);
            } else if ((currentScale.x < 25) && (currentScale.x >= 1)) {
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 10.5);
            } else {
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 8.5);
            }
            stringSpliter(g, currentScale.x, i, textPosition, false);
        }
    }
}

/**
 * 2.13 POI type selector and drawer
 * @param g
 * @param i
 * @param x
 * @param y
 * @param width
 * @param height
 * @param currentScale
 * @param icon
 * @param parking
 * @param restaurant
 * @param school
 * @param theatre
 * @param bar
 * @param bus
 * @param hospital
 * @param bank
 * @param gas
 * @param police
 * @param highlight
 */
void typeSelector(ezgl::renderer *g, int i, float x, float y, float width, float height, ezgl::surface *icon,
                  ezgl::surface *parking, ezgl::surface *restaurant, ezgl::surface *school, ezgl::surface *theatre,
                  ezgl::surface *bar, ezgl::surface *bus, ezgl::surface *hospital, ezgl::surface *bank,
                  ezgl::surface *gas, ezgl::surface *police, ezgl::surface *cinema) {
    if (!mapDB.pointOfInt[i].highlight) {
        if (mapDB.pointOfInt[i].type == "parking" || mapDB.pointOfInt[i].type == "parking_entrance") {
            g->draw_surface(parking, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "theatre") {
            g->draw_surface(theatre, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "cinema"){
            g->draw_surface(cinema, {x - width, y + height});
        }else if (mapDB.pointOfInt[i].type == "fast_food" || mapDB.pointOfInt[i].type == "restaurant"|| mapDB.pointOfInt[i].type == "cafe") {
            g->draw_surface(restaurant, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "library" || mapDB.pointOfInt[i].type == "school") {
            g->draw_surface(school, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "bank" || mapDB.pointOfInt[i].type == "atm") {
            g->draw_surface(bank, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "bus station") {
            g->draw_surface(bus, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "bar" || mapDB.pointOfInt[i].type == "nightclub") {
            g->draw_surface(bar, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "pharmacy" || mapDB.pointOfInt[i].type == "hospital"
                   || mapDB.pointOfInt[i].type == "clinic" || mapDB.pointOfInt[i].type == "dentist"
                   || mapDB.pointOfInt[i].type == "doctors") {
            g->draw_surface(hospital, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "police") {
            g->draw_surface(police, {x - width, y + height});
        } else if (mapDB.pointOfInt[i].type == "fuel") {
            g->draw_surface(gas, {x - width, y + height});
        } else {
            g->draw_surface(icon, {x - width, y + height});
        }
    }
}

/**
 * 2.14 Draw POI helper
 * @param g
 * @param currentCanvas
 */
void drawPOIHelper(ezgl::renderer *g, ezgl::camera &c, ezgl::surface *icon,
                   ezgl::surface *parking, ezgl::surface *restaurant, ezgl::surface *school, ezgl::surface *theatre,
                   ezgl::surface *bar, ezgl::surface *bus, ezgl::surface *hospital, ezgl::surface *bank,
                   ezgl::surface *gas, ezgl::surface *police, ezgl::surface *highlight, ezgl::surface *cinema) {
    //currentScale
    ezgl::point2d currentScale = c.get_world_scale_factor();

    //current screen coordination
    ezgl::rectangle currentScreen = g->get_visible_world();

    // Draw highlight
    if ((isFind) && (!isType)) {
        const int POI_NUMBER = getNumPointsOfInterest();
        for (POIIdx POIId = 0; POIId < POI_NUMBER; ++POIId) {
            if (mapDB.pointOfInt[POIId].highlight) {
                float x = mapDB.pointOfInt[POIId].xyPosition.x;
                float y = mapDB.pointOfInt[POIId].xyPosition.y;
                g->draw_surface(highlight, {x - 15 * currentScale.x, y + 30 * currentScale.x});
            }
        }
    }

    //condition 1: top level not shown
    if (currentScale.x > 60 && (!isType)) {
        return;
    } else if (currentScale.x <= 60 && currentScale.x > 35) {
        //condition 2: secondary level shown
        int increment = 4 * currentScale.x;
        for (size_t i = 0; i < mapDB.pointOfInt.size(); i += increment) {
            //1 find locations
            float x = mapDB.pointOfInt[i].xyPosition.x;
            float y = mapDB.pointOfInt[i].xyPosition.y;

            float width = 4 * currentScale.x;
            float height = 4 * currentScale.x;

            if (currentScreen.contains(x, y)) {
                //2 determine whether or not draw it
                bool isContained = false;
                for (int index = 0; index < mapDB.pointOfInt.size(); index += increment) {
                    if (mapDB.pointOfInt[index].isShown &&
                        mapDB.pointOfInt[i].poiRectangle.contains(mapDB.pointOfInt[index].xyPosition) && index != i) {
                        isContained = true;
                        break;
                    }
                }

                //3 set shape of POI
                if (!isContained) {
                    //choose icon
                    typeSelector(g, i, x, y, width, height, icon, parking, restaurant, school, theatre,
                                 bar, bus, hospital, bank, gas, police, cinema);
                    //draw POI
                    mapDB.pointOfInt[i].isShown = true;
                }
            }
        }
    } else if (currentScale.x <= 35 && currentScale.x > 25) {
        //condition 3: medium level Shown
        int increment = 3 * currentScale.x;
        for (size_t i = 0; i < mapDB.pointOfInt.size(); i += increment) {
            //1 find locations
            float x = mapDB.pointOfInt[i].xyPosition.x;
            float y = mapDB.pointOfInt[i].xyPosition.y;

            float width = 8 * currentScale.x;
            float height = 5 * currentScale.x;

            //2 determine whether or not draw it
            if (!currentScreen.contains(x, y)) {
                continue;
            }

            bool isContained = false;
            for (int index = 0; index < mapDB.pointOfInt.size(); index += increment) {
                if (mapDB.pointOfInt[index].isShown &&
                    mapDB.pointOfInt[i].poiRectangle.contains(mapDB.pointOfInt[index].xyPosition) && index != i) {
                    isContained = true;
                    break;
                }
            }

            //3 set shape of POI
            if (!isContained) {
                //choose icon based on types
                typeSelector(g, i, x, y, width, height, icon, parking, restaurant, school, theatre,
                             bar, bus, hospital, bank, gas, police, cinema);

                //draw POI
                mapDB.pointOfInt[i].isShown = true;

                //4 set format and text
                ezgl::point2d textPosition;
                textPosition.x = x;
                textPosition.y = y - 5 * height;
                g->set_color(ezgl::MINORFONT);
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, currentScale.x / 3);
                g->set_text_rotation(0);

                if(mapDB.pointOfInt[i].name.size() > 25)
                    stringSpliter(g, currentScale.x, i, textPosition, true);
                else
                    g->draw_text(textPosition, mapDB.pointOfInt[i].name);
            }
        }
    } else if (currentScale.x <= 25 && currentScale.x > 1) {
        int increment = 2 * currentScale.x;
        for (size_t i = 0; i < mapDB.pointOfInt.size(); i += increment) {
            //1 find locations
            float x = mapDB.pointOfInt[i].xyPosition.x;
            float y = mapDB.pointOfInt[i].xyPosition.y;

            float width = 8 * currentScale.x;
            float height = 5 * currentScale.x;

            //2 determine whether or not draw it
            if (!currentScreen.contains(x, y)) {
                continue;
            }

            bool isContained = false;
            for (int index = 0; index < mapDB.pointOfInt.size(); index += increment) {
                if (mapDB.pointOfInt[index].isShown &&
                    mapDB.pointOfInt[i].poiRectangle.contains(mapDB.pointOfInt[index].xyPosition) && index != i) {
                    isContained = true;
                    break;
                }
            }

            //3 set shape of POI
            if (!isContained) {
                //choose icon based on types
                typeSelector(g, i, x, y, width, height, icon, parking, restaurant, school, theatre,
                             bar, bus, hospital, bank, gas, police, cinema);

                //draw POI
                mapDB.pointOfInt[i].isShown = true;

                //4 set format and text
                ezgl::point2d textPosition;
                textPosition.x = x;
                textPosition.y = y - 5 * height;

                g->set_color(ezgl::MINORFONT);
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_text_rotation(0);

                if(mapDB.pointOfInt[i].name.size() > 20)
                    stringSpliter(g, currentScale.x, i, textPosition, true);
                else
                    g->draw_text(textPosition, mapDB.pointOfInt[i].name);
            }
        }
    } else if (currentScale.x < 1) {
        //condition 4: bottom level Shown
        for (size_t i = 0; i < mapDB.pointOfInt.size(); ++i) {
            //1 find locations
            float x = mapDB.pointOfInt[i].xyPosition.x;
            float y = mapDB.pointOfInt[i].xyPosition.y;

            float width = 10 * currentScale.x;
            float height = 10 * currentScale.x;

            //2 set shape of POI
            if (!currentScreen.contains(x, y)) {
                continue;
            }

            bool isContained = false;
            for (int index = 0; index < mapDB.pointOfInt.size(); index++) {
                if (mapDB.pointOfInt[index].isShown &&
                    mapDB.pointOfInt[i].poiRectangle.contains(mapDB.pointOfInt[index].xyPosition) && index != i) {
                    isContained = true;
                    break;
                }
            }

            //3 set shape of POI
            if (!isContained) {
                //choose icon based on types
                typeSelector(g, i, x, y, width, height, icon, parking, restaurant, school, theatre,
                             bar, bus, hospital, bank, gas, police, cinema);

                //draw POI
                mapDB.pointOfInt[i].isShown = true;

                //4 set format and text
                ezgl::point2d textPosition;
                textPosition.x = x;
                textPosition.y = y - 3 * height;

                g->set_color(ezgl::MINORFONT);
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 10);
                g->set_text_rotation(0);

                if(mapDB.pointOfInt[i].name.size() > 20)
                    stringSpliter(g, currentScale.x, i, textPosition, true);
                else
                    g->draw_text(textPosition, mapDB.pointOfInt[i].name);
            }
        }
    }

    if ((!isFind) && (isType)) {
        const int POI_NUMBER = getNumPointsOfInterest();
        for (POIIdx POIId = 0; POIId < POI_NUMBER; ++POIId) {
            if (mapDB.pointOfInt[POIId].highlight) {
                float x = mapDB.pointOfInt[POIId].xyPosition.x;
                float y = mapDB.pointOfInt[POIId].xyPosition.y;

                if (!currentScreen.contains(x, y)) {
                    continue;
                }

                g->draw_surface(highlight, {x - 15 * currentScale.x, y + 30 * currentScale.x});
            }
        }
    }
}

/**
 * 2.15 Draw Legend
 * @param g
 */
void drawLegend(ezgl::renderer *g) {
    ezgl::rectangle currentScreen = g->get_visible_screen();
    ezgl::rectangle currentWorld = g->get_visible_world();

    float legendLength =
            100 * (currentWorld.right() - currentWorld.left()) / (currentScreen.right() - currentScreen.left());

    g->set_coordinate_system(ezgl::SCREEN);
    g->set_color(ezgl::WHITE);
    g->fill_rectangle({20, 10}, {130, 30});

    g->set_color(ezgl::MAJORFONT);
    g->set_line_width(2);
    g->draw_rectangle({20, 10}, {130, 30});
    g->draw_line({30, 25}, {120, 25});
    g->draw_line({30, 25}, {30, 20});
    g->draw_line({120, 25}, {120, 20});

    std::string legendText = to_string(legendLength);
    g->set_font_size(11);
    g->set_text_rotation(0);
    g->draw_text({70, 18}, legendText);
    g->set_coordinate_system(ezgl::WORLD);
}

/**
 * 2.9 Set Screen
 * @param application
 */
void setScreen(ezgl::application *application, bool isStreet, bool isPOI, bool isFeature, bool isIntersection) {
    // Initialize variables
    int count = 0;
    double originX = 0;
    double originY = 0;
    double topX = 0;
    double topY = 0;

    // Traverse POI to check for highlighted ones
    if (isStreet) {
        for (StreetSegmentIdx segmentId = 0; segmentId < mapDB.segment.size(); ++segmentId) {
            if (mapDB.segment[segmentId].highlight) {
                if (count == 0) {
                    originX = mapDB.segment[segmentId].xyLocation[0].x;
                    originY = mapDB.segment[segmentId].xyLocation[0].y;
                    topX = mapDB.segment[segmentId].xyLocation[0].x;
                    topY = mapDB.segment[segmentId].xyLocation[0].y;
                    for (int i = 1; i <mapDB.segment[segmentId].xyLocation.size(); ++i){
                        originX = std::min(originX, mapDB.segment[segmentId].xyLocation[i].x);
                        originY = std::min(originY, mapDB.segment[segmentId].xyLocation[i].y);
                        topX = std::max(topX, mapDB.segment[segmentId].xyLocation[i].x);
                        topY = std::max(topY, mapDB.segment[segmentId].xyLocation[i].y);
                    }
                    count++;
                } else {
                    for (int i = 0; i <mapDB.segment[segmentId].xyLocation.size(); ++i){
                        originX = std::min(originX, mapDB.segment[segmentId].xyLocation[i].x);
                        originY = std::min(originY, mapDB.segment[segmentId].xyLocation[i].y);
                        topX = std::max(topX, mapDB.segment[segmentId].xyLocation[i].x);
                        topY = std::max(topY, mapDB.segment[segmentId].xyLocation[i].y);
                    }
                }
            }
        }
    }

    // Traverse POI to check for highlighted ones
    if (isPOI) {
        for (POIIdx POIId = 0; POIId < mapDB.pointOfInt.size(); ++POIId) {
            if (mapDB.pointOfInt[POIId].highlight) {
                if (count == 0) {
                    originX = mapDB.pointOfInt[POIId].xyPosition.x;
                    originY = mapDB.pointOfInt[POIId].xyPosition.y;
                    topX = mapDB.pointOfInt[POIId].xyPosition.x;
                    topY = mapDB.pointOfInt[POIId].xyPosition.y;
                    count++;
                } else {
                    originX = std::min(originX, mapDB.pointOfInt[POIId].xyPosition.x);
                    originY = std::min(originY, mapDB.pointOfInt[POIId].xyPosition.y);
                    topX = std::max(topX, mapDB.pointOfInt[POIId].xyPosition.x);
                    topY = std::max(topY, mapDB.pointOfInt[POIId].xyPosition.y);
                }
            }
        }
    }

    // Traverse features to check for highlighted ones
    if (isFeature) {
        for (FeatureIdx featureId = 0; featureId < mapDB.feature.size(); ++featureId) {
            if (mapDB.feature[featureId].highlight) {
                if (count == 0) {
                    originX = mapDB.feature[featureId].xyLocation[0].x;
                    originY = mapDB.feature[featureId].xyLocation[0].y;
                    topX = mapDB.feature[featureId].xyLocation[0].x;
                    topY = mapDB.feature[featureId].xyLocation[0].y;
                    count++;
                }
                int pointNum = mapDB.feature[featureId].latLonLocation.size();

                for (int num = 0; num < pointNum; ++num) {
                    originX = std::min(originX, mapDB.feature[featureId].xyLocation[num].x);
                    originY = std::min(originY, mapDB.feature[featureId].xyLocation[num].y);
                    topX = std::max(topX, mapDB.feature[featureId].xyLocation[num].x);
                    topY = std::max(topY, mapDB.feature[featureId].xyLocation[num].y);
                }
            }
        }
    }

    // Traverse intersections to check for highlighted ones
    if (isIntersection) {
        for (IntersectionIdx intersectionId = 0; intersectionId < mapDB.intersection.size(); ++intersectionId) {
            if (mapDB.intersection[intersectionId].highlight) {
                if (count == 0) {
                    originX = mapDB.intersection[intersectionId].xyLocation.x;
                    originY = mapDB.intersection[intersectionId].xyLocation.y;
                    topX = mapDB.intersection[intersectionId].xyLocation.x;
                    topY = mapDB.intersection[intersectionId].xyLocation.y;
                    count++;
                }
                originX = std::min(originX, mapDB.intersection[intersectionId].xyLocation.x);
                originY = std::min(originY, mapDB.intersection[intersectionId].xyLocation.y);
                topX = std::max(topX, mapDB.intersection[intersectionId].xyLocation.x);
                topY = std::max(topY, mapDB.intersection[intersectionId].xyLocation.y);
            }
        }
    }

    double leftX = 0.9997 * originX;
    double leftY = 0.9997 * originY;
    double rightX = 1.0003 * topX;
    double rightY = 1.0003 * topY;

    ezgl::point2d origin(leftX, leftY);
    ezgl::point2d top(rightX, rightY);
    ezgl::rectangle screen(origin, top);
    application->get_renderer()->set_visible_world(screen);
}

/**
 * 2.16 set world to Zoom Fit Level
 * @param app
 */
void worldToFit(ezgl::application *app) {
    ezgl::point2d origin(min_x, min_y);
    ezgl::point2d top(max_x, max_y);
    ezgl::rectangle screen(origin, top);
    app->get_renderer()->set_visible_world(screen);
}

/**
 * 2.17 draw  box
 * @param from and to destination input and show on screen
 */
void drawBox(ezgl::renderer *g, ezgl::surface *detective, ezgl::surface *close,
             ezgl::surface *transfer, ezgl::surface *direct) {
    g->set_coordinate_system(ezgl::SCREEN);
    ezgl::rectangle currentScreen = g->get_visible_screen();

    double originX = currentScreen.m_first.x;
    double originY = currentScreen.m_first.y;
    double topX = currentScreen.m_second.x;
    double topY = currentScreen.m_second.y;

    // Draw background box
    g->set_color(ezgl::backBox, 200);
    g->fill_rectangle({originX, originY}, {0.4 * topX, topY});
    //g->set_line_width(3);

    // Draw box for first input
    g->set_color(ezgl::inputBox, 200);
    g->fill_rectangle({0.01 * topX, 0.02 * topY},
                      {0.345 * topX, 0.07 * topY});
    // Box for navigation icon
    g->fill_rectangle({0.355 * topX, 0.02 * topY},
                      {0.39 * topX, 0.07 * topY});

    // Draw icon
    g->draw_surface(detective, {0.0143 * topX, 0.0337 * topY});
    g->draw_surface(close, {0.321 * topX, 0.0333 * topY});
    g->draw_surface(direct, {0.3626 * topX, 0.0333 * topY});

    // Draw box for second input
    g->set_color(ezgl::inputBox);
    g->fill_rectangle({0.01 * topX, 0.09 * topY},
                      {0.345 * topX, 0.14 * topY});
    // Box for swap icon
    g->fill_rectangle({0.355 * topX, 0.09 * topY},
                      {0.39 * topX, 0.14 * topY});

    // Draw icon
    g->draw_surface(detective, {0.0143 * topX, 0.1037 * topY});
    g->draw_surface(close, {0.321 * topX, 0.1033 * topY});
    g->draw_surface(transfer, {0.3626 * topX, 0.1033 * topY});

    // Draw output box
    //g->set_color(ezgl::backBox);
    //g->fill_rectangle({originX, 0.162 * topY}, {0.4 * topX, topY});
}

/**
 * 2.18 draw input text
 * @param g
 */
void drawInput(ezgl::renderer *g) {
    g->set_coordinate_system(ezgl::SCREEN);
    ezgl::rectangle currentScreen = g->get_visible_screen();

    double topX = currentScreen.m_second.x;
    double topY = currentScreen.m_second.y;

    if (firstInput.length() > 0) {
        int length = firstInput.length();
        g->set_color(ezgl::inputFont);
        g->set_text_rotation(0);

        if (topX < 800){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.045 * topX + length * 0.0048 * topX, 0.05 * topY}, firstInput);
        } else if ((topX >= 800) && (topX < 900)){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.038 * topX + length * 0.0045 * topX, 0.05 * topY}, firstInput);
        } else if ((topX >= 900) && (topX < 1000)){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.03 * topX + length * 0.0045 * topX, 0.05 * topY}, firstInput);
        } else if ((topX >= 1000) && (topX < 1100)){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.023 * topX + length * 0.0045 * topX, 0.05 * topY}, firstInput);
        } else if (topX >= 1100){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.015 * topX + length * 0.0045 * topX, 0.05 * topY}, firstInput);
        }
    }
    if (secondInput.length() > 0){
        int length = secondInput.length();
        g->set_color(ezgl::inputFont);
        g->set_text_rotation(0);

        if (topX < 800){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.045 * topX + length * 0.0048 * topX, 0.117 * topY}, secondInput);
        } else if ((topX >= 800) && (topX < 900)){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.038 * topX + length * 0.0045 * topX, 0.117 * topY}, secondInput);
        } else if ((topX >= 900) && (topX < 1000)){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.03 * topX + length * 0.0045 * topX, 0.117 * topY}, secondInput);
        } else if ((topX >= 1000) && (topX < 1100)){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.023 * topX + length * 0.0045 * topX, 0.117 * topY}, secondInput);
        } else if (topX >= 1100){
            g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                           ezgl::font_weight::bold, 12);
            g->draw_text({0.015 * topX + length * 0.0045 * topX, 0.117 * topY}, secondInput);
        }
    }
}

/**
 * 2.19 draw output way direction in the map
 * ezgl::renderer *g
 * std::vector<StreetSegmentIdx>
 * ezgl::camera &c
 */
void drawOutputHelper(ezgl::renderer *g, std::vector<StreetSegmentIdx> pathVector, ezgl::camera &c) {
    ezgl::point2d currentScale = c.get_world_scale_factor();
    g->set_coordinate_system(ezgl::WORLD);

    for (int pathIndex = 0; pathIndex < pathVector.size(); pathIndex++) {
        //get the return segmenIdx
        StreetSegmentIdx currentSegmentIdx = pathVector[pathIndex];
        StreetSegmentInfo currentSegmentInfo = getStreetSegmentInfo(currentSegmentIdx);

        int numOfCurvePoints = currentSegmentInfo.numCurvePoints;
        g->set_color(ezgl::BLUE);
        ezgl::point2d from = mapDB.segment[currentSegmentIdx].xyLocation[0];
        wayWidth(g, currentScale.x);

        if (numOfCurvePoints == 0) {
            //we only have from and to points
            ezgl::point2d to = mapDB.segment[currentSegmentIdx].xyLocation[1];
            g->draw_line(from, to);

        } else {
            ezgl::point2d firstCurvePoint = mapDB.segment[currentSegmentIdx].xyLocation[1];
            g->draw_line(from, firstCurvePoint);

            for (int curvePointIdx = 0; curvePointIdx < numOfCurvePoints; ++curvePointIdx) {
                ezgl::point2d lastCurvePoint = mapDB.segment[currentSegmentIdx].xyLocation[curvePointIdx + 1];
                ezgl::point2d currentCurPoint = mapDB.segment[currentSegmentIdx].xyLocation[curvePointIdx + 2];
                g->draw_line(lastCurvePoint, currentCurPoint);
                g->set_line_cap(ezgl::line_cap(1));
            }

            ezgl::point2d lastCur = mapDB.segment[currentSegmentIdx].xyLocation[numOfCurvePoints];
            ezgl::point2d to = mapDB.segment[currentSegmentIdx].xyLocation[numOfCurvePoints + 1];
            g->draw_line(lastCur, to);
        }
    }
}

/**
 * 2.20 load path from algorithm
 * @param intersect_id_start
 * @param intersect_id_destination
 * @param turn_penalty
 */
void loadPath(const IntersectionIdx intersect_id_start,
              const IntersectionIdx intersect_id_destination,
              const double turn_penalty) {
    //get find shortest segmentIdx vector
    path = findPathBetweenIntersections(intersect_id_start, intersect_id_destination, turn_penalty);
    //travel time is in seconds
    travelTimeInSeconds = computePathTravelTime(path, turn_penalty);
}


/**
 * 2.21 basicInformationHelper
 * @param travelTimeInSec
 */
void basicInformationHelper(double travelTimeInSec){
    // Line 1: Estimated time
    double travelTimeInMinutes = travelTimeInSec / 60.0;
    string timeFront = "Estimated time: ";
    string timeTransition = to_string(travelTimeInMinutes);
    string timeMiddle = timeTransition.substr(0, timeTransition.find(".") + 2);
    string timeBack = " minutes";
    timeFront.append(timeMiddle).append(timeBack);
    mapDB.result.emplace_back(timeFront);
    // Icon for first line
    MapDBStruct::dir timeLine;
    timeLine.time = true;
    mapDB.direction.emplace_back(timeLine);

    // Line 2: Driving path
    string travelPath = "Driving Path";
    mapDB.result.emplace_back(travelPath);
    // Icon for second line
    MapDBStruct::dir pathLine;
    pathLine.pathNote = true;
    mapDB.direction.emplace_back(pathLine);

    // Line 3: Your location
    string location = "Your location";
    mapDB.result.emplace_back(location);
    // Icon for third line
    MapDBStruct::dir locateLine;
    locateLine.yourLoc = true;
    mapDB.direction.emplace_back(locateLine);
}

/**
 * 2.21 pathTurningHelper
 * @param currentAngle
 * @param nextAngle
 * @param nextStreetName
 */
void pathTurningHelper(double deltaAngle, string nextStreetName){
    // Case 1: Turn Right
    // 0-180 & -360--180
    if (deltaAngle < 0){
        //if we get the right logo, we want to print turn right on xxx
        string di = "Turn right onto ";
        di.append(nextStreetName);
        mapDB.result.emplace_back(di);
        MapDBStruct::dir rightLine;
        rightLine.turnRight = true;
        mapDB.direction.emplace_back(rightLine);
    } else if (deltaAngle > 0){
        // Case 2: Turn Left
        // -180-0 & 180-360
        string di = "Turn left onto ";
        di.append(nextStreetName);
        mapDB.result.emplace_back(di);
        MapDBStruct::dir leftLine;
        leftLine.turnLeft = true;
        mapDB.direction.emplace_back(leftLine);
    } else {
        // Case 3: Go Straight
        string di = "Go straight onto ";
        di.append(nextStreetName);
        mapDB.result.emplace_back(di);
        MapDBStruct::dir straightLine;
        straightLine.goStraight = true;
        mapDB.direction.emplace_back(straightLine);
    }
}

/**
 * 2.22 Store Output information for multi segment Street
 * @param path
 * @param travelTimeInSeconds
 */
void storeForBet(std::vector <StreetSegmentIdx> input, double travelTimeInSec) {
    // Store basic information
    basicInformationHelper(travelTimeInSec);

    // Save for future use
    string streetAdd = " m on ";
    int streetLength = 0;

    for (int j = 0; j < input.size(); ++j) {
        // Whether this is the last Segment
        if (j == input.size() - 1){
            // See whether the current segment and the next segment are on the same street
            StreetSegmentIdx currentSegId = input[j];
            StreetSegmentInfo currentInfo = getStreetSegmentInfo(currentSegId);
            StreetIdx currentStreet = currentInfo.streetID;
            string currentName = getStreetName(currentStreet);

            streetLength += mapDB.segment[currentSegId].segmentLength;
            string output = to_string(streetLength);
            output.append(streetAdd).append(currentName);
            mapDB.result.emplace_back(output);
            MapDBStruct::dir straightLine;
            straightLine.goStraight = true;
            mapDB.direction.emplace_back(straightLine);

            string arrive = "Arrive";
            mapDB.result.emplace_back(arrive);
            MapDBStruct::dir endLine;
            endLine.end = true;
            mapDB.direction.emplace_back(endLine);
        } else {
            // See whether the current segment and the next segment are on the same street
            StreetSegmentIdx currentSegId = input[j];
            StreetSegmentInfo currentInfo = getStreetSegmentInfo(currentSegId);
            StreetIdx currentStreet = currentInfo.streetID;
            string currentName = getStreetName(currentStreet);

            StreetSegmentIdx nextSegId = input[j + 1];
            StreetSegmentInfo nextSegInfo = getStreetSegmentInfo(nextSegId);
            StreetIdx nextStreet = nextSegInfo.streetID;
            string nextStreetName = getStreetName(nextStreet);

            if (currentStreet != nextStreet){
                streetLength += mapDB.segment[currentSegId].segmentLength;
                string output = to_string(streetLength);
                output.append(streetAdd).append(currentName);
                mapDB.result.emplace_back(output);
                MapDBStruct::dir straightLine;
                straightLine.goStraight = true;
                mapDB.direction.emplace_back(straightLine);
                streetLength = 0;

                // Calculate cross product to define turning condition
                // nextSegmentTo node's parent node is intersection
                // intersection node's parent is currentSegmentFrom
                ezgl::point2d currentSegmentFrom;
                ezgl::point2d intersection;
                ezgl::point2d nextSegmentTo;

                // currentSegment.to = nextSegment.from
                if(currentInfo.to == nextSegInfo.from){
                    nextSegmentTo = mapDB.intersection[nextSegInfo.to].xyLocation;
                    intersection = mapDB.intersection[nextSegInfo.from].xyLocation;
                    currentSegmentFrom = mapDB.intersection[currentInfo.from].xyLocation;
                }

                // currentSegment.from = nextSegment.from
                if(currentInfo.from == nextSegInfo.from){
                    nextSegmentTo = mapDB.intersection[nextSegInfo.to].xyLocation;
                    intersection = mapDB.intersection[nextSegInfo.from].xyLocation;
                    currentSegmentFrom = mapDB.intersection[currentInfo.to].xyLocation;
                }

                // currentSegment.to = nextSegment.to
                if(currentInfo.to == nextSegInfo.to){
                    nextSegmentTo = mapDB.intersection[nextSegInfo.from].xyLocation;
                    intersection = mapDB.intersection[nextSegInfo.to].xyLocation;
                    currentSegmentFrom = mapDB.intersection[currentInfo.from].xyLocation;
                }

                // currentSegment.from = nextSegment.to
                if(currentInfo.from == nextSegInfo.to){
                    nextSegmentTo = mapDB.intersection[nextSegInfo.from].xyLocation;
                    intersection = mapDB.intersection[nextSegInfo.to].xyLocation;
                    currentSegmentFrom = mapDB.intersection[currentInfo.to].xyLocation;
                }

                double x1 = intersection.x - currentSegmentFrom.x;
                double y1 = intersection.y - currentSegmentFrom.y;
                double x2 = nextSegmentTo.x - intersection.x;
                double y2 = nextSegmentTo.x - intersection.x;

                double crossProduct = x1*y2 - x2*y1;

                pathTurningHelper(crossProduct, nextStreetName);
            } else {
                streetLength += mapDB.segment[currentSegId].segmentLength;
            }
        }
    }
}


/**
 * 2.23 store output
 * @param g
 * @param the order:
 *
 */
void storeOutput(std::vector <StreetSegmentIdx> pathVector, double travelTimeInSec) {
    mapDB.result.clear();
    mapDB.direction.clear();

    if (firstDrew){
        currentFirstLine = 0;
    }

    if (pathVector.size() == 1) {
        basicInformationHelper(travelTimeInSec);

        //only 1 segment in the output path result
        StreetSegmentIdx currentSegmentIdx = pathVector[0];
        StreetSegmentInfo currentSegmentInfo = getStreetSegmentInfo(currentSegmentIdx);
        StreetIdx currentStreet = currentSegmentInfo.streetID;
        string currentStreetName = getStreetName(currentStreet);
        int currentSegmentLength = mapDB.segment[currentSegmentIdx].segmentLength;

        string currentSegmentLen = to_string(currentSegmentLength);
        string streetAdd = "m on ";
        currentSegmentLen.append(streetAdd).append(currentStreetName);
        mapDB.result.emplace_back(currentSegmentLen);

        //we only have 1 statement to adjust the 1 segment on its direction
        // noDirection = true;
        MapDBStruct::dir endLine1;
        endLine1.noDirection = true;
        mapDB.direction.emplace_back(endLine1);

        // end here
        string arrive = "Arrive";
        mapDB.result.push_back(arrive);

        // end = true
        MapDBStruct::dir endLine2;
        endLine2.end = true;
        mapDB.direction.emplace_back(endLine2);

    } else {
        storeForBet(pathVector, travelTimeInSec);
    }
}


/**
 * 2.24 output way fit in different zoom in level
 * @param ezgl::renderer *g
 * @param float scale
 */
void wayWidth(ezgl::renderer *g, float scale) {
    if (scale > 45) {
        g->set_line_width(scale / 13);
    } else if (scale < 45 && scale > 29) {
        g->set_line_width(scale / 10);
    } else if (scale < 29 && scale > 25) {
        g->set_line_width(scale / 10);
    } else if (scale > 17) {
        g->set_line_width(scale / 5);
    } else if (scale > 12) {
        g->set_line_width(scale / 1.5);
    } else if (scale > 11) {
        g->set_line_width(scale / 2);
    } else if (scale < 11 && scale > 7) {
        g->set_line_width(scale / 1.8);
    } else if (scale < 7 && scale > 1.5) {
        g->set_line_width(scale * exp(-scale / 2) * 25);
    } else if (scale < 1.5) {
        g->set_line_width(15 / scale);
    }
}

/**
 * 2.25 draw output display
 * @param g
 */
void noValidPath(ezgl::renderer *g, ezgl::surface *alram) {
    //assume we have already got the stored output vector and direction vector
    ezgl::rectangle currentScreen = g->get_visible_screen();
    g->set_coordinate_system(ezgl::SCREEN);

    //prepare for update text and logo position according to the screen
    double topX = currentScreen.m_second.x;
    double topY = currentScreen.m_second.y;

    g->draw_surface(alram, {0.019 * topX, 0.168 * topY + 0.07});

    string output = "Sorry, there is no valid path.";
    g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                   ezgl::font_weight::bold, 12);
    g->set_color(ezgl::BLACK);
    g->draw_text({0.2 * topX + 0.075, 0.195 * topY}, output);
}

/**
 * 2.26 drawOutput function
 * @param g
 * @param up
 * @param down
 * @param car
 * @param clock
 * @param flag
 * @param loc
 * @param more
 * @param left
 * @param right
 * @param straight
 */
void drawOutput(ezgl::renderer *g, bool up, bool down, ezgl::surface *car, ezgl::surface *clock,
                ezgl::surface *flag, ezgl::surface *loc, ezgl::surface *more, ezgl::surface *left,
                ezgl::surface *right, ezgl::surface *straight) {
    ezgl::rectangle currentScreen = g->get_visible_screen();
    g->set_coordinate_system(ezgl::SCREEN);
    double topX = currentScreen.m_second.x;
    double topY = currentScreen.m_second.y;
    g->set_text_rotation(0);

    if (mapDB.result.size() <= 12) {
        for (int pathSize = 0; pathSize < mapDB.result.size(); ++pathSize) {
            if (mapDB.direction[pathSize].noDirection) {
                g->draw_surface(more, {0.019 * topX, 0.168 * topY + topY * (pathSize * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (pathSize * 0.074), 0.18 * topY + topY * (pathSize * 0.072)},
                             mapDB.result[pathSize]);
            } else if (mapDB.direction[pathSize].time) {
                g->draw_surface(clock, {0.019 * topX, 0.168 * topY + topY * (pathSize * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (pathSize * 0.074), 0.18 * topY + topY * (pathSize * 0.072)},
                             mapDB.result[pathSize]);
            } else if (mapDB.direction[pathSize].pathNote) {
                g->draw_surface(car, {0.019 * topX, 0.168 * topY + topY * (pathSize * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (pathSize * 0.074), 0.18 * topY + topY * (pathSize * 0.072)},
                             mapDB.result[pathSize]);
            } else if (mapDB.direction[pathSize].yourLoc) {
                g->draw_surface(loc, {0.019 * topX, 0.168 * topY + topY * (pathSize * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (pathSize * 0.074), 0.18 * topY + topY * (pathSize * 0.072)},
                             mapDB.result[pathSize]);
            } else if (mapDB.direction[pathSize].turnLeft) {
                g->draw_surface(left, {0.019 * topX, 0.168 * topY + topY * (pathSize * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (pathSize * 0.074), 0.18 * topY + topY * (pathSize * 0.072)},
                             mapDB.result[pathSize]);
            } else if (mapDB.direction[pathSize].turnRight) {
                g->draw_surface(right, {0.019 * topX, 0.168 * topY + topY * (pathSize * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (pathSize * 0.075), 0.18 * topY + topY * (pathSize * 0.072)},
                             mapDB.result[pathSize]);
            } else if (mapDB.direction[pathSize].goStraight) {
                g->draw_surface(straight, {0.019 * topX, 0.168 * topY + topY * (pathSize * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (pathSize * 0.074), 0.18 * topY + topY * (pathSize * 0.072)},
                             mapDB.result[pathSize]);
            } else if (mapDB.direction[pathSize].end) {
                g->draw_surface(flag, {0.019 * topX, 0.168 * topY + topY * (pathSize * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (pathSize * 0.074), 0.18 * topY + topY * (pathSize * 0.072)},
                             mapDB.result[pathSize]);
            }
        }
    } else {
        if (firstDrew) {
            currentFirstLine = 0;
            firstDrew = false;
        }

        int changeLines = 0;
        if (up) {
            //print previous 8 lines
            if (currentFirstLine < 8) {
                changeLines = - currentFirstLine;
            } else {
                changeLines = -8; // for example, if we are in row 2, we can only scroll up 2 rows
            }
        } else if (down) {
            //print next 8 lines
            if (mapDB.result.size() <= 20){
                changeLines = mapDB.result.size() - currentFirstLine - 12;
            } else {
                if (currentFirstLine > (mapDB.result.size() - 20)) {
                    changeLines = mapDB.result.size() - currentFirstLine -12;
                } else {
                    changeLines = 8;
                }
            }
        }

        currentFirstLine = currentFirstLine + changeLines;

        //every line only has 1 logo mark
        for (int currentLine = currentFirstLine; currentLine < currentFirstLine + 12; ++currentLine) {
            //begin of the output display
            int count = currentLine - currentFirstLine;

            if (mapDB.direction[currentLine].noDirection) {
                g->draw_surface(more, {0.019 * topX, 0.168 * topY + topY * (count * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (count * 0.074), 0.18 * topY + topY * (count * 0.072)},
                             mapDB.result[currentLine]);

            } else if (mapDB.direction[currentLine].time) {
                g->draw_surface(clock, {0.019 * topX, 0.168 * topY + topY * (count * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (count * 0.074), 0.18 * topY + topY * (count * 0.072)},
                             mapDB.result[currentLine]);

            } else if (mapDB.direction[currentLine].pathNote) {
                g->draw_surface(car, {0.019 * topX, 0.168 * topY + topY * (count * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (count * 0.074), 0.18 * topY + topY * (count * 0.072)},
                             mapDB.result[currentLine]);

            } else if (mapDB.direction[currentLine].yourLoc) {
                g->draw_surface(loc, {0.019 * topX, 0.168 * topY + topY * (count * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (count * 0.074), 0.18 * topY + topY * (count * 0.072)},
                             mapDB.result[currentLine]);

            } else if (mapDB.direction[currentLine].turnLeft) {
                g->draw_surface(left, {0.019 * topX, 0.168 * topY + topY * (count * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (count * 0.074), 0.18 * topY + topY * (count * 0.072)},
                             mapDB.result[currentLine]);

            } else if (mapDB.direction[currentLine].turnRight) {
                g->draw_surface(right, {0.019 * topX, 0.168 * topY + topY * (count * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (count * 0.075), 0.18 * topY + topY * (count * 0.072)},
                             mapDB.result[currentLine]);

            } else if (mapDB.direction[currentLine].goStraight) {
                g->draw_surface(straight, {0.019 * topX, 0.168 * topY + topY * (count * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (count * 0.074), 0.18 * topY + topY * (count * 0.072)},
                             mapDB.result[currentLine]);

            } else if (mapDB.direction[currentLine].end) {
                g->draw_surface(flag, {0.019 * topX, 0.168 * topY + topY * (count * 0.07)});
                g->format_font("Noto Sans CJK SC", ezgl::font_slant::normal,
                               ezgl::font_weight::bold, 12);
                g->set_color(ezgl::BLACK);
                g->draw_text({0.2 * topX + (count * 0.074), 0.18 * topY + topY * (count * 0.072)},
                             mapDB.result[currentLine]);

            }
        }
        isUp = false;
        isDown = false;
    }
}

/**
 * 2.27 setScreenForPath function
 * @param application
 */
void setScreenForPath(ezgl::application *application){
    // Initialize variables
    int count = 0;
    double originX = 0;
    double originY = 0;
    double topX = 0;
    double topY = 0;

    // Traverse path vector
    for (int j = 0; j < path.size(); ++j) {
        StreetSegmentIdx segmentId = path[j];
        if (count == 0) {
            originX = mapDB.segment[segmentId].xyLocation[0].x;
            originY = mapDB.segment[segmentId].xyLocation[0].y;
            topX = mapDB.segment[segmentId].xyLocation[0].x;
            topY = mapDB.segment[segmentId].xyLocation[0].y;
            for (int i = 1; i < mapDB.segment[segmentId].xyLocation.size(); ++i){
                originX = std::min(originX, mapDB.segment[segmentId].xyLocation[i].x);
                originY = std::min(originY, mapDB.segment[segmentId].xyLocation[i].y);
                topX = std::max(topX, mapDB.segment[segmentId].xyLocation[i].x);
                topY = std::max(topY, mapDB.segment[segmentId].xyLocation[i].y);
            }
            count++;
        } else {
            for (int i = 0; i < mapDB.segment[segmentId].xyLocation.size(); ++i){
                originX = std::min(originX, mapDB.segment[segmentId].xyLocation[i].x);
                originY = std::min(originY, mapDB.segment[segmentId].xyLocation[i].y);
                topX = std::max(topX, mapDB.segment[segmentId].xyLocation[i].x);
                topY = std::max(topY, mapDB.segment[segmentId].xyLocation[i].y);
            }
        }
    }

    double leftX = 1.002 * originX;
    double leftY = 0.9997 * originY;
    double rightX = 1.0002 * topX;
    double rightY = 1.0003 * topY;

    ezgl::point2d origin(leftX, leftY);
    ezgl::point2d top(rightX, rightY);
    ezgl::rectangle screen(origin, top);
    //application->get_renderer()->set_coordinate_system(ezgl::SCREEN);
    application->get_renderer()->set_visible_world(screen);
}


/**
 * 3.1 Draw_main_canvas
 * @param renderer * g
 *        3.1.0 Get Information
 *        3.1.1 Features
 *        3.1.2 Streets
 *        3.1.3 POIs
 *        3.1.4 Intersections
 *        3.1.5 Border
 */
static void drawMainCanvas(ezgl::renderer *g) {
    //3.1.0 Get Information
    if (appPtr == nullptr) {
        std::cout << "not ready. skip this" << std::endl;
        return;
    }
    std::cout << "entering drawMainCanvas()" << std::endl;
    ezgl::camera &c = (appPtr->get_canvas(appPtr->get_main_canvas_id()))->get_camera();

    //create a surface
    ezgl::surface *otherPOI = ezgl::renderer::load_png("libstreetmap/resources/icons8-star-24.png");
    ezgl::surface *interIcon = ezgl::renderer::load_png("libstreetmap/resources/inter_icon.png");
    ezgl::surface *parking = ezgl::renderer::load_png("libstreetmap/resources/parking.png");
    ezgl::surface *restaurant = ezgl::renderer::load_png("libstreetmap/resources/restaurant.png");
    ezgl::surface *school = ezgl::renderer::load_png("libstreetmap/resources/school.png");
    ezgl::surface *theatre = ezgl::renderer::load_png("libstreetmap/resources/theatre-mask.png");
    ezgl::surface *bar = ezgl::renderer::load_png("libstreetmap/resources/bar.png");
    ezgl::surface *bus = ezgl::renderer::load_png("libstreetmap/resources/bus.png");
    ezgl::surface *hospital = ezgl::renderer::load_png("libstreetmap/resources/icons8-hospital-24.png");
    ezgl::surface *bank = ezgl::renderer::load_png("libstreetmap/resources/bank-building.png");
    ezgl::surface *gas = ezgl::renderer::load_png("libstreetmap/resources/gas-station.png");
    ezgl::surface *police = ezgl::renderer::load_png("libstreetmap/resources/icons8-police-car-24.png");
    ezgl::surface *Pin = ezgl::renderer::load_png("libstreetmap/resources/icons8-map-pin-30.png");
    ezgl::surface *cinema = ezgl::renderer::load_png("libstreetmap/resources/cinema.png");
    ezgl::surface *detective = ezgl::renderer::load_png("libstreetmap/resources/detective.png");
    ezgl::surface *close = ezgl::renderer::load_png("libstreetmap/resources/close.png");
    ezgl::surface *transfer = ezgl::renderer::load_png("libstreetmap/resources/transfer.png");
    ezgl::surface *direct = ezgl::renderer::load_png("libstreetmap/resources/direct.png");
    ezgl::surface *car = ezgl::renderer::load_png("libstreetmap/resources/car.png");
    ezgl::surface *clock = ezgl::renderer::load_png("libstreetmap/resources/clock.png");
    ezgl::surface *flag = ezgl::renderer::load_png("libstreetmap/resources/flag.png");
    ezgl::surface *locate = ezgl::renderer::load_png("libstreetmap/resources/locate.png");
    ezgl::surface *more = ezgl::renderer::load_png("libstreetmap/resources/more.png");
    ezgl::surface *left = ezgl::renderer::load_png("libstreetmap/resources/left.png");
    ezgl::surface *right = ezgl::renderer::load_png("libstreetmap/resources/right.png");
    ezgl::surface *straight = ezgl::renderer::load_png("libstreetmap/resources/straight.png");
    ezgl::surface *loc = ezgl::renderer::load_png("libstreetmap/resources/loc.png");
    ezgl::surface *alarm = ezgl::renderer::load_png("libstreetmap/resources/alarm.png");

    //3.1.1 draw features
    drawFeatureHelper(g, c);

    //3.1.2 draw streets
    drawStreetHelper(g, c);

    drawFeatureNameHelper(g, c);

    drawFeatureHighlightHelper(g, c, Pin);

    drawStreetHighlightHelper(g, c);

    //3.1.3 draw POI
    drawPOIHelper(g, c, otherPOI, parking, restaurant, school, theatre, bar, bus,
                  hospital, bank, gas, police, Pin, cinema);

    //3.1.5 draw legend
    drawLegend(g);

    //3.1.6 Draw Black Border
    g->set_color(ezgl::BLACK);
    g->set_line_width(3);
    g->draw_rectangle({min_x, min_y}, {max_x, max_y});

    if (isNavigation){
        if (readyForOutput){
            if (firstDrew){
                path = findPathBetweenIntersections(intersection_id_start, intersection_id_destination, 15);
                if (path.size() != 0){
                    travelTimeInSeconds = computePathTravelTime(path, 15);
                    storeOutput(path, travelTimeInSeconds);
                }
                firstDrew = false;
            }
            if (path.size() != 0){
                drawOutputHelper(g, path, c);
            }
        }
    }

    //3.1.4 draw intersections
    drawIntersectionHelper(g, c, interIcon, flag, locate);
    if (isNavigation){
        drawBox(g, detective, close, transfer, direct);
        drawInput(g);
        if (readyForOutput){
            if (path.size() != 0){
                drawOutput(g, isUp, isDown, car, clock, flag, loc, more, left, right, straight);
            }
        }
    }

    if (readyForOutput && path.size() == 0){
        noValidPath(g, alarm);
    }

    //free surface
    ezgl::renderer::free_surface(otherPOI);
    ezgl::renderer::free_surface(interIcon);
    ezgl::renderer::free_surface(parking);
    ezgl::renderer::free_surface(restaurant);
    ezgl::renderer::free_surface(school);
    ezgl::renderer::free_surface(theatre);
    ezgl::renderer::free_surface(bar);
    ezgl::renderer::free_surface(bus);
    ezgl::renderer::free_surface(hospital);
    ezgl::renderer::free_surface(bank);
    ezgl::renderer::free_surface(gas);
    ezgl::renderer::free_surface(cinema);
    ezgl::renderer::free_surface(police);
    ezgl::renderer::free_surface(Pin);
    ezgl::renderer::free_surface(detective);
    ezgl::renderer::free_surface(close);
    ezgl::renderer::free_surface(direct);
    ezgl::renderer::free_surface(transfer);
    ezgl::renderer::free_surface(car);
    ezgl::renderer::free_surface(clock);
    ezgl::renderer::free_surface(flag);
    ezgl::renderer::free_surface(locate);
    ezgl::renderer::free_surface(more);
    ezgl::renderer::free_surface(left);
    ezgl::renderer::free_surface(right);
    ezgl::renderer::free_surface(straight);
    ezgl::renderer::free_surface(loc);
    ezgl::renderer::free_surface(alarm);
}

/**
 * 3.2 Act on Mouse click
 *     3.2.1 Show Click Basic Information
 *     3.2.2 Click Intersection
 */
void on_dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data, ezgl::application *application) {
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

/**
 * 3.3 Act on Mouse CLick
 * @param app
 * @param event
 * @param x
 * @param y
 */
void actOnMouseClick(ezgl::application *app, GdkEventButton *event, double x, double y) {
    // When multiple start positions or/and destinations are displayed, select desired position
    if (selectStartIntersection || selectDestination){
        LatLon pos = LatLon(latFromY(y), lonFromX(x));
        // Get intersection that the user click on
        int id = findClosestIntersection(pos);

        if (findDistanceBetweenTwoPoints(std::make_pair(pos, mapDB.intersection[id].latLonLocation)) < 300) {
            if (selectStartIntersection && mapDB.intersection[id].start) {
                // Find unique start position
                intersection_id_start = id;
                const int INTERSECTION_NUMBER = getNumIntersections();
                for (int num = 0; num < INTERSECTION_NUMBER; ++num){
                    if ((mapDB.intersection[num].start) && (num != id)){
                        mapDB.intersection[num].start = false;
                    }
                }
                selectStartIntersection = false;
            } else if (selectDestination && mapDB.intersection[id].destination){
                // Find unique destination
                intersection_id_destination = id;
                const int INTERSECTION_NUMBER = getNumIntersections();
                for (int num = 0; num < INTERSECTION_NUMBER; ++num){
                    if ((mapDB.intersection[num].destination) && (num != id)){
                        mapDB.intersection[num].destination = false;
                    }
                }
                selectDestination = false;
            }
        }

        // May add pop-up window here to indicate
        // Ex. Destinations have not been chosen, please choose unique destination
        if ((!selectStartIntersection) && (!selectDestination)){
            readyForOutput = true;
            firstDrew = true;
            path = findPathBetweenIntersections(intersection_id_start, intersection_id_destination, 15);
            if (path.size() != 0){
               // setScreenForPath(appPtr);
            }
        }
        app->refresh_drawing();
    } else if (isNavigation) {
        // Convert WORLD coordinate system to SCREEN coordinate system (coule be a helper function)
        ezgl::rectangle worldBound = app->get_renderer()->get_visible_world();
        double lowX = worldBound.m_first.x;
        double lowY = worldBound.m_first.y;
        double highX = worldBound.m_second.x;
        double highY = worldBound.m_second.y;

        ezgl::rectangle screenBound = app->get_renderer()->get_visible_screen();
        double screenX = screenBound.m_second.x;
        double screenY = screenBound.m_second.y;

        double positionX = (x - lowX) / ((highX - lowX) / screenX);
        double positionY = (highY - y) / ((highY - lowY) / screenY);

        // Check whether click is on the first or the second input box
        if ((positionX > 0.01 * screenX) && (positionX < 0.32 * screenX)
            && (positionY > 0.02 * screenY) && (positionY < 0.07 * screenY)) {
            if (firstInput == "Enter a start position"){
                firstInput.erase(firstInput.begin(), firstInput.end());
            }
            drawFirstInput = true;
            drawSecondInput = false;
            // May draw a line to indicate input (guang biao)
            app->refresh_drawing();
        } else if ((positionX > 0.01 * screenX) && (positionX < 0.32 * screenX)
                   && (positionY > 0.09 * screenY) && (positionY < 0.14 * screenY)){
            if (secondInput == "Enter a destination"){
                secondInput.erase(secondInput.begin(), secondInput.end());
            }
            drawFirstInput = false;
            drawSecondInput = true;
            // May draw a line to indicate input (guang biao)
            app->refresh_drawing();
        } else if ((positionX >= 0.32 * screenX) && (positionX < 0.345 * screenX)
                   && (positionY > 0.02 * screenY) && (positionY < 0.07 * screenY)){
            firstInput.erase(firstInput.begin(), firstInput.end());                         // first input delete icon
            readyForOutput = false;
            for (int i = 0; i < mapDB.intersection.size(); ++i){
                if (mapDB.intersection[i].start){
                    mapDB.intersection[i].start = false;
                }
            }
            startFound = false;
            app->refresh_drawing();
        } else if ((positionX >= 0.32 * screenX) && (positionX < 0.345 * screenX)
                   && (positionY > 0.09 * screenY) && (positionY < 0.14 * screenY)){
            secondInput.erase(secondInput.begin(), secondInput.end());                      // second input delete icon
            readyForOutput = false;
            for (int i = 0; i < mapDB.intersection.size(); ++i){
                if (mapDB.intersection[i].destination){
                    mapDB.intersection[i].destination = false;
                }
            }
            destinationFound = false;
            app->refresh_drawing();
        } else if ((positionX > 0.355 * screenX) && (positionX < 0.39 * screenX)
                   && (positionY > 0.02 * screenY) && (positionY < 0.07 * screenY)) {       // Navigation icon
            // Initialize signal and drawing
            readyForOutput = false;
            for (int i = 0; i < mapDB.intersection.size(); ++i){
                if (mapDB.intersection[i].start){
                    mapDB.intersection[i].start = false;
                }
                if (mapDB.intersection[i].destination){
                    mapDB.intersection[i].destination = false;
                }
            }
            vector<IntersectionIdx> intersectionId1, intersectionId2;
            // Check whether start position with invalid input (not two streets)
            if (firstInput.find_first_of(',') < firstInput.length() - 1) {
                int delimiter1 = firstInput.find_first_of(',');
                string firstName1 = firstInput.substr(0, delimiter1);
                string secondName1 = firstInput.substr(delimiter1 + 1);
                // Make the input working for partial names
                vector<StreetIdx> streetOne1 = findStreetIdsFromPartialStreetName(firstName1);
                vector<StreetIdx> streetTwo1 = findStreetIdsFromPartialStreetName(secondName1);

                // Find common intersections from input streets
                unordered_set<IntersectionIdx> tempIntersectionId1;
                for (int i = 0; i < streetOne1.size(); ++i) {
                    for (int j = 0; j < streetTwo1.size(); ++j) {
                        std::pair<StreetIdx, StreetIdx> streetIds1(streetOne1[i], streetTwo1[j]);
                        std::vector<IntersectionIdx> intersectionIds1 = findIntersectionsOfTwoStreets(streetIds1);
                        for (int k = 0; k < intersectionIds1.size(); ++k) {
                            tempIntersectionId1.insert(intersectionIds1[k]);
                        }
                    }
                }
                // Get rid of duplicated intersections
                intersectionId1 = vector<IntersectionIdx>(tempIntersectionId1.begin(), tempIntersectionId1.end());

                // Check the number of start positions, branch into different cases
                if (intersectionId1.size() == 0){
                    // Case 1: No intersection between two input streets
                    // Initialize start position to value indicating error
                    intersection_id_start = -1;

                    // Pop-up dialog window indicates no intersection between two input streets
                    GObject *window;
                    GtkWidget *content_area, *label, *dialog;
                    window = app->get_object(app->get_main_window_id().c_str());

                    // Create the pop-up dialog window
                    dialog = gtk_dialog_new_with_buttons("Cannot Find Start Position", (GtkWindow *) window,
                                                         GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                         GTK_RESPONSE_ACCEPT, ("CLOSE"), GTK_RESPONSE_REJECT, NULL);
                    // Add specific street name into the output
                    string output = "No intersection between two input streets.\n"
                                    "Start position cannot be found.\n"
                                    "Please double check the position of your start position.\n";
                    const char *informationText = output.c_str();

                    // Attach the label to the content of the window
                    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                    label = gtk_label_new(informationText);
                    gtk_container_add(GTK_CONTAINER(content_area), label);
                    gtk_widget_show_all(dialog);

                    // Register dialog response to the callback function
                    g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
                } else {
                    // Display start position(s) on map
                    for (int i = 0; i < intersectionId1.size(); ++i) {
                        mapDB.intersection[intersectionId1[i]].start = true;
                    }

                    if (intersectionId1.size() == 1){
                        // Case 2: Unique start position found
                        intersection_id_start = intersectionId1[0];
                        // Wait for destination to be checked
                    } else {
                        // Case 3: Multiple start positions found
                        // Initialize start position to value indicating error
                        intersection_id_start = -1;
                        selectStartIntersection = true;
                    }

                    // Check whether destination with invalid input (not two streets)
                    if (secondInput.find_first_of(',') < secondInput.length() - 1) {
                        int delimiter2 = secondInput.find_first_of(',');
                        string firstName2 = secondInput.substr(0, delimiter2);
                        string secondName2 = secondInput.substr(delimiter2 + 1);
                        vector<StreetIdx> streetOne2 = findStreetIdsFromPartialStreetName(firstName2);
                        vector<StreetIdx> streetTwo2 = findStreetIdsFromPartialStreetName(secondName2);

                        // Find common intersections from input streets
                        unordered_set<IntersectionIdx> tempIntersectionId2;
                        for (int i = 0; i < streetOne2.size(); ++i) {
                            for (int j = 0; j < streetTwo2.size(); ++j) {
                                std::pair<StreetIdx, StreetIdx> streetIds2(streetOne2[i], streetTwo2[j]);
                                std::vector<IntersectionIdx> intersectionIds2 =
                                        findIntersectionsOfTwoStreets(streetIds2);
                                for (int k = 0; k < intersectionIds2.size(); ++k) {
                                    tempIntersectionId2.insert(intersectionIds2[k]);
                                }
                            }
                        }
                        // Get rid of duplicated intersections
                        intersectionId2 = vector<IntersectionIdx>(tempIntersectionId2.begin(),
                                                                  tempIntersectionId2.end());

                        // Check the number of destinations, branch into different cases
                        if (intersectionId2.size() == 0){
                            // Case 1: No intersection between two input streets
                            // Initialize destination to value indicating error
                            intersection_id_destination = -1;
                            // Cancel the highlight of start positions
                            for (int i = 0; i < mapDB.intersection.size(); ++i) {
                                if (mapDB.intersection[i].start){
                                    mapDB.intersection[i].start = false;
                                }
                            }

                            // Pop-up dialog window indicates no intersection between two input streets
                            GObject *window;
                            GtkWidget *content_area, *label, *dialog;
                            window = app->get_object(app->get_main_window_id().c_str());

                            // Create the pop-up dialog window
                            dialog = gtk_dialog_new_with_buttons("Cannot Find Destination", (GtkWindow *) window,
                                                                 GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                                 GTK_RESPONSE_ACCEPT, ("CLOSE"),
                                                                 GTK_RESPONSE_REJECT, NULL);
                            // Add specific street name into the output
                            string output = "No intersection between two input streets.\n"
                                            "Destination cannot be found.\n"
                                            "Please double check the position of your destination.\n";
                            const char *informationText = output.c_str();

                            // Attach the label to the content of the window
                            content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                            label = gtk_label_new(informationText);
                            gtk_container_add(GTK_CONTAINER(content_area), label);
                            gtk_widget_show_all(dialog);

                            // Register dialog response to the callback function
                            g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
                        } else {
                            // Case 2: Unique destination found
                            if (intersectionId2.size() == 1){
                                intersection_id_destination = intersectionId2[0];

                                // Check whether start position and destination are the same intersection
                                if (intersection_id_start == intersection_id_destination){
                                    // Initialize start position and destination to values indicating error
                                    intersection_id_start = -1;
                                    intersection_id_destination = -1;
                                    // Cancel the highlight of start positions
                                    for (int i = 0; i < mapDB.intersection.size(); ++i) {
                                        if (mapDB.intersection[i].start){
                                            mapDB.intersection[i].start = false;
                                        }
                                        if (mapDB.intersection[i].destination){
                                            mapDB.intersection[i].destination = false;
                                        }
                                    }

                                    // Pop-up dialog window indicates start position and destination are the same
                                    GObject *window;
                                    GtkWidget *content_area, *label, *dialog;
                                    window = app->get_object(app->get_main_window_id().c_str());

                                    // Create the pop-up dialog window
                                    dialog = gtk_dialog_new_with_buttons("Same Position", (GtkWindow *) window,
                                                                         GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                                         GTK_RESPONSE_ACCEPT, ("CLOSE"),
                                                                         GTK_RESPONSE_REJECT, NULL);
                                    string output = "Start position and destination are the same intersection.\n";
                                    const char *informationText = output.c_str();

                                    // Attach the label to the content of the window
                                    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                                    label = gtk_label_new(informationText);
                                    gtk_container_add(GTK_CONTAINER(content_area), label);
                                    gtk_widget_show_all(dialog);

                                    // Register dialog response to the callback function
                                    g_signal_connect(GTK_DIALOG(dialog), "response",
                                                     G_CALLBACK(on_dialog_response), app);
                                } else {
                                    // Case 3: Unique destination found
                                    if (selectStartIntersection) {
                                        // Pop-up dialog window indicates start position and destination are the same
                                        GObject *window;
                                        GtkWidget *content_area, *label, *dialog;
                                        window = app->get_object(app->get_main_window_id().c_str());

                                        // Create the pop-up dialog window
                                        dialog = gtk_dialog_new_with_buttons("Multiple Start Positions Found",
                                                                             (GtkWindow *) window,
                                                                             GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                                             GTK_RESPONSE_ACCEPT, ("CLOSE"),
                                                                             GTK_RESPONSE_REJECT, NULL);
                                        string output = "Multiple start positions are found.\n"
                                                        "Please choose your desired start position by clicking "
                                                        "the icon on the map.\n\n"
                                                        "Here are the possible results of your start position:\n"
                                                        "Intersection ";
                                        for (int k = 0; k < intersectionId1.size(); ++k) {
                                            if (intersectionId1.size() > 1){
                                                output.append(to_string(k + 1));
                                            }
                                            unordered_set<string> streetNames;
                                            streetNames = unordered_set<string>(mapDB.intersection[intersectionId1[k]].streetName.begin(),
                                                                                mapDB.intersection[intersectionId1[k]].streetName.end());
                                            vector<string> streetNameString;
                                            streetNameString = vector<string>(streetNames.begin(), streetNames.end());
                                            if ((streetNameString.size() == 1) &&
                                                (mapDB.intersection[intersectionId1[k]].streetName[0] == "<unknown>")) {
                                                output.append(" :\n");
                                            } else {
                                                if (intersectionId1.size() > 1){
                                                    output.append(" on ");
                                                } else {
                                                    output.append("on ");
                                                }
                                                for (int j = 0; j < streetNameString.size(); ++j) {
                                                    if (mapDB.intersection[intersectionId1[k]].streetName[0] != "<unknown>") {
                                                        output.append(mapDB.intersection[intersectionId1[k]].streetName[j]);
                                                        if (j != streetNameString.size() - 1) {
                                                            output.append(" & ");
                                                        } else {
                                                            output.append(":\n");
                                                        }
                                                    }
                                                }
                                                output.append("- Latitude: ");
                                                output.append(to_string(
                                                        mapDB.intersection[intersectionId1[k]].latLonLocation.latitude()));
                                                output.append("\n- Longitude: ");
                                                output.append(to_string(
                                                        mapDB.intersection[intersectionId1[k]].latLonLocation.longitude()));
                                                if (k != (intersectionId1.size() - 1)) {
                                                    output.append("\nIntersection ");
                                                }
                                            }
                                        }
                                        const char *informationText = output.c_str();

                                        // Attach the label to the content of the window
                                        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                                        label = gtk_label_new(informationText);
                                        gtk_container_add(GTK_CONTAINER(content_area), label);
                                        gtk_widget_show_all(dialog);

                                        // Register dialog response to the callback function
                                        g_signal_connect(GTK_DIALOG(dialog), "response",
                                                         G_CALLBACK(on_dialog_response), app);
                                    } else {
                                        readyForOutput = true;
                                        firstDrew = true;
                                        path = findPathBetweenIntersections(intersection_id_start,
                                                                            intersection_id_destination, 15);
                                        if (path.size() != 0){
                                            //setScreenForPath(appPtr);
                                        }
                                    }
                                    // Display the destination on the map
                                    for (int i = 0; i < intersectionId2.size(); ++i) {
                                        mapDB.intersection[intersectionId2[i]].destination = true;
                                    }
                                }
                            } else {
                                // Case 4: Multiple destinations found
                                // Initialize destination to value indicating error
                                intersection_id_destination = -1;
                                selectDestination = true;
                                // Display destinations on the map
                                for (int i = 0; i < intersectionId2.size(); ++i) {
                                    mapDB.intersection[intersectionId2[i]].destination = true;
                                }

                                // Pop-up dialog window indicates start position and destination are the same
                                GObject *window;
                                GtkWidget *content_area, *label, *dialog;
                                window = app->get_object(app->get_main_window_id().c_str());

                                string output;
                                if (selectStartIntersection) {
                                    // Create the pop-up dialog window
                                    dialog = gtk_dialog_new_with_buttons("Multiple Start Positions and Destinations",
                                                                         (GtkWindow *) window,
                                                                         GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                                         GTK_RESPONSE_ACCEPT, ("CLOSE"),
                                                                         GTK_RESPONSE_REJECT, NULL);
                                    output = "Multiple start positions and destinations are found.\n"
                                             "Please choose your desired start position and destination by clicking "
                                             "the icons on the map.\n\n"
                                             "Here are the possible results of your start position:\n"
                                             "Intersection ";
                                    for (int k = 0; k < intersectionId1.size(); ++k) {
                                        if (intersectionId1.size() > 1){
                                            output.append(to_string(k + 1));
                                        }
                                        unordered_set<string> streetNames;
                                        streetNames = unordered_set<string>(mapDB.intersection[intersectionId1[k]].streetName.begin(),
                                                                            mapDB.intersection[intersectionId1[k]].streetName.end());
                                        vector<string> streetNameString;
                                        streetNameString = vector<string>(streetNames.begin(), streetNames.end());
                                        if ((streetNameString.size() == 1) &&
                                            (mapDB.intersection[intersectionId1[k]].streetName[0] == "<unknown>")) {
                                            output.append(" :\n");
                                        } else {
                                            if (intersectionId1.size() > 1){
                                                output.append(" on ");
                                            } else {
                                                output.append("on ");
                                            }
                                            for (int j = 0; j < streetNameString.size(); ++j) {
                                                if (mapDB.intersection[intersectionId1[k]].streetName[0] != "<unknown>") {
                                                    output.append(mapDB.intersection[intersectionId1[k]].streetName[j]);
                                                    if (j != streetNameString.size() - 1) {
                                                        output.append(" & ");
                                                    } else {
                                                        output.append(":\n");
                                                    }
                                                }
                                            }
                                            output.append("- Latitude: ");
                                            output.append(to_string(
                                                    mapDB.intersection[intersectionId1[k]].latLonLocation.latitude()));
                                            output.append("\n- Longitude: ");
                                            output.append(to_string(
                                                    mapDB.intersection[intersectionId1[k]].latLonLocation.longitude()));
                                            if (k != (intersectionId1.size() - 1)) {
                                                output.append("\nIntersection ");
                                            }
                                        }
                                    }
                                    output.append("\n\nHere are the possible results of your destination:\n"
                                                  "Intersection ");
                                    for (int k = 0; k < intersectionId2.size(); ++k) {
                                        if (intersectionId2.size() > 1){
                                            output.append(to_string(k + 1));
                                        }
                                        unordered_set<string> streetNames;
                                        streetNames = unordered_set<string>(mapDB.intersection[intersectionId2[k]].streetName.begin(),
                                                                            mapDB.intersection[intersectionId2[k]].streetName.end());
                                        vector<string> streetNameString;
                                        streetNameString = vector<string>(streetNames.begin(), streetNames.end());
                                        if ((streetNameString.size() == 1) &&
                                            (mapDB.intersection[intersectionId2[k]].streetName[0] == "<unknown>")) {
                                            output.append(" :\n");
                                        } else {
                                            if (intersectionId2.size() > 1){
                                                output.append(" on ");
                                            } else {
                                                output.append("on ");
                                            }
                                            for (int j = 0; j < streetNameString.size(); ++j) {
                                                if (mapDB.intersection[intersectionId2[k]].streetName[0] != "<unknown>") {
                                                    output.append(mapDB.intersection[intersectionId2[k]].streetName[j]);
                                                    if (j != streetNameString.size() - 1) {
                                                        output.append(" & ");
                                                    } else {
                                                        output.append(":\n");
                                                    }
                                                }
                                            }
                                            output.append("- Latitude: ");
                                            output.append(to_string(
                                                    mapDB.intersection[intersectionId2[k]].latLonLocation.latitude()));
                                            output.append("\n- Longitude: ");
                                            output.append(to_string(
                                                    mapDB.intersection[intersectionId2[k]].latLonLocation.longitude()));
                                            if (k != (intersectionId2.size() - 1)) {
                                                output.append("\nIntersection ");
                                            }
                                        }
                                    }
                                } else {
                                    dialog = gtk_dialog_new_with_buttons("Multiple Destinations Found ",
                                                                         (GtkWindow *) window,
                                                                         GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                                         GTK_RESPONSE_ACCEPT, ("CLOSE"),
                                                                         GTK_RESPONSE_REJECT, NULL);
                                    output = "Multiple destinations are found.\n"
                                             "Please choose your desired destination by clicking "
                                             "the icon on the map.\n";
                                    output.append("\nHere are the possible results of your destination:\n"
                                                  "Intersection ");
                                    for (int k = 0; k < intersectionId2.size(); ++k) {
                                        if (intersectionId2.size() > 1){
                                            output.append(to_string(k + 1));
                                        }
                                        unordered_set<string> streetNames;
                                        streetNames = unordered_set<string>(mapDB.intersection[intersectionId2[k]].streetName.begin(),
                                                                            mapDB.intersection[intersectionId2[k]].streetName.end());
                                        vector<string> streetNameString;
                                        streetNameString = vector<string>(streetNames.begin(), streetNames.end());
                                        if ((streetNameString.size() == 1) &&
                                            (mapDB.intersection[intersectionId2[k]].streetName[0] == "<unknown>")) {
                                            output.append(" :\n");
                                        } else {
                                            if (intersectionId2.size() > 1){
                                                output.append(" on ");
                                            } else {
                                                output.append("on ");
                                            }
                                            for (int j = 0; j < streetNameString.size(); ++j) {
                                                if (mapDB.intersection[intersectionId2[k]].streetName[0] != "<unknown>") {
                                                    output.append(mapDB.intersection[intersectionId2[k]].streetName[j]);
                                                    if (j != streetNameString.size() - 1) {
                                                        output.append(" & ");
                                                    } else {
                                                        output.append(":\n");
                                                    }
                                                }
                                            }
                                            output.append("- Latitude: ");
                                            output.append(to_string(
                                                    mapDB.intersection[intersectionId2[k]].latLonLocation.latitude()));
                                            output.append("\n- Longitude: ");
                                            output.append(to_string(
                                                    mapDB.intersection[intersectionId2[k]].latLonLocation.longitude()));
                                            if (k != (intersectionId2.size() - 1)) {
                                                output.append("\nIntersection ");
                                            }
                                        }
                                    }
                                }
                                const char *informationText = output.c_str();

                                // Attach the label to the content of the window
                                content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                                label = gtk_label_new(informationText);
                                gtk_container_add(GTK_CONTAINER(content_area), label);
                                gtk_widget_show_all(dialog);

                                // Register dialog response to the callback function
                                g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
                            }
                        }
                    } else {
                        // Initialize start position and destination to values indicating error
                        intersection_id_start = -1;
                        intersection_id_destination = -1;
                        // Cancel the highlight of start position(s)
                        for (int i = 0; i < mapDB.intersection.size(); ++i) {
                            if (mapDB.intersection[i].start){
                                mapDB.intersection[i].start = false;
                            }
                        }

                        // Pop-up dialog window indicates destination cannot be found by input string
                        GObject *window;
                        GtkWidget *content_area, *label, *dialog;
                        window = app->get_object(app->get_main_window_id().c_str());

                        // Create the pop-up dialog window
                        dialog = gtk_dialog_new_with_buttons("Cannot Find Destination", (GtkWindow *) window,
                                                             GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                             GTK_RESPONSE_ACCEPT, ("CLOSE"), GTK_RESPONSE_REJECT, NULL);
                        string output = "Destination cannot be found.\n"
                                        "Please enter in format of 'street1, street2' to show your destination\n"
                                        "or double check your spelling.\n";
                        const char *informationText = output.c_str();

                        // Attach the label to the content of the window
                        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                        label = gtk_label_new(informationText);
                        gtk_container_add(GTK_CONTAINER(content_area), label);
                        gtk_widget_show_all(dialog);

                        // Register dialog response to the callback function
                        g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
                    }
                }
            } else {
                // Initialize start position and destination to values indicating error
                intersection_id_start = -1;
                intersection_id_destination = -1;

                // Pop-up dialog window indicates start position cannot be found by input string
                GObject *window;
                GtkWidget *content_area, *label, *dialog;
                window = app->get_object(app->get_main_window_id().c_str());

                // Create the pop-up dialog window
                dialog = gtk_dialog_new_with_buttons("Cannot Find Start Position", (GtkWindow *) window,
                                                     GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                     GTK_RESPONSE_ACCEPT, ("CLOSE"), GTK_RESPONSE_REJECT, NULL);
                string output = "Start position cannot be found.\n"
                                "Please enter in format of 'street1, street2' to show your start position\n"
                                "or double check your spelling.\n";
                const char *informationText = output.c_str();

                // Attach the label to the content of the window
                content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                label = gtk_label_new(informationText);
                gtk_container_add(GTK_CONTAINER(content_area), label);
                gtk_widget_show_all(dialog);

                // Register dialog response to the callback function
                g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
            }
            app->refresh_drawing();
        } else if ((positionX > 0.355 * screenX) && (positionX < 0.39 * screenX)
                   && (positionY > 0.09 * screenY) && (positionY < 0.14 * screenY)) {       // Swap icon
            // swap two input strings
            if (readyForOutput){
                mapDB.intersection[intersection_id_start].start = false;
                mapDB.intersection[intersection_id_destination].destination = false;
                int temp = intersection_id_start;
                intersection_id_start = intersection_id_destination;
                intersection_id_destination = temp;
                mapDB.intersection[intersection_id_start].start = true;
                mapDB.intersection[intersection_id_destination].destination = true;
                string tempS = firstInput;
                firstInput = secondInput;
                secondInput = tempS;
                firstDrew = true;
            } else {
                string temp = firstInput;
                firstInput = secondInput;
                secondInput = temp;
                if (startFound){
                    startFound = false;
                    destinationFound = true;
                    mapDB.intersection[intersection_id_start].start = false;
                    intersection_id_destination = intersection_id_start;
                    mapDB.intersection[intersection_id_destination].destination = true;
                } else if (destinationFound){
                    destinationFound = false;
                    startFound = true;
                    mapDB.intersection[intersection_id_destination].destination = false;
                    intersection_id_start = intersection_id_destination;
                    mapDB.intersection[intersection_id_start].start = true;
                }
            }
            app->refresh_drawing();
        } else if ((positionX > 0.4 * screenX) && (positionX < screenX)
                    && (positionY > 0) && (positionY < screenY)){
            startFound = false;
            destinationFound = false;
            drawFirstInput = false;
            drawSecondInput = false;
            for (int i = 0; i < mapDB.intersection.size(); ++i){
                if (mapDB.intersection[i].start){
                    startFound = true;
                }
                if (mapDB.intersection[i].destination){
                    destinationFound = true;
                }
            }

            LatLon pos = LatLon(latFromY(y), lonFromX(x));
            int id = findClosestIntersection(pos);

            if (findDistanceBetweenTwoPoints(std::make_pair(pos, mapDB.intersection[id].latLonLocation)) < 2000) {
                if ((!startFound) && (!destinationFound)){                      // no start position, no destination
                    // Assume users' first input is the start position
                    intersection_id_start = id;
                    mapDB.intersection[intersection_id_start].start = true;
                    startFound = true;
                    string s = mapDB.intersection[intersection_id_start].intersectionName;
                    replace(s.begin(), s.end(), '&', ',');
                    firstInput = s;
                } else if ((!startFound) && (destinationFound)){                // no start position, has destination
                    if (mapDB.intersection[id].destination){
                        // Case 1: Cancel the highlight of destination
                        // Initialize destination to values indicating not found
                        intersection_id_destination = -1;
                        mapDB.intersection[id].destination = false;
                        destinationFound = false;
                        secondInput.erase(secondInput.begin(), secondInput.end());
                    } else {
                        // Case 2: Choose start position
                        intersection_id_start = id;
                        mapDB.intersection[intersection_id_start].start = true;
                        startFound = true;
                        string s = mapDB.intersection[intersection_id_start].intersectionName;
                        replace(s.begin(), s.end(), '&', ',');
                        firstInput = s;
                        readyForOutput = true;
                        firstDrew = true;
                        path = findPathBetweenIntersections(intersection_id_start,
                                                            intersection_id_destination, 15);
                        if (path.size() != 0){
                           // setScreenForPath(appPtr);
                        }
                    }
                } else if ((startFound) && (!destinationFound)){                // has start position, no destination
                    if (mapDB.intersection[id].start){
                        // Case 1: Cancel the highlight of start position
                        // Initialize start position to values indicating not found
                        intersection_id_start = -1;
                        mapDB.intersection[id].start = false;
                        startFound = false;
                        firstInput.erase(firstInput.begin(), firstInput.end());
                    } else {
                        // Case 2: Choose destination
                        intersection_id_destination = id;
                        mapDB.intersection[intersection_id_destination].destination = true;
                        destinationFound = true;
                        string s = mapDB.intersection[intersection_id_destination].intersectionName;
                        replace(s.begin(), s.end(), '&', ',');
                        secondInput = s;
                        readyForOutput = true;
                        firstDrew = true;
                        path = findPathBetweenIntersections(intersection_id_start,
                                                            intersection_id_destination, 15);
                        if (path.size() != 0){
                            //setScreenForPath(appPtr);
                        }
                    }
                } else {                                                        // has start position, has destination
                    if (mapDB.intersection[id].start){
                        // Case 1: Cancel the highlight of start position
                        // Initialize start position to values indicating not found
                        intersection_id_start = -1;
                        mapDB.intersection[id].start = false;
                        startFound = false;
                        firstInput.erase(firstInput.begin(), firstInput.end());
                        readyForOutput = false;
                    } else if (mapDB.intersection[id].destination){
                        // Case 2: Cancel the highlight of destination
                        // Initialize start position to values indicating not found
                        intersection_id_destination = -1;
                        mapDB.intersection[id].destination = false;
                        destinationFound = false;
                        secondInput.erase(secondInput.begin(), secondInput.end());
                        readyForOutput = false;
                    } else {
                        // Case 3: Want to add new intersection
                        // Pop-up dialog window suggests to cancel start position or destination
                        GObject *window;
                        GtkWidget *content_area, *label, *dialog;
                        window = app->get_object(app->get_main_window_id().c_str());

                        // Create the pop-up dialog window
                        dialog = gtk_dialog_new_with_buttons("Another Path", (GtkWindow *) window,
                                                             GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                             GTK_RESPONSE_ACCEPT, ("CLOSE"), GTK_RESPONSE_REJECT, NULL);
                        string output = "If you want to search for other paths, "
                                        "please first cancel either current start position or\n"
                                        "destination by clicking the icon on the map, "
                                        "or by clicking the delete icon in input boxes.\n";
                        const char *informationText = output.c_str();

                        // Attach the label to the content of the window
                        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                        label = gtk_label_new(informationText);
                        gtk_container_add(GTK_CONTAINER(content_area), label);
                        gtk_widget_show_all(dialog);

                        // Register dialog response to the callback function
                        g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
                    }
                }
            } else {
                // Cannot find an intersection at users clicking position
                // Pop-up dialog window suggests to cancel start position or destination
                GObject *window;
                GtkWidget *content_area, *label, *dialog;
                window = app->get_object(app->get_main_window_id().c_str());

                // Create the pop-up dialog window
                dialog = gtk_dialog_new_with_buttons("No Intersection", (GtkWindow *) window,
                                                     GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                                     GTK_RESPONSE_ACCEPT, ("CLOSE"), GTK_RESPONSE_REJECT, NULL);
                string output = "Cannot find an intersection at clicking position,\n"
                                "please move closer to your start position/destination and click again.\n";
                const char *informationText = output.c_str();

                // Attach the label to the content of the window
                content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                label = gtk_label_new(informationText);
                gtk_container_add(GTK_CONTAINER(content_area), label);
                gtk_widget_show_all(dialog);

                // Register dialog response to the callback function
                g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
            }
            app->refresh_drawing();
        } else {
            drawFirstInput = false;
            drawSecondInput = false;
        }
    } else {
        // Outside of navigation mode
        // Click Intersection
        LatLon pos = LatLon(latFromY(y), lonFromX(x));
        int id = findClosestIntersection(pos);

        if (findDistanceBetweenTwoPoints(std::make_pair(pos, mapDB.intersection[id].latLonLocation)) < 100) {
            // Print closest intersection information
            //std::cout << "Closest Intersection: " << mapDB.intersection[id].intersectionName << "\n";

            //Highlight Intersection
            if (mapDB.intersection[id].highlight) {
                mapDB.intersection[id].highlight = false;
            } else {
                mapDB.intersection[id].highlight = true;
                // Popup dialog window
                GObject *window;
                GtkWidget *content_area;
                GtkWidget *label;
                GtkWidget *dialog;

                window = app->get_object(app->get_main_window_id().c_str());

                // Create dialog window
                dialog = gtk_dialog_new_with_buttons(
                        "Information",
                        (GtkWindow *) window,
                        GTK_DIALOG_DESTROY_WITH_PARENT,
                        ("OK"),
                        GTK_RESPONSE_ACCEPT,
                        ("CLOSE"),
                        GTK_RESPONSE_REJECT,
                        NULL);

                string output = "Intersection";
                unordered_set<string> streetNames;
                streetNames = unordered_set<string>(mapDB.intersection[id].streetName.begin(),
                                                    mapDB.intersection[id].streetName.end());
                vector<string> names;
                names = vector<string>(streetNames.begin(), streetNames.end());
                if ((names.size() == 1) && (mapDB.intersection[id].streetName[0] == "<unknown>")) {
                    output.append(":\n");
                } else {
                    int count = 0;
                    for (int i = 0; i < names.size(); ++i) {
                        if (mapDB.intersection[id].streetName[0] != "<unknown>") {
                            if (count == 0){
                                output.append(" on ");
                                count++;
                            }
                            output.append(mapDB.intersection[id].streetName[i]);
                            if (i != names.size() - 1) {
                                output.append(" & ");
                            } else {
                                output.append(":\n");
                            }
                        }
                    }
                }

                output.append("Latitude: ");
                output.append(to_string(mapDB.intersection[id].latLonLocation.latitude()));
                output.append("\nLongitude: ");
                output.append(to_string(mapDB.intersection[id].latLonLocation.longitude()));
                const char *informationText = output.c_str();

                // Create a label and attach it to the content area of the dialog
                content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                label = gtk_label_new(informationText);
                gtk_container_add(GTK_CONTAINER(content_area), label);
                gtk_widget_show_all(dialog);

                // Connecting the "response" signal from the user to the associated callback function
                g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
            }

            int count = 0;
            for (IntersectionIdx i = 0; i < mapDB.intersection.size(); ++i){
                if (mapDB.intersection[i].highlight){
                    count++;
                }
            }
            if (count){
                setScreen(appPtr, false, false, false, true);
            }

            // Redraw the canvas
            app->refresh_drawing();
        }

        const int INTERSECTION_NUMBER = getNumIntersections();
        vector<IntersectionIdx> highlightIntersection;
        for (int num = 0; num < INTERSECTION_NUMBER; ++num){
            if (mapDB.intersection[num].highlight){
                highlightIntersection.emplace_back(num);
            }
        }
        if (highlightIntersection.size() == 2){
            double distance = 0, time = 0;
            double SPEED = 1.35;
            int numOne = highlightIntersection[0];
            int numTwo = highlightIntersection[1];

            distance = findDistanceBetweenTwoPoints(std::make_pair(mapDB.intersection[numOne].latLonLocation,
                                                                   mapDB.intersection[numTwo].latLonLocation));
            time = distance / SPEED;
            string output = "Take ";
            if (time < 60) {
                output.append("less than 1 minute to get the destination.");
            } else {
                output.append("around ");
                double minute = time / 60;
                string minuteString = to_string(minute);
                string minutes = minuteString.substr(0, minuteString.find(".") + 2);
                if (minutes == "1"){
                    output.append(minutes);
                    output.append(" minute to get the destination.");
                } else {
                    output.append(minutes);
                    output.append(" minutes to get the destination.");
                }
            }
            app->update_message(output);
        }
    }
}

/**
 * 3.4 Act on Find Button
 */
void actOnFind(GtkWidget *, ezgl::application *application) {
    // Initialise isFind to false
    isFind = false;
    bool isStreet = false;
    bool isPOI = false;
    bool isFeature = false;
    bool isIntersection = false;
    bool isMany = false;

    // Un-highlight all icons
    for (POIIdx POIId = 0; POIId < mapDB.pointOfInt.size(); ++POIId) {
        mapDB.pointOfInt[POIId].highlight = false;
    }
    for (FeatureIdx featureId = 0; featureId < mapDB.feature.size(); ++featureId) {
        mapDB.feature[featureId].highlight = false;
    }
    for (IntersectionIdx intersectionId = 0; intersectionId < mapDB.intersection.size(); ++intersectionId) {
        mapDB.intersection[intersectionId].highlight = false;
    }
    for (StreetSegmentIdx segmentId = 0; segmentId < mapDB.segment.size(); segmentId++) {
        mapDB.segment[segmentId].highlight = false;
    }

    // Get the GtkEntry object
    auto *searchText = (GtkEntry *) application->get_object("SearchBar");

    // Get the text written in the widget
    const char *text = gtk_entry_get_text(searchText);

    // Convert char array into a string
    string isText(text);

    // Check whether it is a single street name, a POI name, or a feature name
    string singleName = modifier(isText);

    // Function1. Check whether it is a single street name
    vector<StreetIdx> street = findStreetIdsFromPartialStreetName(singleName);
    // If it gives a unique result
    if ((street.size() == 1) && (street[0] != 0)){
        StreetIdx streetId = street[0];
        // Find segmentId from streetId
        vector<StreetSegmentIdx> segmentIds = mapDB.street[streetId].segmentsOfStreet;
        for (int i = 0; i < segmentIds.size(); ++i){
            StreetSegmentIdx segmentId = segmentIds[i];
            mapDB.segment[segmentId].highlight = true;
            isFind = true;
            isType = false;
            isStreet = true;
        }
    } else if (street.size() > 1){
        // If not a unique result
        // Initialize a dialog window
        isMany = true;
        GObject *window;
        GtkWidget *content_area;
        GtkWidget *label;
        GtkWidget *dialog;
        window = application->get_object(application->get_main_window_id().c_str());

        // Create dialog window
        dialog = gtk_dialog_new_with_buttons(
                "Many Results",
                (GtkWindow *) window,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                ("OK"),
                GTK_RESPONSE_ACCEPT,
                ("CLOSE"),
                GTK_RESPONSE_REJECT,
                NULL);

        string output = "Please give a more specific name. Here are some suggestions:\n";
        // Limit the length of suggestions
        int end = 15;
        if (street.size() < 15){
            end = street.size();
        }
        for (int i = 0; i < end; ++i){
            output.append(getStreetName(street[i]));
            output.append("\n");
        }
        const char *informationText = output.c_str();

        // Create label and widget
        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        label = gtk_label_new(informationText);
        gtk_container_add(GTK_CONTAINER(content_area), label);
        gtk_widget_show_all(dialog);

        // Register the callback function
        g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), application);
    }

    // Function2, Check whether it is a POI name
    const int POI_NUMBER = getNumPointsOfInterest();
    for (POIIdx POIId = 0; POIId < POI_NUMBER; ++POIId) {
        string POIName = mapDB.pointOfInt[POIId].name;
        if (singleName == modifier(POIName)) {
            mapDB.pointOfInt[POIId].highlight = true;
            isFind = true;
            isType = false;
            isPOI = true;
        }
    }

    // Function3. Check whether it is a feature name
    const int FEATURE_NUMBER = getNumFeatures();
    for (FeatureIdx featureId = 0; featureId < FEATURE_NUMBER; ++featureId) {
        string featureName = mapDB.feature[featureId].name;
        if (singleName == modifier(featureName)) {
            mapDB.feature[featureId].highlight = true;
            isFind = true;
            isType = false;
            isFeature = true;
        }
    }

    if (isText.find_first_of(',') < isText.length() - 1) {
        int delimiter = isText.find_first_of(',');
        string firstName = isText.substr(0, delimiter);
        string secondName = isText.substr(delimiter + 1);
        vector<StreetIdx> streetOne = findStreetIdsFromPartialStreetName(firstName);
        vector<StreetIdx> streetTwo = findStreetIdsFromPartialStreetName(secondName);

        // Find common intersections from street ids
        unordered_set<IntersectionIdx> tempIntersectionId;
        for (int i = 0; i < streetOne.size(); ++i) {
            for (int j = 0; j < streetTwo.size(); ++j) {
                std::pair<StreetIdx, StreetIdx> streetIds(streetOne[i], streetTwo[j]);
                std::vector<IntersectionIdx> intersectionIds = findIntersectionsOfTwoStreets(streetIds);
                for (int k = 0; k < intersectionIds.size(); ++k) {
                    tempIntersectionId.insert(intersectionIds[k]);
                    mapDB.intersection[intersectionIds[k]].highlight = true;
                    isFind = true;
                    isType = false;
                    isIntersection = true;
                }
            }
        }

        // Get rid of duplicated intersections
        vector<IntersectionIdx> intersectionId;
        intersectionId = vector<IntersectionIdx>(tempIntersectionId.begin(), tempIntersectionId.end());
        if (isIntersection) {
            // Initialize a popup dialog window
            GObject *window;
            GtkWidget *content_area;
            GtkWidget *label;
            GtkWidget *dialog;
            window = application->get_object(application->get_main_window_id().c_str());

            // Create the dialog window
            dialog = gtk_dialog_new_with_buttons(
                    "Information",
                    (GtkWindow *) window,
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    ("OK"),
                    GTK_RESPONSE_ACCEPT,
                    ("CLOSE"),
                    GTK_RESPONSE_REJECT,
                    NULL);

            string output = "Intersection ";
            for (int k = 0; k < intersectionId.size(); ++k) {
                if (intersectionId.size() > 1){
                    output.append(to_string(k + 1));
                }
                unordered_set<string> streetNames;
                streetNames = unordered_set<string>(mapDB.intersection[intersectionId[k]].streetName.begin(),
                                                    mapDB.intersection[intersectionId[k]].streetName.end());
                vector<string> streetNameString;
                streetNameString = vector<string>(streetNames.begin(), streetNames.end());
                if ((streetNameString.size() == 1) &&
                    (mapDB.intersection[intersectionId[k]].streetName[0] == "<unknown>")) {
                    output.append(" :\n");
                } else {
                    if (intersectionId.size() > 1){
                        output.append(" on ");
                    } else {
                        output.append("on ");
                    }
                    for (int j = 0; j < streetNameString.size(); ++j) {
                        if (mapDB.intersection[intersectionId[k]].streetName[0] != "<unknown>") {
                            output.append(mapDB.intersection[intersectionId[k]].streetName[j]);
                            if (j != streetNameString.size() - 1) {
                                output.append(" & ");
                            } else {
                                output.append(":\n");
                            }
                        }
                    }
                    output.append("- Latitude: ");
                    output.append(to_string(mapDB.intersection[intersectionId[k]].latLonLocation.latitude()));
                    output.append("\n- Longitude: ");
                    output.append(to_string(mapDB.intersection[intersectionId[k]].latLonLocation.longitude()));
                    if (k != (intersectionId.size() - 1)) {
                        output.append("\nIntersection ");
                    }
                }
            }

            const char *informationText = output.c_str();
            content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
            label = gtk_label_new(informationText);
            gtk_container_add(GTK_CONTAINER(content_area), label);
            gtk_widget_show_all(dialog);

            // Register the callback function
            g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), application);
        }
    }

    //if found zoom to set screen
    if (isPOI || isFeature || isIntersection) {
        setScreen(appPtr, isStreet, isPOI, isFeature, isIntersection);
    } else if (!isMany) {/*
        //print a sentence or window
        //print a sentence or window
        // Initialize a popup dialog window
        GObject *window;
        GtkWidget *content_area;
        GtkWidget *label;
        GtkWidget *dialog;

        window = application->get_object(application->get_main_window_id().c_str());

        // Create the dialog window.
        // Modal windows prevent interaction with other windows in the same application
        dialog = gtk_dialog_new_with_buttons(
                "No Results",
                (GtkWindow *) window,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                ("OK"),
                GTK_RESPONSE_ACCEPT,
                ("CLOSE"),
                GTK_RESPONSE_REJECT,
                NULL);

        string output;
        if (isText.find_first_of(',') < isText.length() - 1) {
            output = "No intersections found between two streets.\n"
                     "Please try again with other streets.";
        } else {
            output = "No streets, points of interest, or features are found.\n"
                     "Please try again with correct names.";
        }
        const char *informationText = output.c_str();

        // Create a label and attach it to the content area of the dialog
        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        label = gtk_label_new(informationText);
        gtk_container_add(GTK_CONTAINER(content_area), label);

        // The main purpose of this is to show dialog??s child widget, label
        gtk_widget_show_all(dialog);

        // Connecting the "response" signal from the user to the associated callback function
        g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), application);*/
    }
    // Redraw the graphics
    application->refresh_drawing();
}

/**
 * 3.5 press zoom in
 * @param widget
 * @param application
 * @param g
 */
void press_zoom_in(GtkWidget *widget, ezgl::application *application, ezgl::renderer *g) {
    const int POI_NUMBER = getNumPointsOfInterest();
    for (POIIdx POIId = 0; POIId < POI_NUMBER; ++POIId) {
        mapDB.pointOfInt[POIId].isShown = false;
    }
    application->refresh_drawing();
}

/**
 * 3.6 Act on Change Map
 * @param combo_box
 * @param application
 * @return
 */
gboolean actOnChangeMap(GtkComboBox *combo_box, ezgl::application *application) {
    //1. Get new path

    readyForOutput = false;
    startFound = false;
    destinationFound = false;
    isNavigation = false;
    firstInput.erase(firstInput.begin(), firstInput.end());
    secondInput.erase(secondInput.begin(), secondInput.end());
    firstInput = "Enter a start position";
    secondInput = "Enter a destination";

    // Get the text written in the widget
    const char *text = gtk_combo_box_get_active_id(combo_box);

    // Convert char array into a string
    string name(text);

    //create a loadMap path string
    string mapPath = "/cad2/ece297s/public/maps/";
    mapPath.append(name);
    mapPath.append(".streets.bin");

    //2. Close -> load -> Refresh
    closeMap();

    if (!loadMap(mapPath)) {
        std::cout << "The entered map \"" << name << "\" doesn't exist. Exiting... " << std::endl;
        application->quit();
        return TRUE;
    }

    ezgl::rectangle new_world({min_x, min_y}, {max_x, max_y});
    application->change_canvas_world_coordinates("MainCanvas", new_world);
    application->refresh_drawing();

    return TRUE;
}

/**
 * 3.7 act on Type List
 * @param combo_box
 * @param application
 */
void actOnTypeList(GtkComboBox *combo_box, ezgl::application *application) {
    //0. initialise & un-highlight
    for (int i = 0; i < mapDB.pointOfInt.size(); i++) {
        mapDB.pointOfInt[i].highlight = false;
        isType = false;
    }

    //1. Get POI & feature type

    // Get the text written in the widget
    const char *text = gtk_combo_box_get_active_id(combo_box);

    // Convert char array into a string
    string name(text);

    //2. modify input
    string typeID = modifier(name);

    if (typeID == "select") {
        worldToFit(application);
    } else {
        for (int i = 0; i < mapDB.pointOfInt.size(); i++) {
            if (mapDB.pointOfInt[i].type == typeID) {
                mapDB.pointOfInt[i].highlight = true;
                isType = true;
                isFind = false;
            }
        }

        if (isType)
            setScreen(application, false, true, false, false);

        appPtr->refresh_drawing();
    }

}

/**
 * 3.8 scroll mounse helper
 * @param event
 * @param data
 */
void actOnScrollMouse(GtkWidget *, GdkEvent *event, gpointer data){
    if (readyForOutput){
        auto application = static_cast<ezgl::application *>(data);
        GdkEventScroll *scroll_event = (GdkEventScroll *)event;
        ezgl::point2d scroll_point(scroll_event->x, scroll_event->y);

        ezgl::rectangle screenBound = application->get_renderer()->get_visible_screen();
        double screenX = screenBound.m_second.x;
        double screenY = screenBound.m_second.y;

        if ((scroll_point.x > 0) && (scroll_point.x < 0.4 * screenX)
                                    && (scroll_point.y > 0.162 * screenY) && (scroll_point.y < screenY)){
            if(scroll_event->direction == 1) {
                isUp = true;
            } else if(scroll_event->direction == 0) {
                isDown = true;
            }
        }
        application->refresh_drawing();
    }
}

/**
 * 3.9 Act on Key Press
 */
string removeLastChar(string s) {
    if (s.length() == 0) {
        return s;
    }
    return s.substr(0, s.length()-1);
}

/**
 * 3.10 actOnKey
 * @param app
 * @param key_name
 */
void actOnKey(ezgl::application *app, GdkEventKey *, char *key_name) {
    //initialize
    // app = appPtr;
    readyForOutput = false;
    string input(key_name);

    if (input == "space"){
        input = " ";
    } else if (input == "comma"){
        input = ",";
    }

    if (drawFirstInput){
        if (input == "BackSpace"){
            firstInput = removeLastChar(firstInput);
            int length = firstInput.length();
            app->get_renderer()->set_font_size(12);
            app->get_renderer()->set_color(ezgl::RED);
            app->get_renderer()->set_text_rotation(0);
            app->get_renderer()->draw_text({30 + length * 3.2, 43}, firstInput);
        } else if (input.length() == 1){
            firstInput.append(input);
            int length = firstInput.length();
            app->get_renderer()->set_font_size(12);
            app->get_renderer()->set_color(ezgl::RED);
            app->get_renderer()->set_text_rotation(0);
            app->get_renderer()->draw_text({30 + length * 3.2, 43}, firstInput);
        }
    }

    if (drawSecondInput){
        if (input == "BackSpace"){
            secondInput = removeLastChar(secondInput);
        } else if (input.length() == 1){
            secondInput.append(input);
        }
    }

    app->refresh_drawing();
}

// Without "GtkWidget *widget", the button will cause the program to crash
// But the warning says "GtkWidget *widget" is unused variable
// How to fix this??
/**
 * 3.11 actOnNavigation
 * @param application
 */
void actOnNavigation(GtkWidget *, ezgl::application *application){
    isNavigation = true;
    readyForOutput = false;
    application->refresh_drawing();
}

/**
 * 3.12 actOnCitySeeker
 * @param application
 */
void actOnCitySeeker(GtkWidget *, ezgl::application *application){
    if (isNavigation){
        isNavigation = false;
        readyForOutput = false;
        startFound = false;
        destinationFound = false;
        firstInput.erase(firstInput.begin(), firstInput.end());
        secondInput.erase(secondInput.begin(), secondInput.end());
        for (int i = 0; i < mapDB.intersection.size(); ++i){
            if (mapDB.intersection[i].start){
                mapDB.intersection[i].start = false;
            }
            if (mapDB.intersection[i].destination){
                mapDB.intersection[i].destination = false;
            }
        }
        application->refresh_drawing();
    }
}

/**
 * 3.13 actOnHelp
 * @param app
 */
void actOnHelp(GtkWidget *, ezgl::application *app){
    // Pop-up dialog window indicates no intersection between two input streets
    GObject *window;
    GtkWidget *content_area, *label, *dialog;
    window = app->get_object(app->get_main_window_id().c_str());

    // Create the pop-up dialog window
    dialog = gtk_dialog_new_with_buttons("How to Use Navigation Mode", (GtkWindow *) window,
                                         GTK_DIALOG_DESTROY_WITH_PARENT, ("OK"),
                                         GTK_RESPONSE_ACCEPT, ("CLOSE"), GTK_RESPONSE_REJECT, NULL);

    string output = "- Switch to Navigation Mode:\n"
                    "1. Open Navigation mode: Press on Navigation button;\n"
                    "2. Close Navigation mode: Press on CitySeeker button.\n\n"
                    "- Input Instructions:\n"
                    "1. Click the input boxes, then enter your start position or destination in the format "
                    "'street 1, street 2';\n"
                    "2. The top input box is for start positions, and the bottom input box is for destinations;\n"
                    "3. Click the search button, with a little blue plane, to search for best path;\n"
                    "4. Clear inputs: Click the delete icon to clear the whole sentence in the box;\n"
                    "5. Switch start position/destination: Click the swap icon.\n\n"
                    "- Find Path without Typing:\n"
                    "1. When navigation mode is open, click any desired start position/destination on map to "
                    "find best path;\n"
                    "2. When one path is shown, click on current start position/destination to cancel this path.\n\n"
                    "- Invalid inputs:\n"
                    "1. Misspells the strat position/destination: a window pops up, directing users to double check "
                    "the position;\n"
                    "2. When the input is a partial name, there may be multiple start positions/destinations:\n"
                    "      a window pops up, directing users to choose the position you want by clicking the icon.\n";

    const char *informationText = output.c_str();

    // Attach the label to the content of the window
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new(informationText);
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(dialog);

    // Register dialog response to the callback function
    g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), app);
}

/**
 * 3.15 Other Callback Setup
 */
void initialSetUp(ezgl::application *app, bool) {
    appPtr = app;
    std::string main_canvas_id = app->get_main_canvas_id();
    GObject *main_canvas = app->get_object(main_canvas_id.c_str());

    // Register callback function
    g_signal_connect(app->get_object("FindButton"), "clicked", G_CALLBACK(actOnFind), app);
    g_signal_connect(app->get_object("SwitchMapComboBox"), "changed", G_CALLBACK(actOnChangeMap), app);
    g_signal_connect(app->get_object("TypeList"), "changed", G_CALLBACK(actOnTypeList), app);
    g_signal_connect(app->get_object("ZoomInButton"), "clicked", G_CALLBACK(press_zoom_in), app);
    g_signal_connect(app->get_object("NavigationButton"), "clicked", G_CALLBACK(actOnNavigation), app);
    g_signal_connect(app->get_object("HelpButton"), "clicked", G_CALLBACK(actOnHelp), app);
    g_signal_connect(app->get_object("CitySeeker"), "clicked", G_CALLBACK(actOnCitySeeker), app);
    g_signal_connect(main_canvas, "scroll_event", G_CALLBACK(actOnScrollMouse), app);

    // Update the status bar message
    //app->update_message("Welcome to our map! Please enter 'street1, street2' to find their intersections.");

    // Redraw
    app->refresh_drawing();
}


/**
 * 4. drawMap function
 * function where we run all the program
 *     4.0 Initialize EZGL
 *     4.1 Initial World
 *     4.2 Canvas-Static
 *     4.3 Callback-Dynamic
 */
void drawMap() {
    //4.0 initialise EZGL
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    ezgl::application application(settings);

    //4.1 Initial World: compare all intersections/curve points/feature points
    ezgl::rectangle initial_world({min_x, min_y}, {max_x, max_y});

    //4.2 Canvas-Static Functions
    application.add_canvas("MainCanvas", drawMainCanvas, initial_world, ezgl::BACKGROUND);

    //4.3 Callback-Dynamic Functions
    application.run(initialSetUp, actOnMouseClick, nullptr, actOnKey);
}