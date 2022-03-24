/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------| Dead Space |-------------------------------------------------------
//======================================= sDs|Aragon* =======================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

/////////////
// Settings |
//==========/

#define INTRO_DELAY					2						// Timpul in care apare intro-ul
#define STARTGAME_DELAY				3						// Secunde pana incepe jocul
#define GAMEOVER_DELAY				5						// Timpul de la final in care iti arata scorul

#define MAX_SPACESHIPS				6						// Numarul maxim de nave inamite -1 (PLAYER)
#define MAX_PROJECTILES				15						// Maximul de proiectile (1 nava)
#define MAX_REWARDS					2						// Numarul maxim de reward-uri pe harta
#define MAX_ASTEROIDS				2						// Numarul maxim de asteroizi pe harta

#define DEGREE						0.0174532925			// Valoarea unui grad (folosit la unghi) in radians
#define MAX_Z						49						// Maxim (Axa Z)


/////////////
// Fereastra |
//==========/

#define MAX_RESOLUTIONS		2								// Numarul maxim de rezolutii din vector

int Resolution[MAX_RESOLUTIONS][2] = {
	{ 1920, 1080 },											// Rezolutia FULL HD
	{ 1280, 720 }											// Rezolutia HD
}, GameResolution = 0;										// Retine rezolutia curenta


/////////////
// PLAYER |
//==========/

//--| Setari |--//
#define PLAYER_LIFE					7						// Vieti (7 max)
#define PLAYER_DELAY_LASER			0.3						// Intervalul minim gloante
#define PLAYER_DELAY_ROCKET			2.0						// Interval minim rachete
#define PLAYER_ROCKET_AMOUNT		2						// Numarul default de rachete
#define PLAYER_DEFAULT_ANGLE		-90						// Unghiul default al navei (Imagine)

int PLAYER_SPEED = 500,										// Viteza de miscare (Scale in functie de rezolutie)
PLAYER_LASER_SPEED = 1000,									// Viteza laser (Scale in functie de rezolutie)
PLAYER_ROCKET_SPEED = 1500;									// Viteza racheta (Scale in functie de rezolutie)

//--| Controls |--//
#define CONTROL_MENU				SDLK_ESCAPE				// Iesi din joc
#define CONTROL_PAUSE				SDLK_p					// Pauza la joc

#define CONTROL_FORWARD				SDLK_w					// Muti nava in fata
#define CONTROL_BACK				SDLK_s					// Muti nava in spate
#define CONTROL_LEFT				SDLK_a					// Muti nava in stanga
#define CONTROL_RIGHT				SDLK_d					// Muti nava in dreapta

#define CONTROL_UP					SDLK_UP					// Muti nava in sus
#define CONTROL_DOWN				SDLK_DOWN				// Muti nava in jos
#define CONTROL_TURN_LEFT			SDLK_LEFT				// Rotesti nava in stanga
#define CONTROL_TURN_RIGHT			SDLK_RIGHT				// Rotesti nava in dreapta
#define CONTROL_STABILIZE			SDLK_LCTRL				// Stabilizezi nava

#define CONTROL_FIRE_LASERS			SDLK_SPACE				// Tragi cu lasere
#define CONTROL_FIRE_ROCKETS		SDLK_LSHIFT				// Tragi cu rachete


/////////////
// ENEMY |
//==========/

#define ENEMY_DELAY_LASER			3.0						// Intervalul minim gloante
#define ENEMY_DELAY_ROCKET			5.0						// Interval minim rachete
#define ENEMY_DEFAULT_ANGLE			90						// Unghiul default al navei (Imagine)

int ENEMY_SPEED = 500,										// Viteza navei  (Scale in functie de rezolutie)
ENEMY_LASER_SPEED = 500,									// Viteza laser  (Scale in functie de rezolutie)
ENEMY_ROCKET_SPEED = 750;									// Viteza racheta  (Scale in functie de rezolutie)


/////////////
// Entitati |
//==========/

#define REWARD_SHIELD_DURATION		4						// Duratia scutului primit
#define REWARD_ROCKETS_AMOUNT		1						// Numarul de rachete primit

int REWARD_SPEED = 1000,									// Viteza entitati reward  (Scale in functie de rezolutie)
ASTEROID_SPEED = 600;										// Viteza entitati asteroid  (Scale in functie de rezolutie)


/////////////
// Images |
//==========/

#define MAX_IMAGES					26						// Numarul maxim de imagini

#define DEFAULT_SCALE				"1.0"					// Modifica marimea imaginii originale
#define SPACESHIP_SCALE				"0.6"					// Modifica marimea imaginii originale (Nava si Proiectile)
#define HUD_SCALE					"0.75"					// Modifica marimea imaginii originale (HUD-ul)
#define MENU_SCALE					"1.0"					// Modifica marimea imaginii originale (Meniu)

#define SPRITE_NONE					"1"						// Pentru imagini care nu sunt sprite-uri

enum ImagesList
{											// Imaginile folosite
	IMG_INTRO,
	IMG_LOGO,
	IMG_BACKGROUND,
	IMG_EXPLOSION,
	// Player
	IMG_PLAYER_SPACESHIP_NORMAL,
	IMG_PLAYER_SPACESHIP_NO_ROCKETS,
	IMG_PLAYER_LASER,
	IMG_PLAYER_ROCKET,
	IMG_PLAYER_SHIELD,
	IMG_PLAYER_HUD,
	// Enemy
	IMG_ENEMY_SPACESHIP_NORMAL,
	IMG_ENEMY_SPACESHIP_NO_ROCKETS,
	IMG_ENEMY_LASER,
	IMG_ENEMY_ROCKET,
	// Entity
	IMG_ENTITY_REWARD_RED,
	IMG_ENTITY_REWARD_BLUE,
	IMG_ENTITY_ASTEROID,

	// Meniu
	IMG_MENU_BACKGROUND,
	// Meniu Principal
	IMG_MENU_STARTGAME,
	IMG_MENU_RESUMEGAME,
	IMG_MENU_SCOREBOARD,
	IMG_MENU_OPTIONS,
	IMG_MENU_QUITGAME,
	// Options
	IMG_MENU_RESOLUTION,
	IMG_MENU_MUSIC,
	IMG_MENU_SOUNDS
};

// Imaginile folosite in joc (Address, Width, Hight, Scale)
char Images[MAX_IMAGES][5][128] = {
	{ "Resources/Images/Intro.png",						"1920",		"1080",	DEFAULT_SCALE,		SPRITE_NONE },	// Imagine Intro
	{ "Resources/Images/Logo.png",						"296",		"100",	DEFAULT_SCALE,		SPRITE_NONE },	// Imagine Logo
	{ "Resources/Images/Background.png",					"1920",		"4320",	DEFAULT_SCALE,		SPRITE_NONE },	// Imaginea de fundal
	{ "Resources/Images/Explosion.png",					"128",		"128",	DEFAULT_SCALE,		"18" },			// Imagine Explozie
	// Player
	{ "Resources/Images/Player/Spaceship.png",			"242",		"159",	SPACESHIP_SCALE,	"2" },			// Imagine Nava jucator
	{ "Resources/Images/Player/Spaceship_Rockets.png",	"242",		"159",	SPACESHIP_SCALE,	"2" },			// Imagine Nava jucator, fara rachete
	{ "Resources/Images/Player/Lasers.png",				"242",		"159",	SPACESHIP_SCALE,	"5" },			// Imagine Lasere jucator
	{ "Resources/Images/Player/Rockets.png",				"242",		"159",	SPACESHIP_SCALE,	"4" },			// Imagine Rachete jucator
	{ "Resources/Images/Player/Shield.png",				"242",		"159",	SPACESHIP_SCALE,	"8" },			// Imagine Scut jucator
	{ "Resources/Images/Player/Hud.png",					"193",		"227",	HUD_SCALE,			"8" },			// Imagine Hud
	// Enemy
	{ "Resources/Images/Enemy/Spaceship.png",				"242",		"159",	SPACESHIP_SCALE,	"2" },			// Imagine Nava inamici
	{ "Resources/Images/Enemy/Spaceship_Rockets.png",		"242",		"159",	SPACESHIP_SCALE,	"2" },			// Imagine Nava inamici, fara rachete
	{ "Resources/Images/Enemy/Lasers.png",				"242",		"159",	SPACESHIP_SCALE,	"5" },			// Imagine Lasere inamici
	{ "Resources/Images/Enemy/Rockets.png",				"242",		"159",	SPACESHIP_SCALE,	"4" },			// Imagine Rachete inamici
	// Entity
	{ "Resources/Images/Entity/Reward_Blue.png",			"128",		"128",	DEFAULT_SCALE,		"12" },			// Imagine Reward albastru
	{ "Resources/Images/Entity/Reward_Red.png",			"128",		"128",	DEFAULT_SCALE,		"12" },			// Imagine Reward rosu
	{ "Resources/Images/Entity/Asteroid.png",				"128",		"52",	DEFAULT_SCALE,		"5" },			// Imagine Asteroid
	// Meniu
	{ "Resources/Images/Menu/Background.jpg",				"1920",		"1200",	DEFAULT_SCALE,		SPRITE_NONE },	// Imagine meniu Background
	{ "Resources/Images/Menu/StartGame.png",				"309",		"105",	MENU_SCALE,			"2" },			// Imagine meniu Start Game
	{ "Resources/Images/Menu/ResumeGame.png",				"309",		"105",	MENU_SCALE,			"2" },			// Imagine meniu Resume Game
	{ "Resources/Images/Menu/Scoreboard.png",				"309",		"105",	MENU_SCALE,			"2" },			// Imagine meniu Scoreboard
	{ "Resources/Images/Menu/Options.png",				"309",		"105",	MENU_SCALE,			"2" },			// Imagine meniu Options
	{ "Resources/Images/Menu/QuitGame.png",				"309",		"105",	MENU_SCALE,			"2" },			// Imagine meniu Quit Game
	{ "Resources/Images/Menu/Resolution.png",				"309",		"105",	MENU_SCALE,			"2" },			// Imagine meniu Quit Game
	{ "Resources/Images/Menu/MusicVol.png",				"309",		"105",	MENU_SCALE,			"2" },			// Imagine meniu Quit Game
	{ "Resources/Images/Menu/SoundsVol.png",				"309",		"105",	MENU_SCALE,			"2" }			// Imagine meniu Quit Game
};

struct Image
{												// Structura folosita pentru imagini
	char IMG_Address[64];									// Adresa imaginii
	int IMG_Size[2],										// Marimea imaginii
		IMG_ScaledSize[2],									// Marimea imaginii
		IMG_Sprite;											// Numarul imagiminilor(In cazul in care este sprite)
	SDL_Surface* IMG_Surface = NULL;						// Suprafata de lucru
	SDL_Texture* IMG_Texture = NULL;						// ID-ul texturii
} IMG_Images[MAX_IMAGES];									// Vectorul care tine minte toate imaginile


/////////////
// Sounds |
//==========/

#define MAX_SOUNDS					5						// Numarul maxim de sunete
#define SOUND_AMBIENCE			"Resources/Sounds/Theme.mp3"	// Sunet background

enum SoundsList
{											// Sunetele folosite
	SND_LASER,
	SND_ROCKET,
	SND_EXPLOSION,
	SND_REWARD_RED,
	SND_REWARD_BLUE,
	SND_ASTEROID
};

// Sunetele folosite in joc (Address, Volum)
char Sounds[MAX_SOUNDS][2][128] = {
	{ "Resources/Sounds/laser.wav" },							// Sunet lasere
	{ "Resources/Sounds/rocket.wav" },						// Sunet rachete
	{ "Resources/Sounds/explosion.wav" },						// Sunet explozii
	{ "Resources/Sounds/reward_blue.wav" },					// Sunet reward albastru
	{ "Resources/Sounds/reward_red.wav" }						// Sunet reward rosu
};

struct Sound
{												// Structura folosita pentru sunete
	char SND_Address[64];									// Adresa sunetului
	Mix_Chunk* SND_Chunk = NULL;							// Pointer catre sunet
} SND_Sounds[MAX_SOUNDS];									// Vectorul care tine minte toate sunetele


/////////////
// Font |
//==========/

#define MAX_FONTS	5										// Numarul maxim de font-uri

enum FontsList
{											// Font-urile folosite
	FNT_SCORE,
	FNT_PLAYER_ALTITUDE,
	FNT_ENEMY_ALTITUDE,
	FNT_MENU_OPTION,
	FNT_MENU_CHOSEN
};

