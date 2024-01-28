#include <navigateLines.h>

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

enum class Orientations {
    NORTH = -7,
    EAST = 1,
    SOUTH = 7,
    WEST = -1
};

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

bool corners[28] = {};



const int inf = 9999;
int path[28] = {};
int prevNodeForFastest[28] = {};
int finalPath[30];
NavigateLines::Directions actionPath[28];

int get_path(int* prevNodeFastest, int node) {
    int cur_entry = node;
    int idx = 0;
    while (cur_entry != -1) {
        path[28 - (++idx)] = cur_entry;
        cur_entry = prevNodeFastest[cur_entry];
    }
    return idx;
}

int dijktra(int (*map)[4][2], int startingNode, int pointToGo){
    int visited[28] = {};
    int maxPath[28] = {};
    

    for (int i = 0; i < 28; i++) {
        prevNodeForFastest[i] = -1;
    }

    for (int i = 0; i < 28; i++){
        maxPath[i] = inf;
    }

    maxPath[startingNode] = 0;
    
    for (int i = 0; i < 28; i++){
        int smallestValue = inf;
        int smallestValueIndex = -1;
        for (int j=0; j < 28; j++){
            if (maxPath[j] < smallestValue && visited[j] == 0) {
                smallestValue = maxPath[j];
                smallestValueIndex = j;
            }
        }
        visited[smallestValueIndex] = 1;
        int (*adjList)[2] = map[smallestValueIndex];
        int entries = mapSize[smallestValueIndex];
        for (int j = 0; j < entries; j++) { //check the neighbours 
            int neighbour = adjList[j][0];
            int neighbourWeight = adjList[j][1];
            int challenger = maxPath[smallestValueIndex] + neighbourWeight;
            if (challenger < maxPath[neighbour]) {
                maxPath[neighbour] = challenger;
                prevNodeForFastest[neighbour] = smallestValueIndex;
            }
        }
    }
    int size = get_path(prevNodeForFastest, pointToGo);
    for (int i = 0; i < size; i++){
        finalPath[i] = path[28 - (size - i)];
    }
    return size;
}

int convertToActions(int starting, int ending, Orientations startingOrientation){
    int size = dijktra(map, starting, ending);
    int positionOfNewArray = 0;
    for (int i = 0; i < size; i ++){
    }
    int currentNode = starting;
    Orientations direction = startingOrientation;
    for (int i = 0; i < 28; i++){
        if (i == 1 || i == 4 || i == 19 || i == 20 || i == 22){
            corners[i] = false;
        } else {
            corners[i] = true;
        }
    }
    for (int i = 1; i < size; i++){
        
        int nextNode = finalPath[i];
        Orientations nextDirection = static_cast<Orientations>((nextNode - currentNode));
        
        //Directions
        if (direction != nextDirection){
            if (static_cast<int>(direction) + static_cast<int>(nextDirection) == 0){
                actionPath[positionOfNewArray] = NavigateLines::Directions::TURN_180;
            } else {
                switch(nextDirection){
                    case Orientations::WEST:
                    case Orientations::NORTH:
                        if (direction == Orientations::EAST || direction == Orientations::NORTH){
                            actionPath[positionOfNewArray] = NavigateLines::Directions::LEFT;
                        } else {
                            actionPath[positionOfNewArray] = NavigateLines::Directions::RIGHT;
                        }
                        break;
                    case Orientations::EAST:
                    case Orientations::SOUTH:
                        if (direction == Orientations::EAST || direction == Orientations::NORTH){
                            actionPath[positionOfNewArray] = NavigateLines::Directions::RIGHT;
                        } else {
                            actionPath[positionOfNewArray] = NavigateLines::Directions::LEFT;
                        }
                        break;
                }
                positionOfNewArray++;
            }
        }
        
        direction = nextDirection;

        
        if (corners[nextNode]) {
            actionPath[positionOfNewArray] = NavigateLines::Directions::STRAIGHT;
            positionOfNewArray++;
        }
        
        currentNode = nextNode;
    }
    return positionOfNewArray;
}

Timer0 timer;

Motor motor(timer, (1 << PB2), (1 << PB5));
MakerLine makerLine(&DDRA, &PINA, (1 << PA3), (1 << PA4), (1 << PA5), (1 << PA6), (1 << PA7));
NavigateLines navigate(&motor, &makerLine);

int main(){
    int size = convertToActions(0, 16, Orientations::SOUTH);

    navigate.movePath(actionPath, size);
}
