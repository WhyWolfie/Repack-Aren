#include "stdafx.h"
#include "MMatchServer.h"
#include "MSharedCommandTable.h"
#include "MErrorTable.h"
#include "MBlobArray.h"
#include "MObject.h"
#include "MMatchObject.h"
#include "MMatchItem.h"
#include "MAgentObject.h"
#include "MMatchNotify.h"
#include "Msg.h"
#include "MMatchObjCache.h"
#include "MMatchStage.h"
#include "MMatchTransDataType.h"
#include "MMatchFormula.h"
#include "MMatchConfig.h"
#include "MCommandCommunicator.h"
#include "MMatchShop.h"
#include "MMatchTransDataType.h"
#include "MDebug.h"
#include "MMatchAuth.h"
#include "MMatchStatus.h"
#include "MAsyncDBJob.h"
#include "MMatchTransDataType.h"
#include "MMatchAntiHack.h"


void MMatchServer::OnAdminTerminal(const MUID& uidAdmin, const char* szText)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL) return;

	// ������ ������ ���� ����� �ƴϸ� ������ ���´�.
	if (!IsAdminGrade(pObj))
	{
//		DisconnectObject(uidAdmin);		
		return;
	}

	char szOut[32768]; szOut[0] = 0;

	if (m_Admin.Execute(uidAdmin, szText))
	{
		MCommand* pNew = CreateCommand(MC_ADMIN_TERMINAL, MUID(0,0));
		pNew->AddParameter(new MCmdParamUID(MUID(0,0)));
		pNew->AddParameter(new MCmdParamStr(szOut));
		RouteToListener(pObj, pNew);
	}
}

void MMatchServer::OnAdminAnnounce(const MUID& uidAdmin, const char* szChat, unsigned long int nType)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL) return;

	// ������ ������ ���� ����� �ƴϸ� ������ ���´�.
	if (!IsAdminGrade(pObj))
	{
//		DisconnectObject(uidAdmin);		
		return;
	}

	char szMsg[256];
	strcpy(szMsg, szChat);
	MCommand* pCmd = CreateCommand(MC_ADMIN_ANNOUNCE, MUID(0,0));
	pCmd->AddParameter(new MCmdParamUID(uidAdmin));
	pCmd->AddParameter(new MCmdParamStr(szMsg));
	pCmd->AddParameter(new MCmdParamUInt(nType));

	RouteToAllClient(pCmd);
}


void MMatchServer::OnAdminForumAnnounce(const MUID& uidAdmin, const char* szChat)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL) return;

	// ������ ������ ���� ����� �ƴϸ� ������ ���´�.
	if (!IsAdminGrade(pObj))
	{
//		DisconnectObject(uidAdmin);		
		return;
	}

	char szMsg[256];
	strcpy(szMsg, szChat);
	MCommand* pCmd = CreateCommand(MC_ADMIN_FORUM_ANNOUNCE, MUID(0,0));
	pCmd->AddParameter(new MCmdParamUID(uidAdmin));
	pCmd->AddParameter(new MCmdParamStr(szMsg));

	RouteToAllClient(pCmd);
}


void MMatchServer::OnAdminRequestServerInfo(const MUID& uidAdmin)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL) return;

	// ������ ������ ���� ����� �ƴϸ� ������ ���´�.
	if (!IsAdminGrade(pObj))
	{
//		DisconnectObject(uidAdmin);		
		return;
	}

	// ���� ���� �����ִ°� ���� �ȳ־���
