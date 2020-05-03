#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34

#define DEF_MAXADMINS				10		// 50 -> 10
#define DEF_MAXMAPS					100
#define DEF_MAXAGRICULTURE			200
#define DEF_MAXNPCTYPES				200
#define DEF_MAXBUILDITEMS			300
#define DEF_SERVERSOCKETBLOCKLIMIT	300
#define DEF_MAXBANNED				500
#define DEF_MAXNPCITEMS				1000
#define DEF_MAXCLIENTS				200		// 2000 -> 200
#define DEF_MAXNPCS					15000 //LifeX Fix Mobs Respawn Limit 01/01
#define DEF_MAXITEMTYPES			5000
#define DEF_CLIENTTIMEOUT			10000
#define DEF_SPUPTIME				10000 //LifeX Regen Time to 10 sec 01/01
#define DEF_POISONTIME				12000
#define DEF_HPUPTIME				10000 //LifeX Regen Time to 10 sec 01/01
#define DEF_MPUPTIME				10000 //LifeX Regen Time to 10 sec 01/01
#define DEF_HUNGERTIME				60000
#define DEF_NOTICETIME				80000
#define DEF_SUMMONTIME				300000
#define DEF_AUTOSAVETIME			600000 // original: 1800000
#define MAX_HELDENIANTOWER			200
#define DEF_MAXTELEPORTDESTINATION	20

#define DEF_EXPSTOCKTIME		10000		// ExpStock을 계산하는 시간 간격 
#define DEF_MSGQUENESIZE		100000		// 메시지 큐 사이즈 10만개 
#define DEF_AUTOEXPTIME			360000	// 자동으로 경험치가 올라가는 시간간격 
#define DEF_TOTALLEVELUPPOINT	3			// 레벨업시 할당하는 총 포인트 수 


#define DEF_MAXDYNAMICOBJECTS	60000
#define DEF_MAXDELAYEVENTS		60000
#define DEF_GUILDSTARTRANK		12

#define DEF_SSN_LIMIT_MULTIPLY_VALUE	2	// SSN-limit 곱하는 수 

#define DEF_MAXNOTIFYMSGS		1000
#define DEF_NIGHTTIME			40

#define DEF_CHARPOINTLIMIT		9999		// 각각의 특성치의 최대값								m_sCharStatLimit
#define DEF_RAGPROTECTIONTIME	7000		// 몇 초 이상 지나면 랙으로 부터 보호를 받는지 


#define DEF_ATTACKAI_NORMAL				1	// 무조건 공격 
#define DEF_ATTACKAI_EXCHANGEATTACK		2	// 교환 공격 - 후퇴 
#define DEF_ATTACKAI_TWOBYONEATTACK		3	// 2-1 공격, 후퇴 

#define DEF_MAXFISHS					200
#define DEF_MAXMINERALS					200
#define	DEF_MAXCROPS					200
#define DEF_MAXENGAGINGFISH				30  // 한 물고기에 낚시를 시도할 수 있는 최대 인원 
#define DEF_MAXPOTIONTYPES				500 // 최대 포션 정의 갯수 

#define DEF_SPECIALEVENTTIME			300000 //600000 // 10분
#define DEF_MAXQUESTTYPE				200
#define DEF_DEF_MAXHELDENIANDOOR			10

#define DEF_MAXSUBLOGSOCK				10

#define DEF_ITEMLOG_GIVE				1
#define DEF_ITEMLOG_DROP				2
#define DEF_ITEMLOG_GET					3
#define DEF_ITEMLOG_DEPLETE				4
#define DEF_ITEMLOG_NEWGENDROP			5
#define DEF_ITEMLOG_DUPITEMID			6

// New 07/05/2004
#define DEF_ITEMLOG_BUY					7
#define DEF_ITEMLOG_SELL				8
#define DEF_ITEMLOG_RETRIEVE			9
#define DEF_ITEMLOG_DEPOSIT				10
#define DEF_ITEMLOG_EXCHANGE			11
#define DEF_ITEMLOG_MAKE				13
#define DEF_ITEMLOG_SUMMONMONSTER		14
#define DEF_ITEMLOG_POISONED			15
#define DEF_ITEMLOG_REPAIR				17
#define DEF_ITEMLOG_SKILLLEARN			12
#define DEF_ITEMLOG_MAGICLEARN			16
#define DEF_ITEMLOG_USE					32
#define DEF_ITEMLOG_UPGRADEPOINT		31

