#include "mainengine.h"
MainEngine::MainEngine(QObject *parent) : QThread(parent)
{
    init_properties();
}
// ------------------------------------------------------- DESTRUCTOR
MainEngine::~MainEngine()
{
    delete LIB_DIR_STR;
    delete LIB_DIR_INFO;
    delete ORIG_FILE_NAME;
    delete DEST_DIR_NAME;
}

// ------------------------------------------------------- INIT PROPERTIES
void MainEngine::init_properties()
{
    PROCEED = false;
    LIB_DIR_STR = new QString;
    LIB_DIR_INFO = new QString;
    ORIG_FILE_NAME = new QString;
    DEST_DIR_NAME = new QString;
    LIB_DIR_ENTRIES_N=0;
}
// ------------------------------------------------------- WRITE TO FILE
void MainEngine::write_to_file(QString content, QFile * file)
{
    QDataStream stream(file);
    long content_len=content.length();
    long c=0;
    uchar uch;
    char ch;
    if(file->isOpen() && file->isWritable())
    {
        while(c<content_len)
        {
            uch = content.at(c).unicode();
            ch=uch;
            stream.writeRawData(&ch,1);
            c++;
        }
    }
}

// ------------------------------------------------------- CHAR TO INT
uint MainEngine::char_to_int(uchar ch[], uint length, int endianness)
{

    uint result=0;
    int c;
    uint factor;

    if(endianness == LITTLE_ENDIAN)
    {
        c=0;
        factor=1;
        while(c<length)
        {
            result += ch[c]*factor;
            factor *= 256;
            c++;
        }
    }

    if(endianness == BIG_ENDIAN)
    {

        c=(length-1);
        factor=1;

        while(c>=0)
        {
            result += ch[c]*factor;
            factor *= 256;
            c--;
        }

    }
    return result;
}
// ------------------------------------------------------- INIT TO CHAR
void MainEngine::int_to_char(uchar ch[], uint value, uint length, int endianness)
{
    int c=1;
    uint div=1;
    uint rest=0;
    while(c<length)
    {
        div *= 256;
        c++;
    }
    if(endianness == BIG_ENDIAN)
    {
        c=0;
        while(c<(length-1))
        {
            ch[c] = value/div;
            rest = value%div;
            div /= 256;
            c++;
        }
        ch[c] = rest;
    }

    if(endianness == LITTLE_ENDIAN)
    {
        c=(length-1);
        while(c>0)
        {
            ch[c] = value/div;
            rest = value%div;
            div /= 256;
            c--;
        }
        ch[c] = rest;
    }
}
// ------------------------------------------------------- C STRING TO QSTRING
QString MainEngine::cstr_to_qstr(char *cstr,int cstr_len)
{
    int c=0;
    QString result("");
    while(c<cstr_len)
    {
        result.append(cstr[c]);
        c++;
    }
    return result;
}

// ------------------------------------------------------- COPY FILE
bool MainEngine::copy_file(QFile *orig_file, QFile *dest_file)
{
    QDataStream read_stream(orig_file);
    QDataStream write_stream(dest_file);
    long c=0;
    long orig_size=orig_file->size();
    char ch=0;
    bool result=false;
    if(orig_file->isOpen() && orig_file->isReadable() && dest_file->isOpen() && dest_file->isWritable())
    {
        while(c<orig_size)
        {
            read_stream.readRawData(&ch,1);
            write_stream.writeRawData(&ch,1);
            c++;
        }
        result=true;
    }
    return result;
}
// ------------------------------------------------------- WRITE NEST MARK TO STRING
void MainEngine::write_nest_mark_to_str(QString *str, int n, QString mark)
{
    int c=0;
    while(c<n)
    {
        str->prepend(mark);
        c++;
    }
    if(n>0){str->prepend(QString::number(n));}
}

// ------------------------------------------------------- GET UINT FROM FILE
uint MainEngine::get_uint_from_file(QFile *file, bool get_lf_after_uint)
{
    uint result=0;
    QDataStream stream(file);
    char ch;
    uchar ch_arr[4]={0,0,0,0};
    int c=0;
    if(file->isOpen() && file->isReadable())
    {
        while(c<4 && !file->atEnd())
        {
            stream.readRawData(&ch,1);
            ch_arr[c]=ch;
            c++;
        }
        if(get_lf_after_uint && !file->atEnd())
        {
            stream.readRawData(&ch,1);
        }
        result=char_to_int(ch_arr,4,BIG_ENDIAN);
    }
    return result;
}

// ------------------------------------------------------- GET LINE FROM FILE
// this function DOES NOT returns the string with the line feed it ends with
int MainEngine::get_line_from_file(QString *line_str, QFile *file)
{
    *line_str="";
    QDataStream read_stream(file);
    char ch=0;
    int pos=0;
    bool first=true;

    if(file->isOpen() && file->isReadable())
    {


        while(!file->atEnd())
        {
            if(first)
            {
                pos=file->pos();
                first=false;
            }
            read_stream.readRawData(&ch,1);
            if(ch==10)
            {
                return pos;
            }else{
                line_str->append(ch);
            }
        }

    }

    return pos;
}
// ------------------------------------------------------- GET FUNCTION IMP POS FROM STR
int MainEngine::get_imp_pos_from_str(QString *line_str)
{
    int len=line_str->length();
    int c=0;
    int num=0;
    int res=0;
    int fac=1000000000;
    if(len>=10)
    {
        while(c<10)
        {
            num=line_str->at(c).unicode();
            num-=48;
            num*=fac;

            res+=num;
            fac/=10;
            c++;
        }
    }
    return res;
}

// ------------------------------------------------------- CAT NEST FILE NAME
QString MainEngine::cat_nest_file_name(QString *base, int nest_level,QString *extension)
{
    QString result("");
    result.append(DEST_DIR_NAME);
    result.append("/");
    result.append(*base);
    result.append("_NEST_");
    result.append(QString::number(nest_level));
    if(!extension->isEmpty())
    {
        result.append(".");
        result.append(*extension);
    }
    return result;
}
// ------------------------------------------------------- CAT INDEX LINE
QString MainEngine::cat_index_line(int func_pos,QString *lib_name,QString *func_name)
{
    QString result("");

    int rest=func_pos;
    char digit;
    int c=0;
    int fac=1000000000;
    while(c<10)
    {
        digit=rest/fac;
        digit+=48;
        result.append(digit);

        rest=rest%fac;
        fac/=10;
        c++;
    }
    result.append(*lib_name);
    result.append("/");
    result.append(*func_name);

    return result;
}

