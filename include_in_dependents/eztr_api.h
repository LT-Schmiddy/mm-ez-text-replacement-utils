#ifndef __EZTR_API__
#define __EZTR_API__

/*! \file eztr_api.h
    \brief The main header for EZTR
*/

#include "modding.h"
#include "global.h"

/**
 * @brief The mod id string for EZTR.
 * 
 * The `eztr_api.h` imports all the functions and events needed to for EZTR, 
 * so you probably won't need to use this directly.
 */
#define EZTR_MOD_ID_STR "MM_EZ_Text_Replacer_API"

#ifdef DOXYGEN
#define EZTR_IMPORT(func) func
#define EZTR_PACK_STRUCT 
#else
#define EZTR_IMPORT(func) RECOMP_IMPORT(EZTR_MOD_ID_STR, func)
#define EZTR_PACK_STRUCT __attribute__((packed))
#endif
#define EZTR_MSG_HIGHEST_ID 0x354C
/**
 * @brief The the full size of message buffer, in bytes.
 * 
 * Also equivalent to maximum length of buffer in single-byte characters (`char`).
 */
#define EZTR_MSG_BUFFER_SIZE 1280

/**
 * @brief The maximum length of buffer in two-byte characters (`wchar`).
 * 
 * Not really used, but included for parity with the base game.
 */
#define EZTR_MSG_BUFFER_WIDE_SIZE 640

/**
 * @brief The size of the message buffer's header region, in bytes.
 * 
 * Also servers as the starting index of the message buffer's content region.
 * 
 */
#define EZTR_MSG_HEADER_SIZE 11

/**
 * @brief The size of the message buffer's content region, in bytes.
 * 
 * Useful if you need to loop throught the buffer.
 */
 #define EZTR_MSG_CONTENT_SIZE 1269 // MESSAGE_BUFFER_SIZE - MESSAGE_HEADER_SIZE

#define EZTR_MSG_ENDING_CHAR '\xBF'
#define EZTR_PIPE_CHAR '|'

/**
 * @brief The message buffers type as defined in the Majora's Mask decompilation.
 * 
 * While you can edit messages by interacting with this struct directly, EZTR 
 * offers much better ways of editing messages.
 */
typedef union {
    char schar[EZTR_MSG_BUFFER_SIZE]; // msgBuf
    u16 wchar[EZTR_MSG_BUFFER_WIDE_SIZE];   // msgBufWide
    u64 force_structure_alignment_msg;
} EZTR_MsgRaw;


/**
 * @brief The message buffer, but with the header and content regions defined as seperate arrays.
 * 
 * The `content` member can be passed to the `EZTR_MsgSContent_` functions for use in text operations.
 * 
 */
typedef struct {
    char header[EZTR_MSG_HEADER_SIZE];
    char content[EZTR_MSG_CONTENT_SIZE];
} EZTR_MsgPartition;


/**
 * @brief The message buffer, with the header represented as its individual members.
 * 
 * When compiled, this struct is marked with the `packed` attribute to ensure that it's members align
 * to their proper locations within the buffer. 
 * 
 * The `padding` member represents a section of the buffer that is unused, and should thus be ignored.
 * 
 * The `content` member can be passed to the `EZTR_MsgSContent_` functions for use in text operations.
 * 
 */
typedef struct EZTR_PACK_STRUCT {
    u8 text_box_type; 
    u8 text_box_y_pos;
    u8 display_icon; 
    u16 next_message_id;
    u16 first_item_rupees;
    u16 second_item_rupees;
    u16 padding;
    char content[EZTR_MSG_CONTENT_SIZE];
} EZTR_MsgData;

/**
 * @brief The main message buffer type, and the primary struct for interacting with message data.
 * 
 * Each member of the union is a different way of representing the buffer in code.
 * 
 * * `raw` is the buffer as it exists in the Majora's Mask decomp.
 * * `partitions` is the buffer, seperated into it's two primary regions (header and content).
 * * `data` is the buffer, with it's header and content values listed as individual members.
 * 
 */
typedef union {
        EZTR_MsgRaw raw;
        EZTR_MsgPartition partitions;
        EZTR_MsgData data;
} EZTR_MsgBuffer;

/**
 * @brief 
 * 
 */
#define EZTR_CUSTOM_MSG_HANDLE_NAME(name) EZTR_CustomMsgHandle_##name

/**
 * @brief 
 * 
 */