#define DEF_MAXDUPITEMID				100

#define DEF_MAXGUILDS					1000 // 동시에 접속할 수 있는 길드수 
#define DEF_MAXONESERVERUSERS			800	// 800 // 한 서버에서 허용할 수 있는 최대 사용자수. 초과된 경우 부활존 혹은 블리딩 아일, 농경지로 보내진다.

#define DEF_MAXGATESERVERSTOCKMSGSIZE	30000

#define DEF_MAXCONSTRUCTNUM				10
#define DEF_MAXSCHEDULE					10
#define DEF_MAXAPOCALYPSE				7
#define DEF_MAXHELDENIAN				10

//v1.4311-3  사투장의 최대 숫자
#define DEF_MAXFIGHTZONE 10 

//============================
#define DEF_LEVELLIMIT		20				// 체험판 레벨 제한치!!!			
//============================

//============================
#define DEF_MINIMUMHITRATIO 15				// 최저 명중 확률 
//============================		

//============================
#define DEF_MAXIMUMHITRATIO	100				// 최대 명중 확률
//============================

//============================
// New Changed 12/05/2004
#define DEF_GMGMANACONSUMEUNIT	15			// Grand Magic Generator 마나 흡수 단위.
//============================


#define DEF_MAXEXPTABLE			3500


#define DEF_STR 0x01 //Correct
#define DEF_DEX 0x02 //Correct
#define DEF_INT	0x03 //Correct
#define DEF_VIT 0x04 //Correct
#define DEF_MAG	0x05 //Correct
#define DEF_CHR 0x06 //Correct

#define DEF_TEST 0xFFFF0000



#define DEF_MAXCRUSADESTRUCTURES		300

#define ANTI_HAX

//#define DEF_DBGAMESERVER				// SQL Server Mode

//#define TEXTOUT

#define NO_MSGSPEEDCHECK


#define DEF_CLIENTSOCKETBLOCKLIMIT	15

#define DEF_MSGBUFFERSIZE	30000
#define DEF_MAXITEMS		50
#define DEF_MAXBANKITEMS	240 // 120 -> 240
#define DEF_MAXGUILDSMAN	128 // 최대 길드원 수 

#define	DEF_MAXMAGICTYPE	100	// 변경하려면 로그서버내용도 바꾸어야 한다.
#define DEF_MAXSKILLTYPE	60

#define DEF_MAXPARTYMEMBERS	8

#define DEF_SPECABLTYTIMESEC	1200

#define DEF_DELAYEVENTTYPE_DAMAGEOBJECT				1
#define DEF_DELAYEVENTTYPE_MAGICRELEASE				2
#define DEF_DELAYEVENTTYPE_USEITEM_SKILL			3
#define DEF_DELAYEVENTTYPE_METEORSTRIKE				4
#define DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE		5
#define DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT	6
#define DEF_DELAYEVENTTYPE_ANCIENT_TABLET			7

#define DEF_DELAYEVENTTYPE_END_APOCALYPSE			8
#define DEF_DELAYEVENTTYPE_KILL_ABADDON				9

#define DEF_DELAYEVENTTYPE_TAMING_SKILL				10

#define DEF_MAXITEMEQUIPPOS		15
#define DEF_EQUIPPOS_NONE		0	
#define DEF_EQUIPPOS_HEAD		1	//66Ah	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD]
#define DEF_EQUIPPOS_BODY		2	//66Ch	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY]
#define DEF_EQUIPPOS_ARMS		3	//66Eh	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS]
#define DEF_EQUIPPOS_PANTS		4	//670h	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS]
#define DEF_EQUIPPOS_LEGGINGS	5	//672h	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS]
#define DEF_EQUIPPOS_NECK		6   //674h	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_NECK]
#define DEF_EQUIPPOS_LHAND		7	//676h  m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND]
#define DEF_EQUIPPOS_RHAND		8	//678h	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]
#define DEF_EQUIPPOS_TWOHAND	9	//67Ah	m_pClientList[]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND]
#define DEF_EQUIPPOS_RFINGER	10	//67Ch
#define DEF_EQUIPPOS_LFINGER	11	//67Eh
#define DEF_EQUIPPOS_BACK		12  //680h
#define DEF_EQUIPPOS_RELEASEALL	13	//682h

