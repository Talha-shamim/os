#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ncurses.h>
using namespace std;

void init()
{
    initscr();
    raw();
    cbreak();
    noecho();
    keypad(stdscr, true);
}

void drawTable(int rows, int columns)
{
    int i, h;

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));

    int student = 1;
    int faculty = 1;
    for (int i = 1; i <= (rows * 2) + 2; i++)
    {
        if (i < rows * 2)
            mvwprintw(stdscr, i + 2, 1, ("s" + to_string(student)).c_str());
        if (i & 1)
            student++;

        for (int j = 0; j <= columns + 4; j++)
        {
            if (i == 1 and j < columns)
            {
                mvwprintw(stdscr, i, j * 4 + 5, ("f" + to_string(faculty)).c_str());
            }
            faculty++;

            if (j == columns + 1 and i == 1)
            {
                mvwprintw(stdscr, i, j * 4 + 1, "Total");
            }

            if (j == columns + 2 and i == 1)
            {
                mvwprintw(stdscr, i, j * 4 + 4, "Average");
            }

            if (j == columns + 4 and i == 1)
            {
                mvwprintw(stdscr, i, j * 4 + 4, "Rank");
            }

            int offset = 0;
            if (j == columns + 2)
            {
                offset = 3;
            }

            if (j == columns + 3)
            {
                offset = 7;
            }

            if (j == columns + 4)
            {
                offset = 8;
            }
            mvaddch(i, j * 4 + offset, '|');
        }

        if (i % 2 == 0)
        {
            for (h = 0; h <= (columns + 6) * 4; h++)
            {
                mvaddch(i, h, '-');
            }
        }
        move(1, 6);
    }
    refresh();
}

void remove_faculty_from_table(string faculty, int rows, int columns)
{
    char code = faculty[1];
    int fac_code = code - '0';

    for (int i = 1; i <= (rows * 2) + 2; i++)
    {
        for (int j = 0; j <= columns + 1; j++)
        {
            if (j == fac_code and (i & 1) and i > 1)
            {
                mvaddch(i, j * 4 + 2, 'X');
            }
        }

        move(1, 6);
    }
    refresh();
}

void remove_student_from_table(string student, int rows, int columns)
{
    char code = student[1];
    int stud_code = code - '0';

    for (int i = 1; i <= (rows * 2) + 2; i++)
    {
        if (i == stud_code * 2 + 1)
        {
            for (int j = 0; j <= columns + 4; j++)
            {
                if (j * 4 + 2 >= 6 and j * 4 + 2 <= (columns + 4) * 4)
                {
                    int offset = 0;
                    if (j == columns + 2)
                    {
                        offset = 5;
                    }
                    if (j == columns + 1)
                    {
                        offset = 1;
                    }

                    if (j == columns + 3)
                    {
                        offset = 7;
                    }

                    mvaddch(i, j * 4 + 2 + offset, 'X');
                }
            }
        }

        move(1, 6);
    }
    refresh();
}

