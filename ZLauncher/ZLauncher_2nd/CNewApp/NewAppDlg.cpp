
#include "stdafx.h"
#include "ZGlobal.h"
#include "NewAppDlg.h"
#include "NewAppAboutBox.h"
#include "NewAppLicenseConfirmDlg.h"
#include "NewAppUpdateConfirm.h"
#include "NewAppUpdateCompleteDlg.h"
#include "NewAppErrorDlg.h"
#include "NewAppWarnVista.h"
#include "CGLEncription.h"
#include <math.h>
#include <Mmsystem.h>


// Settings of USA
#ifdef LOCALE_NHNUSA
#include "ZNHNModule.h"
#define STR_GUNZ_PROCESS_NAME				"Gunz"
#define STR_GUNZ_EXCUTE_FILENAME			"Gunz.exe"
#define STR_LAUNCHER_EXCUTE_FILENAME		"GunzLauncher.exe"
#define STR_PATCHINFO_FILENAME				"patch.xml"
#define STR_WEBPAGE_URL						"http://www.herolatingunz.com/Launcher/start.php"
#define STR_ADDRESS_REAL					"www.herolatingunz.com"
#define STR_ADDRESS_ALPHA					"www.herolatingunz.com"
#define STR_DIRECTORY_REAL					"GunzUpdate"
#define STR_DIRECTORY_ALPHA					"GunzUpdate"
#endif LOCALE_NHNUSA

// Settings of Brazil
#ifdef LOCALE_BRAZIL
#define STR_GUNZ_PROCESS_NAME				"Gunz"
#define STR_GUNZ_EXCUTE_FILENAME			"Gunz.exe"
#define STR_LAUNCHER_EXCUTE_FILENAME		"GunzLauncher.exe"
#define STR_PATCHINFO_FILENAME				"patch.xml"
#define STR_WEBPAGE_URL						"http://launcher.armygaming.net/web/start.php"
#define STR_ADDRESS_REAL					"www.herolatingunz.com"
#define STR_ADDRESS_ALPHA					"www.herolatingunz.com"
#define STR_DIRECTORY_REAL					"GunzUpdate"
#define STR_DIRECTORY_ALPHA					"GunzUpdate"
#endif LOCALE_BRAZIL

// Settings of India
#ifdef LOCALE_INDIA
#define STR_GUNZ_PROCESS_NAME				"Gunz"
#define STR_GUNZ_EXCUTE_FILENAME			"Gunz.exe"
#define STR_LAUNCHER_EXCUTE_FILENAME		"GunzLauncher.exe"
#define STR_PATCHINFO_FILENAME				"patch.xml"
#define STR_WEBPAGE_URL						"http://202.92.10.151/gunzweb/launcher/start.html"
#define STR_ADDRESS_REAL					"patch.gunzonline.co.in"
#define STR_ADDRESS_ALPHA					"patch.gunzonline.co.in"
#define STR_DIRECTORY_REAL					"gunzweb/GunzUpdate"
#define STR_DIRECTORY_ALPHA					"gunzweb/GunzUpdateTest"
#endif LOCALE_INDIA

// Common
#define STR_EXT_UPDATER_FILENAME			"updater.exe"




// Thread
UINT ThreadUpdate( LPVOID pParam);


// DoDataExchange
void CNewAppDlg::DoDataExchange( CDataExchange* pDX)
{
	CDDBDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppDlg)
	DDX_Control(pDX, IDC_STARTGAME, m_cStartGame);
	DDX_Control(pDX, IDC_STOP, m_cStop);
	DDX_Control(pDX, IDC_EXPLORER, m_cExplorer);
	DDX_Control(pDX, IDC_ANIMATION, m_cAnimation);
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP( CNewAppDlg, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_STARTGAME, OnBnClickedStartgame)
	ON_BN_CLICKED(IDC_STOP, OnBnClickedStop)
	ON_BN_CLICKED(IDC_ABOUTBOX, OnAbout)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_MESSAGE(WM_FIND_PATCHINFO, OnFindPatchInfo)
	ON_MESSAGE(WM_UPDATE_COMPLETE, OnUpdateComplete)
	ON_MESSAGE(WM_UPDATE_ERROR, OnUpdateError)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  Dialog
*************************************************************************************/
// ?????? ??????
CNewAppDlg::CNewAppDlg( CWnd* pParent) : CDDBDialog( IDD_DIALOG_MAINDLG, pParent)
{
	// ???????? ?????? ???????????? ?????? ????????
	// ?? ?? ?????? ?????? ?????? CDDBDialog.h ?????? Structure of CDDBDIALOGINFO ?????? ????????
	CDDBDLGINFO DlgInfo;
	DlgInfo.nWidth  = 750;
	DlgInfo.nHeight = 541;
	DlgInfo.IDSkinResource = IDR_BMP_MAINDLG;
	DlgInfo.IDIconResource = IDR_ICO_MAINFRAME;
	DlgInfo.IDWaveResource = IDR_WAV_PUSHBUTTON;
	DlgInfo.bEnableMoveDlg = true;
	DlgInfo.bUseAboutboxButton = true;
	DlgInfo.bUseMinimizeButton = true;
	DlgInfo.bUseQuitButton = true;
	DlgInfo.bUseTitlebar = true;
	DlgInfo.bUseBmpTitle = true;
	DlgInfo.nTextColor = RGB( 30, 30, 30);
	DlgInfo.nTextBkColor = RGB( 150, 150, 150);
#ifdef LOCALE_NHNUSA
	DlgInfo.nTextColor = RGB( 150, 150, 150);
	DlgInfo.nTextBkColor = RGB( 38, 38, 38);
#endif
	SetDialogInfo( DlgInfo);


	// ???? ??????
	m_nStatus = STATUS_NONE;

	m_pThreadUpdate = NULL;
	m_bThreadContinue = false;
	
	m_nMajorVersion = 0;

	m_bUpdateCanceled = false;

	m_bReservedQuit = false;

	m_bSkipUpdate = false;
	m_bIsAlpha = false;
	
	m_bNavigationComplete = false;

	m_bUpdateComplete = false;
}


/************************************************************************************
  ?????? ?????? ??????
*************************************************************************************/
// OnInitDialog : ???????????? ???????? ????
BOOL CNewAppDlg::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();


	// ?????? ?????? ??????
	HRSRC hRes = ::FindResource( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_STRING), _T( "TXT"));
	if ( hRes != NULL)
	{
		HGLOBAL hData = ::LoadResource( AfxGetResourceHandle(), hRes);
		if ( hData != NULL)
		{
			LPVOID lpData= LockResource( hData);

			// Read string table
			STLoadFromMemory( (char*)lpData);

			FreeResource( hData);
		}
	}


	// ???? ???? ??????
	InitLog();


	// NHN ???? ???? ????
