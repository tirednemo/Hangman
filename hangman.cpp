#include<bits/stdc++.h>
#include<windows.h>
#include<mmsystem.h>
#include<conio.h>
//#include <cstdlib>
using namespace std;

//color
HANDLE hCon;
enum Color {DARKBLUE = 1, DARKGREEN, DARKTEAL, DARKRED, DARKPINK, DARKYELLOW, GRAY, DARKGRAY, BLUE, GREEN, TEAL, RED, PINK, YELLOW, WHITE};
void SetColor(Color c)
{
    if(hCon == NULL)
        hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, c);
}


struct position
{
    int px, py;
    int wx1,wx2,wx3,wx4;
    int wy1,wy2,wy3,wy4;
};


struct grid
{
    int width, height;

    void Render(position pos)
    {
        for (int row=0; row<height; row++)
        {
            for (int col=0; col<width; col++)
            {
                if (col==pos.px && row==pos.py)
                {
                    printf("P");
                }

                else if(col==pos.wx1 && row==pos.wy1 || col==pos.wx2 && row==pos.wy2 || col==pos.wx3 && row==pos.wy3 || col==pos.wx4 && row==pos.wy4)
                {
                    cout<<"W";
                }

                else
                {
                    cout<<".";
                }
            }

            printf("\n");
        }
    }
};

void hangman();
position check(position);

struct playerinput
{
    char getinput(){
    cout<<"'d' for going right\n'a' for going left\n'w' for going up\n's' for going down\n'q' for quit\n";
    cout<<"Type in which direction you want to move: ";

    char input=getch();
    cin.clear(); // clear error flags
    return input;
    }
};
int main()
{

    mciSendString("seek BG.mp3 to start", NULL, 100, NULL);
    mciSendString("play BG.mp3", NULL, 0, NULL);
    //PlaySound(TEXT("welcome.wav"), NULL, SND_SYNC);

    grid jungle {50, 25};   //jungle.width=10;jungle.height=10;
    //system("chcp 65001  > nul");
    position pos;
    pos.px = 0;
    pos.py = 0;
    srand(time(NULL));
    pos.wx1=rand()%50;
    pos.wy1=rand()%25;
    pos.wx2=rand()%50;
    pos.wy2=rand()%25;
    pos.wx3=rand()%50;
    pos.wy3=rand()%25;
    pos.wx4=rand()%50;
    pos.wy4=rand()%25;
    playerinput p;

    char input = '0';
    while (input!='q' && pos.wx1>=0 && pos.wx2>=0 && pos.wx3>=0 && pos.wx4>=0)
    {
        printf("\n");
        jungle.Render(pos);
        input=p.getinput();

//        cout<<"'d' for going right\n'a' for going left\n'w' for going up\n's' for going down\n'q' for quit\n";
//        cout<<"Type in which direction you want to move: ";
//
//        char input=getch();
//        cin.clear(); // clear error flags

        if (input == 'w')
        {
            pos.py--;
            if(pos.py<0)
                pos.py=24;
        }
        else if (input == 's')
        {
            pos.py++;
            if(pos.py>24)
                pos.py=0;
        }
        else if (input == 'd')
        {
            pos.px++;
            if(pos.px>49)
                pos.px=0;
        }
        else if (input == 'a')
        {
            pos.px--;
            if(pos.px<0)
                pos.px=49;
        }


        system("CLS");
        pos=check(pos);
    }

    /*printf("\n\n\t\tWooHoo!! You won! :)");
      PlaySound(TEXT("won.wav"), NULL, SND_SYNC);
      printf("\n\n\t\tOh no!! You lose! :(\n");
      PlaySound(TEXT("lose.wav"), NULL, SND_SYNC);*/
    mciSendString("close BG.mp3", NULL, 0, NULL);
    return 0;
}

position check(position pos)
{
    struct position newpos=pos;
    if(pos.px==pos.wx1 && pos.py==pos.wy1)
    {
        newpos.wx1=-333;
        hangman();
    }
    else if(pos.px==pos.wx2 && pos.py==pos.wy2)
    {
        newpos.wx2=-333;
        hangman();
    }
    else if(pos.px==pos.wx3 && pos.py==pos.wy3)
    {
        newpos.wx3=-333;
        hangman();
    }
    else if(pos.px==pos.wx4 && pos.py==pos.wy4)
    {
        newpos.wx4=-333;
        hangman();
    }

    return newpos;
}