#define EZTR_DEFINE_CUSTOM_MSG_HANDLE_NO_EXPORT(name) \
u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(u16* new_id) \
{ static u16 id; if (new_id != NULL) { id = *new_id; } return id; }

/**
 * @brief 
 * 
 */
#define EZTR_DEFINE_CUSTOM_MSG_HANDLE(name) RECOMP_EXPORT \
u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(u16* new_id) \
{ static u16 id; if (new_id != NULL) { id = *new_id; } return id; }

/**
 * @brief 
 * 
 */
#define EZTR_EXTERN_CUSTOM_MSG_HANDLE(name) u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(u16* new_id)


#define EZTR_IMPORT_CUSTOM_MSG_HANDLE(mod_str, name) RECOMP_IMPORT(mod_str, u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(u16* new_id))

/**
 * @brief 
 * 
 */
#define EZTR_GET_CUSTOM_MSG_ID_FROM_NAME(name) EZTR_CUSTOM_MSG_HANDLE_NAME(name)(NULL)

/**
 * @brief 
 * 
 */
#define EZTR_GET_CUSTOM_MSG_ID(handle) handle(NULL)

// Shorthand:
#define EZTR_HANDLE_NAME(handle) EZTR_CUSTOM_MSG_HANDLE_NAME(handle)
#define EZTR_GET_ID(handle) EZTR_GET_CUSTOM_MSG_ID(handle)
#define EZTR_GET_ID_N(handle) EZTR_GET_CUSTOM_MSG_ID_FROM_NAME(handle)

// Type of the custom message handle:
typedef u16 (*EZTR_CustomMsgHandle)(u16* new_id);

/**
 * @brief The function pointer type for message callbacks. 
 * 
 * To easily create functions that match this type, see the `EZTR_MSG_CALLBACK` macro.
 * 
 */
typedef void (*EZTR_MsgCallback)(EZTR_MsgBuffer* buf, u16 textId, PlayState* play);

/**
 * @brief A macro to easily create message callback functions.
 * 
 * This macro can be used to create both the function definition and declaration (if one is needed);
 * 
 * * To create the definition, use `EZTR_MSG_CALLBACK(my_callback) {...}`
 * * To create the declaration, use `EZTR_MSG_CALLBACK(my_callback);`
 * 
 * `my_callback` can then be passed to `EZTR_Basic_ReplaceText_WithCallback()` or 
 * `EZTR_Basic_ReplaceText_EmptyWithCallback()` as the callback argument.
 * 
 */
#define EZTR_MSG_CALLBACK(fname) void fname(EZTR_MsgBuffer* buf, u16 textId, PlayState* play)

/**
 * @brief Used to declare a function that should run after EZTR has finished initializing.
 * 
 * This is where you should declare all of your text replacements. You don't want to declare them during a 
 * `recomp_on_init` event, since EZTR may not have initialized itself yet, and attempting to declare text
 * replacements before that will cause a crash. Additionally, declaring messaged here will ensure that mod 
 * priority order is respected when declaring replacements.
 * 
 * Example: `EZTR_ON_INIT void declare_my_text() {...}`
 * 
 */
#define EZTR_ON_INIT RECOMP_CALLBACK("MM_EZ_Text_Replacer_API", EZTR_OnInit)

/**
 * @brief Used by certain members of `EZTR_MsgData` (and the message header generally) to indicate that said member is not in use.
 * 
 * The header members in question are:
 * 
 * * `next_message_id`
 * * `first_item_rupees`
 * * `second_item_rupees`
 * 
 */
#define EZTR_NO_VALUE 0xffff


/**
 * @brief Used in the message header to indicate the style of textbox used for the message.
 * 
 * You can set the text box type by assigning to the `text_box_type` member of EZTR_MsgData,
 * or by using `EZTR_MsgBuffer_SetTextBoxType()`.
 * 
 */