/*
	MCommand* pNew = CreateCommand(MC_MATCH_ANNOUNCE, MUID(0,0));
	pNew->AddParameter(new MCmdParamUInt(0));

	RouteToListener(pObj, pNew);
*/
}
void MMatchServer::OnAdminServerHalt(const MUID& uidAdmin)
{
	LOG(LOG_PROG, "OnAdminServerHalt(...) Called");

	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL) return;

	MMatchUserGradeID nGrade = pObj->GetAccountInfo()->m_nUGrade;

	// ������ ������ ���� ����� �ƴϸ� ����.
	if ((nGrade != MMUG_ADMIN) && (nGrade != MMUG_DEVELOPER) && (nGrade != MMUG_POLICE)) return;

	// Shutdown ����	
	m_MatchShutdown.Start(GetGlobalClockCount());	
}

// �������� �޴��θ� ���� ��ɾ�..
void MMatchServer::OnAdminServerHalt()
{
	LOG(LOG_PROG, "OnAdminServerHalt() Called");

	// Shutdown ����	
	m_MatchShutdown.Start(GetGlobalClockCount());	
}

void MMatchServer::OnAdminRequestUpdateAccountUGrade(const MUID& uidAdmin, const char* szPlayer)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL) return;

	// ������ ������ ���� ����� �ƴϸ� ������ ���´�.
	if (!IsAdminGrade(pObj))
	{
//		DisconnectObject(uidAdmin);		
		return;
	}

	int nRet = MOK;

	if ((strlen(szPlayer)) < 2) return;
	MMatchObject* pTargetObj = GetPlayerByName(szPlayer);
	if (pTargetObj == NULL) return;



/*
	MCommand* pNew = CreateCommand(MC_ADMIN_REQUEST_UPDATE_ACCOUNT_UGRADE, MUID(0,0));
	pNew->AddParameter(new MCmdParamUInt(nRet));
	RouteToListener(pObj, pNew);
*/
}

void MMatchServer::OnAdminPingToAll(const MUID& uidAdmin)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL) return;

	// ������ ������ ���� ����� �ƴϸ� ������ ���´�.
	if (!IsAdminGrade(pObj))
	{
//		DisconnectObject(uidAdmin);		
		return;
	}

	MCommand* pNew = CreateCommand(MC_NET_PING, MUID(0,0));
	pNew->AddParameter(new MCmdParamUInt(GetGlobalClockCount()));
	RouteToAllConnection(pNew);
}


void MMatchServer::OnAdminRequestSwitchLadderGame(const MUID& uidAdmin, const bool bEnabled)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (!IsEnabledObject(pObj)) return;

	// ������ ������ ���� ����� �ƴϸ� ������ ���´�.
	if (!IsAdminGrade(pObj))
	{
//		DisconnectObject(uidAdmin);		
		return;
	}

	
	MGetServerConfig()->SetEnabledCreateLadderGame(bEnabled);


	char szMsg[256] = "Action completed.";
	Announce(pObj, szMsg);
}

void MMatchServer::OnAdminHide(const MUID& uidAdmin)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (!IsEnabledObject(pObj)) return;

	// ������ ������ ���� ����� �ƴϸ� ������ ���´�.
	if (!IsAdminGrade(pObj))
	{
//		DisconnectObject(uidAdmin);		
		return;
	}

#if defined(LOCALE_NHNUSA) || defined(_DEBUG)
	m_HackingChatList.Init();
	mlog( "reload hacking chat list.\n" );
#endif

	if (pObj->CheckPlayerFlags(MTD_PlayerFlags_AdminHide)) {
		pObj->SetPlayerFlag(MTD_PlayerFlags_AdminHide, false);
		Announce(pObj, "Now Revealing...");
	} else {
		pObj->SetPlayerFlag(MTD_PlayerFlags_AdminHide, true);
		Announce(pObj, "Now Hiding...");
	}
}

void MMatchServer::OnAdminResetAllHackingBlock( const MUID& uidAdmin )
{
	MMatchObject* pObj = GetObject( uidAdmin );
	if( (0 != pObj) && IsAdminGrade(pObj) )
	{
		GetDBMgr()->AdminResetAllHackingBlock();
	}
}

