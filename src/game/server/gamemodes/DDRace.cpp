/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
/* Based on Race mod stuff and tweaked by GreYFoX@GTi and others to fit our DDRace needs. */
#include "DDRace.h"
#include "gamemode.h"
#include <engine/server.h>
#include <engine/shared/config.h>
#include <game/mapitems.h>
#include <game/server/entities/character.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>

CGameControllerDDRace::CGameControllerDDRace(class CGameContext *pGameServer) :
	IGameController(pGameServer), m_Teams(pGameServer) //, m_pInitResult(nullptr)
{
	m_pGameType = g_Config.m_SvTestingCommands ? TEST_NAME : GAME_NAME;
	InitTeleporter();
}

CGameControllerDDRace::~CGameControllerDDRace()
{
	// Nothing to clean
}

void CGameControllerDDRace::OnCharacterSpawn(CCharacter *pChr)
{
	IGameController::OnCharacterSpawn(pChr);
	pChr->SetTeams(&m_Teams);
	pChr->SetTeleports(&m_TeleOuts, &m_TeleCheckOuts);
	m_Teams.OnCharacterSpawn(pChr->GetPlayer()->GetCID());
}

void CGameControllerDDRace::HandleCharacterTiles(CCharacter *pChr, int MapIndex)
{
	CPlayer *pPlayer = pChr->GetPlayer();
	int ClientID = pPlayer->GetCID();

	int m_TileIndex = GameServer()->Collision()->GetTileIndex(MapIndex);
	int m_TileFIndex = GameServer()->Collision()->GetFTileIndex(MapIndex);

	//Sensitivity
	int S1 = GameServer()->Collision()->GetPureMapIndex(vec2(pChr->GetPos().x + pChr->GetProximityRadius() / 3.f, pChr->GetPos().y - pChr->GetProximityRadius() / 3.f));
	int S2 = GameServer()->Collision()->GetPureMapIndex(vec2(pChr->GetPos().x + pChr->GetProximityRadius() / 3.f, pChr->GetPos().y + pChr->GetProximityRadius() / 3.f));
	int S3 = GameServer()->Collision()->GetPureMapIndex(vec2(pChr->GetPos().x - pChr->GetProximityRadius() / 3.f, pChr->GetPos().y - pChr->GetProximityRadius() / 3.f));
	int S4 = GameServer()->Collision()->GetPureMapIndex(vec2(pChr->GetPos().x - pChr->GetProximityRadius() / 3.f, pChr->GetPos().y + pChr->GetProximityRadius() / 3.f));
	int Tile1 = GameServer()->Collision()->GetTileIndex(S1);
	int Tile2 = GameServer()->Collision()->GetTileIndex(S2);
	int Tile3 = GameServer()->Collision()->GetTileIndex(S3);
	int Tile4 = GameServer()->Collision()->GetTileIndex(S4);
	int FTile1 = GameServer()->Collision()->GetFTileIndex(S1);
	int FTile2 = GameServer()->Collision()->GetFTileIndex(S2);
	int FTile3 = GameServer()->Collision()->GetFTileIndex(S3);
	int FTile4 = GameServer()->Collision()->GetFTileIndex(S4);

	const int PlayerDDRaceState = pChr->m_DDRaceState;
	// // start
	// if(((m_TileIndex == TILE_START) || (m_TileFIndex == TILE_START) || FTile1 == TILE_START || FTile2 == TILE_START || FTile3 == TILE_START || FTile4 == TILE_START || Tile1 == TILE_START || Tile2 == TILE_START || Tile3 == TILE_START || Tile4 == TILE_START) && (PlayerDDRaceState == DDRACE_NONE || PlayerDDRaceState == DDRACE_FINISHED || (PlayerDDRaceState == DDRACE_STARTED && !GetPlayerTeam(ClientID) && g_Config.m_SvTeam != 3)))
	// {
	// 	if(m_Teams.GetSaving(GetPlayerTeam(ClientID)))
	// 	{
	// 		if(pChr->m_LastStartWarning < Server()->Tick() - 3 * Server()->TickSpeed())
	// 		{
	// 			GameServer()->SendChatTarget(ClientID, "You can't start while loading/saving of team is in progress");
	// 			pChr->m_LastStartWarning = Server()->Tick();
	// 		}
	// 		pChr->Die(ClientID, WEAPON_WORLD);
	// 		return;
	// 	}
	// 	if(g_Config.m_SvTeam == 2 && (GetPlayerTeam(ClientID) == TEAM_FLOCK || m_Teams.Count(GetPlayerTeam(ClientID)) <= 1))
	// 	{
	// 		if(pChr->m_LastStartWarning < Server()->Tick() - 3 * Server()->TickSpeed())
	// 		{
	// 			GameServer()->SendChatTarget(ClientID, "You have to be in a team with other tees to start");
	// 			pChr->m_LastStartWarning = Server()->Tick();
	// 		}
	// 		pChr->Die(ClientID, WEAPON_WORLD);
	// 		return;
	// 	}
	// 	if(g_Config.m_SvResetPickups)
	// 	{
	// 		pChr->ResetPickups();
	// 	}

	// 	m_Teams.OnCharacterStart(ClientID);
	// 	pChr->m_CpActive = -2;
	// }

	// // finish
	// if(((m_TileIndex == TILE_FINISH) || (m_TileFIndex == TILE_FINISH) || FTile1 == TILE_FINISH || FTile2 == TILE_FINISH || FTile3 == TILE_FINISH || FTile4 == TILE_FINISH || Tile1 == TILE_FINISH || Tile2 == TILE_FINISH || Tile3 == TILE_FINISH || Tile4 == TILE_FINISH) && PlayerDDRaceState == DDRACE_STARTED)
	// 	m_Teams.OnCharacterFinish(ClientID);

	// // unlock team
	// else if(((m_TileIndex == TILE_UNLOCK_TEAM) || (m_TileFIndex == TILE_UNLOCK_TEAM)) && m_Teams.TeamLocked(GetPlayerTeam(ClientID)))
	// {
	// 	m_Teams.SetTeamLock(GetPlayerTeam(ClientID), false);

	// 	for(int i = 0; i < MAX_CLIENTS; i++)
	// 		if(GetPlayerTeam(i) == GetPlayerTeam(ClientID))
	// 			GameServer()->SendChatTarget(i, "Your team was unlocked by an unlock team tile");
	// }

	// // solo part
	// if(((m_TileIndex == TILE_SOLO_ENABLE) || (m_TileFIndex == TILE_SOLO_ENABLE)) && !m_Teams.m_Core.GetSolo(ClientID))
	// {
	// 	GameServer()->SendChatTarget(ClientID, "You are now in a solo part");
	// 	pChr->SetSolo(true);
	// }
	// else if(((m_TileIndex == TILE_SOLO_DISABLE) || (m_TileFIndex == TILE_SOLO_DISABLE)) && m_Teams.m_Core.GetSolo(ClientID))
	// {
	// 	GameServer()->SendChatTarget(ClientID, "You are now out of the solo part");
	// 	pChr->SetSolo(false);
	// }
}

