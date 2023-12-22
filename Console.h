//set the font size
void SetFont(int size = 26, bool square = 0) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = (square?size:0); //0 for the default ratio
    cfi.dwFontSize.Y = size; //set the size of the font
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
//set location of the cursor
void setcursor(short x = 0, short y = 0){
    COORD temp = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}
void setcursor(const COORD &temp){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
}
 
// get location of the cursor
void getcursor(COORD &other)
{
    CONSOLE_SCREEN_BUFFER_INFO temp;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
    other = temp.dwCursorPosition;
}
COORD getcursor()
{
    CONSOLE_SCREEN_BUFFER_INFO temp;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &temp);
    return temp.dwCursorPosition;
}
// hide the cursor
void hidecursor(bool hide = true)
{
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
    CursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);
}
// clear the screen above cursor's location(specialize for menu highlight function) 
 void clean()
{
    COORD temp = getcursor();
    setcursor(0, 0);
    for (int i = 0; i <= temp.Y; i++)
        std::cout << "                                                                                                                                                                                                                                                        \n";
    setcursor(0, 0);
}
// clear the screen
void clrscr(){
    setcursor(0, 0);
    hidecursor(0);
    for (int i = 0; i <= 110; i++)
        std::cout << "                                                                                                                                                                                                                                        \n";
    setcursor(0, 0);
    hidecursor(1);
}
// pause the screen until any key is pressed
void pause(){
    DWORD        mode;          /* Preserved console mode */
    INPUT_RECORD event;         /* Input event */
    BOOL         done = false;  /* Program termination flag */
    unsigned int counter = 0;   /* The number of times 'Esc' is pressed */

    /* Don't use binary for text files, OK?  ;-) */

    /* Get the console input handle */
    HANDLE hstdin = GetStdHandle( STD_INPUT_HANDLE );

    /* Preserve the original console mode */
    GetConsoleMode( hstdin, &mode );

    /* Set to no line-buffering, no echo, no special-key-processing */
    SetConsoleMode( hstdin, 0 );

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
            if ((event.EventType == KEY_EVENT)
            &&  !event.Event.KeyEvent.bKeyDown)
                done = true;
        }
    }
    SetConsoleMode( hstdin, mode );
}
//set the console to fullscreen mode
void FullScreen() {
  HWND hwnd = GetConsoleWindow();
  ShowWindow(hwnd, SW_SHOWMAXIMIZED);
}
//set the console size
//*it won't actually change the windows size, but rather number of rows and columns
void SetConsoleSize(int x, int y, int cols, int lines)
{
    HANDLE hOut;
    CONSOLE_FONT_INFO consoleCurrentFont;
    COORD bufferSize,fontSize;
    TCHAR title[256];
    HWND hWnd;
    //Set console buffer size
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFont(hOut, false, &consoleCurrentFont);
    fontSize = GetConsoleFontSize(hOut,consoleCurrentFont.nFont);
    bufferSize.X = cols;
    bufferSize.Y = lines;
    SetConsoleScreenBufferSize(hOut, bufferSize);
    //Set console's size
    hWnd = GetConsoleWindow();
    MoveWindow(hWnd,0,0,(cols+4)*fontSize.X,(lines+2)*fontSize.Y,true);
}

