#pragma once

#include "Libraries.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"

enum
{
    CELL_VISITED = 2,
    UNVISITED = 3
};

struct Positions
{
    Positions(int x, int y)
        : x(x), y(y)
    {}
    
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
    
    std::vector<Positions> vector_of_N_pos;
    std::vector<Positions> vector_of_E_pos;
    std::vector<Positions> vector_of_S_pos;
    std::vector<Positions> vector_of_W_pos;
    
    int maze[28][22];

    void MakeMaze();
    
    void DrawMaze(const Texture& texture, const Texture& texture2,
                  const VertexArray& va, const VertexArray& vao5, const VertexArray& vao6,
                  const IndexBuffer& ib) const;
    
    Renderer renderer;
};
