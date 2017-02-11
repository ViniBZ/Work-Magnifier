#ifndef MAINENGINE_H
#define MAINENGINE_H

#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QFile>
#include <QDataStream>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QDirIterator>
#include <QProcess>
#include <QMutex>

#include <QMessageBox>

#include "global_definitions.h"

class MainEngine : public QThread
{
    Q_OBJECT
public:
    MainEngine(QObject *parent);
    ~MainEngine();
//private:
    int ACTION;
    bool PROCEED;
    bool* PROCEED_PT;
    bool CANCELED;
    QMutex* mutex;
    SETTINGS SET;

    QString * LIB_DIR_INFO;
    QString * LIB_DIR_STR;

    QString * ORIG_FILE_NAME;
    QString * DEST_DIR_NAME;
    int LIB_DIR_ENTRIES_N;
/*
    QString * lib_dir_status;
    int lib_dir_entries_status;
    int disass_lib_status;
    int index_status;
    QString * orig_file_name;
    SETTINGS SET;
    LOADED_DATA LOADED;
*/
    /////////
    void init_properties();
    /////////
    void write_to_file(QString content,QFile * file);

    uint char_to_int(uchar ch[], uint length, int endianness);
    void int_to_char(uchar ch[], uint value, uint length, int endianness);
    QString cstr_to_qstr(char *cstr, int cstr_len);

    bool copy_file(QFile *orig_file, QFile *dest_file);
    void write_nest_mark_to_str(QString *str, int n, QString mark);
    uint get_uint_from_file(QFile *file,bool get_lf_after_uint);
    int get_line_from_file(QString *line_str, QFile *file);
    int get_imp_pos_from_str(QString *line_str);
    QString cat_nest_file_name(QString *base, int nest_level, QString *extension);
    QString cat_index_line(int func_pos,QString *lib_name,QString *func_name);
    void strip_file_name(QString *file_name, QString *base, QString *extension);
    //QString strip_file_name(QString str, bool add_suffix, QString suffix, bool add_number, int suf_num);
    void write_line_to_file(QString line_str, QFile *file , bool lf_at_end);
    void write_number_to_file(uint num, int char_len, int endianness,QFile *file,bool lf_at_end);
    QString parse_func_name(QString line_str);//(from function call)
    bool is_space(char ch);
    bool is_digit(char ch);
    bool is_func_imp(QString line_str,bool count_main_as_imp);
    int count_file_lines(QFile *file);


    void process_progress(ulong total_size, ulong cur_pos);
    int qstring_to_str(QString *orig_str,char *dest_str);
    uint str_to_uint(QString str);
    QString uint_to_str(uint num);
    int parse_index_to_arr(QFile *orig_file,QList <INDEX_STRUCT> *list, int lib);
    int parse_index_to_file(QFile *orig_file, QFile *dest_file, bool write_file_name);
    int disassemble(QString file_name, QFile *dest_file);

    void load_lib_dir(bool send_st);
    void save_lib_dir();

    void check_lib_dir_entries_file(bool send_st);

    void make_lib_dir_entries_file();

    void check_disass_lib(bool send_st);

    void disass_lib();

    void check_index(bool send_st);
    void make_index();
    void expand();
    bool get_proceed();
signals:
    // TO MAIN ENGINE THREAD:
    void sig_finished();
    // TO MAIN CONTROL:
    void sig_send_lib_dir(int,QString,QString);
    void sig_send_lib_dir_entries(int,int);
    void sig_send_disass_lib(int);
    void sig_send_index_check(int,int);

    void sig_send_status(int, QString);
    void sig_send_progress(uint);

    // TO OTOOL ENGINE THREAD:
    void sig_disassemble_file();
    // TO OTOOL ENGINE:
    void sig_send_arguments(QString,QString);
public slots:
    // FROM MAIN ENGINE THREAD:
    void sl_start_process();
    // FROM MAIN CONTROL:
    void sl_receive_action(int a);

    void sl_receive_lib_dir_str(QString);
    void sl_receive_orig_file(QString);
    void sl_receive_dest_dir(QString);
    void sl_receive_settings(SETTINGS set);
};

#endif // MAINENGINE_H
