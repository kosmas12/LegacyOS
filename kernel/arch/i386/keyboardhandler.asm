global keyboardHandler
extern keyboardHandlerMain

keyboardHandler:
    pushad
    call keyboardHandlerMain
    popad
    iret