// ------------------------------------------------------- STRIP FILE NAME
void MainEngine::strip_file_name(QString *file_name, QString *base, QString *extension)
{
    *base=file_name->section("/",-1,-1);
    *extension=base->section(".",-1,-1);

    int bas_len=base->length();
    int ext_len=extension->length();

    if(ext_len>bas_len-2)
    {
        ext_len=0;
        *extension="";
    }
    *base=base->remove(bas_len-ext_len,ext_len);

}
// ------------------------------------------------------- WRITE LINE TO FILE
void MainEngine::write_line_to_file(QString line_str, QFile *file, bool lf_at_end)
{
    QDataStream write_stream(file);
    uchar uch;
    char ch=0;
    long line_len=line_str.length();
    long c=0;

    if(file->isOpen() && file->isWritable())
    {

        while(c<line_len)
        {
            uch = line_str.at(c).unicode();
            ch=uch;
            write_stream.writeRawData(&ch,1);
            c++;
        }
        if(lf_at_end)
        {
            ch=10;
            write_stream.writeRawData(&ch,1);
        }
    }

}
// ------------------------------------------------------- WRITE NUMBER TO FILE
void MainEngine::write_number_to_file(uint num, int char_len, int endianness, QFile *file,bool lf_at_end)
{
    if(file->isOpen() && file->isWritable())
    {
        uchar * ch_arr = new uchar[char_len];
        int_to_char(ch_arr,num,char_len,endianness);

        QDataStream stream(file);
        char ch;

        int c=0;
        while(c<char_len)
        {
            ch=ch_arr[c];
            stream.writeRawData(&ch,1);
            c++;
        }
        if(lf_at_end)
        {
            ch=10;
            stream.writeRawData(&ch,1);
        }

        delete[] ch_arr;
    }
}

// ------------------------------------------------------- PARSE FUNC NAME (from function call)
QString MainEngine::parse_func_name(QString line_str)
{
    //Those are the characters the function names may start with
    //'-+$.' and any letter in both uppercase and lowercase
    // but functions may have spaces(32) inside '[]' thanks
    // to the sadists who made the apple's frameworks, NS in particular
    QString func_name("");
    if(line_str.contains(NOTE_MARK)){return func_name;}
    QString first_section=line_str.section("#",0,0);
    if(first_section.isEmpty()){return func_name;}
    if(first_section.contains("call"))
    {
        //func_name.prepend("1: ");
        QString before_call=line_str.section("call",0,0);
        QString after_call=line_str.section("call",1,-1);
        if(before_call.isEmpty() || after_call.length()<3)
        {
            //func_name.prepend("2: ");
            return func_name;
        }
        char bef_ch=before_call.at(before_call.length()-1).unicode();
        char aft_ch=after_call.at(0).unicode();
        char aft_aft_ch=after_call.at(1).unicode();
        if(is_space(bef_ch) && (is_space(aft_ch) || (aft_ch=='q' && is_space(aft_aft_ch))))
        {
            if(aft_ch=='q')
            {
                after_call.remove(0,1);
            }
            if(after_call.contains("stub for:"))
            {
                after_call=after_call.section("stub for:",-1,-1);
            }
            QString rest("");
            after_call=after_call.trimmed();
            func_name=after_call.section("]",0,-2,QString::SectionIncludeTrailingSep);
            rest=after_call.section("]",-1,-1);
            func_name.append(rest.section(" ",0,0));
        }

    }
    if(!func_name.isEmpty())
    {
        char first_ch=func_name.at(0).unicode();
        if(first_ch=='*' || first_ch=='%')
        {
            func_name=CALL_REG_MARK;
        }
        if(is_digit(first_ch))
        {
            func_name=CALL_CODE_MARK;
        }
    }
    return func_name;

}
// ------------------------------------------------------- IS SPACE
bool MainEngine::is_space(char ch)
{
    if(ch==' ' || ch=='\t' || ch==32 || ch==9)
    {
        return true;
    }else{
        return false;
    }
}
// ------------------------------------------------------- IS DIGIT
bool MainEngine::is_digit(char ch)
{
    if(ch>47 && ch<58)
    {
        return true;
    }else{
        return false;
    }
}
// ------------------------------------------------------- IS FUNC IMP
// pass the string without trailing line feed
bool MainEngine::is_func_imp(QString line_str, bool count_main_as_imp)
{
    bool result=false;
    int str_len=line_str.length();
    char first_ch=0;
    if(str_len>0)
    {
        if(line_str!="_main:" || count_main_as_imp)
        {
            first_ch=line_str.at(0).unicode();
            if(!is_digit(first_ch) && line_str.at(str_len-1).unicode()==':')
            {
                result=true;
            }
        }
    }
    return result;
}
// ------------------------------------------------------- COUNT FILE LF
int MainEngine::count_file_lines(QFile *file)
{
    int count=0;
    QDataStream stream(file);
    char ch;
    if(file->isOpen() && file->isReadable())
    {

        while(!file->atEnd() && PROCEED)
        {
            PROCEED=get_proceed();
            stream.readRawData(&ch,1);
            if(ch==10)
            {
                count++;
            }else{

            }
        }
    }else{
        count=(-1);
    }
    return count;
}
// ------------------------------------------------------- QSTRING TO C LIKE STRING
int qstring_to_str(QString *orig_str,char *dest_str)
{
    int len=orig_str->length();
    int c=0;
    while(c<len)
    {
        dest_str[c]=orig_str->at(c).unicode();
        c++;
    }
    dest_str[c]=0;
    c++;

    return c;
}