//to load words and hints from file to vector
bool load(string file_name, vector<string> &list)
{
    fstream fp(file_name, ios::in);
    char buffer[70];

    if(fp.is_open())
    {
        list.clear();
        while(!fp.eof())
        {
            fp.getline(buffer, 70);
            list.push_back(buffer);
        }
        fp.close();
        return 1;
    }
    else
        return 0;
}

void hangman()
{
    //std::cout << R"()" << std::endl;


    //PlaySound(TEXT("BG.mp3"), NULL, SND_ASYNC |SND_LOOP);

    //loading letters
    vector<char> alphabet;
    for (int i = 65; i <=90; i++)
        alphabet.push_back(i);

    //loading words
    vector<string> wordlist;
    if (!load("words.txt", wordlist))
    {
        printf("words.txt is missing\n");
        exit(1);
    }

    //loading hints
    vector<string> hintlist;
    if (!load("hints.txt", hintlist))
    {
        printf("hints.txt missing\n");
        exit(1);
    }


    //choosing random word
    srand(time(0));
    string word;
    word = wordlist.at(rand() % wordlist.size());

    int serial;

    //erasing word from vector to prevent repetition
    vector<string>::iterator itr = std::find(wordlist.begin(), wordlist.end(), word);
    if (itr != wordlist.end())
    {
        wordlist.erase(itr);
        serial = (itr - wordlist.begin())+1;
        //printf("\n%d  ", serial);
    }
    //printing remaining words
    //for (itr = wordlist.begin(); itr != wordlist.end(); itr++)    cout << ' ' << *itr;

    string hint = hintlist[serial-1];


    int total_letters, filled_blanks = 0, lives = 6, flag = 0, h = 1;;
    char current_blanks[10], temp='n', temp1, temp2;
    total_letters = word.size();
    for (int i = 0; i < total_letters; i++)
        current_blanks[i] = '_';



    while (lives > 0)
    {
        flag = 0;

        SetColor(PINK);
        printf("\n\n\t\t\t HANGMAN \t\t\tlives: %d/6", lives);

        SetColor(WHITE);
        if(lives<3 && h==1)
        {
            printf("\n\n\t Need a hint?(Y/N) ");
            scanf("%c", &temp);
            getchar();
        }

        if(temp=='Y' || temp=='y')
        {
            cout<<"\n\n\t"<<hint<<"\n";
            h=0;
        }


        printf("\n\n\t ");
        for (int i = 0; i < total_letters; i++)
        {
            SetColor(DARKBLUE);
            printf("%c ", current_blanks[i]);
        }

        SetColor(TEAL);
        printf("\n\n\n\t ");
        for (auto j = alphabet.begin(); j != alphabet.end(); j++)   cout << ' ' << *j;
        printf("\n\n\t Guess a letter: ");

        scanf("%c", &temp1);
        getchar();


        temp2=toupper(temp1);
        std::replace (alphabet.begin(), alphabet.end(), temp2, (char)177);



        for (int i = 0; i < total_letters; i++)
        {
            if ((temp1 == word[i]) || (temp1 - 32 == word[i]) || (temp1 + 32 == word[i]))
            {
                flag = 1;
                current_blanks[i] = word[i];
                filled_blanks++;
            }
        }



        if (flag == 1)
        {
            SetColor(YELLOW);
            printf("\n\n\t\tYou got it!");
            PlaySound(TEXT("lilwin.wav"), NULL, SND_SYNC);
        }

        else
        {
            lives--;
            SetColor(DARKPINK);
            printf("\n\n\t\t%c is not here!", temp1);
            printf("\n\n\t\t%d guesses remaining", lives);
            PlaySound(TEXT("lillose.wav"), NULL, SND_SYNC);
        }

        system("CLS");
        if (filled_blanks == total_letters)
            break;
    }


    if (filled_blanks == total_letters)
    {
        SetColor(GREEN);
        cout << "\n\n\t\t" << "The word is " << word << "\n\n";
        PlaySound(TEXT("bigwin.wav"), NULL, SND_SYNC);
    }

    else
    {
        SetColor(BLUE);
        cout << "\n\n\t\t" << "The word was " << word << "\n\n";
        PlaySound(TEXT("biglose.wav"), NULL, SND_SYNC);
    }

    SetColor(WHITE);
    system("CLS");
}