typedef enum {
    EZTR_STANDARD_TEXT_BOX_I = 0X00,
    EZTR_WOODEN_SIGN_BACKGROUND = 0X01,
    EZTR_TRANSLUSCENT_BLUE_TEXT_BOX = 0X02,
    EZTR_OCARINA_STAFF = 0X03,
    EZTR_INVISIBLE_TEXT_BOX_I = 0X04,
    EZTR_INVISIBLE_TEXT_BOX_II = 0X05,
    EZTR_STANDARD_TEXT_BOX_II = 0X06,
    EZTR_INVISIBLE_TEXT_BOX = 0X07,
    EZTR_BLUE_TEXT_BOX = 0X08,
    EZTR_RED_TEXT_BOX_I = 0X09,
    EZTR_INVISIBLE_TEXT_BOX_III = 0X0A,
    EZTR_INVISIBLE_TEXT_BOX_IV = 0X0B,
    EZTR_INVISIBLE_TEXT_BOX_V = 0X0C,
    EZTR_BOMBERS_NOTEBOOK = 0X0D,
    EZTR_INVISIBLE_TEXT_BOX_VI = 0X0E,
    EZTR_RED_TEXT_BOX_II = 0X0F
} EZTR_TextBoxType;

/**
 * @brief Used in the message header to indicate a display icon for the message.
 * 
 * You can set the display icon by assigning to the `display_icon` member of EZTR_MsgData,
 * or by using `EZTR_MsgBuffer_SetTextBoxDIsplayIcon()`.
 * 
 * Note that the value for not displaying an icon is `EZTR_ICON_NO_ICON`, and NOT 
 * 
 * `EZTR_ICON_NOTHING` or it's variants.
 */