void set_table_for_current_user(string user, int rows, int columns)
{
    char type = user[0];
    int id = user[1] - '0';
    if (type == 'f')
    {
        int faculty = 1;
        for (int j = 0; j <= columns + 4; j++)
        {
            if (j < columns)
            {
                mvwprintw(stdscr, 1, j * 4 + 5, ("f" + to_string(faculty)).c_str());
            }
            faculty++;
        }
        // get the data
        vector<string> marks;

        vector<string> students;
        string names;
        ifstream MyReadFile("database/student.txt");
        while (getline(MyReadFile, names))
        {
            students.push_back(names);
        }

        sort(students.begin(), students.end());

        string marks_student;

        for (auto str : students)
        {
            string filename = user + "_" + str + ".txt";
            ifstream MyReadFile("database/marks/" + filename);
            while (getline(MyReadFile, marks_student))
            {
                marks.push_back(marks_student);
             }
        }

        // set the board
        int k = 0;
        for (int i = 1; i <= (rows * 2) + 2; i++)
        {
            string student = "s" + to_string(i);
            for (int j = 0; j <= columns + 1; j++)
            {
                if (j == id and find(students.begin(), students.end(), student) != students.end())
                {
                    mvwprintw(stdscr, i * 2 + 1, j * 4 + 1, marks[k++].c_str());
                }
            }
            move(1, 6);
        }
        refresh();
    }
    else if (type == 's')
    {
        for (int j = 0; j <= columns + 4; j++)
        {
            if (j < columns)
            {
                mvwprintw(stdscr, 1, j * 4 + 5, "F ");
            }
        }

        // get the data
        // vectors for marsk
        // and faculties
        vector<string> marks;
        vector<string> faculties;

        // for getitng the name of faculties
        string names;

        // read the file of faculty
        ifstream MyReadFile("database/faculty.txt");

        while (getline(MyReadFile, names))
        {
            // push to the faculty vector
            faculties.push_back(names);
        }

        // sort for ordering in printing
        sort(faculties.begin(), faculties.end());

        // to read the marks from file
        string marks_student;

        // variables for total_marsk
        // and average_marks
        int total_marks = 0;
        float average_marks;

        for (auto str : faculties)
        {
            // get the file name
            string filename = str + "_" + user + ".txt";

            // read the file
            ifstream MyReadFile("database/marks/" + filename);

            while (getline(MyReadFile, marks_student))
            {
                // cahnge to int
                int _marks = stoi(marks_student);

                // add to total marks
                total_marks += _marks;

                // push to the marks vector
                marks.push_back(marks_student);
            }
        }

        // get the  no of faculty
        int no_of_faculty = faculties.size();

        // calculate the average
        average_marks = total_marks * 1.0 / no_of_faculty * 1.0;

        // cauculate the rank
        int rank_ = 0;

        // get all the students
        vector<string> students;
        string names_;

        // read the file
        ifstream MyReadFile1("database/student.txt");
        while (getline(MyReadFile1, names_))
        {
            students.push_back(names_);
        }

        // make the arnking vector
        // this is a marks to name vector
        vector<pair<int, string>> ranking;

        // iterate all the stiudents
        for (auto stud : students)
        {
            // initialize for each student to 0
            int t_marks = 0;

            // iterate all the faculties
            for (auto fact : faculties)
            {
                // for a student get all the files for
                // each faculties and make the filename and retrieve the data
                string filename = fact + "_" + stud + ".txt";

                // read the file
                ifstream MyReadFile("database/marks/" + filename);

                while (getline(MyReadFile, marks_student))
                {
                    // cahnge to int
                    int _marks = stoi(marks_student);

                    // add to total marks
                    t_marks += _marks;
                }
            }

            // finaaly push toi the ranking vector to get the rank
            ranking.push_back({t_marks, stud});
        }

        // sorting of ranking vector
        // according to the marks in
        // descending order
        sort(ranking.begin(), ranking.end(), greater<pair<int, string>>());

        // iterating to find the position of
        // my current user and giving him the rank
        for (int i = 0; i < ranking.size(); i++)
        {
            if (ranking[i].second == user)
            {
                rank_ = i + 1;

                // once i get it we can break the loop
                break;
            }
        }

        // set the board
        int k = 0;
        for (int i = 1; i <= (rows * 2) + 2; i++)
        {
            for (int j = 0; j <= columns + 4; j++)
            {
                if (i == id * 2 + 1)
                {
                    // ame the name of faculty
                    string student = "f" + to_string(j);
                    if (j * 4 + 2 >= 6 and j * 4 + 2 <= (columns + 3) * 4 and find(faculties.begin(), faculties.end(), student) != faculties.end())
                    {
                        // print the marks of each faculty
                        // found in the vector
                        if (j * 4 + 2 <= (columns + 1) * 4)
                        {
                            mvwprintw(stdscr, id * 2 + 1, j * 4 + 1, marks[k++].c_str());
                        }
                    }

                    // now fora each row print the total_marks
                    // first convert to string
                    if (j == (columns + 2))
                    {
                        mvwprintw(stdscr, id * 2 + 1, j * 4 - 1, to_string(total_marks).c_str());
                    }

                    // now fora each row print the average_marks
                    // first convert to string
                    if (j == (columns + 3))
                    {
                        mvwprintw(stdscr, id * 2 + 1, j * 4 + 1, to_string(average_marks).substr(0, 4).c_str());
                    }

                    // now fora each row print the rank
                    // first convert to string
                    if (j == (columns + 4))
                    {
                        mvwprintw(stdscr, id * 2 + 1, j * 4 + 5, to_string(rank_).substr(0, 4).c_str());
                    }
                }
            }

            // bring the cursorto the position
            move(1, 6);
        }

        // rerferesh the table
        refresh();
    }
}

