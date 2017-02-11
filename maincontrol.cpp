#include "maincontrol.h"

// If you want to take a stroll through this code lines I recommend having the
// global_definitiong.h file opened to improve understanding

MainControl::MainControl(QWidget *parent) : QWidget(parent)
{
    //just initializing values and allocating stuff not related to GUI
    init_properties();
    //do all the interface stuff, not connections
    init_layout();

    //self explanatory right?
    set_interface_connections();
    //the signals used with the engine doing the actual work
    set_engine_connections();
    //init the user settings defaults
    init_settings();
    //set default values for 'loaded' flags
    init_loaded();
    //this function sets the GUI usability according to the flags that indicate the program
    //engine running status, these variables are initialized in init_properties()
    set_interface_enabled();


}
// --------------------------------------------------------- START WORK
void MainControl::start_work()
{
    mutex.lock();
    PROCEED=true;
    mutex.unlock();
    IS_PRELOADING=true;
    set_interface_enabled();
    emit sig_send_action(1);
    emit sig_start_process();

}
// --------------------------------------------------------- INIT PROPERTIES
void MainControl::init_properties()
{
    progress_sig=0;

    main_engine_thread = new QThread;
    main_engine = new MainEngine(NULL);
    main_engine->PROCEED_PT=&PROCEED;
    main_engine->mutex=&mutex;
    main_engine->moveToThread(main_engine_thread);

    CLOSED=false;
    IS_PRELOADING=false;
    IS_EDITING_LIB_DIR=false;
    IS_PROCESSING=false;

}
// --------------------------------------------------------- DELETE PROPERTIES
void MainControl::delete_properties()
{
    delete main_engine_thread;
    delete main_engine;
}