#define DEF_ITEMTYPE_NOTUSED	-1	// v1.4 기존에 사용되었으나 현재로서는 사용되지 않는 아이템: 포상금으로 대체된다.
#define DEF_ITEMTYPE_NONE		 0
#define DEF_ITEMTYPE_EQUIP		 1
#define DEF_ITEMTYPE_APPLY		 2
#define DEF_ITEMTYPE_USE_DEPLETE 3       
#define DEF_ITEMTYPE_INSTALL	 4
#define DEF_ITEMTYPE_CONSUME	 5
#define DEF_ITEMTYPE_ARROW		 6
#define DEF_ITEMTYPE_EAT		 7
#define DEF_ITEMTYPE_USE_SKILL   8
#define DEF_ITEMTYPE_USE_PERM    9
#define DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define DEF_ITEMTYPE_USE_DEPLETE_DEST			11
#define DEF_ITEMTYPE_MATERIAL					12


#define DEF_ITEMEFFECTTYPE_NONE				0
#define DEF_ITEMEFFECTTYPE_ATTACK			1		// 공격치: value1 D value2 + value3
#define DEF_ITEMEFFECTTYPE_DEFENSE			2		// 방어능력 
#define DEF_ITEMEFFECTTYPE_ATTACK_ARROW		3		// 화살을 사용하는 공격무기. 무기 자체는 공격력이 없다.		
#define DEF_ITEMEFFECTTYPE_HP   		    4		// HP관련 효과 
#define DEF_ITEMEFFECTTYPE_MP   		    5		// MP관련 효과 
#define DEF_ITEMEFFECTTYPE_SP   		    6		// SP관련 효과 
#define DEF_ITEMEFFECTTYPE_HPSTOCK 		    7		// HP관련 효과. 즉각적으로 반응이 나타나지는 않는다. 
#define DEF_ITEMEFFECTTYPE_GET			    8		// 얻는다. 물고기나 광물등 
#define DEF_ITEMEFFECTTYPE_STUDYSKILL		9		// 기술을 배우는 아이템이다.
#define DEF_ITEMEFFECTTYPE_SHOWLOCATION		10		// 위치를 표시하는 아이템. 
#define DEF_ITEMEFFECTTYPE_MAGIC			11		// 사용하면 마법 효과를 얻는 아이템 
#define DEF_ITEMEFFECTTYPE_CHANGEATTR		12		// 플레이어의 속성을 변환시킨다. 성별, 머리모양, 피부색, 헤어스타일 
#define DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE	13		// 공격효과와 더불어 마나의 절약 효과가 있다.(마법사용)
#define DEF_ITEMEFFECTTYPE_ADDEFFECT	    14		// 추가 효과
#define DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE	15		// 마법으로 인한 공격의 대미지를 절감한다. 반지나 목걸이류 
#define DEF_ITEMEFFECTTYPE_OCCUPYFLAG		16		// 점령 깃발 
#define DEF_ITEMEFFECTTYPE_DYE				17		// 염색약. 
#define DEF_ITEMEFFECTTYPE_STUDYMAGIC		18		// 마법을 배우는 아이템이다.
#define DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19		// 공격 효과와 함께 최대  HP 및 HP 회복량이 떨어지는 아이템 
#define DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE	20		// 공격 효과와 함께 갑옷과 같이 물리 방어구 효과가 있는 아이템 
#define DEF_ITEMEFFECTTYPE_MATERIAL_ATTR	21		// 무기 제작 재료 속성 
#define DEF_ITEMEFFECTTYPE_FIRMSTAMINAR		22		// 스태미너 고정 아이템 
#define DEF_ITEMEFFECTTYPE_LOTTERY			23		// 복권 아이템
#define DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY		24	// 공격 아이템 특수 효과 
#define DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25	// 방어구 아이템 특수 효과 
#define DEF_ITEMEFFECTTYPE_ALTERITEMDROP		26	// 다른 아이템 떨어질 것 대신 이 아이템이 떨어진다.
#define DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT		27	// 건축 키트
#define DEF_ITEMEFFECTTYPE_WARM				28		// Unfreeze pot ?
#define DEF_ITEMEFFECTTYPE_FARMING			30
#define DEF_ITEMEFFECTTYPE_SLATES			31
#define DEF_ITEMEFFECTTYPE_ARMORDYE			32

