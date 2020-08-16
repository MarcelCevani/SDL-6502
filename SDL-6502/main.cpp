#include "SDL.h"
#include "core_6502.h"


#define WINDOW_WIDTH 640 

SDL_Color colorTable[16];

int main(int argc, char* argv[])
{
    SDL_Event event;
    SDL_Renderer* render;
    SDL_Window* window;
    int i;

    SDL_Init(SDL_INIT_VIDEO);

    /* Create a Window */
    window = SDL_CreateWindow("Hello World!", 100, 100, 320, 320, SDL_WINDOW_SHOWN);
    /* Create a Render */
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);
    
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
  
	for (i = 0; i < WINDOW_WIDTH; ++i)
        SDL_RenderDrawPoint(render, i, i);
    SDL_RenderPresent(render);
    
	struct system system_6502;

	colorTable[0].r = 0;
	colorTable[0].g = 0;
	colorTable[0].b = 0;

	colorTable[1].r = 0xff;
	colorTable[1].g = 0xff;
	colorTable[1].b = 0xff;

	colorTable[2].r = 0x88;
	colorTable[2].g = 0x00;
	colorTable[2].b = 0x00;

	colorTable[3].r = 0xaa;
	colorTable[3].g = 0xff;
	colorTable[3].b = 0xee;

	colorTable[4].r = 0xcc;
	colorTable[4].g = 0x44;
	colorTable[4].b = 0xcc;

	colorTable[5].r = 0x00;
	colorTable[5].g = 0xcc;
	colorTable[5].b = 0x55;

	colorTable[6].r = 0x00;
	colorTable[6].g = 0x00;
	colorTable[6].b = 0xaa;

	colorTable[7].r = 0xee;
	colorTable[7].g = 0xee;
	colorTable[7].b = 0x77;

	colorTable[8].r = 0xdd;
	colorTable[8].g = 0x88;
	colorTable[8].b = 0x55;

	colorTable[9].r = 0x66;
	colorTable[9].g = 0x44;
	colorTable[9].b = 0x00;

	colorTable[10].r = 0xff;
	colorTable[10].g = 0x77;
	colorTable[10].b = 0x77;

	colorTable[11].r = 0x33;
	colorTable[11].g = 0x33;
	colorTable[11].b = 0x33;

	colorTable[12].r = 0x77;
	colorTable[12].g = 0x77;
	colorTable[12].b = 0x77;

	colorTable[13].r = 0xaa;
	colorTable[13].g = 0xff;
	colorTable[13].b = 0x66;

	colorTable[14].r = 0x00;
	colorTable[14].g = 0x88;
	colorTable[14].b = 0xff;

	colorTable[15].r = 0xbb;
	colorTable[15].g = 0xbb;
	colorTable[15].b = 0xbb;

	init_6502_sytem(&system_6502);

	while(1)
	{
		long ticks = 800;

		while (ticks)
		{
			tick_6502_system(&system_6502);
			ticks--;
		}

		SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
		SDL_RenderClear(render);


#if 0
		SDL_SetRenderDrawColor(render, 255, 0, 0, 255);

		for (i = 0; i < WINDOW_WIDTH; ++i)
			SDL_RenderDrawPoint(render, i, i);
#endif	
		int x, y;

		for (y = 0; y < 64; y++)
		for (x = 0; x < 64; x++)
		{
			int color = this->colorTable[this->system_6502.bus.mem[((y / 2) * 32 + (x / 2)) + 0x200] & 0x0F];
			dc.SetPixel(x, y, color);
		}

		
		SDL_RenderPresent(render);

        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

	SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();








#if 0

	COLORREF colorTable[16];



	colorTable[0] = RGB(0, 0, 0);
	colorTable[1] = RGB(0xff, 0xff, 0xff);
	colorTable[2] = RGB(0x88, 0, 0);
	colorTable[3] = RGB(0xaa, 0xff, 0xee);
	colorTable[4] = RGB(0xcc, 0x44, 0xcc);
	colorTable[5] = RGB(0x00, 0xcc, 0x55);
	colorTable[6] = RGB(0x00, 0x00, 0xaa);
	colorTable[7] = RGB(0xee, 0xee, 0x77);
	colorTable[8] = RGB(0xdd, 0x88, 0x55);
	colorTable[9] = RGB(0x66, 0x44, 0x00);
	colorTable[10] = RGB(0xff, 0x77, 0x77);
	colorTable[11] = RGB(0x33, 0x33, 0x33);
	colorTable[12] = RGB(0x77, 0x77, 0x77);
	colorTable[13] = RGB(0xaa, 0xff, 0x66);
	colorTable[14] = RGB(0x00, 0x88, 0xff);
	colorTable[15] = RGB(0xbb, 0xbb, 0xbb);


	init_6502_sytem(&system_6502);
	//CDialogEx::OnOK();


	while (1)
	{
		long ticks = 800;

		while (ticks)
		{
			tick_6502_system(&system_6502);
			ticks--;
		}

		this->RedrawWindow();
	}



#endif
    return 0;
}