// ----------------------------------------------------------- INIT LAYOUT
void MainControl::init_layout()
{
    int c=0;
    //////////////////////////////DECLARING MAIN LAYOUT
    //MAIN LAYOUT
    main_layout = new QVBoxLayout;

    lib_dir_layout = new QHBoxLayout;
    list_lib_dir_entries_layout = new QHBoxLayout;
    disass_lib_layout = new QHBoxLayout;
    index_layout = new QHBoxLayout;
    orig_file_layout = new QHBoxLayout;
    dest_dir_layout = new QHBoxLayout;
    settings_layout = new QHBoxLayout;
    expand_layout = new QHBoxLayout;
    cancel_layout = new QHBoxLayout;
    status_layout = new QHBoxLayout;
    // STEP 1: LIB DIR
    lib_dir_label = new QLabel("---------");
    lib_dir_textedit = new QTextEdit("-----------------");

    lib_dir_buttons_layout = new QVBoxLayout;
    reload_lib_dir_button = new QPushButton("Reload");
    edit_save_lib_dir_button = new QPushButton("Edit");

    lib_dir_line = new QFrame;
    // STEP 2: LIST LIB DIR ENTRIES
    list_lib_dir_entries_label = new QLabel("List library entries");

    reload_lib_dir_entries_button = new QPushButton("Reload entries");
    make_lib_dir_entries_button = new QPushButton("Make libraries entries");

    list_lib_dir_entries_line = new QFrame;
    // STEP 3: DISASSEMBLE
    disass_lib_label = new QLabel("...Disassemble libraries...");

    reload_disass_lib_button = new QPushButton("Reload libraries");
    make_disass_lib_button = new QPushButton("Disassemble libraries");
    overwrite_homonymous_lib_checkbox = new QCheckBox("Overwrite\nhomonymous libraries");

    disass_lib_line = new QFrame;
    // STEP 4: INDEX
    index_label = new QLabel("...Generate index...");

    reload_index_button = new QPushButton("Reload index");
    make_index_button = new QPushButton("Generate index");

    index_line = new QFrame;
    // STEP 5: EXPAND
    orig_file_lineedit = new QLineEdit;
    orig_file_label = new QLabel("File to be expanded:");
    orig_file_button = new QPushButton("browse");

    dest_dir_lineedit = new QLineEdit;
    dest_dir_label = new QLabel("Directory where expanded code will be placed:");
    dest_dir_button = new QPushButton("browse");

    nest_label = new QLabel("Nest");
    nest_combobox = new QComboBox;
    expand_local_func_checkbox = new QCheckBox("Expand local functions");
    repeat_imp_checkbox = new QCheckBox("Repeat functions");
    keepi_checkbox = new QCheckBox("Keep intermediate nesting files");

    orig_type_executable_button = new QPushButton("From executable");
    orig_type_assembly_button = new QPushButton("From assembly");
    expand_button = new QPushButton("Expand");
    expand_line = new QFrame;
    // CANCEL
    cancel_button = new QPushButton("Cancel");
    cancel_line= new QFrame;
    // STATUS
    status_label = new QLabel("......");
    progress_label = new QLabel("......");

    //////////////////////////////SETTING MAIN LAYOUT
    reload_lib_dir_button->setFixedSize(180,36);
    edit_save_lib_dir_button->setFixedSize(180,36);
    reload_lib_dir_entries_button->setFixedSize(180,36);
    make_lib_dir_entries_button->setFixedSize(180,36);
    reload_disass_lib_button->setFixedSize(180,36);
    make_disass_lib_button->setFixedSize(180,36);
    reload_index_button->setFixedSize(180,36);
    make_index_button->setFixedSize(180,36);

    while(c<10)
    {
        nest_combobox->addItem(QString::number(c));
        c++;
    }
    nest_combobox->addItem("Max");
    c=0;

    orig_type_executable_button->setFixedSize(180,36);
    orig_type_assembly_button->setFixedSize(180,36);
    cancel_button->setFixedSize(180,36);
    expand_button->setFixedSize(180,36);

    orig_type_executable_button->setCheckable(true);
    orig_type_assembly_button->setCheckable(true);

    progress_label->setAlignment(Qt::AlignLeft);

    main_layout->setAlignment(Qt::AlignRight);
    //status_label->setMaximumWidth(300);
    // LINES
    lib_dir_line->setFrameShape(QFrame::HLine);
    lib_dir_line->setFrameShadow(QFrame::Sunken);

    list_lib_dir_entries_line->setFrameShape(QFrame::HLine);
    list_lib_dir_entries_line->setFrameShadow(QFrame::Sunken);

    disass_lib_line->setFrameShape(QFrame::HLine);
    disass_lib_line->setFrameShadow(QFrame::Sunken);

    index_line->setFrameShape(QFrame::HLine);
    index_line->setFrameShadow(QFrame::Sunken);

    expand_line->setFrameShape(QFrame::HLine);
    expand_line->setFrameShadow(QFrame::Sunken);

    cancel_line->setFrameShape(QFrame::HLine);
    cancel_line->setFrameShadow(QFrame::Sunken);
    //////////////////////////////BUILDING MAIN LAYOUT

    // STEP 1: LIB DIR

    lib_dir_buttons_layout->addWidget(reload_lib_dir_button);
    lib_dir_buttons_layout->addWidget(edit_save_lib_dir_button);
    lib_dir_buttons_layout->addStretch();

    lib_dir_layout->addWidget(lib_dir_textedit);
    lib_dir_layout->addLayout(lib_dir_buttons_layout);

    // STEP 2: LIST LIB DIR ENTRIES    
    list_lib_dir_entries_layout->addWidget(list_lib_dir_entries_label);
    list_lib_dir_entries_layout->addWidget(reload_lib_dir_entries_button);
    list_lib_dir_entries_layout->addWidget(make_lib_dir_entries_button);

    // STEP 3: DISASSEMBLE
    disass_lib_layout->addWidget(disass_lib_label);
    disass_lib_layout->addWidget(reload_disass_lib_button);
    disass_lib_layout->addWidget(make_disass_lib_button);
    disass_lib_layout->addWidget(overwrite_homonymous_lib_checkbox);

    // STEP 4: INDEX
    index_layout->addWidget(index_label);
    index_layout->addWidget(reload_index_button);
    index_layout->addWidget(make_index_button);

    // STEP 5: EXPAND
    orig_file_layout->addWidget(orig_file_lineedit);
    orig_file_layout->addWidget(orig_file_button);

    dest_dir_layout->addWidget(dest_dir_lineedit);
    dest_dir_layout->addWidget(dest_dir_button);

    settings_layout->addStretch();
    settings_layout->addWidget(nest_label);
    settings_layout->addWidget(nest_combobox);
    settings_layout->addWidget(expand_local_func_checkbox);
    settings_layout->addWidget(repeat_imp_checkbox);
    settings_layout->addWidget(keepi_checkbox);

    expand_layout->addStretch();
    expand_layout->addWidget(orig_type_executable_button);
    expand_layout->addWidget(orig_type_assembly_button);
    expand_layout->addWidget(expand_button);

    // CANCEL
    cancel_layout->addStretch();
    cancel_layout->addWidget(cancel_button);

    // STATUS
    status_layout->addWidget(status_label);
    status_layout->addStretch();
    status_layout->addWidget(progress_label);


    //////
    main_layout->addWidget(lib_dir_label);
    main_layout->addLayout(lib_dir_layout);
    main_layout->addWidget(lib_dir_line);

    main_layout->addLayout(list_lib_dir_entries_layout);
    main_layout->addWidget(list_lib_dir_entries_line);

    main_layout->addLayout(disass_lib_layout);
    main_layout->addWidget(disass_lib_line);

    main_layout->addLayout(index_layout);
    main_layout->addWidget(index_line);

    main_layout->addWidget(orig_file_label);
    main_layout->addLayout(orig_file_layout);
    main_layout->addWidget(dest_dir_label);
    main_layout->addLayout(dest_dir_layout);
    main_layout->addLayout(settings_layout);
    main_layout->addLayout(expand_layout);
    main_layout->addWidget(expand_line);

    main_layout->addLayout(cancel_layout);
    main_layout->addWidget(cancel_line);

    main_layout->addLayout(status_layout);

    setWindowTitle(WINDOW_TITLE);
    setLayout(main_layout);


}
// --------------------------------------------------------- INIT SETTINGS
void MainControl::init_settings()
{
    //in order for this function to alter the SET variable
    //call this function after set_interface_connections

    overwrite_homonymous_lib_checkbox->setChecked(false);
    SET.OVERWRITE_DIS=false;

    nest_combobox->setCurrentIndex(1);
    SET.NEST_N=1;

    expand_local_func_checkbox->setChecked(false);
    SET.EXPAND_LOCAL=false;

    repeat_imp_checkbox->setChecked(false);
    SET.REPEAT_IMP=false;

    keepi_checkbox->setChecked(false);
    SET.KEEP_I=false;

    orig_type_executable_button->setChecked(true);
    sl_orig_type_executable_button_clicked(true);
    SET.ORIG_EXEC=true;

}
// --------------------------------------------------------- INIT LOADED
void MainControl::init_loaded()
{
    LOADED.LIB_DIR=false;
    LOADED.LIB_DIR_ENTRIES=false;
    LOADED.DISASS_LIB=false;
    LOADED.INDEX=false;
}