/*#ifndef _DEBUG
#ifdef LOCALE_NHNUSA
	if ( !GetNHNModule().GetAuth().InitAuth())
	{
		// ???? ????
		PutLog( "[APP] Cannot find a command.");

		// ?????? ????
		WinExec( "explorer.exe \"http://gunz.ijji.com/?from=desktop\"", SW_SHOW);

		// ???????? ????
		exit( 0);
	}
#endif LOCALE_NHNUSA
#endif*/


	// ?????? ???? ????
	char szVersion[ 128];
	m_nMajorVersion = GetOSVersion( szVersion);
	
	CString strBuff;
	strBuff.Format( "[APP] OS version : %s", szVersion);
	PutLog( strBuff);


	// ?????? ?????????? ???? ????(My Documents, Program Files, WINDOWS)???? ???????? ?????? ????????
	if ( m_nMajorVersion >= 6)
	{
		// ?????? ???? ???????? ?????????? ????
		if ( AfxGetApp()->GetProfileInt( "Flag", "DoNotDisplayVistaWarnning", 0) == 0)
		{
			// ???? ?????? ???? ???? ???????? ????
			char szCurrDir[ 512],  szPath[ 512];
			GetCurrentDirectory( sizeof( szCurrDir), szCurrDir);
			bool bInvalid = false;
			LPITEMIDLIST pidl;


			// Windows ????
			memset( szPath, 0, sizeof( szPath));
			SHGetSpecialFolderLocation( NULL, CSIDL_WINDOWS, &pidl);
			SHGetPathFromIDList( pidl, szPath);
			if ( strstr( szCurrDir, szPath) != NULL)	bInvalid = true;

			// Program Files ????
			memset( szPath, 0, sizeof( szPath));
			ZeroMemory( szPath, sizeof( szPath));
			SHGetSpecialFolderLocation( NULL, CSIDL_PROGRAM_FILES, &pidl);
			SHGetPathFromIDList( pidl, szPath);
			if ( strstr( szCurrDir, szPath) != NULL)	bInvalid = true;

			// My Documents ????
			memset( szPath, 0, sizeof( szPath));
			SHGetSpecialFolderLocation( NULL, CSIDL_PERSONAL, &pidl);
			SHGetPathFromIDList( pidl, szPath);
			if ( strstr( szCurrDir, szPath) != NULL)	bInvalid = true;


			// ?????? ???????? ????
			if ( bInvalid)
			{
				CNewAppWarnVista dlg;
				dlg.SetDialog( /*Warning*/ ZGetString( "STR_100"), /*In Windows Vista, GunZ will not operate normally if installed at current directory.\n\nIf the patch fails, we advise you to uninstall GunZ and reinstall to its default setting.*/ ZGetString( "STR_101"), MB_OK);
				dlg.DoModal();

				if ( dlg.GetChecked())
					AfxGetApp()->WriteProfileInt( "Flag", "DoNotDisplayVistaWarnning", 1);
			}
		}
	}


    // ?????????? ?????? ????
	SetWindowText( /*GunZ Launcher*/ ZGetString( "STR_102"));


	// ?????????? ???? ???? ?? ???? ????
	CRect rect; 
	GetWindowRect( &rect);
	int nWidth = rect.Width(), nHeight = rect.Height();
	rect.left = AfxGetApp()->GetProfileInt( "Window Position", "x", 50);
	rect.top  = AfxGetApp()->GetProfileInt( "Window Position", "y", 50);
	rect.right  = rect.left + nWidth;
	rect.bottom = rect.top  + nHeight;
	MoveWindow( rect, true);


	// ???????? ?????? ??????
	m_cExplorer.MoveWindow( 1, 1, 0, 0, false);


	// ???? ???? ??????
	m_cStartGame.InitDDBButton( IDR_BMP_STARTBTN, IDR_WAV_PUSHBUTTON);
	m_cStartGame.MoveWindow( 518, 461);
	m_cStartGame.EnableWindow( FALSE);


	// ???? ???? ??????
	m_cStop.InitDDBButton( IDR_BMP_BUTTON, IDR_WAV_PUSHBUTTON);
	m_cStop.MoveWindow( 530, 510);
	m_cStop.ShowWindow( SW_HIDE);		// ???????? ??????


	// ?????????? ?????? ??????
	m_cAnimation.InitDDBAniBmp( IDR_BMP_ANIMATION, 6);
	m_cAnimation.SetTimerValue( 200);
	m_cAnimation.MoveWindow( 25, 470, 10, 10);
	m_cAnimation.EnableAnimate( FALSE);
	m_cAnimation.ShowScene( 0);


	// ???? ??????
	GetScreen()->PutBitmap( 28,  20,  IDR_BMP_GUNZBG, 0, 0, 694, 434);
#ifndef LOCALE_NHNUSA
	GetScreen()->PutBitmap( 0,   140, GetSkin()->GetDC(), 490, 0, 18, 165);
	GetScreen()->PutBitmap( 732, 140, GetSkin()->GetDC(), 509, 0, 18, 165);
#endif


	// ???? ???? ????
	m_nStatus = STATUS_NONE;


	// ???? ???? ???????? ????
	UpdateInfo();


	// ???? ?????? ??????
	GetRunInfo();


	// ???? ???????? ???? ????
	GetFileVersion( STR_GUNZ_EXCUTE_FILENAME, m_szClientVersion);


	// ???? ???? ????
	GetFileVersion( STR_LAUNCHER_EXCUTE_FILENAME, m_szLauncherVersion);


	// ???? ????
	PutLog( "[APP] Window created successfully.");


	// ???? ?????????? ?????? ????????
	WIN32_FIND_DATA FindData;
	HANDLE hFind = FindFirstFile( STR_EXT_UPDATER_FILENAME, &FindData);
	if ( hFind != INVALID_HANDLE_VALUE)
		DeleteFile( STR_EXT_UPDATER_FILENAME);


	// ???????? ?????? ????
	SetTimer( TIMER_UPDATE, 100, NULL);


	return true;
}


