#include "game_of_life.h"

#define WIDTH  128
#define HEIGHT 128
#define BITS_PER_BYTE 8

uint8_t gol_Grid[HEIGHT][WIDTH / BITS_PER_BYTE] = {0};  // 2D bit array
uint8_t gol_Grid_Buffer[HEIGHT][WIDTH / BITS_PER_BYTE] = {0};  // 2D bit array
uint16_t frameBuffer[HEIGHT][WIDTH];  // Expanded grid with 1 byte per pixel


void seed_rand() {
    srand(SysCtlClockGet());  // Seed with system clock
}

uint8_t get_random_bit() {
    return (rand() % 4) == 0;  // 75% chance for 1, 25% for 0
}

void set_cell(int x, int y, int state) {
    if (state)
        gol_Grid[y][x / BITS_PER_BYTE] |= (1 << (x % BITS_PER_BYTE));
    else
        gol_Grid[y][x / BITS_PER_BYTE] &= ~(1 << (x % BITS_PER_BYTE));
}

// int get_cell(int x, int y) {
//     x = (x + WIDTH) % WIDTH;   // Wrap around horizontally
//     y = (y + HEIGHT) % HEIGHT; // Wrap around vertically
//     return (gol_Grid_Buffer[y][x / BITS_PER_BYTE] >> (x % BITS_PER_BYTE)) & 1;
// }
int get_cell(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return 0;  // Treat out-of-bounds cells as dead
    }
    return (gol_Grid_Buffer[y][x / BITS_PER_BYTE] >> (x % BITS_PER_BYTE)) & 1;
}

void randomizeBoard()
{
    for(int i = 0; i < WIDTH; i++)
    {
        for(int j = 0; j < HEIGHT; j++)
        {
            uint8_t random_bit = get_random_bit();
            set_cell(i, j, random_bit);
            if(random_bit == 0)
            {
                //BSP_LCD_DrawPixel(i,j,BSP_LCD_Color565(0,0,0));
            }
            else{
                //BSP_LCD_DrawPixel(i,j,BSP_LCD_Color565(0,0,200));
            }
        }
    }

    memcpy(gol_Grid_Buffer, gol_Grid, sizeof(gol_Grid));
}


void golInit()
{
    seed_rand();
    randomizeBoard();
}

int max_cells = 0;
void nextGeneration()
{
    for(int i = 0; i < WIDTH; i++)
    {
        for(int j = 0; j < HEIGHT; j++)
        {
            int cellState = get_cell(i, j);
            int totalNeighbors = 0;

            // Top Left
            totalNeighbors += get_cell(i-1, j+1);

            // Top
            totalNeighbors += get_cell(i, j+1);

            // Top Right
            totalNeighbors += get_cell(i+1, j+1);

            // Left
            totalNeighbors += get_cell(i-1, j);

            // Right
            totalNeighbors += get_cell(i+1, j);

            // Bottom Left
            totalNeighbors += get_cell(i-1, j-1);

            // Bottom
            totalNeighbors += get_cell(i, j-1);

            // Bottom Right
            totalNeighbors += get_cell(i+1, j-1);

            //UARTprintf("x: %d    y: %d     totalNeighbors: %d      state: %d      \n  ", i, j, totalNeighbors, cellState);
            if((totalNeighbors < 2 || totalNeighbors > 3) && cellState == 1)
            {
                //UARTprintf("Cell died \n  ");
                set_cell(i, j, 0);
                // BSP_LCD_DrawPixel(i,j,BSP_LCD_Color565(0,0,0));
            }
            else if(totalNeighbors == 3 && cellState == 0)
            {
                //UARTprintf("totalNeighbors == 3 cellState==0  \n  ");
                set_cell(i, j, 1);
                
                // BSP_LCD_DrawPixel(i,j,BSP_LCD_Color565(0,200,0));
            }
            else if((totalNeighbors == 2 || totalNeighbors == 3) && cellState == 1)
            {
                set_cell(i, j, 1);
                if(totalNeighbors == 2)
                {
                   // UARTprintf("totalNeighbors == 2\n  ");
                // BSP_LCD_DrawPixel(i,j,BSP_LCD_Color565(0,200,0));
                }
                else
                {
                    //UARTprintf("totalNeighbors == 3 \n  ");
                // BSP_LCD_DrawPixel(i,j,BSP_LCD_Color565(0,200,0));
                }
            }
            else
            {
                //UARTprintf("else \n  ");
                set_cell(i, j, 0);
                // BSP_LCD_DrawPixel(i,j,BSP_LCD_Color565(0,0,0));
            }
        }
    }

    // int counter = 0;
    // bool dobreak = false;
    // for(int i = 0; i < HEIGHT; i++)
    // {
    //     for(int j = 0; j < WIDTH; j++)
    //     {
    //         set_cell(j, i, 1);
    //         counter++;
    //         if(counter >= max_cells)
    //         {
    //             dobreak = true;
    //             UARTprintf("i: %d\tj: %d\n  ", i, j);
    //             max_cells++; 
    //             break;
    //         }
    //     }

    //     if(dobreak)
    //     {
    //         break;
    //     }
    // }


    
    memcpy(gol_Grid_Buffer, gol_Grid, sizeof(gol_Grid));

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            // int byteIndex = x / 8;
            // int bitIndex = x % 8;

            // // Extract bit value (1 or 0) and convert it to a full byte (0xFF or 0x00)
            // frameBuffer[y][x] = (gol_Grid[y][byteIndex] & (1 << (7 - bitIndex))) ? 0xFF : 0x00;
            frameBuffer[y][x] = get_cell(x, y) != 0 ? 0xFF : 0x00;
        }
    }


    // UARTprintf("\n");
    // for(int y = 0; y < HEIGHT; y++)
    // {
    //     for(int x = 0; x < WIDTH; x++)
    //     {
    //         // Print '1' or '0' based on cell value
    //         // Assuming your print function is called print_char()
    //         if (frameBuffer[y][x] != 0) {
    //             UARTprintf("1");
    //         } else {
    //             UARTprintf("0");
    //         }
    //     }
    //     // Print newline character at the end of each row
    //     UARTprintf("\n");
    // }

    // UARTprintf("\n");
    // UARTprintf("\n");
    // UARTprintf("\n");
    // UARTprintf("sendGolGridToLCD...\n  ");
    // BSP_LCD_DrawBuffer((uint16_t *)frameBuffer);
}