void CGameControllerDDRace::OnPlayerDisconnect(CPlayer *pPlayer, const char *pReason)
{
	int ClientID = pPlayer->GetCID();
	bool WasModerator = pPlayer->m_Moderating && Server()->ClientIngame(ClientID);

	IGameController::OnPlayerDisconnect(pPlayer, pReason);

	if(!GameServer()->PlayerModerating() && WasModerator)
		GameServer()->SendChat(-1, CGameContext::CHAT_ALL, "Server kick/spec votes are no longer actively moderated.");

	if(g_Config.m_SvTeam != 3)
		m_Teams.SetForceCharacterTeam(ClientID, TEAM_FLOCK);
}

void CGameControllerDDRace::Tick()
{
	IGameController::Tick();
	// m_Teams.ProcessSaveTeam();

	// if(m_pInitResult != nullptr && m_pInitResult->m_Completed)
	// {
	// 	if(m_pInitResult->m_Success)
	// 	{
	// 		m_CurrentRecord = m_pInitResult->m_CurrentRecord;
	// 	}
	// 	m_pInitResult = nullptr;
	// }
}

void CGameControllerDDRace::DoTeamChange(class CPlayer *pPlayer, int Team, bool DoChatMsg)
{
	Team = ClampTeam(Team);
	if(Team == pPlayer->GetTeam())
		return;

	CCharacter *pCharacter = pPlayer->GetCharacter();

	if(Team == TEAM_SPECTATORS)
	{
		if(g_Config.m_SvTeam != 3 && pCharacter)
		{
			// Joining spectators should not kill a locked team, but should still
			// check if the team finished by you leaving it.
			int DDRTeam = pCharacter->Team();
			m_Teams.SetForceCharacterTeam(pPlayer->GetCID(), TEAM_FLOCK);
			// m_Teams.CheckTeamFinished(DDRTeam);
		}
	}

	IGameController::DoTeamChange(pPlayer, Team, DoChatMsg);
}

int64 CGameControllerDDRace::GetMaskForPlayerWorldEvent(int Asker, int ExceptID)
{
	if(Asker == -1)
		return CmaskAllExceptOne(ExceptID);

	return m_Teams.TeamMask(GetPlayerTeam(Asker), ExceptID, Asker);
}

void CGameControllerDDRace::InitTeleporter()
{
	if(!GameServer()->Collision()->Layers()->TeleLayer())
		return;
	int Width = GameServer()->Collision()->Layers()->TeleLayer()->m_Width;
	int Height = GameServer()->Collision()->Layers()->TeleLayer()->m_Height;

	for(int i = 0; i < Width * Height; i++)
	{
		int Number = GameServer()->Collision()->TeleLayer()[i].m_Number;
		int Type = GameServer()->Collision()->TeleLayer()[i].m_Type;
		if(Number > 0)
		{
			if(Type == TILE_TELEOUT)
			{
				m_TeleOuts[Number - 1].push_back(
					vec2(i % Width * 32 + 16, i / Width * 32 + 16));
			}
			else if(Type == TILE_TELECHECKOUT)
			{
				m_TeleCheckOuts[Number - 1].push_back(
					vec2(i % Width * 32 + 16, i / Width * 32 + 16));
			}
		}
	}
}

int CGameControllerDDRace::GetPlayerTeam(int ClientID) const
{
	return m_Teams.m_Core.Team(ClientID);
}
