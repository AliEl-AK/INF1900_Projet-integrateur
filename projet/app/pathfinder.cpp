#include <pathfinder.h>

Pathfinder::Pathfinder(){

}

int Pathfinder::get_path(int* prevNodeFastest, int node){
    int cur_entry = node;
    int idx = 0;
    while (cur_entry != -1) {
        path[28 - (++idx)] = cur_entry;
        cur_entry = prevNodeFastest[cur_entry];
    }
    return idx;
}

int Pathfinder::dijktra(int (*map)[4][2], int startingNode, int pointToGo){
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

int Pathfinder::convertToActions(int starting, int ending, Orientations startingOrientation){
    int size = dijktra(map, starting, ending);
    int positionOfNewArray = 0;
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
            }
            positionOfNewArray++;
        }
        
        direction = nextDirection;
        directionMap[positionOfNewArray] = direction;

        
        if (corners[nextNode]) {
            actionPath[positionOfNewArray] = NavigateLines::Directions::STRAIGHT;
            positionOfNewArray++;
        }
        
        currentNode = nextNode;
    }
    if (actionPath[positionOfNewArray] != NavigateLines::Directions::STRAIGHT) {
        actionPath[positionOfNewArray] = NavigateLines::Directions::STRAIGHT;
        positionOfNewArray++;
    }

    return positionOfNewArray;
}

void Pathfinder::deleteNode(uint8_t node){
    int entries = mapSize[node];
    for (int i = 0; i < entries; i++){
        map[node][i][1] = inf;
    }
}

void Pathfinder::uint8ToString(uint8_t value, char* buffer, unsigned int size) {
    // Ensure buffer is not null and has enough space
    if (buffer != nullptr && size > 0) {
        // Convert each digit of the number to characters
        for (unsigned int i = size - 1; i > 0; --i) {
            buffer[i - 1] = '0' + value % 10;
            value /= 10;
        }
        buffer[size - 1] = '\0';  // Null-terminate the string
    }
}



bool Pathfinder::validationPressed(){
    if (!(PINB & (1 << PB0))){
        _delay_ms(30);
        return !(PINB & (1 << PB0));
    }
    return false;
}


uint8_t Pathfinder::init(){
    LCM disp(&LCD_DDR, &LCD_PORT);
    row = 1;
    col = 1;
    DDRB &= ~(1 << PB0);
    resetMap();
    while (1){
        switch(currentAddState){
            case Pathfinder::AddState::ROW:
                if (rowFlag){
                    disp.clear();
                    disp.write("LIGNES");
                    char buffer[2];
                    uint8ToString(row, buffer, sizeof(buffer));
                    disp.write(buffer, 16);
                    rowFlag = false;
                    _delay_ms(700);
                    buttonFlag = true;
                }
                if (validationPressed()){
                    disp.clear();
                    currentAddState = Pathfinder::AddState::COL;
                }
                break;
            case Pathfinder::AddState::COL:
                if (colFlag){
                    disp.clear();
                    disp.write("COLONNES");
                    char buffer2[2];
                    uint8ToString(col, buffer2, sizeof(buffer2));
                    disp.write(buffer2, 16);
                    colFlag = false;
                    _delay_ms(700);
                    buttonFlag = true;
                }
                if (validationPressed()){
                    disp.clear();
                    currentAddState = Pathfinder::AddState::VAL;
                }
                break;
            case Pathfinder::AddState::VAL:
                if (valFlag){
                    disp.clear();
                    char buffer3[2];
                    uint8ToString(row, buffer3, sizeof(buffer3));
                    char buffer4[2];
                    uint8ToString(col, buffer4, sizeof(buffer4));
                    disp.write("( ,  ) OK?");
                    disp.write(buffer3, 1);
                    disp.write(buffer4, 4);
                    if (isYes){
                        disp.write("OUI", 16);
                    } else {
                        disp.write("NON", 16);
                    }
                    valFlag = false;
                    _delay_ms(700);
                    buttonFlag = true;
                }
                if (validationPressed()){
                    disp.clear();
                    currentAddState = Pathfinder::AddState::ROW;
                    rowFlag = true;
                    colFlag = true;
                    valFlag = true;
                    if (isYes) {
                        return (((row-1)*7) + col - 1);
                    }
                    row = 1;
                    col = 1;
                    isYes = true;
                }
                break;
        }
    }
}



void Pathfinder::resetMap(){
    for (int i = 0; i < 28; i++){
        for (int j = 0; j < mapSize[i]; j++){
            for (int z = 0; z < 2; z++){
                map[i][j][z] = savedMap[i][j][z];
            }
        }
    }
}