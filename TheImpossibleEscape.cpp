#include <bits/stdc++.h>
#include <GLUT/glut.h>
#include <FreeImage.h>
#include "loadimage.h"
using namespace std;

double LogWidth = 2200;
double logHeight = 2500;
int physWidth = 1400;
int physHeight = 1000;
int centerX = LogWidth/2;
int centerY = logHeight/2;
int mouseX = centerX;
int mouseY = centerY;
int sizeofcube = 5;
vector<double> ColorBackground = {0.0, 0.0, 0.0};
vector<GLuint> imageids;

const int roomH = 800;
const int roomW = 440;
const int prisonerH = 110;
const int prisonerW = 65;
const int policeH = 150;
const int policeW = 100;
int policeNum = 0;
const int prisonerX = 200;
const int prisonerY = 200;
const int TELETIME = 20;
vector<int> RoomP = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
vector< pair<int, int> > prisonerMove(10, {prisonerX, prisonerY});
vector<int> roomState(10, 0);
vector<pair<int, int> > roomplace(10);
vector<pair<int, int> > policePos;
int curRoom = 0;
int walls = 1280;
int walle = 1620;
int show = 0;




enum ids {
    D_CLOSED,
    L_CLOSED,
    D_OPEN,
    L_OPEN,
    COLOR,
    POLICE,
    P0,
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,
    P7,
    P8,
    P9
};

vector< pair<string, string> > Puzzle =
{
    {"Hello", "1"},
    {"bye", "314"},
    {"marijuana", "420"},
    {"Not found", "404"},
    {"you are elite", "1337"},
    {"Fibonacci 9", "34"},
    {"Factorial 5", "120"},
    {"solo(Dota)", "322"},
    {"the game of life", "42"},
    {"Prime Number in order", "2357"},
    {"111,111,111 x 111,111,111", "12345678987654321"},
    {"The most popular favourite number", "7"},
    {"682 one number is correct and well placed\n"
        "614 one number is correct but in wrong place\n"
        "206 two numbers are correct but in wrong place\n", "042"}
    
}; //

string semiS = "Don't Go Out now!!";

pair<string, string> finalpuzzle = {"Go Out in Order", "8549176320"}; // todo
string finalout = "";
vector<int> teleport = {41, 33, 64, 35, 36, 37, 94, 38, 42, 40};
vector<pair<int, int> > teleportpow(10);
vector<int> Out(10, 0);
vector<int> myPuzzle(10, 0);

int Sq[5][4] =
{
    {0, 0, 0, 0},
    {45, 60, 65, 70},
    {45, 50, 65, 60},
    {45, 40, 65, 50},
    {45, 30, 65, 40}
};

int vv[5] = {0, 0, 0, 0, 0};


vector<pair<string, int> > llock(10, {"", 0});
int openRooms = 0;
void init2D(float r, float g, float b);
void init(int argc, char *argv[]);
void displayText();
void initPolice();
void initPrison();
void sufflePrisoners();
void displayPrison();
bool checkPolice(int x, int y, int i);
void sufflePolice();
void displayPolice();
bool checkPrison(int x, int y, int curRoom);
void PrisonKeyboard(int key);
void keyboard(int key, int x, int y);
void timer(int val);
void display (void);
void easteregg(int key);
void printText(string s, int x, int y);
void initPluzzle();
void unlock(int key);
void NewGame();
void displayinit();
void DrawS(int x0, int y0, int x1, int y1, double r, double g, double b, string s);
void mouse2(int x, int y);
void mouse(int btn, int state, int x, int y);
// functions....

void NewGame()
{
    llock.clear();
    finalout = "";
    myPuzzle.clear();
    Out.clear();
    teleportpow.clear();
    roomplace.clear();
    roomState.clear();
    prisonerMove.clear();
    llock.resize(10, {"", 0});
    myPuzzle.resize(10, 0);
    RoomP = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    prisonerMove.resize(10, {prisonerX, prisonerY});
    roomState.resize(10, 0);
    roomplace.resize(10);
    policePos.clear();
    curRoom = 0;
    policeNum = 0;
    openRooms = 0;
    teleportpow.resize(10);
    finalout = "";
    Out.resize(10);
    semiS = "Don't Go Out now!!";
    initPrison();
    initPolice();
    
}