typedef enum {
    EZTR_ICON_NOTHING = 0x00,
    EZTR_ICON_GREEN_RUPEE = 0x01,
    EZTR_ICON_BLUE_RUPEE = 0x02,
    EZTR_ICON_WHITE_RUPEE = 0x03,
    EZTR_ICON_RED_RUPEE = 0x04,
    EZTR_ICON_PURPLE_RUPEE = 0x05,
    EZTR_ICON_WHITE_RUPEE_1 = 0x06,
    EZTR_ICON_ORANGE_RUPEE = 0x07,
    EZTR_ICON_ADULT_WALLET = 0x08,
    EZTR_ICON_GIANTS_WALLET = 0x09,
    EZTR_ICON_RECOVERY_HEART = 0x0A,
    EZTR_ICON_RECOVERY_HEART_1 = 0x0B,
    EZTR_ICON_PIECE_OF_HEART = 0x0C,
    EZTR_ICON_HEART_CONTAINER = 0x0D,
    EZTR_ICON_SMALL_MAGIC_JAR = 0x0E,
    EZTR_ICON_LARGE_MAGIC_JAR = 0x0F,
    EZTR_ICON_RECOVERY_HEART_2 = 0x10,
    EZTR_ICON_STRAY_FAIRY = 0x11,
    EZTR_ICON_RECOVERY_HEART_3 = 0x12,
    EZTR_ICON_RECOVERY_HEART_4 = 0x13,
    EZTR_ICON_BOMB = 0x14,
    EZTR_ICON_BOMB_1 = 0x15,
    EZTR_ICON_BOMB_2 = 0x16,
    EZTR_ICON_BOMB_3 = 0x17,
    EZTR_ICON_BOMB_4 = 0x18,
    EZTR_ICON_DEKU_STICK = 0x19,
    EZTR_ICON_BOMBCHU = 0x1A,
    EZTR_ICON_BOMB_BAG = 0x1B,
    EZTR_ICON_BIG_BOMB_BAG = 0x1C,
    EZTR_ICON_BIGGER_BOMB_BAG = 0x1D,
    EZTR_ICON_HEROS_BOW = 0x1E,
    EZTR_ICON_HEROS_BOW_1 = 0x1F,
    EZTR_ICON_HEROS_BOW_2 = 0x20,
    EZTR_ICON_HEROS_BOW_3 = 0x21,
    EZTR_ICON_QUIVER = 0x22,
    EZTR_ICON_BIG_QUIVER = 0x23,
    EZTR_ICON_BIGGEST_QUIVER = 0x24,
    EZTR_ICON_FIRE_ARROW = 0x25,
    EZTR_ICON_ICE_ARROW = 0x26,
    EZTR_ICON_LIGHT_ARROW = 0x27,
    EZTR_ICON_DEKU_NUT = 0x28,
    EZTR_ICON_DEKU_NUT_1 = 0x29,
    EZTR_ICON_DEKU_NUT_2 = 0x2A,
    EZTR_ICON_NOTHING_1 = 0x2B,
    EZTR_ICON_NOTHING_2 = 0x2C,
    EZTR_ICON_NOTHING_3 = 0x2D,
    EZTR_ICON_NOTHING_4 = 0x2E,
    EZTR_ICON_NOTHING_5 = 0x2F,
    EZTR_ICON_NOTHING_6 = 0x30,
    EZTR_ICON_NOTHING_7 = 0x31,
    EZTR_ICON_HEROS_SHIELD = 0x32,
    EZTR_ICON_MIRROR_SHIELD = 0x33,
    EZTR_ICON_POWDER_KEG = 0x34,
    EZTR_ICON_MAGIC_BEAN = 0x35,
    EZTR_ICON_PICTOGRAPH_BOX = 0x36,
    EZTR_ICON_KOKIRI_SWORD = 0x37,
    EZTR_ICON_RAZOR_SWORD = 0x38,
    EZTR_ICON_GILDED_SWORD = 0x39,
    EZTR_ICON_FIERCE_DEITYS_SWORD = 0x3A,
    EZTR_ICON_GREAT_FAIRYS_SWORD = 0x3B,
    EZTR_ICON_SMALL_KEY = 0x3C,
    EZTR_ICON_BOSS_KEY = 0x3D,
    EZTR_ICON_DUNGEON_MAP = 0x3E,
    EZTR_ICON_COMPASS = 0x3F,
    EZTR_ICON_POWDER_KEG_1 = 0x40,
    EZTR_ICON_HOOKSHOT = 0x41,
    EZTR_ICON_LENS_OF_TRUTH = 0x42,
    EZTR_ICON_PICTOGRAPH_BOX_1 = 0x43,
    EZTR_ICON_FISHING_ROD = 0x44,
    EZTR_ICON_NOTHING_8 = 0x45,
    EZTR_ICON_NOTHING_9 = 0x46,
    EZTR_ICON_NOTHING_10 = 0x47,
    EZTR_ICON_NOTHING_11 = 0x48,
    EZTR_ICON_NOTHING_12 = 0x49,
    EZTR_ICON_NOTHING_13 = 0x4A,
    EZTR_ICON_NOTHING_14 = 0x4B,
    EZTR_ICON_OCARINA_OF_TIME = 0x4C,
    EZTR_ICON_NOTHING_15 = 0x4D,
    EZTR_ICON_NOTHING_16 = 0x4E,
    EZTR_ICON_NOTHING_17 = 0x4F,
    EZTR_ICON_BOMBERS_NOTEBOOK = 0x50,
    EZTR_ICON_NOTHING_18 = 0x51,
    EZTR_ICON_GOLD_SKULLTULA_TOKEN = 0x52,
    EZTR_ICON_NOTHING_19 = 0x53,
    EZTR_ICON_NOTHING_20 = 0x54,
    EZTR_ICON_ODOLWAS_REMAINS = 0x55,
    EZTR_ICON_GOHTS_REMAINS = 0x56,
    EZTR_ICON_GYORGS_REMAINS = 0x57,
    EZTR_ICON_TWINMOLDS_REMAINS = 0x58,
    EZTR_ICON_RED_POTION = 0x59,
    EZTR_ICON_EMPTY_BOTTLE = 0x5A,
    EZTR_ICON_RED_POTION_1 = 0x5B,
    EZTR_ICON_GREEN_POTION = 0x5C,
    EZTR_ICON_BLUE_POTION = 0x5D,
    EZTR_ICON_FAIRYS_SPIRIT = 0x5E,
    EZTR_ICON_DEKU_PRINCESS = 0x5F,
    EZTR_ICON_MILK = 0x60,
    EZTR_ICON_MILK_HALF = 0x61,
    EZTR_ICON_FISH = 0x62,
    EZTR_ICON_BUG = 0x63,
    EZTR_ICON_BLUE_FIRE = 0x64,
    EZTR_ICON_POE = 0x65,
    EZTR_ICON_BIG_POE = 0x66,
    EZTR_ICON_SPRING_WATER = 0x67,
    EZTR_ICON_HOT_SPRING_WATER = 0x68,
    EZTR_ICON_ZORA_EGG = 0x69,
    EZTR_ICON_GOLD_DUST = 0x6A,
    EZTR_ICON_MUSHROOM = 0x6B,
    EZTR_ICON_NOTHING_21 = 0x6C,
    EZTR_ICON_NOTHING_22 = 0x6D,
    EZTR_ICON_SEAHORSE = 0x6E,
    EZTR_ICON_CHATEAU_ROMANI = 0x6F,
    EZTR_ICON_HYLIAN_LOACH = 0x70,
    EZTR_ICON_NOTHING_23 = 0x71,
    EZTR_ICON_NOTHING_24 = 0x72,
    EZTR_ICON_NOTHING_25 = 0x73,
    EZTR_ICON_NOTHING_26 = 0x74,
    EZTR_ICON_NOTHING_27 = 0x75,
    EZTR_ICON_NOTHING_28 = 0x76,
    EZTR_ICON_NOTHING_29 = 0x77,
    EZTR_ICON_DEKU_MASK = 0x78,
    EZTR_ICON_GORON_MASK = 0x79,
    EZTR_ICON_ZORA_MASK = 0x7A,
    EZTR_ICON_FIERCE_DEITY_MASK = 0x7B,
    EZTR_ICON_MASK_OF_TRUTH = 0x7C,
    EZTR_ICON_KAFEIS_MASK = 0x7D,
    EZTR_ICON_ALL_NIGHT_MASK = 0x7E,
    EZTR_ICON_BUNNY_HOOD = 0x7F,
    EZTR_ICON_KEATON_MASK = 0x80,
    EZTR_ICON_GARO_MASK = 0x81,
    EZTR_ICON_ROMANI_MASK = 0x82,
    EZTR_ICON_CIRCUS_LEADERS_MASK = 0x83,
    EZTR_ICON_POSTMANS_HAT = 0x84,
    EZTR_ICON_COUPLES_MASK = 0x85,
    EZTR_ICON_GREAT_FAIRYS_MASK = 0x86,
    EZTR_ICON_GIBDO_MASK = 0x87,
    EZTR_ICON_DON_GEROS_MASK = 0x88,
    EZTR_ICON_KAMAROS_MASK = 0x89,
    EZTR_ICON_CAPTAINS_HAT = 0x8A,
    EZTR_ICON_STONE_MASK = 0x8B,
    EZTR_ICON_BREMEN_MASK = 0x8C,
    EZTR_ICON_BLAST_MASK = 0x8D,
    EZTR_ICON_MASK_OF_SCENTS = 0x8E,
    EZTR_ICON_GIANTS_MASK = 0x8F,
    EZTR_ICON_NOTHING_30 = 0x90,
    EZTR_ICON_CHATEAU_ROMANI_1 = 0x91,
    EZTR_ICON_MILK_1 = 0x92,
    EZTR_ICON_GOLD_DUST_1 = 0x93,
    EZTR_ICON_HYLIAN_LOACH_1 = 0x94,
    EZTR_ICON_SEAHORSE_1 = 0x95,
    EZTR_ICON_MOONS_TEAR = 0x96,
    EZTR_ICON_TOWN_TITLE_DEED = 0x97,
    EZTR_ICON_SWAMP_TITLE_DEED = 0x98,
    EZTR_ICON_MOUNTAIN_TITLE_DEED = 0x99,
    EZTR_ICON_OCEAN_TITLE_DEED = 0x9A,
    EZTR_ICON_NOTHING_31 = 0x9B,
    EZTR_ICON_NOTHING_32 = 0x9C,
    EZTR_ICON_NOTHING_33 = 0x9D,
    EZTR_ICON_NOTHING_34 = 0x9E,
    EZTR_ICON_NOTHING_35 = 0x9F,
    EZTR_ICON_ROOM_KEY = 0xA0,
    EZTR_ICON_SPECIAL_DELIVERY_TO_MAMA = 0xA1,
    EZTR_ICON_NOTHING_36 = 0xA2,
    EZTR_ICON_NOTHING_37 = 0xA3,
    EZTR_ICON_NOTHING_38 = 0xA4,
    EZTR_ICON_NOTHING_39 = 0xA5,
    EZTR_ICON_NOTHING_40 = 0xA6,
    EZTR_ICON_NOTHING_41 = 0xA7,
    EZTR_ICON_NOTHING_42 = 0xA8,
    EZTR_ICON_NOTHING_43 = 0xA9,
    EZTR_ICON_LETTER_TO_KAFEI = 0xAA,
    EZTR_ICON_PENDANT_OF_MEMORIES = 0xAB,
    EZTR_ICON_NOTHING_44 = 0xAC,
    EZTR_ICON_NOTHING_45 = 0xAD,
    EZTR_ICON_NOTHING_46 = 0xAE,
    EZTR_ICON_NOTHING_47 = 0xAF,
    EZTR_ICON_NOTHING_48 = 0xB0,
    EZTR_ICON_NOTHING_49 = 0xB1,
    EZTR_ICON_NOTHING_50 = 0xB2,
    EZTR_ICON_TINGLES_MAP = 0xB3,
    EZTR_ICON_TINGLES_MAP_1 = 0xB4,
    EZTR_ICON_TINGLES_MAP_2 = 0xB5,
    EZTR_ICON_TINGLES_MAP_3 = 0xB6,
    EZTR_ICON_TINGLES_MAP_4 = 0xB7,
    EZTR_ICON_TINGLES_MAP_5 = 0xB8,
    EZTR_ICON_TINGLES_MAP_6 = 0xB9,
    EZTR_ICON_NOTHING_51 = 0xBA,
    EZTR_ICON_NOTHING_52 = 0xBB,
    EZTR_ICON_NOTHING_53 = 0xBC,
    EZTR_ICON_NOTHING_54 = 0xBD,
    EZTR_ICON_NOTHING_55 = 0xBE,
    EZTR_ICON_NOTHING_56 = 0xBF,
    EZTR_ICON_NOTHING_57 = 0xC0,
    EZTR_ICON_NOTHING_58 = 0xC1,
    EZTR_ICON_NOTHING_59 = 0xC2,
    EZTR_ICON_NOTHING_60 = 0xC3,
    EZTR_ICON_NOTHING_61 = 0xC4,
    EZTR_ICON_NOTHING_62 = 0xC5,
    EZTR_ICON_NOTHING_63 = 0xC6,
    EZTR_ICON_NOTHING_64 = 0xC7,
    EZTR_ICON_NOTHING_65 = 0xC8,
    EZTR_ICON_NOTHING_66 = 0xC9,
    EZTR_ICON_NOTHING_67 = 0xCA,
    EZTR_ICON_NOTHING_68 = 0xCB,
    EZTR_ICON_NOTHING_69 = 0xCC,
    EZTR_ICON_NOTHING_70 = 0xCD,
    EZTR_ICON_NOTHING_71 = 0xCE,
    EZTR_ICON_NOTHING_72 = 0xCF,
    EZTR_ICON_NOTHING_73 = 0xD0,
    EZTR_ICON_NOTHING_74 = 0xD1,
    EZTR_ICON_NOTHING_75 = 0xD2,
    EZTR_ICON_NOTHING_76 = 0xD3,
    EZTR_ICON_NOTHING_77 = 0xD4,
    EZTR_ICON_NOTHING_78 = 0xD5,
    EZTR_ICON_NOTHING_79 = 0xD6,
    EZTR_ICON_NOTHING_80 = 0xD7,
    EZTR_ICON_SMALL_BLACK_LINE = 0xD8,
    EZTR_ICON_SMALL_BLACK_LINE_1 = 0xD9,
    EZTR_ICON_SMALL_BLACK_LINE_2 = 0xDA,
    EZTR_ICON_SMALL_BLACK_LINE_3 = 0xDB,
    EZTR_ICON_ANJU = 0xDC,
    EZTR_ICON_KAFEI = 0xDD,
    EZTR_ICON_CURIOSITY_SHOP_OWNER = 0xDE,
    EZTR_ICON_BOMB_SHOP_OWNERS_MOTHER = 0xDF,
    EZTR_ICON_ROMANI = 0xE0,
    EZTR_ICON_CREMIA = 0xE1,
    EZTR_ICON_MAYOR_DOTOUR = 0xE2,
    EZTR_ICON_MADAME_AROMA = 0xE3,
    EZTR_ICON_TOTO = 0xE4,
    EZTR_ICON_GORMAN = 0xE5,
    EZTR_ICON_POSTMAN = 0xE6,
    EZTR_ICON_ROSA_SISTERS = 0xE7,
    EZTR_ICON_TOILET_HAND = 0xE8,
    EZTR_ICON_GRANNY = 0xE9,
    EZTR_ICON_KAMARO = 0xEA,
    EZTR_ICON_GROG = 0xEB,
    EZTR_ICON_GORMAN_BROTHERS = 0xEC,
    EZTR_ICON_SHIRO = 0xED,
    EZTR_ICON_GURU_GURU = 0xEE,
    EZTR_ICON_BOMBERS = 0xEF,
    EZTR_ICON_EXCLAMATION_MARK = 0xF0,
    EZTR_ICON_NOTHING_81 = 0xF1,
    EZTR_ICON_NOTHING_82 = 0xF2,
    EZTR_ICON_NOTHING_83 = 0xF3,
    EZTR_ICON_NOTHING_84 = 0xF4,
    EZTR_ICON_NOTHING_85 = 0xF5,
    EZTR_ICON_NOTHING_86 = 0xF6,
    EZTR_ICON_NOTHING_87 = 0xF7,
    EZTR_ICON_NOTHING_88 = 0xF8,
    EZTR_ICON_NOTHING_89 = 0xF9,
    EZTR_ICON_NOTHING_90 = 0xFA,
    EZTR_ICON_NOTHING_91 = 0xFB,
    EZTR_ICON_NOTHING_92 = 0xFC,
    EZTR_ICON_NOTHING_93 = 0xFD,
    EZTR_ICON_NO_ICON = 0xFE
} EZTR_TextBoxIcon;

