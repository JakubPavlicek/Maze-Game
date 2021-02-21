#include "Maze.h"

void Maze::MakeMaze()
{
    std::stack<std::pair<int, int>> stack;
    
    stack.push(std::make_pair(0, 0));
    
    VisitedCells = 1;
    
    maze[0][0] = CELL_VISITED;
    
    std::srand((unsigned int)(std::time(0)));
    
    
    for(int x = 0; x < m_Width / m_CellWidth; x++)
    {
        for(int y = 0; y < m_Height / m_CellHeight; y++)
        {
            maze[x][y] = UNVISITED;
        }
    }
    
    
    while(!stack.empty() && VisitedCells < ((m_Width / m_CellWidth) * (m_Height / m_CellHeight)))
    {
        std::cout << "dalsi iterace cislo: " << VisitedCells << std::endl;
        int x = stack.top().first;
        int y = stack.top().second;
        
        //stack.pop();
        
        std::vector<int> avaibleNeigbours;
        avaibleNeigbours.reserve(4);
        
        // North
        if(maze[x][y + 1] == UNVISITED && y < 21)
        {
            std::cout << "NORTH" << std::endl;
            avaibleNeigbours.push_back(0);
        }
        // East
        if(maze[x - 1][y] == UNVISITED && x > 0)
        {
            std::cout << "EAST" << std::endl;
            avaibleNeigbours.push_back(1);
        }
        // South
        if(maze[x][y - 1] == UNVISITED && y > 0)
        {
            std::cout << "SOUTH" << std::endl;
            avaibleNeigbours.push_back(2);
        }
        // West
        if(maze[x + 1][y] == UNVISITED && x < 28)
        {
            std::cout << "WEST" << std::endl;
            avaibleNeigbours.push_back(3);
        }
        
        
        
        if(!avaibleNeigbours.empty())
        {
            int randInt = avaibleNeigbours[rand() % avaibleNeigbours.size()];
            
            switch (randInt)
            {
                    // North
                case 0:
                    stack.push(std::make_pair(x, y + 1));
                    m_Maze[x][y] = CELL_PATH_N;
                    m_Maze[x][y + 1] = CELL_PATH_S;
                    break;
                    
                    // East
                case 1:
                    stack.push(std::make_pair(x - 1, y));
                    m_Maze[x][y] = CELL_PATH_E;
                    m_Maze[x - 1][y] = CELL_PATH_W;
                    break;
                    
                    // South
                case 2:
                    stack.push(std::make_pair(x, y - 1));
                    m_Maze[x][y] = CELL_PATH_S;
                    m_Maze[x][y - 1] = CELL_PATH_N;
                    break;
                    
                    // West
                case 3:
                    stack.push(std::make_pair(x + 1, y));
                    m_Maze[x][y] = CELL_PATH_W;
                    m_Maze[x + 1][y] = CELL_PATH_E;
                    break;
                    
                default:
                    break;
            }
            maze[x][y] = CELL_VISITED;
            VisitedCells++;
        }
        else
        {
            stack.pop();
        }
    }
}

void Maze::DrawMaze(const Texture& texture, const Texture& texture2, const VertexArray& va, const IndexBuffer& ib)
{
    Shader shader("OpenGL_tutorial/basic.shader");
    shader.Bind();
    
    for(int x = 0; x < m_Width / m_CellWidth; x++)
    {
        for(int y = 0; y < m_Height / m_CellHeight; y++)
        {
            if(maze[x][y] == CELL_VISITED)
            {
                texture.Bind();
                renderer.DrawCell(x * 23, y * 23, va, ib, shader);
            }
            if(maze[x][y] != CELL_VISITED)
            {
                texture2.Bind();
                renderer.DrawCell(x * 23, y * 23, va, ib, shader);
            }
            //                if(m_Maze[x][y] == CELL_PATH_N)
            //                {
            //                    texture_red.Bind();
            //                    renderer.DrawCell(x - 20 + (x * 20), y - 20 + (y * 20), vao5, ibo, shader);
            //                }
            //                if(m_Maze[x][y] == CELL_PATH_W)
            //                {
            //                    texture_red.Bind();
            //                    renderer.DrawCell(x - 20 + (x * 20), y - 20 + (y * 20), vao6, ibo, shader);
            //                }
        }
    }
}

int Maze::GetMazeWidth() const
{
    return m_Width;
}

int Maze::GetMazeHeight() const
{
    return m_Height;
}

int Maze::GetCellWidth() const
{
    return m_CellWidth;
}

int Maze::GetCellHeight() const
{
    return m_CellHeight;
}