int main()
{

    int TABLE_ROWS;
    int TABLE_COLUMNS;
    string USER = "ADMIN";
    if (getuid() != 0)
    {
        USER = "NORMAL";
    }

    if (USER == "ADMIN")
    {
        while (1)
        {
            cout << "Wanna add user[Y/N]" << endl;
            string input;
            cin >> input;

            if (input == "Y")
            {
                // add user here
                string group;
                string name;
                cout << "Wanna add Faculty, Student or Instructor [F/S/I]" << endl;
                string input1;
                cin >> input1;

                bool fac = false;
                bool stud = false;
                bool inst = false;
                if (input1 == "F")
                {
                    group = "faculty";
                    fac = true;
                }
                else if (input1 == "S")
                {
                    group = "student";
                    stud = true;
                }
                else if (input1 == "I")
                {
                    group = "instructor";
                    inst = true;
                }
                else
                {
                    cout << "Please enter valid responce" << endl;
                    continue;
                }

                cout << "Enter the Name" << endl;
                cin >> name;
                

                // add corresponding files
                if (fac)
                {
                    system(("adduser " + name).c_str());
                    system(("usermod -G " + group + " " + name).c_str());
                    // add all students file and the faculty
                    bool already_present = false;
                    vector<string> students;
                    string names;
                    ifstream MyReadFile("database/student.txt");
                    while (getline(MyReadFile, names))
                    {
                        students.push_back(names);
                    }

                    ifstream MyReadFile2("database/faculty.txt");
                    while (getline(MyReadFile2, names))
                    {
                        if(names == name){
                            already_present = true;
                        }
                    }

                    if (already_present == false)
                    {
                        ofstream outfile;
                        outfile.open("database/faculty.txt", std::ios_base::app);
                        outfile << name << endl;
                    }
                    
                    ofstream outfile2;
                    outfile2.open("database/all_faculties.txt", std::ios_base::app);
                    outfile2 << name << endl;
                    

                    // make file in directory marks fac_stud
                    for (auto str : students)
                    {
                        system(("touch database/marks/" + name + "_" + str + ".txt").c_str());
                        ofstream MyFile("database/marks/" + name + "_" + str + ".txt");
                        MyFile << "0" << endl;
                        MyFile.close();
                        // permissions
                        system(("sudo chown "  +  name + " database/marks/" + name + "_" + str + ".txt").c_str());
                        system(("sudo chmod u+w database/marks/" + name + "_" + str + ".txt").c_str());
                    }
                }

                if (stud)
                {
                    system(("adduser " + name).c_str());
                    system(("usermod -G " + group + " " + name).c_str());
                    // add all faculty files + total file and the student
                    bool already_present = false;
                    vector<string> faculties;
                    string names;
                    ifstream MyReadFile("database/faculty.txt");

                    while (getline(MyReadFile, names))
                    {
                        faculties.push_back(names);
                    }

                    ifstream MyReadFile2("database/student.txt");
                    while (getline(MyReadFile2, names))
                    {
                        if(names == name){
                            already_present = true;
                        }
                    }

                    if (already_present == false)
                    {
                        ofstream outfile;
                        outfile.open("database/student.txt", std::ios_base::app);
                        outfile << name << endl;
                    }

                    ofstream outfile2;
                    outfile2.open("database/all_students.txt", std::ios_base::app);
                    outfile2 << name << endl;

                    for (auto str : faculties)
                    {
                        system(("touch database/marks/" + str + "_" + name + ".txt").c_str());
                        ofstream MyFile("database/marks/" + str + "_" + name + ".txt");
                        MyFile << "0" << endl;
                        MyFile.close();
                        // permissions
                        // system(("chown "+ str + " database/marks/" + str + "_" + name + ".txt").c_str());
                        system(("chmod 700 database/marks/" + str + "_" + name + ".txt").c_str());
                        // system(("setfacl -m u:" + str + ":rw- database/marks/" + str + "_" + name + ".txt").c_str());
                    }
                }

                if(inst){
                    bool already_present = false;
                    string names;
                    vector<string>instructors;
                    ifstream MyReadFile("database/instructor.txt");

                    while (getline(MyReadFile, names))
                    {
                        instructors.push_back(names);
                    }

                    if(instructors.size()){
                        cout<<instructors[0]<<endl;
                        already_present = true;
                    }

                    if(already_present){
                        cout<<"Already an Instrcutor is Assigned..."<<endl;
                    }
                    
                    else{
                        system(("adduser " + name).c_str());
                        system(("usermod -G " + group + " " + name).c_str());
                        ofstream outfile;
                        outfile.open("database/instructor.txt", std::ios_base::app);
                        outfile << name << endl;
                    }
                }
            }
            else if (input == "N")
            {
                break;
            }
            else
            {
                cout << "Please enter a valid response" << endl;
            }
        }

        while (1)
        {
            cout << "Wanna Delete user[Y/N]" << endl;
            string input;
            cin >> input;
            string name;

            if (input == "Y")
            {
                // delete user here
                cout << "Enter the Name" << endl;
                cin >> name;
                system(("deluser " + name).c_str());

                bool fac = false;
                bool stud = false;
                bool inst = false;
                if(name[0] == 'i')inst = true;

                // ================================delete corresponding names===============================

                // ================================FACULTY================================
                ifstream fin;
                fin.open("database/faculty.txt");
                ofstream temp;
                temp.open("database/temp.txt");
                string line;

                while (getline(fin, line))
                {
                    if (line != name)
                        temp << line << endl;

                    if (line == name)
                        fac = true;
                }

                ifstream ini_file{"database/temp.txt"};
                ofstream out_file{"database/faculty.txt"};

                if (ini_file && out_file)
                {
                    while (getline(ini_file, line))
                    {
                        out_file << line << "\n";
                    }
                }
                else
                {
                    // Something went wrong
                    printf("Cannot read File");
                }

                // Closing file
                ini_file.close();
                out_file.close();

                // Closing file
                temp.close();
                fin.close();

                ofstream file("database/temp.txt");

                // ======================================STUDENT=========================

                fin.open("database/student.txt");
                temp.open("database/temp.txt");

                while (getline(fin, line))
                {
                    if (line != name)
                        temp << line << endl;

                    if (line == name)
                        stud = true;
                }

                ifstream init_file{"database/temp.txt"};
                ofstream final_file{"database/student.txt"};

                if (init_file && final_file)
                {
                    while (getline(init_file, line))
                    {
                        final_file << line << "\n";
                    }
                }
                else
                {
                    // Something went wrong
                    printf("Cannot read File");
                }

                // Closing file
                init_file.close();
                final_file.close();

                // Closing file
                temp.close();
                fin.close();

                ofstream temp_file("database/temp.txt");

                // ==========================delete corresponding files====================================

                // =========================FACULTY===========================
                if (fac)
                {
                    vector<string> students;
                    string names;
                    ifstream MyReadFile("database/student.txt");

                    while (getline(MyReadFile, names))
                    {
                        students.push_back(names);
                    }

                    for (auto str : students)
                    {
                        remove(("database/marks/" + name + "_" + str + ".txt").c_str());
                    }
                }

                // =========================STUDENT===========================

                if (stud)
                {
                    vector<string> faculties;
                    string names;
                    ifstream MyReadFile("database/faculty.txt");

                    while (getline(MyReadFile, names))
                    {
                        faculties.push_back(names);
                    }

                    for (auto str : faculties)
                    {
                        remove(("database/marks/" + str + "_" + name + ".txt").c_str());
                    }
                }

                if(inst){
                    std::ofstream ofs;
                    ofs.open("database/instructor.txt", std::ofstream::out | std::ofstream::trunc);
                    ofs.close();
                }
            }
            else if (input == "N")
            {
                break;
            }
            else
            {
                cout << "Please enter a valid response" << endl;
            }
        }
    }
    else
    {
        // either faculty or student registered else dont show

        uid_t uid = geteuid();
        struct passwd *pw = getpwuid(uid);
        string CURRENT_USER = pw->pw_name;

        // get the count of faculties and students

        set<string> students;
        string names;
        ifstream MyReadFile("database/all_students.txt");

        while (getline(MyReadFile, names))
        {
            students.insert(names);
            cout<<names<<endl;
        }

        set<string> faculties;
        ifstream MyReadFile2("database/all_faculties.txt");

        while (getline(MyReadFile2, names))
        {
            faculties.insert(names);
        }

        TABLE_ROWS = students.size();
        TABLE_COLUMNS = faculties.size();

        init();
        drawTable(TABLE_ROWS, TABLE_COLUMNS);

        // ====================get all the present faculties====================

        set<string> present_faculties;
        ifstream MyReadFile3("database/faculty.txt");

        while (getline(MyReadFile3, names))
        {
            present_faculties.insert(names);
        }

        set<string> present_students;
        ifstream MyReadFile4("database/student.txt");
        while (getline(MyReadFile4, names))
        {
            present_students.insert(names);
        }

        // ======================if fac not present mark X=========================

        for (auto str : faculties)
        {
            if (present_faculties.find(str) == present_faculties.end())
            {
                remove_faculty_from_table(str, TABLE_ROWS, TABLE_COLUMNS);
            }
        }

        // ======================if student not present mark X=======================

        for (auto str : students)
        {
            if (present_students.find(str) == present_students.end())
            {
                remove_student_from_table(str, TABLE_ROWS, TABLE_COLUMNS);
            }
        }

        

        if (CURRENT_USER[0] == 's')
        {
            set_table_for_current_user(CURRENT_USER, TABLE_ROWS, TABLE_COLUMNS);
            // set to an initial position
            int id = CURRENT_USER[1] - '0';
            wmove(stdscr, id * 2 + 1, 6);

            int move;
            while (move = getch())
            {
                int x, y;
                getyx(stdscr, y, x);

                int CUR_X, CUR_Y;
                CUR_X = x;
                CUR_Y = y;

                if (move == KEY_UP)
                {
                    if (y <= CUR_Y)
                    {
                        wmove(stdscr, CUR_Y, x);
                    }
                    else
                    {
                        wmove(stdscr, y - 2, x);
                    }
                }
                else if (move == KEY_DOWN)
                {
                    if (y >= CUR_Y)
                    {
                        wmove(stdscr, CUR_Y, x);
                    }
                    else
                        wmove(stdscr, y + 2, x);
                }
                else if (move == KEY_LEFT)
                {
                    if (x <= 6)
                    {
                        wmove(stdscr, y, TABLE_COLUMNS * 4 + 2);
                    }
                    else
                    {
                        wmove(stdscr, y, x - 4);
                    }
                }
                else if (move == KEY_RIGHT)
                {
                    if (x >= TABLE_COLUMNS * 4)
                    {
                        wmove(stdscr, y, 6);
                    }
                    else
                        wmove(stdscr, y, x + 4);
                }

                refresh();
            }
        }

        else if (CURRENT_USER[0] == 'f')
        {
            set_table_for_current_user(CURRENT_USER, TABLE_ROWS, TABLE_COLUMNS);
            // set an initial position
            int id = CURRENT_USER[1] - '0';
            wmove(stdscr, 3, id * 4 + 2);

            int move;
            while (move = getch())
            {
                // get the initial position
                int x, y;
                getyx(stdscr, y, x);

                // set the initial position
                int CUR_X, CUR_Y;
                CUR_X = x;
                CUR_Y = y;

                // user pressed enter
                if (move == 10)
                {
                    string stud_name = "s"+to_string(y/2);
                    if(present_students.find(stud_name) != present_students.end()){
                        attron(A_BLINK);
                        // make cell empty
                        // get into cell
                        // make the cursor blink
                        wmove(stdscr, CUR_Y, CUR_X);
                        mvaddch(y, x - 1, ' ');
                        mvaddch(y, x, ' ');
                        mvaddch(y, x + 1, ' ');

                        // get the filename
                        string filename = CURRENT_USER + "_s" + to_string(y / 2) + ".txt";

                        // read the file
                        string marks;
                        string prev_marks;
                        ifstream MyReadFile("database/marks/" + filename);
                        while (getline(MyReadFile, marks))
                        {
                            // stored in prev_marks
                            prev_marks = marks;
                        }

                        // move the character
                        wmove(stdscr, CUR_Y, CUR_X);
                        refresh();

                        // get the value
                        int val;
                        cin >> val;

                        // if the user presses escape
                        // restore the prev value
                        // get out of cell
                        // remove the blinking cursor
                        // if user presses other keys
                        // get the pos of cursor to original pos
                        if (val == 0)
                        {
                            mvwprintw(stdscr, CUR_Y, CUR_X - 1, prev_marks.c_str());
                            wmove(stdscr, CUR_Y, CUR_X);
                            refresh();
                        }

                        // chaneg the value in cell
                        // change the content of file
                        // get the pos of cursor to original pos
                        else if (val > 0 and val <= 100)
                        {
                            
                                mvwprintw(stdscr, CUR_Y, CUR_X - 1, to_string(val).c_str());
                                wmove(stdscr, CUR_Y, CUR_X);
                                refresh();

                                // write to the file
                                ofstream MyFile("database/marks/" + filename);
                                MyFile << val;
                                MyFile.close();
                            
                        }
                        attroff(A_BLINK);
                    }
                }

                if (move == KEY_UP)
                {
                    if (y <= 4)
                    {
                        wmove(stdscr, TABLE_ROWS * 2 + 1, x);
                    }
                    else
                    {
                        wmove(stdscr, y - 2, x);
                    }
                }
                else if (move == KEY_DOWN)
                {
                    if (y >= TABLE_ROWS * 2)
                    {
                        wmove(stdscr, 3, x);
                    }
                    else
                        wmove(stdscr, y + 2, x);
                }
                else if (move == KEY_LEFT)
                {
                    if (x <= CUR_X)
                    {
                        wmove(stdscr, y, CUR_X);
                    }
                    else
                    {
                        wmove(stdscr, y, x - 4);
                    }
                }
                else if (move == KEY_RIGHT)
                {
                    if (x >= CUR_X)
                    {
                        wmove(stdscr, y, CUR_X);
                    }
                    else
                        wmove(stdscr, y, x + 4);
                }

                refresh();
            }
        }

        else if(CURRENT_USER[0] == 'i'){
            
            for(auto stu : present_students){
                set_table_for_current_user(stu,TABLE_ROWS, TABLE_COLUMNS);
            }

            for(auto fac : present_faculties){
                set_table_for_current_user(fac,TABLE_ROWS, TABLE_COLUMNS);
            }

            int move;
            while (move = getch())
            {
                int x, y;
                getyx(stdscr, y, x);

                int CUR_X, CUR_Y;
                CUR_X = x;
                CUR_Y = y;

                if (move == KEY_UP)
                {
                    if (y <= 4)
                    {
                        wmove(stdscr, TABLE_ROWS * 2 + 1, x);
                    }
                    else
                    {
                        wmove(stdscr, y - 2, x);
                    }
                }
                else if (move == KEY_DOWN)
                {
                    if (y >= TABLE_ROWS*2)
                    {
                        wmove(stdscr,3, x);
                    }
                    else
                        wmove(stdscr, y + 2, x);
                }
                else if (move == KEY_LEFT)
                {
                    if (x <= 6)
                    {
                        wmove(stdscr, y, TABLE_COLUMNS * 4 + 2);
                    }
                    else
                    {
                        wmove(stdscr, y, x - 4);
                    }
                }
                else if (move == KEY_RIGHT)
                {
                    if (x >= TABLE_COLUMNS * 4)
                    {
                        wmove(stdscr, y, 6);
                    }
                    else
                        wmove(stdscr, y, x + 4);
                }
                refresh();
            }
        }
    }
}