/**
 * @brief This is the mose basic function for declaring replacement text.
 * 
 * @param textId 
 * @param text_box_type 
 * @param text_box_y_pos 
 * @param display_icon 
 * @param next_message_id 
 * @param first_item_rupees 
 * @param second_item_rupees 
 * @param pipe_escape_bytes 
 * @param content 
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceText(
    u16 textId, 
    u8 text_box_type, 
    u8 text_box_y_pos, 
    u8 display_icon, 
    u16 next_message_id, 
    u16 first_item_rupees, 
    u16 second_item_rupees, 
    bool pipe_escape_bytes, 
    char* content
));

/**
 * @brief 
 * 
 * @param textId 
 * @param text_box_type 
 * @param text_box_y_pos 
 * @param display_icon 
 * @param next_message_id 
 * @param first_item_rupees 
 * @param second_item_rupees 
 * @param pipe_escape_bytes 
 * @param content 
 * @param callback 
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceText_WithCallback(
    u16 textId, 
    u8 text_box_type, 
    u8 text_box_y_pos, 
    u8 display_icon, 
    u16 next_message_id, 
    u16 first_item_rupees, 
    u16 second_item_rupees, 
    bool pipe_escape_bytes, 
    char* content,
    EZTR_MsgCallback callback
));

/**
 * @brief 
 * 
 * @param textId 
 * @param callback 
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceText_EmptyWithCallback(u16 textId, EZTR_MsgCallback callback));

EZTR_IMPORT(void EZTR_Basic_CustomText_WithCallback(EZTR_CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, EZTR_MsgCallback callback));

EZTR_IMPORT(void EZTR_Basic_CustomText(EZTR_CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content));

EZTR_IMPORT(void EZTR_Basic_CustomText_EmptyWithCallback(EZTR_CustomMsgHandle handle, EZTR_MsgCallback callback));

/**
 * @brief Create msgBuffer
 * 
 * @return MsgBuffer* 
 */
