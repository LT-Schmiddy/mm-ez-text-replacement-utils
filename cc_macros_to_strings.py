# I used this to create msg_control_codes.c. Keeping it around in case I need it later.

macros = """
#define EZTR_CC_COLOR_DEFAULT 00
#define EZTR_CC_COLOR_RED 01
#define EZTR_CC_COLOR_GREEN 02
#define EZTR_CC_COLOR_BLUE 03
#define EZTR_CC_COLOR_YELLOW 04
#define EZTR_CC_COLOR_LIGHTBLUE 05
#define EZTR_CC_COLOR_PINK 06
#define EZTR_CC_COLOR_SILVER 07
#define EZTR_CC_COLOR_ORANGE 08
#define EZTR_CC_TEXT_SPEED 0A
#define EZTR_CC_HS_BOAT_ARCHERY 0B
#define EZTR_CC_STRAY_FAIRIES 0C
#define EZTR_CC_TOKENS 0D
#define EZTR_CC_POINTS_TENS 0E
#define EZTR_CC_POINTS_THOUSANDS 0F
#define EZTR_CC_BOX_BREAK 10
#define EZTR_CC_NEWLINE 11
#define EZTR_CC_BOX_BREAK2 12
#define EZTR_CC_CARRIAGE_RETURN 13
#define EZTR_CC_SHIFT 14
#define EZTR_CC_CONTINUE 15
#define EZTR_CC_NAME 16
#define EZTR_CC_QUICKTEXT_ENABLE 17
#define EZTR_CC_QUICKTEXT_DISABLE 18
#define EZTR_CC_EVENT 19
#define EZTR_CC_PERSISTENT 1A
#define EZTR_CC_BOX_BREAK_DELAYED 1B
#define EZTR_CC_FADE 1C
#define EZTR_CC_FADE_SKIPPABLE 1D
#define EZTR_CC_SFX 1E
#define EZTR_CC_DELAY 1F
#define EZTR_CC_BTN_A B0
#define EZTR_CC_BTN_B B1
#define EZTR_CC_BTN_C B2
#define EZTR_CC_BTN_L B3
#define EZTR_CC_BTN_R B4
#define EZTR_CC_BTN_Z B5
#define EZTR_CC_BTN_CUP B6
#define EZTR_CC_BTN_CDOWN B7
#define EZTR_CC_BTN_CLEFT B8
#define EZTR_CC_BTN_CRIGHT B9
#define EZTR_CC_Z_TARGET BA
#define EZTR_CC_CONTROL_PAD BB
#define EZTR_CC_END BF
#define EZTR_CC_BACKGROUND C1
#define EZTR_CC_TWO_CHOICE C2
#define EZTR_CC_THREE_CHOICE C3
#define EZTR_CC_TIMER_POSTMAN C4
#define EZTR_CC_TIMER_MINIGAME_1 C5
#define EZTR_CC_TIMER_2 C6
#define EZTR_CC_TIMER_MOON_CRASH C7
#define EZTR_CC_TIMER_MINIGAME_2 C8
#define EZTR_CC_TIMER_ENV_HAZARD C9
#define EZTR_CC_TIME CA
#define EZTR_CC_CHEST_FLAGS CB
#define EZTR_CC_INPUT_BANK CC
#define EZTR_CC_RUPEES_SELECTED CD
#define EZTR_CC_RUPEES_TOTAL CE
#define EZTR_CC_TIME_UNTIL_MOON_CRASH CF
#define EZTR_CC_INPUT_DOGGY_RACETRACK_BET D0
#define EZTR_CC_INPUT_BOMBER_CODE D1
#define EZTR_CC_PAUSE_MENU D2
#define EZTR_CC_TIME_SPEED D3
#define EZTR_CC_OWL_WARP D4
#define EZTR_CC_INPUT_LOTTERY_CODE D5
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE D6
#define EZTR_CC_STRAY_FAIRIES_LEFT_WOODFALL D7
#define EZTR_CC_STRAY_FAIRIES_LEFT_SNOWHEAD D8
#define EZTR_CC_STRAY_FAIRIES_LEFT_GREAT_BAY D9
#define EZTR_CC_STRAY_FAIRIES_LEFT_STONE_TOWER DA
#define EZTR_CC_POINTS_BOAT_ARCHERY DB
#define EZTR_CC_LOTTERY_CODE DC
#define EZTR_CC_LOTTERY_CODE_GUESS DD
#define EZTR_CC_HELD_ITEM_PRICE DE
#define EZTR_CC_BOMBER_CODE DF
#define EZTR_CC_EVENT2 E0
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_1 E1
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_2 E2
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_3 E3
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_4 E4
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_5 E5
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_6 E6
#define EZTR_CC_HOURS_UNTIL_MOON_CRASH E7
#define EZTR_CC_TIME_UNTIL_NEW_DAY E8
#define EZTR_CC_HS_POINTS_BANK_RUPEES F0
#define EZTR_CC_HS_POINTS_UNK_1 F1
#define EZTR_CC_HS_POINTS_FISHING F2
#define EZTR_CC_HS_TIME_BOAT_ARCHERY F3
#define EZTR_CC_HS_TIME_HORSE_BACK_BALLOON F4
#define EZTR_CC_HS_TIME_LOTTERY_GUESS F5
#define EZTR_CC_HS_TOWN_SHOOTING_GALLERY F6
#define EZTR_CC_HS_UNK_1 F7
#define EZTR_CC_HS_UNK_3_LOWER F8
#define EZTR_CC_HS_HORSE_BACK_BALLOON F9
#define EZTR_CC_HS_DEKU_PLAYGROUND_DAY_1 FA
#define EZTR_CC_HS_DEKU_PLAYGROUND_DAY_2 FB
#define EZTR_CC_HS_DEKU_PLAYGROUND_DAY_3 FC
#define EZTR_CC_DEKU_PLAYGROUND_NAME_DAY_1 FD
#define EZTR_CC_DEKU_PLAYGROUND_NAME_DAY_2 FE
#define EZTR_CC_DEKU_PLAYGROUND_NAME_DAY_3 FF
""".strip()