// ------------------------------------------------------- PROCESS PROGRESS
void MainEngine::process_progress(ulong total_size, ulong cur_pos)
{
    if(total_size)
    {
        ulong unit=total_size/100;
        if(unit==0){unit=1;}
        ulong progress=cur_pos/unit;
        emit sig_send_progress(progress);
    }else{
        emit sig_send_progress(102);
    }
}
// ------------------------------------------------------- STR TO UINT
uint MainEngine::str_to_uint(QString str)
{
    int fac=1;
    int c=str.length()-1;
    uint num=0;
    uint result=0;
    while(c>=0)
    {
        num=str.at(c).unicode();
        num-=48;

        result+=(fac*num);

        fac*=10;
        c--;
    }
    return result;
}
// ------------------------------------------------------- UINT TO STR
QString MainEngine::uint_to_str(uint num)
{
    QString result("");
    uint div=1000000000;
    uint rest=num;
    char ch=0;
    int c=0;
    while(c<10)
    {
        ch=(rest/div)+48;
        result.append(ch);
        rest=rest%div;
        div/=10;

        c++;
    }
    return result;
}
// ------------------------------------------------------- PARSE INDEX TO ARR
int MainEngine::parse_index_to_arr(QFile *orig_file, QList<INDEX_STRUCT> *list, int lib)
{

    int res=(-1);

    if(orig_file->isOpen() && orig_file->isReadable())
    {
        res=0;
        QString line_str;
        bool first_line=true;
        char first_ch=0;
        char last_ch=0;
        int line_len=0;
        int func_pos=0;
        INDEX_STRUCT func_struct;

        // --------------------- progress
        int orig_file_size=orig_file->size();
        int prog_size_min=12000000;
        bool show_percentage=false;
        int cur_prog=0;
        if(orig_file_size>prog_size_min){show_percentage=true;}
        int p=0;
        process_progress(orig_file_size,0);

        while(!orig_file->atEnd() && PROCEED)
        {
            PROCEED=get_proceed();
            if(p%PARSE_INDEX_PROGRESS_UNIT==0)
            {
                if(show_percentage)
                {
                    process_progress(orig_file_size,orig_file->pos());
                }else{
                    process_progress(0,0);
                }
            }
            get_line_from_file(&line_str,orig_file);

            if(!first_line)
            {
                line_str=line_str.section("#",0,0);
                line_len=line_str.length();
                if(line_len>1)
                {
                    first_ch=line_str.at(0).unicode();
                    last_ch=line_str.at(line_len-1).unicode();
                    if(!is_digit(first_ch) && last_ch==':')
                    {
                        func_pos=orig_file->pos()-(line_len+1);

                        func_struct.FUNC_NAME=line_str;
                        func_struct.LIB=lib;
                        func_struct.POS=func_pos;
                        list->append(func_struct);
                        res++;
                    }
                }
            }else{
                first_line=false;
            }

            p++;
        }// end loop
    }

    return res;
}
// ------------------------------------------------------- DISASSEMBLE
int MainEngine::disassemble(QString file_name, QFile *dest_file)
{
    if(dest_file->isOpen() && dest_file->isWritable())
    {
        QString disass_str("");
        QStringList arg_list;
        QProcess process;
        process.setProgram(OTOOL_PATH);
        arg_list.insert(0,"-tV");
        arg_list.insert(1,file_name);
        process.setArguments(arg_list);

        process.start();
        process.waitForFinished(300000);

        if(process.exitStatus()!=QProcess::NormalExit)
        {
            return -1;
        }
        if(process.exitCode()!=0)
        {
            return -2;
        }

        disass_str=process.readAllStandardOutput();

        if(!disass_str.isEmpty())
        {
            write_to_file(disass_str,dest_file);
            return 0;
        }else{
            return -3;
        }
    }else{
        return -4;
    }
}

// ------------------------------------------------------- LOAD LIB DIR

void MainEngine::load_lib_dir(bool send_st)
{

    QFile lib_dir_file(LIB_DIR_PATH);
    QDataStream stream_lib_dir(&lib_dir_file);
    QString str("");
    QString lib_dir_str("");
    QString lib_dir_info("");
    QDir dir;
    char ch;
    int file_c=0;
    bool get_line=false;
    bool first=true;

    if(lib_dir_file.exists())
    {
        if(lib_dir_file.open(QIODevice::ReadOnly))
        {

            while(!lib_dir_file.atEnd() && file_c < 20000 && PROCEED)
            {
                PROCEED=get_proceed();
                if(!file_c%10)
                {
                    emit sig_send_progress(102);
                }
                    get_line_from_file(&str,&lib_dir_file);
                    str=str.section("#",0,0);
                    str=str.simplified();
                    if(!str.isEmpty())
                    {
                        if(!first)
                        {
                            lib_dir_str.append("\n");
                        }
                        first=false;
                        lib_dir_str.append(str);
                        dir.setPath(str);
                        if(dir.exists())
                        {
                            if(dir.entryList(QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot).count()>0)
                            {
                                lib_dir_info.append("2");
                            }else{
                                lib_dir_info.append("1");
                            }
                        }else{
                            lib_dir_info.append("0");
                        }
                        str="";
                    }//end if str not empty
                //}//end if get_line

                file_c++;
            }
            *LIB_DIR_STR=lib_dir_str;
            *LIB_DIR_INFO=lib_dir_info;

            if(send_st){emit sig_send_status(11,"");}
            emit sig_send_lib_dir(2,lib_dir_info,lib_dir_str);
            lib_dir_file.close();
        }else{
            //Couldn't open lib_dir.txt
            if(send_st){emit sig_send_status(12,"");}
            emit sig_send_lib_dir(1,lib_dir_info,lib_dir_str);
        }
    }else{
        //lib_dir.txt doesn't exist
        if(send_st){emit sig_send_status(13,"");}
        emit sig_send_lib_dir(0,lib_dir_info,lib_dir_str);
    }

}
// ------------------------------------------------------- SAVE LIB DIR
void MainEngine::save_lib_dir()
{
    QFile file(LIB_DIR_PATH);
    QDataStream stream(&file);
    uchar uch;
    char ch;
    uint c=0;

    if(file.open(QIODevice::WriteOnly))
    {
        while(c<LIB_DIR_STR->length() && c<5000)
        {
            uch = LIB_DIR_STR->at(c).unicode();
            ch=uch;
            stream.writeRawData(&ch,1);
            c++;
        }
        file.close();
        emit sig_send_status(20,"");
    }else{
        emit sig_send_status(21,"");
    }
}