void init2D(float r, float g, float b)
{
    glClearColor(r, g, b, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, LogWidth, 0.0, logHeight);
}

void init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(physWidth, physHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("The Impossible Escape");
    init2D(ColorBackground[0], ColorBackground[1], ColorBackground[2]);
    FreeImage_Initialise(true);
}

void timer(int val)
{
    glutTimerFunc(50, timer, val);
    glutPostRedisplay();
}

void keyboard(int key, int x, int y)
{
    PrisonKeyboard(key);
    easteregg(key);
    glutPostRedisplay();
    
}


void DrawS(int x0, int y0, int x1, int y1, double r, double g, double b, string s)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2i(x0, y0);
    glVertex2i(x1, y0);
    glVertex2i(x1, y1);
    glVertex2i(x0, y1);
    glEnd();
    if(s[0] != ' ')
        printText(s, x0 + ( (x1 - x0) / 2 ) - 5, y0 + (y1 - y0) / 2);
}




void displayinit()
{
    drawImg(0, 0, LogWidth, logHeight, 0, imageids[COLOR]);
    string Namegame = "The impossible Game";
    glColor3f(0, 0, 0);
    printText(Namegame, 911, 2000);
    if(show++ > 10 == 0)
            drawImg(1000, 1800, 1100, 1950, 0, imageids[P0]);
    else      drawImg(1000, 1800, 1100, 1950, 0, imageids[POLICE]);
    show %= 20;
    
    
    

}


void display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    displayinit();
   // displayText();
   // displayPrison();
   // displayPolice();
    glutSwapBuffers();
    glFlush();
}

void printText(string s, int x, int y)
{
   // glColor3f(0, 1.0, 0.0);
    glRasterPos2d(x, y);
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i] == '\n')
        {
            y -= 100;
            glRasterPos2d(x, y);
            continue;
        }
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }
    glFlush();
}

void initPluzzle()
{
    set<int> curpuzzle;
    srand(time(NULL));
    for(int i = 0; i < 10; i++)
    {
        bool ok = 1;
        while(ok)
        {
            int x = rand() % Puzzle.size();
            if(curpuzzle.find(x) == curpuzzle.end())
            {
                ok = 0;
                myPuzzle[i] = x;
                curpuzzle.insert(x);
                break;
            }
        }
    }
}

void displayText()
{
    drawImg(0, 0, LogWidth, logHeight, 0, imageids[COLOR]);
    string Room = "Room Number 0";
    string prisoner = "Prisoner Number 0";
    string roomstate = "Room State : ";
    if(roomState[curRoom] & 2)
         roomstate += "Open";
    else roomstate += "Closed";
    Room.back() += curRoom;
    prisoner.back() += RoomP[curRoom];
    glColor3f(0.7, 0.545, 0.5);
    printText(Room, 100, 400);
    printText(prisoner, 100, 300);
    printText(roomstate, 100, 200);
    string plu;
    if(openRooms == 10)
        plu = finalpuzzle.first;
    else if(roomState[curRoom] & 2)
        plu = semiS;
    else plu = Puzzle[myPuzzle[curRoom]].first;
    glColor3f(0.3, 0.645, 0.5);
    printText(plu, 500, 350);
}

// police
void initPolice()
{
    policePos.resize(policeNum);
    srand(time(NULL));
    for(int i = 0; i < policeNum; i++)
    {
        policePos[i].first = 0;
        int p = rand() % (walle - walls);
        policePos[i].second = p + walls;
    }
}

bool checkPolice(int x, int y, int i)
{
    x += policePos[i].first;
    y += policePos[i].second;
    if(x >= 0 && x <= 2150 && y >= walls && y <= walle)
        return 1;
    return 0;
}

