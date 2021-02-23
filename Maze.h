#pragma once

#include "Libraries.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"

enum
{
    CELL_PATH_N = 5,
    CELL_PATH_E = 6,
    CELL_PATH_S = 7,
    CELL_PATH_W = 8,
    CELL_VISITED = 9,
    UNVISITED = 10
};

struct Positions
{
    int x;
    int y;
};

struct Maze
{
    Maze(int width, int height)
        : m_Width(width), m_Height(height), m_CellWidth(23), m_CellHeight(23)
    {}
    
    int m_Width, m_Height;
    
    int m_CellWidth, m_CellHeight;

    std::stack<std::pair<int, int>> stack;
    
    std::vector<Positions> stack_drawN;
    std::vector<Positions> stack_drawE;
    
    int maze[28][22];
    
    void MakeMaze();
    
    void DrawMaze(const Texture& texture, const Texture& texture2,
                  const VertexArray& va, const VertexArray& vao5, const VertexArray& vao6,
                  const IndexBuffer& ib);
    
    Renderer renderer;
};
