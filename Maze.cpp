#include "Maze.h"

void Maze::MakeMaze()
{
// Nastav vsechny Celly na UNVISITED
    for(int x = 0; x < m_Width / m_CellWidth; x++)
    {
        for(int y = 0; y < m_Height / m_CellHeight; y++)
        {
            maze[x][y] = UNVISITED;
        }
    }
    
// Zacatek na pozici X = 0, Y = 0
    stack.push(std::make_pair(0, 0));
    maze[0][0] = CELL_VISITED;
    
    while(!stack.empty())
    {
        int x = stack.top().first;
        int y = stack.top().second;
        
        std::vector<int> avaibleNeigbours;
        avaibleNeigbours.reserve(4);
        
        // North
        if(maze[x][y + 1] == UNVISITED && y < 22)
            avaibleNeigbours.push_back(0);
        
        // East
        if(maze[x + 1][y] == UNVISITED && x < 28)
            avaibleNeigbours.push_back(1);
            
        // South
        if(maze[x][y - 1] == UNVISITED && y > 0)
            avaibleNeigbours.push_back(2);
        
        // West
        if(maze[x - 1][y] == UNVISITED && x > 0)
            avaibleNeigbours.push_back(3);
        

        if(!avaibleNeigbours.empty())
        {
            int randInt = avaibleNeigbours[rand() % avaibleNeigbours.size()];
            
            switch (randInt)
            {
                    // North
                case 0:
                    stack.push(std::make_pair(x, y + 1));
                    maze[x][y + 1] = CELL_VISITED;
                    vector_of_N_pos.push_back({x, y});
                    vector_of_S_pos.push_back({x, y + 1});
                    break;
                    
                    // East
                case 1:
                    stack.push(std::make_pair(x + 1, y));
                    maze[x + 1][y] = CELL_VISITED;
                    vector_of_E_pos.push_back({x, y});
                    vector_of_W_pos.push_back({x + 1, y});
                    break;
                    
                    // South
                case 2:
                    stack.push(std::make_pair(x, y - 1));
                    maze[x][y - 1] = CELL_VISITED;
                    vector_of_S_pos.push_back({x, y});
                    vector_of_N_pos.push_back({x, y - 1});
                    break;
                    
                    // West
                case 3:
                    stack.push(std::make_pair(x - 1, y));
                    maze[x - 1][y] = CELL_VISITED;
                    vector_of_W_pos.push_back({x, y});
                    vector_of_E_pos.push_back({x - 1, y});
                    break;
                    
                default:
                    break;
            }
        }
        else
        {
            stack.pop();
            maze[x][y] = CELL_VISITED;
        }
    }
}

void Maze::DrawMaze(const Texture& texture, const Texture& texture2,
                    const VertexArray& va, const VertexArray& vao5, const VertexArray& vao6,
                    const IndexBuffer& ib)
{
    Shader shader("OpenGL_tutorial/basic.shader");
    shader.Bind();

    texture.Bind();
    
    for(auto pos : vector_of_N_pos)
    {
        renderer.DrawCell(pos.x * 23, pos.y * 23, vao5, ib, shader);
    }
    
    for(auto pos : vector_of_E_pos)
        renderer.DrawCell(pos.x * 23, pos.y * 23, vao6, ib, shader);

    for(int x = 0; x < m_Width / m_CellWidth; x++)
    {
        for(int y = 0; y < m_Height / m_CellHeight; y++)
        {
            if(maze[x][y] == CELL_VISITED)
                renderer.DrawCell(x * 23, y * 23, va, ib, shader);
            
            if(maze[x][y] == UNVISITED)
            {
                texture2.Bind();
                renderer.DrawCell(x * 23, y * 23, va, ib, shader);
            }
        }
    }
}