EZTR_IMPORT(EZTR_MsgBuffer* EZTR_MsgBuffer_Create());

/**
 * @brief 
 * 
 * @param src 
 * @return MsgBuffer* 
 */
EZTR_IMPORT(EZTR_MsgBuffer* EZTR_MsgBuffer_CreateFromStr(char* src));

/**
 * @brief 
 * 
 * @param src 
 * @param len 
 * @return MsgBuffer* 
 */
EZTR_IMPORT(EZTR_MsgBuffer* EZTR_MsgBuffer_CreateFromStrN(char* src, size_t len));

/**
 * @brief 
 * 
 * @param buf 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_Destroy(EZTR_MsgBuffer* buf));

// Copy:
/**
 * @brief 
 * 
 * @param dst 
 * @param src 
 * @return u32 
 */
EZTR_IMPORT(u32 EZTR_MsgBuffer_Copy(char* dst, char* src));

/**
 * @brief 
 * 
 * @param dst 
 * @param src 
 * @param len 
 * @return u32 
 */
EZTR_IMPORT(u32 EZTR_MsgBuffer_NCopy(char* dst, char* src, size_t len));

/**
 * @brief 
 * 
 * @param buf 
 * @return u32 
 */
EZTR_IMPORT(u32 EZTR_MsgBuffer_Len(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 * @return u32 
 */
EZTR_IMPORT(u32 EZTR_MsgBuffer_ContentLen(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_WriteDefaultHeader(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 * @param text_box_type 
 * @param text_box_y_pos 
 * @param display_icon 
 * @param next_message_id 
 * @param first_item_rupees 
 * @param second_item_rupees 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_WriteHeader(EZTR_MsgBuffer* buf, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees));

    /**
 * @brief 
 * 
 * @param buf 
 * @return u8 
 */
EZTR_IMPORT(u8 EZTR_MsgBuffer_GetTextBoxType(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 * @param type 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetTextBoxType(EZTR_MsgBuffer* buf, u8 type));

/**
 * @brief 
 * 
 * @param buf 
 * @return u8 
 */
EZTR_IMPORT(u8 EZTR_MsgBuffer_GetTextBoxYPos(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 * @param pos 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetTextBoxYPos(EZTR_MsgBuffer* buf, u8 pos));

/**
 * @brief 
 * 
 * @param buf 
 * @return u8 
 */
EZTR_IMPORT(u8 EZTR_MsgBuffer_GetTextBoxDisplayIcon(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 * @param icon 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetTextBoxDisplayIcon(EZTR_MsgBuffer* buf, u8 icon));

/**
 * @brief 
 * 
 * @param buf 
 * @return u16 
 */
EZTR_IMPORT(u16 EZTR_MsgBuffer_GetNextMsg(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 * @param textId 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetNextMsg(EZTR_MsgBuffer* buf, u16 textId));

/**
 * @brief 
 * 
 * @param buf 
 * @return u16 
 */
EZTR_IMPORT(u16 EZTR_MsgBuffer_GetFirstItemRupees(EZTR_MsgBuffer* buf));
/**
 * @brief 
 * 
 * @param buf 
 * @param val 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetFirstItemRupees(EZTR_MsgBuffer* buf, u16 val));

/**
 * @brief 
 * 
 * @param buf 
 * @return u16 
 */
EZTR_IMPORT(u16 EZTR_MsgBuffer_GetSecondItemRupees(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 * @param val 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetSecondItemRupees(EZTR_MsgBuffer* buf, u16 val));

/**
 * @brief 
 * 
 * @param buf 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_Print(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 */
EZTR_IMPORT(void EZTR_MsgBuffer_PrintFull(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf 
 * @return char* 
 */
EZTR_IMPORT(char* EZTR_MsgBuffer_GetContentPtr(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param cont 
 */
EZTR_IMPORT(void EZTR_MsgSContent_SetEmpty(char* cont));

/**
 * @brief 
 * 
 * @param cont 
 * @return u32 
 */
EZTR_IMPORT(u32 EZTR_MsgSContent_Len(char* cont));

/**
 * @brief 
 * 
 * @param dst 
 * @param src 
 * @param len 
 * @return u32 
 */
EZTR_IMPORT(u32 EZTR_MsgSContent_NCopy(char* dst, char* src, size_t len));

/**
 * @brief 
 * 
 * @param dst 
 * @param src 
 * @return u32 
 */
EZTR_IMPORT(u32 EZTR_MsgSContent_Copy(char* dst, char* src));

/**
 * @brief 
 * 
 * @param dst 
 * @param src 
 * @param len 
 * @return u32 
 */
EZTR_IMPORT(char* EZTR_MsgSContent_NCat(char* dst, char* src, size_t len));
/**
 * @brief 
 * 
 * @param dst 
 * @param src 
 * @return char* 
 */
EZTR_IMPORT(char* EZTR_MsgSContent_Cat(char* dst, char* src));
/**
 * @brief 
 * 
 * @param str1 
 * @param str2 
 * @param len 
 * @return s32 
 */
EZTR_IMPORT(s32 EZTR_MsgSContent_NCmp(char* str1, char* str2, size_t len));
/**
 * @brief 
 * 
 * @param str1 
 * @param str2 
 * @return s32 
 */
EZTR_IMPORT(s32 EZTR_MsgSContent_Cmp(char* str1, char* str2));

/**
 * @brief 
 * 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Printf(const char* format, ...));

/**
 * @brief 
 * 
 * @param buffer 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Sprintf(char* buffer, const char* format, ...));

/**
 * @brief 
 * 
 * @param buffer 
 * @param count 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Snprintf(char* buffer, size_t count, const char* format, ...));

/**
 * @brief 
 * 
 * @param buffer 
 * @param count 
 * @param format 
 * @param va 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Vsnprintf(char* buffer, size_t count, const char* format, va_list va));

/**
 * @brief 
 * 
 * @param format 
 * @param va 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Vprintf(const char* format, va_list va));

/**
 * @brief 
 * 
 * @param out 
 * @param arg 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...));


#endif