#define DEF_ITEMEFFECTTYPE_PANDORA   34

#define DEF_ITEMEFFECTTYPE_WEAPONDYE   41 // WeaponDye

#define DEF_ITET_UNIQUE_OWNER				1		// ItemTouchEffect: 주인을 갖는 아이템 
#define DEF_ITET_ID							2		// 그냥 아이템 아이디
#define DEF_ITET_DATE						3		// 사용가능한 날짜가 지정된 아이템 

#define DEF_MAGICTYPE_DAMAGE_SPOT				1
#define DEF_MAGICTYPE_HPUP_SPOT					2
#define DEF_MAGICTYPE_DAMAGE_AREA				3
#define DEF_MAGICTYPE_SPDOWN_SPOT				4
#define DEF_MAGICTYPE_SPDOWN_AREA				5
#define DEF_MAGICTYPE_SPUP_SPOT					6
#define DEF_MAGICTYPE_SPUP_AREA					7
#define DEF_MAGICTYPE_TELEPORT					8
#define DEF_MAGICTYPE_SUMMON					9 
#define DEF_MAGICTYPE_CREATE					10
#define DEF_MAGICTYPE_PROTECT					11	
#define DEF_MAGICTYPE_HOLDOBJECT				12
#define DEF_MAGICTYPE_INVISIBILITY				13
#define DEF_MAGICTYPE_CREATE_DYNAMIC			14
#define DEF_MAGICTYPE_POSSESSION				15
#define DEF_MAGICTYPE_CONFUSE					16 // Mass-Illusion, Illusion, Illusion-Movement, Mass-Illusion-Movement
#define DEF_MAGICTYPE_POISON					17
#define DEF_MAGICTYPE_BERSERK					18
#define DEF_MAGICTYPE_DAMAGE_LINEAR				19
#define DEF_MAGICTYPE_POLYMORPH					20
#define DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT		21 // Meteor-Strike, Mass-Magic-Missile
#define DEF_MAGICTYPE_TREMOR					22
#define DEF_MAGICTYPE_ICE						23
// 24 
#define DEF_MAGICTYPE_DAMAGE_AREA_NOSPOT_SPDOWN	25
#define DEF_MAGICTYPE_ICE_LINEAR				26 // Blizzard
// 27
#define DEF_MAGICTYPE_DAMAGE_AREA_ARMOR_BREAK   28
#define DEF_MAGICTYPE_CANCELLATION				29
#define DEF_MAGICTYPE_DAMAGE_LINEAR_SPDOWN		30 // Earth-Shock-Wave
#define DEF_MAGICTYPE_INHIBITION				31 // Inhibition-Casting	
#define DEF_MAGICTYPE_RESURRECTION				32 // Resurrection
#define DEF_MAGICTYPE_SCAN						33 // Scan

#define DEF_MAGICTYPE_CREATEPOTION                34

#define DEF_MAGICTYPE_HELLFIRE 44
#define DEF_MAGICTYPE_FURYOFTHOR 24

#define DEF_MAXMAGICEFFECTS			100

#define DEF_OWNERTYPE_PLAYER			1
#define DEF_OWNERTYPE_NPC				2
#define DEF_OWNERTYPE_PLAYER_INDIRECT	3

#define DEF_OWNERTYPE_PLAYER_WAITING	8 // Snoopy: added to wait for the player connecting again...
#define DEF_OWNERTYPE_PLAYER_FREE		9 // Snoopy: added for every player to take over this summon

// for Client Data

