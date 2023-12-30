void goto_xy(short x, short y) {COORD pos = {x, y}; SetConsoleCursorPosition(hConsole, pos);}// move the cursor
void set_color(const unsigned short textColor) {SetConsoleTextAttribute(hConsole, textColor);}
//set the font size depend on how much width and height do you need in screen
void SetFont(bool square, int col, int line = 0) {
    int size;
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    if(square){
        if(line)
            size = std::min(ResX/col, ResY/line);
        else
            size = ResX/col;
    }
    else{
        if(line)
            size = std::min(ResX*2/col, ResY/line);
        else
            size = ResX*2/col;
    }
    cfi.dwFontSize.X = (square?size:0); //0 for the default ratio
    cfi.dwFontSize.Y = size; //set the size of the font
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
}
//set location of the cursor
void setcursor(short x = 0, short y = 0){SetConsoleCursorPosition(hConsole, COORD{x,y});}
void setcursor(const COORD &temp){SetConsoleCursorPosition(hConsole, temp);}
 
// get location of the cursor
void getcursor(COORD &other){
    CONSOLE_SCREEN_BUFFER_INFO temp;
    GetConsoleScreenBufferInfo(hConsole, &temp);
    other = temp.dwCursorPosition;
}
COORD getcursor(){
    CONSOLE_SCREEN_BUFFER_INFO temp;
    GetConsoleScreenBufferInfo(hConsole, &temp);
    return temp.dwCursorPosition;
}
// hide the cursor
void hidecursor(bool hide = true){
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(hConsole, &CursorInfo);
    CursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(hConsole, &CursorInfo);
}
// clear the screen above cursor's location(specialize for menu highlight function) 
void clean(){
    COORD temp = getcursor(), topLeft = {0, 0};
    setcursor(0, 0);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(hConsole, &screen);
    FillConsoleOutputCharacterA(hConsole, ' ', screen.dwSize.X * temp.Y, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, 0, screen.dwSize.X * temp.Y, topLeft, &written);
}
// clear the screen
void clrscr(){
    COORD topLeft = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(hConsole, &screen);
    FillConsoleOutputCharacterA(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, 0, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    setcursor(0, 0);
    hidecursor(1);
}
// pause the screen until any key is pressed
void pause(){
    DWORD        mode;          /* Preserved console mode */
    INPUT_RECORD event;         /* Input event */
    BOOL         done = false;  /* Program termination flag */
    unsigned int counter = 0;   /* The number of times 'Esc' is pressed */
    /* Get the console input handle */
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);

    /* Preserve the original console mode */
    GetConsoleMode(hstdin, &mode );

    /* Set to no line-buffering, no echo, no special-key-processing */
    SetConsoleMode(hstdin, 0 );

    std::cout << "Press Any Key to Continue\n";
    // Flush the buffer to make sure that it won't be affected by the loads of key presses we did while playing the game
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    while (!done)
    {
        if (WaitForSingleObject( hstdin, 0 ) == WAIT_OBJECT_0)  /* if kbhit */
        {
            DWORD count;  /* ignored */
            /* Get the input event */
            ReadConsoleInput( hstdin, &event, 1, &count );
            /* Only respond to key release events */
            if ((event.EventType == KEY_EVENT) &&  !event.Event.KeyEvent.bKeyDown)
                done = true;
        }
    }
    SetConsoleMode(hstdin, mode );
}
//set the console to fullscreen mode
void FullScreen() {ShowWindow(GetConsoleWindow(), SW_SHOWMAXIMIZED);}

//set the console size
//*it won't actually change the windows size, but rather number of rows and columns
void SetConsoleSize(int cols, int lines){
    CONSOLE_FONT_INFO consoleCurrentFont;
    COORD bufferSize, fontSize;
    //Set console buffer size
    GetCurrentConsoleFont(hConsole, false, &consoleCurrentFont);
    fontSize = GetConsoleFontSize(hConsole, consoleCurrentFont.nFont);
    bufferSize.X = cols;
    bufferSize.Y = lines;
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    //Set console's size
    MoveWindow(GetConsoleWindow(),0,0,(cols+4)*fontSize.X,(lines+2)*fontSize.Y, 1);
}

void DisableIME(){
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    HWND hWnd = GetConsoleWindow();
    PostMessage(hWnd, WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)0x04090409);
    //HKL inputContext = GetKeyboardLayout(threadId);
    HKL keyboard = LoadKeyboardLayout("0x0409", KLF_ACTIVATE);
	ActivateKeyboardLayout(keyboard, 0);
}