desc_table = """
2000	00	Following Text Color: Default	Default is usually white, but may be black (ie: for notebook updates).	
2001	01	Following Text Color: Red		
2002	02	Following Text Color: Green		
2003	03	Following Text Color: Blue		
2004	04	Following Text Color: Yellow		
2005	05	Following Text Color: Turquoise		
2006	06	Following Text Color: Pink		
2007	07	Following Text Color: Silver		
2008	08	Following Text Color: Orange		
[?]	09	[Crash]		-
[?]	0A	Slows down text (not used)	Text normally prints 2 letters at a time. 0A acts as a null character. So 0A0A prints nothing not even a space when normally 2 letters are printed.	
0310	0B	Print: Hits Required to Win Jungle Cruise Reward		
021C	0C	Print: Stray Fairies Collected in Current Dungeon	Japanese version adds "人目".	
021D	0D	Print: Gold Skulltulas Collected in Current Spider House	
[?]	0E	Print: 0		-
[?]	0F	Print: 0		-
0009	10	Box Break I	Used when four lines of text have been printed, but can technically be used anywhere. More robust than 12/000B? [?]	
000A	11	Line Break		
000B	12	Box Break II	Used when three lines of text have been printed. Usually preceded by a 13/000C character.	
000C	13	Reset Cursor Position to Start of Current Line	Used as a filler when there are fewer than four lines of text. Usually preceded by a newline when two lines of text have been printed.	
001F 00xx	14 xx	Print: xx Spaces		-
0240	15	Disable Text Skip I	Triangle box. Does not play sound.	
0100	16	Print: Player Name		
0101	17	Enable: Instantaneous Text		
0102	18	Disable: Instantaneous Text		
0103	19	Disable Text Skip II	Triangle box. Plays "Text Finished" sound.	
0104	1A	Disable Text Box Close	Used for shop item descriptions.	+
0110 xxxx	1B xxxx	Delay for xxxx Before Printing Remaining Text		
0111 xxxx	1C xxxx	Keep Text on Screen for xxxx Before Closing	Player can move around while text is displayed.	
0112 xxxx	1D xxxx	Delay for xxxx Before Ending Conversation		
0120 xxxx	1E xxxx	Play Sound Effect xxxx	See Sound Effects.	
0128 xxxx	1F xxxx	Delay for xxxx Before Resuming Text Flow		
0020	20	Space	Listed here because (J) seems to use a nonstandard encoding?	
0135	[?]	[?]	Seems to be unique to (J). (Msg: 2948, 3330)	+
037E	[?]	[?]	Seems to be unique to (J) 1.0 & 1.1. (Msg: 08CA)	+
[?]	BC	[Crash]	Outputs "s". [?]	-
[?]	BD	[Crash]	Outputs "h". [?]	-
[?]	BE	[Crash]	Outputs "o". [?]	-
0500	BF	End Marker		
0200	C0	Print: Broken Character [?]		-
0201	C1	Ocarina Song Failure	Draws red X across the text box on the screen & centers text vertically/pushes it out.	
0202	C2	Initialize Selection: Two Choices		
0203	C3	Initialize Selection: Three Choices		
0204	C4	Print: Postman's Counting Game Results		
0205	C5	Timer Value		-
0206	C6	Timer Value		-
0207	C7	Print: Remaining Time Till Moon Falls (Clock Tower Roof)		-
0208	C8	Print: Deku Scrub Playground Results		
0209	C9	Timer Value		-
020A	CA	Timer Value		-
020B	CB	Print: Shooting Gallery Minigame Results		
020C	CC	Display Prompt: Withdraw or Deposit Rupees		
020D	CD	Print: Number of Rupees Entered or Bet		
020E	CE	Print: Total Rupees in Bank or Won by Betting		
020F	CF	Print: Time Remaining in Hours & Minutes		
0220	D0	Display Prompt: Rupees to Bet		
0221	D1	Display Prompt: Bombers' Code		
0222	D2	Item Prompt	Delays closing the text box.	
0223	D3	[?]		-
0224	D4	Print: Song of Soaring Destination		
0225	D5	Display Prompt: Lottery Number		
0226	D6	Print: 123456	Each number's color is indicative of the Spider House mask code.	-
0227	D7	Print: Remaining Stray Fairies in Woodfall Temple		
0228	D8	Print: Remaining Stray Fairies in Snowhead Temple		
0229	D9	Print: Remaining Stray Fairies in Great Bay Temple		
022A	DA	Print: Remaining Stray Fairies in Stone Tower Temple		
022B	DB	Print: Jungle Cruise Minigame Results		
022C	DC	Print: Winning Lottery Numbers		
022D	DD	Print: Player's Lottery Numbers		
022E	DE	Print: Item Value in Rupees	Default: 51 Rupees	
022F	DF	Print: Bombers' Code		
0230	E0	End Conversation	Used exclusively for NPCs. Usually followed by a BF/0500 command.	
0231	E1	Print: Color of Oceanside Spider House Mask 1		
0232	E2	Print: Color of Oceanside Spider House Mask 2		
0233	E3	Print: Color of Oceanside Spider House Mask 3		
0234	E4	Print: Color of Oceanside Spider House Mask 4		
0235	E5	Print: Color of Oceanside Spider House Mask 5		
0236	E6	Print: Color of Oceanside Spider House Mask 6		
0237	E7	Print: Remaining Time Till Moon Falls		
0238	E8	Print: Remaining Time Till Morning in Hours & Minutes		
[?]	E9	Print: Broken Character & Large Gap [?]		-
[?]	EA	Print: Broken Character [?]		-
[?]	EB	Print: Broken Character [?]		-
[?]	EC	Print: Broken Character [?]		-
[?]	ED	Print: Broken Character [?]		-
[?]	EF	Print: Broken Character [?]		-
[?]	F0	Print: Total Rupees in Bank		-
[?]	F1	Print: 0		-
[?]	F2	Print: 0		-
[?]	F3	Print: 0"10'		-
[?]	F4	Print: :0"00'		-
[?]	F5	Print: Timer or Highscore in 00"00' Format [?]	SRAM Offset: 0xEF2	-
0306	F6	Print: Town Shooting Gallery Highscore		
[?]	F7	Print: 00'00"00		-
[?]	F8	Print: Magic Bean Price		 ?
0309	F9	Print: Epona Balloon Archery Highscore I			
030A	FA	Print: Deku Scrub Playground Highscore (Day 1)		
030B	FB	Print: Deku Scrub Playground Highscore (Day 2)		
030C	FC	Print: Deku Scrub Playground Highscore (Day 3)		
[?]	FD	Print: Broken Character [?]		-
[?]	FE	[?]		-
[?]	FF	[?]		-
[?]	FD	Print: Broken Character [?]		-
[?]	FE	[?]		-
[?]	FF	[?]		-
""".strip()

entries = {}

# Handling Macros
for row in macros.split("\n"):
    values = row.split(" ")
    name = values[1]
    hexv = int(values[2], 16)
    entries[hexv] = {"name": name}
    
# Get Descriptions:
for row in desc_table.split("\n"):
    values = row.split("\t")
    try:
        # print(f"{values[1].split(" ")[0]=}")
        hexv = int(values[1].split(" ")[0], 16)
    except ValueError as e: 
        print(f"// Skipping row '{values[1]}'...")
        continue
    
    if hexv in entries:
        entries[hexv]["brief"] = values[2]
        entries[hexv]["notes"] = values[3]
# print(entries)
# Printing Name Array
for i in range(0, 256):
    if i not in entries:
        print("NULL,")
    else:
        print(f"\"{entries[i]["name"]}\",")
        
print("=========================")     

#Printing Macros   
for i in range(0, 256):
    if i not in entries:
        continue
    else:
        try: 
            print(f"""/**
* @brief {entries[i]['brief']}
* 
* {entries[i]['notes']}
* 
*/""")
        except:
            pass
        
        print(f"#define {entries[i]["name"]} \"\\x{i:02x}\"\n")