// --------------------------------------------------------- SET ENGINE CONNECTIONS
void MainControl::set_engine_connections()
{

    // FROM MAIN ENGINE:
    connect(main_engine,SIGNAL(sig_send_lib_dir(int,QString,QString)),this,SLOT(sl_receive_lib_dir(int,QString,QString)));
    connect(main_engine,SIGNAL(sig_send_lib_dir_entries(int,int)),this,SLOT(sl_receive_lib_dir_entries(int,int)));
    connect(main_engine,SIGNAL(sig_send_disass_lib(int)),this,SLOT(sl_receive_disass_lib(int)));
    connect(main_engine,SIGNAL(sig_send_index_check(int,int)),this,SLOT(sl_receive_index_check(int,int)));
    connect(main_engine,SIGNAL(sig_send_status(int, QString)),this,SLOT(sl_receive_status(int,QString)));
    connect(main_engine,SIGNAL(sig_send_progress(uint)),this,SLOT(sl_receive_progress(uint)));
    // FROM MAIN ENGIEN THREAD:
    connect(main_engine_thread,SIGNAL(finished()),this,SLOT(sl_process_finished()));

    // TO MAIN ENGINE THREAD:
    connect(this,SIGNAL(sig_start_process()),main_engine_thread,SLOT(start()),Qt::DirectConnection);
    // TO MAIN ENGINE:
    connect(this,SIGNAL(sig_send_action(int)),main_engine,SLOT(sl_receive_action(int)),Qt::DirectConnection);

    connect(this,SIGNAL(sig_send_lib_dir_str(QString)),main_engine,SLOT(sl_receive_lib_dir_str(QString)),Qt::DirectConnection);
    connect(this,SIGNAL(sig_send_orig_file(QString)),main_engine,SLOT(sl_receive_orig_file(QString)),Qt::DirectConnection);
    connect(this,SIGNAL(sig_send_dest_dir(QString)),main_engine,SLOT(sl_receive_dest_dir(QString)),Qt::DirectConnection);

    connect(this,SIGNAL(sig_send_settings(SETTINGS)),main_engine,SLOT(sl_receive_settings(SETTINGS)),Qt::DirectConnection);

    // FROM MAIN ENGINE TO MAIN ENGINE THREAD:
    connect(main_engine,SIGNAL(sig_finished()),main_engine_thread,SLOT(quit()));
    // FROM MAIN ENGINE THREAD TO MAIN ENGINE:
    connect(main_engine_thread,SIGNAL(started()),main_engine,SLOT(sl_start_process()),Qt::DirectConnection);

}
// --------------------------------------------------------- SET INTERFACE CONNECTIONS
void MainControl::set_interface_connections()
{
    connect(reload_lib_dir_button,SIGNAL(clicked()),this,SLOT(sl_reload_lib_dir_button_clicked()));
    connect(edit_save_lib_dir_button,SIGNAL(clicked()),this,SLOT(sl_edit_save_lib_dir_button_clicked()));

    connect(reload_lib_dir_entries_button,SIGNAL(clicked()),this,SLOT(sl_reload_lib_dir_entries_button_clicked()));
    connect(make_lib_dir_entries_button,SIGNAL(clicked()),this,SLOT(sl_make_lib_dir_entries_button_clicked()));

    connect(reload_disass_lib_button,SIGNAL(clicked()),this,SLOT(sl_reload_disass_lib_button_clicked()));
    connect(make_disass_lib_button,SIGNAL(clicked()),this,SLOT(sl_make_disass_lib_button_clicked()));
    connect(overwrite_homonymous_lib_checkbox,SIGNAL(toggled(bool)),this,SLOT(sl_overwrite_homonymous_lib_checkbox_toggled(bool)));

    connect(reload_index_button,SIGNAL(clicked()),this,SLOT(sl_reload_index_button_clicked()));
    connect(make_index_button,SIGNAL(clicked()),this,SLOT(sl_make_index_button_clicked()));

    connect(orig_file_lineedit,SIGNAL(textChanged(QString)),this,SLOT(sl_orig_file_lineedit_changed(QString)));
    connect(orig_file_button,SIGNAL(clicked()),this,SLOT(sl_orig_file_button_clicked()));
    connect(dest_dir_lineedit,SIGNAL(textChanged(QString)),this,SLOT(sl_dest_dir_lineedit_changed(QString)));
    connect(dest_dir_button,SIGNAL(clicked()),this,SLOT(sl_dest_dir_button_clicked()));
    connect(nest_combobox,SIGNAL(currentIndexChanged(int)),this,SLOT(sl_nest_combobox_changed(int)));
    connect(expand_local_func_checkbox,SIGNAL(toggled(bool)),this,SLOT(sl_expand_local_func_checkbox_toggled(bool)));
    connect(repeat_imp_checkbox,SIGNAL(toggled(bool)),this,SLOT(sl_repeat_imp_checkbox_toggled(bool)));
    connect(keepi_checkbox,SIGNAL(toggled(bool)),this,SLOT(sl_keepi_checkbox_toggled(bool)));
    connect(orig_type_executable_button,SIGNAL(clicked(bool)),this,SLOT(sl_orig_type_executable_button_clicked(bool)));
    connect(orig_type_assembly_button,SIGNAL(clicked(bool)),this,SLOT(sl_orig_type_assembly_button_clicked(bool)));

    connect(expand_button,SIGNAL(clicked()),this,SLOT(sl_expand_button_clicked()));

    connect(cancel_button,SIGNAL(clicked()),this,SLOT(sl_cancel_button_clicked()));

}
// --------------------------------------------------------- SET INTERFACE ENABLED
void MainControl::set_interface_enabled()
{
    bool proc=(IS_PRELOADING || IS_EDITING_LIB_DIR || IS_PROCESSING);

    //lib_dir_textedit->setEnabled(IS_EDITING_LIB_DIR);
    if(IS_EDITING_LIB_DIR)
    {
        lib_dir_textedit->setTextInteractionFlags(Qt::TextEditorInteraction);
        edit_save_lib_dir_button->setText("Save");
    }else{
        lib_dir_textedit->setTextInteractionFlags(Qt::TextBrowserInteraction);
        edit_save_lib_dir_button->setText("Edit");
    }
    reload_lib_dir_button->setEnabled(!proc);
    edit_save_lib_dir_button->setEnabled(IS_EDITING_LIB_DIR || !proc);
    reload_lib_dir_entries_button->setEnabled(LOADED.LIB_DIR && !proc);
    make_lib_dir_entries_button->setEnabled(LOADED.LIB_DIR && !proc);
    reload_disass_lib_button->setEnabled(LOADED.LIB_DIR_ENTRIES && !proc);
    make_disass_lib_button->setEnabled(LOADED.LIB_DIR_ENTRIES && !proc);
    overwrite_homonymous_lib_checkbox->setEnabled(LOADED.LIB_DIR_ENTRIES && !proc);
    reload_index_button->setEnabled(LOADED.DISASS_LIB && !proc);
    make_index_button->setEnabled(LOADED.DISASS_LIB && !proc);
    orig_file_lineedit->setEnabled(!proc);
    orig_file_button->setEnabled(!proc);
    dest_dir_lineedit->setEnabled(!proc);
    dest_dir_button->setEnabled(!proc);
    nest_combobox->setEnabled(!proc);
    expand_local_func_checkbox->setEnabled(!proc);
    repeat_imp_checkbox->setEnabled(!proc);
    keepi_checkbox->setEnabled(!proc);
    orig_type_assembly_button->setEnabled(!proc);
    orig_type_executable_button->setEnabled(!proc);
    expand_button->setEnabled(!proc);
    cancel_button->setEnabled(!IS_PRELOADING && proc);
}
// --------------------------------------------------------- CLOSE EVENT
void MainControl::closeEvent(QCloseEvent * close_event)
{
    if(IS_PRELOADING || IS_PROCESSING)
    {
        close_event->ignore();
        CLOSED=true;
        mutex.lock();
        PROCEED=false;
        mutex.unlock();
    }else{
        delete_properties();
    }
}