/************************************************************************************
  ??????
*************************************************************************************/
void CNewAppDlg::OnTimer(UINT nIDEvent)
{
	// ???????? ??????
	if ( nIDEvent == TIMER_UPDATE)
	{
		// ???????? ???? ???? ???????? ????????
		if ( m_bReservedQuit)
		{
			// ???? ???? ????
			switch ( m_nStatus)
			{
				case STATUS_NONE :
				case STATUS_READY :
				case STATUS_CANNOT_FIND_PATCHINFO :
				case STATUS_FIND_NEW_FILE :
				case STATUS_UPDATE_FAIL :
				case STATUS_UPDATE_COMPLETE :
				case STATUS_STARTGAME :
					// ???????? ????
					OnDestroyDialog();
					break;
			}
		}


		// ???????? ???? ???? UI ????
		UpdateStartUI();


		// ???????? ????
		UpdateWebLoading();


		// InfoBox?? ???????? ????
		UpdateInfo();


		// ???? ???? ???????? ???? ?????????? ??????????
		if ( m_nStatus == STATUS_STARTGAME)
			StartGame();
	}


	CDDBDialog::OnTimer(nIDEvent);
}


/************************************************************************************
  ???????? ????
*************************************************************************************/
void CNewAppDlg::OnBnClickedStartgame()
{
	// ???????? ???? ???????? ?????? ??????????
	if ( (m_bSkipUpdate == true) || (m_bUpdateComplete == true))
	{
		// ???????? ?????? ????
#ifdef LOCALE_BRAZIL
		if ( ShowLicenseDlg() == false)
			return;
#endif

		// ???? ????
		m_nStatus = STATUS_STARTGAME;

		StartGame();
	}
	else
	{
		// ???? ????
		if ( !m_Update.IsCreated())
		{
			// ???? ????????...
			if ( m_bIsAlpha)
				m_Update.Create( _T( STR_ADDRESS_ALPHA), 80, STR_DIRECTORY_ALPHA, "", "");

			// ???? ????????...
			else
				m_Update.Create( _T( STR_ADDRESS_REAL), 80, STR_DIRECTORY_REAL, "", "");
		}


		// ?????? ?????? ????

		// ???????? ???? ???????? ???? ?????? ???? ????
		int i = 0;
		while ( m_bThreadContinue == true)
		{
			// ???? ???? ?????? ???????? ???????? ?????? ????
			if ( i > 10)
			{
				PutLog( "[APP] ERROR - The thread is not terminating.");

				m_nStatus = STATUS_READY;

				return;
			}

			Sleep( 100);

			i++;
		}


		m_bThreadContinue = true;
		m_pThreadUpdate = AfxBeginThread( ThreadUpdate, (LPVOID*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		m_pThreadUpdate->ResumeThread();				// Run thread


		// ???????? ???????? ?????????? ???????? ?????? ?????? ???????? ???????? ???? ???????? ?????? ??????
		m_cExplorer.SetFocus();
	}
}


/************************************************************************************
  ???????? ??????
*************************************************************************************/
UINT ThreadUpdate( LPVOID pParam)
{
	// pDlg?? ?????? NewAppDlg ???????? ???? ????/????(public?? ????????)??
	// ?????? ?? ????.
	CNewAppDlg *pDlg = (CNewAppDlg*)pParam;


	// ???? ???????????? ???????? ?????? ???? ??????
	if ( (pDlg->m_nStatus == STATUS_READY) || (pDlg->m_nStatus == STATUS_CANNOT_FIND_PATCHINFO) || (pDlg->m_nStatus == STATUS_UPDATE_FAIL))
	{
		// ???? ????
		PutLog( "[APP] Download patch info file");

		// ???? ????
		pDlg->m_nStatus = STATUS_ANALYSE_PATCHINFO;


		// ???? ???? ?????? ??????
		CString strMessageSecond;
		strMessageSecond.Format( "./%s", STR_PATCHINFO_FILENAME);


		// ???? ?????? ??????
		bool bRetVal = pDlg->m_Update.CheckUpdate( strMessageSecond.GetBuffer());


		// ???? ?????? ???? ????????...
		if ( bRetVal == false)
		{
			// ???? ????
			PutLog( "[APP] ERROR - Cannot download patch info file");


			// ???? ????
			pDlg->m_nStatus = STATUS_CANNOT_FIND_PATCHINFO;


			// ?????? ??????
			pDlg->PostMessage( WM_UPDATE_ERROR, 0, 0);


			// ????
			pDlg->m_bThreadContinue = false;

			return true;
		}

		// ???? ?????? ????????...

		// ?????? ?????? ???? ????,
		if ( pDlg->m_Update.GetUpdateInfo().GetTotalPatchFileCount() > 0)
		{
			// ???? ????
			PutLog( "[APP] Exist patch file(s)");


			// ???? ????
			pDlg->m_nStatus = STATUS_FIND_NEW_FILE;


			// ?????? ??????
			pDlg->PostMessage( WM_FIND_PATCHINFO, 0, 0);
		}

		// ?????? ?????? ???? ????,
		else
		{
			// ???? ????
			pDlg->m_bUpdateComplete = true;
			pDlg->m_nStatus = STATUS_READY;
		}

	}


	// ???????? ?????? ?????????? ???? ?????? ??????????
	else if ( pDlg->m_nStatus == STATUS_FIND_NEW_FILE)
	{
		// ???? ????
		PutLog( "[APP] Start file updating...");


		// ???? ????
		pDlg->m_nStatus = STATUS_UPDATING;


		// ???????? ????
		bool bRetVal = bRetVal = pDlg->m_Update.StartUpdate();


		// ???????? ?????? ????...
		if ( bRetVal == false)
		{
			// ???? ????
			PutLog( "[APP] ERROR - Update fail");


			// ???? ????
			pDlg->m_nStatus = STATUS_UPDATE_FAIL;


			// ?????? ??????
			pDlg->PostMessage( WM_UPDATE_ERROR, 0, 0);
		}

		// ???????? ?????? ????...
		else
		{
			// ???? ????
			PutLog( "[APP] Update complete.");


			// ???? ????
			pDlg->m_nStatus = STATUS_UPDATE_COMPLETE;


			// ?????? ??????
			pDlg->PostMessage( WM_UPDATE_COMPLETE, 0, 0);
		}
	}


	pDlg->m_bThreadContinue = false;

	return true;
}


/************************************************************************************
  ?????????? ????????
*************************************************************************************/
void CNewAppDlg::OnBnClickedStop()
{
	// ???? ????
	PutLog( "[APP] Update canceled...");


	// ???????? ???? ?????? ????
	m_bUpdateCanceled = true;


	// ???????? ????
	m_Update.StopUpdate();


	// ???????? ???????? ?????????? ???????? ?????? ?????? ???????? ???????? ???? ???????? ?????? ??????
	m_cExplorer.SetFocus();
}


/************************************************************************************
  ???????? ???? ???? UI ????????
*************************************************************************************/
void CNewAppDlg::UpdateStartUI()
{
	static bool bUpdateStartUI = true;
	if ( !bUpdateStartUI)
		return;


	// ???? ?????? ????????
	static DWORD dwTime = timeGetTime();
	DWORD currTime = timeGetTime() - dwTime;


	// ???? ???? ???? start game ?????? ?????? ??????
	if ( currTime > 2000)
	{
		m_nStatus = STATUS_READY;

		if ( m_bSkipUpdate == true)
		{
			m_bUpdateComplete = true;

			m_cStartGame.EnableWindow( TRUE);
		}
		else
			OnBnClickedStartgame();


		bUpdateStartUI = false;
	}
}


/************************************************************************************
  ?????????? ????????
*************************************************************************************/
void CNewAppDlg::UpdateWebLoading()
{
	// ???????? ?????? ???????? ????
	static bool bCompleteOpenWeb = false;
	if ( bCompleteOpenWeb)
		return;


	// ?????????? ???????? ????
	static bool bOpenWeb = false;

	// ?????????? ???????? ???? ???? ???? ?????????? ????. ???????? ???? ?????? CPU ???????? ???? ?? ????????...
	static DWORD dwTime = timeGetTime() + 1000;
	if ( !bOpenWeb && (timeGetTime() > dwTime))
	{
		// ???????? ????
		m_cExplorer.Navigate( _T( STR_WEBPAGE_URL), NULL, NULL, NULL, NULL);
		bOpenWeb = true;
	}


	// ???? ?? ???????? ???????? ???????????? ????????. ???? 1???? ??????
	static bool bLoadingComplete = false;
	if ( !bLoadingComplete && m_bNavigationComplete)
	{
		bLoadingComplete = true;
		dwTime = timeGetTime() + 1000;
	}


	// ???????? ?????? ???????? ???????? ????
	if ( !bCompleteOpenWeb && bOpenWeb && bLoadingComplete && (timeGetTime() > dwTime))
	{
#define SIZE_WEBPAGE_X		720
#define SIZE_WEBPAGE_Y		425

//		m_cExplorer.MoveWindow( 15, 25, SIZE_WEBPAGE_X, SIZE_WEBPAGE_Y, FALSE);
//		bCompleteOpenWeb = true;


		// ???? ?????? ?????? ?????? ????
		CRect rect;
		m_cExplorer.GetClientRect( &rect);


		// ????
		float size = (float)(SIZE_WEBPAGE_X - rect.Width()) * 0.7f + 0.5f;
		size = (size > 150.0f) ? 150.0f : size;

		int width = rect.Width() + (int)size;
		width = (width > SIZE_WEBPAGE_X) ? SIZE_WEBPAGE_X : width;


		// ????
		int height = SIZE_WEBPAGE_Y;


		// ?? ?????? ???? ?????? ????????
		m_cExplorer.MoveWindow( 15 + SIZE_WEBPAGE_X - width, 25, width, height, FALSE);


		if ( (width == SIZE_WEBPAGE_X) && (height == SIZE_WEBPAGE_Y))
			bCompleteOpenWeb = true;

		return;
	}


	// ???? ?????? ??????
	int x = 690, y = 415, r = 18;
	float gap = 6.28f / 10.0f;
	for ( int i = 0;  i < 10;  i++)
	{
		float value = gap * (float)i;

		int dx = x + (int)( (float)r * sin( value));
		int dy = y + (int)( (float)r * cos( value));


		int color = (int)( 70.0f * sin( value + (double)timeGetTime() / 300.0f) + 70.0f);

		GetScreen()->PutFillCircle( dx, dy, 3, RGB( color, color, color));
	}


	// ???? ????
	Rendering( x - r - 20,  y - r - 5,  r * 2 + 40,  r * 2 + 10);
}


/************************************************************************************
  ???? ???? ????
*************************************************************************************/
LRESULT CNewAppDlg::OnFindPatchInfo( WPARAM wParam, LPARAM lParam)
{
	// ???? ???? ?????? ???? ????
	if ( m_bReservedQuit)
		return TRUE;


// 	// ?????? ????
// 	CNewAppUpdateConfirm dlg;
// 	CString str;
// 	str.Format( /*There are %d file(s) to update. The game must go through updating process for normal operation.  It will update automatically after %%d second(s)...*/ ZGetString( "STR_104"),  m_Update.GetUpdateInfo().GetTotalPatchFileCount());
// 	dlg.SetDialog( this, /*Update Confirm*/ ZGetString( "STR_103"), str, MB_YESNO);
// 	this->SetFocus();			// ?????? ???? ????
// 	int nRetVal = dlg.DoModal();
// 
// 
// 	// Yes ?????? ?????? ???????? ????
// 	if ( nRetVal == IDOK)
// 		OnBnClickedStartgame();
// 
// 	// No ?????? ????
// 	else
// 		m_nStatus = STATUS_UPDATE_FAIL;
	

	OnBnClickedStartgame();

	return TRUE;
}


/************************************************************************************
  ???????? ????
*************************************************************************************/
LRESULT CNewAppDlg::OnUpdateComplete( WPARAM wParam, LPARAM lParam)
{
	// ???? ???? ?????? ???? ????
	if ( m_bReservedQuit)
		return TRUE;


	// update complete box ????
	CNewAppUpdateCompleteDlg dlg;


	// ???????? ?????? ????
	this->SetFocus();			// ?????? ???? ????
	if ( m_Update.IsReboot())
	{
		// ?????????? ?????? ????
		dlg.SetDialog( this, /*Update Complete*/ ZGetString( "STR_105"), /*The launcher has to restart in order to finish file update.  The launcher will restart in %d second(s)...*/ ZGetString( "STR_106"), MB_OK);
		dlg.DoModal();


		// ???? ???????? ?????????? ????????
		RunExternalUpdater();


		// ???????? ????
		OnQuit();
	}

	// ???? ?????? ???????? ????????...
	else
	{
		// ???????? ???? ????
		m_bUpdateComplete = true;


//		// ???????? ??????
// 		dlg.SetDialog( this, /*Update Complete*/ ZGetString( "STR_105"), /*The Update is successfully finished.  The game will start automatically in %d second(s)...*/ ZGetString( "STR_107"), MB_OK);
// 		dlg.DoModal();
// 
// 		// ???? ????
// 		m_nStatus = STATUS_STARTGAME;

		m_nStatus = STATUS_READY;
	}


	return TRUE;
}


/************************************************************************************
  ???????? ???? ????
*************************************************************************************/
LRESULT CNewAppDlg::OnUpdateError( WPARAM wParam, LPARAM lParam)
{
	// ???? ???? ?????? ???? ????
	if ( m_bReservedQuit)
		return TRUE;


	// ?????? ??????
	CString strLog;
	strLog.Format( "[APP] Updater error message :\n\n%s", m_Update.GetErrorMsg()->GetBuffer());
	PutLog( strLog);

	strLog.Format( "[App] File transfer error message :\n\n%s", m_Update.GetFileTransferError());
	PutLog( strLog);


	// Error dialog ????
	CNewAppErrorDlg dlg;
	dlg.SetDialog( this, /*Error Report*/ ZGetString( "STR_108"), /*There was an error on the process.  We appologize for your inconvenience.  Please check the error report below and elimate the source or error manually.*/ ZGetString( "STR_109"), m_Update.GetErrorMsg(), MB_OK);
	this->SetFocus();			// ?????? ???? ????
	int nRetVal = dlg.DoModal();


	return TRUE;
}


/************************************************************************************
  ???? ????
*************************************************************************************/
void CNewAppDlg::StartGame()
{
	static DWORD dwStartTime = timeGetTime();
	DWORD curr_time = timeGetTime() - dwStartTime;


	// 1?? ???? ???? ?????????? ??????????
	static bool bRunGameProcess = false;
	if ( !bRunGameProcess && (curr_time > 1000))
	{
		// ?????? ????
		CGLEncription cEncription;
		if ( m_bIsAlpha)
			cEncription.CreateSerialKey( 2);			// Alpha testing mode
		else
			cEncription.CreateSerialKey( 0);


		// ???????? ????
		char szCmdLine[ 4096] = { 0, };

#ifdef LOCALE_NHNUSA
		sprintf( szCmdLine, ".\\%s \"%s\"", STR_GUNZ_EXCUTE_FILENAME, GetNHNModule().GetAuth().GetAuthStr());
#else
		sprintf( szCmdLine, ".\\%s", STR_GUNZ_EXCUTE_FILENAME);
#endif

		// ???? ?????????? ??????????
		WinExec( szCmdLine, SW_SHOW);

		bRunGameProcess = true;
		dwStartTime = timeGetTime();

		return;
	}


	// ???? ?????????? ???????? ????????
	static bool bQuit = false;
	if ( bRunGameProcess && !bQuit && (curr_time > 5000))
	{
		// ???????? ????
		OnQuit();

		bQuit = true;
	}
}


/************************************************************************************
  ???? ?? ????????
*************************************************************************************/
void CNewAppDlg::UpdateInfo()
{
	ZUpdateUIInfo info = m_Update.GetUpdateInfo();
	CString strMessageFirst = "";
	CString strMessageSecond = "";
	float fPercent = 0.0f;


	// ???? ?? ?????? ???? ????
	switch ( m_nStatus)
	{
		// ???? ???????? ?????????? ???? ????
		case STATUS_NONE :
		{
			// ???? ????
			strMessageFirst = /*Initializing...*/ ZGetString( "STR_110");
			DWORD dwTime = (timeGetTime() / 120) % 15;
			for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";

			break;
		}

		// ?????? ?????? ?????? ?????????? ????
		case STATUS_READY :
		{
			// ?????????? ????
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);

			// ???? ????
			strMessageFirst = /*Ready to start.*/ ZGetString( "STR_111");

			// UI ????
			if ( m_bUpdateComplete == true)
			{
				m_cStartGame.SetBlanking( TRUE, 800);
				m_cStartGame.EnableWindow( TRUE);
			}
			else
			{
				m_cStartGame.SetBlanking( FALSE);
				m_cStartGame.EnableWindow( FALSE);
			}
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ???????? ???? ??????
		case STATUS_ANALYSE_PATCHINFO :
		{
			// ?????????? ????
			m_cAnimation.EnableAnimate( TRUE);

			// ???? ????
			strMessageFirst = /*Analyzing patch info*/ ZGetString( "STR_114");
			DWORD dwTime = (timeGetTime() / 120) % 15;
			for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";

			// UI ????
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ???????? ?????? ???????? ?????? ????
		case STATUS_CANNOT_FIND_PATCHINFO :
		{
			// ?????????? ????
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);

			// ???? ????
			strMessageFirst = /*Cannot find patch info.*/ ZGetString( "STR_115");

			// UI ????
			m_cStartGame.SetBlanking( FALSE);
			m_cStartGame.EnableWindow( TRUE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ???????? ?? ?????? ???? ????
		case STATUS_FIND_NEW_FILE:
		{
			// ???? ????
			strMessageFirst = /*Find new patch files.*/ ZGetString( "STR_116");

			// UI ????
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ???? ???????? ??
		case STATUS_UPDATING :
		{
			// ???? ????
			unsigned long nCount = info.GetCurrPatchedFileCount() + 1;
			if ( nCount > info.GetTotalPatchFileCount())
			{
				strMessageFirst = /*Patching files*/ ZGetString( "STR_117");
				strMessageSecond = /*Please wait while the file duplication process takes place.*/ ZGetString( "STR_118");

				fPercent = 100.0f;
			}
			else
			{
				strMessageFirst.Format( "%s (%d / %d)", /*Updating files*/ ZGetString( "STR_119"), nCount, info.GetTotalPatchFileCount());


				// ???? ???????????? ?????? ???? ??
				if ( (int)strlen( info.GetCurrPatchFileName()) > 2)
				{
					float _fPercent = (float)info.GetCurrDownloadSize() / (float)info.GetTotalDownloadSize() * 100.0f;
					if ( _fPercent < 0.0f)		_fPercent = 0.0f;
					if ( _fPercent > 100.0f)	_fPercent = 100.0f;

					strMessageSecond.Format( "'%s'    %.1f%%   (%d / %d byte)", info.GetCurrPatchFileName(), _fPercent, info.GetCurrDownloadSize(), info.GetTotalDownloadSize());
				}
				else
				{
					strMessageSecond = /*Read patch file info*/ ZGetString( "STR_120");
					DWORD dwTime = (timeGetTime() / 500) % 5;
					for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";
				}
				fPercent = (float)(info.GetCurrPatchedFileSize() + info.GetCurrDownloadSize()) / (float)info.GetTotalPatchFileSize() * 100.0f + 0.5f;
			}

			DWORD dwTime = (timeGetTime() / 500) % 5;
			for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";


			// UI ????
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( TRUE);
			break;
		}

		// ???? ???????? ????
		case STATUS_UPDATE_FAIL :
		{
			// ?????????? ????
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);

			// ???? ????
			strMessageFirst = /*Update failed.*/ ZGetString( "STR_121");

			// UI ????
			m_cStartGame.SetBlanking( FALSE);
			m_cStartGame.EnableWindow( TRUE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ???? ???????? ????
		case STATUS_UPDATE_COMPLETE :
		{
			// ?????????? ????
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);

			// ???? ????
			strMessageFirst = /*Update complete.*/ ZGetString( "STR_122");
			strMessageSecond = /*Update complete.*/ ZGetString( "STR_122");
			fPercent = 100.0f;

			// UI ????
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( FALSE);
			break;
		}

		// ???? ????
		case STATUS_STARTGAME :
		{
			// ?????????? ????
			m_cAnimation.EnableAnimate( FALSE);
			m_cAnimation.ShowScene( 0);


			// ???? ????
			strMessageFirst = /*Starting game*/ ZGetString( "STR_123");
			DWORD dwTime = (timeGetTime() / 500) % 5;
			for ( int i = 0;  i < (int)dwTime;  i++)	strMessageFirst += ".";

			fPercent = 0.0f;

			// UI ????
			m_cStartGame.EnableWindow( FALSE);
			m_cStop.EnableWindow( FALSE);
			break;
		}
	}


	if ( strMessageSecond.GetLength() == 0)
	{
		if ( m_bSkipUpdate)
			strMessageSecond += /*Skip update.*/ ZGetString( "STR_112");
		strMessageSecond += "    ";

		if ( m_bIsAlpha)
			strMessageSecond += /*Alpha testing mode.*/ ZGetString( "STR_113");
		strMessageSecond += "    ";
	}



	// ???????? 0???? ?????? 100?? ???? ?????? ????
	if ( fPercent < 0.0f)		fPercent = 0.0f;
	if ( fPercent > 100.0f)		fPercent = 100.0f;


	// ???? ???? ????
	strMessageFirst = "     " + strMessageFirst;
	DrawPanel( 10, 460, 500, 70, strMessageFirst);


	// ???? ???????? ???? ???? ????
#ifdef LOCALE_NHNUSA
	GetScreen()->PutText( 23, 489, 15, "Arial", strMessageSecond, GetTextColor());
#else
	GetScreen()->PutText( 20, 489, 15, "Arial", strMessageSecond, GetTextColor());
#endif


	// ?????? ?? ????
	float fGausse = 489.0f * (fPercent / 100.0f) + 1.5f * (float)sin( (double)timeGetTime() / 100.0f);
	if ( (fGausse < 0.0f) || (fPercent < 0.01f))		fGausse = 0.0f;
	if ( (fGausse > 489.0f) || (fPercent > 99.99f)) 	fGausse = 489.0f;

	GetScreen()->PutBitmap( 16, 506, GetSkin()->GetDC(), 0, 86, 489, 19);
	GetScreen()->PutBitmap( 16, 506, GetSkin()->GetDC(), 0, 63, (int)fGausse, 19);


	// ???? ???????? ?????????? ????
	if ( fPercent > 0.0f)
	{
		CString strPercent;
		strPercent.Format( "%.1f%%", fPercent);
#ifdef LOCALE_NHNUSA
		GetScreen()->PutText( 17, 507, 489, 19, 15, "Arial", strPercent.GetBuffer(), RGB( 50, 50, 50), TS_NORMAL, DT_CENTER);
		GetScreen()->PutText( 16, 506, 489, 19, 15, "Arial", strPercent.GetBuffer(), RGB( 200, 200, 200), TS_NORMAL, DT_CENTER);
#else
		GetScreen()->PutText( 16, 508, 489, 19, 15, "Arial", strPercent.GetBuffer(), GetTextColor(), TS_NORMAL, DT_CENTER);
#endif
	}


	// ???? ????
	Rendering( 10, 460, 500, 70);
}


/************************************************************************************
  Run external updater
*************************************************************************************/
void CNewAppDlg::RunExternalUpdater()
{
	HRSRC hRes = ::FindResource( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_UPDATER), _T( "EXE"));
	if ( hRes == NULL)
		return;

	HGLOBAL hData = ::LoadResource( AfxGetResourceHandle(), hRes);
	if ( hData == NULL)
		return;

	LPVOID lpData= LockResource( hData);

	DWORD dwSize = SizeofResource( AfxGetInstanceHandle(), hRes);
	DWORD dwWriteSize = 0;
	HANDLE hFile = ::CreateFile( STR_EXT_UPDATER_FILENAME, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile != INVALID_HANDLE_VALUE)
	{
		// Make 'updater.exe' file
		::WriteFile( hFile, lpData, dwSize, &dwWriteSize, NULL);
		::CloseHandle( hFile);


		// Wait...
		Sleep( 1000);


		// Run updater
		if ( dwSize == dwWriteSize)
		{
			PutLog( "[APP] Run external updater");

			char szCmdLine[ 4096] = { 0, };

#ifdef LOCALE_NHNUSA
			sprintf( szCmdLine, ".\\%s \"%s\"", STR_EXT_UPDATER_FILENAME, GetNHNModule().GetAuth().GetAuthStr());
#else
			sprintf( szCmdLine, ".\\%s", STR_EXT_UPDATER_FILENAME);
#endif

			// Run updater
			WinExec( szCmdLine, SW_SHOW);
		}

		else
			PutLog( "[APP] Invalid updater file");
	}

	else
		PutLog( "[APP] Cannot create updater file");


	FreeResource( hData);
}


/************************************************************************************
  Get run information
*************************************************************************************/
void CNewAppDlg::GetRunInfo()
{
	// INI ???? ????
	CFile file;
	CFileException ex;
	if ( file.Open( "Developer/GunzLauncher.ini", CFile::modeRead, &ex) == TRUE)
	{
		char* szBuff = new char[ (unsigned int)file.GetLength()];
		file.Read( szBuff, (UINT)file.GetLength());


		// ?????????? ???? ?????? ????
		if ( strstr( szBuff, "@SKIP_GUNZ") != NULL)
			m_bSkipUpdate = true;

		// Alpha ???????? ????
		if ( strstr( szBuff, "@TEST_GUNZARMYARENGAMES3212") != NULL)
			m_bIsAlpha = true;


		// ??????, ???? ????
		delete [] szBuff;
		file.Close();
	}


	// USA?? ???? ???? ?????????? ??????
#ifdef LOCALE_NHNUSA

	m_bIsAlpha = false;

	if ( GetNHNModule().GetAuth().IsAlpha())
		m_bIsAlpha = true;

#endif


	// ???? ????
	if ( m_bIsAlpha)
		PutLog( "[APP] Set alpha mode");
}


/************************************************************************************
  About box
*************************************************************************************/
void CNewAppDlg::OnAbout()
{
	// ?????? ????
	CString strMsg;
	strMsg.Format( "%s Launcher\nCopyright (c) 2005 MAIET entertainment, Inc.\n\n%s Version : %s\nLauncher Version : %s",
		STR_GUNZ_PROCESS_NAME, STR_GUNZ_PROCESS_NAME, m_szClientVersion, m_szLauncherVersion);

	// About box ????
	CNewAppAboutBox dlg;
	dlg.SetDialog( this, "About", strMsg, MB_OK);
	this->SetFocus();			// ?????? ???? ????
	dlg.DoModal();
}


/************************************************************************************
  ???? ????
*************************************************************************************/
// OnOK : ???? ???? ???? ?????? ????
void CNewAppDlg::OnOK()
{
}

// OnCancel : ESC ???? ????
void CNewAppDlg::OnCancel()
{
	// ???????? ?????? ?????????? ????????
	if ( m_nStatus == STATUS_UPDATING)
		OnBnClickedStop();

	// ?????? ???? ????????
	else
		OnQuit();
}

// OnQuit
void CNewAppDlg::OnQuit()
{
	// ???????? ???? ?????? ????. ???????? ?????????? ??????????
	m_bReservedQuit = true;


	// ???????? ?????? ?????????? ????????
	if ( m_nStatus == STATUS_UPDATING)
		OnBnClickedStop();
}

// OnDestroyDialog
void CNewAppDlg::OnDestroyDialog()
{
	// ???????? ????
	m_Update.Destroy();


	// ???????????? ???? ?????? ????
	CRect rect;
	GetWindowRect( rect);
	AfxGetApp()->WriteProfileInt( "Window Position", "x", rect.left);
	AfxGetApp()->WriteProfileInt( "Window Position", "y", rect.top);


	// ?????? ????
	CDialog::OnOK();
}


/************************************************************************************
  ???? ?????? ????
*************************************************************************************/
void CNewAppDlg::GetFileVersion( const char* pszFileName, char* pszVersion)
{
	strcpy( pszVersion, "unknown");


	DWORD dwHandle;
	DWORD dwLength = GetFileVersionInfoSize( pszFileName, &dwHandle);

	if ( !dwLength)
		return;

	std::auto_ptr<BYTE> lpBlock(new BYTE[dwLength]);
	
	if ( !GetFileVersionInfo( pszFileName, dwHandle, dwLength, lpBlock.get()))
		return;


	LPVOID pVersion = NULL;
	DWORD uLength;
	if ( !VerQueryValue( lpBlock.get(), "\\VarFileInfo\\Translation", (LPVOID*)&pVersion, (UINT*)&uLength))
		return;


	CString rVersion;
	if ( uLength == 4)
	{
		DWORD langD;
		memcpy( &langD, pVersion, 4);            

		rVersion.Format( "\\StringFileInfo\\%02X%02X%02X%02X\\ProductVersion",
							( langD & 0xff00) >> 8,
							( langD & 0xff),
							( langD & 0xff000000) >> 24,
							( langD & 0xff0000) >> 16);
	}
	else
	{
		rVersion.Format( "\\StringFileInfo\\%04X04B0\\ProductVersion", GetUserDefaultLangID());
	}


	if ( VerQueryValue( lpBlock.get(), (LPSTR)(LPCTSTR)rVersion, (LPVOID*)&pVersion, (UINT *)&uLength) == 0)
		return;


	// Success
	strcpy( pszVersion, (char*)pVersion);
}


/************************************************************************************
  OS ?????? ????
*************************************************************************************/
#define SM_SERVERR2		89
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

int CNewAppDlg::GetOSVersion( char* pszBuff)
{
	int nMajorID = -1;

	memset( pszBuff, 0, sizeof( pszBuff));

	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	BOOL bOsVersionInfoEx;

	ZeroMemory( &si, sizeof( SYSTEM_INFO));
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX);
	if ( !(bOsVersionInfoEx = GetVersionEx( (OSVERSIONINFO*) &osvi)))
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO);
		if ( !GetVersionEx( (OSVERSIONINFO*)&osvi)) 
			return -1;
	}

	pGNSI = (PGNSI)GetProcAddress( GetModuleHandle( TEXT( "kernel32.dll")), "GetNativeSystemInfo");
	if ( NULL != pGNSI)		pGNSI( &si);
	else					GetSystemInfo( &si);

	nMajorID = osvi.dwMajorVersion;

	switch ( osvi.dwPlatformId)
	{
		case VER_PLATFORM_WIN32_NT:
			if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
			{
				if ( osvi.wProductType == VER_NT_WORKSTATION)					strcat( pszBuff, "Windows Vista ");
				else															strcat( pszBuff, "Windows Server \"Longhorn\" ");
			}
			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
			{
				if ( GetSystemMetrics( SM_SERVERR2))							strcat( pszBuff, "Microsoft Windows Server 2003 \"R2\" ");
				else if ( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
																				strcat( pszBuff, "Microsoft Windows XP Professional x64 Edition ");
				else															strcat( pszBuff, "Microsoft Windows Server 2003, ");
			}
			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)			strcat( pszBuff, "Microsoft Windows XP ");
			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)			strcat( pszBuff, "Microsoft Windows 2000 ");
			if ( osvi.dwMajorVersion <= 4)										strcat( pszBuff, "Microsoft Windows NT ");

			if ( bOsVersionInfoEx)
			{
				if ( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture!=PROCESSOR_ARCHITECTURE_AMD64)
				{
					if ( osvi.dwMajorVersion == 4)								strcat( pszBuff, "Workstation 4.0 ");
					else if ( osvi.wSuiteMask & VER_SUITE_PERSONAL)				strcat( pszBuff, "Home Edition ");
					else														strcat( pszBuff, "Professional ");
				}

				else if ( osvi.wProductType == VER_NT_SERVER || osvi.wProductType == VER_NT_DOMAIN_CONTROLLER)
				{
					if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
					{
						if ( si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
						{
							if ( osvi.wSuiteMask & VER_SUITE_DATACENTER)		strcat( pszBuff, "Datacenter Edition for Itanium-based Systems ");
							else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)	strcat( pszBuff, "Enterprise Edition for Itanium-based Systems ");
						}
						else if ( si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
						{
							if ( osvi.wSuiteMask & VER_SUITE_DATACENTER)		strcat( pszBuff, "Datacenter x64 Edition ");
							else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)	strcat( pszBuff, "Enterprise x64 Edition ");
							else												strcat( pszBuff, "Standard x64 Edition ");
						}
						else
						{
							if ( osvi.wSuiteMask & VER_SUITE_DATACENTER)		strcat( pszBuff, "Datacenter Edition ");
							else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)	strcat( pszBuff, "Enterprise Edition ");
							else if ( osvi.wSuiteMask & VER_SUITE_BLADE)		strcat( pszBuff, "Web Edition ");
							else												strcat( pszBuff, "Standard Edition ");
						}
					}
					else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
					{
						if ( osvi.wSuiteMask & VER_SUITE_DATACENTER)			strcat( pszBuff, "Datacenter Server ");
						else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)		strcat( pszBuff, "Advanced Server ");
						else													strcat( pszBuff, "Server ");
					}
					else  // Windows NT 4.0 
					{
						if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE)			strcat( pszBuff, "Server 4.0, Enterprise Edition ");
						else													strcat( pszBuff, "Server 4.0 ");
					}
				}
			}

