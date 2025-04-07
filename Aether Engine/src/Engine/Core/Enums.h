#pragma once

//Skills & Scores
enum class Ab_Scores {
	STR,
	CON,
	AGI,
	DEX,
	INT,
	WIS,
	CHA
};

enum class A_Skills {
	APPRAISAL,
	ARCANA,
	BEAST_HANDLING,
	BEGUILEMENT,
	CLIMB,
	INSIGHT,
	INTIMIDATION,
	LORE,
	MECHANICS,
	MEDICINE,
	NATURE,
	NAVIGATION,
	PERCEPTION,
	PERSUASION,
	SOH,
	STEALTH,
	SURVIVAL,
	SWIM
};

enum class T_Skills {
	MED_KITS,
	THIEVING_KITS
};

enum class W_Skills {
	ARCHERY,
	FIREARM,
	LARGE,
	MED,
	POLEARM,
	SMALL,
	THROWING,
	UNARMED
};


//Mechanics
enum class Actions {
	DEFAULT,
	NOACTION, //Slashed circle
	ATTACK_M, //Sword
	ATTACK_R, //Gun? Bow?
	AOO,
	CHANGE_EQUIP,
	CLOSE, //Closed door
	DISENGAGE,
	EVADE,
	EXAMINE, //Eye
	FALL_PRONE,
	HELP,
	HIDE,
	JUMP,
	LOOT, //Treasure Chest
	MOVE, //Boot
	OPEN, //Open door
	PICKUP, //Hand
	SHOVE,
	STAND,
	TALK, //Mouth? Or speech bubble?
	THROW_ITEM
};

enum class Conditions {
	BLIND,
	CHARMED,
	DEAF,
	PRONE,
	SICK,
	STUNNED,
	UNCONSCIOUS
};

enum class Classes {
	NONE,
	ARCANIST,
	ROGUE,
	WARRIOR
};

enum class Genuses {
	BEAST,
	MONSTROSITY,
	OTHER,
	SCORCHED,
	SENTIENT,
	UNDEAD
};

enum class Items {

};

enum class PreGens {
	SPARK, //Automaton Arcanist
	ESSEK, //Female Kobold Rogue
	DAKN //Male Dwarf Warrior
};

enum class Races {
	AUTOMATON,
	DWARF,
	ELF,
	GNOME,
	HUMAN,
	KOBOLD
};

enum class Sizes {
	TINY,
	SMALL,
	MED,
	BIG,
	LARGE,
	HUGE,
	MASSIVE,
	COLOSSAL
};

enum class Terrains {
	NORMAL,
	ROUGH,
	WATER
};

enum class Weathers {
	CLEAR
};


//Game details
enum class Areas {
	DEFAULT,
	DEBUG,
	TUTTON
};

enum class Cutscenes {

};

enum class Interfaces {
	NONE,
	CHARSHEET, //C
	INVENTORY, //I
	JOURNAL,   //J
	MAP_AREA,  //M
	MAP_WORLD,
	OPTIONS    //O
};



//Engine
enum class Scenes {
	CUTSCENE,
	AREA,
	TITLE
};

enum class Menus {
	CHARCREA,
	GO,
	INVENTORY,
	LEVELUP,
	LOAD,
	MAIN,
	OPTIONS,
	OPTIONS_G,
	PAUSE,
	//Sub-Menus
	//CC
	CCAS,
	CCBG,
	CCCLASS,
	CCEQUIP,
	CCRACE,
	CCSKILLS
};

enum class UIElems {
	DEFAULT,
	CONFIRM, //For Confirm Boxes
	//Buttons
	APPLY,
	AS,
	BACK,
	BACKGROUND_B,
	CHARCREA,
	CLASS_B,
	CLOSE,
	CREATE,
	DEBUG_ROOM,
	EQUIPMENT_CC,
	FULLSCREEN,
	LOAD,
	NO,
	OPTIONS,
	QUIT,
	RACE_B,
	RESUME,
	SKILLS,
	TITLE,
	TUTORIAL,
	YES,
	//Pickers
	ASSTR,
	ASCON,
	ASAGI,
	ASDEX,
	ASINT,
	ASWIS,
	ASCHA,
	BACKGROUND_P,
	CLASS_P,
	RACE_P,
	RESOLUTION,
	SEX,
	SIZE,
	//Sliders
	CAMSPD,
	MUSIC_V,
	SFX_V
};