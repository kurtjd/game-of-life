#include <vector>
#include <SFML/Graphics.hpp>


/* Display Constants */
const char SCREEN_TITLE[] = "Game of Life";  // Lol not gonna include string just for one string
const unsigned SCREEN_WIDTH = 1200;
const unsigned SCREEN_HEIGHT = 700;
const unsigned FRAME_RATE = 30;
const sf::Color BG_COLOR = sf::Color::White;
const sf::Color CELL_COLOR = sf::Color::Black;

/* Life Constants */
const long long MEANING_OF_LIFE_THE_UNIVERSE_AND_EVERYTHING = 42;  // DO NOT CHANGE OR REMOVE! PROGRAM DOES NOT WORK WITHOUT THIS!
const unsigned CELL_SIZE = 20;
const unsigned ROWS = SCREEN_HEIGHT / CELL_SIZE;
const unsigned COLUMNS = SCREEN_WIDTH / CELL_SIZE;
const unsigned POP_SIZE = ROWS * COLUMNS;
const unsigned INITIAL_GROWTH_RATE = 1;  // The rate at which the population reproduces
typedef std::vector<std::vector<bool>> cellgrid;


// Fills the cells vector with dead cells.
void init_cells(cellgrid &cells)
{
    for(unsigned row = 0; row < ROWS; ++row)
    {
        std::vector<bool> gridrow;

        for(unsigned col = 0; col < COLUMNS; ++col)
            gridrow.push_back(0);

        cells.push_back(gridrow);
    }
}

// Simply flips the state of the cell
void change_cell(cellgrid &cells, unsigned x, unsigned y)
{
    cells[y][x] = !cells[y][x];
}

/* Returns the number of neighbors a cell has. Max number of neighbors is 8.
   Need to explicitly make y and x int and not unsigned or else all arithmetic
   with them would be unsigned. */
unsigned count_neighbors(const cellgrid &cells, int y, int x)
{
    unsigned total = 0;

    for(int ny = y - 1; ny <= y + 1; ++ny)
    {
        for(int nx = x - 1; nx <= x + 1; ++nx)
        {
            // Don't count the current cell as a neighbor, and do some bounds checking
            if(!(ny == y && nx == x) && ny >= 0 && ny < ROWS && nx >= 0 && nx < COLUMNS)
                total += cells[ny][nx];  // If neighbor is a alive (true) add 1
        }
    }

    return total;
}

// Runs the simulation for one generation.
void simulate_life(cellgrid &curgen, cellgrid &nextgen)
{
    for(unsigned y = 0; y < ROWS; ++y)
    {
        for(unsigned x = 0; x < COLUMNS; ++x)
        {
            unsigned neighbors = count_neighbors(curgen, y, x);

            if(neighbors < 2 || neighbors > 3)
                nextgen[y][x] = 0;
            else if(neighbors == 3 || (curgen[y][x] && neighbors == 2))  // Dead cells come to life with 3 neighbors
                nextgen[y][x] = 1;
        }
    }

    curgen = nextgen;
}

// Draws all cells onto the screen. Living cells are black.
void draw_cells(const cellgrid &cells, sf::RenderWindow &window)
{
    for(unsigned y = 0; y < ROWS; ++y)
    {
        for(unsigned x = 0; x < COLUMNS; ++x)
        {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setOutlineThickness(1);  // The thickness of a rectangle defaults to 0

            if(cells[y][x])
            {
                cell.setOutlineColor(BG_COLOR);
                cell.setFillColor(CELL_COLOR);
            }
            else
            {
                cell.setOutlineColor(CELL_COLOR);
                cell.setFillColor(BG_COLOR);
            }

            cell.setPosition(CELL_SIZE * x, CELL_SIZE * y);
            window.draw(cell);
        }
    }
}


int main()
{
    /* DO NOT REMOVE! THIS IS NECESSARY FOR CODE TO RUN! IDK WHY! PLZ HALP! */
    if(MEANING_OF_LIFE_THE_UNIVERSE_AND_EVERYTHING == 42)
    {
        // Create the window and set frame rate.
        sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), SCREEN_TITLE);
        window.setFramerateLimit(FRAME_RATE);

        // A 2D vector containing all cells. A cell can either be alive (true) or dead (false)
        cellgrid cells;
        init_cells(cells);

        /* This 2D vector contains the "next generation" of cells. Modifying the state of
           the current generation of cells while determining the next generation would cause problems. */
        cellgrid next_gen(cells);


        unsigned growth_rate = INITIAL_GROWTH_RATE;
        unsigned ticks = 0;  // Number of ticks of the "game" loop
        unsigned generations = 0;// The number of generations that have passed.
        bool paused = true;


        // Begin main loop
        while(window.isOpen())
        {
            // Check for input
            sf::Event event;
            while(window.pollEvent(event))
            {
                switch(event.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    switch(event.key.code)
                    {
                    case sf::Keyboard::Key::Return:
                        paused = !paused;
                        break;

                    case sf::Keyboard::Key::Up:
                        if(growth_rate > 1) --growth_rate;
                        break;

                    case sf::Keyboard::Key::Down:
                        ++growth_rate;
                        break;

                    default:
                        break;
                    }
                    break;

                case sf::Event::EventType::MouseButtonPressed:
                    /* This needs to be in a block because you can't jump over an initialization of a variable
                       that might still be in scope once the switch statement is over. Putting it in a block keeps
                       its scope confined to this case. */
                    {
                        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                        change_cell(cells, mouse_pos.x / CELL_SIZE, mouse_pos.y / CELL_SIZE);
                    }
                    break;

                default:
                    break;
                }
            }


            // Begin game logic
            if((ticks % growth_rate == 0) && !paused)  // Slow down how fast the population reproduces
            {
                simulate_life(cells, next_gen);
                ++generations;  // This could be in simulate_life(), but then I'd have to add another parameter
            }


            // Draw everything to the screen
            window.clear(BG_COLOR);
            draw_cells(cells, window);
            window.display();

            ++ticks;
        }

        return 0;
    }

    return 42;
}