#define BUFSIZE		80
			else  
			{
				HKEY hKey;
				TCHAR szProductType[ BUFSIZE];
				DWORD dwBufLen = BUFSIZE * sizeof( TCHAR);
				LONG lRet;

				lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT( "SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey);
				if ( lRet != ERROR_SUCCESS)
					return -1;

				lRet = RegQueryValueEx( hKey, TEXT( "ProductType"), NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
				RegCloseKey( hKey);

				if ( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE * sizeof( TCHAR)))
					return -1;

				if ( lstrcmpi( TEXT( "WINNT"), szProductType) == 0)				strcat( pszBuff, "Workstation ");
				if ( lstrcmpi( TEXT( "LANMANNT"), szProductType) == 0)			strcat( pszBuff, "Server ");
				if ( lstrcmpi( TEXT( "SERVERNT"), szProductType) == 0)			strcat( pszBuff, "Advanced Server ");
				
				
				char szTmp[ 128];
				sprintf( szTmp, "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
				strcat( pszBuff, szTmp);
			}

			// Display service pack (if any) and build number.
			if ( osvi.dwMajorVersion == 4 && lstrcmpi( osvi.szCSDVersion, TEXT( "Service Pack 6")) == 0)
			{
				HKEY hKey;
				LONG lRet;

				// Test for SP6 versus SP6a.
				lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT( "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"), 0, KEY_QUERY_VALUE, &hKey);
				if ( lRet == ERROR_SUCCESS)
				{
					char szTmp[ 128];
					sprintf( szTmp, "Service Pack 6a (Build %d)", osvi.dwBuildNumber & 0xFFFF);
					strcat( pszBuff, szTmp);
				}
				else // Windows NT 4.0 prior to SP6a
				{
					char szTmp[ 128];
					sprintf( szTmp, "%s (Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
					strcat( pszBuff, szTmp);
				}

				RegCloseKey( hKey);
			}
			else // not Windows NT 4.0 
			{
				char szTmp[ 128];
				sprintf( szTmp, "%s (Build %d)", osvi.szCSDVersion, osvi.dwBuildNumber & 0xFFFF);
				strcat( pszBuff, szTmp);
			}

			break;


		// Test for the Windows Me/98/95.
		case VER_PLATFORM_WIN32_WINDOWS:
			if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
			{
				strcat( pszBuff, "Microsoft Windows 95 ");
				if ( osvi.szCSDVersion[ 1] == 'C' || osvi.szCSDVersion[ 1] == 'B')
					strcat( pszBuff, "OSR2");
			} 
			if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
			{
				strcat( pszBuff, "Microsoft Windows 98 ");
				if ( osvi.szCSDVersion[ 1] == 'A' || osvi.szCSDVersion[ 1] == 'B')
					strcat( pszBuff, "SE");
			} 
			if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
				strcat( pszBuff, "Microsoft Windows Millennium Edition");
			break;


		case VER_PLATFORM_WIN32s:
			strcat( pszBuff, "Microsoft Win32s");
			break;
	}

	return nMajorID;
}


