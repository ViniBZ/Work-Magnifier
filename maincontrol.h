#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QScrollArea>
#include <QCloseEvent>
#include <QProcess>
#include <QThread>
#include <QFrame>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMutex>

#include <QMessageBox>


#include "global_definitions.h"
#include "mainengine.h"

class MainControl : public QWidget
{
    Q_OBJECT
public:
    explicit MainControl(QWidget *parent = 0);
    void start_work();
//private:
       // LAYOUT STUFF
       //MAIN LAYOUT
       QVBoxLayout * main_layout;

       QHBoxLayout * lib_dir_layout;
       QHBoxLayout * list_lib_dir_entries_layout;
       QHBoxLayout * disass_lib_layout;
       QHBoxLayout * index_layout;
       QHBoxLayout * orig_file_layout;
       QHBoxLayout * dest_dir_layout;
       QHBoxLayout * settings_layout;
       QHBoxLayout * expand_layout;
       QHBoxLayout * cancel_layout;
       QHBoxLayout * status_layout;
       // STEP 1: LIB DIR
       QLabel * lib_dir_label;
       QTextEdit * lib_dir_textedit;

       QVBoxLayout * lib_dir_buttons_layout;
       QPushButton * reload_lib_dir_button;
       QPushButton * edit_save_lib_dir_button;

       QFrame * lib_dir_line;
       // STEP 2: LIST LIB DIR ENTRIES
       QVBoxLayout * list_lib_dir_entries_label_layout;
       QLabel * list_lib_dir_entries_label;

       QVBoxLayout * list_lib_dir_entries_buttons_layout;
       QPushButton * reload_lib_dir_entries_button;
       QPushButton * make_lib_dir_entries_button;

       QFrame * list_lib_dir_entries_line;
       // STEP 3: DISASSEMBLE
       QVBoxLayout * disass_lib_label_layout;
       QLabel * disass_lib_label;

       QVBoxLayout * disass_lib_buttons_layout;
       QPushButton * reload_disass_lib_button;
       QPushButton * make_disass_lib_button;
       QCheckBox * overwrite_homonymous_lib_checkbox;

       QFrame * disass_lib_line;
       // STEP 4: INDEX
       QVBoxLayout * index_label_layout;
       QLabel * index_label;

       QVBoxLayout * index_buttons_layout;
       QPushButton * reload_index_button;
       QPushButton * make_index_button;

       QFrame * index_line;
       // STEP 5: EXPAND
       QLineEdit * orig_file_lineedit;
       QLabel * orig_file_label;
       QPushButton * orig_file_button;

       QLineEdit * dest_dir_lineedit;
       QLabel * dest_dir_label;
       QPushButton * dest_dir_button;

       QLabel * nest_label;
       QComboBox * nest_combobox;
       QCheckBox * expand_local_func_checkbox;
       QCheckBox * repeat_imp_checkbox;
       QCheckBox * keepi_checkbox;

       QPushButton * orig_type_executable_button;
       QPushButton * orig_type_assembly_button;

       QPushButton * expand_button;

       QFrame * expand_line;
       // CANCEL
       QPushButton * cancel_button;

       QFrame * cancel_line;
       // STATUS
       QLabel * status_label;
       QLabel * progress_label;
       //END LAYOUT STUFF

       QThread * main_engine_thread;

       MainEngine * main_engine;
       SETTINGS SET;
       LOADED_DATA LOADED;

       QString PREV_LIB_DIR_STR;

       bool CLOSED;
       bool IS_PRELOADING;
       bool IS_PROCESSING;
       bool IS_EDITING_LIB_DIR;

       int progress_sig;

       bool PROCEED;
       QMutex mutex;

       //-------------- METHODS

       void init_properties();
       void delete_properties();

       void init_layout();
       void init_settings();
       void init_loaded();

       void set_engine_connections();

       void set_interface_connections();

       void set_interface_enabled();
protected:
       void closeEvent(QCloseEvent *close_event);
signals:
       // TO MAIN ENGINE THREAD:
       void sig_start_process();
       // TO MAIN ENGINE:
       void sig_send_action(int);
       void sig_cancel();

       void sig_send_lib_dir_str(QString);
       void sig_send_orig_file(QString);
       void sig_send_dest_dir(QString);
       void sig_send_settings(SETTINGS);
public slots:
       // FROM INTERFACE:
       void sl_reload_lib_dir_button_clicked();
       void sl_edit_save_lib_dir_button_clicked();

       void sl_reload_lib_dir_entries_button_clicked();
       void sl_make_lib_dir_entries_button_clicked();

       void sl_reload_disass_lib_button_clicked();
       void sl_make_disass_lib_button_clicked();
       void sl_overwrite_homonymous_lib_checkbox_toggled(bool ck);

       void sl_reload_index_button_clicked();
       void sl_make_index_button_clicked();

       void sl_orig_file_lineedit_changed(QString str);
       void sl_orig_file_button_clicked();

       void sl_dest_dir_lineedit_changed(QString str);
       void sl_dest_dir_button_clicked();

       void sl_nest_combobox_changed(int n);
       void sl_expand_local_func_checkbox_toggled(bool ck);
       void sl_repeat_imp_checkbox_toggled(bool ck);
       void sl_keepi_checkbox_toggled(bool ck);
       void sl_orig_type_executable_button_clicked(bool ck);
       void sl_orig_type_assembly_button_clicked(bool ck);

       void sl_expand_button_clicked();

       void sl_cancel_button_clicked();

       // FROM MAIN ENGINE:
       void sl_receive_lib_dir(int st,QString lib_dir_info, QString lib_dir_str);

       void sl_receive_lib_dir_entries(int check, int lib_dir_entries_n);
       void sl_receive_disass_lib(int disass_lib);
       void sl_receive_index_check(int check,int index_size);

       void sl_receive_status(int status, QString complement);
       void sl_receive_progress(uint progress);
       // FROM MAIN ENGINE THREAD:
       void sl_process_finished();
};

#endif // MAINCONTROL_H