// Font-ul folosite in joc (Address, Marime, Red, Green, Blue (RGB))
char Fonts[MAX_FONTS][5][128] = {
	{ "Resources/Fonts/ScoreBoard.ttf",	"40",	"200",	"50",	"0" },			// Font scor
	{ "Resources/Fonts/ScoreBoard.ttf",	"38",	"0",	"150",	"200" },		// Font altitudine Player
	{ "Resources/Fonts/ScoreBoard.ttf",	"38",	"200",	"50",	"0" },			// Font altitudine Enemy
	{ "Resources/Fonts/DeadSpace.ttf",	"38",	"0",	"150",	"255" },		// Font optiune meniu
	{ "Resources/Fonts/DeadSpace.ttf",	"38",	"255",	"150",	"0" }			// Font optiune aleasa
};

struct Font
{												// Structura folosita pentru font
	char FNT_Address[64];									// Adresa font-ului
	int FNT_Size[2];										// Dimensiunea text-ului pe ecran
	TTF_Font* FNT_Font = NULL;								// Pointer catre font
	SDL_Color FNT_Color;									// Culoarea font-ului
	SDL_Surface* FNT_Surface = NULL;						// Suprafata de lucru
	SDL_Texture* FNT_Texture = NULL;						// ID-ul texturii
} FNT_Fonts[MAX_FONTS];										// Vectorul care tine minte toate font-urile


/////////////
// Meniu |
//==========/

#define CONTROL_MENU_EXIT			SDLK_ESCAPE				// Iesi din meniu
#define CONTROL_MENU_UP				SDLK_UP					// Navighezi in sus in optiuni
#define CONTROL_MENU_DOWN			SDLK_DOWN				// Navighezi in jos in optiuni
#define CONTROL_MENU_ENTER			SDLK_RETURN				// Alegi o optiune

#define MAX_ITEMS_MAIN				4						// Numarul de iteme din meniul principal
#define MAX_ITEMS_OPTIONS			3						// Numarul de iteme din meniul de optiuni
#define MENU_ANGLE					-5						// Unghiul la care sunt inclinate imaginile

ImagesList Menu_Main[MAX_ITEMS_MAIN] = {					// Lista cu itemele pentru meniul principal (StartGame)
	IMG_MENU_STARTGAME,
	IMG_MENU_SCOREBOARD,
	IMG_MENU_OPTIONS,
	IMG_MENU_QUITGAME
};

ImagesList Menu_Main2[MAX_ITEMS_MAIN] = {					// Lista cu itemele pentru meniul principal (ResumeGame)
	IMG_MENU_RESUMEGAME,
	IMG_MENU_SCOREBOARD,
	IMG_MENU_OPTIONS,
	IMG_MENU_QUITGAME
};

ImagesList Menu_Options[MAX_ITEMS_OPTIONS] = {				// Lista cu itemele pentru meniul de optiuni
	IMG_MENU_RESOLUTION,
	IMG_MENU_MUSIC,
	IMG_MENU_SOUNDS
};


////////////
// Globals |
//=========/

//--| Structuri |--//
enum Sprite_Vector
{										// Tipul de data din vector(folosit in structura)
	SPRITE_POS = 0,
	SPRITE_DELAY
};

//--| Proiectile |--//
enum Projectile_Types
{										// Tipul de proiectile (folosit in structura)
	LASER_PLAYER = 0,
	ROCKET_PLAYER,
	LASER_ENEMY,
	ROCKET_ENEMY
};

struct Projectile
{											// Structura pentru proiectile
	bool P_Active;											// Statusul proiectilului
	double P_Angle;											// Unghiul proiectilului
	Projectile_Types P_Type;								// Tipul proiectilului din (Projectile_Types)
	int P_Origin[3],										// Originea proiectilului din (Positions)
		P_Sprite[2],										// Sprite pentru proiectil (Sprite_Vector)
		P_Speed;											// Viteza proiectilului
};

//--| Nave |--//
enum SpaceShip_Types
{										// Tipul de nave (folosit in structura)
	PLAYER = 0,
	ENEMY
};

enum Delay_Vector
{											// Tipul de delay(data) din vector (folosit in structura)
	DELAY_SHIELD_DURATION = 0,
	DELAY_SHIELD_FLICK,
	DELAY_LASER,
	DELAY_ROCKET
};

enum Sprite_Types
{											// Tipul de data din matrice (folosit in structura)
	SPRITE_SPACESHIP = 0,
	SPRITE_SHIELD,
	SPRITE_EXPLOSION
};

struct Spaceship
{											// Structura pentru nave
	bool SS_Alive;											// Statusul navei
	double SS_Angle;										// Unghiul navei
	SpaceShip_Types SS_Type;								// Tipul navei din (SpaceShip_Types)
	int SS_Origin[3],										// Originea navei din (Positions)
		SS_Sprite[3][2],									// Sprite pentru nava (Sprite_Types), (Sprite_Vector)
		SS_Delay[4],										// Delay pentru facilitati ale navei (Delay_Vector)
		SS_RocketsAmmo,										// Numarul de rachete
		SS_Speed;											// Viteza navei
	Projectile SS_Projectile[MAX_PROJECTILES];				// Variabila de tip Projectile ce contine proiectilele navei
} SS_Spacehips[MAX_SPACESHIPS];								// Vectorul care tine minte toate navele

//--| Entitati |--//
enum Entity_Types
{											// Tipul de entitati (folosit in structura)
	ENT_REWARD_RED,
	ENT_REWARD_BLUE,
	ENT_ASTEROID
};

struct Entity
{
	bool ENT_Active;										// Statusul asteroidului
	double ENT_Angle;										// Unghiul asteroidului
	Entity_Types ENT_Type;									// Tipul asteroidului
	int ENT_Origin[3],										// Originea asteroidului din (Positions)
		ENT_Sprite[2],										// Sprite pentru asteroid (Sprite_Vector)
		ENT_Speed;											// Viteza asteroidului
}; Entity ENT_Entity[MAX_ASTEROIDS + MAX_REWARDS];			// Vectorul care tine minte toate entitatile

//--| Variabile generale |--//
enum Positions
{											// Enumera pozitila, originea (Axele X, Y, Z)
	X = 0,													// Pozitia X
	Y,														// Pozitia Y
	Z														// Pozitia Z
};

//--| Fereastra |--//
SDL_Window* Window;											// Fereastra
SDL_Renderer* Render;										// Render
int ORIGIN_Y_TOP = 0,										// Y1
ORIGIN_Y_CENTER = int(Resolution[GameResolution][Y] / 2),	// Y0 - Center
ORIGIN_Y_BUTTOM = Resolution[GameResolution][Y],			// Y2
ORIGIN_X_LEFT = 0,											// X1
ORIGIN_X_CENTER = int(Resolution[GameResolution][X] / 2),	// X0 - Center
ORIGIN_X_RIGHT = Resolution[GameResolution][X];				// X2

//--| Game |--//
double DeltaTime;											// Diferenta dintre timpul actual, si ultima data cand sa executat while-ul.
bool IsRunning = true,										// Folosit in while, verifica daca jocul ruleaza
GameStarted = false,										// Folosit in while, verifica daca jocul ruleaza
PauseGame = false,											// Pauza joc
GameOver = false;											// Este evident ce face

//--| Meniu |--//
int SelectedItem;
bool InMenu = true,											// Verifica daca jocatorul se afla in meniu		
InOptions = false,											// Verifica daca jucatorul este in meniul de optiuni
InScoreboard = false,										// Verifica daca jucatorul este in scoreboard
InResolution = false,										// Verifica daca jucatorul este in optiunea de rezolutie
InMusicVolume = false,										// Verifica daca jucatorul este in optiunea de volum muzica
InSoundsVolume = false,										// Verifica daca jucatorul este in optiunea de volum sunete
P1_MENU_ENTER = false;

//--| Player |--//
int PlayerLives, PlayerScore;								// Tine minte vietile si scorul jucatorului
bool P1_C_FORWARD, P1_C_BACK,								// Fata, spate
P1_C_TURN_LEFT, P1_C_TURN_RIGHT,							// Rotore stanga, dreapta
P1_C_MOVE_LEFT, P1_C_MOVE_RIGHT,							// Inclinare(Miscare) stanga, dreapta
P1_C_UP, P1_C_DOWN,											// Urcare, coborarea layer
P1_C_STABILIZE,												// Stabilizeaza nava in pozitia originala
P1_C_FIRE_LASER, P1_C_FIRE_ROCKET;							// Trage laser, racheta

//--| Imagini |--//
int ORIGIN_X_HUD = ORIGIN_X_LEFT,							// Originea X a hud-ului
ORIGIN_Y_HUD = ORIGIN_Y_TOP;								// Originea Y a hud-ului

//--| Sunete |--//
Mix_Music* Sound_Ambience = NULL;							// Sunetul ambiance
int MusicVolume = 128, SoundsVolume = 15;

//--| Functii |--//
// Initializare
bool Game_Init();											// Incarca fereastra, navele, imaginile si sunetele
bool LoadImages();											// Incarca imaginile folosite de joc
bool LoadSounds();											// Incarca sunetele folosite de joc
bool LoadFonts();											// Incarca font-urile folosite de joc
void FreeAndQuit();											// Elibereaza memoria si iasa

// GamePlay
void G_Events(int MENU_ITEMS = 0);							// Verifica input de la tastatura
void G_Controls(int CurrentTime);							// Actioneaza in functie de input
void G_IAEnemy(int CurrentTime);							// Inteligenta Artificiala a navelor inamice
void G_Draw(int CurrentTime);								// Afisarea navelor/proiectilelor
void G_Collision(int CurrentTime);							// Coliziunea dintre obiecte

// SDL
void ShowImage(ImagesList ImageID, SDL_Rect DstRect);										// Afisaza o imagine pe ecran
void ShowSprite(ImagesList ImageID, SDL_Rect SrcRect, SDL_Rect DstRect, double Angle);		// Folosit pentru a afisa un sprite pe ecran
bool SetText(FontsList FontID, char Mesaj[], double Scale);									// Modifica un text pentru afisare
void ShowText(FontsList FontID, SDL_Rect DstRect);											// Afisaza unui Text pe ecran
void PlaySound(SoundsList SoundID);															// Porneste un sunet

// Altele
void ShowMenu();																			// Modifica fereastra si tot ce tine de rezolutie
void ResizeWindow(int ResizeWindow);														// Modifica fereastra si tot ce tine de rezolutie
void ADD_Laser(int SpaceShipID, int LaserID, int Speed);									// Adauga un laser pe harta
void ADD_Rocket(int SpaceShipID, int RocketID, int Speed);									// Adauga o racheta pe harta
void ADD_Reward(int SpaceShipID, int RewardID);												// Adauga un reward pe harta (Entitate)
void ADD_Asteroid(int AsteroidID);															// Adauga un asteroid pe harta
void Init_Explosion(int SpaceShipID);														// Distruge si initializeaza explozia unei nave
bool CollisionCircle(float X1, float Y1, float R1, float X2, float Y2, float R2);			// Verifica daca 2 cercuri au coliziune
int SpaceshipScale(int Number, int OriginZ);												// Calculeaza marimea navei, in functie de Z, creste sau scade cu 1%
int clamp(int value, int minValue, int maxValue);											// Limiteaza un numar intre minim si maxim (int)


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main |
//==========================================================================================================