// ???????? ???? ????
BEGIN_EVENTSINK_MAP(CNewAppDlg, CDDBDialog)
	ON_EVENT(CNewAppDlg, IDC_EXPLORER, 259, DocumentCompleteExplorer, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()


void CNewAppDlg::DocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT* URL)
{
	HRESULT		hr;
	LPUNKNOWN	lpUnknown;
	LPUNKNOWN	lpUnknownWB = NULL;
	LPUNKNOWN	lpUnknownDC = NULL;

	lpUnknown = m_cExplorer.GetControlUnknown();
	if ( lpUnknown)
	{
		// Get the pointer to the IUnknown interface of the WebBrowser 
		// control being hosted. The pointer to the IUnknown returned from 
		// GetControlUnknown is not the pointer to the IUnknown of the 
		// WebBrowser control. It's actually a pointer to the IOleObject.
		hr = lpUnknown->QueryInterface( IID_IUnknown, (LPVOID*)&lpUnknownWB);
		if ( FAILED( hr))
			return;


		// Get the pointer to the IUnknown of the object that fired this 
		// event.
		hr = pDisp->QueryInterface( IID_IUnknown, (LPVOID*)&lpUnknownDC);
		if ( FAILED( hr))
			return;


		if ( lpUnknownWB == lpUnknownDC)
		{
			// The document has finished loading.
			m_bNavigationComplete = true;
		}


		if ( lpUnknownWB)
			lpUnknownWB->Release();

		if ( lpUnknownDC)
			lpUnknownDC->Release();
	}
}


/************************************************************************************
  ?????????? ??????
*************************************************************************************/
bool CNewAppDlg::ShowLicenseDlg()
{
	// ???????????? ????
	CNewAppLicenseConfirmDlg dlg;
	dlg.SetDialog( this, /*Gunz License Agreement*/ZGetString( "STR_149"), "license.htm", ZGetString( "STR_150"), /*I Agree*/ZGetString( "STR_151"), /*Deny*/ZGetString( "STR_152"));

	int nRetVal = dlg.DoModal();
	if ( nRetVal == IDOK)
		return true;


	return false;
}