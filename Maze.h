#pragma once

#include <cstdlib>
#include <ctime>
#include <stack>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <utility>
#include <vector>

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"

enum
{
    CELL_PATH_N = 0,
    CELL_PATH_E = 1,
    CELL_PATH_S = 2,
    CELL_PATH_W = 3,
    CELL_VISITED = 4,
    UNVISITED = 5
};

struct Maze
{
    Maze(int width, int height)
        : m_Width(width), m_Height(height), m_CellWidth(23), m_CellHeight(23)
    {}
    
    int m_Width, m_Height;
    
    int m_CellWidth, m_CellHeight;
    
    int VisitedCells;
    
    int m_Maze[28][21];
    
    int maze[28][21];
    
    void MakeMaze();
    
    void DrawMaze(const Texture& texture, const Texture& texture2, const VertexArray& va, const IndexBuffer& ib);
    
    int GetMazeWidth() const;
    int GetMazeHeight() const;
    
    int GetCellWidth() const;
    int GetCellHeight() const;
    
    Renderer renderer;
};