void sufflePolice()
{
    srand(time(NULL));
    int x = 0, y = 0;
    for(int i = 0; i < policeNum; i++)
    {
        x = rand() % 20;
        if(checkPolice(x, y, i))
            policePos[i].first += x, policePos[i].second += y;
        
        if(policePos[i].first > 2000)
            NewGame();
    }
}

void displayPolice()
{
    sufflePolice();
    for(int i = 0; i < policeNum; i++)
    {
        drawImg(policePos[i].first, policePos[i].second,
                policePos[i].first + policeW, policePos[i].second + policeH, 0, imageids[POLICE]);
    }
}

// prison & prisoners
void initPrison()
{
    initPluzzle();
    roomState[0] = 1;
    imageids = loadAllImg();
    int HH = 500, WW = 0;
    for(int i = 0; i < 10; i++)
    {
        if(i == 5) HH = 1700, WW = 5;
        roomplace[i].first = roomW * (i - WW);
        roomplace[i].second = HH;
    }
}

void sufflePrisoners()
{
    srand(time(NULL));
    int x = 0, y = 0;
    for(int i = 0; i < 10; i++)
    {
        if(i == curRoom) continue;
        if(roomState[i] >= 1) continue;
        x = rand() % 20;
        y = rand() % 20;
        x-= 10;
        y -= 10;
        if(checkPrison(x, y, i))
            prisonerMove[i].first += x, prisonerMove[i].second += y;
    }
}

void displayPrison()
{
    bool f = 1;
    
    sufflePrisoners();
    for(int i = 0; i < 10; i++)
    {
        if(i == 5) f = f - 1;
        drawImg(roomplace[i].first, roomplace[i].second,
                roomplace[i].first + roomW, roomplace[i].second + roomH, f,
                imageids[roomState[i]]);
    }
    for(int i = 0; i < 10; i++)
    {
        int pic = RoomP[i];
        if(teleportpow[i].second % 3 == 1)
            pic = teleportpow[i].first;
        if(teleportpow[i].second % 3 != 2)
            drawImg(roomplace[i].first + prisonerMove[i].first,
                    roomplace[i].second + prisonerMove[i].second,
               roomplace[i].first + prisonerMove[i].first + prisonerW,
                roomplace[i].second + prisonerMove[i].second + prisonerH, 0,
                imageids[P0 + pic]);
        if(teleportpow[i].second) teleportpow[i].second--;
        
    }
    
    
}


int checkorder()
{
    if(finalpuzzle.second == finalout)
        return 2;
    int n = (int)finalout.size();
    string comp = finalpuzzle.second.substr(0, n);
    cout << finalout << endl;
    cout << comp << endl;
    if(comp == finalout)
        return 1;
    openRooms--;
    return 0;
}


bool checkPrison(int x, int y, int curRoom)
{
    x += prisonerMove[curRoom].first + roomplace[curRoom].first;
    y += prisonerMove[curRoom].second + roomplace[curRoom].second;
    int Yup = 200, Ydown = 0;
    int off = 0;
    if(curRoom >= 5)
    {
        Yup = 120;
        Ydown = 73;
        off = 100;
    }

    if(x >= roomplace[curRoom].first + 40 && x <= roomplace[curRoom].first + roomW - prisonerW
       && y >= roomplace[curRoom].second + Ydown && y <= roomplace[curRoom].second + roomH - Yup)
        return 1;

    if(roomState[curRoom] <= 1)
        return 0;
    
    if(x >= roomplace[curRoom].first + 330 && x <= roomplace[curRoom].first + roomW - prisonerW
       && y >= roomplace[curRoom].second - off  && y <= roomplace[curRoom].second + roomH)
        return 1;
    
    if(x >= 0 && x <= 2200
       && y >= walls && y <= walle)
    {
        if(openRooms == 10 && Out[RoomP[curRoom]] == 0)
        {
            finalout += char(RoomP[curRoom] + '0');
            Out[RoomP[curRoom]] = 1;
        }
        int win = checkorder();
        if((openRooms < 10 || win == 0 || win == 2) && policeNum == 0)
        {
            for(int i = 0; i < 10 ; i++)
            {
                roomState[i] |= 1;
            }
            if(win == 2) semiS = "You Win !!!";
            else semiS = "Game Over.............";
            if(win == 2) policeNum = 1;
            else policeNum = 10;
            initPolice();
        }
        return 1;
    }
    
    
    
    return 0;
    
    
}