// ------------------------------------------------------- CHECK LIB DIR ENTRIES FILE
void MainEngine::check_lib_dir_entries_file(bool send_st)
{
    QFile file(LIB_DIR_ENTRIES_PATH);
    int ln=0;
    QString str("");    
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            // ------ progress info
            int file_size=file.size();
            int prog_size_min=20000000;
            bool show_percentage=false;
            if(file_size>prog_size_min){show_percentage=true;}
            int p=0;

            while(!file.atEnd())
            {
                if(ln%300)
                {
                    if(show_percentage)
                    {
                        process_progress(file_size,file.pos());
                    }else{
                        process_progress(0,0);
                    }
                }
                get_line_from_file(&str,&file);
                str=str.trimmed();
                if(!str.isEmpty())
                {
                    ln++;
                }

            }
            file.close();
            if(send_st){emit sig_send_status(30,"");}
            LIB_DIR_ENTRIES_N=ln;
            emit sig_send_lib_dir_entries(2,ln);
        }else{
            if(send_st){emit sig_send_status(31,"");}
            emit sig_send_lib_dir_entries(1,0);
        }
    }else{
        if(send_st){emit sig_send_status(32,"");}
        emit sig_send_lib_dir_entries(0,0);
    }
}
// ------------------------------------------------------- MAKE LIB DIR ENTRIES FILE
void MainEngine::make_lib_dir_entries_file()
{
    QStringList dir_list=LIB_DIR_STR->split("\n",QString::SkipEmptyParts);
    QStringList dir_info=LIB_DIR_INFO->split("",QString::SkipEmptyParts);
    int dn=dir_list.count();
    int dc=0;
    int entries_res=0;
    int entries_n=0;

    int ic=0;
    int proc_c=0;
    QString proc_str("");

    QDirIterator * dir_it;
    QFile entry_item_file;
    QFile entries_file(LIB_DIR_ENTRIES_PATH);

    uint lib_meta_arr[META_N]={META_NUMBER_0,
                         META_NUMBER_1,
                         META_NUMBER_2,
                         META_NUMBER_3,
                         META_NUMBER_4,
                         META_NUMBER_5};

    bool is_lib=false;
    uint meta_num=0;
    int c=0;

    if(entries_file.open(QIODevice::WriteOnly))
    {
        emit sig_send_status(34,"");
        while(dc<dn)
        {
            if(dir_info.at(dc)=="2")
            {
                dir_it = new QDirIterator(dir_list.at(dc),QDir::Files|QDir::NoDotAndDotDot,QDirIterator::Subdirectories|QDirIterator::FollowSymlinks);

                while(dir_it->hasNext() && PROCEED)
                {
                    PROCEED=get_proceed();
                    entry_item_file.setFileName(dir_it->next());
                    if(entry_item_file.exists())
                    {
                        if(entry_item_file.open(QIODevice::ReadOnly))
                        {
                            meta_num=get_uint_from_file(&entry_item_file,false);

                            is_lib=false;
                            c=0;
                            while(c<META_N)
                            {
                                if(meta_num==lib_meta_arr[c])
                                {
                                    is_lib=true;
                                }
                                c++;
                            }

                            entry_item_file.close();
                        }
                        if(is_lib)
                        {
                            write_line_to_file(entry_item_file.fileName(),&entries_file,true);
                            entries_n++;
                        }

                    }
                    if(ic%500==0)
                    {
                        emit sig_send_progress(102);
                    }
                    ic++;
                }

                delete dir_it;
            }
            dc++;
        }//end lib dir iterator
        entries_file.close();
        emit sig_send_status(36,"");
    }else{
        emit sig_send_status(35,"");
    }
}
// ------------------------------------------------------- CHECK DISASS LIB
void MainEngine::check_disass_lib(bool send_st)
{
    QDir dir(DISASS_LIB_PATH);
    int disass_lib_n = 0;
    disass_lib_n = dir.entryList(QDir::Files|QDir::NoDotAndDotDot).count();
    emit sig_send_disass_lib(disass_lib_n);
    if(send_st){emit sig_send_status(40, QString::number(disass_lib_n));}
}
// ------------------------------------------------------- DISASS LIBS
void MainEngine::disass_lib()
{
    QFile entries_file(LIB_DIR_ENTRIES_PATH);
    QString entry_filename("");
    QFile entry_file;
    QFile dest_file;
    QString disass_str;

    QDataStream read_stream(&entry_file);
    bool proceed_disass=true;//used with the overwrite homonymous setting
    QString disass_destiny_path("");
    int dis_n=0;

    QString st_str("");

    if(entries_file.open(QIODevice::ReadOnly))
    {
        while(!entries_file.atEnd() && PROCEED)
        {
            PROCEED=get_proceed();
            get_line_from_file(&entry_filename,&entries_file);
            entry_filename=entry_filename.trimmed();
            entry_file.setFileName(entry_filename);

            disass_destiny_path=DISASS_LIB_PATH;
            disass_destiny_path.append(entry_filename.section("/",-1,-1));

            proceed_disass=true;
            dest_file.setFileName(disass_destiny_path);
            if(!SET.OVERWRITE_DIS && dest_file.exists())
            {
                proceed_disass=false;
            }

            if(proceed_disass)
            {
                    //------------------ OTOOL
                if(dest_file.open(QIODevice::WriteOnly))
                {

                    st_str=" ( of ) ";
                    st_str.insert(2,QString::number(dis_n));
                    st_str.insert(st_str.length()-2,QString::number(LIB_DIR_ENTRIES_N));
                    st_str.append(entry_filename);

                    emit sig_send_status(44,st_str);
                    if(disassemble(entry_filename,&dest_file))
                    {
                        dis_n++;
                    }

                    dest_file.close();
                }else{
                    //emit sig_send_status(43, entry_filename);//too fast to show that
                }
            }//if can overwrite or not repeated
        }
        //emit sig_send_progress(cc);
        emit sig_send_status(46,QString::number(dis_n));
        entries_file.close();
    }else{
        emit sig_send_status(42,"");
    }
}
// ------------------------------------------------------- CHECK INDEX
void MainEngine::check_index(bool send_st)
{
    QFile file(INDEX_PATH);
    QString str("");
    int check=0;
    int entry_n=0;
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            check=2;
            entry_n=count_file_lines(&file);
            file.close();
        }else{
            check=1;
        }

        if(send_st){emit sig_send_status(51,"");}
    }else{
        if(send_st){emit sig_send_status(50,"");}
    }
    emit sig_send_index_check(check,entry_n);
}