// ----------------------------------- (from interface) SL RELOAD LIB DIR BUTTON CLICKED
void MainControl::sl_reload_lib_dir_button_clicked()
{
    IS_PROCESSING=true;
    set_interface_enabled();
    emit sig_send_action(2);
    emit sig_start_process();
}

// ----------------------------------- (from interface) SL EDIT SAVE LIB DIR BUTTON CLICKED
void MainControl::sl_edit_save_lib_dir_button_clicked()
{
    if(IS_EDITING_LIB_DIR)
    {
        mutex.lock();
        PROCEED=true;
        mutex.unlock();
        IS_EDITING_LIB_DIR=false;
        IS_PROCESSING=true;
        set_interface_enabled();
        emit sig_send_lib_dir_str(lib_dir_textedit->toPlainText());
        emit sig_send_action(3);
        emit sig_start_process();
    }else{
        IS_EDITING_LIB_DIR=true;
        set_interface_enabled();
    }
}

// ----------------------------------- (from interface) SL RELOAD LIB DIR ENTRIES BUTTON CLICKED
void MainControl::sl_reload_lib_dir_entries_button_clicked()
{
    mutex.lock();
    PROCEED=true;
    mutex.unlock();
    IS_PROCESSING=true;
    set_interface_enabled();
    emit sig_send_action(4);
    emit sig_start_process();
}
// ----------------------------------- (from interface) SL MAKE LIB DIR ENTRIES BUTTON CLICKED
void MainControl::sl_make_lib_dir_entries_button_clicked()
{
    mutex.lock();
    PROCEED=true;
    mutex.unlock();
    IS_PROCESSING=true;
    set_interface_enabled();
    emit sig_send_action(5);
    emit sig_start_process();
}
// ----------------------------------- (from interface) SL RELOAD DISASS LIB BUTTON CLICKED
void MainControl::sl_reload_disass_lib_button_clicked()
{
    mutex.lock();
    PROCEED=true;
    mutex.unlock();
    IS_PROCESSING=true;
    set_interface_enabled();
    emit sig_send_action(6);
    emit sig_start_process();
}
// ----------------------------------- (from interface) SL DISASS LIB BUTTON CLICKED
void MainControl::sl_make_disass_lib_button_clicked()
{
    mutex.lock();
    PROCEED=true;
    mutex.unlock();
    IS_PROCESSING=true;
    set_interface_enabled();
    emit sig_send_action(7);
    emit sig_start_process();
}
// ----------------------------------- (from interface) SL OVERWRITE HOMONYMOUS LIB CHECKBOX TOGGLED
void MainControl::sl_overwrite_homonymous_lib_checkbox_toggled(bool ck)
{
if(ck)
{
    status_label->setText(" - sl_overwrite_homonymous_lib_checkbox_changed true - ");
}else{
    status_label->setText(" - sl_overwrite_homonymous_lib_checkbox_changed false - ");
}
    SET.OVERWRITE_DIS=ck;

}
// ----------------------------------- (from interface) SL RELOAD INDEX BUTTON CLICKED
void MainControl::sl_reload_index_button_clicked()
{
    mutex.lock();
    PROCEED=true;
    mutex.unlock();
    IS_PROCESSING=true;
    set_interface_enabled();
    emit sig_send_action(8);
    emit sig_start_process();
}
// ----------------------------------- (from interface) SL GENERATE INDEX BUTTON CLICKED
void MainControl::sl_make_index_button_clicked()
{
    mutex.lock();
    PROCEED=true;
    mutex.unlock();
    IS_PROCESSING=true;
    set_interface_enabled();
    emit sig_send_action(9);
    emit sig_start_process();

}
// ----------------------------------- (from interface) SL ORIG FILE LINEEDIT CHANGED
void MainControl::sl_orig_file_lineedit_changed(QString str)
{
    emit sig_send_orig_file(str);
}
// ----------------------------------- (from interface) SL BROWSE ORIG FILE BUTTON CLICKED
void MainControl::sl_orig_file_button_clicked()
{
    QString str=QFileDialog::getOpenFileName(NULL,"Open File",ORIG_FILE_DIR_PATH);
    orig_file_lineedit->setText(str);
}
// ----------------------------------- (from interface) SL ORIG FILE LINEEDIT CHANGED
void MainControl::sl_dest_dir_lineedit_changed(QString str)
{
    emit sig_send_dest_dir(str);
}
// ----------------------------------- (from interface) SL BROWSE ORIG FILE BUTTON CLICKED
void MainControl::sl_dest_dir_button_clicked()
{
    QString str=QFileDialog::getExistingDirectory(NULL,"Choose directory",ORIG_FILE_DIR_PATH);
    dest_dir_lineedit->setText(str);
}
// ----------------------------------- (from interface) SL NEST COMBOBOX CHANGED
void MainControl::sl_nest_combobox_changed(int n)
{
    SET.NEST_N=n;
    emit sig_send_settings(SET);
}
// ----------------------------------- (from interface) SL EXPAND LOCAL CHECKBOX CHANGED
void MainControl::sl_expand_local_func_checkbox_toggled(bool ck)
{
    SET.EXPAND_LOCAL=ck;
    emit sig_send_settings(SET);
}
// ----------------------------------- (from interface) SL REPEAT CHECKBOX CHANGED
void MainControl::sl_repeat_imp_checkbox_toggled(bool ck)
{
    SET.REPEAT_IMP=ck;
    emit sig_send_settings(SET);
}
// ----------------------------------- (from interface) SL KEEPI CHECKBOX CHANGED
void MainControl::sl_keepi_checkbox_toggled(bool ck)
{
    SET.KEEP_I=ck;
    emit sig_send_settings(SET);
}
// ----------------------------------- (from interface) SL FROM EXEC BUTTON CLICKED
void MainControl::sl_orig_type_executable_button_clicked(bool ck)
{
    if(ck)
    {
        orig_type_assembly_button->setChecked(false);
        SET.ORIG_EXEC=true;
    }else{
        orig_type_assembly_button->setChecked(true);
        SET.ORIG_EXEC=false;
    }
    emit sig_send_settings(SET);
}
// ----------------------------------- (from interface) SL FROM ASS BUTTON CLICKED
void MainControl::sl_orig_type_assembly_button_clicked(bool ck)
{
    if(ck)
    {
        orig_type_executable_button->setChecked(false);
        SET.ORIG_EXEC=false;
    }else{
        orig_type_executable_button->setChecked(true);
        SET.ORIG_EXEC=true;
    }
    emit sig_send_settings(SET);
}