void MMatchServer::OnAdminRequestKickPlayer(const MUID& uidAdmin, const char* szPlayer)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL)			return;
	if (!IsAdminGrade(pObj))	return;
	if ((strlen(szPlayer)) < 2) return;

	int nRet = MOK;
	MMatchObject* pTargetObj = GetPlayerByName(szPlayer);
	if (pTargetObj != NULL) 
	{
#ifdef LOCALE_KOREA
		pTargetObj->DisconnectHacker( MMHT_COMMAND_BLOCK_BY_ADMIN );
#else
		// Notify Message �ʿ� -> ������ ���� - �ذ�(Ư���� �޼��� �ʿ� ����)
		Disconnect(pTargetObj->GetUID());
#endif
	} else {
		nRet = MERR_ADMIN_NO_TARGET;
	}

	MCommand* pNew = CreateCommand(MC_ADMIN_RESPONSE_KICK_PLAYER, MUID(0,0));
	pNew->AddParameter(new MCmdParamInt(nRet));
	RouteToListener(pObj, pNew);
}

void MMatchServer::OnAdminRequestMutePlayer(const MUID& uidAdmin, const char* szPlayer, const int nPenaltyHour)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL)			return;	
	if (!IsAdminGrade(pObj))	return;
	if ((strlen(szPlayer)) < 2) return;

	int nRet = MOK;
	MMatchObject* pTargetObj = GetPlayerByName(szPlayer);	
	if (pTargetObj != NULL) 
	{
		pTargetObj->GetAccountPenaltyInfo()->SetPenaltyInfo(MPC_CHAT_BLOCK, nPenaltyHour);
		
		const MPenaltyInfo* pPenaltyInfo = pTargetObj->GetAccountPenaltyInfo()->GetPenaltyInfo(MPC_CHAT_BLOCK);
		if( m_MatchDBMgr.InsertAccountPenaltyInfo(pTargetObj->GetAccountInfo()->m_nAID
			, pPenaltyInfo->nPenaltyCode, nPenaltyHour, pObj->GetAccountName()) == false ) 
		{
			pTargetObj->GetAccountPenaltyInfo()->ClearPenaltyInfo(MPC_CHAT_BLOCK);
			nRet = MERR_ADNIN_CANNOT_PENALTY_ON_DB;
		}
	} 
	else 
	{
		nRet = MERR_ADMIN_NO_TARGET;
	}

	MCommand* pNew = CreateCommand(MC_ADMIN_RESPONSE_MUTE_PLAYER, MUID(0,0));
	pNew->AddParameter(new MCmdParamInt(nRet));
	
	if( nRet == MOK ) {
		RouteToListener(pTargetObj, pNew->Clone());
	}

	RouteToListener(pObj, pNew);
}

void MMatchServer::OnAdminRequestBlockPlayer(const MUID& uidAdmin, const char* szPlayer, const int nPenaltyHour)
{
	MMatchObject* pObj = GetObject(uidAdmin);
	if (pObj == NULL)			return;	
	if (!IsAdminGrade(pObj))	return;
	if ((strlen(szPlayer)) < 2) return;

	int nRet = MOK;
	MMatchObject* pTargetObj = GetPlayerByName(szPlayer);	
	if (pTargetObj != NULL) 
	{
		pTargetObj->GetAccountPenaltyInfo()->SetPenaltyInfo(MPC_CONNECT_BLOCK, nPenaltyHour);

		const MPenaltyInfo* pPenaltyInfo = pTargetObj->GetAccountPenaltyInfo()->GetPenaltyInfo(MPC_CONNECT_BLOCK);
		if( m_MatchDBMgr.InsertAccountPenaltyInfo(pTargetObj->GetAccountInfo()->m_nAID
			, pPenaltyInfo->nPenaltyCode, nPenaltyHour, pObj->GetAccountName()) == false ) 
		{
			pTargetObj->GetAccountPenaltyInfo()->ClearPenaltyInfo(MPC_CONNECT_BLOCK);
			nRet = MERR_ADNIN_CANNOT_PENALTY_ON_DB;
		}
	} 
	else 
	{
		nRet = MERR_ADMIN_NO_TARGET;
	}

	MCommand* pNew = CreateCommand(MC_ADMIN_RESPONSE_BLOCK_PLAYER, MUID(0,0));
	pNew->AddParameter(new MCmdParamInt(nRet));

	if( nRet == MOK ) {
		Disconnect(pTargetObj->GetUID());
	}

	RouteToListener(pObj, pNew);
}