// ------------------------------------------------------- MAKE INDEX
void MainEngine::make_index()
{

    QDir disass_dir(DISASS_LIB_PATH);
    QStringList disass_list = disass_dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
    QFile lib_file;
    QString lib_filename;

    ulong func_n=0;

    ulong disass_entries_n = disass_list.count();
    ulong disass_entries_c=0;
    int parsed_libs=0;
    ulong parsed=0;
    // ------------------- MAIN PRESORTING VARIABLES
    int psort_list_n=0;// number of lists
    int psort_valid_list_n=0;// number of lists containing elements
    QList<INDEX_STRUCT> *psort_list[PSORT_LIST_MAX_N];// pointers to every sublist

    //holds the status of each list(1 is the list with the elements, 0 is a clear one)
    char psort_list_st[PSORT_LIST_MAX_N];
    int psort_list_nest[PSORT_LIST_MAX_N];
    char psort_ch[PSORT_LIST_MAX_N];//the letter that sorts every sublist
    int psort_par[PSORT_LIST_MAX_N];
    int psort_list_count[PSORT_LIST_MAX_N];
    INDEX_STRUCT small_elm;
    bool has_small_elm=false;

    QList<int> psort_ord;// order in which lists will be ordered
    int nest_c=0;// it indicates the current letter being evaluated(first,second...)

    // already found letters, reseted every nesting
    QList<char> already_list;
    int already_n=0;
    int already_c=0;
    bool already=false;

    QString func_name;
    int ch=0;

    int func_c=0;
    int aux_c=0;

    //initializing first list
    psort_list[0]=new QList<INDEX_STRUCT>;
    psort_list_n=1;
    psort_list_st[0]=1;

    QString st_str;
    while(disass_entries_c<disass_entries_n && PROCEED)
    {
        PROCEED=get_proceed();
        lib_filename=disass_list.at(disass_entries_c);
        lib_filename.prepend(DISASS_LIB_PATH);
        lib_file.setFileName(lib_filename);
        if(lib_file.open(QIODevice::ReadOnly))
        {
            st_str="( ";
            st_str.append(QString::number(disass_entries_c+1));
            st_str.append(" out of ");
            st_str.append(QString::number(disass_entries_n));
            st_str.append(") ");
            emit sig_send_status(61,st_str);
            parsed=parse_index_to_arr(&lib_file,psort_list[0],disass_entries_c);
            func_n+=parsed;
            if(parsed>0)
            {
                parsed_libs++;
            }

            lib_file.close();
        }
        disass_entries_c++;
    }

    bool first_sublists=false;
    PROCEED=get_proceed();
    if(PROCEED)
    {
        aux_c=1;
        while(aux_c<PSORT_LIST_MAX_N)
        {
            psort_list_st[aux_c]=0;
            aux_c++;
        }
        psort_valid_list_n=1;
        psort_ord.insert(1,0);
        psort_ch[0]=0;
        psort_list_nest[0]=(-1);
        psort_par[0]=(-1);
        psort_list_count[0]=func_n;

        // ------------------------------ check letters beyond

        int list_n=0;
        int sublist_i=0;
        int ord=0;
        int elm_i=0;
        bool has_min=true;
        bool in_limit=true;
        while(ord < psort_list_n && in_limit && PROCEED)
        {
            PROCEED=get_proceed();
            elm_i=psort_ord.at(ord);
            if(psort_list_st[elm_i]==1)
            {
                if(psort_list[elm_i]->count() > PSORT_TRIGGER_N)
                {
                    if(psort_list_n>PSORT_LIST_MAX_N-150){in_limit=false;}

                    list_n=psort_list[elm_i]->count();

                    already_list.clear();
                    already_n=0;

                    sublist_i=psort_list_n;

                    nest_c=psort_list_nest[elm_i]+1;

                    has_small_elm=false;

                    first_sublists=false;
                    //2265898
                    // loop for making sublists for further sorting
                    func_c=0;
                    while(func_c < list_n)
                    {
                        func_name=psort_list[elm_i]->at(func_c).FUNC_NAME;

                        has_min=true;
                        if(func_name.length() < (nest_c+1)){has_min=false;}

                        if(has_min)
                        {
                            ch=func_name.at(nest_c).unicode();

                            //already has list for character
                            already=false;
                            already_c=0;
                            while(already_c<already_n)
                            {
                                if(ch==already_list.at(already_c))
                                {
                                    psort_list[sublist_i+already_c]->append(psort_list[elm_i]->at(func_c));
                                    already=true;
                                    break;
                                }
                                already_c++;
                            }
                            //make new list for character
                            if(already==false)
                            {

                                already_list.append(ch);
                                already_n++;

                                psort_ch[psort_list_n]=ch;

                                psort_par[psort_list_n]=elm_i;

                                psort_list_st[psort_list_n]=1;
                                psort_list_nest[psort_list_n]=nest_c;

                                //INSERT
                                // notice already_n is already incremented
                                psort_ord.insert(ord+already_n,psort_list_n);

                                psort_list[psort_list_n]=new QList<INDEX_STRUCT>;
                                psort_list[psort_list_n]->append(psort_list[elm_i]->at(func_c));

                                psort_list_n++;
                                psort_valid_list_n++;

                            }

                        }else{
                            //element which doesn't have next character
                            small_elm=psort_list[elm_i]->at(func_c);
                            has_small_elm=true;

                        }

                        func_c++;
                    }// end sublist's loop

                    //clear and delete parent lists no longer needed
                    psort_list_count[elm_i]=psort_list[elm_i]->count();
                    psort_list[elm_i]->clear();
                    psort_list_st[elm_i]=0;
                    delete psort_list[elm_i];
                    psort_valid_list_n--;

                    // SORT SUBLIST'S ORDER
                    int x=ord+1;
                    int y=ord+2;
                    char ch_x=0;
                    char ch_y=0;
                    while(x<ord+already_n && PROCEED)
                    {

                        while(y<ord+already_n+1)
                        {
                            ch_x=psort_ch[psort_ord.at(x)];
                            ch_y=psort_ch[psort_ord.at(y)];
                            if(ch_y<ch_x)
                            {
                                psort_ord.swap(y,x);
                            }
                            y++;
                        }
                        x++;
                        y=(x+1);
                    }


                    if(has_small_elm)
                    {
                        //put on the first list in order
                        psort_list[psort_ord[ord+1]]->prepend(small_elm);
                        has_small_elm=false;
                    }

                }//end if has lots of functions
            }
            ord++;
        }
    }
    PROCEED=get_proceed();
    // ----------------------- sorting
    if(PROCEED && func_n>1)
    {
        ulong x=0;
        ulong y=1;
        bool show_percentage=false;
        ulong total_cycles;
        ulong cycles_c=0;

        int ord=0;
        int elm_i=0;
        ulong list_n=0;

        int psort_valid_list_c=0;

        emit sig_send_status(64,"");
        emit sig_send_progress(103);

        ord=0;
        while(ord < psort_list_n && PROCEED)
        {
            PROCEED=get_proceed();
            elm_i=psort_ord[ord];

            if(psort_list_st[elm_i]==1)
            {
                psort_valid_list_c++;
                emit sig_send_status(66,st_str);
                list_n=psort_list[elm_i]->count();
                total_cycles=((list_n)*(list_n-1))/2;

                // status
                st_str="( ";
                st_str.append(QString::number(psort_valid_list_c));
                st_str.append(" of ");
                st_str.append(QString::number(psort_valid_list_n));
                st_str.append(" )");
                emit sig_send_status(66,st_str);


                cycles_c=0;
                if(total_cycles<(10*SORT_INDEX_PROGRESS_UNIT))
                {
                    show_percentage=false;
                }else{
                    show_percentage=true;
                }

                x=0;
                y=1;
                while(x<(list_n-1) && PROCEED)
                {
                    PROCEED=get_proceed();
                    while(y<list_n && PROCEED)
                    {

                        if(cycles_c%SORT_INDEX_PROGRESS_UNIT==0)
                        {

                            if(show_percentage)
                            {
                                process_progress(total_cycles,cycles_c);
                            }else{
                                process_progress(0,0);
                            }
                        }

                        if(psort_list[elm_i]->at(x).FUNC_NAME.compare(psort_list[elm_i]->at(y).FUNC_NAME) > 0)
                        {
                            psort_list[elm_i]->swap(x,y);
                        }

                        y++;
                        cycles_c++;
                    }
                    x++;
                    y=(x+1);
                }

            }


            ord++;

        }// end loop through the sublist's list

    }
    PROCEED=get_proceed();
    // ----------------------- writing to file
    if(func_n>1 && PROCEED)
    {
        QFile index_file(INDEX_PATH);
        if(index_file.open(QIODevice::WriteOnly))
        {
            QString line_str;
            QString func_name;
            QString lib_name;
            int func_pos=0;
            int elm_i=0;
            int ord=0;
            int list_n=0;
            int wrote_func_n=0;

            emit sig_send_status(65,"");

            while(ord < psort_list_n)
            {
                elm_i=psort_ord[ord];
                if(psort_list_st[elm_i]==1)
                {
                    int sublist_c=0;
                    list_n=psort_list[elm_i]->count();
                    while(sublist_c<list_n)
                    {
                        func_name=psort_list[elm_i]->at(sublist_c).FUNC_NAME;
                        lib_name=disass_list.at(psort_list[elm_i]->at(sublist_c).LIB);
                        func_pos=psort_list[elm_i]->at(sublist_c).POS;

                        line_str=cat_index_line(func_pos,&lib_name,&func_name);
                        write_line_to_file(line_str,&index_file,true);
                        wrote_func_n++;
                        sublist_c++;
                    }
                }
                ord++;
            }       
            st_str=QString::number(wrote_func_n);
            st_str.append(" functions parsed from ");
            st_str.append(QString::number(parsed_libs));
            st_str.append(" dependencies.");
            emit sig_send_status(68,st_str);
            index_file.close();
        }else{
            emit sig_send_status(60,"");
        }
    }else{
        st_str=QString::number(0);
        emit sig_send_status(68,st_str);
    }

    //clearing and deleting lists
    aux_c=0;
    while(aux_c<psort_list_n)
    {
        if(psort_list_st[aux_c]==1)
        {
            psort_list[aux_c]->clear();
            delete psort_list[aux_c];
        }
        aux_c++;
    }
}
// ------------------------------------------------------- EXPAND
// this turns out to be the main purpose of this program
//
void MainEngine::expand()
{
    // ------------------- files variables
    QFile orig_file;
    QFile dest_file;
    // ------------------- libraries variables
    QFile LIB_fp;
    // ------------------- index variables
    QFile local_INDEX_fp;
    QFile INDEX_fp;
    local_INDEX_fp.setFileName(LOCAL_INDEX_PATH);
    INDEX_fp.setFileName(INDEX_PATH);
    bool has_index=false;
    QList <INDEX_STRUCT> local_index;
    int local_index_n=0;
    int local_index_c=0;

    QString orig_file_base("");
    QString orig_file_ext("");
    strip_file_name(ORIG_FILE_NAME,&orig_file_base,&orig_file_ext);
    // ------------------- expansion variables
    int nest_c=0;

    QStringList repeated_call_list;
    int rep_c=0;
    bool repeated_call=false;

    QString str("");
    QString line_str("");
    QString func_call("");
    QString index_func_name("");

    QString lib_name("");
    QString lib_path("");
    int imp_pos=0;

    int nest_n=SET.NEST_N;
    if(nest_n==10){nest_n=35;}
    bool proceed_expanding=true;
    bool something_expanded=false;
    int found_in_index=0;
    int which_index=0;
    int index_c=0;
    int expansion_n=0;

    int index_size=0;
    // amount of times the area_spot will be guessed
    int try_n=0;
    int try_c=0;
    // value that compare returns when comparing function name with
    // tried line in index
    int comp=0;
    // beggining and end of the search area of the sorted index
    int area_beg=0;
    int area_end=0;
    int area_len=0;
    // spot chosen to see if the function is there
    // usually right between area_beg and area_end
    int area_spot=0;


    // prev_pos is used to keep the current position in the original code
    // for when the function implementation is being taken
    // from the original code itself
    int prev_pos=0;
    bool in_imp=false;
    bool first_imp_line=false;

    bool expanded=false;
    bool is_call=false;
    bool is_expandable=false;

    QString st_str("");

    // ------ progress info
    int orig_file_size;
    int prog_size_min=20000;
    bool show_percentage=false;
    int p=0;
    PROCEED=get_proceed();
    // ------------------- make nest 0 assembly
    if(SET.ORIG_EXEC)
    {
        orig_file.setFileName(cat_nest_file_name(&orig_file_base,0,&orig_file_ext));
        orig_file.open(QIODevice::WriteOnly);
        if(orig_file.isOpen())
        {
            emit sig_send_status(44,*ORIG_FILE_NAME);
            int dis_ret=disassemble(*ORIG_FILE_NAME,&orig_file);
            if(dis_ret<0)
            {
                emit sig_send_status(71,st_str);
                PROCEED=false;
            }
            orig_file.close();
        }else{
            emit sig_send_status(72,st_str);
            PROCEED=false;
        }
    }else{
        orig_file.setFileName(*ORIG_FILE_NAME);
        dest_file.setFileName(cat_nest_file_name(&orig_file_base,0,&orig_file_ext));
        orig_file.open(QIODevice::ReadOnly);
        dest_file.open(QIODevice::WriteOnly);

        if(copy_file(&orig_file,&dest_file))
        {
            orig_file.close();
            dest_file.close();

            orig_file.setFileName(dest_file.fileName());
        }else{
            if(orig_file.isOpen()){orig_file.close();}
            if(dest_file.isOpen()){dest_file.close();}
            emit sig_send_status(73,"");
            PROCEED=false;
        }
    }

    PROCEED=get_proceed();
    // ------------------- make local index
    if(PROCEED)
    {
        orig_file.open(QIODevice::ReadOnly);

        if(orig_file.isOpen())
        {
            emit sig_send_status(63,"");
            //parse_index_to_file(&orig_file,&local_INDEX_fp,false);
            local_index_n=parse_index_to_arr(&orig_file,&local_index,0);
            if(local_index_n==(-1)){local_index_n=0;}
        }
        if(orig_file.isOpen()){orig_file.close();}
    }
    PROCEED=get_proceed();
    // ------------------- expanding
    if(PROCEED)
    {
        if(INDEX_fp.exists())
        {
            if(INDEX_fp.open(QIODevice::ReadOnly))
            {
                index_size=INDEX_fp.size();
                int rest=index_size;
                try_n=0;
                while(rest>6000)
                {
                    rest/=2;
                    try_n++;
                }
            }
            has_index=true;
        }
        PROCEED=get_proceed();
        nest_c=1;
        while(nest_c<=nest_n && PROCEED && proceed_expanding)
        {
            PROCEED=get_proceed();
            st_str=" (nest  of ) ";
            st_str.insert(7,QString::number(nest_c));
            if(nest_n<20)
            {
                st_str.insert(st_str.length()-2,QString::number(nest_n));
            }else{
                st_str.insert(st_str.length()-2,"...");
            }

            if(!has_index)
            {
                st_str.append(" - index missing - ");
            }

            emit sig_send_status(74,st_str);
            orig_file.setFileName(cat_nest_file_name(&orig_file_base,(nest_c-1),&orig_file_ext));
            dest_file.setFileName(cat_nest_file_name(&orig_file_base,nest_c,&orig_file_ext));

            orig_file.open(QIODevice::ReadOnly);
            dest_file.open(QIODevice::WriteOnly);

            if(!orig_file.isOpen() || !dest_file.isOpen())
            {
                PROCEED=false;
                emit sig_send_status(75,"");

            }else{
                orig_file.seek(0);
                dest_file.seek(0);

                orig_file_size=orig_file.size();
                show_percentage=false;
                if(orig_file_size>prog_size_min){show_percentage=true;}
            }

            something_expanded=false;
            while(!orig_file.atEnd() && PROCEED)
            {
                PROCEED=get_proceed();
                // ----------- progress info
                if(p%EXPAND_PROGRESS_UNIT==0)
                {
                    if(show_percentage)
                    {
                        process_progress(orig_file_size,orig_file.pos());
                    }else{
                        process_progress(0,0);
                    }
                }
                p++;
                // -----------
                get_line_from_file(&line_str,&orig_file);
                func_call=parse_func_name(line_str);

                repeated_call=false;
                found_in_index=0;
                expanded=false;
                is_call=false;
                is_expandable=true;
                // --------------------- from here on means its a function call
                if(!func_call.isEmpty())
                {
                    is_call=true;
                    if(func_call==CALL_REG_MARK || func_call==CALL_CODE_MARK)
                    {
                        is_expandable=false;
                    }else{
                        func_call.append(":");
                    }
                    // ------------------------ check for repeated function calls
                    if(!SET.REPEAT_IMP && is_expandable)
                    {
                        rep_c=0;
                        while(rep_c<repeated_call_list.count())
                        {
                            if(func_call==repeated_call_list.at(rep_c))
                            {
                                repeated_call=true;
                            }
                            rep_c++;
                        }
                        if(!repeated_call)
                        {
                            repeated_call_list.insert(rep_c,func_call);
                        }
                    }

                    // ------------------------- the actual expansion
                    // ------------------------- search in local index

                    if(!repeated_call && is_expandable)
                    {
                        local_index_c=0;
                        while(local_index_c<local_index_n)
                        {
                            str=local_index.at(local_index_c).FUNC_NAME;
                            imp_pos=local_index.at(local_index_c).POS;
                            // ---------------------- found local
                            if(str==func_call)
                            {
                                if(SET.EXPAND_LOCAL && nest_c==1)
                                {
                                    prev_pos=orig_file.pos();
                                    orig_file.seek(imp_pos);
                                    first_imp_line=true;
                                    while(!orig_file.atEnd())
                                    {
                                        get_line_from_file(&str,&orig_file);
                                        if(!first_imp_line && is_func_imp(str,true))
                                        {
                                            break;
                                        }
                                        if(first_imp_line)
                                        {
                                            str.append(" ## (LOCAL)");
                                            first_imp_line=false;
                                        }

                                        write_nest_mark_to_str(&str,nest_c,NEST_MARK);
                                        write_line_to_file(str,&dest_file,true);
                                        expanded=true;
                                        expansion_n++;
                                        something_expanded=true;
                                    }

                                    orig_file.seek(prev_pos);
                                }else{
                                    write_line_to_file(line_str,&dest_file,true);
                                }
                                found_in_index=1;
                            }// end found
                            local_index_c++;
                        }// end loop in local index
                    }// end local index open

                    // ------------------------- search in libraries index
                    if(INDEX_fp.isOpen() && !found_in_index && !repeated_call && is_expandable)
                    {


                        area_beg=0;
                        area_end=index_size-1;
                        area_len=area_end-area_beg;
                        try_c=0;
                        while(try_c<try_n && area_len>6000)
                        {
                            area_spot=area_beg+((area_end-area_beg)/2);

                            comp=func_call.compare(index_func_name,Qt::CaseSensitive);
                            INDEX_fp.seek(area_spot);
                            get_line_from_file(&str,&INDEX_fp);
                            area_spot=INDEX_fp.pos();
                            get_line_from_file(&str,&INDEX_fp);


                            index_func_name=str.section("/",-1,-1);
                            comp=func_call.compare(index_func_name,Qt::CaseSensitive);
                            if(comp>0){area_beg=area_spot;}
                            if(comp<0){area_end=area_spot;}
                            if(comp==0){found_in_index=2;break;}



                            area_len=area_end-area_beg;
                            try_c++;
                        }
                        // ---------------- search remaining area
                        if(!found_in_index)
                        {
                            INDEX_fp.seek(area_beg);
                            while(INDEX_fp.pos()<area_end)
                            {
                                get_line_from_file(&str,&INDEX_fp);
                                index_func_name=str.section("/",-1,-1);

                                comp=func_call.compare(index_func_name,Qt::CaseSensitive);

                                if(comp==0)
                                {

                                    imp_pos=get_imp_pos_from_str(&str);
                                    str.remove(0,10);
                                    lib_name=str.section("/",0,0);
                                    found_in_index=2;

                                    break;
                                }
                            }

                        }

                        // ---------------------- found in index
                        if(found_in_index==2)
                        {
                            lib_path=lib_name;
                            lib_path.prepend(DISASS_LIB_PATH);
                            LIB_fp.setFileName(lib_path);
                            LIB_fp.open(QIODevice::ReadOnly);
                            if(LIB_fp.isOpen())
                            {        
                                LIB_fp.seek(imp_pos);
                                first_imp_line=true;
                                while(!LIB_fp.atEnd())
                                {
                                    get_line_from_file(&str,&LIB_fp);
                                    if(!first_imp_line && is_func_imp(str,true))
                                    {
                                        break;
                                    }
                                    if(first_imp_line)
                                    {
                                        str.append(" ## () ");
                                        str.insert(str.length()-2,lib_name);
                                        first_imp_line=false;
                                    }

                                    write_nest_mark_to_str(&str,nest_c,NEST_MARK);
                                    write_line_to_file(str,&dest_file,true);
                                    expanded=true;
                                    expansion_n++;
                                    something_expanded=true;
                                }
                                LIB_fp.close();
                            }

                        }// end found in index 2
                    }// end index open


                }// end is function call
                if(is_call)
                {
                    if(!expanded)
                    {
                        if(!is_expandable)
                        {
                            if(func_call==CALL_REG_MARK)
                            {
                                line_str.append(" ## ");
                                line_str.append(NOTE_MARK);
                                line_str.append(" on register call");
                            }
                            if(func_call==CALL_CODE_MARK)
                            {
                                line_str.append(" ## ");
                                line_str.append(NOTE_MARK);
                                line_str.append(" on code call");
                            }
                        }else{

                            if(repeated_call)
                            {
                                line_str.append(" ## ");
                                line_str.append(NOTE_MARK);
                                line_str.append(" repeated call");
                            }else{

                                if(found_in_index==0)
                                {
                                    line_str.append(" ## ");
                                    line_str.append(NOTE_MARK);
                                    line_str.append(" found in no index");
                                }
                                if(found_in_index==1)
                                {
                                    if(SET.EXPAND_LOCAL)
                                    {
                                        line_str.append(" ## ");
                                        line_str.append(NOTE_MARK);
                                        line_str.append(" found in local index but couldn't expand");
                                    }else{
                                        line_str.append(" ## ");
                                        line_str.append(NOTE_MARK);
                                        line_str.append(" local call");
                                    }
                                }
                                if(found_in_index==2)
                                {
                                    line_str.append(" ## ");
                                    line_str.append(NOTE_MARK);
                                    line_str.append(" found in general index but couldn't expand");
                                }
                            }//end not repeated
                        }//end is expandable
                        write_line_to_file(line_str,&dest_file,true);

                    }
                }else{
                    write_line_to_file(line_str,&dest_file,true);
                }
            QCoreApplication::processEvents();
            }// ----- end each nest loop
            if(something_expanded==false){proceed_expanding=false;}

            if(orig_file.isOpen()){orig_file.close();}
            if(dest_file.isOpen()){dest_file.close();}

            if(!SET.KEEP_I && nest_c>1)
            {
                orig_file.remove();
            }
            QCoreApplication::processEvents();
            nest_c++;
        }// ----- end all nestings loop

        if(PROCEED)
        {
            st_str=" expansions ocurred.";
            st_str.prepend(QString::number(expansion_n));
            emit sig_send_status(76,st_str);
        }

        if(INDEX_fp.isOpen()){INDEX_fp.close();}

    }//end if proceed after making local index and nest 0

}
// ------------------------------------------------------- (from main_engine_thread) SL START PROCESS
bool MainEngine::get_proceed()
{
    bool ret=true;
    mutex->lock();
    ret=*PROCEED_PT;
    mutex->unlock();

    if(ret==false){CANCELED=true;}

    return ret;
}

