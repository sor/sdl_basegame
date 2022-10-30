#include "global.h"

#include "examplegame.h"

extern "C"
int main( int argc, char* argv[] )
try
{
	ExampleGame game;
	return game.Run();
}
catch( ... )
{
	std::cout << "Non-SDL Exception has occured!" << std::endl;

	// put a breakpoint in this line, to prevent the shell from closing
	return 66;
}
