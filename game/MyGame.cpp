#include "stdafx.h"
#include "MyGame.h"

bool PathFind(vector<NODE>& graph, int nStart, int nGoal, vector<int>& path)
{
	path.push_back(nStart);
	path.push_back(nGoal);
	return true;
}
#pragma warning (disable: 4244)
float Coords[][2] = {
	{ 352, 160 },
	{ 480, 160 },
	{ 608, 288 },
	{ 608, 96 },
	{ 992, 288 },
	{ 912, 96 },
	{ 992, 160 },
	{ 1184, 160 },
	{ 992, 480 },
	{ 1184, 480 },
	{ 1184, 672 },
	{ 800, 672 },
	{ 736, 480 },
	{ 544, 480 },
	{ 512, 672 },
	{ 96, 672 },
	{ 96, 416 },
	{ 352, 416 },
};

int Connections[][2] = {
	{ 0, 1 },
	{ 0, 17 },
	{ 1, 2 },
	{ 1, 3 },
	{ 2, 3 },
	{ 2, 4 },
	{ 3, 5 },
	{ 4, 5 },
	{ 4, 6 },
	{ 4, 8 },
	{ 5, 6 },
	{ 6, 7 },
	{ 7, 9 },
	{ 8, 9 },
	{ 9, 10 },
	{ 10, 11 },
	{ 11, 12 },
	{ 11, 14 },
	{ 12, 13 },
	{ 13, 14 },
	{ 14, 15 },
	{ 15, 16 },
	{ 16, 17 },
};

char* CMyGame::m_tileLayout[12] =
{
	"XXXXXXXXXXXXXXXXXXXX",
	"X     XX        XX X",
	"X         XXXX     X",
	"X     XX  XXXX  XX X",
	"XXXXX XX        XX X",
	"XXXXX XXXXXXXXX XX X",
	"X     XXXXXXXXX XX X",
	"X  XXXXX     XX    X",
	"X XXXXXX XX  XXXXX X",
	"X XXX    XX  XXXXX X",
	"X                  X",
	"XXXXXXXXXXXXXXXXXXXX",
};

CMyGame::CMyGame(void) :
	m_npc(544, 96, 64, 64, 0),
	m_pumpkin(-100, -100, "Pumpkin22.png", CColor::White(), 0),
	m_background(640, 384, "Area51Background.png", CColor::White(), 0)
{
	m_up = false;
	m_down = false;
	m_left = false;
	m_right = false;

	m_npc.LoadAnimation("Ross.png", "walkRight", CSprite::Sheet(4, 2).Row(1).From(0).To(1));
	m_npc.LoadAnimation("Ross.png", "walkLeft", CSprite::Sheet(4, 2).Row(1).From(2).To(3));
	m_npc.LoadAnimation("Ross.png", "idle", CSprite::Sheet(4, 2).Row(0).From(0).To(1));
	m_npc.LoadAnimation("Ross.png", "walkUp", CSprite::Sheet(4, 2).Row(0).From(2).To(2));
	m_npc.LoadAnimation("Ross.png", "walkDown", CSprite::Sheet(4, 2).Row(0).From(3).To(3));

	m_npc.SetAnimation("idle", 4);

	// create graph structure - nodes
	for (float* coord : Coords)
		m_graph.push_back(NODE{ CVector(coord[0], coord[1]) });

	// create graph structure - connections
	for (int* conn : Connections)
	{
		int ind1 = conn[0];
		int ind2 = conn[1];
		NODE& node1 = m_graph[ind1];
		NODE& node2 = m_graph[ind2];
		float dist = Distance(node1.pos, node2.pos);
		node1.conlist.push_back(CONNECTION{ ind2, dist });
		node2.conlist.push_back(CONNECTION{ ind1, dist });
	}

	


}