// ------------------------------------------------------- (from main_engine_thread) SL START PROCESS
void MainEngine::sl_start_process()
{
    CANCELED=false;
    PROCEED=true;
    // ------------------- PRELOADING
    if(ACTION==1)
    {
        emit sig_send_status(1,"");
        load_lib_dir(false);
        check_lib_dir_entries_file(false);
        check_disass_lib(false);
        check_index(false);
        emit sig_send_status(0,"");
    }
    // ------------------- LOAD LIB DIR FROM FILE
    if(ACTION==2)
    {
        load_lib_dir(true);
    }
    // ------------------- SAVE LIB DIR TO FILE
    if(ACTION==3)
    {
        save_lib_dir();
        load_lib_dir(false);
    }
    // ------------------- RECHECK LIB DIR ENTRIES
    if(ACTION==4)
    {
        check_lib_dir_entries_file(true);
    }
    // ------------------- MAKE LIB DIR ENTRIES
    if(ACTION==5)
    {
        make_lib_dir_entries_file();
        check_lib_dir_entries_file(false);
    }
    // ------------------- RECHECK DISASS LIBRARIES
    if(ACTION==6)
    {
        check_disass_lib(true);
    }
    // ------------------- DISASSEMBLE LIBRARIES
    if(ACTION==7)
    {
        disass_lib();
        check_disass_lib(false);
    }
    // ------------------- RECHECK INDEX
    if(ACTION==8)
    {
        check_index(true);
    }
    // ------------------- GENERATE INDEX
    if(ACTION==9)
    {
        make_index();
        check_index(false);
    }
    // ------------------- EXPAND
    if(ACTION==10)
    {
        expand();
    }
    if(CANCELED)
    {
        emit sig_send_status(99,"");
    }
    emit sig_finished();
}

// ------------------------------------------------------- (from main_control) SL RECEIVE ACTION
void MainEngine::sl_receive_action(int a)
{
    ACTION=a;
}
// ------------------------------------------------------- (from main_control) RECEIVE LIB DIR
void MainEngine::sl_receive_lib_dir_str(QString lib_dir_str)
{
    *LIB_DIR_STR=lib_dir_str;

}
// ------------------------------------------------------- (from main_control) SL RECEIVE ORIG FILE
void MainEngine::sl_receive_orig_file(QString str)
{
    *ORIG_FILE_NAME = str;
}
// ------------------------------------------------------- (from main_control) SL RECEIVE ORIG FILE
void MainEngine::sl_receive_dest_dir(QString str)
{
    *DEST_DIR_NAME = str;
}
// ------------------------------------------------------- (from main_control) SL RECEIVE SETTINGS
void MainEngine::sl_receive_settings(SETTINGS set)
{
    SET=set;
}