#define DEF_MAPDATASIZEX		60 // 30
#define DEF_MAPDATASIZEY		55 // 25

#define DEF_MAXTELEPORTLOC		200
#define DEF_MAXWAYPOINTCFG		200
#define DEF_MAXMGAR				50
#define DEF_MAXNMR				50
#define DEF_MAXSPOTMOBGENERATOR	100
#define DEF_MAXFISHPOINT		200
#define DEF_MAXITEMEVENTS		200
#define DEF_MAXMINERALPOINT		200
#define DEF_MAXHELDENIANDOOR	200
#define DEF_MAXOCCUPYFLAG		20001 //3001
#define	DEF_MAXINITIALPOINT		20
#define DEF_MAXAGRICULTURE		200
#define DEF_MAXHELDENIANTOWER	200
#define DEF_MAXDYNAMICGATE		10

#define DEF_MAPTYPE_NORMAL				0
#define DEF_MAPTYPE_NOPENALTY_NOREWARD	1
#define DEF_MAXENERGYSPHERES			10

#define DEF_MAXSTRATEGICPOINTS			200
#define DEF_MAXSECTORS					60
#define DEF_MAXSTRIKEPOINTS				20

#define DEF_INDEX4_MSGID			0
#define DEF_INDEX2_MSGTYPE			4

#define DEF_MSGFROM_CLIENT		1
#define DEF_MSGFROM_LOGSERVER	2
#define DEF_MSGFROM_GATESERVER	3
#define DEF_MSGFROM_BOT			4

#define DEF_MAXWAYPOINTS			10

#define DEF_MOVETYPE_STOP			0		// 정지해 있는다. 초기화된 NPC의 기본 행동.
#define DEF_MOVETYPE_SEQWAYPOINT	1		// 이동경로를 순차적으로 이동. 맨 마지막 이동경로에서 해당 행동을 계속한다.
#define DEF_MOVETYPE_RANDOMWAYPOINT	2		// 이동경로를 랜덤하게 선택하여 움직인다.
#define DEF_MOVETYPE_FOLLOW			3		// 특정 대상을 따라 움직인다. 
#define DEF_MOVETYPE_RANDOMAREA		4		// 지정된 영역 안에서 랜덤한 곳으로 움직인다.
#define DEF_MOVETYPE_RANDOM			5		// 맵 내의 아무영역으로나 움직인다.
#define DEF_MOVETYPE_GUARD			6		// 처음 생성될 당시의 위치에 정지해 있으나 목표물 발견시는 추격한다.

#define DEF_BEHAVIOR_STOP			0
#define DEF_BEHAVIOR_MOVE			1
#define DEF_BEHAVIOR_ATTACK			2
#define DEF_BEHAVIOR_FLEE			3
#define DEF_BEHAVIOR_DEAD			4

#define DEF_QUESTTYPE_MONSTERHUNT				1		// 일반적인 몬스터 헌팅 
#define DEF_QUESTTYPE_MONSTERHUNT_TIMELIMIT		2		// 시간 제한이 걸려있는 몬스터 헌팅 
#define DEF_QUESTTYPE_ASSASSINATION 			3		// 암살 
#define DEF_QUESTTYPE_DELIVERY					4		// 배달: 특정 장소로 아이템을 배달한다.
#define DEF_QUESTTYPE_ESCORT					5		// 보호: 특정 캐릭터를 보호한다.
#define DEF_QUESTTYPE_GUARD						6		// 특정 지역을 적의 공격으로부터 방어한다. 
#define DEF_QUESTTYPE_GOPLACE					7		// 특정 장소로 간다. 적국 침투 
#define DEF_QUESTTYPE_BUILDSTRUCTURE			8		// 구조물 설치 임무
#define DEF_QUESTTYPE_SUPPLYBUILDSTRUCTURE		9		// 구조물 설치 보급 임무
#define DEF_QUESTTYPE_STRATEGICSTRIKE			10		// 전술적 폭격임무 
#define DEF_QUESTTYPE_SENDTOBATTLE				11		// 곧바로 교전장소 참가하는 임무
#define DEF_QUESTTYPE_SETOCCUPYFLAG				12		// 영토 점령 깃발을 설치하는 임무 