void unlock(int key)
{

    if(llock[curRoom].second == 1)
        return;
    llock[curRoom].first += char(key + '0');
  //  cout << llock[curRoom].first << endl;
    int n = (int)llock[curRoom].first.size();
    string comp = Puzzle[myPuzzle[curRoom]].second.substr(0, n);
    if(comp == llock[curRoom].first)
    {
        if(llock[curRoom].first == Puzzle[myPuzzle[curRoom]].second)
        {
            llock[curRoom].second = 1;
            roomState[curRoom] += 2;
            openRooms++;
        }
    } else
    {
        llock[curRoom].first = "";
    }
    
    
}


void PrisonKeyboard(int key)
{
    
    int sh = glutGetModifiers();
    if(sh == GLUT_ACTIVE_SHIFT && openRooms != 10)
    {
        for(int i = 0; i < 10; i++)
            if(key == teleport[i])
            {
                int PP = 0;
                for(int j = 0; j < 10; j++)
                    if(RoomP[j] == i)
                        PP = j;
                unlock(i);
                teleportpow[PP].first = RoomP[PP];
                teleportpow[curRoom].first = RoomP[curRoom];
                teleportpow[PP].second = teleportpow[curRoom].second = TELETIME;
                RoomP[PP] = RoomP[curRoom];
                RoomP[curRoom] = i;

            }
        return;
    }
    if(key >= 48 && key <= 57)
    {
        key -= 48;
        roomState[curRoom] ^= 1;
        curRoom = key;
        roomState[curRoom] ^= 1;
    }
    
    if(key == 97 && checkPrison(-15, 0, curRoom))
        prisonerMove[curRoom].first -= 15;
    else if(key == 100 && checkPrison(15, 0, curRoom))
        prisonerMove[curRoom].first += 15;
    else if(key == 115 && checkPrison(0, -15, curRoom))
        prisonerMove[curRoom].second -= 15;
    else if(key == 119 && checkPrison(0, 15, curRoom))
        prisonerMove[curRoom].second += 15;
}

void easteregg(int key)
{
    if(key == 1111111111)
        roomState[curRoom] |= 2;
}

void mouse2(int x, int y)
{
    mouseX = x;
    mouseX = 0.5 + 1.0 * mouseX * LogWidth / physWidth;
    mouseY = physWidth - y;
    mouseY = 0.5 + 1.0 * mouseY * logHeight / physHeight;
    for(int i = 1; i <= 4; i++)
    {
        if(mouseX >= Sq[i][0] && mouseX <= Sq[i][2] && mouseY >= Sq[i][1] && mouseY <= Sq[i][3])
        {
            vv[i] = 1;
        }
        else vv[i] = 0;
    }
    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
    mouseX = x;
    mouseX = 0.5 + 1.0 * mouseX * LogWidth / physWidth;
    mouseY = physWidth - y;
    mouseY = 0.5 + 1.0 * mouseY * logHeight / physHeight;
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        for(int i = 1; i <= 4; i++)
        {
            if(mouseX >= Sq[i][0] && mouseX <= Sq[i][2] && mouseY >= Sq[i][1] && mouseY <= Sq[i][3])
            {
            }
        }
    }
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        
    }
}



int main(int argc, char *argv[])
{
    init(argc, argv);
    initPrison();
    initPolice();
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouse2);
    timer(0);
    glutMainLoop();
}



