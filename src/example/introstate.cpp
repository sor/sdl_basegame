#include "examplegame.h"
#include "hsnr64.h"

#include <tilefont.h>

void IntroState::Init()
{
	if( !font )
	{
		font = TTF_OpenFont( BasePath "asset/font/RobotoSlab-Bold.ttf", 24 );
		TTF_SetFontHinting( font, TTF_HINTING_LIGHT );
		if( !font )
			print( stderr, "TTF_OpenFont failed: {}\n", TTF_GetError() );
	}

	if( !image )
	{
		image = IMG_LoadTexture( renderer, BasePath "asset/graphic/background.png" );
		if( !image )
			print( stderr, "IMG_LoadTexture failed: {}\n", IMG_GetError() );
	}

	if( !music )
	{
		music = Mix_LoadMUS( BasePath "asset/music/severance.ogg" );
		if( !music )
			print( stderr, "Mix_LoadMUS failed: {}\n", Mix_GetError() );
		else
			Mix_PlayMusic( music, -1 );
	}
	else if( Mix_PausedMusic() )
	{
		Mix_ResumeMusic();
	}

	if( !sound )
	{
		sound = Mix_LoadWAV( BasePath "asset/sound/pew.wav" );
		if( !sound )
			print( stderr, "Mix_LoadWAV failed: {}\n", Mix_GetError() );
	}
}

void IntroState::UnInit()
{
	if( !Mix_PausedMusic() )
		Mix_PauseMusic();

	// Keep everything loaded/allocated is also an option
	/*
	TTF_CloseFont( font );
	SDL_DestroyTexture( image );
	SDL_DestroyTexture( blendedText );
	font = nullptr;
	image = nullptr;
	blendedText = nullptr;
	*/
}

bool IntroState::HandleEvent( const Event & event )
{
	switch( event.type )
	{
		case SDL_KEYDOWN:
		{
			const Keysym & what_key = event.key.keysym;

			if( what_key.scancode == SDL_SCANCODE_F1 && event.key.repeat == 0 )
			{
				if( Mix_PausedMusic() )
					Mix_ResumeMusic();
				else
					Mix_PauseMusic();
			}
			else if( what_key.scancode == SDL_SCANCODE_F2 && event.key.repeat == 0 )
			{
				if( Mix_VolumeMusic( -1 ) == MIX_MAX_VOLUME )
					Mix_VolumeMusic( 0 );
				else
					Mix_VolumeMusic( MIX_MAX_VOLUME );
			}
			else if( what_key.scancode == SDL_SCANCODE_F3 && event.key.repeat == 0 )
			{
				Mix_PlayChannel( -1, sound, 0 );
			}
			else if( what_key.scancode == SDL_SCANCODE_F4 && event.key.repeat == 0 )
			{
				textmode = (textmode + 1) % 2;
			}
			else if( what_key.scancode == SDL_SCANCODE_F9 )
			{
				// crash/shutdown, since State #6 does not exist
				game.SetNextState( 99 );
			}
			else if( what_key.scancode == SDL_SCANCODE_ESCAPE )
			{
				game.SetNextState( 0 );
			}
			else
			{
				return false; // Not handled
			}

			return true; // Confusing control flow: Handled by all but the else case

			break;
		}

		case SDL_MOUSEBUTTONDOWN:
			//game.SetNextState( 1 );
			break;

		default:
			break;
	}

	return false;
}

void IntroState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
}

