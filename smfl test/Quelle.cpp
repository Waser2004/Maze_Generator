#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

// base variables
const int width = 50;
const int height = 50;
const int box_size = 10;

// maze properties
int box_state[width][height];
int box_props[width][height][4];

int box_loc[2] = { 0, 0 };

string path = "";

// function used to get one tile back
void backtracking() {
    char step_back = path[path.length() - 1];
    // north
    if (step_back == 'N') {
        box_loc[1] += 1;
    }
    // east
    if (step_back == 'E') {
        box_loc[0] -= 1;
    }
    // south
    if (step_back == 'S') {
        box_loc[1] -= 1;
    }
    // west
    if (step_back == 'W') {
        box_loc[0] += 1;
    }
    path.pop_back();
}

// function used to determin which way to go
void path_selection(int counter, bool unvis_neigh[]) {
    // get random number
    float random_num = rand() % counter + 1;
    int index = -1;
    while (random_num > 0) {
        index += 1;
        if (unvis_neigh[index] == true) {
            random_num -= 1;
        }
    }
    // move to next pos
    // north
    if (index == 0) {
        box_props[box_loc[0]][box_loc[1]][0] = 1;
        box_props[box_loc[0]][box_loc[1] - 1][2] = 1;
        box_loc[1] -= 1;
        path += "N";
    }
    // east
    if (index == 1) {
        box_props[box_loc[0]][box_loc[1]][1] = 1;
        box_props[box_loc[0] + 1][box_loc[1]][3] = 1;
        box_loc[0] += 1;
        path += "E";
    }
    // south
    if (index == 2) {
        box_props[box_loc[0]][box_loc[1]][2] = 1;
        box_props[box_loc[0]][box_loc[1] + 1][0] = 1;
        box_loc[1] += 1;
        path += "S";
    }
    // west
    if (index == 3) {
        box_props[box_loc[0]][box_loc[1]][3] = 1;
        box_props[box_loc[0] - 1][box_loc[1]][1] = 1;
        box_loc[0] -= 1;
        path += "W";
    }
}

bool status_check() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (box_state[x][y] == 0) {
                return false;
                break;
            }
        }
    }
    return true;
}

void maze_generator() {
    // repeating algorythem until every tile has been visited
    while (status_check() == false) {
        // set current position to visited
        box_state[box_loc[0]][box_loc[1]] = 1;
        // --- check for unvisited neighbourcells --- //
        bool unvis_neigh[4] = { false, false, false, false };
        int counter = 0;
        // north
        if (box_loc[1] - 1 >= 0 && box_state[box_loc[0]][box_loc[1] - 1] == 0) {
            unvis_neigh[0] = true;
            counter += 1;
        }
        // east
        if (box_loc[0] + 1 < width && box_state[box_loc[0] + 1][box_loc[1]] == 0) {
            unvis_neigh[1] = true;
            counter += 1;
        }
        // south
        if (box_loc[1] + 1 < height && box_state[box_loc[0]][box_loc[1] + 1] == 0) {
            unvis_neigh[2] = true;
            counter += 1;
        }
        // west
        if (box_loc[0] - 1 >= 0 && box_state[box_loc[0] - 1][box_loc[1]] == 0) {
            unvis_neigh[3] = true;
            counter += 1;
        }
        // choose random direction
        if (counter > 0) {
            path_selection(counter, unvis_neigh);
        }
        else {
            backtracking();
        }
    }
}

int main()
{  
    // create the window
    RenderWindow window(VideoMode(width*box_size, height*box_size), "Maze Generator");

    // set Framerate
    window.setFramerateLimit(60);

    // generate maze
    maze_generator();

    while (window.isOpen())
    {   
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed)
                window.close();
        }

        // -------- draw stuff on window ------- //
        window.clear(Color::White);

        // draw edges
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) {
                for (int r = 0; r < 4; r++) {
                    // draw line only if edge is 1
                    if (box_props[w][h][r] == 0 && box_state[w][h] == 1) {
                        Vertex line[2];
                        // north
                        if (r == 0) {
                            line[0] = Vertex(Vector2f(w * box_size, h * box_size), Color::Black);
                            line[1] = Vertex(Vector2f(w * box_size + box_size, h * box_size), Color::Black);
                        }
                        // east
                        if (r == 1) {
                            line[0] = Vertex(Vector2f(w * box_size + box_size, h * box_size), Color::Black);
                            line[1] = Vertex(Vector2f(w * box_size + box_size, h * box_size + box_size), Color::Black);
                        }
                        // south
                        if(r == 2) {
                            line[0] = Vertex(Vector2f(w * box_size, h * box_size + box_size), Color::Black);
                            line[1] = Vertex(Vector2f(w * box_size + box_size, h * box_size + box_size), Color::Black);
                        }
                        // west
                        if (r == 3) {
                            line[0] = Vertex(Vector2f(w * box_size, h * box_size), Color::Black);
                            line[1] = Vertex(Vector2f(w * box_size, h * box_size + box_size), Color::Black);
                        }
                        window.draw(line, 2, Lines);
                    }
                }
            }
        }
        window.display();
    }
}