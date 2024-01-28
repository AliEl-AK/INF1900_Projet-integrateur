#include <navigateLines.h>
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"
#include <common.h>

class Pathfinder{
    public:
        volatile uint8_t row = 1;
        volatile uint8_t col = 1;
        volatile bool isYes = true;
        volatile bool rowFlag = true;
        volatile bool colFlag = true;
        volatile bool valFlag = true;
        volatile bool buttonFlag = true;

        int map[28][4][2] = {
            {{1,1}, {7,1}}, //0
            {{0,1}, {2,1}}, //1
            {{1,1}, {9,1}, {3,1}}, //2
            {{2,1}, {10,1}}, //3
            {{5,1}}, //4
            {{4,1}, {6,2}}, //5
            {{5,2}, {13,1}}, //6
            {{0,1}, {8,5}}, //7
            {{7,5}, {9,1}, {15,1}}, //8
            {{8,1}, {2,1}, {16, 5}}, //9
            {{3,1}, {11,2}}, //10
            {{10,2}, {18,5}, {12,1}}, //11
            {{11,1}, {5,2}, {13,1}, {19,1}}, //12
            {{12,1}, {6,1}, {20,1}}, //13
            {{15,2}, {21,5}}, //14
            {{14,2}, {8,1}, {16,1}}, //15
            {{15,1}, {9,5}, {17,1}, {23,1}}, //16
            {{16,1}, {24,1}, {18,5}}, //17
            {{17,5}, {11,5}, {25,5}}, //18
            {{12,1}, {26,1}}, //19
            {{13,1}, {27,1}}, //20
            {{14,5}, {22,1}}, //21
            {{21,1}, {23,1}}, //22
            {{22,1}, {16,1}}, //23
            {{17,1}, {25,2}}, //24
            {{24,2}, {18,5}, {26,5}}, //25
            {{25,5}, {19,1}, {27,1}}, //26
            {{26,1}, {20,1}} //27
        };


        int savedMap[28][4][2] = {
            {{1,1}, {7,1}}, //0
            {{0,1}, {2,1}}, //1
            {{1,1}, {9,1}, {3,1}}, //2
            {{2,1}, {10,1}}, //3
            {{5,1}}, //4
            {{4,1}, {6,2}}, //5
            {{5,2}, {13,1}}, //6
            {{0,1}, {8,5}}, //7
            {{7,5}, {9,1}, {15,1}}, //8
            {{8,1}, {2,1}, {16, 5}}, //9
            {{3,1}, {11,2}}, //10
            {{10,2}, {18,5}, {12,1}}, //11
            {{11,1}, {5,2}, {13,1}, {19,1}}, //12
            {{12,1}, {6,1}, {20,1}}, //13
            {{15,2}, {21,5}}, //14
            {{14,2}, {8,1}, {16,1}}, //15
            {{15,1}, {9,5}, {17,1}, {23,1}}, //16
            {{16,1}, {24,1}, {18,5}}, //17
            {{17,5}, {11,5}, {25,5}}, //18
            {{12,1}, {26,1}}, //19
            {{13,1}, {27,1}}, //20
            {{14,5}, {22,1}}, //21
            {{21,1}, {23,1}}, //22
            {{22,1}, {16,1}}, //23
            {{17,1}, {25,2}}, //24
            {{24,2}, {18,5}, {26,5}}, //25
            {{25,5}, {19,1}, {27,1}}, //26
            {{26,1}, {20,1}} //27
        };
        enum class Orientations {
            NORTH = -7,
            EAST = 1,
            SOUTH = 7,
            WEST = -1
        };

        enum class AddState {
            ROW,
            COL,
            VAL
        };

        AddState currentAddState = AddState::ROW;

        int mapSize[28] = {
            2,
            2,
            3,
            2,
            1,
            2,
            2,
            2,
            3,
            3,
            2,
            3,
            4,
            3,
            2,
            3,
            4,
            3,
            3,
            2,
            2,
            2,
            2,
            2,
            2,
            3,
            3,
            2
        };

        Orientations directionMap[30] = {};

        NavigateLines::Directions actionPath[28];

        Pathfinder();

        int get_path(int* prevNodeFastest, int node);

        int dijktra(int (*map)[4][2], int startingNode, int pointToGo);

        int convertToActions(int starting, int ending, Orientations startingOrientation);

        void deleteNode(uint8_t node);

        void uint8ToString(uint8_t value, char* buffer, unsigned int size);

        uint8_t init();

        bool validationPressed();

        void resetMap();
    private:
        bool corners[28] = {};
        const int inf = 9999;
        int path[28] = {};
        int prevNodeForFastest[28] = {};
        int finalPath[30];
};