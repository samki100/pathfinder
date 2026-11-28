#pragma once
struct CONNECTION
{
	int nEnd;			  // index of the destination node
	float cost;		  // cost of the transition
};
struct NODE
{
	CVector pos;		  // 2D position of the node
	list<CONNECTION> conlist;	  // list of connections
};
class CMyGame : public CGame
{
	CSprite m_npc;					// Ross
	CSprite m_guard;				// Guard
	CSpriteList m_tiles;			// Tiles
	CSpriteList m_nodes;			// Nodes
	CSprite m_background;
	static char* m_tileLayout[12];	// Tiles layout

	list<CVector> m_waypoints;

	vector<NODE> m_graph;

	bool m_up = false;
	bool m_down = false;
	bool m_left = false;
	bool m_right = false;
	bool m_guardMovingDown = true;
	int m_guardCurrentAnimation = -1;
	bool m_guardHasSpottedRoss = false;
	int m_currentAnimation = -1;

public:
	CMyGame(void);
	~CMyGame(void);

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle);
	virtual void OnLButtonDown(Uint16 x, Uint16 y);
	virtual void OnLButtonUp(Uint16 x, Uint16 y);
	virtual void OnRButtonDown(Uint16 x, Uint16 y);
	virtual void OnRButtonUp(Uint16 x, Uint16 y);
	virtual void OnMButtonDown(Uint16 x, Uint16 y);
	virtual void OnMButtonUp(Uint16 x, Uint16 y);
};