void IntroState::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
	const Point & winSize = game.GetWindowSize();

	{
		const Rect dst_rect { 0, 0, winSize.x, winSize.y };
		SDL_RenderCopy( renderer, image, EntireRect, &dst_rect /* same result as EntireRect */ );
	}

	// Poor persons benchmark
	//for (uint x = 0; x < 100; ++x)
	{
		constexpr const char * text =
			"                                          --== Introscreen of my Super Mega Gamey Game 3000 ==--\n\n"
			"Dies ist ein Typoblindtext. An ihm kann man sehen, ob alle Buchstaben da sind und wie sie aussehen. "
			"Manchmal benutzt man Worte wie Hamburgefonts, Rafgenduks oder Handgloves, um Schriften zu testen. "
			"Manchmal Sätze, die alle Buchstaben des Alphabets enthalten - man nennt diese Sätze »Pangrams«. "
			"Sehr bekannt ist dieser: The quick brown fox jumps over the lazy old dog. "
			"Oft werden in Typoblindtexte auch fremdsprachige Satzteile eingebaut (AVAIL® and Wefox™ are testing aussi la Kerning), um die Wirkung in anderen Sprachen zu testen. "
			"In Lateinisch sieht zum Beispiel fast jede Schrift gut aus. Quod erat demonstrandum. "
			"Seit 1975 fehlen in den meisten Testtexten die Zahlen, weswegen nach TypoGb. 204 § ab dem Jahr 2034 Zahlen in 86 der Texte zur Pflicht werden. "
			"Nichteinhaltung wird mit bis zu 245 € oder 368 $ bestraft. Genauso wichtig in sind mittlerweile auch Âçcèñtë, die in neueren Schriften aber fast immer enthalten sind. "
			"Ein wichtiges aber schwierig zu integrierendes Feld sind OpenType-Funktionalitäten. "
			"Je nach Software und Voreinstellungen können eingebaute Kapitälchen, Kerning oder Ligaturen (sehr pfiffig) nicht richtig dargestellt werden."
			"\n\nRoyality free music by Karl Casey @ White Bat Audio"
			"\n  - Press [F1] to (un)pause music."
			"\n  - Press [F2] to (un)mute music."
			"\nSource: https://www.youtube.com/watch?v=aFITtvK64B4"
			"\n\nPress any key to continue!"
			"\n\n!\"#$%&'()*+,-./0123456789:;<=>?"
			"\n@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
			"\n`abcdefghijklmnopqrstuvwxyz{|}~"
			"\n\u00A0¡¢£¤¥¦§¨©ª«¬\u00AD®¯°±²³´µ¶·¸¹º»¼½¾¿"
			"\nAÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß"
			"\naàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
			"\ninjektion enjoy major Heij project object farbe Farbe tw";

		static Point p{ 32, 50 };
		static int colorIndex = 9;

#ifdef IMGUI
		//ImGuiOnly(
		{
			static bool auto_update = false;
			static bool drawColorNumber = false;
			//ImGuiIO & io = ImGui::GetIO();
			ImGui::Begin( "Introstate", nullptr, ImGuiWindowFlags_NoFocusOnAppearing );
			if( frame == 0 ) // Do not focus this new window
				ImGui::SetWindowFocus( nullptr );

			if( ImGui::SliderInt( "int", &p.x, 0, 320 ) && auto_update )
				blendedText = nullptr;

			ImGui::Checkbox( "Auto-Redraw", &auto_update );      // Edit bools storing our window open/close state

			if( ImGui::Button( "Redraw" ) )                            // Buttons return true when clicked (most widgets return true when edited/activated)
				blendedText = nullptr;

			if( ImGui::SliderInt( "color index", &colorIndex, 0, 63 ) )
				asm("NOP");

			ImGui::Checkbox( "Draw Color Number", &drawColorNumber );

			ImGui::PushID("fg color");
			ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1);
			// CARE: ImU32 as color is 0xAABBGGRR - opposite of what might be expected
			ImGui::PushStyleColor( ImGuiCol_Border, 0xAAFFFFFF );
			const fmt::format_string<int> fmt = drawColorNumber
			                                    ? fmt::format_string<int>( "{:02}" )
			                                    : fmt::format_string<int>( "  ##{:02}" );
			for( int i = 0; i < 64; ++i )
			{
				ImU32 pcol = std::bit_cast<ImU32>( sor::hsnr64::Palette[i] );
				//Color color = sor::hsnr64::Palette[i];
				ImGui::PushStyleColor( ImGuiCol_Button, pcol );
				ImGui::PushStyleColor( ImGuiCol_Text, pcol ^ 0x00808080 );
				if( ImGui::Button( format( fmt::runtime(fmt), i ).c_str() ) )
					colorIndex = i;
				ImGui::PopStyleColor( 2 );
				//ImGui::ColorButton( format( "color{}", i ).c_str(), *((ImVec4*)&sor::hsnr64::Palette[i]), ImGuiColorEditFlags_Uint8 );
				//if(i%10 != 0)
				if( true
					//&& i != 0
				    && i != 10
				    //&& i != 17
				    && i != 25
				    //&& i != 32
				    && i != 40
				    //&& i != 48
				    //&& i != 57
					&& i != 52
					)
					ImGui::SameLine();
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::PopID();

			ImGui::End();
		}
		//)
#endif

		// Comment out to disable the cache. Uses 5ms without / 20 ms with harfbuzz
		if( blendedText == nullptr )
		{
			if( blendedText != nullptr )
				SDL_DestroyTexture( blendedText );

			Surface * surf = TTF_RenderUTF8_Blended_Wrapped( font, text, white, winSize.x - p.x );
			blendedText = SDL_CreateTextureFromSurface( renderer, surf );
			SDL_FreeSurface( surf );

			u32 fmt;
			int access;
			SDL_QueryTexture( blendedText, &fmt, &access, &blendedTextSize.x, &blendedTextSize.y );
		}

		// Draw the text on top
		if( textmode == 0 )
		{
			SDL_SetTextureColorMod( blendedText, 0, 0, 0 );
			for( const Point & pd : shadowOffsets )
			{
				const Rect dst_rect = Rect{ p.x + pd.x, p.y + pd.y, blendedTextSize.x, blendedTextSize.y };
				SDL_RenderCopy( renderer, blendedText, EntireRect, &dst_rect );
			}

			SDL_SetTextureColorMod( blendedText, 255, 255, 255 );
			const Rect dst_rect = { p.x, p.y, blendedTextSize.x, blendedTextSize.y };
			SDL_RenderCopy( renderer, blendedText, EntireRect, &dst_rect );
		}
		else
		{
			TF_Init( renderer );

			Rect dimension { p.x, p.y, winSize.x - (32+p.x), 9999 };
			TF_Render( renderer, text, dimension, sor::hsnr64::Palette[colorIndex] );
		}
	}
}
