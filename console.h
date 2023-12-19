//set location of the cursor
#ifdef FONT
void SetFont(int size = 30) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = size;
    cfi.dwFontSize.Y = size;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
#endif // FONT

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


void FullScreen() {
    DWORD last_style;
    RECT last_rect;
    HWND hwnd = GetConsoleWindow();
    last_style = GetWindowLong(hwnd, GWL_STYLE); //存?上次的窗口?格
    GetWindowRect(hwnd, &last_rect);             //存?上次的窗口位置和大小
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);       // ?取最大化的窗口大小
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP); // 去掉???
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED); // ?置位置和大小
//    HIMC g_hIMC = NULL; 
//    HIMC himc = ImmGetContext(hwnd);                       // g_hIMC 用于恢复?使用
//    g_hIMC = ImmAssociateContext(hwnd, nullptr);
//    SetConsoleCtrlHandler(NULL, TRUE);
//    LoadKeyboardLayout("0x0409", KLF_ACTIVATE | KLF_SETFORPROCESS);
    //ImmReleaseContext(hwnd, himc); 
    //HKL hKL = GetKeyboardLayout(0);               //保存?有的?入法  
	//ActivateKeyboardLayout(hKL, KLF_RESET);
}

