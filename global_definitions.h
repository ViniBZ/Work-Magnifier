#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H

#define WINDOW_TITLE "Work Magnifier 1.0"

//#define BASE_PATH "/Users/viniciusbaez/build-WorkMagnifier-Desktop_Qt_5_5_0_clang_64bit-Debug"
//#define BASE_PATH "build-WorkMagnifier-Desktop_Qt_5_5_0_clang_64bit-Debug/"
//#define BASE_PATH "../../../"
#define BASE_PATH "./Resources/"
#define LIB_DIR_PATH BASE_PATH"lib_dir.txt"
#define LIB_DIR_ENTRIES_PATH BASE_PATH"lib_dir_entries.txt"
#define DISASS_LIB_PATH BASE_PATH"disass_lib/"
#define INDEX_PATH BASE_PATH"index.txt"
#define LOCAL_INDEX_PATH BASE_PATH"local_index.txt"
#define OTOOL_PATH BASE_PATH"otool"
#define ORIG_FILE_DIR_PATH BASE_PATH"../../../"
#define DEST_FILE_PATH BASE_PATH"dest_files/"

#define NEST_MARK "> "
#define NOTE_MARK "NOTEzzz:"
#define CALL_REG_MARK "NOTEzzz:register"
#define CALL_CODE_MARK "NOTEzzz:code"

#define PARSE_INDEX_PROGRESS_UNIT 40000
#define CHECK_LIB_DIR_PROGRESS_UNIT 300
#define SORT_INDEX_PROGRESS_UNIT 2000000
#define EXPAND_PROGRESS_UNIT 40

//Maximum number of possible lists
#define PSORT_LIST_MAX_N 7000
//minimum amount of functions in sublist to make it
//subdivide in more sublists
#define PSORT_TRIGGER_N 10000

//meta numbers count:
#define META_N 6

#define META_NUMBER_0 3405691582 // CAFEBABE
#define META_NUMBER_1 3489328638 // CFFAEDFE
#define META_NUMBER_2 4277009102 // FEEDFACE
#define META_NUMBER_3 3472551422 // CEFAEDFE
#define META_NUMBER_4 4277009103 // FEEDFACF
#define META_NUMBER_5 3199925962 // BEBAFECA

//LIBRARY FUNCTIONS HAVE BEEN FOUND TO BEGIN WITH THE FOLOWING CHARACTERS: '$+_-.'
// and letters both upper wnd lower case
// but they may contain '[]:' and digits

// the supposed lower level of make_index() has shown an ETA of 60 hours for
// ~1 trillion cycles

//#define RED_COLOR 0xD00000
//#define YELLOW_COLOR 0xCCBB00

//if using inverted screen colors use the codes bellow to see the same colors

#define RED_COLOR 0x30FFFF
#define YELLOW_COLOR 0x3445FF

// preferences:
//
// OVERWRITE_DIS is for disassemble libraries function, the rest
// are preferences for expanding
//
// OVERWRITE_DIS: overwrite homonimous libraries when making the disassembled
// libraries in disass_libs directory
//
// NEST_N: How many levels of nested functions it must expand
//
// EXPAND_LOCAL: If it will expand functions local to the program
//
// REPEAT_IMP: If it will expand again a function already expanded before
//
// KEEP_I: Every nesting level that does not correspond to the final file
// will create a temporary file, this option tells wether it will keep them
//
// ORIG_EXEC: Whether the file to expand is an executable or not(assembly code)
typedef struct SETTINGS_STRUCT{

    bool OVERWRITE_DIS;
    int NEST_N;
    bool EXPAND_LOCAL;
    bool REPEAT_IMP;
    bool KEEP_I;
    bool ORIG_EXEC;

}SETTINGS;

typedef struct LOADED_DATA_STRUCT{
    bool LIB_DIR;
    bool LIB_DIR_ENTRIES;
    bool DISASS_LIB;
    bool INDEX;
}LOADED_DATA;

class QString;
typedef struct INDEX_DATA_STRUCT{
    int POS;
    int LIB;
    QString FUNC_NAME;
}INDEX_STRUCT;

#endif // GLOBAL_DEFINITIONS_H