// ----------------------------------- (from interface) SL EXPAND BUTTON CLICKED
void MainControl::sl_expand_button_clicked()
{


    if(orig_file_lineedit->text().isEmpty())
    {
        sl_receive_status(70,"");
    }else if(dest_dir_lineedit->text().isEmpty())
    {
        sl_receive_status(77,"");
    }else{
        mutex.lock();
        PROCEED=true;
        mutex.unlock();
        IS_PROCESSING=true;
        set_interface_enabled();
        emit sig_send_action(10);
        emit sig_start_process();
    }

}
// ----------------------------------- (from interface) SL CANCEL BUTTON CLICKED
void MainControl::sl_cancel_button_clicked()
{
    sl_receive_status(98,"");
    if(IS_PROCESSING || IS_PRELOADING)
    {
        mutex.lock();
        PROCEED=false;
        mutex.unlock();
        //emit sig_cancel();
    }
    if(IS_EDITING_LIB_DIR)
    {
        lib_dir_textedit->setText(PREV_LIB_DIR_STR);
        IS_EDITING_LIB_DIR=false;
        set_interface_enabled();
    }
}

// ----------------------------------- (from main_engine) SL RECEIVE LIB DIR
void MainControl::sl_receive_lib_dir(int st,QString lib_dir_info, QString lib_dir_str)
{
    QString info_str("");
    QString textedit_str("");
    if(st==2)
    {
        LOADED.LIB_DIR=true;

        //---------------------------------- textedit lib dir text
        QString red_font_tag("<font color=\"#");
        red_font_tag.append(QString::number(RED_COLOR,16));
        red_font_tag.append("\">");

        QString yellow_font_tag("<font color=\"#");
        yellow_font_tag.append(QString::number(YELLOW_COLOR,16));
        yellow_font_tag.append("\">");

        QString close_font_tag("</font>");

        int dir_entries_n=lib_dir_info.count();
        int dir_not_found_n=0;
        int dir_empty_n=0;
        int c=0;
        while(c<dir_entries_n)
        {

            if(lib_dir_info.at(c)==QChar::fromLatin1('0'))
            {
                textedit_str.append(red_font_tag);
                textedit_str.append(lib_dir_str.section("\n",c,c));
                textedit_str.append(close_font_tag);
                dir_not_found_n++;
            }
            if(lib_dir_info.at(c)==QChar::fromLatin1('1'))
            {
                textedit_str.append(yellow_font_tag);
                textedit_str.append(lib_dir_str.section("\n",c,c));
                textedit_str.append(close_font_tag);
                dir_empty_n++;
            }
            if(lib_dir_info.at(c)==QChar::fromLatin1('2'))
            {
                textedit_str.append(lib_dir_str.section("\n",c,c));
            }
            textedit_str.append("<br />");
            c++;
        }
        //---------------------------------- info about lib dir label
        info_str.append(QString::number(dir_entries_n));
        info_str.append(" directory names parsed. ");

        info_str.append(red_font_tag);
        info_str.append(QString::number(dir_not_found_n));
        info_str.append(" not found. ");
        info_str.append(close_font_tag);

        info_str.append(yellow_font_tag);
        info_str.append(QString::number(dir_empty_n));
        info_str.append(" empty. ");
        info_str.append(close_font_tag);
    }else{
        LOADED.LIB_DIR=false;
        if(st==1)
        {
            info_str.append("lib_dir.txt couldn't be opened");
        }
        if(st==0)
        {
            info_str.append("lib_dir.txt file not found");
        }
    }
    //---------
    lib_dir_label->setText(info_str);
    PREV_LIB_DIR_STR=textedit_str;
    lib_dir_textedit->setText(textedit_str);
}
// ----------------------------------- (from main_engine) SL RECEIVE LIB DIR ENTRIES
void MainControl::sl_receive_lib_dir_entries(int check, int lib_dir_entries_n)
{
    QString entries_str("");
    if(check==0)
    {
        LOADED.LIB_DIR_ENTRIES=false;
        entries_str.append("Entries file not found.");
    }
    if(check==1)
    {
        LOADED.LIB_DIR_ENTRIES=false;
        entries_str.append("Couldn't open lib_dir_entries.txt");
    }
    if(check==2)
    {
        entries_str.append(QString::number(lib_dir_entries_n));
        entries_str.append(" entries found.");
        if(lib_dir_entries_n>0)
        {
            LOADED.LIB_DIR_ENTRIES=true;
        }else{
            LOADED.LIB_DIR_ENTRIES=false;
        }

    }
    list_lib_dir_entries_label->setText(entries_str);
}
// ----------------------------------- (from main_engine) SL RECEIVE DISASS LIBS
void MainControl::sl_receive_disass_lib(int disass_lib)
{
    QString disass_str("");
    if(disass_lib>0)
    {
        disass_str.append(QString::number(disass_lib));
        disass_str.append(" entries in disass lib folder");
        LOADED.DISASS_LIB=true;
    }else{
        disass_str.append("No disassembed libraries");
        LOADED.DISASS_LIB=false;
    }
    disass_lib_label->setText(disass_str);
}
// ----------------------------------- (from main_engine) SL RECEIVE INDEX CHECK
void MainControl::sl_receive_index_check(int check,int index_size)
{
    QString index_str("");
    if(check==0)
    {
        index_str.append("Index file not found.");
        LOADED.INDEX=false;
    }
    if(check==1){
        index_str.append("Couldn't open index file.");
        LOADED.INDEX=false;
    }
    if(check==2){
        index_str.append("Index file found: ");
        index_str.append(QString::number(index_size));
        index_str.append(" entries long");
        LOADED.INDEX=true;
    }
    index_label->setText(index_str);
}
// ----------------------------------- (from main_engine) SL RECEIVE STATUS
void MainControl::sl_receive_status(int status, QString complement)
{
    QString st_str("");
    st_str.append(QString::number(status));
    st_str.append(" - ");

    if(status==0){st_str.append("Stand by.");}
    if(status==1){st_str.append("Preloading...");}

    if(status==11){st_str.append("Library paths found and loaded.");}
    if(status==12){st_str.append("Couldn't open library paths.");}
    if(status==13){st_str.append("Library paths not found.");}

    if(status==20){st_str.append("Library paths successfully saved.");}
    if(status==21){st_str.append("Couldn't write to library paths.");}

    if(status==30){st_str.append("Library entries found and loaded.");}
    if(status==31){st_str.append("Couldn't open library entries.");}
    if(status==32){st_str.append("Library entries not found.");}

    if(status==34){st_str.append("Generating library entries.");}
    if(status==35){st_str.append("Couldn't write to library entries.");}
    if(status==36){st_str.append("Library entries successfully generated.");}

    if(status==40){st_str.append("Disassembled entries found: ");}
    if(status==42){st_str.append("Couldn't open library entries.");}
    if(status==43){st_str.append("Couldn't open ");}
    if(status==44){st_str.append("Disassembling... ");}
    if(status==45){st_str.append("Writing to file... ");}
    if(status==46){st_str.append("Files disassembled: ");}

    if(status==50){st_str.append("Index not found.");}
    if(status==51){st_str.append("Index found.");}

    if(status==60){st_str.append("Couldn't write to Index.");}
    if(status==61){st_str.append("Parsing index: ");}
    if(status==62){st_str.append("Library entries parsed: ");}
    if(status==63){st_str.append("Parsing local functions... ");}
    if(status==64){st_str.append("Sorting index... ");}
    if(status==65){st_str.append("Writing index to file... ");}
    if(status==66){st_str.append("Sorting sublists: ");}
    if(status==67){st_str.append("Error opening index file... ");}
    if(status==68){st_str.append("Functions parsed to index: ");}


    if(status==70){st_str.append("Origin file name empty. Select a file for expanding.");}
    if(status==77){st_str.append("Destiny directory name empty. Select a destiny directory.");}
    if(status==71){st_str.append("otool failed.");}
    if(status==72){st_str.append("Failed to create diasssembled '0'.");}
    if(status==73){st_str.append("Failed to copy origin file.");}
    if(status==74){st_str.append("Expanding: ");}
    if(status==75){st_str.append("Intermediary file access error. ");}
    if(status==76){st_str.append("Finished expanding. ");}

    if(status==98){st_str.append("...Canceling... ");}
    if(status==99){st_str.append("...Canceled... ");}

    if(complement.length()>70)
    {
        complement.insert(45," . . .\n . . . ");
    }else{
        complement.append("\n");
    }

    st_str.append(complement);
    if(!complement.isEmpty()){
        st_str.append(".");
    }
    status_label->setText(st_str);

}
// ----------------------------------- (from main_engine) SL RECEIVE PROGRESS
void MainControl::sl_receive_progress(uint progress)
{
    QString str("");
    if(progress<101)
    {
        str.append(QString::number(progress));
        str.append(" %");
    }
    if(progress==102)
    {
        str="-------";
        str.insert(progress_sig,"o");
        progress_sig++;
        if(progress_sig>7){progress_sig=0;}

    }
    if(progress==103)
    {
        str="--------";
    }
    //str.append(QString::number(progress));
    progress_label->setText(str);
}
// ----------------------------------- (from main_engine_thread) SL RECEIVE PROGRESS
void MainControl::sl_process_finished()
{
    IS_PRELOADING=false;
    IS_PROCESSING=false;
    set_interface_enabled();
    sl_receive_progress(103);
    if(CLOSED)
    {
        delete_properties();
        exit(0);
    }

}
