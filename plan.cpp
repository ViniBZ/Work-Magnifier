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
		
			void init_properties();//initiate the variables
			void delete_properties();//delete the variables
		
			void init_layout();//calls the layout methods and show()
			void init_settings();//initiate the SET variable
			//connect main_engine to main_control
			void set_main_engine_connections();
			//connect the interface objects to main_control
			void set_interface_connections();
		
			//sets the variables and interface for when 'start_work()' is called
			//all buttons disabled
			void set_preloading(bool);
			//sets the variables and interface for when some of the steps process is called
			//all buttons disabled except for 'Cancel'
			void set_processing(bool);
			//sets the variables and interface for when the user calls to 'Edit' the  
			//lib_dir_textedit space
			//all buttons disabled except for 'Cancel' and 'Save'
			void set_editing_libs_dir(bool);
		
			//function called before the main event loop to load the files
			//(lib_dir.txt, lib_dir_entries.txt, lib_func_index, local_func_index)
			void start_work();
			
			//empty the 'file_orig_lineedit'
			void reset_file_orig_lineedit();
		
		PROTECTED:
			//calls for 'delete_properties()'
			void closeEvent(QCloseEvent *close_event);
	
	SIGNALS:
		TO MAIN ENGINE THREAD:
			void sig_start_process();
		TO MAIN ENGINE:
			void sig_send_action(int);
			void sig_cancel();
			
			void sig_send_file_orig(QString);
			void sig_send_settings(SETTINGS);
		///////////////////////////////
		/*
			void sig_reload_lib_dir();
			void sig_send_lib_dir(QString);
			
			void sig_reload_lib_dir_entries_list();
			void sig_make_lib_dir_entries_list();
			
			void sig_reload_disass_libs_entries();
			void sig_disass_libs();
			
			void sig_reload_index();
			void sig_generate_index();
			
			void sig_send_file_orig(QString);
			void sig_send_settings(SETTINGS);
			
			void sig_expand();
			
			void sig_cancel();
			*/

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
			//check result if it exists and number of lines
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
			
			QString get_line_from_file(QFile *file);	//pass already open descriptor
			bool check_for_func_call(QString line_str);
			QString parse_func_name(QString line_str);
			
			//open_mode is write or append
			//parses the function name from implementation in file_o
			//return the number of parsed function implementations
			//function name and position
			//pass already open descriptors
			int parse_index_to_file(QFile *file_o, QFile *file_d, int open_mode);
			
			//opens lib_dir.txt and verify if the directories exist and if
			//they're not empty
			//and permissions
			void load_lib_dir();
				//OPEN lib_dir.txt 
				//PARSE lines
				//CLOSE lib_dir.txt 
					//strip off string part after #
					//strip off prohibited characters for path
					//use simplified method from QString
				//check if directories exist and permisisons
				//check if they are not empty
				//codify status into string 
				//emit 'sig_send_lib_dir(QString,QString)' signal to main_control
			void receive_lib_dir(QString lib_dir_str);
				//OPEN lib_dir.txt
				//write lib_dir_str to file
				//CLOSE lib_dir.txt
				//call sl_load_lib_dir()
			void check_lib_dir_entries_file();
				//IF lib_dir_entries.txt exists
					//check if empty
				//codify result with status and number of entries
				//emit 'sig_send_lib_dir_entries(int,int)' 
			void make_lib_dir_entries_file();
				//OPEN lib_dir_entries.txt
				//LOOP QDirIterator to go recursively through the directories in lib_dir.txt
					//IF the entry is a file
						//copy the file path
						//write file path to lib_dir_entries
				//CLOSE lib_dir_entries.txt
				
			void check_disass_libs();
				//count the amount of files in disass_libs directory
				//emit 'sig_send_disass_libs(int)'
			void disass_libs();
				//OPEN lib_dir_entries.txt
					//LOOP through the lines
						//OPEN file	
							//check the first meta data bytes to see if it is code
						//CLOSE file
						//IF code
							
							//emit 'sig_send_arguments(QString,QString)'  with file path 
							//and path for the disassembled version in disass_libs directory
							//emit 'sig_disassemble_file()' to start the otool process
				
			void check_index();
			void generate_index();
		
	SIGNALS:
		TO MAIN ENGINE THREAD:
			void sig_finished();//calls quit() slot on QThread
		TO MAIN CONTROL:
			void sig_send_lib_dir(QString,QString);//send a string of status characters and the lib dir themselves
			void sig_send_lib_dir_entries(int,int)//send a result status and the number of entries in the file
			void sig_send_disass_libs(int);//send the number of disassembled libraries
			void sig_send_index_check(int,int);//send a result status and the index file size
			
			void sig_send_status(int, QString, int); //status, complement and progress
			
		TO OTOOL ENGINE THREAD:
			void void sig_disassemble_file(); //call start();
		TO OTOOL ENGINE:
			void sig_send_arguments(QString,QString);//file to disassemble and destiny
			//void sig_cancel() ???

	SLOTS:
		FROM MAIN ENGINE THREAD:
			void sl_start_process();//called by 'started()' signal
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
			