#define DEF_SKILLEFFECTTYPE_GET			    1		// 얻는다. 물고기나 광물등 
#define DEF_SKILLEFFECTTYPE_PRETEND			2		// ...한 체 하다 
#define DEF_SKILLEFFECTTYPE_TAMING			3		//  길들이기 계열	


#define WM_USER_STARTGAMESIGNAL	WM_USER + 10

#define WM_USER_ACCEPT			WM_USER + 100
#define WM_USER_BOT_ACCEPT		WM_USER + 300
#define WM_ONLOGSOCKETEVENT		WM_USER + 200
#define WM_ONGATESOCKETEVENT	WM_USER + 1000
#define WM_ONBOTSOCKETEVENT		WM_USER + 2000

// 클라이언트가 1100번 이하를 점유하므로 이 이후로 메시지가 정의되어서는 안된다. 
#define WM_ONCLIENTSOCKETEVENT	WM_USER + 1100


#define DEF_UPPERVERSION		"3"
#define DEF_LOWERVERSION 		"00"



// Those versions are hidden the client version
// they are used to change some MSGID (Anti-hgack)
#define DEF_HIDDEN_LOWERVERSION		95

#define DEF_VERSIONPLUSMODIFIER     1+(DEF_HIDDEN_LOWERVERSION+33 % 9+3) 
//that can be whatever number you wish, 1 byte-sized numbers will work fine

//NB: The version check is with the MainLog server, not the gserver!

#define DEF_XSOCK_LISTENSOCK			1
#define DEF_XSOCK_NORMALSOCK			2				
#define DEF_XSOCK_SHUTDOWNEDSOCK		3				

#define DEF_XSOCKSTATUS_READINGHEADER	11
#define DEF_XSOCKSTATUS_READINGBODY		12

#define DEF_XSOCKEVENT_SOCKETMISMATCH			-121	// 소켓 이벤트와 소켓이 서로 다르다. (이런 경우가?)
#define DEF_XSOCKEVENT_CONNECTIONESTABLISH		-122	// 접속이 이루어 졌다.
#define DEF_XSOCKEVENT_RETRYINGCONNECTION		-123	// 접속을 다시 시도하는 중이다.
#define DEF_XSOCKEVENT_ONREAD					-124	// 메시지를 읽는 중이다. 
#define DEF_XSOCKEVENT_READCOMPLETE				-125	// 하나의 메시지를 완전히 수신했다.
#define DEF_XSOCKEVENT_UNKNOWN					-126	// 알수없는 이벤트이다.
#define DEF_XSOCKEVENT_SOCKETCLOSED				-127	// 소켓이 닫혔다. 
#define DEF_XSOCKEVENT_BLOCK					-128	// 메시지를 보내다가 소켓이 블럭된 상태이다.
#define DEF_XSOCKEVENT_SOCKETERROR				-129	// 소켓에 에러가 발생했다. 이경우 클래스를 삭제하여야 한다.
#define DEF_XSOCKEVENT_CRITICALERROR			-130    // 치명적인 에러로 프로그램 전체가 멈춰야 한다.
#define DEF_XSOCKEVENT_NOTINITIALIZED			-131	// 클래스가 초기화 되지 않은 상태로 사용됐다.
#define DEF_XSOCKEVENT_MSGSIZETOOLARGE			-132	// 보내고자 하는 메시지의 사이즈가 너무 크다.
#define DEF_XSOCKEVENT_CONFIRMCODENOTMATCH		-133	// 확인코드가 일치하지 않는다. 삭제되어야 한다.
#define DEF_XSOCKEVENT_QUENEFULL                -134    // 블록 큐의 공간이 없다.
#define DEF_XSOCKEVENT_UNSENTDATASENDBLOCK		-135    // 큐에 있는 데이터를 보내다가 또 블록이 걸렸다. 
#define DEF_XSOCKEVENT_UNSENTDATASENDCOMPLETE	-136	// 큐에 있는 모든 데이터를 보냈다.

#define DEF_XSOCKBLOCKLIMIT						300		// 이 클래스 소켓은 ?번이상 블록되면 더이상 데이터를 저장할 수 없다. 	