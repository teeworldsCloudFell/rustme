/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#ifndef GAME_SERVER_GAMEMODES_DDRACE_H
#define GAME_SERVER_GAMEMODES_DDRACE_H
#include <game/server/entities/door.h>
#include <game/server/gamecontroller.h>
#include <game/server/teams.h>

#include <map>
#include <vector>

struct CScoreInitResult;
class CGameControllerDDRace : public IGameController
{
public:
	class CFlag *m_apFlags[2];
	
	CGameControllerDDRace(class CGameContext *pGameServer);
	~CGameControllerDDRace();

	virtual void DoWincheck();
	virtual bool CanBeMovedOnBalance(int ClientID);
	
	virtual bool OnEntity(int Index, vec2 Pos);
	virtual int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon);
	
	void OnCharacterSpawn(class CCharacter *pChr) override;
	void HandleCharacterTiles(class CCharacter *pChr, int MapIndex) override;

	void OnPlayerDisconnect(class CPlayer *pPlayer, const char *pReason) override;

	void Tick();

	void DoTeamChange(class CPlayer *pPlayer, int Team, bool DoChatMsg = true) override;

	int64 GetMaskForPlayerWorldEvent(int Asker, int ExceptID = -1) override;

	void InitTeleporter();

	int GetPlayerTeam(int ClientID) const;

	CGameTeams m_Teams;

	std::map<int, std::vector<vec2>> m_TeleOuts;
	std::map<int, std::vector<vec2>> m_TeleCheckOuts;

	// std::shared_ptr<CScoreInitResult> m_pInitResult;
};
#endif // GAME_SERVER_GAMEMODES_DDRACE_H