CMyGame::~CMyGame(void)
{
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	Uint32 t = GetTime();

	float dx = 0;
	float dy = 0;

	if (m_up) dy += 1;
	if (m_down) dy -= 1;
	if (m_left) dx -= 1;
	if (m_right) dx += 1;

	if (dx != 0 && dy != 0)
	{
		dx *= 0.7071f;
		dy *= 0.7071f;
	}

	m_npc.SetVelocity(dx * 200, dy * 200);

	if (dx > 0)
	{
		if (m_currentAnimation != 1)
		{
			m_npc.SetAnimation("walkRight", 8);
			m_currentAnimation = 1;
		}
	}
	else if (dx < 0)
	{
		if (m_currentAnimation != 2)
		{
			m_npc.SetAnimation("walkLeft", 8);
			m_currentAnimation = 2;
		}
	}
	else if (dy > 0)
	{
		if (m_currentAnimation != 3)
		{
			m_npc.SetAnimation("walkUp", 4);
			m_currentAnimation = 3;
		}
	}
	else if (dy < 0)
	{
		if (m_currentAnimation != 4)
		{
			m_npc.SetAnimation("walkDown", 4);
			m_currentAnimation = 4;
		}
	}
	else
	{
		if (m_currentAnimation != 0)
		{
			m_npc.SetAnimation("idle", 4);
			m_currentAnimation = 0;
		}
	}

	CVector oldPosition = m_npc.GetPosition();

	m_npc.Update(t);

	CVector newPosition = m_npc.GetPosition();

	float x = newPosition.GetX();
	float y = newPosition.GetY();

	int tileX = (int)(x / 64);
	int tileY = (int)(y / 64);

	if (x < 0 || x >= 1280 || y < 0 || y >= 768 || m_tileLayout[tileY][tileX] != ' ')
	{
		m_npc.SetPosition(oldPosition);
	}
}

void CMyGame::OnDraw(CGraphics* g)
{
	for (NODE n : m_graph)
		for (CONNECTION c : n.conlist)
			g->DrawLine(n.pos, m_graph[c.nEnd].pos, CColor::Black());
	m_background.Draw(g);
	
	m_tiles.for_each(&CSprite::Draw, g);
	m_pumpkin.Draw(g);
	m_npc.Draw(g);
	
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	// Create Nodes

	int i = 0;
	for (NODE n : m_graph)
	{
		stringstream s;
		s << i++;
		m_nodes.push_back(new CSpriteOval(n.pos, 12, CColor::White(), CColor::Black(), 0));
		m_nodes.push_back(new CSpriteText(n.pos, "arial.ttf", 14, s.str(), CColor::Black(), 0));
	}

	// Create Tiles
	for (int y = 0; y < 12; y++)
		for (int x = 0; x < 20; x++)
		{
			if (m_tileLayout[y][x] == ' ')
				continue;

			int nTile = 5;
			if (y > 0 && m_tileLayout[y - 1][x] == ' ') nTile -= 3;
			if (y < 11 && m_tileLayout[y + 1][x] == ' ') nTile += 3;
			if (x > 0 && m_tileLayout[y][x - 1] == ' ') nTile--;
			if (x < 20 && m_tileLayout[y][x + 1] == ' ') nTile++;
			if (nTile == 5 && x > 0 && y > 0 && m_tileLayout[y - 1][x - 1] == ' ') nTile = 14;
			if (nTile == 5 && x < 20 && y > 0 && m_tileLayout[y - 1][x + 1] == ' ') nTile = 13;
			if (nTile == 5 && x > 0 && y < 11 && m_tileLayout[y + 1][x - 1] == ' ') nTile = 11;
			if (nTile == 5 && x < 20 && y < 11 && m_tileLayout[y + 1][x + 1] == ' ') nTile = 10;
			
			nTile--;
			m_tiles.push_back(new CSprite(x * 64.f + 32.f, y * 64.f + 32.f, new CGraphics("MyTiles.png", 3, 5, nTile % 3, nTile / 3), 0));
		}
}

// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	StartGame();	// exits the menu mode and starts the game mode
}

// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{
}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
}

// called when the game is over
void CMyGame::OnGameOver()
{
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();

	if (sym == SDLK_UP) m_up = true;
	if (sym == SDLK_DOWN) m_down = true;
	if (sym == SDLK_LEFT) m_left = true; 
	if (sym == SDLK_RIGHT) m_right = true;
	
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_UP)m_up = false;
	if (sym == SDLK_DOWN)m_down = false;
	if (sym == SDLK_LEFT)m_left = false;
	if (sym == SDLK_RIGHT)m_right = false;
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
}

void CMyGame::OnLButtonDown(Uint16 x, Uint16 y)
{
	
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