int main(int argv, char* args[])
{
	//--| Initializeaza jocul |--//
	if (!Game_Init())
		return -1;

	//--| Variabile |--//
	int CurrentTime = 0, PrevTime = 0,								// Tine minte timpul
		Intro_Delay = 0, StartGame_Delay = 0, GameOver_Delay = 0,	// Delay pentru intro, pornire si sfarsit
		Background_Pos = 0;											// Tine minte pozitia fundalului

	PlayerLives = clamp(PLAYER_LIFE, 0, 7);							// Seteaza vietile jucatorului

	//--| The game itself |--//
	do
	{
		SDL_RenderClear(Render);									// Clear la screen

		//--| Memoreaza timpii |--//
		PrevTime = CurrentTime;										// Timpul precedent
		CurrentTime = SDL_GetTicks();								// Timpul curent
		DeltaTime = (CurrentTime - PrevTime) / 1000.0f;				// Diferenta dintre prev time si timpul curent

		//--| Afisare intro |--//
		if (Intro_Delay == 0)
			Intro_Delay = CurrentTime + INTRO_DELAY * 1000;
		else if (Intro_Delay > CurrentTime)
			ShowImage(IMG_INTRO, { 0, 0, IMG_Images[IMG_INTRO].IMG_ScaledSize[X], IMG_Images[IMG_INTRO].IMG_ScaledSize[Y] });
		else
		{
			//--| Afisare meniu |--//
			if (InMenu)
			{
				if (StartGame_Delay > CurrentTime)
					StartGame_Delay = int(StartGame_Delay + DeltaTime);

				ShowMenu();
			}
			else
			{
				//--| Seteaza delay-ul pentru pornirea jocului |--//
				if (StartGame_Delay == 0)
					StartGame_Delay = CurrentTime + STARTGAME_DELAY * 1000;

				//--| Verifica input |--//
				G_Events();

				//--| Verifica daca jocul este in pauza |--//
				if (PauseGame)
				{
					SDL_Delay((Uint32)DeltaTime);
					continue;
				}
				//--| Verifica daca jocul s-a terminat |--//
				else if (GameOver)
				{
					if (!GameOver_Delay)
						GameOver_Delay = CurrentTime + GAMEOVER_DELAY * 1000;

					if (GameOver_Delay < CurrentTime)
					{
						InMenu = true;
						break;
					}
				}

				//--| Sunet background |--//
				if (Mix_PlayingMusic() == 0)
					Mix_PlayMusic(Sound_Ambience, 1);

				//--| Afisare fundal (Loop) |--//
				if (Background_Pos > 0)
					ShowImage(IMG_BACKGROUND,
							  { 0, -IMG_Images[IMG_BACKGROUND].IMG_ScaledSize[Y] + Background_Pos, IMG_Images[IMG_BACKGROUND].IMG_ScaledSize[X], IMG_Images[IMG_BACKGROUND].IMG_ScaledSize[Y] });

				// Acopera partea goala a ecranului cand imaginea de fundal ajunge la capat
				ShowImage(IMG_BACKGROUND, { 0, Background_Pos, IMG_Images[IMG_BACKGROUND].IMG_ScaledSize[X], IMG_Images[IMG_BACKGROUND].IMG_ScaledSize[Y] });
				Background_Pos = Background_Pos >= Resolution[GameResolution][Y] ? (-IMG_Images[IMG_BACKGROUND].IMG_ScaledSize[Y] + Resolution[GameResolution][Y]) : Background_Pos + 1;

				//--| Functii |--//
				G_Draw(CurrentTime);								// Deseneaza nave/proiectile etc
				G_Controls(CurrentTime);							// Verifica daca exista input

				if (StartGame_Delay < CurrentTime)
				{
					GameStarted = true;								// Seteaza jocul ca find pornit

					G_IAEnemy(CurrentTime);							// Inteligenta navelor inamice
					G_Collision(CurrentTime);						// Detecteaza coliziunile dintre obiecte (Nave, proiectile, entitati)
				}
				else
					//--| Afisare logo |--//
					ShowImage(IMG_LOGO, { ORIGIN_X_CENTER - int(IMG_Images[IMG_LOGO].IMG_ScaledSize[X] / 2), ORIGIN_Y_CENTER - int(IMG_Images[IMG_LOGO].IMG_ScaledSize[Y] / 2), IMG_Images[IMG_LOGO].IMG_ScaledSize[X], IMG_Images[IMG_LOGO].IMG_ScaledSize[Y] });
			}
		}

		SDL_RenderPresent(Render);									// Da update la screen
	} while (IsRunning);

	//--| Eliberare memorie etc |--//
	FreeAndQuit();

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// G_Events |
//==========================================================================================================

void G_Events(int MENU_ITEMS)
{
	SDL_Event Event;

	if (InMenu)
	{
		while (SDL_PollEvent(&Event))
		{
			switch (Event.type)
			{
				case SDL_QUIT:
				{
					InMenu = false;
					IsRunning = false;
					break;
				}
				case SDL_KEYDOWN:
				{
					switch (Event.key.keysym.sym)
					{
						case CONTROL_MENU_EXIT:
						{
							if (InScoreboard)
								InScoreboard = false;
							else if (InResolution)
								InResolution = false;
							else if (InMusicVolume)
								InMusicVolume = false;
							else if (InSoundsVolume)
								InSoundsVolume = false;
							else if (InOptions)
							{
								SelectedItem = 0;
								InOptions = false;
							}
							else if (GameStarted && !PauseGame)
							{
								Mix_ResumeMusic();	// Resume la ambiance
								Mix_Resume(-1);		// Resume la sunete
								InMenu = false;
							}
							break;
						}
						case CONTROL_MENU_UP:
						{
							if (MENU_ITEMS)
							{
								if (InResolution)
								{
									int PrevRez = GameResolution;
									GameResolution = clamp(GameResolution - 1, 0, MAX_RESOLUTIONS - 1);

									if (PrevRez != GameResolution)
										ResizeWindow(Resolution[PrevRez][X] + Resolution[PrevRez][Y]);
								}
								else if (InMusicVolume)
								{
									int PrevMusicVol = MusicVolume;
									MusicVolume = clamp(MusicVolume + 1, 0, 128);

									if (PrevMusicVol != MusicVolume)
										Mix_VolumeMusic(MusicVolume);
								}
								else if (InSoundsVolume)
								{
									int PrevSoundsVol = SoundsVolume;
									SoundsVolume = clamp(SoundsVolume + 1, 0, 128);

									if (PrevSoundsVol != SoundsVolume)
										for (int SoundID = 0; SoundID < MAX_SOUNDS; SoundID++)
											Mix_VolumeChunk(SND_Sounds[SoundID].SND_Chunk, SoundsVolume);
								}
								else
									SelectedItem = clamp(SelectedItem - 1, 0, MENU_ITEMS - 1);
							}
							break;
						}
						case CONTROL_MENU_DOWN:
						{
							if (MENU_ITEMS)
							{
								if (InResolution)
								{
									int PrevRez = GameResolution;
									GameResolution = clamp(GameResolution + 1, 0, MAX_RESOLUTIONS - 1);

									if (PrevRez != GameResolution)
										ResizeWindow(Resolution[PrevRez][X] + Resolution[PrevRez][Y]);
								}
								else if (InMusicVolume)
								{
									int PrevMusicVol = MusicVolume;
									MusicVolume = clamp(MusicVolume - 1, 0, 128);

									if (PrevMusicVol != MusicVolume)
										Mix_VolumeMusic(MusicVolume);
								}
								else if (InSoundsVolume)
								{
									int PrevSoundsVol = SoundsVolume;
									SoundsVolume = clamp(SoundsVolume - 1, 0, 128);

									if (PrevSoundsVol != SoundsVolume)
										for (int SoundID = 0; SoundID < MAX_SOUNDS; SoundID++)
											Mix_VolumeChunk(SND_Sounds[SoundID].SND_Chunk, SoundsVolume);
								}
								else
									SelectedItem = clamp(SelectedItem + 1, 0, MENU_ITEMS - 1);
							}
							break;
						}
						case CONTROL_MENU_ENTER:
						{
							if (P1_MENU_ENTER)
								break;

							if (InOptions)
							{
								switch (SelectedItem)
								{
									case 0: InResolution = !InResolution; break;
									case 1: InMusicVolume = !InMusicVolume; break;
									case 2: InSoundsVolume = !InSoundsVolume; break;
									default: break;
								}
							}
							else
							{
								switch (SelectedItem)
								{
									case 0: InMenu = false; break;
									case 1: InScoreboard = !InScoreboard; break;
									case 2:
									{
										SelectedItem = 0;
										InOptions = true;
										break;
									}
									case 3:
									{
										InMenu = false;
										IsRunning = false;
										break;
									}
									default: break;
								}
							}
							P1_MENU_ENTER = true;
							break;
						}
						default: break;
					}
					break;
				}
				case SDL_KEYUP:
				{
					if (Event.key.keysym.sym == CONTROL_MENU_ENTER)
						P1_MENU_ENTER = false;
				}
				default: break;
			}
		}
	}
	else
	{
		while (SDL_PollEvent(&Event))
		{
			switch (Event.type)
			{
				case SDL_QUIT:
				{
					IsRunning = false;
					InMenu = false;
					break;
				}
				case SDL_KEYDOWN:
				{
					switch (Event.key.keysym.sym)
					{
						case CONTROL_MENU:
						{
							Mix_PauseMusic();	// Pauza la ambiance
							Mix_Pause(-1);		// Pauza la sunete

							SelectedItem = 0;
							InMenu = true;
							P1_MENU_ENTER = false;
							break;
						}
						case CONTROL_PAUSE:
						{
							PauseGame = !PauseGame;

							if (PauseGame)
							{
								SDL_Delay((Uint32)DeltaTime);
								Mix_PauseMusic();	// Pauza la ambiance
								Mix_Pause(-1);		// Pauza la sunete
							}
							else
							{
								Mix_ResumeMusic();	// Resume la ambiance
								Mix_Resume(-1);		// Resume la sunete
							}
							break;
						}
						case CONTROL_FORWARD: P1_C_FORWARD = true; break;
						case CONTROL_BACK: P1_C_BACK = true; break;
						case CONTROL_LEFT: P1_C_MOVE_LEFT = true; break;
						case CONTROL_RIGHT: P1_C_MOVE_RIGHT = true; break;
						case CONTROL_UP: P1_C_UP = true; break;
						case CONTROL_DOWN: P1_C_DOWN = true; break;
						case CONTROL_TURN_LEFT: P1_C_TURN_LEFT = true; break;
						case CONTROL_TURN_RIGHT: P1_C_TURN_RIGHT = true; break;
						case CONTROL_STABILIZE: P1_C_STABILIZE = true; break;
						case CONTROL_FIRE_LASERS: P1_C_FIRE_LASER = true; break;
						case CONTROL_FIRE_ROCKETS: P1_C_FIRE_ROCKET = true; break;
						default: break;
					}
					break;
				}
				case SDL_KEYUP:
				{
					switch (Event.key.keysym.sym)
					{
						case CONTROL_FORWARD: P1_C_FORWARD = false; break;
						case CONTROL_BACK: P1_C_BACK = false; break;
						case CONTROL_LEFT: P1_C_MOVE_LEFT = false; break;
						case CONTROL_RIGHT: P1_C_MOVE_RIGHT = false; break;
						case CONTROL_UP: P1_C_UP = false; break;
						case CONTROL_DOWN: P1_C_DOWN = false; break;
						case CONTROL_TURN_LEFT: P1_C_TURN_LEFT = false; break;
						case CONTROL_TURN_RIGHT: P1_C_TURN_RIGHT = false; break;
						case CONTROL_FIRE_LASERS: P1_C_FIRE_LASER = false; break;
						case CONTROL_FIRE_ROCKETS: P1_C_FIRE_ROCKET = false; break;
						default: break;
					}
					break;
				}
				default: break;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Player G_Controls |
//==========================================================================================================

void G_Controls(int CurrentTime)
{
	//--| Verifica daca jucatorul este in viata pentru a continua |--//
	if (!SS_Spacehips[PLAYER].SS_Alive)
		return;

	// Muta nava in fata
	if (P1_C_FORWARD)
	{
		// Calculeaza directia de mutare
		int NewX = int(cos(SS_Spacehips[PLAYER].SS_Angle * DEGREE) * DeltaTime * SS_Spacehips[PLAYER].SS_Speed),
			NewY = int(sin(SS_Spacehips[PLAYER].SS_Angle * DEGREE) * DeltaTime * SS_Spacehips[PLAYER].SS_Speed);

		// Verifica daca putem muta nava
		if (SS_Spacehips[PLAYER].SS_Origin[X] + NewX > 0 && SS_Spacehips[PLAYER].SS_Origin[Y] + NewY > 0 && SS_Spacehips[PLAYER].SS_Origin[X] + NewX < (Resolution[GameResolution][X] - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Origin[Z])) && SS_Spacehips[PLAYER].SS_Origin[Y] + NewY < (Resolution[GameResolution][Y] - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Origin[Z])))
		{
			SS_Spacehips[PLAYER].SS_Origin[X] += NewX;
			SS_Spacehips[PLAYER].SS_Origin[Y] += NewY;
		}
	}
	// Muta nava in spate
	else if (P1_C_BACK)
	{
		// Calculeaza directia de mutare
		int NewX = int(cos(SS_Spacehips[PLAYER].SS_Angle * DEGREE) * DeltaTime * SS_Spacehips[PLAYER].SS_Speed),
			NewY = int(sin(SS_Spacehips[PLAYER].SS_Angle * DEGREE) * DeltaTime * SS_Spacehips[PLAYER].SS_Speed);

		// Verifica daca putem muta nava
		if (SS_Spacehips[PLAYER].SS_Origin[X] - NewX > 0 && SS_Spacehips[PLAYER].SS_Origin[Y] - NewY > 0 && SS_Spacehips[PLAYER].SS_Origin[X] - NewX < (Resolution[GameResolution][X] - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Origin[Z])) && SS_Spacehips[PLAYER].SS_Origin[Y] - NewY < (Resolution[GameResolution][Y] - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Origin[Z])))
		{
			SS_Spacehips[PLAYER].SS_Origin[X] -= NewX;
			SS_Spacehips[PLAYER].SS_Origin[Y] -= NewY;
		}
	}
	// Muta nava in stanga
	if (P1_C_MOVE_LEFT)
	{
		// Calculeaza directia de mutare
		int NewX = int(cos((SS_Spacehips[PLAYER].SS_Angle - 90) * DEGREE) * DeltaTime * SS_Spacehips[PLAYER].SS_Speed),
			NewY = int(sin((SS_Spacehips[PLAYER].SS_Angle - 90) * DEGREE) * DeltaTime * SS_Spacehips[PLAYER].SS_Speed);

		// Verifica daca putem muta nava
		if (SS_Spacehips[PLAYER].SS_Origin[X] + NewX > 0 && SS_Spacehips[PLAYER].SS_Origin[Y] + NewY > 0 && SS_Spacehips[PLAYER].SS_Origin[X] + NewX < (Resolution[GameResolution][X] - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Origin[Z])) && SS_Spacehips[PLAYER].SS_Origin[Y] + NewY < (Resolution[GameResolution][Y] - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Origin[Z])))
		{
			SS_Spacehips[PLAYER].SS_Origin[X] += NewX;
			SS_Spacehips[PLAYER].SS_Origin[Y] += NewY;
		}
	}
	// Muta nava in dreapta
	else if (P1_C_MOVE_RIGHT)
	{
		// Calculeaza directia de mutare
		int NewX = int(cos((SS_Spacehips[PLAYER].SS_Angle + 90) * DEGREE) * DeltaTime * SS_Spacehips[PLAYER].SS_Speed),
			NewY = int(sin((SS_Spacehips[PLAYER].SS_Angle + 90) * DEGREE) * DeltaTime * SS_Spacehips[PLAYER].SS_Speed);

		// Verifica daca putem muta nava
		if (SS_Spacehips[PLAYER].SS_Origin[X] + NewX > 0 && SS_Spacehips[PLAYER].SS_Origin[Y] + NewY > 0 && SS_Spacehips[PLAYER].SS_Origin[X] + NewX < (Resolution[GameResolution][X] - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Origin[Z])) && SS_Spacehips[PLAYER].SS_Origin[Y] + NewY < (Resolution[GameResolution][Y] - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Origin[Z])))
		{
			SS_Spacehips[PLAYER].SS_Origin[X] += NewX;
			SS_Spacehips[PLAYER].SS_Origin[Y] += NewY;
		}
	}

	// Intoarce nava in pozitia initiala
	if (P1_C_STABILIZE)
	{
		double NewAngle = SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE < 180 ? SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE - double(DeltaTime * (SS_Spacehips[PLAYER].SS_Speed * 0.4)) <= 0 ? 0 : SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE - double(DeltaTime * (SS_Spacehips[PLAYER].SS_Speed * 0.4)) : SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE + double(DeltaTime * (SS_Spacehips[PLAYER].SS_Speed * 0.4)) >= 360 ? 0 : SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE + double(DeltaTime * (SS_Spacehips[PLAYER].SS_Speed * 0.4));
		NewAngle = double(360 * (NewAngle / 360));
		SS_Spacehips[PLAYER].SS_Angle = NewAngle + PLAYER_DEFAULT_ANGLE;

		if (SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE == 0)
			P1_C_STABILIZE = false;
	}
	// Roteste nava spre stanga
	else if (P1_C_TURN_LEFT)
	{
		double NewAngle = SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE <= 0 ? 360 : SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE - DeltaTime * (SS_Spacehips[PLAYER].SS_Speed * 0.2);
		NewAngle = double(360 * (NewAngle / 360));
		SS_Spacehips[PLAYER].SS_Angle = NewAngle + PLAYER_DEFAULT_ANGLE;
	}
	// Roteste nava spre dreapta
	else if (P1_C_TURN_RIGHT)
	{
		double NewAngle = SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE >= 360 ? 0 : SS_Spacehips[PLAYER].SS_Angle - PLAYER_DEFAULT_ANGLE + DeltaTime * (SS_Spacehips[PLAYER].SS_Speed * 0.20);
		NewAngle = double(360 * (NewAngle / 360));
		SS_Spacehips[PLAYER].SS_Angle = NewAngle + PLAYER_DEFAULT_ANGLE;
	}

	// Coboara nava 1 layer (Axa Z)
	if (P1_C_DOWN)
	{
		SS_Spacehips[PLAYER].SS_Origin[Z] = clamp(SS_Spacehips[PLAYER].SS_Origin[Z] - 1, 0, MAX_Z);
		P1_C_DOWN = false;
	}
	// Urca nava 1 layer (Axa Z)
	else if (P1_C_UP)
	{
		SS_Spacehips[PLAYER].SS_Origin[Z] = clamp(SS_Spacehips[PLAYER].SS_Origin[Z] + 1, 0, MAX_Z);
		P1_C_UP = false;
	}

	//--| Trage cu laser |--//
	if (P1_C_FIRE_LASER)
	{
		//--| Verifica daca nava poate trage, apoi gaseste un proiectil dezactivat si il foloseste |--//
		if (SS_Spacehips[PLAYER].SS_Delay[DELAY_LASER] < CurrentTime)
		{
			for (int i = 0; i < MAX_PROJECTILES; i++)
			{
				if (!SS_Spacehips[PLAYER].SS_Projectile[i].P_Active)
				{
					SS_Spacehips[PLAYER].SS_Delay[DELAY_LASER] = int(CurrentTime + (PLAYER_DELAY_LASER * 1000));
					ADD_Laser(PLAYER, i, PLAYER_LASER_SPEED);
					break;
				}
			}
		}
	}

	//--| Trage cu racheta |--//
	if (P1_C_FIRE_ROCKET)
	{
		//--| Verifica daca nava poate trage, apoi gaseste un proiectil dezactivat si il foloseste |--//
		if (SS_Spacehips[PLAYER].SS_Delay[DELAY_ROCKET] < CurrentTime && SS_Spacehips[PLAYER].SS_RocketsAmmo >= 1)
		{
			for (int i = 0; i < MAX_PROJECTILES; i++)
			{
				if (!SS_Spacehips[PLAYER].SS_Projectile[i].P_Active)
				{
					SS_Spacehips[PLAYER].SS_Delay[DELAY_ROCKET] = int(CurrentTime + (PLAYER_DELAY_ROCKET * 1000));
					ADD_Rocket(PLAYER, i, PLAYER_ROCKET_SPEED);
					break;
				}
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// G_Draw Spaceships |
//==========================================================================================================

void G_Draw(int CurrentTime)
{
	int i = 0, j = 0, k = 0;
	char Text[128];
	ImagesList ImageID;
	FontsList FontID;

	if (GameOver)
	{
		//--| Afisare Scor (Game Over) |--//
		sprintf_s(Text, sizeof(Text), "SCORE %d", PlayerScore);

		if (SetText(FNT_SCORE, Text, atof(DEFAULT_SCALE)))
			ShowText(FNT_SCORE, { ORIGIN_X_CENTER - FNT_Fonts[FNT_SCORE].FNT_Size[X] / 2, ORIGIN_Y_CENTER - FNT_Fonts[FNT_SCORE].FNT_Size[Y] / 2, FNT_Fonts[FNT_SCORE].FNT_Size[X], FNT_Fonts[FNT_SCORE].FNT_Size[Y] });

	}
	else
	{
		//--| Afisare Scor |--//
		sprintf_s(Text, sizeof(Text), "[%d]", PlayerScore);

		if (SetText(FNT_SCORE, Text, atof(DEFAULT_SCALE)))
			ShowText(FNT_SCORE, { ORIGIN_X_RIGHT - FNT_Fonts[FNT_SCORE].FNT_Size[X], ORIGIN_Y_TOP, FNT_Fonts[FNT_SCORE].FNT_Size[X], FNT_Fonts[FNT_SCORE].FNT_Size[Y] });

		//--| Afisare Hud-ul |--//
		ShowSprite(IMG_PLAYER_HUD,
				   { (IMG_Images[IMG_PLAYER_HUD].IMG_Size[X] * PlayerLives - 1), 0, IMG_Images[IMG_PLAYER_HUD].IMG_Size[X], IMG_Images[IMG_PLAYER_HUD].IMG_Size[Y] },
				   { ORIGIN_X_HUD, ORIGIN_Y_HUD, IMG_Images[IMG_PLAYER_HUD].IMG_ScaledSize[X], IMG_Images[IMG_PLAYER_HUD].IMG_ScaledSize[Y] }, 0);
	}

	//--| Afisare Nave, Proiectile, Explozie |--//
	for (i = 0; i <= MAX_Z; i++)
	{	// Axele Z
		for (j = 0; j < MAX_SPACESHIPS; j++)
		{	// Nave
			for (k = 0; k < MAX_PROJECTILES; k++)
			{	// Proiectile
// Daca Proiectilul nu corespunde layer-ului respectiv sau este dezactivat, trece la urmatorul
				if (SS_Spacehips[j].SS_Projectile[k].P_Origin[Z] != i || !SS_Spacehips[j].SS_Projectile[k].P_Active)
					continue;

				//--| Retine ID-ul imaginii |--//
				ImageID = SS_Spacehips[j].SS_Type == PLAYER ? (SS_Spacehips[j].SS_Projectile[k].P_Type == LASER_PLAYER ? IMG_PLAYER_LASER : IMG_PLAYER_ROCKET) : (SS_Spacehips[j].SS_Projectile[k].P_Type == LASER_ENEMY ? IMG_ENEMY_LASER : IMG_ENEMY_ROCKET);

				//--| Verifica daca proiectilul este iesit afara din harta si-l dezactiveaza |--//
				if (SS_Spacehips[j].SS_Projectile[k].P_Origin[X] - SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[X], i) > Resolution[GameResolution][X] || SS_Spacehips[j].SS_Projectile[k].P_Origin[X] + SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[X], i) < 0
					|| SS_Spacehips[j].SS_Projectile[k].P_Origin[Y] - SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[Y], i) > Resolution[GameResolution][Y] || SS_Spacehips[j].SS_Projectile[k].P_Origin[Y] + SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[Y], i) < 0)
					SS_Spacehips[j].SS_Projectile[k].P_Active = false;
				else
				{
					// Calculeaza si misca proiectilul in unghiul de tragere
					SS_Spacehips[j].SS_Projectile[k].P_Origin[X] += int(cos(SS_Spacehips[j].SS_Projectile[k].P_Angle * DEGREE) * (DeltaTime * SS_Spacehips[j].SS_Projectile[k].P_Speed));
					SS_Spacehips[j].SS_Projectile[k].P_Origin[Y] += int(sin(SS_Spacehips[j].SS_Projectile[k].P_Angle * DEGREE) * (DeltaTime * SS_Spacehips[j].SS_Projectile[k].P_Speed));

					//--| Afisaza proiectil |--//
					ShowSprite(ImageID,
							   { 0, (IMG_Images[ImageID].IMG_Size[Y] * SS_Spacehips[j].SS_Projectile[k].P_Sprite[SPRITE_POS]), IMG_Images[ImageID].IMG_Size[X], IMG_Images[ImageID].IMG_Size[Y] },
							   { SS_Spacehips[j].SS_Projectile[k].P_Origin[X], SS_Spacehips[j].SS_Projectile[k].P_Origin[Y], SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[X], i), SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[Y], i) }, SS_Spacehips[j].SS_Projectile[k].P_Angle);

					//--| Count pentru animatie |--//
					if (SS_Spacehips[j].SS_Projectile[k].P_Sprite[SPRITE_DELAY] < CurrentTime)
					{
						SS_Spacehips[j].SS_Projectile[k].P_Sprite[SPRITE_DELAY] = CurrentTime + 50;
						SS_Spacehips[j].SS_Projectile[k].P_Sprite[SPRITE_POS] = SS_Spacehips[j].SS_Projectile[k].P_Sprite[SPRITE_POS] >= (IMG_Images[ImageID].IMG_Sprite - 1) ? 0 : SS_Spacehips[j].SS_Projectile[k].P_Sprite[SPRITE_POS] + 1;
					}
				}
			}

			// Daca Nava nu corespunde layer-ului, trece la urmatoarea
			if (SS_Spacehips[j].SS_Origin[Z] != i)
				continue;

			//--| Verifica daca nava este sau nu distrusa |--//
			if (SS_Spacehips[j].SS_Alive)
			{
				//--| Retine ID-ul imaginii |--//
				ImageID = SS_Spacehips[j].SS_Type == PLAYER ? ((SS_Spacehips[j].SS_Delay[DELAY_ROCKET] > CurrentTime || SS_Spacehips[j].SS_RocketsAmmo < 1) ? IMG_PLAYER_SPACESHIP_NO_ROCKETS : IMG_PLAYER_SPACESHIP_NORMAL) : ((SS_Spacehips[j].SS_Delay[DELAY_ROCKET] > CurrentTime || SS_Spacehips[j].SS_RocketsAmmo < 1) ? IMG_ENEMY_SPACESHIP_NO_ROCKETS : IMG_ENEMY_SPACESHIP_NORMAL);

				//--| Afisaza nava |--//
				ShowSprite(ImageID,
						   { 0, IMG_Images[ImageID].IMG_Size[Y] * SS_Spacehips[j].SS_Sprite[SPRITE_SPACESHIP][SPRITE_POS], IMG_Images[ImageID].IMG_Size[X], IMG_Images[ImageID].IMG_Size[Y] },
						   { SS_Spacehips[j].SS_Origin[X], SS_Spacehips[j].SS_Origin[Y], SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[X], i), SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[Y], i) }, SS_Spacehips[j].SS_Angle);

				//--| Count pentru animatie |--//
				if (SS_Spacehips[j].SS_Sprite[SPRITE_SPACESHIP][SPRITE_DELAY] < CurrentTime)
				{
					SS_Spacehips[j].SS_Sprite[SPRITE_SPACESHIP][SPRITE_DELAY] = CurrentTime + 50;
					SS_Spacehips[j].SS_Sprite[SPRITE_SPACESHIP][SPRITE_POS] = SS_Spacehips[j].SS_Sprite[SPRITE_SPACESHIP][SPRITE_POS] >= (IMG_Images[ImageID].IMG_Sprite - 1) ? 0 : SS_Spacehips[j].SS_Sprite[SPRITE_SPACESHIP][SPRITE_POS] + 1;
				}

				//--| Daca este in GodMode, seteaza un delay pentru efect |--//
				if (SS_Spacehips[j].SS_Delay[DELAY_SHIELD_DURATION] > CurrentTime)
				{
					//--| Afisaza scut |--//
					ShowSprite(IMG_PLAYER_SHIELD,
							   { 0, IMG_Images[IMG_PLAYER_SHIELD].IMG_Size[Y] * SS_Spacehips[j].SS_Sprite[SPRITE_SHIELD][SPRITE_POS], IMG_Images[IMG_PLAYER_SHIELD].IMG_Size[X], IMG_Images[IMG_PLAYER_SHIELD].IMG_Size[Y] },
							   { SS_Spacehips[j].SS_Origin[X], SS_Spacehips[j].SS_Origin[Y], SpaceshipScale(IMG_Images[IMG_PLAYER_SHIELD].IMG_ScaledSize[X], i), SpaceshipScale(IMG_Images[IMG_PLAYER_SHIELD].IMG_ScaledSize[Y], i) }, SS_Spacehips[j].SS_Angle);

					//--| Count pentru animatie |--//
					if (SS_Spacehips[j].SS_Sprite[SPRITE_SHIELD][SPRITE_DELAY] < CurrentTime)
					{
						SS_Spacehips[j].SS_Sprite[SPRITE_SHIELD][SPRITE_DELAY] = CurrentTime + 50;
						SS_Spacehips[j].SS_Sprite[SPRITE_SHIELD][SPRITE_POS] = SS_Spacehips[j].SS_Sprite[SPRITE_SHIELD][SPRITE_POS] >= (IMG_Images[IMG_PLAYER_SHIELD].IMG_Sprite - 1) ? 0 : SS_Spacehips[j].SS_Sprite[SPRITE_SHIELD][SPRITE_POS] + 1;
					}

				}

				//--| Afisaza axa Z |--//
				sprintf_s(Text, sizeof(Text), "%d", i + 1);
				FontID = SS_Spacehips[j].SS_Type == PLAYER ? FNT_PLAYER_ALTITUDE : FNT_ENEMY_ALTITUDE;

				if (SetText(FontID, Text, atof(SPACESHIP_SCALE)))
					ShowText(FontID, { SS_Spacehips[j].SS_Origin[X], SS_Spacehips[j].SS_Origin[Y], SpaceshipScale(FNT_Fonts[FontID].FNT_Size[X], i), SpaceshipScale(FNT_Fonts[FontID].FNT_Size[Y], i) });
			}
			//--| Verifica animatia de explozie |--//
			else
			{
				if (SS_Spacehips[j].SS_Sprite[SPRITE_EXPLOSION][SPRITE_POS] <= (IMG_Images[IMG_EXPLOSION].IMG_Sprite - 1))
				{
					//--| Afisaza explozia |--//
					ShowSprite(IMG_EXPLOSION,
							   { 0, IMG_Images[IMG_EXPLOSION].IMG_Size[Y] * SS_Spacehips[j].SS_Sprite[SPRITE_EXPLOSION][SPRITE_POS], IMG_Images[IMG_EXPLOSION].IMG_Size[X], IMG_Images[IMG_EXPLOSION].IMG_Size[Y] },
							   { SS_Spacehips[j].SS_Origin[X], SS_Spacehips[j].SS_Origin[Y], SpaceshipScale(IMG_Images[IMG_EXPLOSION].IMG_ScaledSize[X], i), SpaceshipScale(IMG_Images[IMG_EXPLOSION].IMG_ScaledSize[Y], i) }, SS_Spacehips[j].SS_Angle);

					//--| Count pentru animatie |--//
					if (SS_Spacehips[j].SS_Sprite[SPRITE_EXPLOSION][SPRITE_DELAY] < CurrentTime)
					{
						SS_Spacehips[j].SS_Sprite[SPRITE_EXPLOSION][SPRITE_DELAY] = CurrentTime + 30;
						SS_Spacehips[j].SS_Sprite[SPRITE_EXPLOSION][SPRITE_POS]++;
					}
				}
				//--| Respawn la nava dupa ce animatia de explozie se termina |--//
				else
				{
					if (PlayerLives && j == PLAYER || j != PLAYER)
					{
						SS_Spacehips[j].SS_Alive = true;

						//--| Respawn Player |--//
						if (j == PLAYER)
						{
							SS_Spacehips[j].SS_Origin[X] = int(ORIGIN_X_CENTER - (SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Origin[Z]) / 2));
							SS_Spacehips[j].SS_Origin[Y] = int(ORIGIN_Y_BUTTOM - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Origin[Z]) * 1.5);
							SS_Spacehips[j].SS_Angle = PLAYER_DEFAULT_ANGLE;
							SS_Spacehips[j].SS_Delay[DELAY_SHIELD_DURATION] = CurrentTime + 2000;
							SS_Spacehips[j].SS_Sprite[SPRITE_SHIELD][SPRITE_POS] = 0;
							PlayerLives--;

							if (SS_Spacehips[j].SS_RocketsAmmo < PLAYER_ROCKET_AMOUNT)
								SS_Spacehips[j].SS_RocketsAmmo = PLAYER_ROCKET_AMOUNT;

							SS_Spacehips[j].SS_Delay[DELAY_LASER] = 0;
							SS_Spacehips[j].SS_Delay[DELAY_ROCKET] = 0;
						}
						//--| Respawn Enemy |--//
						else
						{
							SS_Spacehips[j].SS_Origin[X] = int(ORIGIN_X_LEFT + (Resolution[GameResolution][X] / (MAX_SPACESHIPS - ENEMY)) * (j - ENEMY) + (IMG_Images[IMG_ENEMY_SPACESHIP_NORMAL].IMG_ScaledSize[X]));
							SS_Spacehips[j].SS_Origin[Z] = rand() % (MAX_Z + 1);
							SS_Spacehips[j].SS_Origin[Y] = int(ORIGIN_Y_TOP - SpaceshipScale(IMG_Images[IMG_ENEMY_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[j].SS_Origin[Z]) * 2);

							SS_Spacehips[j].SS_RocketsAmmo = PLAYER_ROCKET_AMOUNT;

							SS_Spacehips[j].SS_Delay[DELAY_LASER] = int(CurrentTime + (ENEMY_DELAY_LASER * 2000 + rand() % int(ENEMY_DELAY_LASER * 1000)));
							SS_Spacehips[j].SS_Delay[DELAY_ROCKET] = int(CurrentTime + (ENEMY_DELAY_ROCKET * 2000 + rand() % int(ENEMY_DELAY_ROCKET * 2000)));
						}
					}
					else GameOver = true;
				}
			}
		}
	}

	//--| Afisare Asteroid, Reward |--//
	for (i = 0; i < MAX_ASTEROIDS + MAX_REWARDS; i++)
	{
		// Daca entitatea nu este activa, trece mai departe
		if (!ENT_Entity[i].ENT_Active)
			continue;

		//--| Retine ID-ul imaginii |--//
		ImageID = ENT_Entity[i].ENT_Type == ENT_ASTEROID ? IMG_ENTITY_ASTEROID : (ENT_Entity[i].ENT_Type == ENT_REWARD_RED ? IMG_ENTITY_REWARD_RED : IMG_ENTITY_REWARD_BLUE);

		//--| Verifica daca entitatea este iesit afara din harta si o dezactiveaza |--//
		if (ENT_Entity[i].ENT_Origin[X] - SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[X], ENT_Entity[i].ENT_Origin[Z]) > Resolution[GameResolution][X] || ENT_Entity[i].ENT_Origin[X] + SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[X], ENT_Entity[i].ENT_Origin[Z]) < 0
			|| ENT_Entity[i].ENT_Origin[Y] - SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[Y], ENT_Entity[i].ENT_Origin[Z]) > Resolution[GameResolution][Y] || ENT_Entity[i].ENT_Origin[Y] + SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[Y], ENT_Entity[i].ENT_Origin[Z]) < 0)
			ENT_Entity[i].ENT_Active = false;

		// Calculeaza si misca entitatea in unghiul respectiv
		if (ENT_Entity[i].ENT_Type != ENT_ASTEROID)
		{
			ENT_Entity[i].ENT_Origin[Z] = SS_Spacehips[PLAYER].SS_Origin[Z];
			ENT_Entity[i].ENT_Angle = (atan2(SS_Spacehips[PLAYER].SS_Origin[Y] - ENT_Entity[i].ENT_Origin[Y], SS_Spacehips[PLAYER].SS_Origin[X] - ENT_Entity[i].ENT_Origin[X]) * 360) / (2 * M_PI);
		}

		// Calculeaza si misca entitatea in unghiul acesteia
		ENT_Entity[i].ENT_Origin[X] += int(cos(ENT_Entity[i].ENT_Angle * DEGREE) * DeltaTime * (ENT_Entity[i].ENT_Type == ENT_ASTEROID ? ASTEROID_SPEED : REWARD_SPEED));
		ENT_Entity[i].ENT_Origin[Y] += int(sin(ENT_Entity[i].ENT_Angle * DEGREE) * DeltaTime * (ENT_Entity[i].ENT_Type == ENT_ASTEROID ? ASTEROID_SPEED : REWARD_SPEED));

		//--| Afisaza entitatea |--//
		ShowSprite(ImageID,
				   { 0, IMG_Images[ImageID].IMG_Size[Y] * ENT_Entity[i].ENT_Sprite[SPRITE_POS], IMG_Images[ImageID].IMG_Size[X], IMG_Images[ImageID].IMG_Size[Y] },
				   { ENT_Entity[i].ENT_Origin[X], ENT_Entity[i].ENT_Origin[Y], SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[X], ENT_Entity[i].ENT_Origin[Z]), SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[Y], ENT_Entity[i].ENT_Origin[Z]) }, ENT_Entity[i].ENT_Angle);

		//--| Count pentru animatie |--//
		if (ENT_Entity[i].ENT_Sprite[SPRITE_DELAY] < CurrentTime)
		{
			ENT_Entity[i].ENT_Sprite[SPRITE_DELAY] = CurrentTime + 30;
			ENT_Entity[i].ENT_Sprite[SPRITE_POS] = ENT_Entity[i].ENT_Sprite[SPRITE_POS] >= (IMG_Images[ImageID].IMG_Sprite - 1) ? 0 : ENT_Entity[i].ENT_Sprite[SPRITE_POS] + 1;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENEMY IA |
//==========================================================================================================

void G_IAEnemy(int CurrentTime)
{
	if (GameOver)
		return;

	int i = 0, j = 0;

	for (i = 0; i < MAX_ASTEROIDS; i++)
	{
		if (ENT_Entity[i].ENT_Active)
			continue;

		if (rand() % 101 == 100)
			ADD_Asteroid(i);
	}

	for (i = ENEMY; i < MAX_SPACESHIPS; i++)
	{
		//--| Verifica daca nava nu este distrusa |--//
		if (!SS_Spacehips[i].SS_Alive)
			continue;

		//--| Update axa Z |--//
		if (i % 2 == 0)
		{
			if (SS_Spacehips[PLAYER].SS_Origin[Z] > SS_Spacehips[i].SS_Origin[Z])
				SS_Spacehips[i].SS_Origin[Z]++;
			else if (SS_Spacehips[PLAYER].SS_Origin[Z] < SS_Spacehips[i].SS_Origin[Z])
				SS_Spacehips[i].SS_Origin[Z]--;
		}

		//--| Verifica daca nava se afla in exteriorul hartii |--//
		if (SS_Spacehips[i].SS_Origin[Y] < ORIGIN_Y_TOP + 50)
		{
			SS_Spacehips[i].SS_Origin[Y] += int(DeltaTime * ENEMY_SPEED);
			SS_Spacehips[i].SS_Delay[DELAY_LASER] = int(CurrentTime + DeltaTime);
			SS_Spacehips[i].SS_Delay[DELAY_ROCKET] = int(CurrentTime + DeltaTime);
			continue;
		}

		//--| Update la unchiul navei (Indreptat spre nava adversa) |--//
		SS_Spacehips[i].SS_Angle = int((atan2(SS_Spacehips[PLAYER].SS_Origin[Y] - SS_Spacehips[i].SS_Origin[Y], SS_Spacehips[PLAYER].SS_Origin[X] - SS_Spacehips[i].SS_Origin[X]) * 360) / (2 * M_PI));


		//--| Verifica daca nava poate trage, apoi gaseste un proiectil dezactivat si il foloseste |--//
		if (SS_Spacehips[i].SS_Delay[DELAY_LASER] < CurrentTime)
		{
			for (j = 0; j < MAX_PROJECTILES; j++)
			{
				if (!SS_Spacehips[i].SS_Projectile[j].P_Active)
				{
					SS_Spacehips[i].SS_Delay[DELAY_LASER] = int(CurrentTime + (ENEMY_DELAY_LASER * 1000 + rand() % int(ENEMY_DELAY_LASER * 1000)));
					ADD_Laser(i, j, ENEMY_LASER_SPEED);
					break;
				}
			}
		}

		//--| Verifica daca nava poate trage, apoi gaseste un proiectil dezactivat si il foloseste |--//
		if (SS_Spacehips[i].SS_Delay[DELAY_ROCKET] < CurrentTime && SS_Spacehips[i].SS_RocketsAmmo >= 1)
		{
			for (j = 0; j < MAX_PROJECTILES; j++)
			{
				if (!SS_Spacehips[i].SS_Projectile[j].P_Active)
				{
					SS_Spacehips[i].SS_Delay[DELAY_ROCKET] = int(CurrentTime + (ENEMY_DELAY_ROCKET * 1000 + rand() % int(ENEMY_DELAY_ROCKET * 1000)));
					ADD_Rocket(i, j, ENEMY_ROCKET_SPEED);
					break;
				}
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// G_Collision |
//==========================================================================================================

void G_Collision(int CurrentTime)
{
	enum Object
	{		// Obiectul ales, pentru vectori.
		FIRST_OBJECT,
		SECOND_OBJECT
	};

	int i = 0, j = 0;
	float Width[2], Height[2], CenterX[2], CenterY[2], Radius[2];

	//--| Verifica coliziunea dintre Proiectilele jucatorului si navele adverse |--//
	for (i = 0; i < MAX_PROJECTILES; i++)
	{
		if (!SS_Spacehips[PLAYER].SS_Projectile[i].P_Active)
			continue;

		for (j = ENEMY; j < MAX_SPACESHIPS; j++)
		{
			if (SS_Spacehips[PLAYER].SS_Projectile[i].P_Origin[Z] != SS_Spacehips[j].SS_Origin[Z] && SS_Spacehips[PLAYER].SS_Projectile[i].P_Type == LASER_PLAYER || !SS_Spacehips[j].SS_Alive)
				continue;

			//--| Calculeaza centrul imaginii(navei) si raza |--//
			Width[FIRST_OBJECT] = (float)SpaceshipScale(IMG_Images[IMG_ENEMY_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[j].SS_Origin[Z]) / 2;
			Height[FIRST_OBJECT] = (float)SpaceshipScale(IMG_Images[IMG_ENEMY_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[j].SS_Origin[Z]) / 2;

			CenterX[FIRST_OBJECT] = (float)SS_Spacehips[j].SS_Origin[X] + int(cos(SS_Spacehips[j].SS_Angle * DEGREE) * Width[FIRST_OBJECT]);
			CenterY[FIRST_OBJECT] = (float)SS_Spacehips[j].SS_Origin[Y] + int(sin(SS_Spacehips[j].SS_Angle * DEGREE) * Height[FIRST_OBJECT]);
			Radius[FIRST_OBJECT] = (float)0.5 * ((Width[FIRST_OBJECT] + Height[FIRST_OBJECT]) / 2);

			//--| Calculeaza centrul imaginii(proiectilului) si raza |--//
			Width[SECOND_OBJECT] = (float)SpaceshipScale(IMG_Images[SS_Spacehips[j].SS_Projectile[i].P_Type == LASER_PLAYER ? IMG_PLAYER_LASER : IMG_PLAYER_ROCKET].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Projectile[i].P_Origin[Z]) / 2;;
			Height[SECOND_OBJECT] = (float)SpaceshipScale(IMG_Images[SS_Spacehips[j].SS_Projectile[i].P_Type == LASER_PLAYER ? IMG_PLAYER_LASER : IMG_PLAYER_ROCKET].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Projectile[i].P_Origin[Z]) / 2;;

			CenterX[SECOND_OBJECT] = (float)SS_Spacehips[PLAYER].SS_Projectile[i].P_Origin[X] + int(cos(SS_Spacehips[PLAYER].SS_Projectile[i].P_Angle * DEGREE) * Width[SECOND_OBJECT]);
			CenterY[SECOND_OBJECT] = (float)SS_Spacehips[PLAYER].SS_Projectile[i].P_Origin[Y] + int(sin(SS_Spacehips[PLAYER].SS_Projectile[i].P_Angle * DEGREE) * Height[SECOND_OBJECT]);
			Radius[SECOND_OBJECT] = (float)0.5 * ((Width[SECOND_OBJECT] + Height[SECOND_OBJECT]) / 2);

			//--| Verifica daca exista coliziune intre cele 2 (Proiectil si Nava) |--//
			if (CollisionCircle(CenterX[FIRST_OBJECT], CenterY[FIRST_OBJECT], Radius[FIRST_OBJECT], CenterX[SECOND_OBJECT], CenterY[SECOND_OBJECT], Radius[SECOND_OBJECT]))
			{
				SS_Spacehips[PLAYER].SS_Projectile[i].P_Active = false;
				Init_Explosion(j);
				PlayerScore += 50;
				break;
			}
		}
	}

	//--| Verifica coliziunea dintre Proiectilele navelor inamice si nava jucatorului |--//
	for (i = ENEMY; i < MAX_SPACESHIPS; i++)
	{
		if (!SS_Spacehips[PLAYER].SS_Alive)
			break;

		for (j = 0; j < MAX_PROJECTILES; j++)
		{
			if (SS_Spacehips[i].SS_Projectile[j].P_Origin[Z] != SS_Spacehips[PLAYER].SS_Origin[Z] && SS_Spacehips[i].SS_Projectile[j].P_Type == LASER_ENEMY || !SS_Spacehips[i].SS_Projectile[j].P_Active)
				continue;

			//--| Calculeaza centrul imaginii(navei) si raza |--//
			Width[FIRST_OBJECT] = (float)SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Origin[Z]) / 2;
			Height[FIRST_OBJECT] = (float)SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Origin[Z]) / 2;

			CenterX[FIRST_OBJECT] = (float)SS_Spacehips[PLAYER].SS_Origin[X] + int(cos(SS_Spacehips[PLAYER].SS_Angle * DEGREE) * Width[FIRST_OBJECT]);
			CenterY[FIRST_OBJECT] = (float)SS_Spacehips[PLAYER].SS_Origin[Y] + int(sin(SS_Spacehips[PLAYER].SS_Angle * DEGREE) * Height[FIRST_OBJECT]);
			Radius[FIRST_OBJECT] = (float)0.5 * ((Width[FIRST_OBJECT] + Height[FIRST_OBJECT]) / 2);

			//--| Calculeaza centrul imaginii(proiectilului) si raza |--//
			Width[SECOND_OBJECT] = (float)SpaceshipScale(IMG_Images[SS_Spacehips[j].SS_Projectile[i].P_Type == LASER_ENEMY ? IMG_ENEMY_LASER : IMG_ENEMY_ROCKET].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Projectile[i].P_Origin[Z]) / 2;;
			Height[SECOND_OBJECT] = (float)SpaceshipScale(IMG_Images[SS_Spacehips[j].SS_Projectile[i].P_Type == LASER_ENEMY ? IMG_ENEMY_LASER : IMG_ENEMY_ROCKET].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Projectile[i].P_Origin[Z]) / 2;;

			CenterX[SECOND_OBJECT] = (float)SS_Spacehips[i].SS_Projectile[j].P_Origin[X] + int(cos(SS_Spacehips[i].SS_Projectile[j].P_Angle * DEGREE) * Width[SECOND_OBJECT]);
			CenterY[SECOND_OBJECT] = (float)SS_Spacehips[i].SS_Projectile[j].P_Origin[Y] + int(sin(SS_Spacehips[i].SS_Projectile[j].P_Angle * DEGREE) * Height[SECOND_OBJECT]);
			Radius[SECOND_OBJECT] = (float)0.5 * ((Width[SECOND_OBJECT] + Height[SECOND_OBJECT]) / 2);

			//--| Verifica daca exista coliziune intre cele 2 (Proiectil si Nava) |--//
			if (CollisionCircle(CenterX[FIRST_OBJECT], CenterY[FIRST_OBJECT], Radius[FIRST_OBJECT], CenterX[SECOND_OBJECT], CenterY[SECOND_OBJECT], Radius[SECOND_OBJECT]))
			{
				SS_Spacehips[i].SS_Projectile[j].P_Active = false;

				if (SS_Spacehips[PLAYER].SS_Delay[DELAY_SHIELD_DURATION] < CurrentTime)
				{
					Init_Explosion(PLAYER);
					break;
				}
			}
		}
	}

	//--| Verifica daca exista coliziune intre entitati si nava |--//
	for (i = 0; i < MAX_ASTEROIDS + MAX_REWARDS; i++)
	{
		if (!SS_Spacehips[PLAYER].SS_Alive)
			break;

		if (!ENT_Entity[i].ENT_Active)
			continue;

		int ImageID = ENT_Entity[i].ENT_Type == ENT_ASTEROID ? IMG_ENTITY_ASTEROID : (ENT_Entity[i].ENT_Type == ENT_REWARD_RED ? IMG_ENTITY_REWARD_RED : IMG_ENTITY_REWARD_BLUE);

		//--| Calculeaza centrul imaginii(navei) si raza |--//
		Width[FIRST_OBJECT] = (float)SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Origin[Z]) / 2;
		Height[FIRST_OBJECT] = (float)SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Origin[Z]) / 2;

		CenterX[FIRST_OBJECT] = (float)SS_Spacehips[PLAYER].SS_Origin[X] + Width[FIRST_OBJECT];
		CenterY[FIRST_OBJECT] = (float)SS_Spacehips[PLAYER].SS_Origin[Y] + Height[FIRST_OBJECT];
		Radius[FIRST_OBJECT] = (float)0.5 * ((Width[FIRST_OBJECT] + Height[FIRST_OBJECT]) / 2);

		//--| Calculeaza centrul imaginii(entitatii) si raza |--//
		Width[SECOND_OBJECT] = (float)SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[X], ENT_Entity[i].ENT_Origin[Z]) / 2;;
		Height[SECOND_OBJECT] = (float)SpaceshipScale(IMG_Images[ImageID].IMG_ScaledSize[Y], ENT_Entity[i].ENT_Origin[Z]) / 2;;

		CenterX[SECOND_OBJECT] = (float)ENT_Entity[i].ENT_Origin[X] + Width[SECOND_OBJECT];
		CenterY[SECOND_OBJECT] = (float)ENT_Entity[i].ENT_Origin[Y] + Height[SECOND_OBJECT];
		Radius[SECOND_OBJECT] = (float)0.8 * ((Width[SECOND_OBJECT] + Height[SECOND_OBJECT]) / 2);

		//--| Verifica daca exista coliziune intre cele 2 (Entitate si Nava) |--//
		if (CollisionCircle(CenterX[FIRST_OBJECT], CenterY[FIRST_OBJECT], Radius[FIRST_OBJECT], CenterX[SECOND_OBJECT], CenterY[SECOND_OBJECT], Radius[SECOND_OBJECT]))
		{
			ENT_Entity[i].ENT_Active = false;

			switch (ENT_Entity[i].ENT_Type)
			{
				case ENT_ASTEROID:
				{
					if (SS_Spacehips[PLAYER].SS_Delay[DELAY_SHIELD_DURATION] < CurrentTime)
						Init_Explosion(PLAYER);
					break;
				}
				case ENT_REWARD_BLUE:
				{
					SS_Spacehips[PLAYER].SS_Delay[DELAY_SHIELD_DURATION] = CurrentTime + REWARD_SHIELD_DURATION * 1000;
					SS_Spacehips[PLAYER].SS_Sprite[SPRITE_SHIELD][SPRITE_POS] = 0;
					PlaySound(SND_REWARD_BLUE);
					break;
				}
				case ENT_REWARD_RED:
				{
					SS_Spacehips[PLAYER].SS_RocketsAmmo += REWARD_ROCKETS_AMOUNT;
					SS_Spacehips[PLAYER].SS_Delay[DELAY_ROCKET] = 0;
					PlaySound(SND_REWARD_RED);
					break;
				}
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stock |
//==========================================================================================================

//--| Afisaza meniul |--//
void ShowMenu()
{
	int CenterY = 0;
	ImagesList ImageID;

	//--| Afisare fundal |--//
	ShowImage(IMG_MENU_BACKGROUND, { 0, 0, IMG_Images[IMG_MENU_BACKGROUND].IMG_ScaledSize[X], IMG_Images[IMG_MENU_BACKGROUND].IMG_ScaledSize[Y] });

	//--| Verifica input |--//
	if (InScoreboard)
		G_Events();
	else if (InOptions)
	{
		char Text[32];
		CenterY = IMG_Images[IMG_MENU_RESUMEGAME].IMG_ScaledSize[Y] * MAX_ITEMS_OPTIONS;
		FontsList FontID = FNT_MENU_OPTION;

		//--| Verifica input |--//
		G_Events(MAX_ITEMS_OPTIONS);

		for (int i = 0; i < MAX_ITEMS_OPTIONS; i++)
		{
			ImageID = Menu_Options[i];
			CenterY -= IMG_Images[ImageID].IMG_ScaledSize[Y];

			//--| Afisare imagine |--//
			ShowSprite(ImageID, { 0, i == SelectedItem ? IMG_Images[ImageID].IMG_Size[Y] : 0, IMG_Images[ImageID].IMG_Size[X], IMG_Images[ImageID].IMG_Size[Y] },
					   { ORIGIN_X_CENTER + int(IMG_Images[ImageID].IMG_ScaledSize[X]), ORIGIN_Y_CENTER - CenterY + int(IMG_Images[ImageID].IMG_ScaledSize[Y] / 2), IMG_Images[ImageID].IMG_ScaledSize[X], IMG_Images[ImageID].IMG_ScaledSize[Y] }, MENU_ANGLE);

			//--| Afisare detalii |--//
			if (ImageID == IMG_MENU_RESOLUTION)
			{
				FontID = InResolution ? FNT_MENU_CHOSEN : FNT_MENU_OPTION;
				sprintf_s(Text, "%d x %d", Resolution[GameResolution][X], Resolution[GameResolution][Y]);
			}
			else if (ImageID == IMG_MENU_MUSIC)
			{
				FontID = InMusicVolume ? FNT_MENU_CHOSEN : FNT_MENU_OPTION;
				sprintf_s(Text, "VOL. %d", MusicVolume);
			}
			else if (ImageID == IMG_MENU_SOUNDS)
			{
				FontID = InSoundsVolume ? FNT_MENU_CHOSEN : FNT_MENU_OPTION;
				sprintf_s(Text, "VOL. %d", SoundsVolume);
			}

			if (SetText(FontID, Text, atof(DEFAULT_SCALE)))
				ShowText(FontID, { ORIGIN_X_LEFT + int(IMG_Images[ImageID].IMG_ScaledSize[X] * 2.3), ORIGIN_Y_CENTER - CenterY + int(IMG_Images[ImageID].IMG_ScaledSize[Y] / 2), FNT_Fonts[FontID].FNT_Size[X], FNT_Fonts[FontID].FNT_Size[Y] });
		}
	}
	else
	{
		CenterY = IMG_Images[IMG_MENU_RESUMEGAME].IMG_ScaledSize[Y] * MAX_ITEMS_MAIN;

		//--| Verifica input |--//
		G_Events(MAX_ITEMS_MAIN);

		for (int i = 0; i < MAX_ITEMS_MAIN; i++)
		{
			ImageID = GameStarted ? Menu_Main2[i] : Menu_Main[i];
			CenterY -= IMG_Images[ImageID].IMG_ScaledSize[Y];

			//--| Afisare imagine |--//
			ShowSprite(ImageID, { 0, i == SelectedItem ? IMG_Images[ImageID].IMG_Size[Y] : 0, IMG_Images[ImageID].IMG_Size[X], IMG_Images[ImageID].IMG_Size[Y] },
					   { ORIGIN_X_CENTER - int(IMG_Images[ImageID].IMG_ScaledSize[X]), ORIGIN_Y_CENTER + int(ORIGIN_Y_CENTER / 2) - CenterY + int(IMG_Images[ImageID].IMG_ScaledSize[Y] / 2), IMG_Images[ImageID].IMG_ScaledSize[X], IMG_Images[ImageID].IMG_ScaledSize[Y] }, MENU_ANGLE);
		}
	}
}

//--| Schimba dimensiunile (Rezolutia) |--//
void ResizeWindow(int PevRezolution)
{
	int i = 0, j = 0;

	// Schimba rezolutia ferestrei
	SDL_SetWindowSize(Window, Resolution[GameResolution][X], Resolution[GameResolution][Y]);
	SDL_RenderSetLogicalSize(Render, Resolution[GameResolution][X], Resolution[GameResolution][Y]);

	// Schimba originile
	ORIGIN_Y_TOP = 0;
	ORIGIN_Y_CENTER = int(Resolution[GameResolution][Y] / 2);
	ORIGIN_Y_BUTTOM = Resolution[GameResolution][Y];
	ORIGIN_X_LEFT = 0;
	ORIGIN_X_CENTER = int(Resolution[GameResolution][X] / 2);
	ORIGIN_X_RIGHT = Resolution[GameResolution][X];

	// Hud
	ORIGIN_X_HUD = int(ORIGIN_X_HUD * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	ORIGIN_Y_HUD = int(ORIGIN_Y_HUD * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);

	// Schimba originile si viteza navelor, proiectilelor, entitatilor
	for (i = 0; i < MAX_SPACESHIPS; i++)
	{
		SS_Spacehips[i].SS_Origin[X] = int(SS_Spacehips[i].SS_Origin[X] * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
		SS_Spacehips[i].SS_Origin[Y] = int(SS_Spacehips[i].SS_Origin[Y] * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
		SS_Spacehips[i].SS_Speed = int(SS_Spacehips[i].SS_Speed * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);

		for (j = 0; j < MAX_PROJECTILES; j++)
		{
			SS_Spacehips[i].SS_Projectile[j].P_Origin[X] = int(SS_Spacehips[i].SS_Projectile[j].P_Origin[X] * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
			SS_Spacehips[i].SS_Projectile[j].P_Origin[Y] = int(SS_Spacehips[i].SS_Projectile[j].P_Origin[Y] * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
			SS_Spacehips[i].SS_Projectile[j].P_Speed = int(SS_Spacehips[i].SS_Projectile[j].P_Speed * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
		}
	}
	for (i = 0; i < MAX_ASTEROIDS + MAX_REWARDS; i++)
	{
		ENT_Entity[i].ENT_Origin[X] = int(ENT_Entity[i].ENT_Origin[X] * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
		ENT_Entity[i].ENT_Origin[Y] = int(ENT_Entity[i].ENT_Origin[Y] * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
		ENT_Entity[i].ENT_Speed = int(ENT_Entity[i].ENT_Speed * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	}

	// Schimba vitezele
	PLAYER_SPEED = int(PLAYER_SPEED * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	PLAYER_LASER_SPEED = int(PLAYER_LASER_SPEED * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	PLAYER_ROCKET_SPEED = int(PLAYER_ROCKET_SPEED * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	ENEMY_SPEED = int(ENEMY_SPEED * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	ENEMY_LASER_SPEED = int(ENEMY_LASER_SPEED * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	ENEMY_ROCKET_SPEED = int(ENEMY_ROCKET_SPEED * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	REWARD_SPEED = int(REWARD_SPEED * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	ASTEROID_SPEED = int(ASTEROID_SPEED * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);

	// Schimba dimensiunea imaginilor
	for (int ImageID = 0; ImageID < MAX_IMAGES; ImageID++)
	{
		IMG_Images[ImageID].IMG_ScaledSize[X] = int(IMG_Images[ImageID].IMG_ScaledSize[X] * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
		IMG_Images[ImageID].IMG_ScaledSize[Y] = int(IMG_Images[ImageID].IMG_ScaledSize[Y] * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / PevRezolution);
	}
}

//--| Adauga lasere |--//
void ADD_Laser(int SpaceShipID, int LaserID, int Speed)
{
	SS_Spacehips[SpaceShipID].SS_Projectile[LaserID].P_Active = true;
	SS_Spacehips[SpaceShipID].SS_Projectile[LaserID].P_Type = SS_Spacehips[SpaceShipID].SS_Type == PLAYER ? LASER_PLAYER : LASER_ENEMY;
	SS_Spacehips[SpaceShipID].SS_Projectile[LaserID].P_Origin[X] = SS_Spacehips[SpaceShipID].SS_Origin[X];
	SS_Spacehips[SpaceShipID].SS_Projectile[LaserID].P_Origin[Y] = SS_Spacehips[SpaceShipID].SS_Origin[Y];
	SS_Spacehips[SpaceShipID].SS_Projectile[LaserID].P_Origin[Z] = SS_Spacehips[SpaceShipID].SS_Origin[Z];
	SS_Spacehips[SpaceShipID].SS_Projectile[LaserID].P_Angle = SS_Spacehips[SpaceShipID].SS_Angle;
	SS_Spacehips[SpaceShipID].SS_Projectile[LaserID].P_Speed = Speed;

	PlaySound(SND_LASER);
}

//--| Adauga rachete |--//
void ADD_Rocket(int SpaceShipID, int RocketID, int Speed)
{
	SS_Spacehips[SpaceShipID].SS_Projectile[RocketID].P_Active = true;
	SS_Spacehips[SpaceShipID].SS_Projectile[RocketID].P_Type = SS_Spacehips[SpaceShipID].SS_Type == PLAYER ? ROCKET_PLAYER : ROCKET_ENEMY;
	SS_Spacehips[SpaceShipID].SS_Projectile[RocketID].P_Origin[X] = SS_Spacehips[SpaceShipID].SS_Origin[X];
	SS_Spacehips[SpaceShipID].SS_Projectile[RocketID].P_Origin[Y] = SS_Spacehips[SpaceShipID].SS_Origin[Y];
	SS_Spacehips[SpaceShipID].SS_Projectile[RocketID].P_Origin[Z] = SS_Spacehips[SpaceShipID].SS_Origin[Z];
	SS_Spacehips[SpaceShipID].SS_Projectile[RocketID].P_Angle = SS_Spacehips[SpaceShipID].SS_Angle;
	SS_Spacehips[SpaceShipID].SS_Projectile[RocketID].P_Speed = Speed;
	SS_Spacehips[SpaceShipID].SS_RocketsAmmo--;

	PlaySound(SND_ROCKET);
}

//--| Adauga reward |--//
void ADD_Reward(int SpaceShipID, int RewardID)
{
	ENT_Entity[RewardID].ENT_Active = true;
	ENT_Entity[RewardID].ENT_Type = Entity_Types(rand() % (ENT_REWARD_BLUE + 1));
	ENT_Entity[RewardID].ENT_Origin[X] = SS_Spacehips[SpaceShipID].SS_Origin[X];
	ENT_Entity[RewardID].ENT_Origin[Y] = SS_Spacehips[SpaceShipID].SS_Origin[Y];
	ENT_Entity[RewardID].ENT_Origin[Z] = SS_Spacehips[PLAYER].SS_Origin[Z];
}

//--| Adauga asteroid |--//
void ADD_Asteroid(int AsteroidID)
{
	ENT_Entity[AsteroidID].ENT_Active = true;
	ENT_Entity[AsteroidID].ENT_Type = ENT_ASTEROID;
	ENT_Entity[AsteroidID].ENT_Origin[X] = rand() % ORIGIN_X_RIGHT;
	ENT_Entity[AsteroidID].ENT_Origin[Z] = rand() % (MAX_Z + 1);
	ENT_Entity[AsteroidID].ENT_Origin[Y] = ORIGIN_Y_TOP - SpaceshipScale(IMG_Images[IMG_ENTITY_ASTEROID].IMG_ScaledSize[Y], ENT_Entity[AsteroidID].ENT_Origin[Z]);
	ENT_Entity[AsteroidID].ENT_Angle = (atan2(ORIGIN_Y_BUTTOM - ENT_Entity[AsteroidID].ENT_Origin[Y], rand() % ORIGIN_X_RIGHT - ENT_Entity[AsteroidID].ENT_Origin[X]) * 360) / (2 * M_PI);
}

//--| Initializeaza explozia |--//
void Init_Explosion(int SpaceShipID)
{
	SS_Spacehips[SpaceShipID].SS_Alive = false;
	SS_Spacehips[SpaceShipID].SS_Sprite[SPRITE_EXPLOSION][SPRITE_POS] = 0;
	PlaySound(SND_EXPLOSION);

	if (SS_Spacehips[SpaceShipID].SS_Type != PLAYER)
	{
		for (int i = MAX_ASTEROIDS; i < MAX_ASTEROIDS + MAX_REWARDS; i++)
		{
			if (ENT_Entity[i].ENT_Active)
				continue;

			if (rand() % 4 == 0) // 25% Sanse
				ADD_Reward(SpaceShipID, i);

			break;
		}
	}
}

//--| Afisaza o imagine |--//
void ShowImage(ImagesList ImageID, SDL_Rect DstRect)
{
	SDL_RenderCopy(Render, IMG_Images[ImageID].IMG_Texture, NULL, &DstRect);
	DstRect = { NULL };
}

//--| Afisaza un sprite |--//
void ShowSprite(ImagesList ImageID, SDL_Rect SrcRect, SDL_Rect DstRect, double Angle)
{
	SDL_RenderCopyEx(Render, IMG_Images[ImageID].IMG_Texture, &SrcRect, &DstRect, Angle, NULL, SDL_FLIP_NONE);
	SrcRect = { NULL };
	DstRect = { NULL };
}

//--| Seteaza un text pentru afisare |--//
bool SetText(FontsList FontID, char Text[], double Scale)
{
	FNT_Fonts[FontID].FNT_Surface = TTF_RenderText_Blended(FNT_Fonts[FontID].FNT_Font, Text, FNT_Fonts[FontID].FNT_Color);
	TTF_SetFontStyle(FNT_Fonts[FontID].FNT_Font, TTF_STYLE_BOLD);

	if (FNT_Fonts[FontID].FNT_Surface == NULL)
		return false;

	FNT_Fonts[FontID].FNT_Texture = SDL_CreateTextureFromSurface(Render, FNT_Fonts[FontID].FNT_Surface);

	if (FNT_Fonts[FontID].FNT_Texture == NULL)
		return false;

	FNT_Fonts[FontID].FNT_Size[X] = int(FNT_Fonts[FontID].FNT_Surface->w * (Scale * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / 3000));
	FNT_Fonts[FontID].FNT_Size[Y] = int(FNT_Fonts[FontID].FNT_Surface->h * (Scale * (Resolution[GameResolution][X] + Resolution[GameResolution][Y]) / 3000));
	return true;
}

//--| Afisaza un text (A se folosi SetText inainte) |--//
void ShowText(FontsList FontID, SDL_Rect DstRect)
{
	SDL_RenderCopy(Render, FNT_Fonts[FontID].FNT_Texture, NULL, &DstRect);

	SDL_DestroyTexture(FNT_Fonts[FontID].FNT_Texture);
	SDL_FreeSurface(FNT_Fonts[FontID].FNT_Surface);

	FNT_Fonts[FontID].FNT_Texture = NULL;
	FNT_Fonts[FontID].FNT_Surface = NULL;
	DstRect = { NULL };
}

//--| Porneste un sunet |--//
void PlaySound(SoundsList Sound)
{
	Mix_VolumeChunk(SND_Sounds[Sound].SND_Chunk, SoundsVolume);
	Mix_PlayChannel(-1, SND_Sounds[Sound].SND_Chunk, 0);
}

//--| Verifica coliziunea |--//
bool CollisionCircle(float X1, float Y1, float R1, float X2, float Y2, float R2)
{
	return (pow((X2 - X1), 2) + pow((Y1 - Y2), 2) < pow((R1 + R2), 2));
}

//--| Modifica dimensiunea navei |--//
int SpaceshipScale(int Number, int OriginZ)
{
	return int(Number + ((Number / 100.0) * OriginZ));
}

//--| Modifica valorea pentru a nu depasi minimul si maximul dat |--//
int clamp(int value, int min, int max)
{
	int NewValue = value;

	if (value <= min) NewValue = min;
	else if (value >= max) NewValue = max;

	return NewValue;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game_Init/Exit |
//==========================================================================================================

//--| Initializeaza jocul |--//
bool Game_Init()
{
	//--| Initializare librarie |--//
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 || TTF_Init() < 0)
		return false;

	//--| Fereastra/Render |--//
	Window = SDL_CreateWindow("Dead Space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Resolution[GameResolution][X], Resolution[GameResolution][Y], SDL_WINDOW_FULLSCREEN_DESKTOP);
	Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (Window == NULL || Render == NULL)
		return false;


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");													// Face imaginile sa arate mai bine (Anisotropic - Direct3D)
	SDL_RenderSetLogicalSize(Render, Resolution[GameResolution][X], Resolution[GameResolution][Y]);		// Seteaza rezolutia renderului
	SDL_ShowCursor(SDL_DISABLE);																		// Dezactiveaza mouse-ul

	//--| Incarca Resources |--//
	if (!LoadImages())
		return false;
	if (!LoadSounds())
		return false;
	if (!LoadFonts())
		return false;

	//--| Nave |--//
	// Initializare Jucator
	SS_Spacehips[PLAYER].SS_Alive = true;
	SS_Spacehips[PLAYER].SS_Origin[X] = int(ORIGIN_X_CENTER - (SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[X], SS_Spacehips[PLAYER].SS_Origin[Z]) / 2));
	SS_Spacehips[PLAYER].SS_Origin[Y] = int(ORIGIN_Y_BUTTOM - SpaceshipScale(IMG_Images[IMG_PLAYER_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[PLAYER].SS_Origin[Z]) * 1.5);
	SS_Spacehips[PLAYER].SS_Origin[Z] = int(MAX_Z / 2);
	SS_Spacehips[PLAYER].SS_Type = PLAYER;
	SS_Spacehips[PLAYER].SS_Speed = PLAYER_SPEED;
	SS_Spacehips[PLAYER].SS_Angle = PLAYER_DEFAULT_ANGLE;
	SS_Spacehips[PLAYER].SS_RocketsAmmo = PLAYER_ROCKET_AMOUNT;

	// Initializare Inamici
	for (int i = ENEMY; i < MAX_SPACESHIPS; i++)
	{
		SS_Spacehips[i].SS_Alive = true;
		SS_Spacehips[i].SS_Origin[X] = int(ORIGIN_X_LEFT + (Resolution[GameResolution][X] / (MAX_SPACESHIPS - ENEMY)) * (i - ENEMY) + (IMG_Images[IMG_ENEMY_SPACESHIP_NORMAL].IMG_ScaledSize[X]));
		SS_Spacehips[i].SS_Origin[Z] = rand() % (MAX_Z + 1);
		SS_Spacehips[i].SS_Origin[Y] = int(ORIGIN_Y_TOP - SpaceshipScale(IMG_Images[IMG_ENEMY_SPACESHIP_NORMAL].IMG_ScaledSize[Y], SS_Spacehips[i].SS_Origin[Z]) * 2);
		SS_Spacehips[i].SS_Type = ENEMY;
		SS_Spacehips[i].SS_Speed = ENEMY_SPEED;
		SS_Spacehips[i].SS_Angle = ENEMY_DEFAULT_ANGLE;
		SS_Spacehips[i].SS_RocketsAmmo = PLAYER_ROCKET_AMOUNT;
	}

	return true;
}

//--| Elibereaza memorie si iasa din joc |--//
void FreeAndQuit()
{
	int i = 0;

	//--| Fereastra/Render |--//
	SDL_DestroyRenderer(Render);
	SDL_DestroyWindow(Window);
	Window = NULL;
	Render = NULL;

	//--| Texturi / Surface |--//
	for (i = 0; i < MAX_IMAGES; i++)
	{
		SDL_DestroyTexture(IMG_Images[i].IMG_Texture);
		SDL_FreeSurface(IMG_Images[i].IMG_Surface);
		IMG_Images[i].IMG_Texture = NULL;
		IMG_Images[i].IMG_Surface = NULL;
	}

	//--| Sounds |--//
	Mix_FreeMusic(Sound_Ambience);
	Sound_Ambience = NULL;

	for (i = 0; i < MAX_SOUNDS; i++)
	{
		Mix_FreeChunk(SND_Sounds[i].SND_Chunk);
		SND_Sounds[i].SND_Chunk = NULL;
	}

	//--| Fonts |--//
	for (i = 0; i < MAX_FONTS; i++)
	{
		TTF_CloseFont(FNT_Fonts[i].FNT_Font);
		SDL_DestroyTexture(FNT_Fonts[i].FNT_Texture);
		SDL_FreeSurface(FNT_Fonts[i].FNT_Surface);
		FNT_Fonts[i].FNT_Font = NULL;
		FNT_Fonts[i].FNT_Texture = NULL;
		FNT_Fonts[i].FNT_Surface = NULL;
	}

	//--| Quit |--//
	TTF_Quit();
	SDL_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

//--| Incarca imaginile |--//
bool LoadImages()
{
	for (int ImageID = 0; ImageID < MAX_IMAGES; ImageID++)
	{
		strcpy_s(IMG_Images[ImageID].IMG_Address, Images[ImageID][0]);
		IMG_Images[ImageID].IMG_Surface = IMG_Load(IMG_Images[ImageID].IMG_Address);

		if (IMG_Images[ImageID].IMG_Surface == NULL)
			return false;

		IMG_Images[ImageID].IMG_Size[X] = atoi(Images[ImageID][1]);
		IMG_Images[ImageID].IMG_Size[Y] = atoi(Images[ImageID][2]);
		IMG_Images[ImageID].IMG_ScaledSize[X] = int(IMG_Images[ImageID].IMG_Size[X] * atof(Images[ImageID][3]));
		IMG_Images[ImageID].IMG_ScaledSize[Y] = int(IMG_Images[ImageID].IMG_Size[Y] * atof(Images[ImageID][3]));
		IMG_Images[ImageID].IMG_Sprite = atoi(Images[ImageID][4]);
		IMG_Images[ImageID].IMG_Texture = SDL_CreateTextureFromSurface(Render, IMG_Images[ImageID].IMG_Surface);
	}

	return true;
}

//--| Incarca Sounds |--//
bool LoadSounds()
{
	for (int SoundID = 0; SoundID < MAX_SOUNDS; SoundID++)
	{
		strcpy_s(SND_Sounds[SoundID].SND_Address, Sounds[SoundID][0]);
		SND_Sounds[SoundID].SND_Chunk = Mix_LoadWAV(SND_Sounds[SoundID].SND_Address);

		if (SND_Sounds[SoundID].SND_Chunk == NULL)
			return false;

		Mix_VolumeChunk(SND_Sounds[SoundID].SND_Chunk, SoundsVolume);
	}

	Sound_Ambience = Mix_LoadMUS(SOUND_AMBIENCE);
	Mix_VolumeMusic(MusicVolume);

	return true;
}

//--| Incarca Font-uri |--//
bool LoadFonts()
{
	for (int FontID = 0; FontID < MAX_FONTS; FontID++)
	{
		strcpy_s(FNT_Fonts[FontID].FNT_Address, Fonts[FontID][0]);
		FNT_Fonts[FontID].FNT_Font = TTF_OpenFont(FNT_Fonts[FontID].FNT_Address, atoi(Fonts[FontID][1]));

		if (FNT_Fonts[FontID].FNT_Font == NULL)
			return false;

		FNT_Fonts[FontID].FNT_Color = { (uint8_t)atoi(Fonts[FontID][2]), (uint8_t)atoi(Fonts[FontID][3]), (uint8_t)atoi(Fonts[FontID][4]) };
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------| Dead Space |-------------------------------------------------------
//======================================= sDs|Aragon* =======================================================

*/