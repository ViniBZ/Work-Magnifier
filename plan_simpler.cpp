/*
MainControl will contain the interface and call for MainEngine
to do the processing work

MainEngine will do all the processing except for the otool
OtoolEngine part

OtoolEngine will run its own thread to run otool

*The way to 'MainEngine' and 'OtoolEngine' to run in another thread will be to move it
to a QThread's thread

*To start the process 'MainControl' sends the signal 'sig_start_process()' to main_engine_thread
then main_engine_thread sends started() to main_engine
but to stop(cancel) it sends the signal 'sig_cancel_process()' to main_engine which will then
send sig_finished() to main_engine_thread to quit it


*/

" signals begin with 'sig' and slots with 'sl' "

//DATA FILES:

lib_dir.txt
lib_dir_entries.txt

disass_lib/ *ALL ENTRIES

lib_func_index
local_func_index

//

MAIN CONTROL : QWIDGET

	MEMBERS:	
	
		PRIVATE:
			
			*
			*LAYOUT STUFF
			*
		
			QThread *main_engine_thread;
			MainEngine *main_engine;
		
			SETTINGS SET;
			
			bool IS_PRELOADING;
       		bool IS_PROCESSING;
       		bool IS_EDITING_LIB_DIR;
		
			////////////
		
			void init_properties();
			void delete_properties();
		
			void init_layout();
			void init_settings();

			void set_engine_connections();

			void set_interface_connections();
		
			void set_preloading(bool);

			void set_processing(bool);
			
			void set_editing_libs_dir(bool);
		
			void start_work();
			
			void reset_file_orig_lineedit();
		
		PROTECTED:
			void closeEvent(QCloseEvent *close_event);
	
	SIGNALS:
		TO MAIN ENGINE THREAD:
			void sig_start_process();
		TO MAIN ENGINE:
			void sig_send_action(int);
			void sig_cancel();
			
			void sig_send_file_orig(QString);
			void sig_send_settings(SETTINGS);
		
	SLOTS:
		FROM INTERFACE:
			void sl_reload_lib_dir_button_clicked();
			void sl_edit_save_lib_dir_button_clicked();
			
			void sl_reload_lib_dir_entries_button_clicked();
			void sl_make_lib_dir_entries_button_clicked();
			
			void sl_reload_disass_libs_button_clicked();
			void sl_make_disass_lib_button_clicked();
			void sl_overwrite_homonimous_lib_checkbox_changed();
			
			void sl_reload_index_button_clicked();
			void sl_make_index_button_clicked();
			
			void sl_file_orig_lineedit_changed(QString);
			void sl_browse_file_orig_button_clicked();
			
			void sl_nest_combobox_changed(int);
			void sl_expand_local_checkbox_changed(bool);
			void sl_repeat_imp_checkbox_changed(bool);
			void sl_keepi_checkbox_changed(bool);
			void sl_from_exec_button_clicked(bool);
			void sl_from_ass_button_clicked(bool);
			
			void sl_expand_button_clicked();
			
			void sl_cancel_button_clicked();
			
		FROM MAIN ENGINE:
			void sl_receive_lib_dir(QString libs_dir_info, QString libs_dir_str);
			
			void sl_receive_lib_dir_entries(int check, int lib_dir_entries_n);
			void sl_receive_disass_libs(int disass_libs);
			void sl_receive_index_check(int check,int index_size);
			
			void sl_receive_status(int status, QString complement, int progress);
	
	
	
MAIN ENGINE : QObject

	MEMBERS:
	
	
		PRIVATE:			
			bool PROCEED;
			QThread *otool_engine_thread;
			/////////
			void init_properties();
			void delete_properties();
			/////////
			void write_to_file(QString file_name, QString content);
			void append_to_file(QString file_name, QString content);
			
			int char_to_int(char ch[], int length, int endianness);
			void int_to_char(char ch[], int value, int length, int endianness);
			
			QString get_line_from_file(QFile *file);
			bool check_for_func_call(QString line_str);
			QString parse_func_name(QString line_str);
			
			
			int parse_index_to_file(QFile *file_o, QFile *file_d, int open_mode);
			
			
			void load_lib_dir();
				
			void receive_lib_dir(QString lib_dir_str);
				
			void check_lib_dir_entries_file();
				
			void make_lib_dir_entries_file();
				
			void check_disass_libs();
				
			void disass_libs();
				
			void check_index();
			void generate_index();
		
	SIGNALS:
		TO MAIN ENGINE THREAD:
			void sig_finished();
		TO MAIN CONTROL:
			void sig_send_lib_dir(QString,QString);
			void sig_send_lib_dir_entries(int,int)
			void sig_send_disass_libs(int);
			void sig_send_index_check(int,int);
			
			void sig_send_status(int, QString, int);
			
		TO OTOOL ENGINE THREAD:
			void void sig_disassemble_file();
		TO OTOOL ENGINE:
			void sig_send_arguments(QString,QString);
			

	SLOTS:
		FROM MAIN ENGINE THREAD:
			void sl_start_process();
		FROM MAIN CONTROL:
			void sl_receive_action(int);
			void sl_cancel();
			
			void sl_receive_file_orig(QString);
			void sl_receive_settings(SETTINGS);
		FROM OTOOL ENGINE:
			void sl_receive_stdout(QString);
			void sl_receive_stderr(QString);
	
OTOOL ENGINE : QThread 
	PROPERTIES:
		
	
	METHODS:
	
	SIGNALS:
		TO OTOOL ENGINE THREAD:
			void sig_finished();
		TO MAIN ENGINE:
			void sig_send_stdout(QString);
			void sig_send_stderr(QString);
			
	SLOTS:
		FROM OTOOL ENGINE THREAD:
			void sl_disassemble_file();
		FROM MAIN ENGINE:
			void sl_receive_arguments(QString,QString)
			