void MMatchServer::OnAdminGiveCoins(const MUID& uidAdmin, const char* szPlayerName, const int nCoins)
{
	char szMsg[256] = {0};
	MMatchObject* pObject = GetObject(uidAdmin);

	if(pObject)
	{
		if(IsAdminGrade(pObject))
		{
			MMatchObject* pTarget = GetPlayerByName(szPlayerName);

			if(pTarget)
			{
				unsigned long nAID = 0;

				nAID = pTarget->GetAccountInfo()->m_nAID;

				if(m_MatchDBMgr.AdminGiveCoins(nAID, nCoins))
				{
					MCommand* pCmd = CreateCommand(MC_ADMIN_GIVE_COINS, pObject->GetUID());
					pCmd->AddParameter(new MCmdParamStr(szPlayerName));
					pCmd->AddParameter(new MCmdParamInt(nCoins));
					RouteToListener(pObject, pCmd);

					sprintf(szMsg, "Has enviado %i coins.", nCoins);
					Announce(pObject, szMsg);

					sprintf(szMsg, "Has recibido %i coins!", nCoins);
					Announce(pTarget, szMsg);

					FILE *f = fopen("CoinsLog.txt", "a+");
					fprintf(f, "%i Coins fueron enviadas por %s(AID: %i) en %s a las %s a %s(AID: %i).\n", nCoins, pObject->GetAccountInfo()->m_szUserID, pObject->GetAccountInfo()->m_nAID, __DATE__, __TIME__, pTarget->GetAccountInfo()->m_szUserID, pTarget->GetAccountInfo()->m_nAID);
					fclose(f);
				}
			}
		}
	}
}


void MMatchServer::OnAdminGiveEventCoins(const MUID& uidAdmin, const char* szPlayerName, const int nCoins)
{
	char szMsg[256] = {0};
	MMatchObject* pObject = GetObject(uidAdmin);

	if(pObject)
	{
		if(IsAdminGrade(pObject))
		{
			MMatchObject* pTarget = GetPlayerByName(szPlayerName);

			if(pTarget)
			{
				unsigned long nAID = 0;

				nAID = pTarget->GetAccountInfo()->m_nAID;

				if(m_MatchDBMgr.AdminGiveEventCoins(nAID, nCoins))
				{
					MCommand* pCmd = CreateCommand(MC_ADMIN_GIVE_EVENTCOINS, pObject->GetUID());
					pCmd->AddParameter(new MCmdParamStr(szPlayerName));
					pCmd->AddParameter(new MCmdParamInt(nCoins));
					RouteToListener(pObject, pCmd);

					sprintf(szMsg, "Has enviado %i Event coins.", nCoins);
					Announce(pObject, szMsg);

					sprintf(szMsg, "Has recibido %i Event coins!", nCoins);
					Announce(pTarget, szMsg);

					FILE *f = fopen("EventCoinsLog.txt", "a+");
					fprintf(f, "%i Event Coins fueron enviadas por %s(AID: %i) en %s a las %s a %s(AID: %i).\n", nCoins, pObject->GetAccountInfo()->m_szUserID, pObject->GetAccountInfo()->m_nAID, __DATE__, __TIME__, pTarget->GetAccountInfo()->m_szUserID, pTarget->GetAccountInfo()->m_nAID);
					fclose(f);
				}
			}
		}
	}
}