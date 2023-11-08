#include <global.h>

// rename "mainx" here to "main" to activate this example and rename the "main" in main.cpp to something else
extern "C"
int mainx( int argc, char * argv [] )
{
	SDL_Init( SDL_INIT_EVERYTHING );

	SDL_Window   * window   = SDL_CreateWindow( "Game", 50, 100, 640, 480, SDL_WINDOW_OPENGL );
	SDL_Renderer * renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_PRESENTVSYNC );

	SDL_Rect ball  = { 200, 220, 20, 20 };
	SDL_Rect left  = {  10,  40, 10, 50 };
	SDL_Rect right = { 620, 380, 10, 50 };
	float v_x = 4.8f, v_y = 2.5f;
	float x = 200.0f, y = 160.0f;
	while( true )
	{
		// -=- Read Events -=-

		SDL_PumpEvents();
		const Uint8 * keystate = SDL_GetKeyboardState( nullptr );

		if( (keystate[SDL_SCANCODE_LCTRL] | keystate[SDL_SCANCODE_RCTRL]) == 1
		    && (keystate[SDL_SCANCODE_Q] == 1) )
			return 0; // QUIT

		const int left_move  = keystate[SDL_SCANCODE_S]    - keystate[SDL_SCANCODE_W];
		const int right_move = keystate[SDL_SCANCODE_DOWN] - keystate[SDL_SCANCODE_UP];

		v_x += (keystate[SDL_SCANCODE_L] - keystate[SDL_SCANCODE_J]) * 0.1f;
		v_y += (keystate[SDL_SCANCODE_K] - keystate[SDL_SCANCODE_I]) * 0.1f;

		// -=- Update World -=-

		left.y  += left_move  * 5;
		right.y += right_move * 5;

		// Gravitation?
		//v_y += 0.3f;

		x += v_x;
		y += v_y;

		bool flash = false;
		if( x < 20 )
		{
			const int ldy = y - left.y;
			if( -15 < ldy && ldy < 50 ) // Hit
			{
				x = 40 - x;
			}
			else // Miss
			{
				flash = true;
				x = 310;
				y = 220;
			}
			v_x *= -1;
		}
		else if( x > 600)
		{
			const int rdy = y - right.y;
			if( -15 < rdy && rdy < 50 )
			{
				x = 1200 - x;
			}
			else
			{
				flash = true;
				x = 310;
				y = 220;
			}
			v_x *= -1;
		}

		if( y < 20 )
		{
			y = 40 - y;
			v_y *= -1;
		}
		else if( y > 440)
		{
			y = 880 - y;
			v_y *= -1;
		}

		ball.x = x;
		ball.y = y;

		/// -=- Rendering -=-

		if( flash )	SDL_SetRenderDrawColor( renderer, 127, 127, 127, 255 );
		else		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
		// Clear the screen with the selected DrawColor
		SDL_RenderClear( renderer );

		// Walls
		SDL_SetRenderDrawColor( renderer, 127, 127, 127, 255 );
		const SDL_Rect walls [] = {
			{ 10,  10,  620, 10 },
			{ 10,  460, 620, 10 },
			{ 318, 30,  4,   420 },
		};
		SDL_RenderFillRects( renderer, walls, 3 );

		// Red/left paddle
		SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
		SDL_RenderFillRect( renderer, &left );

		// Blue/right paddle
		SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
		SDL_RenderFillRect( renderer, &right );

		// Green Ball
		SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
		SDL_RenderFillRect( renderer, &ball );

		// Show everything
		SDL_RenderPresent( renderer );
	}
}
